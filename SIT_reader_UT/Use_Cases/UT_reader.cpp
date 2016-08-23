//===========================================================================
//                           UT_GmlPrinterVisitor.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_GmlPrinterVisitor.cpp
//!
//! Implements test fixture for testing GmlPrinterVisitor
//!
//===========================================================================


#include <experimental/string_view>
#include <cxxtest/ValueTraits.h>
#include <iostream>
#include <tuple>
#include "SIT_reader.hpp"
#include "PrettyPrinterVisitor.hpp"
#include "UT_reader.hpp"
#include "UT_reader_wrapper.hpp"

using std::tuple;
using std::make_tuple;
using std::string;
using std::experimental::string_view;
using test::UT_reader_wrapper;
 
UT_reader::UT_reader()
{ 
 sm = std::make_shared<mast::SystemModel>();
}

string expectedReport = "Errors   (0):\n"
                        "Warnings (0):\n"
                        "Infos    (0):\n";
string expectedMIBReport = "Errors   (0):\n"
			   "Warnings (1):\n"
			   "  - Linker 'test_MIB_mux' (id: 4) has only 2 children, even though it can select 4 paths\n"
                        "Infos    (0):\n";

//! Initializes tests (called for each test)
//!
void UT_reader::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped
 /*Faire un reset des identifiants, regarder les UT du SystemModel*/  
 SystemModelNode::ResetNodeIdentifier();
 sm = std::make_shared<mast::SystemModel>();
 
}


