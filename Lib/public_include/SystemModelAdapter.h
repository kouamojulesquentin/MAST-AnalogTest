//===========================================================================
//                           SystemModelAdapter.h
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelAdapter.h
//!
//! Declares 'C' language API to system model
//===========================================================================


#ifndef SYSTEMMODELADAPTER_H__A6DC6C83_7B2F_4CC8_7DAA_31AA4CDB849D__INCLUDED_
  #define SYSTEMMODELADAPTER_H__A6DC6C83_7B2F_4CC8_7DAA_31AA4CDB849D__INCLUDED_

#include "Platform.hpp"

#include <stdint.h>
extern "C"
{
  enum ErrorCode_t
  {
    Ok               = 0,
    InvalidArgument  = 0xFFFFFFF9,
    LogicError       = 0xFFFFFFFA,
    OutOfRange       = 0xFFFFFFFB,
    RuntimeError     = 0xFFFFFFFC,
    StdException     = 0xFFFFFFFD,
    UndefinedFailure = 0xFFFFFFFE,
    Failure          = 0xFFFFFFFF,
  };
  typedef enum ErrorCode_t ErrorCode;

  typedef void(*PDLApplication_t)();

  struct ApplicationAssociation
  {
    PDLApplication_t function;
    const char*      topNodePath;
    const char*      debugName;
  };

  //! Runs Mast til applications terminates
  //!
  DLL_EXPORT ErrorCode RunMast(const char* modelFilePath, ApplicationAssociation* pAssociations, uint32_t count);


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
  DLL_EXPORT ErrorCode CreateApplications(ApplicationAssociation* pAssociations, uint32_t count);


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

#endif  // not defined SYSTEMMODELADAPTER_H__A6DC6C83_7B2F_4CC8_7DAA_31AA4CDB849D__INCLUDED_
//===========================================================================
// End of SystemModelAdapter.h
//===========================================================================



