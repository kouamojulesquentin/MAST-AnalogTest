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

#include <string>
#include <experimental/string_view>
#include <iostream>
#include <sstream>
#include "SIT_reader.hpp"
#include "PrettyPrinter.hpp"
#include "UT_reader_wrapper.hpp"
#include "PrettyPrinter.hpp"
#include "../Use_Cases/UT_reader.hpp"

using std::string;
using std::experimental::string_view;
using std::pair;
using std::make_pair;
using std::shared_ptr;

using namespace test;
using namespace mast;
using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;

pair<string, std::shared_ptr<mast::SystemModelNode>> UT_reader_wrapper::run_parser_for_UT(const string& input_SIT, std::shared_ptr<mast::SystemModel> sm)
{
  std::stringstream stream(input_SIT);
  SIT::SIT_Reader driver(sm);

  auto succeeded = driver.parse(stream);

  if (!succeeded)
  {
    return make_pair(""s, driver.parsed_sut);
  }

  auto actual_PrettyPrint = PrettyPrinter::PrettyPrint(driver.parsed_sut, PrettyPrinterOptions::Parser_debug);

  return make_pair(std::move(actual_PrettyPrint), driver.parsed_sut);
}



//===========================================================================
// End of UT_reader_wrapper.cpp
//===========================================================================
