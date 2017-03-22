//===========================================================================
//                           UT_reader.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_GmlPrinter.cpp
//!
//! Implements test fixture for testing SIT_Reader
//!
//===========================================================================


#include "SIT_reader.hpp"
#include "PrettyPrinter.hpp"
#include "SystemModelBuilder.hpp"
#include "Utility.hpp"
#include "LoopbackAccessInterfaceProtocol.hpp"
#include "g3log/g3log.hpp"
#include "UT_reader.hpp"
#include "UT_reader_wrapper.hpp"

#include <cxxtest/ValueTraits.h>
#include <experimental/string_view>
#include <iostream>
#include <tuple>
#include <memory>

using std::tuple;
using std::make_tuple;
using std::string;
using std::experimental::string_view;
using namespace std::chrono_literals;
using namespace std::experimental::literals::string_view_literals;
using std::shared_ptr;
using std::make_shared;
using test::UT_reader_wrapper;

using namespace mast;

#define REDIRECT_CERR(OSS_VAR) \
std::ostringstream OSS_VAR;    \
AT_SCOPE_EXIT([prevRdBuf = std::cerr.rdbuf(errorSink.rdbuf())]() { std::cerr.rdbuf(prevRdBuf); })

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
void UT_reader::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped
  /*Faire un reset des identifiants, regarder les UT du SystemModel*/
  SystemModelNode::ResetNodeIdentifier();

  sm = std::make_shared<mast::SystemModel> ();
  m_loggerInitialState = g3::logEnabled(true);
}

//! Cleanups test (called for each test)
void UT_reader::tearDown ()
{
  g3::logEnabled(m_loggerInitialState);
}