/*Test construction of register nodes from Simplified ICL Tree input*/
void UT_reader::test_register ()
{
  // ---------------- Exercise
  //

  //
  auto checker = [&](auto data)
  {
    auto input_SIT = std::get<0>(data);
    auto expected_PrettyPrinter = std::get<1>(data);
    
    // ---------------- Exercise & Verify
    //
      UT_reader_wrapper reader;
    auto result=reader.run_parser_for_UT(input_SIT,sm);
    auto actual_PrettyPrinter=result.first;
    TS_ASSERT_EQUALS (actual_PrettyPrinter, expected_PrettyPrinter);
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


/*Test construction of chain nodes from Simplified ICL Tree input*/
void UT_reader::test_chain ()
{
  // ---------------- Exercise
  //

  //
  auto checker = [&](auto data)
  {
    auto input_SIT = std::get<0>(data);
    auto expected_PrettyPrinter = std::get<1>(data);
    
    // ---------------- Exercise & Verify
    //
      UT_reader_wrapper reader;
    auto result=reader.run_parser_for_UT(input_SIT,sm);
    auto actual_PrettyPrinter=result.first;
    TS_ASSERT_EQUALS (actual_PrettyPrinter, expected_PrettyPrinter);

   auto parent = dynamic_pointer_cast<ParentNode> (result.second);
   sm->ReplaceRoot(parent,false);
   auto check_result = sm->Check();

    auto   report         = check_result.MakeReport();
  TS_ASSERT_EQUALS (report, expectedReport);
  }; 
  
  auto data =
  { /*Chain with one register*/
   make_tuple( "CHAIN test_chain\n { REGISTER test_register 12 Bypass: \"0b1001:0110:1100\"\n}",  
   "[Chain](1)     \"test_chain\"\n [Register](0)  \"test_register\", length: 12, bypass: 1001_0110:1100"),
    /*Chain with two registers*/ 
    make_tuple(    "CHAIN test_chain\
    { REGISTER test_register_1 12 Bypass: \"0b1001:0110:1100\"\
      REGISTER test_register_2 12 Bypass: \"0b1001:0110:1100\"\
    }",
"[Chain](2)     \"test_chain\"\n\
 [Register](0)  \"test_register_1\", length: 12, bypass: 1001_0110:1100\n\
 [Register](1)  \"test_register_2\", length: 12, bypass: 1001_0110:1100"
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
"[Chain](6)     \"test_chain_1\"\n\
 [Chain](5)     \"test_chain_2\"\n\
  [Chain](4)     \"test_chain_3\"\n\
   [Chain](3)     \"test_chain_4\"\n\
    [Chain](2)     \"test_chain_5\"\n\
     [Register](0)  \"test_register_1\", length: 12, bypass: 1001_0110:1100\n\
     [Register](1)  \"test_register_2\", length: 12, bypass: 1001_0110:1100"),

  };

     
  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);

}

/*Test construction of MIB macro from Simplified ICL Tree input*/
void UT_reader::test_MIB ()
{
  // ---------------- Exercise
  //

  //
  auto checker = [&](auto data)
  {
    auto input_SIT = std::get<0>(data);
    auto expected_PrettyPrinter = std::get<1>(data);
    
    // ---------------- Exercise & Verify
    //
      UT_reader_wrapper reader;
    auto result=reader.run_parser_for_UT(input_SIT,sm);
    auto actual_PrettyPrinter=result.first;
    TS_ASSERT_EQUALS (actual_PrettyPrinter, expected_PrettyPrinter);

   auto parent = dynamic_pointer_cast<ParentNode> (result.second);
   sm->ReplaceRoot(parent,false);
   auto check_result = sm->Check();

    auto   report         = check_result.MakeReport();
  TS_ASSERT_EQUALS (report, expectedMIBReport);
  }; 
  
  auto data =
  { /*Exhaustive test of all possible macro parameter combinations*/
make_tuple( "MIB test_MIB POST HIGH 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB POST LOW 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 1, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB PRE LOW 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 1, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE LOW REVERSE 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 1, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB POST LOW REVERSE 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 1, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB POST HIGH REVERSE 4 Binary \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB POST HIGH 4 N_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 1, inverted_bits: 0, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000"),
make_tuple( "MIB test_MIB POST LOW 4 N_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 1, inverted_bits: 1, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111"),
make_tuple( "MIB test_MIB PRE LOW 4 N_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 1, inverted_bits: 1, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH 4 N_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 1, inverted_bits: 0, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 N_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 1, inverted_bits: 0, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE LOW REVERSE 4 N_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 1, inverted_bits: 1, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB POST LOW REVERSE 4 N_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 1, inverted_bits: 1, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111"),
make_tuple( "MIB test_MIB POST HIGH REVERSE 4 N_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 1, inverted_bits: 0, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000"),
make_tuple( "MIB test_MIB POST HIGH 4 One_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 1, inverted_bits: 0, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000"),
make_tuple( "MIB test_MIB POST LOW 4 One_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 1, inverted_bits: 1, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111"),
make_tuple( "MIB test_MIB PRE LOW 4 One_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 1, inverted_bits: 1, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH 4 One_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 1, inverted_bits: 0, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 One_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 1, inverted_bits: 0, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE LOW REVERSE 4 One_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 1, inverted_bits: 1, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB POST LOW REVERSE 4 One_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 1, inverted_bits: 1, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1111"),
make_tuple( "MIB test_MIB POST HIGH REVERSE 4 One_Hot \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 1, inverted_bits: 0, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0000"),
make_tuple( "MIB test_MIB POST HIGH 4 Binary_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB POST LOW 4 Binary_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 1, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB PRE LOW 4 Binary_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 1, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH 4 Binary_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 Binary_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE LOW REVERSE 4 Binary_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 1, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB POST LOW REVERSE 4 Binary_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 1, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB POST HIGH REVERSE 4 Binary_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 2, Hold value: true, bypass: 00"),
make_tuple( "MIB test_MIB POST HIGH 4 One_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1000"),
make_tuple( "MIB test_MIB POST LOW 4 One_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 0, inverted_bits: 1, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0111"),
make_tuple( "MIB test_MIB PRE LOW 4 One_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0111\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 0, inverted_bits: 1, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH 4 One_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1000\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 One_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0001\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 0, inverted_bits: 0, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE LOW REVERSE 4 One_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1110\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 0, inverted_bits: 1, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB POST LOW REVERSE 4 One_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 0, inverted_bits: 1, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1110"),
make_tuple( "MIB test_MIB POST HIGH REVERSE 4 One_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: One_Hot, can_select_none: 0, inverted_bits: 0, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0001"),
make_tuple( "MIB test_MIB POST HIGH 4 N_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1000"),
make_tuple( "MIB test_MIB POST LOW 4 N_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 0, inverted_bits: 1, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0111"),
make_tuple( "MIB test_MIB PRE LOW 4 N_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0111\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 0, inverted_bits: 1, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH 4 N_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1000\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE HIGH REVERSE 4 N_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0001\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 0, inverted_bits: 0, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB PRE LOW REVERSE 4 N_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1110\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 0, inverted_bits: 1, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100"),
make_tuple( "MIB test_MIB POST LOW REVERSE 4 N_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 0, inverted_bits: 1, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 1110"),
make_tuple( "MIB test_MIB POST HIGH REVERSE 4 N_Hot_noidle \
    {REGISTER test_reg_1 4 Bypass: \"0b1001\"\
     REGISTER test_reg_2 4 Bypass: \"0b1100\"\
    }",
"[Chain](3)     \"test_MIB\"\n\
 [Linker](4)    \"test_MIB_mux\"\n\
  :Selector:(2)  \"test_MIB_ctrl\", kind: N_Hot, can_select_none: 0, inverted_bits: 0, reversed_order: 1\n\
  [Register](0)  \"test_reg_1\", length: 4, bypass: 1001\n\
  [Register](1)  \"test_reg_2\", length: 4, bypass: 1100\n\
 [Register](2)  \"test_MIB_ctrl\", length: 4, Hold value: true, bypass: 0001"),

  };

     
  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);

}

