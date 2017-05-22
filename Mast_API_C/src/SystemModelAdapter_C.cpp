//===========================================================================
//                           SystemModelAdapter_C.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelAdapter_C.cpp
//!
//! Implements 'C' language API to system model
//===========================================================================

#include "SystemModelAdapter_C.h"
#include "SystemModelManager.hpp"
#include "SystemModel.hpp"
#include "Commons_C.hpp"
#include "Startup.hpp"
#include "Utility.hpp"
#include "Session.hpp"
#include "SIT_reader.hpp"
#include "g3log/g3log.hpp"

#include <memory>
#include <string>

using std::shared_ptr;
using std::string;

using namespace std::string_literals;
using std::experimental::string_view;
using std::dynamic_pointer_cast;
using namespace mast;


namespace
{
thread_local string thread_error_message;


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
//!< @note  To start using Mast (e.g. with a new model), one must call InitializeMast again
//!<
ErrorCode CleanupMast ()
{
  auto retCode = ErrorCode::Ok;

  TRY_CATCH_ALL(retCode,
                LOG(INFO) << "Cleaning MAST Core library";
                mast::Startup::ForgetManager();
                mast::Startup::ForgetSystemModel();
               );

  return retCode;
}
//
//  End of: CleanupMast
//---------------------------------------------------------------------------


//! Clears last error message
//!
void ClearErrorMessage()
{
  thread_error_message.clear();
}


//! Sets error message
//!
//! @note This is normally used only by library internals
void SetErrorMessage(const char* message)
{
  thread_error_message = message;
}


//! Returns error message from last command
//!
//! @return Pointer to the message string (only valid till no other command is issued)
const char* ErrorMessage ()
{
  return thread_error_message.c_str();
}


//! Registers applications functions with their associated node to the System Model Manager
//!
ErrorCode CreateApplications (AppFunctionAndNodePath* pAssociations, uint32_t count)
{
  auto retCode = ErrorCode::Ok;

  try
  {
    ClearErrorMessage();

    auto sm      = mast::Startup::GetSystemModel();
    auto manager = mast::Startup::GetManager();

    for (uint32_t ii = 0 ; ii < count ; ++ii)
    {
      const auto& nextAssociation = pAssociations[count];

      CHECK_VALUE_NOT_NULL(nextAssociation.function,    "Invalid nullptr for application function");
      CHECK_VALUE_NOT_NULL(nextAssociation.topNodePath, "Invalid nullptr for application top node path");

      auto foundNode = sm->Root()->FindNode(nextAssociation.topNodePath);
      auto topNode   = std::dynamic_pointer_cast<ParentNode>(foundNode);

      manager->CreateApplicationThread(topNode, nextAssociation.function, nextAssociation.debugName);
    }
  }
  CATCH_ALL(retCode)

  return retCode;
}
//
//  End of: CreateApplications
//---------------------------------------------------------------------------



//! Starts up mast library, building model using specified file
//!
//! @note This function is not thread safe: It must be call by only one thread prior to any usage of mast library
//!       This function must be called only after main has been call by the runtime system
//!
ErrorCode InitializeMast (const char* modelFilePath)
{
  auto retCode = ErrorCode::Ok;

  TRY_CATCH_ALL(retCode,

    // Those 2 lines do the initialization
    /* unused */ Startup::GetSystemModel();
    /* unused */ Startup::GetManager();

    if (modelFilePath != nullptr)
    {
      LoadSystemModel(modelFilePath);
    }
  );

  return retCode;
}
//
//  End of: InitializeMast
//---------------------------------------------------------------------------


//! Runs Mast til applications terminates
//!
ErrorCode RunMast (const char* modelFilePath, AppFunctionAndNodePath* pAssociations, uint32_t count)
{
  auto retCode = ErrorCode::Ok;

  try
  {
    Session session;

    CHECK_PARAMETER_NOT_NULL(modelFilePath, "Cannot run Mast without a valid path for system model (SIT file)");
    CHECK_FILE_EXISTS(modelFilePath);

    retCode = CreateApplications(pAssociations, count);
    if (retCode != ErrorCode::Ok) return retCode;

    retCode = Start();
    if (retCode != ErrorCode::Ok) return retCode;

    retCode = WaitForApplicationsEnd();
  }
  CATCH_ALL(retCode)

  return retCode;
}
//
//  End of: RunMast
//---------------------------------------------------------------------------


//! Starts System Model Manager
//!
ErrorCode Start ()
{
  auto retCode = ErrorCode::Ok;

  TRY_CATCH_ALL(retCode,
    auto manager = mast::Startup::GetManager();
    manager->Start();
  );

  return retCode;
}
//
//  End of: Start
//---------------------------------------------------------------------------


//! Waits (blocks) until all application thread terminates (on their own or by a mechanism unknown to mast manager)
//!
ErrorCode WaitForApplicationsEnd ()
{
  auto retCode = ErrorCode::Ok;

  TRY_CATCH_ALL(retCode,
    auto manager = mast::Startup::GetManager();
    manager->WaitForApplicationsEnd();
  );

  return retCode;
}
//
//  End of: Start
//---------------------------------------------------------------------------


//===========================================================================
// End of SystemModelAdapter_C.cpp
//===========================================================================
