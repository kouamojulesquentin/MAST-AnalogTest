//===========================================================================
//                           MastEnvironment_impl.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file MastEnvironment_impl.hpp
//!
//! Declares MastEnvironment_impl class
//!
//===========================================================================


#ifndef MASTENVIRONMENT_IMPL_H__8AA36FD0_4B76_4464_E0BA_FCCE3E4926FA__INCLUDED_
  #define MASTENVIRONMENT_IMPL_H__8AA36FD0_4B76_4464_E0BA_FCCE3E4926FA__INCLUDED_

#include "LoggerSinks.h"

#include <memory>
#include <vector>
#include <string>

namespace g3
{
  class LogWorker;
  class LogFormatter;
}

namespace mast
{
class MastConfiguration;
class SystemModelManagerMonitor;
class SystemModel;
class SystemModelManager;

//! Defines complete environment for running mast.
//!
//! Options are given through command line arguments and optional configuration file.
//! Plugins are used to defined test algorithms, ConfigurationAlgorithm,
//! AccessInterfaceProtocol, PathSelector.
//!
//! @note The environment encompass a logger, a SystemModel and SystemModelManager
//!
class MastEnvironment_impl final
{
  // ---------------- Public Methods
  //
  public:
  ~MastEnvironment_impl() = default;

  MastEnvironment_impl(bool unitTestContext = false);         //!< Initializes MastEnvironment_impl

  void ParseOptions(int argc, const char* argv[]);       //!< Parses options - from C-Style command line arguments
  void ParseOptions(std::vector<std::string> arguments); //!< Parses options - from list of command line arguments

  void LoadPlugins();       //!< Loads plugin(s) defined by parsed options
  void CreateSystemModel(); //!< Creates system model using parsed options and loaded plugins

//+  bool UnitTestsContext() const { return m_unitTestsContext; }
//+  void UnitTestsContext (bool unitTestsContext) { m_unitTestsContext = unitTestsContext; }

  // ---------------- Private Methods
  //
  private:
  void CheckModel ();
  void ConfigureLogger ();
  void InitializeLogger ();
  std::string GetActualSitFilePath (const std::string& sitFile) const;

  // ---------------- Private Fields
  //
  private:
  using CerrSinkHandle_t = g3::SinkHandle<g3::ErrorsOnCerrLoggerSink>;

  std::unique_ptr<CerrSinkHandle_t>   m_cerrSinkHandle; //!< Initial logger sink that is disabled once user requested sink are connected
  std::shared_ptr<g3::LogFormatter>   m_logFormatter;
  std::shared_ptr<g3::LogWorker>      m_logger;
  std::shared_ptr<MastConfiguration>  m_configuration;
  std::shared_ptr<SystemModel>        m_sm;
  std::shared_ptr<SystemModelManager> m_manager;
  std::vector<std::string>            m_loadedPluginsPath; //!< To avoid loading them twice and to search for SIT file in same directories

  bool m_unitTestsContext = false;  //!< To manage logger differently in unit tests context
};
//
//  End of MastEnvironment_impl class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined MASTENVIRONMENT_IMPL_H__8AA36FD0_4B76_4464_E0BA_FCCE3E4926FA__INCLUDED_

//===========================================================================
// End of MastEnvironment_impl.hpp
//===========================================================================