/*Test SIB macro from Simplified ICL Tree input*/
void UT_reader::test_SIB ()
{
  // ---------------- Exercise
  //

  //
  auto checker = [&](auto data)
  {
    auto input_SIT = std::get<0>(data);
    auto expected_PrettyPrinter = std::get<1>(data);
    
    // ---------------- Exercise & Verify
    //
      UT_reader_wrapper reader;
    auto result=reader.run_parser_for_UT(input_SIT,sm);
    auto actual_PrettyPrinter=result.first;
    TS_ASSERT_EQUALS (actual_PrettyPrinter, expected_PrettyPrinter);

   auto parent = dynamic_pointer_cast<ParentNode> (result.second);
   sm->ReplaceRoot(parent,false);
   auto check_result = sm->Check();

    auto   report         = check_result.MakeReport();
  TS_ASSERT_EQUALS (report, expectedReport);
  }; 
  
  auto data =
  { /*Exhaustive test of all possible macro parameter combinations*/
   make_tuple( "SIB test_SIB POST HIGH\
    {REGISTER test_reg 4 Bypass: \"0b1001\"}\n",

"[Chain](2)     \"test_SIB\"\n\
 [Linker](3)    \"test_SIB_mux\"\n\
  :Selector:(1)  \"test_SIB_ctrl\", kind: Binary, can_select_none: 1, inverted_bits: 0, reversed_order: 0\n\
  [Register](0)  \"test_reg\", length: 4, bypass: 1001\n\
 [Register](1)  \"test_SIB_ctrl\", length: 1, Hold value: true, bypass: 0"),
   make_tuple( "SIB test_SIB POST LOW\
    {REGISTER test_reg 4 Bypass: \"0b1001\"}\n",

"[Chain](2)     \"test_SIB\"\n\
 [Linker](3)    \"test_SIB_mux\"\n\
  :Selector:(1)  \"test_SIB_ctrl\", kind: Binary, can_select_none: 1, inverted_bits: 1, reversed_order: 0\n\
  [Register](0)  \"test_reg\", length: 4, bypass: 1001\n\
 [Register](1)  \"test_SIB_ctrl\", length: 1, Hold value: true, bypass: 0"),
   make_tuple( "SIB test_SIB PRE HIGH\
    {REGISTER test_reg 4 Bypass: \"0b1001\"}\n",
"[Chain](2)     \"test_SIB\"\n\
 [Register](1)  \"test_SIB_ctrl\", length: 1, Hold value: true, bypass: 0\n\
 [Linker](3)    \"test_SIB_mux\"\n\
  :Selector:(1)  \"test_SIB_ctrl\", kind: Binary, can_select_none: 1, inverted_bits: 0, reversed_order: 0\n\
  [Register](0)  \"test_reg\", length: 4, bypass: 1001"),
   make_tuple( "SIB test_SIB PRE LOW\
    {REGISTER test_reg 4 Bypass: \"0b1001\"}\n",
"[Chain](2)     \"test_SIB\"\n\
 [Register](1)  \"test_SIB_ctrl\", length: 1, Hold value: true, bypass: 0\n\
 [Linker](3)    \"test_SIB_mux\"\n\
  :Selector:(1)  \"test_SIB_ctrl\", kind: Binary, can_select_none: 1, inverted_bits: 1, reversed_order: 0\n\
  [Register](0)  \"test_reg\", length: 4, bypass: 1001"),

  };

     
  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);

}