// Test construction of register nodes from Simplified ICL Tree input when the input is correct
//
void UT_reader::test_register_Success ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    auto input_SIT            = std::get<0> (data);
    auto expected_PrettyPrint = std::get<1> (data);

    REDIRECT_CERR(errorSink);

    // ---------------- Exercise
    //
    auto parseResult = UT_reader_wrapper::run_parser_for_UT(input_SIT, sm);

    // ---------------- Verify
    //
    std::this_thread::sleep_for(5ms); // To get messages from logger (running in another thread)

    const auto gotErrorMessage = errorSink.str();
    TS_ASSERT_TRUE (gotErrorMessage.empty());

    // With PrettyPrinter
    auto actual_PrettyPrint = parseResult.first;
    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, parseResult.second);   // This is to avoid warnings about missing AccessInterface
    auto checkResult = sm->Check();
    TS_ASSERT_EQUALS (checkResult.InformativeReport(), "");
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
void UT_reader::test_register_Error ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    auto input_SIT       = std::get<0> (data);
    auto expected_errMSG = std::get<1> (data);
    REDIRECT_CERR(errorSink);

    // ---------------- Exercise
    //
    auto parseResult = UT_reader_wrapper::run_parser_for_UT(input_SIT, sm);

    // ---------------- Verify
    //
    std::this_thread::sleep_for(10ms); // To get messages from logger (running in another thread)

    const auto gotErrorMessage = errorSink.str();
    TS_ASSERT_EQUALS  (gotErrorMessage, expected_errMSG);

    auto systemModelNode = parseResult.second;
    TS_ASSERT_NULLPTR(systemModelNode);
  };

  auto data =  // Sit input, expected PrettyPrint, expected error message, checker error count
  {
    // 00: Wrong Bypass length
    make_tuple("REGISTER test_register 8 Bypass: \"0b1001:011\"\n",
               "Line 1:34-46: REGISTER node \"test_register\" size (8) does not match Bypass value bit count (7)\nParse failed!!\n"),

    // 01: Missing register name
    make_tuple("REGISTER test_register 5 Bypass: \"0b001:0110:1100\"\n",
               "Line 1:34-51: REGISTER node \"test_register\" size (5) does not match Bypass value bit count (11)\nParse failed!!\n"),

    // 02: Missing register length
    make_tuple("REGISTER test_register Bypass: \"0b001\"\n",
               "Line 1:24-30: syntax error\nParse failed!!\n"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}



// Test construction of chain nodes from Simplified ICL Tree input
void UT_reader::test_chain ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    auto input_SIT              = std::get<0> (data);
    auto expected_PrettyPrint = std::get<1> (data);

    // ---------------- Exercise
    //
    auto parseResult = UT_reader_wrapper::run_parser_for_UT(input_SIT, sm);

    // ---------------- Verify
    //
    // With PrettyPrinter
    auto actual_PrettyPrint = parseResult.first;
    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, parseResult.second);   // This is to avoid warnings about missing AccessInterface
    auto checkResult = sm->Check();
    TS_ASSERT_EQUALS (checkResult.InformativeReport(), "");
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
void UT_reader::test_MIB ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    auto input_SIT            = std::get<0> (data);
    auto expected_PrettyPrint = std::get<1> (data);

    // ---------------- Exercise
    //
    auto parseResult = UT_reader_wrapper::run_parser_for_UT(input_SIT, sm);

    // ---------------- Verify
    //
    // With PrettyPrint
    auto actual_PrettyPrint = parseResult.first;
    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, parseResult.second);   // This is to avoid warnings about missing AccessInterface
    auto checkResult = sm->Check();
    TS_ASSERT_EQUALS (checkResult.warningsCount, 1u); // 1 for "Linker 'test_xxx' (id: x) has only 2 children, even though it can select 4 paths"
    TS_ASSERT_EQUALS (checkResult.errorsCount,   0u);
    TS_ASSERT_EQUALS (checkResult.infosCount,    0u);

    if (checkResult.errorsCount != 0)
    {
      TS_ASSERT_EQUALS (checkResult.MakeReport(), "");
    }
  };

  auto data =
  {
    // 00
    make_tuple("MIB test_MIB POST HIGH 4 Binary \n"
               "{\n"
               " REGISTER test_reg_1 4 Bypass: \"0b1001\"\n"
               " REGISTER test_reg_2 4 Bypass: \"0b1100\"\n"
               "}\n",
               "[Chain](1)     \"test_MIB\"\n"
               " [Linker](2)    \"test_MIB_mux\"\n"
               "  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 1, inverted_bits: 0, reversed_order: 0\n"
               "  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n"
               "  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n"
               " [Register](0)  \"test_MIB_ctrl\", length: 3, Hold value: true, bypass: 000"),

    // 01
    make_tuple("MIB test_MIB POST HIGH 4 Binary_noidle \n"
               "{\n"
               "  REGISTER test_reg_1 4 Bypass: \"0b1001\"\n"
               "  REGISTER test_reg_2 4 Bypass: \"0b1100\"\n"
               "}\n",
               "[Chain](1)     \"test_MIB\"\n"
               " [Linker](2)    \"test_MIB_mux\"\n"
               "  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n"
               "  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n"
               "  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n"
               " [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),

    // 02
    make_tuple( "MIB test_MIB POST LOW 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 1, inverted_bits: 1, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 3, Hold value: true, bypass: 000"),

    // 03
    make_tuple( "MIB test_MIB PRE LOW 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 3, Hold value: true, bypass: 000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 1, inverted_bits: 1, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),

    // 04
    make_tuple( "MIB test_MIB PRE HIGH 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 3, Hold value: true, bypass: 000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 1, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),

    // 05
    make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 3, Hold value: true, bypass: 000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 1, inverted_bits: 0, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),

    // 06
    make_tuple( "MIB test_MIB PRE LOW REVERSE 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 3, Hold value: true, bypass: 000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 1, inverted_bits: 1, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),

    // 07
    make_tuple( "MIB test_MIB POST LOW REVERSE 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 1, inverted_bits: 1, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 3, Hold value: true, bypass: 000"),

    // 08
    make_tuple( "MIB test_MIB POST HIGH REVERSE 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 1, inverted_bits: 0, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 3, Hold value: true, bypass: 000"),

make_tuple( "MIB test_MIB POST HIGH 4 N_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 1, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000"),
make_tuple( "MIB test_MIB POST LOW 4 N_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 1, inverted_bits: 1, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111"),
make_tuple( "MIB test_MIB PRE LOW 4 N_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 1, inverted_bits: 1, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH 4 N_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 1, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 N_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 1, inverted_bits: 0, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE LOW REVERSE 4 N_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 1, inverted_bits: 1, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB POST LOW REVERSE 4 N_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 1, inverted_bits: 1, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111"),
make_tuple( "MIB test_MIB POST HIGH REVERSE 4 N_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 1, inverted_bits: 0, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000"),
make_tuple( "MIB test_MIB POST HIGH 4 One_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 1, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000"),
make_tuple( "MIB test_MIB POST LOW 4 One_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 1, inverted_bits: 1, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111"),
make_tuple( "MIB test_MIB PRE LOW 4 One_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 1, inverted_bits: 1, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH 4 One_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 1, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 One_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 1, inverted_bits: 0, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE LOW REVERSE 4 One_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 1, inverted_bits: 1, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB POST LOW REVERSE 4 One_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 1, inverted_bits: 1, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111"),
make_tuple( "MIB test_MIB POST HIGH REVERSE 4 One_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 1, inverted_bits: 0, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000"),
make_tuple( "MIB test_MIB POST HIGH 4 Binary_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB POST LOW 4 Binary_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 1, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB PRE LOW 4 Binary_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 1, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH 4 Binary_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 Binary_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE LOW REVERSE 4 Binary_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 1, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB POST LOW REVERSE 4 Binary_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 1, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB POST HIGH REVERSE 4 Binary_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB POST HIGH 4 One_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1000"),
make_tuple( "MIB test_MIB POST LOW 4 One_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 0, inverted_bits: 1, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0111"),
make_tuple( "MIB test_MIB PRE LOW 4 One_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0111\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 0, inverted_bits: 1, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH 4 One_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 One_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0001\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 0, inverted_bits: 0, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE LOW REVERSE 4 One_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1110\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 0, inverted_bits: 1, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB POST LOW REVERSE 4 One_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 0, inverted_bits: 1, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1110"),
make_tuple( "MIB test_MIB POST HIGH REVERSE 4 One_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 0, inverted_bits: 0, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0001"),
make_tuple( "MIB test_MIB POST HIGH 4 N_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1000"),
make_tuple( "MIB test_MIB POST LOW 4 N_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 0, inverted_bits: 1, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0111"),
make_tuple( "MIB test_MIB PRE LOW 4 N_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0111\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 0, inverted_bits: 1, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH 4 N_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1000\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 N_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0001\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 0, inverted_bits: 0, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE LOW REVERSE 4 N_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1110\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 0, inverted_bits: 1, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB POST LOW REVERSE 4 N_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 0, inverted_bits: 1, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1110"),
make_tuple( "MIB test_MIB POST HIGH REVERSE 4 N_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 0, inverted_bits: 0, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0001"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}



/*Test SIB macro from Simplified ICL Tree input*/
void UT_reader::test_SIB ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    auto input_SIT            = std::get<0> (data);
    auto expected_PrettyPrint = std::get<1> (data);

    // ---------------- Exercise
    //
    auto parseResult = UT_reader_wrapper::run_parser_for_UT(input_SIT, sm);

    // ---------------- Verify
    //
    // With PrettyPrint
    auto actual_PrettyPrint = parseResult.first;
    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, parseResult.second);   // This is to avoid warnings about missing AccessInterface
    auto checkResult = sm->Check();
    TS_ASSERT_EQUALS (checkResult.InformativeReport(), "");
  };

  auto data =
  { /*Exhaustive test of all possible macro parameter combinations*/
   make_tuple( "SIB test_SIB POST HIGH\
    {REGISTER test_reg 4 Bypass: \"0b1001\"}\n",

"[Chain](1)     \"test_SIB\"\n\
 [Linker](2)    \"test_SIB_mux\"\n\
  :Selector:(0)  \"test_SIB_ctrl\", kind: Binary, can_select_none: 1, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"test_reg\", length: 4, bypass: 1001\n\
 [Register](0)  \"test_SIB_ctrl\", length: 1, Hold value: true, bypass: 0"),
   make_tuple( "SIB test_SIB POST LOW\
    {REGISTER test_reg 4 Bypass: \"0b1001\"}\n",

"[Chain](1)     \"test_SIB\"\n\
 [Linker](2)    \"test_SIB_mux\"\n\
  :Selector:(0)  \"test_SIB_ctrl\", kind: Binary, can_select_none: 1, inverted_bits: 1, reversed_order: 0\n\
  [Register](3)  \"test_reg\", length: 4, bypass: 1001\n\
 [Register](0)  \"test_SIB_ctrl\", length: 1, Hold value: true, bypass: 0"),
   make_tuple( "SIB test_SIB PRE HIGH\
    {REGISTER test_reg 4 Bypass: \"0b1001\"}\n",
"[Chain](1)     \"test_SIB\"\n\
 [Register](0)  \"test_SIB_ctrl\", length: 1, Hold value: true, bypass: 0\n\
 [Linker](2)    \"test_SIB_mux\"\n\
  :Selector:(0)  \"test_SIB_ctrl\", kind: Binary, can_select_none: 1, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"test_reg\", length: 4, bypass: 1001"),
   make_tuple( "SIB test_SIB PRE LOW\
    {REGISTER test_reg 4 Bypass: \"0b1001\"}\n",
"[Chain](1)     \"test_SIB\"\n\
 [Register](0)  \"test_SIB_ctrl\", length: 1, Hold value: true, bypass: 0\n\
 [Linker](2)    \"test_SIB_mux\"\n\
  :Selector:(0)  \"test_SIB_ctrl\", kind: Binary, can_select_none: 1, inverted_bits: 1, reversed_order: 0\n\
  [Register](3)  \"test_reg\", length: 4, bypass: 1001"),

  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}

//! Test JTAG macro from Simplified ICL Tree input - In cases with success
//!
void UT_reader::test_JTAG_TAP_Success ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    auto input_SIT            = std::get<0> (data);
    auto expected_PrettyPrint = std::get<1> (data);

    REDIRECT_CERR(errorSink);

    // ---------------- Exercise
    //
    auto parseResult = UT_reader_wrapper::run_parser_for_UT(input_SIT, sm);

    // ---------------- Verify
    //
    // Error messages
    std::this_thread::sleep_for(5ms); // To get messages from logger (running in another thread)
    const auto gotErrorMessage = errorSink.str();
    TS_ASSERT_EQUALS (gotErrorMessage, "");

    // With PrettyPrint
    auto actual_PrettyPrint = parseResult.first;
    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    auto checkResult = sm->Check();
    TS_ASSERT_EQUALS (checkResult.InformativeReport(), "");
  };


  auto data =
  {
    // 00: Loopback
    make_tuple("JTAG_TAP my_tap Loopback 4 1\n"
               "{"
               "  REGISTER test_reg 4 Bypass: \"0b1100\"\n"
               "}\n",
               "[Access_I](0)  \"my_tap\", Protocol: Loopback\n"
               " [Register](1)  \"my_tap_IR\", length: 4, Hold value: true, bypass: 1111\n"
               " [Linker](2)    \"my_tap_DR_Mux\"\n"
               "  :Selector:(1)  \"my_tap_IR\", kind: Table_Based, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n"
               "  [Register](3)  \"my_tap_BPY\", length: 1, bypass: 1\n"
               "  [Register](4)  \"test_reg\", length: 4, bypass: 1100"),

    // 01: SVF Simulation
    make_tuple("JTAG_TAP my_tap SVF_Simulation 4 1\n"
               "{\n"
               "  REGISTER test_reg 4 Bypass: \"0b1100\"\n"
               "}\n",
               "[Access_I](0)  \"my_tap\", Protocol: SVF_Simulation\n"
               " [Register](1)  \"my_tap_IR\", length: 4, Hold value: true, bypass: 1111\n"
               " [Linker](2)    \"my_tap_DR_Mux\"\n"
               "  :Selector:(1)  \"my_tap_IR\", kind: Table_Based, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n"
               "  [Register](3)  \"my_tap_BPY\", length: 1, bypass: 1\n"
               "  [Register](4)  \"test_reg\", length: 4, bypass: 1100"),

    // 02: SVF Simulation
    make_tuple("JTAG_TAP my_tap SVF_Simulation 4 [ \"0xF\" , \"0x2\" ] 1"    // Ignored array
               "{"
               " REGISTER test_reg 4 Bypass: \"0b1100\""
               "}\n",
               "[Access_I](0)  \"my_tap\", Protocol: SVF_Simulation\n"
               " [Register](1)  \"my_tap_IR\", length: 4, Hold value: true, bypass: 1111\n"
               " [Linker](2)    \"my_tap_DR_Mux\"\n"
               "  :Selector:(1)  \"my_tap_IR\", kind: Table_Based, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n"
               "  [Register](3)  \"my_tap_BPY\", length: 1, bypass: 1\n"
               "  [Register](4)  \"test_reg\", length: 4, bypass: 1100"),

    // 03: SVF Emulation
    make_tuple("JTAG_TAP my_tap SVF_Emulation 4 1"
               "{"
               " REGISTER test_reg 4 Bypass: \"0b1100\""
               "}\n",
               "[Access_I](0)  \"my_tap\", Protocol: SVF_Emulation\n"
               " [Register](1)  \"my_tap_IR\", length: 4, Hold value: true, bypass: 1111\n"
               " [Linker](2)    \"my_tap_DR_Mux\"\n"
               "  :Selector:(1)  \"my_tap_IR\", kind: Table_Based, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n"
               "  [Register](3)  \"my_tap_BPY\", length: 1, bypass: 1\n"
               "  [Register](4)  \"test_reg\", length: 4, bypass: 1100"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Test JTAG macro from Simplified ICL Tree input - In cases with failure
//!
void UT_reader::test_JTAG_TAP_Failure ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    auto input_SIT         = std::get<0> (data);
    auto expected_ErrorMsg = std::get<1> (data);

    REDIRECT_CERR(errorSink);

    // ---------------- Exercise
    //
    auto parseResult = UT_reader_wrapper::run_parser_for_UT(input_SIT, sm);

    // ---------------- Verify
    //
    std::this_thread::sleep_for(5ms); // To get messages from logger (running in another thread)
    const auto gotErrorMessage = errorSink.str();

    auto containsExpected = gotErrorMessage.find(expected_ErrorMsg) != string::npos;

    //+ (begin JFC February/07/2017): for debug purpose
    if (!containsExpected) { TS_FAIL (gotErrorMessage); }
    //+ (end   JFC February/07/2017):

    TS_ASSERT_TRUE (containsExpected);
  };

  auto data =
  {
    // 00: SVF OpenOCD
    make_tuple("JTAG_TAP my_tap SVF_openOCD 4 1"
               "{"
               "  REGISTER test_reg 4 Bypass: \"0b1100\""
               "}\n",
               "Line 1:31-32: JTAG_TAP node \"my_tap\" Cannot create protocol: \"SVF_openOCD\"; std::invalid_argument: There is no creation method registered with name: JTAG_SVF_openOCD."
               ),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}

//! Test ACCES_INTERFACE from Simplified ICL Tree input - In cases with success
//!
void UT_reader::test_ACCES_INTERFACE_Success ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    auto input_SIT            = std::get<0> (data);
    auto expected_PrettyPrint = std::get<1> (data);

    REDIRECT_CERR(errorSink);

    // ---------------- Exercise
    //
    auto parseResult = UT_reader_wrapper::run_parser_for_UT(input_SIT, sm);

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);

    // Error messages
    std::this_thread::sleep_for(5ms); // To get messages from logger (running in another thread)
    const auto gotErrorMessage = errorSink.str();
    TS_ASSERT_EQUALS (gotErrorMessage, "");

    // With PrettyPrint
    auto actual_PrettyPrint = parseResult.first;
    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    auto checkResult = sm->Check();
    TS_ASSERT_EQUALS (checkResult.InformativeReport(), "");
  };


  auto data =
  {
    // 00: Loopback
    make_tuple("ACCESS_INTERFACE my_tap JTAG_Loopback\n"
               "{\n"
               "  REGISTER test_reg 4 Bypass: \"0b1100\"\n"
               "}\n",
               "[Access_I](0)  \"my_tap\", Protocol: Loopback\n"
               " [Register](1)  \"test_reg\", length: 4, bypass: 1100"),

    // 01: SVF Simulation
    make_tuple("ACCESS_INTERFACE my_tap JTAG_SVF_Simulation\n"
               "{\n"
               "  REGISTER reg_1 3 Bypass: \"0b101\"\n"
               "  REGISTER reg_2 5 Bypass: \"0b11001\"\n"
               "}\n",
               "[Access_I](0)  \"my_tap\", Protocol: SVF_Simulation\n"
               " [Register](1)  \"reg_1\", length: 3, bypass: 101\n"
               " [Register](2)  \"reg_2\", length: 5, bypass: 1100_1"),

    // 02: SVF Emulation
    make_tuple("ACCESS_INTERFACE my_tap  JTAG_SVF_Emulation \n"
               "{\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               "}\n",
               "[Access_I](0)  \"my_tap\", Protocol: SVF_Emulation\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),

    // 03: Offline
    make_tuple("ACCESS_INTERFACE my_tap  Offline  \n"
               "{\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               "}\n",
               "[Access_I](0)  \"my_tap\", Protocol: Offline\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),

    // 04: STIL_Emulation
    make_tuple("ACCESS_INTERFACE my_tap  STIL_Emulation \"3\" \n"
               "{\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               "}\n",
               "[Access_I](0)  \"my_tap\", Protocol: STIL_Emulation\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),

    // 05: I2C_Emulation
    make_tuple("ACCESS_INTERFACE my_tap  I2C_Emulation \"0x40, 0x41, 0x42, [I2C]\" \n"
               "{\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               "}\n",
               "[Access_I](0)  \"my_tap\", Protocol: I2C_Emulation\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),

    // 06: Any letter in parameters
    make_tuple("ACCESS_INTERFACE letters I2C_Emulation \"0x40, 0x41, 0x42, abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ\" \n"
               "{\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               "}\n",
               "[Access_I](0)  \"letters\", Protocol: I2C_Emulation\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),

    // 07: Any digits in parameters
    make_tuple("ACCESS_INTERFACE digits I2C_Emulation \"0x40, 0x41, 0x42, _0123456789 \" \n"
               "{\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               "}\n",
               "[Access_I](0)  \"digits\", Protocol: I2C_Emulation\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),

    // 08: Any brackets characters in parameters
    make_tuple("ACCESS_INTERFACE brackets  I2C_Emulation \"0x40, 0x41, 0x42, ([<()>])\" \n"
               "{\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               "}\n",
               "[Access_I](0)  \"brackets\", Protocol: I2C_Emulation\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),

    // 09: Any punctuation characters in parameters
    make_tuple("ACCESS_INTERFACE punctuation  I2C_Emulation \"0x40, 0x41, 0x42, ?!.:; \" \n"
               "{\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               "}\n",
               "[Access_I](0)  \"punctuation\", Protocol: I2C_Emulation\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),

    // 10: Any operators characters in parameters
    make_tuple("ACCESS_INTERFACE operators  I2C_Emulation \"0x40, 0x41, 0x42, /*-+~%\" \n"
               "{\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               "}\n",
               "[Access_I](0)  \"operators\", Protocol: I2C_Emulation\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),

    // 11: Any special characters in parameters
    make_tuple("ACCESS_INTERFACE special  I2C_Emulation \"0x40, 0x41, 0x42, $=@'_|& \" \n"
               "{\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               "}\n",
               "[Access_I](0)  \"special\", Protocol: I2C_Emulation\n"
               " [Register](1)  \"r1\", length: 1, bypass: 1\n"
               " [Register](2)  \"r2\", length: 2, bypass: 11"),


    #ifdef INTEL_EXPERIMENT
    // 12: Intel_Packet
    make_tuple("ACCESS_INTERFACE tap  Intel_Packet \"0x41, 0x42\" \n"
               "{\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               "}\n",
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
void UT_reader::test_ACCES_INTERFACE_Failure ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    auto input_SIT         = std::get<0> (data);
    auto expected_ErrorMsg = std::get<1> (data);

    REDIRECT_CERR(errorSink);

    // ---------------- Exercise
    //
    auto parseResult = UT_reader_wrapper::run_parser_for_UT(input_SIT, sm);

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);

    std::this_thread::sleep_for(5ms);         // To get messages from logger (running in another thread)
    const auto gotErrorMessage = errorSink.str();

    TS_ASSERT_FALSE (gotErrorMessage.empty()); // Make sure there is an error message


    auto containsExpected = gotErrorMessage.find(expected_ErrorMsg) != string::npos;

    //+ (begin JFC February/07/2017): for debug purpose
    if (!containsExpected) { TS_FAIL (gotErrorMessage); }
    //+ (end   JFC February/07/2017):

    TS_ASSERT_TRUE (containsExpected);
  };


  auto data =
  {
    // 00: Empty integer array
    make_tuple("ACCESS_INTERFACE Useless_Square_Brackets JTAG_Loopback []\n"
               "{\n"
               "  REGISTER test_reg 4 Bypass: \"0b1100\"\n"
               "}\n",
               "Line 1:56-57: syntax error"),

    // 01: Unregistered protocol type
    make_tuple("ACCESS_INTERFACE Unregistered_Protocol MyProtocol\n"
               "{\n"
               "  REGISTER reg_1 3 Bypass: \"0b101\"\n"
               "  REGISTER reg_2 5 Bypass: \"0b11001\"\n"
               "}\n",
               "Line 2:1-2: ACCESS_INTERFACE node \"Unregistered_Protocol\" Cannot create protocol: \"MyProtocol\"; std::invalid_argument: There is no creation method registered with name: MyProtocol."),

    // 02: No AccessInterface name (same as no protocol)
    make_tuple("ACCESS_INTERFACE Offline  \n"
               "{\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               "}\n",
               "Line 2:1-2: syntax error"),

    // 03: Not enough addresses
    make_tuple("ACCESS_INTERFACE Not_enough_Adresses I2C_Emulation \"0x40, (i2c)\"  \n"
               "{\n"
               "   REGISTER r1 1 Bypass: \"0b1\"\n"
               "   REGISTER r2 2 Bypass: \"0b11\"\n"
               "}\n",
               "Line 1:52-65: ACCESS_INTERFACE node \"Not_enough_Adresses\" Cannot create protocol: \"I2C_Emulation\"; std::invalid_argument: I2C Addresses must have at least two entries."),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


