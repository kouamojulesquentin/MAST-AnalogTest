//===========================================================================
//                           MastConfiguration.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file MastConfiguration.cpp
//!
//! Implements class MastConfiguration
//!
//===========================================================================

#include "MastConfiguration.hpp"
#include "MastConfig.hpp"
#include "g3log/g3log.hpp"

#include <tclap/CmdLine.h>
#include <tclap/StreamOutput.h>

using std::vector;
using std::string;
using namespace mast;

//! Does nothing in particular
//! @internal
//! @note Not in header file as default because there is unique_ptr member variable
//!       with only forward declared class.
//! @endinternal
//+MastConfiguration::~MastConfiguration ()
//+{
//+}
//
//  End of: MastConfiguration::~MastConfiguration
//---------------------------------------------------------------------------



//! Initializes with default configurations
//!
MastConfiguration::MastConfiguration ()
  : m_sitFilePath                 ("DUT.sit")
  , m_configurationAlgorithm      ("LastOrDefault")
  , m_accessInterfaceProtocol     ("")
  , m_pluginDLLs                  ({PLUGINS_DIRECTORY_NAME})
  , m_modelCheckingFilePath       ()
  , m_loggerFilePath              ("Mast.log")
  , m_gmlFilePath                 ("MastModel.gml")
  , m_gmlGraphName                ("DUT")
  , m_prettyPrintingFilePath      ("MastModel.txt")
  , m_managerActivityFileBasePath ("DUT")
{
}
//
//  End of: MastConfiguration::MastConfiguration
//---------------------------------------------------------------------------




//! Updates from C-Style command line arguments
//!
//! @param argc   Number of arguments (first one is application name)
//! @param argv   Array of arguments
//!
void MastConfiguration::Update (int argc, char* argv[])
{
  vector<string> arguments;

  for (int ii = 0 ; ii < argc ; ++ii)
  {
    arguments.emplace_back(argv[ii]);
  }
  Update(arguments);
}
//
//  End of: MastConfiguration::Update
//---------------------------------------------------------------------------



//! Updates from list of command line arguments
//!
//! @param arguments  Command line arguments (first one is application name)
//!
void MastConfiguration::Update (vector<string> arguments)
{
  try
  {
    // ---------------- Prepare the parser
    //
    TCLAP::CmdLine cmdLine("Mast", '=', MAST_VERSION);

    if (m_cmdLineOutput)
    {
      cmdLine.setOutput(m_cmdLineOutput.get());
    }
    cmdLine.automaticExit(m_automaticExit);

    // ---------------- Prepare accepted arguments
    //
    vector<string> allowedLogLevel {"debug", "info", "warning", "error"};
    vector<string> allowedLogKind  {"std", "copy_all_on_cout", "copy_errors_on_cerr"};

    TCLAP::ValuesConstraint<string> logLevelConstraint(allowedLogLevel);
    TCLAP::ValuesConstraint<string> logKindConstraint(allowedLogKind);

    TCLAP::ValueArg<std::string> configurationFileArg ("-c", "--conf",        "Define configuration file",                                                      false, "mast.cfg",               "File path", cmdLine);
    TCLAP::ValueArg<std::string> sitFilePathArg       ("-s", "--sit",         "Define SIT that specified SUT model",                                            false, "project.sit",            "File path", cmdLine);
    TCLAP::ValueArg<std::string> configurationAlgoArg ("-a", "--config_algo", "Name of configuration algorithm used to select linker  (mux) path",              false, "last_or_default",        "One of [last_lazy, last_or_default, last_or_default_greedy] or one defined by a plugin", cmdLine);
    TCLAP::ValueArg<std::string> aiProtocolArg        ("",   "--protocol",    "Override access interface protocol defined in SIT file",                         false, "",                       "Protocol name", cmdLine);
    TCLAP::ValueArg<std::string> checkModelFileArg    ("",   "--check_file",  "Defines result of model checking  (it is always logged when logger is enabled)", false, "mast_check.txt",         "File path", cmdLine);
    TCLAP::ValueArg<std::string> logFileArg           ("",   "--log_file",    "Define logger file path",                                                        false, "mast.log",               "File path", cmdLine);
    TCLAP::ValueArg<std::string> logLevelArg          ("",   "--log_level",   "Define log level",                                                               false, "info",                   &logLevelConstraint, cmdLine);
    TCLAP::ValueArg<std::string> logKindArg           ("",   "--log_kind",    "Define logger kind",                                                             false, "std",                    &logKindConstraint, cmdLine);
    TCLAP::SwitchArg             logEnabledArg        ("-l", "--log",         "Enable logger",                                                                  cmdLine, false);
    TCLAP::SwitchArg             checkModelArg        ("",   "--check",       "Enable model checking  (resulting from parsing SIT file)",                       cmdLine, false);
    TCLAP::MultiArg<std::string> pluginDLLsArg        ("",   "--plugin",      "Define plugins to load  (may be directory of file path)",                        false, "Directory or file path", cmdLine);

    // ---------------- Do parse command line arguments
    //
    cmdLine.parse(arguments);
  }
  catch(std::exception& exc)  // Catch C++ standard exceptions
  {
    LOG(ERROR_LVL) << "Failed to parse command line: " << exc.what();
    throw;
  }
  catch (...)
  {
    LOG(ERROR_LVL) << "Cautch unknown exception while parsing command line";
    throw;
  }


  //! @todo [JFC]-[June/02/2017]: use try catch re-throw to provide additional context
  //!
}
//
//  End of: MastConfiguration::Update
//---------------------------------------------------------------------------





//===========================================================================
// End of MastConfiguration.cpp
//===========================================================================
