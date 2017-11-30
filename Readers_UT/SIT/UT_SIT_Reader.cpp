//===========================================================================
//                           UT_SIT_Reader.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SIT_Reader.cpp
//!
//! Implements test fixture for testing SIT_Reader
//!
//===========================================================================


#include "SIT_Reader.hpp"
#include "ParserException.hpp"
#include "PrettyPrinter.hpp"
#include "SystemModelBuilder.hpp"
#include "Utility.hpp"
#include "LoopbackAccessInterfaceProtocol.hpp"
#include "g3log/g3log.hpp"
#include "UT_SIT_Reader.hpp"

#include "Mast_Core_Traits.hpp"

#include <cxxtest/ValueTraits.h>
#include <experimental/string_view>
#include <iostream>
#include <tuple>
#include <vector>
#include <memory>

using std::tuple;
using std::make_tuple;
using std::string;
using std::stringstream;
using std::experimental::string_view;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;

using namespace std::chrono_literals;
using namespace std::experimental::literals::string_view_literals;
using namespace mast;

namespace
{

//! Add a JTAG Tap to system mode, then append node to it
//!
void PrependWithTap (shared_ptr<SystemModel> sm, shared_ptr<SystemModelNode> node)
{
  auto protocol = make_shared<LoopbackAccessInterfaceProtocol> ();
  SystemModelBuilder builder(*sm);

  auto tap = builder.Create_JTAG_TAP("TAP", 8u, 2u, protocol);

  sm->ReplaceRoot(tap, false);
  tap->AppendChild(node);
}
//
//  End of: PrependWithTap
//---------------------------------------------------------------------------
} // End of unnamed namespace


//! Initializes tests (called for each test)
//!
void UT_SIT_Reader::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier(); // Needed to check with pretty print that include node identifiers
}

//! Cleanups test (called for each test)
void UT_SIT_Reader::tearDown ()
{
}



// Test construction of register nodes from Simplified ICL Tree input when the input is correct
//
void UT_SIT_Reader::test_register_Success ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    stringstream    sit(std::get<0>(data));
    auto            expected_PrettyPrint = std::get<1> (data);
    auto            sm                   = make_shared<SystemModel>();
    SIT::SIT_Reader sut(sm);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.ParseExcerpt(sit));

    // ---------------- Verify
    //
    TS_ASSERT_EMPTY (sut.ErrorMessage());
    TS_ASSERT_EMPTY (sut.PlaceHolders());

    auto parsedModel = sut.ParsedSystemModel();

    // With PrettyPrinter
    auto actual_PrettyPrint = PrettyPrinter::PrettyPrint(parsedModel, PrettyPrinterOptions::Parser_debug);
    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, parsedModel);   // This is to avoid warnings about missing AccessInterface
    auto checkResult = sm->Check();
    TS_ASSERT_EMPTY (checkResult.InformativeReport());
  };

  auto data =
  {
    /*Basic constructor, different sizes and bypass values*/
    make_tuple( "REGISTER test_register 12 Bypass: \"0b1001:0110:1100\"\n",  "[Register](0)  \"test_register\", length: 12, bypass: 1001_0110:1100"),
    make_tuple( "REGISTER test_register 11 Bypass: \"0b001:0110:1100\"\n",   "[Register](0)  \"test_register\", length: 11, bypass: 0010_1101:100"),

    /*Hold value*/
    make_tuple( "REGISTER test_register 12 Hold_value Bypass: \"0b1001:0110:1100\"\n",  "[Register](0)  \"test_register\", length: 12, Hold value: true, bypass: 1001_0110:1100"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


// Test construction of register nodes from Simplified ICL Tree input when the input is NOT correct
//
void UT_SIT_Reader::test_register_Error ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    stringstream    sit(std::get<0>(data));
    auto            expected_errMSG = std::get<1>(data);
    auto            sm              = make_shared<SystemModel>();
    SIT::SIT_Reader sut(sm);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS (sut.ParseExcerpt(sit), mast::ParserException);

    // ---------------- Verify
    //
    const auto gotErrorMessage = sut.ErrorMessage();
    TS_ASSERT_EQUALS  (gotErrorMessage, expected_errMSG);

    auto systemModelNode = sut.ParsedSystemModel();
    TS_ASSERT_NULLPTR(systemModelNode);
  };

  auto data =  // Sit input, expected PrettyPrint, expected error message, checker error count
  {
    // 00: Wrong Bypass length
    make_tuple("REGISTER test_register 8 Bypass: \"0b1001:011\"\n",
               "SIT Parsing error: Line 1:34-46: REGISTER node \"test_register\" size (8) does not match Bypass value bit count (7)"),

    // 01: Missing register name
    make_tuple("REGISTER test_register 5 Bypass: \"0b001:0110:1100\"\n",
               "SIT Parsing error: Line 1:34-51: REGISTER node \"test_register\" size (5) does not match Bypass value bit count (11)"),

    // 02: Missing register length
    make_tuple("REGISTER test_register Bypass: \"0b001\"\n",
               "SIT Parsing error: Line 1:24-30: syntax error"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}



// Test construction of chain nodes from Simplified ICL Tree input
void UT_SIT_Reader::test_chain ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    stringstream    sit(std::get<0>(data));
    auto            expected_PrettyPrint = std::get<1> (data);
    auto            sm                   = make_shared<SystemModel>();
    SIT::SIT_Reader sut(sm);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.ParseExcerpt(sit));

    // ---------------- Verify
    //
    auto parsedModel = sut.ParsedSystemModel();

    TS_ASSERT_EMPTY (sut.PlaceHolders());

    // With PrettyPrinter
    auto actual_PrettyPrint = PrettyPrinter::PrettyPrint(parsedModel, PrettyPrinterOptions::Parser_debug);
    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, parsedModel);   // This is to avoid warnings about missing AccessInterface
    auto checkResult = sm->Check();
    TS_ASSERT_EMPTY (checkResult.InformativeReport());
  };

  auto data =
  { /*Chain with one register*/
    make_tuple("CHAIN test_chain\n { REGISTER test_register 12 Bypass: \"0b1001:0110:1100\"\n}",
               "[Chain](0)     \"test_chain\"\n [Register](1)  \"test_register\", length: 12, bypass: 1001_0110:1100"),
    /*Chain with two registers*/
    make_tuple(    "CHAIN test_chain\
    { REGISTER test_register_1 12 Bypass: \"0b1001:0110:1100\"\
      REGISTER test_register_2 12 Bypass: \"0b1001:0110:1100\"\
    }",
"[Chain](0)     \"test_chain\"\n\
 [Register](1)  \"test_register_1\", length: 12, bypass: 1001_0110:1100\n\
 [Register](2)  \"test_register_2\", length: 12, bypass: 1001_0110:1100"
),
    /*Nested Chains*/
    make_tuple(    " CHAIN test_chain_1\
  { CHAIN test_chain_2\
   { CHAIN test_chain_3\
    { CHAIN test_chain_4\
     { CHAIN test_chain_5\
      { REGISTER test_register_1 12 Bypass: \"0b1001:0110:1100\"\
      REGISTER test_register_2 12 Bypass: \"0b1001:0110:1100\"\
     }}}}}",
"[Chain](0)     \"test_chain_1\"\n\
 [Chain](1)     \"test_chain_2\"\n\
  [Chain](2)     \"test_chain_3\"\n\
   [Chain](3)     \"test_chain_4\"\n\
    [Chain](4)     \"test_chain_5\"\n\
     [Register](5)  \"test_register_1\", length: 12, bypass: 1001_0110:1100\n\
     [Register](6)  \"test_register_2\", length: 12, bypass: 1001_0110:1100"),

    /*Chain with two registers, name not in path*/
    make_tuple(    "CHAIN test_chain NOT_IN_PATH\
    { REGISTER test_register_1 12 Bypass: \"0b1001:0110:1100\"\
      REGISTER test_register_2 12 Bypass: \"0b1001:0110:1100\"\
    }",
"[Chain](0)     \"test_chain\", ignore_in_path: true\n\
 [Register](1)  \"test_register_1\", length: 12, bypass: 1001_0110:1100\n\
 [Register](2)  \"test_register_2\", length: 12, bypass: 1001_0110:1100"
),

  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}

