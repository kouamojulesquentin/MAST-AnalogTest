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
#include "../Use_Cases/UT_reader.hpp"

using std::string;
using std::experimental::string_view;

using namespace test;


string UT_reader_wrapper::run_parser_for_UT(string input_SIT, std::shared_ptr<mast::SystemModel> sm)
{
  // ---------------- Exercise
  //
//  GmlPrinterVisitor sut;

  // ---------------- Verify
  //
      SIT::SIT_Reader driver(sm);

   std::stringstream stream;
  
 
     stream << input_SIT ;

   auto result = driver.parse(stream);

   if (result == false) return "PARSING ERROR";
   
   /*Regarder les appels de PrettyPrinter sans "accept"
   prettyPrinter::xxxquelque chosexxx qui retourne le string*/
   PrettyPrinterVisitor 
           prettyPrinter(PrettyPrinterOptions::ShowSelectorProperties);

   driver.parsed_sut->Accept(prettyPrinter);
   auto actual_PrettyPrint      = prettyPrinter.PrettyPrint();

   
  return actual_PrettyPrint;
}



//===========================================================================
// End of UT_reader_wrapper.cpp
//===========================================================================