/*Test JTAG macro from Simplified ICL Tree input*/
void UT_reader::test_JTAG_TAP ()
{
  // ---------------- Exercise
  //

  //
  auto checker = [&](auto data)
  {
    auto input_SIT = std::get<0>(data);
    auto expected_PrettyPrinter = std::get<1>(data);
    
    // ---------------- Exercise & Verify
    //
      UT_reader_wrapper reader;
    auto result=reader.run_parser_for_UT(input_SIT,sm);
    auto actual_PrettyPrinter=result.first;
    TS_ASSERT_EQUALS (actual_PrettyPrinter, expected_PrettyPrinter);

   auto parent = dynamic_pointer_cast<ParentNode> (result.second);
   sm->ReplaceRoot(parent,false);
   auto check_result = sm->Check();

    auto   report         = check_result.MakeReport();
  TS_ASSERT_EQUALS (report, expectedReport);
  }; 
  
  auto data =
  { 
   make_tuple( "JTAG_TAP my_tap Loopback 4 2\
  {\
     REGISTER test_reg 4 Bypass: \"0b1100\"\
   }\n",
"[Access_I](1)  \"my_tap\"\n\
 [Register](2)  \"my_tap_IR\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](3)    \"my_tap_DR_Mux\"\n\
  :Selector:(2)  \"my_tap_IR\", kind: Table_Based, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](4)  \"my_tap_BPY\", length: 1, bypass: 1\n\
  [Register](0)  \"test_reg\", length: 4, bypass: 1100"),
   make_tuple( "JTAG_TAP my_tap SVF_simulation 4 2\
  {\
     REGISTER test_reg 4 Bypass: \"0b1100\"\
   }\n",
"[Access_I](1)  \"my_tap\"\n\
 [Register](2)  \"my_tap_IR\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](3)    \"my_tap_DR_Mux\"\n\
  :Selector:(2)  \"my_tap_IR\", kind: Table_Based, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](4)  \"my_tap_BPY\", length: 1, bypass: 1\n\
  [Register](0)  \"test_reg\", length: 4, bypass: 1100"),
   make_tuple( "JTAG_TAP my_tap SVF_openOCD 4 2\
  {\
     REGISTER test_reg 4 Bypass: \"0b1100\"\
   }\n",
"[Access_I](1)  \"my_tap\"\n\
 [Register](2)  \"my_tap_IR\", length: 4, Hold value: true, bypass: 1111\n\
 [Linker](3)    \"my_tap_DR_Mux\"\n\
  :Selector:(2)  \"my_tap_IR\", kind: Table_Based, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
  [Register](4)  \"my_tap_BPY\", length: 1, bypass: 1\n\
  [Register](0)  \"test_reg\", length: 4, bypass: 1100"),
 
 
 
   };

     
  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);

}

/*Test 1500 Wrapper macro from Simplified ICL Tree input*/
void UT_reader::test_1500 ()
{
  // ---------------- Exercise
  //

  //
  auto checker = [&](auto data)
  {
    auto input_SIT = std::get<0>(data);
    auto expected_PrettyPrinter = std::get<1>(data);
    
    // ---------------- Exercise & Verify
    //
      UT_reader_wrapper reader;
    auto result=reader.run_parser_for_UT(input_SIT,sm);
    auto actual_PrettyPrinter=result.first;
    TS_ASSERT_EQUALS (actual_PrettyPrinter, expected_PrettyPrinter);

   auto parent = dynamic_pointer_cast<ParentNode> (result.second);
   sm->ReplaceRoot(parent,false);
   auto check_result = sm->Check();

    auto   report         = check_result.MakeReport();
  TS_ASSERT_EQUALS (report, expectedReport);
  }; 
  
  auto data =
  { 
   make_tuple( "WRAPPER_1500 test_wrapper 2\
 { REGISTER test_register_1 12 Bypass: \"0b1001:0110:1100\"\
   REGISTER test_register_2 12 Bypass: \"0b1001:0110:1100\"}\n",
"[Chain](2)     \"test_wrapper\"\n\
 [Chain](4)     \"SWIR\"\n\
  [Register](3)  \"SWIR_ctrl\", length: 1, Hold value: true, bypass: 0\n\
  [Linker](5)    \"SWIR_mux\"\n\
   :Selector:(3)  \"SWIR_ctrl\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
   [Chain](7)     \"WIR\"\n\
    [Linker](8)    \"WIR_mux\"\n\
     :Selector:(6)  \"WIR_reg\", kind: Binary, can_select_none: 0, inverted_bits: 0, reversed_order: 0\n\
     [Register](9)  \"WBY\", length: 1, bypass: 0\n\
     [Register](0)  \"test_register_1\", length: 12, bypass: 1001_0110:1100\n\
     [Register](1)  \"test_register_2\", length: 12, bypass: 1001_0110:1100\n\
   [Register](6)  \"WIR_reg\", length: 2, Hold value: true, bypass: 00"),
 
 
   };

     
  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);

}
//===========================================================================
// End of UT_reader.cpp
//===========================================================================
