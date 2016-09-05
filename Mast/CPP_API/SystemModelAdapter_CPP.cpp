//===========================================================================
//                           SystemModelAdapter_CPP.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelAdapter_CPP.cpp
//!
//! Implements 'CPP' language API to system model
//===========================================================================

#include "SystemModelAdapter_CPP.h"
#include "SystemModelManager.hpp"
#include "Commons_C.hpp"
#include "Startup.hpp"
#include "Utility.hpp"
#include "Session.hpp"
#include "SIT_reader.hpp"

#include <memory>

using std::shared_ptr;
using std::vector;
using std::string;
using std::experimental::string_view;
using std::dynamic_pointer_cast;
using namespace std::string_literals;
using namespace mast;


namespace
{
inline auto GetAndCheckManager()
{
  auto manager = Startup::GetManager();
  CHECK_VALUE_NOT_NULL(manager, "Mast library has not been properly initialized");\
  return manager;
}

//! Loads model from "SIT" file and set it as root of SystemModel
//!
void LoadSystemModel (string_view filePath)
{
  CHECK_PARAMETER_NOT_EMPTY(filePath, "Expect valid file path");
  CHECK_FILE_EXISTS(filePath);

  auto sm     = Startup::GetSystemModel();
  auto reader = SIT::SIT_Reader(sm);

  reader.parse(filePath);

  auto topNode = dynamic_pointer_cast<ParentNode>(reader.parsed_sut);

  sm->ReplaceRoot(topNode, false);
}
//
//  End of: LoadSystemModel
//---------------------------------------------------------------------------


} // End of unnamed namespace


//!< Cleans up Mast library
//!<
//!< @note  To start using Mast again (e.g. with a new model), one must call InitializeMast again
//!<
void mast::CleanupMast ()
{
  mast::Startup::ForgetManager();
  mast::Startup::ForgetSystemModel();
}
//
//  End of: mast::CleanupMast
//---------------------------------------------------------------------------



//! Registers applications functions with their associated node to the System Model Manager
//!
void mast::CreateApplications (const vector<ApplicationAssociation>& appAssociations)
{
  auto sm      = mast::Startup::GetSystemModel();
  auto manager = mast::Startup::GetManager();

  for (const auto& nextAssociation : appAssociations)
  {
    CHECK_PARAMETER_NOT_EMPTY(nextAssociation.topNodePath, "An empty node path is not valid to associate an application");

    auto foundNode = sm->Root()->FindNode(nextAssociation.topNodePath);
    auto topNode   = std::dynamic_pointer_cast<ParentNode>(foundNode);

    CHECK_VALUE_NOT_NULL(topNode,   "Cannot create application "                     + Utility::IfNotEmpty_SingleQuoteAndSuffixWithSpace(nextAssociation.debugName)
                                  + "because cannot find an actual node with path: " + Utility::SingleQuote(nextAssociation.topNodePath));

    manager->CreateApplicationThread(topNode, nextAssociation.function, nextAssociation.debugName);
  }
}
//
//  End of: mast::CreateApplications
//---------------------------------------------------------------------------



//! Starts up mast library, building model using specified file
//!
//! @note This function is not thread safe: It must be call by only one thread prior to any usage of mast library
//!       This function must be called only after main has been call by the runtime system
//!
void mast::InitializeMast (string_view modelFilePath)
{
  // Those 2 lines do the initialization
  /* unused */ Startup::GetSystemModel();
  /* unused */ Startup::GetManager();

  LoadSystemModel(modelFilePath);
}
//
//  End of: mast::InitializeMast
//---------------------------------------------------------------------------


//! Runs Mast til applications terminates
//!
void mast::RunMast (string_view modelFilePath, const vector<ApplicationAssociation>& appAssociations)
{
  CHECK_PARAMETER_NOT_EMPTY(modelFilePath, "Cannot run Mast without a valid path for system model (SIT file)");
  CHECK_FILE_EXISTS(modelFilePath);

  Session session;
  auto manager = mast::Startup::GetManager();

  CreateApplications(appAssociations);

  manager->Start();
  manager->StartCreatedApplicationThreads();
  manager->WaitForApplicationsEnd();
  manager->Stop();
}
//
//  End of: mast::RunMast
//---------------------------------------------------------------------------


//! Starts System Model Manager
//!
void mast::Start ()
{
  auto manager = GetAndCheckManager();
  manager->Start();
}
//
//  End of: mast::Start
//---------------------------------------------------------------------------


//! Stops System Model Manager
//!
void mast::Stop ()
{
  auto manager = GetAndCheckManager();
  manager->Stop();
}
//
//  End of: mast::Stop
//---------------------------------------------------------------------------


//! Waits (blocks) until all application threads terminate (on their own or by a mechanism unknown to mast manager)
//!
void mast::WaitForApplicationsEnd ()
{
  auto manager = GetAndCheckManager();
  manager->WaitForApplicationsEnd();
}
//
//  End of: mast::WaitForApplicationsEnd
//---------------------------------------------------------------------------


//===========================================================================
// End of SystemModelAdapter_CPP.cpp
//===========================================================================
