//===========================================================================
//                           SystemModelAdapter_CPP.h
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelAdapter_CPP.h
//!
//! Declares 'CPP' language API to system model
//===========================================================================

#ifndef SYSTEMMODELADAPTER_CPP_H__C34A0543_25E_40B0_64AB_80AA19C3259__INCLUDED_
  #define SYSTEMMODELADAPTER_CPP_H__C34A0543_25E_40B0_64AB_80AA19C3259__INCLUDED_

#include "Platform.hpp"
#include <string>
#include <vector>
#include <experimental/string_view>
#include <functional>

namespace mast
{
  //! Associates functions (applications) with some node path and optionally a (debug) name
  //!
  struct ApplicationAssociation final
  {
    using Application_t = std::function<void()>;

    Application_t function;
    std::string   topNodePath;
    std::string   debugName;

    ApplicationAssociation(Application_t p_function, std::string p_topNodePath, std::string p_debugName = "")
      : function    (p_function)
      , topNodePath (std::move(p_topNodePath))
      , debugName   (std::move(p_debugName))
    {}
  };
  //
  //  End of ApplicationAssociation struct declaration
  //---------------------------------------------------------------------------


  //! Runs Mast till applications terminates
  //!
  DLL_EXPORT void RunMast(std::experimental::string_view modelFilePath, const std::vector<ApplicationAssociation>& appAssociations);

  //!< Starts up mast library, building model using specified file
  //!<
  //!< @note  This is to be used when there is no specific code to
  //!<        build initialize the model and an associated manager
  DLL_EXPORT void InitializeMast(std::experimental::string_view modelFilePath);

  //!< Cleans up Mast library
  //!<
  //!< @note  To start using Mast (e.g. with a new model), one must call InitializeMast again
  //!<
  DLL_EXPORT void CleanupMast();

  //! Starts System Model Manager
  //!
  DLL_EXPORT void Start();

  //! Stops System Model Manager
  //!
  DLL_EXPORT void Stop();


  //! Registers applications functions with their associated node to the System Model Manager
  //!
  DLL_EXPORT void CreateApplications(const std::vector<ApplicationAssociation>& appAssociations);

  //! Waits (blocks) until all application thread terminates (on their own or by a mechanism unknown to mast manager)
  //!
  DLL_EXPORT void WaitForApplicationsEnd();
}

#endif  // not defined SYSTEMMODELADAPTER_CPP_H__C34A0543_25E_40B0_64AB_80AA19C3259__INCLUDED_
//===========================================================================
// End of SystemModelAdapter_CPP.h
//===========================================================================