/*Test construction of MIB macro from Simplified ICL Tree input*/
void UT_SIT_Reader::test_MIB ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    stringstream    sit(std::get<0>(data));
    auto            expected_PrettyPrint = std::get<1> (data);
    auto            sm                   = make_shared<SystemModel>();
    SIT::SIT_Reader sut(sm);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.ParseExcerpt(sit));

    // ---------------- Verify
    //
    auto parsedModel = sut.ParsedSystemModel();

    TS_ASSERT_EMPTY (sut.PlaceHolders());

    // With PrettyPrinter
    auto actual_PrettyPrint = PrettyPrinter::PrettyPrint(parsedModel, PrettyPrinterOptions::Parser_debug);
    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, parsedModel);   // This is to avoid warnings about missing AccessInterface
    auto checkResult = sm->Check();
    TS_ASSERT_EQUALS (checkResult.warningsCount, 1u); // 1 for "Linker 'test_xxx' (id: x) has only 2 children, even though it can select 4 paths"
    TS_ASSERT_EQUALS (checkResult.errorsCount,   0u);
    TS_ASSERT_EQUALS (checkResult.infosCount,    0u);

    if (checkResult.errorsCount != 0)
    {
      TS_ASSERT_EMPTY (checkResult.MakeReport());
    }
  };

  auto data =
  {
    // 00
    make_tuple("MIB test_MIB POST HIGH 4 Binary \n"
               "(\n"
               " REGISTER test_reg_1 4 Bypass: \"0b1001\"\n"
               " REGISTER test_reg_2 4 Bypass: \"0b1100\"\n"
               ")\n",
               "[Chain](1)     \"test_MIB\"\n"
               " [Linker](2)    \"test_MIB_mux\"\n"
               "  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: true, inverted_bits: false, reversed_order: false\n"
               "  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n"
               "  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n"
               " [Register](0)  \"test_MIB_ctrl\", length: 3, Hold value: true, bypass: 000"),

    // 01
    make_tuple("MIB test_MIB POST HIGH 4 Binary_noidle \n"
               "(\n"
               "  REGISTER test_reg_1 4 Bypass: \"0b1001\"\n"
               "  REGISTER test_reg_2 4 Bypass: \"0b1100\"\n"
               ")\n",
               "[Chain](1)     \"test_MIB\"\n"
               " [Linker](2)    \"test_MIB_mux\"\n"
               "  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: false, inverted_bits: false, reversed_order: false\n"
               "  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n"
               "  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n"
               " [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),

    // 02
    make_tuple( "MIB test_MIB POST LOW 4 Binary \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: true, inverted_bits: true, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 3, Hold value: true, bypass: 000"),

    // 03
    make_tuple( "MIB test_MIB PRE LOW 4 Binary \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 3, Hold value: true, bypass: 000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: true, inverted_bits: true, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),

    // 04
    make_tuple( "MIB test_MIB PRE HIGH 4 Binary \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 3, Hold value: true, bypass: 000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: true, inverted_bits: false, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),

    // 05
    make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 Binary \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 3, Hold value: true, bypass: 000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: true, inverted_bits: false, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),

    // 06
    make_tuple( "MIB test_MIB PRE LOW REVERSE 4 Binary \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 3, Hold value: true, bypass: 000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: true, inverted_bits: true, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),

    // 07
    make_tuple( "MIB test_MIB POST LOW REVERSE 4 Binary \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: true, inverted_bits: true, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 3, Hold value: true, bypass: 000"),

    // 08
    make_tuple( "MIB test_MIB POST HIGH REVERSE 4 Binary \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: true, inverted_bits: false, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 3, Hold value: true, bypass: 000"),

make_tuple( "MIB test_MIB POST HIGH 4 N_Hot \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: true, inverted_bits: false, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000"),
make_tuple( "MIB test_MIB POST LOW 4 N_Hot \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: true, inverted_bits: true, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111"),
make_tuple( "MIB test_MIB PRE LOW 4 N_Hot \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: true, inverted_bits: true, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH 4 N_Hot \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: true, inverted_bits: false, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 N_Hot \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: true, inverted_bits: false, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE LOW REVERSE 4 N_Hot \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: true, inverted_bits: true, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB POST LOW REVERSE 4 N_Hot \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: true, inverted_bits: true, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111"),
make_tuple( "MIB test_MIB POST HIGH REVERSE 4 N_Hot \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: true, inverted_bits: false, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000"),
make_tuple( "MIB test_MIB POST HIGH 4 One_Hot \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: true, inverted_bits: false, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000"),
make_tuple( "MIB test_MIB POST LOW 4 One_Hot \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: true, inverted_bits: true, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111"),
make_tuple( "MIB test_MIB PRE LOW 4 One_Hot \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: true, inverted_bits: true, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH 4 One_Hot \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: true, inverted_bits: false, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 One_Hot \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: true, inverted_bits: false, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE LOW REVERSE 4 One_Hot \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: true, inverted_bits: true, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB POST LOW REVERSE 4 One_Hot \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: true, inverted_bits: true, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111"),
make_tuple( "MIB test_MIB POST HIGH REVERSE 4 One_Hot \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: true, inverted_bits: false, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000"),
make_tuple( "MIB test_MIB POST HIGH 4 Binary_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: false, inverted_bits: false, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB POST LOW 4 Binary_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: false, inverted_bits: true, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB PRE LOW 4 Binary_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: false, inverted_bits: true, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH 4 Binary_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: false, inverted_bits: false, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 Binary_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: false, inverted_bits: false, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE LOW REVERSE 4 Binary_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: false, inverted_bits: true, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB POST LOW REVERSE 4 Binary_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: false, inverted_bits: true, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB POST HIGH REVERSE 4 Binary_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: false, inverted_bits: false, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB POST HIGH 4 One_Hot_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: false, inverted_bits: false, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1000"),
make_tuple( "MIB test_MIB POST LOW 4 One_Hot_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: false, inverted_bits: true, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0111"),
make_tuple( "MIB test_MIB PRE LOW 4 One_Hot_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0111\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: false, inverted_bits: true, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH 4 One_Hot_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: false, inverted_bits: false, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 One_Hot_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0001\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: false, inverted_bits: false, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE LOW REVERSE 4 One_Hot_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1110\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: false, inverted_bits: true, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB POST LOW REVERSE 4 One_Hot_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: false, inverted_bits: true, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1110"),
make_tuple( "MIB test_MIB POST HIGH REVERSE 4 One_Hot_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: false, inverted_bits: false, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0001"),
make_tuple( "MIB test_MIB POST HIGH 4 N_Hot_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: false, inverted_bits: false, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1000"),
make_tuple( "MIB test_MIB POST LOW 4 N_Hot_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: false, inverted_bits: true, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0111"),
make_tuple( "MIB test_MIB PRE LOW 4 N_Hot_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0111\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: false, inverted_bits: true, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH 4 N_Hot_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: false, inverted_bits: false, reversed_order: false\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 N_Hot_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0001\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: false, inverted_bits: false, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE LOW REVERSE 4 N_Hot_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1110\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: false, inverted_bits: true, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB POST LOW REVERSE 4 N_Hot_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: false, inverted_bits: true, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1110"),
make_tuple( "MIB test_MIB POST HIGH REVERSE 4 N_Hot_noidle \
    (REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    )",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: false, inverted_bits: false, reversed_order: true\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0001"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}



/*Test SIB macro from Simplified ICL Tree input*/
void UT_SIT_Reader::test_SIB ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    stringstream    sit(std::get<0>(data));
    auto            expected_PrettyPrint = std::get<1> (data);
    auto            sm                   = make_shared<SystemModel>();
    SIT::SIT_Reader sut(sm);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.ParseExcerpt(sit));

    // ---------------- Verify
    //
    auto parsedModel = sut.ParsedSystemModel();

    TS_ASSERT_EMPTY (sut.PlaceHolders());

    // With PrettyPrinter
    auto actual_PrettyPrint = PrettyPrinter::PrettyPrint(parsedModel, PrettyPrinterOptions::Parser_debug);
    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, parsedModel);   // This is to avoid warnings about missing AccessInterface
    auto checkResult = sm->Check();
    TS_ASSERT_EMPTY (checkResult.InformativeReport());
  };

  auto data = // Exhaustive test of all possible macro parameter combinations
  {
    // 0
    make_tuple("SIB test_SIB POST HIGH (REGISTER test_reg 4 Bypass: \"0b1001\")\n",
               // Expected:
               "[Chain](1)     \"test_SIB\"\n"
               " [Linker](2)    \"test_SIB_mux\"\n"
               "  :Selector:(0)  \"test_SIB_ctrl\", kind: Binary, can_select_none: true, inverted_bits: false, reversed_order: false\n"
               "  [Register](3)  \"test_reg\", length: 4, bypass: 1001\n"
               " [Register](0)  \"test_SIB_ctrl\", length: 1, Hold value: true, bypass: 0"),

    // 1
    make_tuple("SIB test_SIB POST LOW (REGISTER test_reg 4 Bypass: \"0b1001\")\n",
               // Expected:
               "[Chain](1)     \"test_SIB\"\n"
               " [Linker](2)    \"test_SIB_mux\"\n"
               "  :Selector:(0)  \"test_SIB_ctrl\", kind: Binary, can_select_none: true, inverted_bits: true, reversed_order: false\n"
               "  [Register](3)  \"test_reg\", length: 4, bypass: 1001\n"
               " [Register](0)  \"test_SIB_ctrl\", length: 1, Hold value: true, bypass: 0"),

    // 2
    make_tuple("SIB test_SIB PRE HIGH (REGISTER test_reg 4 Bypass: \"0b1001\")\n",
               // Expected:
               "[Chain](1)     \"test_SIB\"\n"
               " [Register](0)  \"test_SIB_ctrl\", length: 1, Hold value: true, bypass: 0\n"
               " [Linker](2)    \"test_SIB_mux\"\n"
               "  :Selector:(0)  \"test_SIB_ctrl\", kind: Binary, can_select_none: true, inverted_bits: false, reversed_order: false\n"
               "  [Register](3)  \"test_reg\", length: 4, bypass: 1001"),
    // 3
    make_tuple("SIB test_SIB PRE LOW (REGISTER test_reg 4 Bypass: \"0b1001\")\n",
               // Expected:
               "[Chain](1)     \"test_SIB\"\n"
               " [Register](0)  \"test_SIB_ctrl\", length: 1, Hold value: true, bypass: 0\n"
               " [Linker](2)    \"test_SIB_mux\"\n"
               "  :Selector:(0)  \"test_SIB_ctrl\", kind: Binary, can_select_none: true, inverted_bits: true, reversed_order: false\n"
               "  [Register](3)  \"test_reg\", length: 4, bypass: 1001"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}

//! Test JTAG macro from Simplified ICL Tree input - In cases with success
//!
void UT_SIT_Reader::test_JTAG_TAP_Success ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    stringstream    sit(std::get<0>(data));
    auto            expected_PrettyPrint = std::get<1> (data);
    auto            sm                   = make_shared<SystemModel>();
    SIT::SIT_Reader sut(sm);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.ParseExcerpt(sit));

    // ---------------- Verify
    //
    TS_ASSERT_EMPTY (sut.ErrorMessage());
    TS_ASSERT_EMPTY (sut.PlaceHolders());

    auto parsedModel = sut.ParsedSystemModel();

    // With PrettyPrinter
    auto actual_PrettyPrint = PrettyPrinter::PrettyPrint(parsedModel, PrettyPrinterOptions::Parser_debug);
    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, parsedModel);   // This is to avoid warnings about missing AccessInterface
    auto checkResult = sm->Check();
    TS_ASSERT_EMPTY (checkResult.InformativeReport());
  };


  auto data =
  {
    // 00: Loopback
    make_tuple("JTAG_TAP my_tap Loopback 4 1\n"
               "("
               "  REGISTER test_reg 4 Bypass: \"0b1100\"\n"
               ")\n",
               "[Access_I](0)  \"my_tap\", Protocol: Loopback\n"
               " [Register](1)  \"my_tap_IR\", length: 4, Hold value: true, bypass: 1111\n"
               " [Linker](2)    \"my_tap_DR_Mux\"\n"
               "  :Selector:(1)  \"my_tap_IR\", kind: Table_Based, can_select_none: false, inverted_bits: false, reversed_order: false\n"
               "  [Register](3)  \"my_tap_BPY\", length: 1, bypass: 1\n"
               "  [Register](4)  \"test_reg\", length: 4, bypass: 1100"),

    // 01: SVF Simulation
    make_tuple("JTAG_TAP my_tap SVF_Simulation 4 1\n"
               "(\n"
               "  REGISTER test_reg 4 Bypass: \"0b1100\"\n"
               ")\n",
               "[Access_I](0)  \"my_tap\", Protocol: SVF_Simulation\n"
               " [Register](1)  \"my_tap_IR\", length: 4, Hold value: true, bypass: 1111\n"
               " [Linker](2)    \"my_tap_DR_Mux\"\n"
               "  :Selector:(1)  \"my_tap_IR\", kind: Table_Based, can_select_none: false, inverted_bits: false, reversed_order: false\n"
               "  [Register](3)  \"my_tap_BPY\", length: 1, bypass: 1\n"
               "  [Register](4)  \"test_reg\", length: 4, bypass: 1100"),

    // 02: SVF Simulation
    make_tuple("JTAG_TAP my_tap SVF_Simulation 4 [ \"0xF\" , \"0x2\" ] 1"    // Ignored array
               "("
               " REGISTER test_reg 4 Bypass: \"0b1100\""
               ")\n",
               "[Access_I](0)  \"my_tap\", Protocol: SVF_Simulation\n"
               " [Register](1)  \"my_tap_IR\", length: 4, Hold value: true, bypass: 1111\n"
               " [Linker](2)    \"my_tap_DR_Mux\"\n"
               "  :Selector:(1)  \"my_tap_IR\", kind: Table_Based, can_select_none: false, inverted_bits: false, reversed_order: false\n"
               "  [Register](3)  \"my_tap_BPY\", length: 1, bypass: 1\n"
               "  [Register](4)  \"test_reg\", length: 4, bypass: 1100"),

    // 03: SVF Emulation
    make_tuple("JTAG_TAP my_tap SVF_Emulation 4 1"
               "("
               " REGISTER test_reg 4 Bypass: \"0b1100\""
               ")\n",
               "[Access_I](0)  \"my_tap\", Protocol: SVF_Emulation\n"
               " [Register](1)  \"my_tap_IR\", length: 4, Hold value: true, bypass: 1111\n"
               " [Linker](2)    \"my_tap_DR_Mux\"\n"
               "  :Selector:(1)  \"my_tap_IR\", kind: Table_Based, can_select_none: false, inverted_bits: false, reversed_order: false\n"
               "  [Register](3)  \"my_tap_BPY\", length: 1, bypass: 1\n"
               "  [Register](4)  \"test_reg\", length: 4, bypass: 1100"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Test JTAG macro from Simplified ICL Tree input - In cases with failure
//!
void UT_SIT_Reader::test_JTAG_TAP_Failure ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    stringstream    sit(std::get<0>(data));
    auto            expected_ErrorMsg = std::get<1>(data);
    auto            sm                = make_shared<SystemModel>();
    SIT::SIT_Reader sut(sm);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut.ParseExcerpt(sit), mast::ParserException);

    // ---------------- Verify
    //
    const auto gotErrorMessage = sut.ErrorMessage();

    TS_ASSERT_CONTAINS (gotErrorMessage, expected_ErrorMsg);
  };

  auto data =
  {
    // 00: SVF OpenOCD
    make_tuple("JTAG_TAP my_tap SVF_openOCD 4 1"
               "("
               "  REGISTER test_reg 4 Bypass: \"0b1100\""
               ")\n",
               "Line 1:31-32: JTAG_TAP node \"my_tap\" Cannot create protocol: \"SVF_openOCD\"; std::invalid_argument: There is no creation method registered with name: SVF_openOCD."
               ),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}

