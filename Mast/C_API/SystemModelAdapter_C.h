//===========================================================================
//                           SystemModelAdapter_C.h
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelAdapter_C.h
//!
//! Declares 'C' language API to system model
//===========================================================================

#ifndef SYSTEMMODELADAPTER_C_H__1D5A30E_BBA2_4C6D_B7B4_5077996B2B04__INCLUDED_
  #define SYSTEMMODELADAPTER_C_H__1D5A30E_BBA2_4C6D_B7B4_5077996B2B04__INCLUDED_

#include "Platform.hpp"
#include "ErrorCode_C.h"

#include <stdint.h>
extern "C"
{
  typedef void(*PDLApplication_t)();

  struct AppFunctionAndNodePath
  {
    PDLApplication_t function;
    const char*      topNodePath;
    const char*      debugName;
  };

  //! Runs Mast till applications terminates
  //!
  DLL_EXPORT ErrorCode RunMast(const char* modelFilePath, AppFunctionAndNodePath* pAssociations, uint32_t count);


  //!< Starts up mast library, building model using specified file
  //!<
  //!< @note  This is to be used when there is no specific code to
  //!<        build initialize the model and an associated manager
  DLL_EXPORT ErrorCode InitializeMast(const char* modelFilePath);

  //!< Cleans up Mast library
  //!<
  //!< @note  To start using Mast (e.g. with a new model), one must call InitializeMast again
  //!<
  DLL_EXPORT ErrorCode CleanupMast();

  //! Starts System Model Manager
  //!
  DLL_EXPORT ErrorCode Start();

  //! Registers applications functions with their associated node to the System Model Manager
  //!
  DLL_EXPORT ErrorCode CreateApplications(AppFunctionAndNodePath* pAssociations, uint32_t count);


  //! Waits (blocks) until all application thread terminates (on their own or by a mechanism unknown to mast manager)
  //!
  DLL_EXPORT ErrorCode WaitForApplicationsEnd();

  //! Returns error message from last command
  //!
  //! @return Pointer to the message string (only valid till no other command is issued)
  DLL_EXPORT const char* ErrorMessage();

  //! Clears last error message
  //!
  DLL_EXPORT void ClearErrorMessage();

  //! Sets error message
  //!
  //! @note This is normally used only by library internals
  DLL_EXPORT void SetErrorMessage(const char* message);
}


#endif  // not defined SYSTEMMODELADAPTER_C_H__1D5A30E_BBA2_4C6D_B7B4_5077996B2B04__INCLUDED_
//===========================================================================
// End of SystemModelAdapter_C.h
//===========================================================================