/*Test 1500 Wrapper macro from Simplified ICL Tree input*/
void UT_reader::test_1500 ()
{
  // ---------------- Setup
  //
  auto checker = [&](auto data)
  {
    auto input_SIT            = std::get<0> (data);
    auto expected_PrettyPrint = std::get<1> (data);

    // ---------------- Exercise
    //
    auto parseResult = UT_reader_wrapper::run_parser_for_UT(input_SIT, sm);

    // ---------------- Verify
    //
    // With PrettyPrint
    auto actual_PrettyPrint = parseResult.first;
    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, parseResult.second);   // This is to avoid warnings about missing AccessInterface
    auto checkResult = sm->Check();
    TS_ASSERT_EQUALS (checkResult.InformativeReport(), "");
  };

  auto data =
  {
   make_tuple( "WRAPPER_1500 test_wrapper 2\
 { REGISTER test_register_1 12 Bypass: \"0b1001:0110:1100\"\
   REGISTER test_register_2 12 Bypass: \"0b1001:0110:1100\"}\n",
"[Chain](0)     \"test_wrapper\"\n\
 [Chain](2)     \"SWIR\"\n\
  [Register](1)  \"SWIR_ctrl\", length: 1, Hold value: true, bypass: 0\n\
  [Linker](3)    \"SWIR_mux\"\n\
   :Selector:(1)  \"SWIR_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
   [Chain](5)     \"WIR\"\n\
    [Linker](6)    \"WIR_mux\"\n\
     :Selector:(4)  \"WIR_reg\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
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
void UT_reader::test_LINKER_Success ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    auto input_SIT            = std::get<0>(data);
    auto expected_PrettyPrint = std::get<1>(data);

    REDIRECT_CERR(errorSink);

    // ---------------- Exercise
    //
    auto parseResult = UT_reader_wrapper::run_parser_for_UT(input_SIT, sm);

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);

    // Error messages
    std::this_thread::sleep_for(5ms); // To get messages from logger (running in another thread)
    const auto gotErrorMessage = errorSink.str();
    TS_ASSERT_EQUALS (gotErrorMessage, "");

    // With PrettyPrint
    auto actual_PrettyPrint = parseResult.first;
    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, parseResult.second);   // This is to avoid warnings about missing AccessInterface
    auto checkResult = sm->Check();
    TS_ASSERT_EQUALS (checkResult.InformativeReport(), "");

  };

  auto data =
  {
    // 01 ==> correct syntax
    make_tuple("LINKER test_LINKER One_Hot test_reg_1 4\n"
               "{\n"
               "  REGISTER test_reg_1 4 Bypass: \"0b1001\"\n"
               "  REGISTER test_reg_2 4 Bypass: \"0b1100\"\n"
               "  REGISTER test_reg_3 2 Bypass: \"0b10\"\n"
               "  REGISTER test_reg_4 5 Bypass: \"0b11001\"\n"
               "}"s,
               "[Linker](0)    \"test_LINKER\"\n"
               " :Selector:(1)  \"test_reg_1\", kind: One_Hot, can_select_none: 1, inverted_bits: 0, reversed_order: 0\n"
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
void UT_reader::test_LINKER_Error ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto data)
  {
    // ---------------- Setup
    //
    auto input_SIT       = std::get<0> (data);
    auto expected_errMSG = std::get<1> (data);

    REDIRECT_CERR(errorSink);

    // ---------------- Exercise
    //
    auto parseResult = UT_reader_wrapper::run_parser_for_UT(input_SIT, sm);

    // ---------------- Verify
    //
    std::this_thread::sleep_for(10ms); // To get messages from logger (running in another thread)

    const auto gotErrorMessage = errorSink.str();
    TS_ASSERT_EQUALS  (gotErrorMessage, expected_errMSG);

    auto systemModelNode = parseResult.second;
    TS_ASSERT_NULLPTR(systemModelNode);
  };

  auto data =
  {
    // 00 ==> Error: selector register not defined
    make_tuple("LINKER test_LINKER Binary 4\n"
               "{REGISTER test_reg_1 4 Bypass: \"0b1001\"\n"
               "REGISTER test_reg_2 4 Bypass: \"0b1100\"\n"
               "}"s,
               "Line 1:27-28: LINKER node \"test_LINKER\" Must specify a control node (Register) for its path selector\n"
               "Parse failed!!\n"s),

    // 01 ==> Error: selector register does not exist
    make_tuple("LINKER test_LINKER Binary selector_reg 4\n"
               "{\n"
               "  REGISTER test_reg_1 4 Bypass: \"0b1001\"\n"
               "  REGISTER test_reg_2 4 Bypass: \"0b1100\"\n"
               "}"s,
               "Line 1:40-41: LINKER node \"test_LINKER\" Error, specified selector register \"selector_reg\" does not exist\n"
               "Parse failed!!\n"s),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//===========================================================================
// End of UT_reader.cpp
//===========================================================================
