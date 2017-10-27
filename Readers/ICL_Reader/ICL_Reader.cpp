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
#include "AST.hpp"

#include "g3log/g3log.hpp"

using ICL::ICL_Reader;

using std::experimental::string_view;
using std::string;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;

//! Release resources
//!
ICL_Reader::~ICL_Reader ()
{
}


//! Initializes instance
//!
ICL_Reader::ICL_Reader(std::shared_ptr<mast::SystemModel> sm)
  : Reader (sm)
{
}

//! Parses an excerpt to construct a SystemModel
//!
//! @param stream  Stream to get excerpt to parse from
//!
void ICL_Reader::Parse_Impl(std::istream& stream)
{
  CHECK_TRUE  (stream.good(), "Invalid ICL stream");
  CHECK_FALSE (stream.eof(),  "Cannot parse ICL from empty stream");

  UpdateAstFromIcl(stream);

  LOG(INFO) << "ICL has been parsed successfully";
}
//
//  End of Parse_Impl
//---------------------------------------------------------------------------



//! Parses an ICL fragment to update current AST
//!
//! @note Supports multiple ICL parsing before converting to SystemModel
//!
//! @param stream  Stream to get excerpt to parse from
//!
void ICL_Reader::UpdateAstFromIcl (std::istream& stream)
{
  if (m_ast == nullptr)
  {
    m_ast = std::make_unique<Parsers::AST>();
  }

  ICL_Scanner scanner(stream);
  ICL_Parser  parser(scanner, *m_ast);

  auto succeeded = parser.parse() == 0;

  CHECK_TRUE(succeeded, "Failed to parse ICL stream");
}
//
//  End of: ICL_Reader::UpdateAstFromIcl
//---------------------------------------------------------------------------


//===========================================================================
// End of ICL_Reader.cpp
//===========================================================================
