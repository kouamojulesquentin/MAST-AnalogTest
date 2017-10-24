//===========================================================================
//                           ICL_Reader.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ICL_Reader.cpp
//!
//! Implements class ICL_Reader
//!
//===========================================================================

#include <experimental/string_view>
#include <fstream>
#include <memory>

#include "ICL_Reader.hpp"
#include "ICL_Parser.tab.hh"
#include "ICL_Scanner.hpp"
#include "ParserException.hpp"
#include "Parser_PrivateData.hpp"
#include "Utility.hpp"

#include "g3log/g3log.hpp"

using std::experimental::string_view;
using std::string;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;


ICL::ICL_Reader::ICL_Reader(std::shared_ptr<mast::SystemModel> sm)
  : Reader (sm)
{
}

void ICL::ICL_Reader::Parse_Impl(std::istream& stream)
{
  CHECK_TRUE  (stream.good(), "Invalid ICL stream");
  CHECK_FALSE (stream.eof(),  "Cannot parse ICL from empty stream");

  Parsers::Parser_PrivateData privateData(PublicData().systemModel);

  ICL_Scanner scanner(stream);
  ICL_Parser  parser(scanner, *this /* driver */, PublicData(), privateData);

  auto succeeded = parser.parse() == 0;

  CHECK_TRUE(succeeded, "Failed to parse ICL stream");

  LOG(INFO) << "ICL has been parsed successfully";
}

//===========================================================================
// End of ICL_Reader.cpp
//===========================================================================