//! Test ACCES_INTERFACE from Simplified ICL Tree input - In cases with success
//!
void UT_SIT_Reader::test_ACCES_INTERFACE_Success ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    stringstream    sit(std::get<0>(data));
    auto            expected_PrettyPrint = std::get<1> (data);
    auto            sm                   = make_shared<SystemModel>();
    SIT::SIT_Reader sut(sm);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.ParseExcerpt(sit));

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);

    TS_ASSERT_EMPTY (sut.ErrorMessage());
    TS_ASSERT_EMPTY (sut.PlaceHolders());

    auto parsedModel = sut.ParsedSystemModel();

    // With PrettyPrinter
    auto actual_PrettyPrint = PrettyPrinter::PrettyPrint(parsedModel, PrettyPrinterOptions::Parser_debug);
    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, parsedModel);   // This is to avoid warnings about missing AccessInterface
    auto checkResult = sm->Check();
    TS_ASSERT_EMPTY (checkResult.InformativeReport());
  };


  auto data =
  {
    // 00: Loopback
    make_tuple("ACCESS_INTERFACE my_tap Loopback\n"
               "(\n"
               "  REGISTER test_reg 4 Bypass: \"0b1100\"\n"
               ")\n",
               "[Access_I](0)  \"my_tap\", Protocol: Loopback\n"
               " [Register](1)  \"test_reg\", length: 4, bypass: 1100"),

    // 01: SVF Simulation
    make_tuple("ACCESS_INTERFACE my_tap SVF_Simulation\n"
               "(\n"
               "  REGISTER reg_1 3 Bypass: \"0b101\"\n"
               "  REGISTER reg_2 5 Bypass: \"0b11001\"\n"
               ")\n",
               "[Access_I](0)  \"my_tap\", Protocol: SVF_Simulation\n"
               " [Register](1)  \"reg_1\", length: 3, bypass: 101\n"
               " [Register](2)  \"reg_2\", length: 5, bypass: 1100_1"),

    // 02: SVF Emulation
    make_tuple("ACCESS_INTERFACE my_tap  SVF_Emulation \n"
               "(\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               ")\n",
               "[Access_I](0)  \"my_tap\", Protocol: SVF_Emulation\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),

    // 03: Offline
    make_tuple("ACCESS_INTERFACE my_tap  Offline  \n"
               "(\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               ")\n",
               "[Access_I](0)  \"my_tap\", Protocol: Offline\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),

    // 04: STIL_Emulation
    make_tuple("ACCESS_INTERFACE my_tap  STIL_Emulation \"3\" \n"
               "(\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               ")\n",
               "[Access_I](0)  \"my_tap\", Protocol: STIL_Emulation\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),

    // 05: I2C_Emulation
    make_tuple("ACCESS_INTERFACE my_tap  I2C_Emulation \"0x40, 0x41, 0x42, [I2C]\" \n"
               "(\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               ")\n",
               "[Access_I](0)  \"my_tap\", Protocol: I2C_Emulation\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),

    // 06: Any letter in parameters
    make_tuple("ACCESS_INTERFACE letters I2C_Emulation \"0x40, 0x41, 0x42, abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ\" \n"
               "(\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               ")\n",
               "[Access_I](0)  \"letters\", Protocol: I2C_Emulation\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),

    // 07: Any digits in parameters
    make_tuple("ACCESS_INTERFACE digits I2C_Emulation \"0x40, 0x41, 0x42, _0123456789 \" \n"
               "(\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               ")\n",
               "[Access_I](0)  \"digits\", Protocol: I2C_Emulation\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),

    // 08: Any brackets characters in parameters
    make_tuple("ACCESS_INTERFACE brackets  I2C_Emulation \"0x40, 0x41, 0x42, ([<()>])\" \n"
               "(\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               ")\n",
               "[Access_I](0)  \"brackets\", Protocol: I2C_Emulation\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),

    // 09: Any punctuation characters in parameters
    make_tuple("ACCESS_INTERFACE punctuation  I2C_Emulation \"0x40, 0x41, 0x42, ?!.:; \" \n"
               "(\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               ")\n",
               "[Access_I](0)  \"punctuation\", Protocol: I2C_Emulation\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),

    // 10: Any operators characters in parameters
    make_tuple("ACCESS_INTERFACE operators  I2C_Emulation \"0x40, 0x41, 0x42, /*-+~%\" \n"
               "(\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               ")\n",
               "[Access_I](0)  \"operators\", Protocol: I2C_Emulation\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),

    // 11: Any special characters in parameters
    make_tuple("ACCESS_INTERFACE special  I2C_Emulation \"0x40, 0x41, 0x42, $=@'_|& \" \n"
               "(\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               ")\n",
               "[Access_I](0)  \"special\", Protocol: I2C_Emulation\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),


    #ifdef INTEL_EXPERIMENT
    // 12: Intel_Packet
    make_tuple("ACCESS_INTERFACE tap  Intel_Packet \"0x41, 0x42\" \n"
               "(\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               ")\n",
               "[Access_I](0)  \"tap\", Protocol: Intel_Packet\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),
    #endif
  };

  #ifndef INTEL_EXPERIMENT
