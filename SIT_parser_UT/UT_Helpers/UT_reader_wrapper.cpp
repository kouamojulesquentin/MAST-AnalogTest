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
#include <iostream>
#include <sstream>
#include "SIT_reader.hpp"
#include "PrettyPrinterVisitor.hpp"
#include "UT_reader_wrapper.hpp"
#include "PrettyPrinterVisitor.hpp"

using std::string;
using std::experimental::string_view;

using namespace test;

//! Initializes tests (called for each test)
//!
void UT_reader_wrapper::setUp ()
{

}


string UT_reader_wrapper::run_parser_for_UT(string input_SIT)
{
  // ---------------- Exercise
  //
//  GmlPrinterVisitor sut;

  // ---------------- Verify
  //
      SIT::SIT_Reader driver;
      driver.parsed_sut = std::make_unique<mast::SystemModel>();

   std::stringstream stream;
  
 
     stream << input_SIT ;

  driver.parse(stream);

   PrettyPrinterVisitor prettyPrinter;
   driver.parsed_sut->Root()->Accept(prettyPrinter);
   auto actual_PrettyPrinter      = prettyPrinter.PrettyPrint();

  return actual_PrettyPrinter;
}



//===========================================================================
// End of UT_reader_wrapper.cpp
//===========================================================================
