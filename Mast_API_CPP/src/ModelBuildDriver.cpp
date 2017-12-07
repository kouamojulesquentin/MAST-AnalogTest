//===========================================================================
//                           ModelBuildDriver.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ModelBuildDriver.cpp
//!
//! Implements class ModelBuildDriver
//!
//===========================================================================

#include "ModelBuildDriver.hpp"
#include "SystemModel.hpp"
#include "SystemModelFactory.hpp"
#include "ParentNode.hpp"
#include "ICL_Reader.hpp"
#include "SIT_Reader.hpp"
#include "Utility.hpp"
#include "MastConfig.hpp"
#include "ParserException.hpp"

#include "g3log/g3log.hpp"

using std::string;
using std::experimental::string_view;
using std::vector;
using std::shared_ptr;

using std::make_shared;
using std::dynamic_pointer_cast;

using namespace std::string_literals;
using namespace mast;


//! Releases resources
//!
ModelBuildDriver::~ModelBuildDriver ()
{
}
//
//  End of: ModelBuildDriver::~ModelBuildDriver
//---------------------------------------------------------------------------


//! Initializes members
//!
ModelBuildDriver::ModelBuildDriver ()
  : m_sitSearchPaths {""} // This is to have opportunity to check file existence just by appending ".sit" to file name
{
}
//
//  End of: ModelBuildDriver::ModelBuildDriver
//---------------------------------------------------------------------------

//! If path has not been seen yet, appends it to search paths
//!
//! @param filePath   A file path
//!
void ModelBuildDriver::AppendToSearchPath (const string& filePath)
{
  if (filePath.empty())
  {
    return;
  }

  auto path            = Utility::ExtractDirectoryPath(filePath);
  auto notInSearchPath = std::find(m_sitSearchPaths.begin(), m_sitSearchPaths.end(), path) == m_sitSearchPaths.end();

  if (notInSearchPath)
  {
    m_sitSearchPaths.emplace_back(path);
  }
}
//
//  End of: ModelBuildDriver::AppendToSearchPath
//---------------------------------------------------------------------------


//! Tries to find actual project file path from its name
//!
//! @param projectFileName  A project file name (or path)
//!
//! @return projectFileName if it represents a file that can be open from current working directory,
//!         first found existing path otherwise
//!
string ModelBuildDriver::AssessActualProjectFilePath (const string& projectFileName, string_view defaultFileExtension)
{
  AppendToSearchPath(projectFileName);
  if (Utility::FileExists(projectFileName))
  {
    return projectFileName;
  }

  for (const auto& hintDir : m_sitSearchPaths)
  {
    auto dirPath = hintDir.empty() ? projectFileName
                                   : hintDir + DIRECTORY_SEPARATOR + projectFileName;
    // Try with hint directory path
    if (Utility::FileExists(dirPath))
    {
      AppendToSearchPath(dirPath);
      return dirPath;
    }

    // Try with default extension
    dirPath.append(defaultFileExtension.cbegin(), defaultFileExtension.cend());
    if (Utility::FileExists(dirPath))
    {
      AppendToSearchPath(dirPath);
      return dirPath;
    }
  }

  return projectFileName;
}
//
//  End of: ModelBuildDriver::AssessActualProjectFilePath
//---------------------------------------------------------------------------


//! Create MAST system model starting from a ICL file
//!
//! @param iclFilePath  ICL file path
//!
//! @return Build MAST system model
//!
shared_ptr<SystemModel> ModelBuildDriver::CreateModelFromIclFile (const string& iclFilePath)
{
  CHECK_PARAMETER_NOT_EMPTY(iclFilePath, "Must specify a valid ICL file path");

  m_systemModel = make_shared<SystemModel>();

  auto topNode = ParseIclFile(AssessActualProjectFilePath(iclFilePath, ".icl"));

  m_systemModel->ReplaceRoot(topNode, false);

  return m_systemModel;
}
//
//  End of: ModelBuildDriver::CreateModelFromIclFile
//---------------------------------------------------------------------------



