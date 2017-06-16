//===========================================================================
//                           MastEnvironment.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file MastEnvironment.hpp
//!
//! Declares MastEnvironment class
//!
//===========================================================================

#ifndef MASTENVIRONMENT_H__6F78EBE_7DDB_4A71_D889_4FB6C8F94905__INCLUDED_
  #define MASTENVIRONMENT_H__6F78EBE_7DDB_4A71_D889_4FB6C8F94905__INCLUDED_

#include "CPP_API_export.hpp"

#include <memory>
#include <vector>
#include <string>

namespace mast
{
class MastEnvironment_impl;

//! Defines complete environment for running mast.
//!
//! Options are given through command line arguments and optional configuration file.
//! Plugins are used to defined test algorithms, ConfigurationAlgorithm,
//! AccessInterfaceProtocol, PathSelector.
//!
//! @note The environment encompass a logger, a SystemModel and SystemModelManager
//!
class CPP_API_EXPORT MastEnvironment final
{
  // ---------------- Public Methods
  //
  public:
  ~MastEnvironment();

  MastEnvironment(bool unitTestContext = false);         //!< Initializes MastEnvironment

  void ParseOptions(int argc, const char* argv[]);       //!< Parses options - from C-Style command line arguments
  void ParseOptions(std::vector<std::string> arguments); //!< Parses options - from list of command line arguments
  void LoadPlugins();                                    //!< Loads plugin(s) defined by parsed options
  void CreateSystemModel();                              //!< Creates system model using parsed options and loaded plugins
  void CreateManager();                                  //!< Creates system model manager
  void CreateApplications();                             //! Creates PDL algorithm with their associated nodes

  // ---------------- Private Fields
  //
  private:
  std::unique_ptr<MastEnvironment_impl> m_impl;
};
//
//  End of MastEnvironment class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined MASTENVIRONMENT_H__6F78EBE_7DDB_4A71_D889_4FB6C8F94905__INCLUDED_

//===========================================================================
// End of MastEnvironment.hpp
//===========================================================================
