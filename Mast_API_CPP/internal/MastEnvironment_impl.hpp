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
#include <experimental/string_view>

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
class AppFunctionNameAndNode;

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
  ~MastEnvironment_impl();

  MastEnvironment_impl(bool unitTestContext = false);         //!< Initializes MastEnvironment_impl

  void Start(std::vector<std::string> arguments); //!< Starts MAST using options from list of command line arguments
  void Start(int argc, const char* argv[])        //!< Starts MAST using options from C-Style command line arguments
  {
    Start(MakeArgumentsVector(argc, argv));
  }

  // ---------------- Optional API (this is what Start call)
  //
  bool ParseOptions(std::vector<std::string> arguments); //!< Parses options - from list of command line arguments
  bool ParseOptions(int argc, const char* argv[])        //!< Parses options - from C-Style command line arguments
  {
    return ParseOptions(MakeArgumentsVector(argc, argv));
  }

  void LoadPlugins();         //!< Loads plugin(s) defined by parsed options
  void CreateSystemModel();   //!< Creates system model using parsed options and loaded plugins
  void CreateManager      (); //! Creates system model manager
  void CreateApplications (); //! Creates PDL algorithm with their associated nodes
  void ReportParsedModel();   //! Save GML/Text view of parsed SIT model
  void Start();               //! Starts system model manager AND wait till it ends

  // ---------------- Private Methods
  //
  private:
  void ChangeAccessInterfaceProtocol (const std::string& protocolName, const std::string& parameters);
  void CheckModel (std::shared_ptr<SystemModel> systemModel);
  void ConfigureLogger ();
  void InitializeLogger ();
  std::string GetActualProjectFilePath (const std::string& sitFile, std::experimental::string_view defaultFileExtension) const;

  std::shared_ptr<SystemModel> CreateSystemModel_FromSitFile     (const std::string& sitFile);
  std::shared_ptr<SystemModel> CreateSystemModel_FromIclFile     (const std::string& iclFile);
  std::shared_ptr<SystemModel> CreateSystemModel_FromIclFileList (const std::string& listFile);

  std::shared_ptr<SystemModelManagerMonitor> MakeManagerMonitor ();
  static std::vector<std::string>            MakeArgumentsVector (int argc, const char* argv[]);

  // ---------------- Private Fields
  //
  private:
  std::shared_ptr<MastConfiguration>  m_configuration;              //!< Mast option overriden or not by user via configuration file or command line arguments
  std::vector<std::string>            m_loadedPluginsPath;          //!< To avoid loading them twice and to search for SIT file in same directories
  std::vector<AppFunctionNameAndNode> m_algoNamesAssociatedToNodes; //!< Associations of PDL algorithm names to nodes
  bool                                m_unitTestsContext = false;   //!< To manage logger differently in unit tests context
};
//
//  End of MastEnvironment_impl class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined MASTENVIRONMENT_IMPL_H__8AA36FD0_4B76_4464_E0BA_FCCE3E4926FA__INCLUDED_

//===========================================================================
// End of MastEnvironment_impl.hpp
//===========================================================================