//+  TS_WARN ("No tests for Intel_Packet protocol");
  #endif

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Test ACCES_INTERFACE from Simplified ICL Tree input - In cases with parsing failure
//!
void UT_SIT_Reader::test_ACCES_INTERFACE_Failure ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    stringstream    sit(std::get<0>(data));
    auto            expected_ErrorMsg = std::get<1>(data);
    auto            sm                = make_shared<SystemModel>();
    SIT::SIT_Reader sut(sm);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut.ParseExcerpt(sit), mast::ParserException);

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);

    const auto gotErrorMessage = sut.ErrorMessage();

    TS_ASSERT_CONTAINS (gotErrorMessage, expected_ErrorMsg);
  };


  auto data =
  {
    // 00: Empty integer array
    make_tuple("ACCESS_INTERFACE Useless_Square_Brackets Loopback []\n"
               "(\n"
               "  REGISTER test_reg 4 Bypass: \"0b1100\"\n"
               ")\n",
               "Line 1:51-52: syntax error"),

    // 01: Unregistered protocol type
    make_tuple("ACCESS_INTERFACE Unregistered_Protocol MyProtocol\n"
               "(\n"
               "  REGISTER reg_1 3 Bypass: \"0b101\"\n"
               "  REGISTER reg_2 5 Bypass: \"0b11001\"\n"
               ")\n",
               "Line 2:1-2: ACCESS_INTERFACE node \"Unregistered_Protocol\" Cannot create protocol: \"MyProtocol\"; std::invalid_argument: There is no creation method registered with name: MyProtocol."),

    // 02: No AccessInterface name (same as no protocol)
    make_tuple("ACCESS_INTERFACE Offline  \n"
               "(\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               ")\n",
               "Line 2:1-2: syntax error"),

    // 03: Not enough addresses
    make_tuple("ACCESS_INTERFACE Not_enough_Adresses I2C_Emulation \"0x40, (i2c)\"  \n"
               "(\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               ")\n",
               "Line 1:52-65: ACCESS_INTERFACE node \"Not_enough_Adresses\" Cannot create protocol: \"I2C_Emulation\"; std::invalid_argument: I2C Addresses must have at least two entries."),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Test 1500 Wrapper macro from Simplified ICL Tree input
