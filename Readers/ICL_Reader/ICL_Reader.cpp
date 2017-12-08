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
#include "AST_Checker.hpp"
#include "AST_SystemModelGenerator.hpp"

#include "g3log/g3log.hpp"

using ICL::ICL_Reader;
using Parsers::AST_Checker;
using Parsers::AST_SystemModelGenerator;

using std::experimental::string_view;
using std::string;
using std::shared_ptr;

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



//! Generates Mast SystemModel from AST
//!
//! @param ast  AST representing the model (usually from parsing ICL files)
//!
//! @return Top node of generated SystemModel (sub-)tree
//!
shared_ptr<mast::SystemModelNode> ICL_Reader::GenerateSystemModelNodes (Parsers::AST* ast)
{
  auto systemModel = PublicData().systemModel;

  AST_SystemModelGenerator generator(systemModel);

  auto network = ast->Network();
  auto topNode = generator.Generate(network);

  return topNode;
}
//
//  End of: ICL_Reader::GenerateSystemModelNodes
//---------------------------------------------------------------------------



//! Parses an excerpt to construct a SystemModel
//!
//! @param stream  Stream to get excerpt to parse from
//!
void ICL_Reader::Parse_Impl(std::istream& stream)
{
  CHECK_TRUE  (stream.good(), "Invalid ICL stream");
  CHECK_FALSE (stream.eof(),  "Cannot parse ICL from empty stream");

  UpdateAstFromIcl(stream);

  if (!m_parseOnlyCheckGrammar)
  {
    auto checkResult = AST_Checker::Check(m_ast->Network());

    CHECK_VALUE_EMPTY(checkResult.IssuesReport(), "Errors have been detected while parsing ICL stream");

    UniquifyAST();
    auto& data = Reader::PublicData();
    data.parsedTopNode = GenerateSystemModelNodes(m_ast.get());
  }

  LOG(INFO) << "ICL has been parsed successfully";
}
//
//  End of Parse_Impl
//---------------------------------------------------------------------------



//! Uniquifies all modules in AST network
//!
void ICL_Reader::UniquifyAST ()
{
//+  CHECK_VALUE_NOT_NULL(m_ast, "An AST must have been created using UpdateAstFromIcl");
  m_ast->Uniquify();
}
//
//  End of: ICL_Reader::UniquifyAST
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
