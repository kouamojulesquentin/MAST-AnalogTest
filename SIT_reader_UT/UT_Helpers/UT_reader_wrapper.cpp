//===========================================================================
//                           UT_GmlPrinter.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_GmlPrinter.cpp
//!
//! Implements test fixture for testing GmlPrinter
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
using std::pair;
using std::make_pair;
using std::shared_ptr;

using namespace test;
using namespace mast;


pair<string, std::shared_ptr<mast::SystemModelNode>> UT_reader_wrapper::run_parser_for_UT(const string& input_SIT, std::shared_ptr<mast::SystemModel> sm)
{
  std::stringstream stream(input_SIT);
  SIT::SIT_Reader driver(sm);

  auto parse_result = driver.parse(stream);

  if (parse_result == false) {
    return make_pair("PARSING ERROR", shared_ptr<SystemModelNode>(nullptr));
  }

  auto actual_PrettyPrint = PrettyPrinterVisitor::PrettyPrint(driver.parsed_sut, PrettyPrinterOptions::Parser_debug);

  return make_pair(actual_PrettyPrint, driver.parsed_sut);
}



//===========================================================================
// End of UT_reader_wrapper.cpp
//===========================================================================