//!
void UT_SIT_Reader::test_1500 ()
{
  // ---------------- Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    stringstream    sit(std::get<0>(data));
    auto            expected_PrettyPrint = std::get<1> (data);
    auto            sm                   = make_shared<SystemModel>();
    SIT::SIT_Reader sut(sm);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.ParseExcerpt(sit));

    // ---------------- Verify
    //
    TS_ASSERT_EMPTY (sut.PlaceHolders());

    auto parsedModel = sut.ParsedSystemModel();

    // With PrettyPrinter
    auto actual_PrettyPrint = PrettyPrinter::PrettyPrint(parsedModel, PrettyPrinterOptions::Parser_debug);
    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, parsedModel);   // This is to avoid warnings about missing AccessInterface
    auto checkResult = sm->Check();
    TS_ASSERT_EMPTY (checkResult.InformativeReport());
  };

  auto data =
  {
   make_tuple( "WRAPPER_1500 test_wrapper 2\
 ( REGISTER test_register_1 12 Bypass: \"0b1001:0110:1100\"\
   REGISTER test_register_2 12 Bypass: \"0b1001:0110:1100\")\n",
"[Chain](0)     \"test_wrapper\"\n\
 [Chain](2)     \"SWIR\"\n\
  [Register](1)  \"SWIR_ctrl\", length: 1, Hold value: true, bypass: 0\n\
  [Linker](3)    \"SWIR_mux\"\n\
   :Selector:(1)  \"SWIR_ctrl\", kind: Binary, can_select_none: false, inverted_bits: false, reversed_order: false\n\
   [Chain](5)     \"WIR\"\n\
    [Linker](6)    \"WIR_mux\"\n\
     :Selector:(4)  \"WIR_reg\", kind: Binary, can_select_none: false, inverted_bits: false, reversed_order: false\n\
     [Register](7)  \"WBY\", length: 1, bypass: 0\n\
     [Register](8)  \"test_register_1\", length: 12, bypass: 1001_0110:1100\n\
     [Register](9)  \"test_register_2\", length: 12, bypass: 1001_0110:1100\n\
   [Register](4)  \"WIR_reg\", length: 2, Hold value: true, bypass: 00"),


   };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


