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

#include <string>
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
#include "Utility.hpp"
#include "MastConfig.hpp"

#include "g3log/g3log.hpp"

using ICL::ICL_Reader;
using Parsers::ParserException;
using Parsers::AST_Checker;
using Parsers::AST_SystemModelGenerator;

using std::vector;
using std::experimental::string_view;
using std::string;
using std::shared_ptr;

using namespace mast;
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


//! Creates a SystemModel sub-tree from list of files
//!
//! @note List can provide relative or absolute path or just file name (actual paths will be guessed from a set of directory path)
//! @note When no extension is provided, if a file cannot be found, an attempt will be made with ".icl" extension
//!
//! @param listFilePath File path for list of ICL files path (one per line)
//!
void ICL_Reader::CreateModelFromFiles (std::experimental::string_view listFilePath)
{
  CHECK_FILE_EXISTS(listFilePath.data());

  std::ifstream listStream(listFilePath.data());

  CHECK_TRUE  (listStream.good(), "Cannot open file: "s                  .append(listFilePath.cbegin(), listFilePath.cend()));
  CHECK_FALSE (listStream.eof(),  "List of ICL file shall not be empty"s .append(listFilePath.cbegin(), listFilePath.cend()));

  const auto& dirPaths = FilesSearchPaths();

  // Lamba: Log path of ICL file that will be parsed
  auto logParsedFile = [](string&& filePath)
  {
    LOG(INFO) << "Will parse ICL file \"" << filePath << "\"";
    return std::move(filePath);
  };

  // Lamba: When needed prefix file name with a path (from set of search path) until it can be opened
  auto resolveFilePath = [&dirPaths, &logParsedFile](string_view fileName) -> string
  {
    using namespace mast;
    if (mast::Utility::FileExists(fileName.data()))
    {
      return logParsedFile(string(fileName));
    }

    for (const auto& dirPath : dirPaths)
    {
      auto filePath = dirPath;
      filePath.append(DIRECTORY_SEPARATOR).append(fileName.cbegin(), fileName.cend());

      if (mast::Utility::FileExists(filePath))
      {
        return logParsedFile(std::move(filePath));
      }

      filePath.append(".icl");
      if (mast::Utility::FileExists(filePath))
      {
        return logParsedFile(std::move(filePath));
      }
    }

    CHECK_FAILED("Cannot find file with name \""s.append(fileName.cbegin(), fileName.cend()).append("\""));
  };


  // ---------------- Convert files names to path
  //
  vector<string> filesPath;
  string fileName;
  while (std::getline(listStream, fileName))
  {
    listStream >> fileName;

    string_view fileNameView(fileName);
    mast::Utility::TrimBoth(fileNameView);

    if (fileNameView.empty() || (fileNameView.front() == '#'))  // Ignore empty lines and comment only lines
    {
      continue;
    }

    filesPath.emplace_back(resolveFilePath(fileNameView));

    fileName.clear();
  }

  // ---------------- Do create model using files path
  //
  CreateModelFromFiles(filesPath);
}
//
//  End of: ICL_Reader::CreateModelFromFiles
//---------------------------------------------------------------------------



//! Creates a SystemModel sub-tree from list of files
//!
//! @param filesPaths   Paths of ICL files to parse in order to build a SystemModel sub-tree
//!
void ICL_Reader::CreateModelFromFiles (const vector<string>& filesPaths)
{
  // ---------------- Build an AST from all SIT files
  //
  for (const auto& filePath : filesPaths)
  {
    CHECK_PARAMETER_NOT_EMPTY(filePath, "Must specify a valid (not empty) file path");

    std::ifstream stream(filePath.data());

    CHECK_TRUE  (stream.good(), "Cannot open file: "s            .append(filePath.cbegin(), filePath.cend()));
    CHECK_FALSE (stream.eof(),  "Cannot parse empty ICL file: "s .append(filePath.cbegin(), filePath.cend()));

    try
    {
      UpdateAstFromIcl(stream);
    }
    catch(ParserException& exc)
    {
      PublicData().parsedTopNode.reset();
      if (exc.filePath.empty())
      {
        exc.filePath.append(filePath.cbegin(), filePath.cend());
        throw exc;
      }
      throw;
    }

    LOG(INFO) << "ICL file \"" << filePath << "\" is grammatically correct";
  }

  // ---------------- Create SystemModel from AST (constructed while parsing files)
  //
  if (!m_parseOnlyCheckGrammar)
  {
    auto checkResult = AST_Checker::Check(m_ast->Network());

    CHECK_VALUE_EMPTY(checkResult.IssuesReport(), "Errors have been detected while parsing ICL stream");

    UniquifyAST();
    auto& data = Reader::PublicData();
    data.parsedTopNode = GenerateSystemModelNodes(m_ast.get());
  }

  LOG(INFO) << "ICL files have been parsed successfully";
}
//
//  End of: ICL_Reader::CreateModelFromFiles
//---------------------------------------------------------------------------


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
  generator.FilesSearchPaths(FilesSearchPaths());

  auto network = ast->Network();
  auto topNode = generator.Generate(network);

  PublicData().namesAndNodes = std::move(generator.Moveable_AlgorithmAssociations());
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