//! Create MAST system model starting from a SIT file
//!
//! @param sitFilePath  SIT file path
//!
//! @return Build MAST system model
//!
shared_ptr<SystemModel> ModelBuildDriver::CreateModelFromSitFile (const string& sitFilePath)
{
  CHECK_PARAMETER_NOT_EMPTY(sitFilePath, "Must specify a valid SIT file path");

  m_systemModel = make_shared<SystemModel>();

  auto topNode = ParseSitFile(AssessActualProjectFilePath(sitFilePath, ".sit"));

  m_systemModel->ReplaceRoot(topNode, false);

  return m_systemModel;
}
//
//  End of: ModelBuildDriver::CreateModelFromSitFile
//---------------------------------------------------------------------------


//! Create MAST sub-model from a ICL file
//!
//! @param iclFilePath  ICL file path
//!
//! @return top node of build sub-model
//!
shared_ptr<ParentNode> ModelBuildDriver::ParseIclFile (const string& iclFilePath)
{
  CHECK_PARAMETER_NOT_EMPTY(iclFilePath, "ICL file path must not be empty");

  ICL::ICL_Reader reader(m_systemModel);

  try
  {
    reader.Parse(iclFilePath);
  }
  catch(mast::ParserException& exc)
  {
    m_errorMessage = reader.ErrorMessage();
    if (m_errorMessage.empty())
    {
      m_errorMessage = exc.Message();
    }
    LOG(ERROR_LVL) << "Failed to create model from ICL file \"" << iclFilePath << "\". " << m_errorMessage;
    throw;
  }

  auto topNode = dynamic_pointer_cast<ParentNode>(reader.ParsedSystemModel());

  CHECK_VALUE_NOT_NULL(topNode, "Failed to parse file: "s.append(iclFilePath).append(" (no top node)"));
  LOG(INFO) << "ICL file \"" << iclFilePath << "\" has been parsed successfully";

  // ---------------- Save PDL algorithm associations with nodes
  //
  const auto& associations = reader.PDLAlgorithmNameToNodeAssociation();

  m_algoNamesAssociatedToNodes.insert(m_algoNamesAssociatedToNodes.end(),
                                      associations.begin(),
                                      associations.end());

  return topNode;
}
//
//  End of: ModelBuildDriver::ParseIclFile
//---------------------------------------------------------------------------



//! Create MAST sub-model from a SIT file
//!
//! @param sitFilePath  SIT file path
//!
//! @return top node of build sub-model
//!
shared_ptr<ParentNode> ModelBuildDriver::ParseSitFile (const string& sitFilePath)
{
  CHECK_PARAMETER_NOT_EMPTY(sitFilePath, "SIT file path must not be empty");
  auto reader = SIT::SIT_Reader(m_systemModel);

  try
  {
    reader.Parse(sitFilePath);
  }
  catch(ParserException&)
  {
    m_errorMessage = reader.ErrorMessage();
    LOG(ERROR_LVL) << "Failed to create model from SIT file \"" << sitFilePath << "\". " << m_errorMessage;
    throw;
  }


  auto topNode = dynamic_pointer_cast<ParentNode>(reader.ParsedSystemModel());

  CHECK_VALUE_NOT_NULL(topNode, "Failed to parse file: "s + sitFilePath);
  LOG(INFO) << "SIT file \"" << sitFilePath << "\" has been parsed successfully";

  // ---------------- Save PDL algorithm associations with nodes
  //
  const auto& associations = reader.PDLAlgorithmNameToNodeAssociation();

  m_algoNamesAssociatedToNodes.insert(m_algoNamesAssociatedToNodes.end(),
                                      associations.begin(),
                                      associations.end());

  // ---------------- Manage hierarchical model
  //
  const auto& placeHolders = reader.PlaceHolders();
  for (const auto& placeHolder : placeHolders)
  {
    auto        localTop = placeHolder.Parent();
    const auto& identifier  = placeHolder.Identifier();

    shared_ptr<ParentNode> subTop;

    if (placeHolder.Kind() == PlaceHolderKind::SIT)
    {
      subTop = ParseSitFile(AssessActualProjectFilePath(identifier, ".sit"));
    }
    else
    {
      CHECK_VALUE_EQ(placeHolder.Kind(), PlaceHolderKind::Factory, "Internal Error: Unsupported PlaceHolderKind");
      const auto& factory = SystemModelFactory::Instance();

      subTop = factory.Create(identifier, *m_systemModel);
    }
    localTop->AppendChild(subTop);
  }

  return topNode;
}
//
//  End of: ModelBuildDriver::ParseSitFile
//---------------------------------------------------------------------------





//===========================================================================
// End of ModelBuildDriver.cpp
//===========================================================================