// Test construction of LINKERs macro from Simplified ICL Tree input - in case of success
//
void UT_SIT_Reader::test_LINKER_Success ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    stringstream    sit(std::get<0>(data));
    auto            expected_PrettyPrint = std::get<1> (data);
    auto            sm                   = make_shared<SystemModel>();
    SIT::SIT_Reader sut(sm);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.ParseExcerpt(sit));

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);

    TS_ASSERT_EMPTY (sut.ErrorMessage());
    TS_ASSERT_EMPTY (sut.PlaceHolders());

    auto parsedModel = sut.ParsedSystemModel();

    // With PrettyPrinter
    auto actual_PrettyPrint = PrettyPrinter::PrettyPrint(parsedModel, PrettyPrinterOptions::Parser_debug);
    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, parsedModel);   // This is to avoid warnings about missing AccessInterface
    auto checkResult = sm->Check();
    TS_ASSERT_EMPTY (checkResult.InformativeReport());

  };

  auto data =
  {
    // 00 ==> correct syntax
    make_tuple("LINKER test_LINKER One_Hot test_reg_1 4\n"
               "(\n"
               "  REGISTER test_reg_1 4 Bypass: \"0b1001\"\n"
               "  REGISTER test_reg_2 4 Bypass: \"0b1100\"\n"
               "  REGISTER test_reg_3 2 Bypass: \"0b10\"\n"
               "  REGISTER test_reg_4 5 Bypass: \"0b11001\"\n"
               ")"s,
               "[Linker](0)    \"test_LINKER\"\n"
               " :Selector:(1)  \"test_reg_1\", kind: One_Hot, can_select_none: true, inverted_bits: false, reversed_order: false\n"
               " [Register](1)  \"test_reg_1\", length: 4, bypass: 1001\n"
               " [Register](2)  \"test_reg_2\", length: 4, bypass: 1100\n"
               " [Register](3)  \"test_reg_3\", length: 2, bypass: 10\n"
               " [Register](4)  \"test_reg_4\", length: 5, bypass: 1100_1"s),
    };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}



// Test construction of LINKERs macro from Simplified ICL Tree input - in case of errors
//
void UT_SIT_Reader::test_LINKER_Error ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    stringstream    sit(std::get<0>(data));
    auto            expected_errMSG = std::get<1>(data);
    auto            sm              = make_shared<SystemModel>();
    SIT::SIT_Reader sut(sm);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut.ParseExcerpt(sit), mast::ParserException);

    // ---------------- Verify
    //
    const auto gotErrorMessage = sut.ErrorMessage();
    TS_ASSERT_EQUALS  (gotErrorMessage, expected_errMSG);

    auto systemModelNode = sut.ParsedSystemModel();
    TS_ASSERT_NULLPTR(systemModelNode);
  };

  auto data =
  {
    // 00 ==> Error: selector register not defined
    make_tuple("LINKER test_LINKER Binary 4\n"
               "{REGISTER test_reg_1 4 Bypass: \"0b1001\"\n"
               "REGISTER test_reg_2 4 Bypass: \"0b1100\"\n"
               ")"s,
               "SIT Parsing error: Line 2:1-2: LINKER node \"test_LINKER\" Must specify a control node (Register) for its path selector"),

    // 01 ==> Error: selector register does not exist
    make_tuple("LINKER test_LINKER Binary selector_reg 4\n"
               "(\n"
               "  REGISTER test_reg_1 4 Bypass: \"0b1001\"\n"
               "  REGISTER test_reg_2 4 Bypass: \"0b1100\"\n"
               ")"s,
               "SIT Parsing error: Line 2:1-2: LINKER node \"test_LINKER\" Error, specified selector register \"selector_reg\" does not exist"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}

// Test construction of LINKERs macro with selector defined from custom table
//
void UT_SIT_Reader::test_LINKER_CustomTable_Success ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    stringstream    sit(std::get<0>(data));
    auto            expected_PrettyPrint = std::get<1> (data);
    auto            sm                   = make_shared<SystemModel>();
    SIT::SIT_Reader sut(sm);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.ParseExcerpt(sit));

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);

    TS_ASSERT_EMPTY (sut.ErrorMessage());
    TS_ASSERT_EMPTY (sut.PlaceHolders());

    auto parsedModel = sut.ParsedSystemModel();

    // With PrettyPrinter
    auto actual_PrettyPrint = PrettyPrinter::PrettyPrint(parsedModel, PrettyPrinterOptions::Parser_debug);
    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, parsedModel);   // This is to avoid warnings about missing AccessInterface
    auto checkResult = sm->Check();
    TS_ASSERT_EMPTY (checkResult.InformativeReport());
  };

  auto data =
  {
    // 00 ==> correct syntax
    make_tuple("LINKER Link_0 Table_Based reg_1 3 \"0b1111, 0b0001, 0b0011, 0b0111,  0b1111, 0b0000, 0b0000, 0b0000\"\n"
               "(\n"
               "  REGISTER reg_1 4 Bypass: \"0b1001\"\n"
               "  REGISTER reg_2 3 Bypass: \"0b110\"\n"
               "  REGISTER reg_3 2 Bypass: \"0b10\"\n"
               ")"s,
               "[Linker](0)    \"Link_0\"\n"
               " :Selector:(1)  \"reg_1\", kind: Table_Based, can_select_none: false, inverted_bits: false, reversed_order: false\n"
               " [Register](1)  \"reg_1\", length: 4, bypass: 1001\n"
               " [Register](2)  \"reg_2\", length: 3, bypass: 110\n"
               " [Register](3)  \"reg_3\", length: 2, bypass: 10"s),
    };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


// Test construction of PDL statement with 1 PDL algorithm name in case of success
//
void UT_SIT_Reader::test_PDL_Success ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](const auto& data)
  {
    // ---------------- Setup
    //
    stringstream    sit(std::get<0>(data));
    auto            expectedAlgoNames = std::get<1>(data);
    auto            sm                = make_shared<SystemModel>();
    SIT::SIT_Reader sut(sm);

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.ParseExcerpt(sit));

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);
    TS_ASSERT_EMPTY (sut.PlaceHolders());

    vector<string>   gotAlgoNames;
    vector<uint32_t> gotLineNumbers;
    const auto&      nameNodesAssociations = sut.PDLAlgorithmNameToNodeAssociation();

    for (const auto& association : nameNodesAssociations)
    {
      gotAlgoNames.emplace_back (association.appName);
      gotLineNumbers.push_back  (association.SIT_line);
    }

    TS_ASSERT_EQUALS (gotAlgoNames, expectedAlgoNames);
  };

  using data_t = tuple<string, vector<string>>;
  auto data =
  {
    // 00 ==> One PDL algorithm
    data_t("JTAG_TAP TAP Loopback 4 1\n"
           "PDL Incr\n"
           "(\n"
           "  REGISTER reg 12 Bypass: \"0xABC\"\n"
           ")\n",
           {"Incr"}
          ),

    // 01 ==> Two PDL algorithms
    data_t("JTAG_TAP TAP Loopback 4 1\n"
           "PDL Incr, Decr\n"
           "(\n"
           "  REGISTER reg 12 Bypass: \"0xABC\"\n"
           ")\n",
           {"Incr", "Decr"}
          ),
    };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


