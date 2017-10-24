//===========================================================================
//                           Reader.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Reader.cpp
//!
//! Implements class Reader
//!
//===========================================================================

#include "Reader.hpp"
#include "ParserException.hpp"
#include "Utility.hpp"

#include "g3log/g3log.hpp"

#include <experimental/string_view>
#include <fstream>
#include <memory>

using std::experimental::string_view;
using std::string;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;
using namespace mast;


Parsers::Reader::Reader(std::shared_ptr<mast::SystemModel> sm)
{
  m_publicData.systemModel = sm;
}

void Parsers::Reader::Parse(string_view filename)
{
  CHECK_PARAMETER_NOT_EMPTY(filename, "Must specify a valid file path");

  std::ifstream iclFileStream(filename.data());

  CHECK_TRUE  (iclFileStream.good(), "Cannot open file: "s            .append(filename.cbegin(), filename.cend()));
  CHECK_FALSE (iclFileStream.eof(),  "Cannot parse empty ICL file: "s .append(filename.cbegin(), filename.cend()));

  try
  {
    Parse_Impl(iclFileStream);
  }
  catch(ParserException& exc)
  {
    m_publicData.parsedTopNode.reset();
    if (exc.filePath.empty())
    {
      exc.filePath.append(filename.cbegin(), filename.cend());
      throw exc;
    }
    throw;
  }
}

//! Parses an excerpt to construct a SystemModel
//!
//! @param stream  Stream to get excerpt to parse from
//!
void Parsers::Reader::ParseExcerpt(std::istream& stream)
{
  CHECK_TRUE  (stream.good(), "Invalid stream");
  CHECK_FALSE (stream.eof(),  "Cannot parse empty stream");

  try
  {
    Parse_Impl(stream);
  }
  catch(ParserException& exc)
  {
    m_errorMessage = exc.Message();
    throw;
  }
}


//! Parses an excerpt to construct a SystemModel
//!
//! @param excerpt  Excerpt to parse
//!
void Parsers::Reader::ParseExcerpt(const string& excerpt)
{
  CHECK_PARAMETER_NOT_EMPTY(excerpt, "Cannot parse empty string");

  try
  {
    std::stringstream stream(excerpt);
    Parse_Impl(stream);
  }
  catch(ParserException& exc)
  {
    m_errorMessage = exc.Message();
    throw;
  }
}

//===========================================================================
// End of Reader.cpp
//===========================================================================
