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
#include "LoopbackAccessInterfaceProtocol.hpp"
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
using std::shared_ptr;
using std::make_shared;
using test::UT_reader_wrapper;

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
void UT_reader::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped
  /*Faire un reset des identifiants, regarder les UT du SystemModel*/
  SystemModelNode::ResetNodeIdentifier();

  sm = std::make_shared<mast::SystemModel> ();
}


// Test construction of register nodes from Simplified ICL Tree input
//
void UT_reader::test_register ()
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
    // With PrettyPrinter
    auto actual_PrettyPrint = parseResult.first;
    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, parseResult.second);   // This is to avoid warnings about missing AccessInterface
    auto checkResult = sm->Check();

    TS_ASSERT_EQUALS (checkResult.warningsCount, 0u);
    TS_ASSERT_EQUALS (checkResult.errorsCount,   0u);
    TS_ASSERT_EQUALS (checkResult.infosCount,    0u);
  };

  auto data =
  { /*Basic constructor, different sizes and bypass values*/
   make_tuple( "REGISTER test_register 12 Bypass: \"0b1001:0110:1100\"\n",  "[Register](0)  \"test_register\", length: 12, bypass: 1001_0110:1100"),
   make_tuple( "REGISTER test_register 11 Bypass: \"0b001:0110:1100\"\n",   "[Register](0)  \"test_register\", length: 11, bypass: 0010_1101:100"),
   /*Hold value*/
   make_tuple( "REGISTER test_register 12 Hold_value Bypass: \"0b1001:0110:1100\"\n",  "[Register](0)  \"test_register\", length: 12, Hold value: true, bypass: 1001_0110:1100"),
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

    TS_ASSERT_EQUALS (checkResult.warningsCount, 0u);
    TS_ASSERT_EQUALS (checkResult.errorsCount,   0u);
    TS_ASSERT_EQUALS (checkResult.infosCount,    0u);
  };

  auto data =
  { /*Chain with one register*/
   make_tuple( "CHAIN test_chain\n { REGISTER test_register 12 Bypass: \"0b1001:0110:1100\"\n}",
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
    //+ (begin JFC August/29/2016): for debug purpose
//+    TS_ASSERT_EQUALS (checkResult.MakeReport(), "");
    //+ (end   JFC August/29/2016):
  };

  auto data =
  { /*Exhaustive test of all possible macro parameter combinations*/
make_tuple( "MIB test_MIB POST HIGH 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB POST LOW 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 1, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB PRE LOW 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 1, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE LOW REVERSE 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 1, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB POST LOW REVERSE 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 1, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB POST HIGH REVERSE 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](1)     \"test_MIB\"\n\
 [Linker](2)    \"test_MIB_mux\"\n\
  :Selector:(0)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 1\n\
  [Register](3)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](4)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](0)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
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

    TS_ASSERT_EQUALS (checkResult.warningsCount, 0u);
    TS_ASSERT_EQUALS (checkResult.errorsCount,   0u);
    TS_ASSERT_EQUALS (checkResult.infosCount,    0u);
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

/*Test JTAG macro from Simplified ICL Tree input*/
void UT_reader::test_JTAG_TAP ()
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
    auto checkResult = sm->Check();

    TS_ASSERT_EQUALS (checkResult.warningsCount, 0u);
    TS_ASSERT_EQUALS (checkResult.errorsCount,   0u);
    TS_ASSERT_EQUALS (checkResult.infosCount,    0u);
  };


  auto data =
  {
   make_tuple( "JTAG_TAP my_tap Loopback 4 1\
  {\
     REGISTER test_reg 4 Bypass: \"0b1100\"\
   }\n",
"[Access_I](0)  \"my_tap\"\n\
 [Register](1)  \"my_tap_IR\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](2)    \"my_tap_DR_Mux\"\n\
  :Selector:(1)  \"my_tap_IR\", kind: Table_Based, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"my_tap_BPY\", length: 1, bypass: 1\n\
  [Register](4)  \"test_reg\", length: 4, bypass: 1100"),
   make_tuple( "JTAG_TAP my_tap SVF_simulation 4 1\
  {\
     REGISTER test_reg 4 Bypass: \"0b1100\"\
   }\n",
"[Access_I](0)  \"my_tap\"\n\
 [Register](1)  \"my_tap_IR\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](2)    \"my_tap_DR_Mux\"\n\
  :Selector:(1)  \"my_tap_IR\", kind: Table_Based, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"my_tap_BPY\", length: 1, bypass: 1\n\
  [Register](4)  \"test_reg\", length: 4, bypass: 1100"),
   make_tuple( "JTAG_TAP my_tap SVF_simulation 4 [ \"0xF\" , \"0x2\" ] 1\
  {\
     REGISTER test_reg 4 Bypass: \"0b1100\"\
   }\n",
"[Access_I](0)  \"my_tap\"\n\
 [Register](1)  \"my_tap_IR\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](2)    \"my_tap_DR_Mux\"\n\
  :Selector:(1)  \"my_tap_IR\", kind: Table_Based, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"my_tap_BPY\", length: 1, bypass: 1\n\
  [Register](4)  \"test_reg\", length: 4, bypass: 1100"),
/*   make_tuple( "JTAG_TAP my_tap SVF_openOCD 4 1\
  {\
     REGISTER test_reg 4 Bypass: \"0b1100\"\
   }\n",
"[Access_I](0)  \"my_tap\"\n\
 [Register](1)  \"my_tap_IR\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](2)    \"my_tap_DR_Mux\"\n\
  :Selector:(1)  \"my_tap_IR\", kind: Table_Based, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](3)  \"my_tap_BPY\", length: 1, bypass: 1\n\
  [Register](4)  \"test_reg\", length: 4, bypass: 1100"),*/
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

    TS_ASSERT_EQUALS (checkResult.warningsCount, 0u);
    TS_ASSERT_EQUALS (checkResult.errorsCount,   0u);
    TS_ASSERT_EQUALS (checkResult.infosCount,    0u);
    //+ (begin JFC August/29/2016): for debug purpose
  //+    TS_ASSERT_EQUALS (checkResult.MakeReport(), "");
    //+ (end   JFC August/29/2016):
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
//===========================================================================
// End of UT_reader.cpp
//===========================================================================