// Test construction of PDL statement with 1 PDL algorithm name in case of failure
//
void UT_SIT_Reader::test_PDL_Failure ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](const auto& data)
  {
    // ---------------- Setup
    //
    stringstream    sit(std::get<0>(data));
    auto            expectedAlgoNames = std::get<1>(data);
    auto            sm                = make_shared<SystemModel>();
    SIT::SIT_Reader sut(sm);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut.ParseExcerpt(sit), mast::ParserException);

    // ---------------- Verify
    //
    TS_ASSERT_EMPTY (sut.PDLAlgorithmNameToNodeAssociation());
  };

  using data_t = tuple<string, vector<string>>;
  auto data =
  {
    // 00 ==> One PDL algorithm
    data_t("JTAG_TAP TAP Loopback 4 1\n"
           "PDL : Incr\n"   // ==> Unexpected colon
           "(\n"
           "  REGISTER reg 12 Bypass: \"0xABC\"\n"
           ")\n",
           {"Incr"}
          ),

    // 01 ==> Two PDL algorithms
    data_t("JTAG_TAP TAP Loopback 4 1\n"
           "PDL increment; Decr\n"  // ==> unexpected semi-colon
           "(\n"
           "  REGISTER reg 12 Bypass: \"0xABC\"\n"
           ")\n",
           {"Incr", "Decr"}
          ),
    };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}

// Test of INSTANCE OF statement with 1 INSTANCE OF
//
void UT_SIT_Reader::test_INSTANCE_OF_Single_Success ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](const auto& data)
  {
    // ---------------- Setup
    //
    stringstream sit(std::get<0>(data));
    auto         expectedKind         = std::get<1>(data);
    auto         expectedInstanceName = std::get<2>(data);
    auto         expectedIdentifier   = std::get<3>(data);

    auto sm = make_shared<SystemModel>();
    SIT::SIT_Reader sut(sm);

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.ParseExcerpt(sit));

    // ---------------- Verify
    //
    const auto& placeHolders = sut.PlaceHolders();
    TS_ASSERT_EQUALS (placeHolders.size(), 1u);

    const auto& placeHolder = placeHolders[0];

    TS_ASSERT_EQUALS (placeHolder.Kind(),       expectedKind);
    TS_ASSERT_EQUALS (placeHolder.Identifier(), expectedIdentifier);

    auto parent = placeHolder.Parent();
    TS_ASSERT_NOT_NULLPTR (parent);
    TS_ASSERT_EQUALS      (parent->Name(), expectedInstanceName);
    TS_ASSERT_NOT_NULLPTR (dynamic_pointer_cast<Chain>(parent));
  };

  using data_t = tuple<string, PlaceHolderKind, string, string>;
  auto data =
  {
    // 00 ==> One SIT INSTANCE_OF 1st node
    data_t("JTAG_TAP TAP Loopback 4 1\n"             // 1
           "(\n"                                     // 2
           "  CHAIN chain_name\n"                    // 3
           "  {\n"                                   // 4
           "    INSTANCE Bar OF Foo.sit\n"           // 5
           "    REGISTER reg 12 Bypass: \"0xABC\"\n" // 6
           "  }\n"                                   // 7
           ")\n",                                    // 8
           PlaceHolderKind::SIT,
           "Bar",
           "Foo.sit"
          ),

    // 01 ==> One SIT INSTANCE - last node
    data_t("JTAG_TAP TAP Loopback 4 1\n"             // 1
           "(\n"                                     // 2
           "  CHAIN chain_name\n"                    // 3
           "  {\n"                                   // 4
           "    REGISTER reg 12 Bypass: \"0xABC\"\n" // 5
           "    INSTANCE Bat OF Foot.sit\n"          // 6
           "  }\n"                                   // 7
           ")\n",                                    // 8
           PlaceHolderKind::SIT,
           "Bat",
           "Foot.sit"
          ),

    // 02 ==> One SIT INSTANCE - middle node
    data_t("JTAG_TAP TAP Loopback 4 1\n"             // 1
           "(\n"                                     // 2
           "  CHAIN chain_name\n"                    // 3
           "  {\n"                                   // 4
           "    REGISTER reg 12 Bypass: \"0xABC\"\n" // 5
           "    INSTANCE Bot OF Fool.sit\n"          // 6
           "    REGISTER reg_2 4 Bypass: \"0xC\"\n"  // 7
           "  }\n"                                   // 8
           ")\n",                                    // 9
           PlaceHolderKind::SIT,
           "Bot",
           "Fool.sit"
          ),

    // 03 ==> One SIT INSTANCE - file path
    data_t("JTAG_TAP TAP Loopback 4 1\n"             // 1
           "(\n"                                     // 2
           "  CHAIN chain_name\n"                    // 3
           "  {\n"                                   // 4
           "    REGISTER reg 12 Bypass: \"0xABC\"\n" // 5
           "    INSTANCE Rat OF One/Two/Three.sit\n" // 6
           "  }\n"                                   // 7
           ")\n",                                    // 8
           PlaceHolderKind::SIT,
           "Rat",
           "One/Two/Three.sit"
          ),

    // 04 ==> One SIT INSTANCE - file path with embedded spaces
    data_t("JTAG_TAP TAP Loopback 4 1\n"                 // 1
           "(\n"                                         // 2
           "  CHAIN chain_name\n"                        // 3
           "  {\n"                                       // 4
           "    REGISTER reg 12 Bypass: \"0xABC\"\n"     // 5
           "    INSTANCE Put OF \"One Two Three.sit\"\n" // 6
           "  }\n"                                       // 7
           ")\n",                                        // 8
           PlaceHolderKind::SIT,
           "Put",
           "One Two Three.sit"
          ),


    // 05 ==> Factory INSTANCE
    data_t("JTAG_TAP TAP Loopback 4 1\n"             // 1
           "(\n"                                     // 2
           "  CHAIN chain_name\n"                    // 3
           "  {\n"                                   // 4
           "    REGISTER reg 12 Bypass: \"0xABC\"\n" // 5
           "    INSTANCE but OF Build_Sub\n"         // 6
           "  }\n"                                   // 7
           ")\n",                                    // 8
           PlaceHolderKind::Factory,
           "but",
           "Build_Sub"
          ),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}






