//===========================================================================
//                           ModelBuildDriver.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ModelBuildDriver.hpp
//!
//! Declares ModelBuildDriver class
//!
//===========================================================================


#ifndef MODELBUILDDRIVER_H__8EE800A3_78A6_4A80_FF94_BA88795AA232__INCLUDED_
  #define MODELBUILDDRIVER_H__8EE800A3_78A6_4A80_FF94_BA88795AA232__INCLUDED_

#include <string>
#include <experimental/string_view>
#include <vector>
#include <memory>

namespace mast
{
class AppFunctionNameAndNode;
class SystemModel;
class ParentNode;

//! Drives the building of a MAST SystemModel
//!
//! @note The model can be build from SIT or ICL file(s) and optionally factories (registered by plugins)
//!
class ModelBuildDriver final
{
  // ---------------- Public Methods
  //
  public:
  ~ModelBuildDriver();
  ModelBuildDriver();

  //! Create MAST system model starting from a ICL file
  //!
  std::shared_ptr<SystemModel> CreateModelFromIclFile(const std::string& iclFilePath);

  //! Create MAST system model starting from a ICL file
  //!
  std::shared_ptr<SystemModel> CreateModelFromIclFileList(const std::string& iclListFilePath);

  //! Create MAST system model starting from a SIT file
  //!
  std::shared_ptr<SystemModel> CreateModelFromSitFile(const std::string& sitFilePath);

  //! Associations of PDL algorithm names to nodes
  //!
  const std::vector<AppFunctionNameAndNode>& PDLAlgorithmNameToNodeAssociation() const { return m_algoNamesAssociatedToNodes; }

  //! Returns error message in case of failure, empty string otherwise
  //!
  std::string ErrorMessage() const { return m_errorMessage; }

  // ---------------- Private Methods
  //
  private:

  void                        AppendToSearchPath          (const std::string& filePath);
  std::string                 AssessActualProjectFilePath (const std::string& projectFileName, std::experimental::string_view defaultFileExtension);
  std::shared_ptr<ParentNode> ParseIclFile                (const std::string& iclFilePath);
  std::shared_ptr<ParentNode> ParseIclFiles               (const std::string& iclListFilePath);
  std::shared_ptr<ParentNode> ParseSitFile                (const std::string& sitFilePath);

  // ---------------- Private Fields
  //
  private:
  std::shared_ptr<SystemModel>        m_systemModel;                //!< Build MAST system model
  std::vector<std::string>            m_searchPaths;                //!< Paths from where to search for SIT or ICL files (updated when got relative paths)
  std::vector<AppFunctionNameAndNode> m_algoNamesAssociatedToNodes; //!< Associations of PDL algorithm names to nodes
  std::string                         m_errorMessage;               //!< Error message build while parsing SIT or ICL (empty when successful)
};
//
//  End of ModelBuildDriver class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined MODELBUILDDRIVER_H__8EE800A3_78A6_4A80_FF94_BA88795AA232__INCLUDED_

//===========================================================================
// End of ModelBuildDriver.hpp
//===========================================================================
