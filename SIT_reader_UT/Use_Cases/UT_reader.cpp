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
 
//! Initializes tests (called for each test)
//!
void UT_reader::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped
 /*Faire un reset des identifiants, regarder les UT du SystemModel*/
}


//! Checks GmlPrinterVisitor constructor
//!
void UT_reader::test_reader ()
{
  // ---------------- Exercise
  //

  //
  auto checker = [](auto data)
  {
    auto input_SIT = std::get<0>(data);
    auto expected_PrettyPrinter = std::get<1>(data);
    
    // ---------------- Exercise & Verify
    //
      UT_reader_wrapper reader;
    auto actual_PrettyPrinter=reader.run_parser_for_UT(input_SIT);
    TS_ASSERT_EQUALS (actual_PrettyPrinter, expected_PrettyPrinter);
  }; 
  
  auto data =
  {
   make_tuple( "REGISTER test_register 12 Bypass: \"0b1001:0110:1100\"\n",   "[Register](0)  \"test_register\", length: 12, bypass: 1001_0110:1100"), 
  };

     
  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);

}



//===========================================================================
// End of UT_reader.cpp
//===========================================================================
