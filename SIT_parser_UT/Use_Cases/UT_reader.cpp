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
#include "SIT_reader.hpp"
#include "PrettyPrinterVisitor.hpp"
#include "UT_reader.hpp"
#include "UT_reader_wrapper.hpp"

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
  auto checker = [](string input_SIT, string expected_PrettyPrinter)
  {
    // ---------------- Exercise & Verify
    //
      UT_reader_wrapper reader;
    auto actual_PrettyPrinter=reader.run_parser_for_UT(input_SIT);
    TS_ASSERT_EQUALS (actual_PrettyPrinter, expected_PrettyPrinter);
  }; 
  
    auto input_SITs =
     {
      "REGISTER Gaspare 12 Bypass: \"0b12\"\n",
      "REGISTER Gaspare 12 Bypass: \"0b1\"\n",
     };
 
    auto expected_PrettyPrinters =
     {
      "",
      "",
     };
     /*vore example des tuple pour lecture plus facile: input+expected meme ligne*/
     
  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, input_SITs, expected_PrettyPrinters);

}



//===========================================================================
// End of UT_reader.cpp
//===========================================================================