// Test of INSTANCE OF statement with multiple INSTANCE OF
//
void UT_SIT_Reader::test_INSTANCE_OF_Multiple_Success ()
{
  // ---------------- Setup
  //
  stringstream sit
  (
    "JTAG_TAP TAP Loopback 4 1\n"             // 01
    "(\n"                                     // 02
    "  CHAIN chain_name\n"                    // 03
    "  {\n"                                   // 04
    "    REGISTER reg 12 Bypass: \"0xABC\"\n" // 05
    "    INSTANCE Bar OF Foo.sit\n"           // 06
    "    INSTANCE Bat OF Fool\n"              // 07
    "    INSTANCE Bit OF \"Pool.sit\"\n"      // 08
    "  }\n"                                   // 09
    ")\n"                                     // 10
  );

  auto sm = make_shared<SystemModel>();
  SIT::SIT_Reader sut(sm);

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.ParseExcerpt(sit));

  // ---------------- Verify
  //
  const auto& placeHolders = sut.PlaceHolders();
  TS_ASSERT_EQUALS (placeHolders.size(), 3u);

  // Instance 1
  const auto& placeHolder_1 = placeHolders[0];

  TS_ASSERT_EQUALS (placeHolder_1.Kind(),       PlaceHolderKind::SIT);
  TS_ASSERT_EQUALS (placeHolder_1.Identifier(), "Foo.sit");

  auto parent_1 = placeHolder_1.Parent();
  TS_ASSERT_NOT_NULLPTR (parent_1);
  TS_ASSERT_EQUALS      (parent_1->Name(), "Bar");
  TS_ASSERT_NOT_NULLPTR (dynamic_pointer_cast<Chain>(parent_1));

  // Instance 2
  const auto& placeHolder_2 = placeHolders[1];
  const auto  parent_2      = placeHolder_2.Parent();

  TS_ASSERT_EQUALS      (placeHolder_2.Kind(),       PlaceHolderKind::Factory);
  TS_ASSERT_EQUALS      (placeHolder_2.Identifier(), "Fool");
  TS_ASSERT_NOT_NULLPTR (parent_2);
  TS_ASSERT_EQUALS      (parent_2->Name(),           "Bat");
  TS_ASSERT_NOT_NULLPTR (dynamic_pointer_cast<Chain>(parent_2));
  TS_ASSERT_TRUE        (parent_1->NextSibling()     == parent_2);

  // Instance 3
  const auto& placeHolder_3 = placeHolders[2];
  const auto  parent_3      = placeHolder_3.Parent();

  TS_ASSERT_EQUALS      (placeHolder_3.Kind(),       PlaceHolderKind::SIT);
  TS_ASSERT_EQUALS      (placeHolder_3.Identifier(), "Pool.sit");
  TS_ASSERT_NOT_NULLPTR (parent_3);
  TS_ASSERT_EQUALS      (parent_3->Name(),           "Bit");
  TS_ASSERT_NOT_NULLPTR (dynamic_pointer_cast<Chain>(parent_3));
  TS_ASSERT_TRUE        (parent_2->NextSibling()     == parent_3);
}

// Test of INSTANCE OF statement in case of failure
//
void UT_SIT_Reader::test_INSTANCE_OF_Failure ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](const auto& data)
  {
    // ---------------- Setup
    //
    auto sit                  = std::get<0>(data);
    auto expectedErrorMessage = std::get<1>(data);

    auto sm = make_shared<SystemModel>();
    SIT::SIT_Reader sut(sm);

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut.ParseExcerpt(sit), std::runtime_error);

    // ---------------- Verify
    //
    TS_ASSERT_CONTAINS (sut.ErrorMessage(), expectedErrorMessage);
  };

  using data_t = tuple<string, string>;
  auto data =
  {
    // 00
    data_t("JTAG_TAP TAP Loopback 4 1\n"             // 1
           "(\n"                                     // 2
           "  CHAIN chain_name\n"                    // 3
           "  {\n"                                   // 4
           "    INSTANCE Bar Foo.sit\n"              // 5 ==> Missing keyword OF
           "    REGISTER reg 12 Bypass: \"0xABC\"\n" // 6
           "  }\n"                                   // 7
           ")\n",                                    // 8
           "Line 5:18-21: syntax error"
          ),

    // 01
    data_t("JTAG_TAP TAP Loopback 4 1\n"             // 1
           "(\n"                                     // 2
           "  CHAIN chain_name\n"                    // 3
           "  {\n"                                   // 4
           "    REGISTER reg 12 Bypass: \"0xABC\"\n" // 5
           "    INSTANCE Bat OF Hello World.sit\n"   // 6 ==> Path with embedded spaces must be within double quote strings (it is parsed correctly up to 'Hello')
           "  }\n"                                   // 7
           ")\n",                                    // 8
           "Line 6:27-32: syntax error"
          ),

    // 02
    data_t("JTAG_TAP TAP Loopback 4 1\n"             // 1
           "(\n"                                     // 2
           "  CHAIN chain_name\n"                    // 3
           "  {\n"                                   // 4
           "    REGISTER reg 12 Bypass: \"0xABC\"\n" // 5
           "    INSTANCE OF Fool.sit\n"              // 6  ==> Missing instance name
           "    REGISTER reg_2 4 Bypass: \"0xC\"\n"  // 7
           "  }\n"                                   // 8
           ")\n",                                    // 9
           "Line 6:14-16: syntax error"
          ),

    // 03
    data_t("JTAG_TAP TAP Loopback 4 1\n"             // 1
           "(\n"                                     // 2
           "  CHAIN chain_name\n"                    // 3
           "  {\n"                                   // 4
           "    REGISTER reg 12 Bypass: \"0xABC\"\n" // 5
           "    INSTANCE Rat OF \n"                  // 6 ==> Missing SIT file or factory name
           "  }\n"                                   // 7
           ")\n",                                    // 8
           "Line 7:3-4: syntax error"
          ),

    // 04
    data_t("JTAG_TAP TAP Loopback 4 1\n"             // 1
           "(\n"                                     // 2
           "  CHAIN chain_name\n"                    // 3
           "  {\n"                                   // 4
           "    REGISTER reg 12 Bypass: \"0xABC\"\n" // 5
           "    INSTANCE OF Put Far\n"               // 6 ==> Out of order
           "  }\n"                                   // 7
           ")\n",                                    // 8
           "error: Line 6:14-16"        // Report misleadingly: Line 6:14-19 ==> Should be Line 6:14-15 !
          ),


    // 05
    data_t("JTAG_TAP TAP Loopback 4 1\n"             // 1
           "(\n"                                     // 2
           "  CHAIN chain_name\n"                    // 3
           "  {\n"                                   // 4
           "    REGISTER reg 12 Bypass: \"0xABC\"\n" // 5
           "    INSTANCE \"but\" OF Build_Sub\n"     // 6 ==> Instance must not be quoted
           "  }\n"                                   // 7
           ")\n",                                    // 8
           "Line 6:14-19: syntax error"
          ),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}

//===========================================================================
// End of UT_SIT_Reader.cpp
//===========================================================================
