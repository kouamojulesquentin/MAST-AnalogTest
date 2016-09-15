//===========================================================================
//                           SystemModelAdapter_CPP.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelAdapter_CPP.hpp
//!
//! Declares 'CPP' language API to system model
//===========================================================================

#ifndef SYSTEMMODELADAPTER_CPP_H__D20B38D9_1D62_4C8A_59AE_115AEF979C69__INCLUDED_
  #define SYSTEMMODELADAPTER_CPP_H__D20B38D9_1D62_4C8A_59AE_115AEF979C69__INCLUDED_

#include "Platform.hpp"
#include <string>
#include <vector>
#include <experimental/string_view>

namespace mast
{
  class AppFunctionAndNodePath;

  //! Flags for running mast
  //!
  enum class RunMastOptions
  {
    None               = 0,      //!< No options set
    LogManagerActivity = 1 << 0, //!< Mast manager activity is logged to file
    CheckModel         = 1 << 1, //!< Constructed model is checked before starting Mast manager
    PrintModelGraph    = 1 << 2, //!< A GML formated graph repesented system model is exported before starting Mast manager
  };

  //! Enables "oring" of RunMastOptions flags
  //!
  inline constexpr RunMastOptions operator | (RunMastOptions X, RunMastOptions Y)
  {
    return static_cast<RunMastOptions>(   static_cast<std::underlying_type_t<RunMastOptions>>(X)
                                        | static_cast<std::underlying_type_t<RunMastOptions>>(Y));
  }

  //! Enables "oring" of RunMastOptions flags
  //!
  inline RunMastOptions& operator |= (RunMastOptions& X, RunMastOptions Y)
  {
    X = static_cast<RunMastOptions>(  static_cast<std::underlying_type_t<RunMastOptions>>(X)
                                    | static_cast<std::underlying_type_t<RunMastOptions>>(Y));
    return X;
  }


  //! Check SystemModel coherency, logging check result and throwing runtime exception in case of error
  //!
  void CheckSystemModel ();

  //! Exports GML formated graph of system model
  //!
  void PrintModelGraph(const std::string& filePath = "MastModel.gml");



  //! Creates a system model from configuration file, then runs Mast till applications terminates
  //!
  DLL_EXPORT void RunMast(std::experimental::string_view             modelFilePath,
                          const std::vector<AppFunctionAndNodePath>& appAssociations,
                          RunMastOptions                             options = RunMastOptions::None);

  //! Runs Mast till applications terminates
  //!
  //! @note Mast must have been initialized and system model been created beforehand
  DLL_EXPORT void RunMast(const std::vector<AppFunctionAndNodePath>& appAssociations, RunMastOptions options = RunMastOptions::None);

  //! Starts up mast library, building model using specified file
  //!
  //! @note  This is to be used when there is no specific code to
  //!        build initialize the model and an associated manager
  DLL_EXPORT void InitializeMast(std::experimental::string_view modelFilePath);

  //! Cleans up Mast library
  //!
  //! @note  To start using Mast (e.g. with a new model), one must call InitializeMast again
  //!
  DLL_EXPORT void CleanupMast();

  //! Starts System Model Manager
  //!
  DLL_EXPORT void Start();

  //! Stops System Model Manager
  //!
  DLL_EXPORT void Stop();


  //! Registers applications functions with their associated node to the System Model Manager
  //!
  DLL_EXPORT void CreateApplications(const std::vector<AppFunctionAndNodePath>& appAssociations);

  //! Waits (blocks) until all application thread terminate (on their own or by a mechanism unknown to mast manager)
  //!
  DLL_EXPORT void WaitForApplicationsEnd();
}


#endif  // not defined SYSTEMMODELADAPTER_CPP_H__D20B38D9_1D62_4C8A_59AE_115AEF979C69__INCLUDED_
//===========================================================================
// End of SystemModelAdapter_CPP.hpp
//===========================================================================



