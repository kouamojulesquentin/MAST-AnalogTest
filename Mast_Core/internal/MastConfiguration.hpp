//===========================================================================
//                           MastConfiguration.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file MastConfiguration.hpp
//!
//! Declares MastConfiguration class
//!
//===========================================================================


#ifndef MASTCONFIGURATION_H__BCED33E8_29EF_4DA9_2F92_6B9AF0DC6FA5__INCLUDED_
  #define MASTCONFIGURATION_H__BCED33E8_29EF_4DA9_2F92_6B9AF0DC6FA5__INCLUDED_

#include "GmlPrinter.hpp"                // For options enums
#include "PrettyPrinter.hpp"             // For options enums
#include "SystemModelManagerMonitor.hpp" // For options enums

#include <vector>
#include <string>
#include <memory>

namespace TCLAP
{
class CmdLineOutput;
} // End of namespace TCLAP

namespace mast
{
//! Defines what kind of logger is used.
//!
enum class LoggerKind
{
  Std,              //!< Logs only into logger file
  CopyAllOnCout,    //!< Messages are logged to log file AND to the console
  CopyErrorsOnCerr, //!< Messages are logged to log file AND error messages also to the console
};

//! Kinds of element that are reported in the log
//!
enum class LoggerShownItems
{
   None         = 0,
   Date         = 1 << 0, //!< Show (calendar) date of message
   Time         = 1 << 1, //!< Show (clock) time of message
   Microseconds = 1 << 2, //!< Show microsecond of message (from start of logger)
   Level        = 1 << 3, //!< Show message level (debug, info, warning, error)
   ThreadId     = 1 << 4, //!< Show thread identifier from which the message was logged
   FileName     = 1 << 5, //!< Show file name from which the message was logged (for Mast developer team)
   LineNumber   = 1 << 6, //!< Show source line number from which the message was logged (for Mast developer team)
   FunctionName = 1 << 7, //!< Show function name from which the message was logged (for Mast developer team)

   Std = Time + Microseconds + Level      + ThreadId,
   All = Std  + FileName     + LineNumber + FunctionName,
};

//! Defines what level of information are logged (from error only to debug messages)
//!
enum class LoggerLevel
{
  Debug,    //!< All messages are logged (including debug ones)
  Info,     //!< Informational, warning and error messages are logged
  Warning,  //!< Warning and error messages are logged
  Error,    //!< Only error messages are logged
};

//! Defines moment(s) in the vector generation for which model state is reported
//!
enum class ReportMoments
{
  AfterModelParsing   = 1 << 0, //!< Just after [SIT] file has been parsed
  BeforeConfiguration = 1 << 1, //!< Monitor System_model state before each configuration  (when manager activity reporting is enabled)
  AfterConfiguration  = 1 << 2, //!< Monitor System_model state after each configuration   (when manager activity reporting is enabled)
};

//! Mast configuration options.
//!
//! @note Configuration is build up from default setting, configuration file and command line switches.
//!       Command line switches override configuration file values that override default settings
//!
//! @see @ref MastOptions for further documentation of configuration file and command line switches
class MAST_CORE_EXPORT MastConfiguration final
{
  // ---------------- Public Methods
  //
  public:
  ~MastConfiguration() = default;
  MastConfiguration();

  void Update(int argc, const char* argv[]);       //!< Updates from C-Style command line arguments
  void Update(std::vector<std::string> arguments); //!< Updates from list of command line arguments

  // ---------------- Options getters
  //
  std::string                     SitFilePath()                 const { return m_sitFilePath;                 } //!< SIT file used by the project
  std::string                     ConfigurationAlgorithm()      const { return m_configurationAlgorithm;      } //!< One of [last_lazy, last_or_default, last_or_default_greedy] or one defined by a [plugin])
  std::string                     AccessInterfaceProtocol()     const { return m_accessInterfaceProtocol;     } //!< When different to "SIT", it override the one define in [SIT] file
  std::string                     ApplicationPath()             const { return m_applicationDirectoryPath;    } //!< Application directory path (extracted from argv[0])
  const std::vector<std::string>& PluginDLLs()                  const { return m_pluginDLLs;                  } //!< List of plugins files paths
  const std::vector<std::string>& PluginDirectories()           const { return m_pluginDirectories;           } //!< List of plugins directories paths
  bool                            ModelChecking()               const { return m_modelChecking;               } //!< Enable/Disable model checking (provided it has been parsed successfully)
  std::string                     ModelCheckingFilePath()       const { return m_modelCheckingFilePath;       } //!< Optional model checking result file path (logged when no path when logged is enabled)
  bool                            LoggerEnabled()               const { return m_loggerEnabled;               } //!< Enable/Disable general logging
  mast::LoggerKind                LoggerKind()                  const { return m_loggerKind;                  } //!< Defines what kind of logger is used.
  std::string                     LoggerFilePath()              const { return m_loggerFilePath;              } //!< File path for logging (there is always a file logging when enabled)
  mast::LoggerShownItems          LoggerShownItems()            const { return m_loggerShownItems;            } //!< Kinds of element that are reported in the log
  mast::LoggerLevel               LoggerLevel()                 const { return m_loggerLevel;                 } //!< Defines what level of information are logged (from error only to debug messages)
  bool                            GmlPrinting()                 const { return m_gmlPrinting;                 } //!< Enable/Disable printing of GML representation of the model state
  mast::ReportMoments             GmlReportMoments()            const { return m_gmlReportMoments;            } //!< Defines moment(s) in the vector generation for which model state is exported as GML file
  std::string                     GmlFilePath()                 const { return m_gmlFilePath;                 } //!< Optional file path for GML graph printing
  std::string                     GmlGraphName()                const { return m_gmlGraphName;                } //!< Optional graph name
  GmlPrinterOptions               GmlOptions()                  const { return m_gmlOptions;                  } //!< Defines what kind of model information pieces are displayed in the model state report.
  bool                            PrettyPrinting()              const { return m_prettyPrinting;              } //!< Enable/Disable printing of textual representation of the model state
  mast::ReportMoments             PrettyPrintReportMoments()    const { return m_prettyPrintingReportMoments; } //!< Defines moment(s) in the vector generation for which model state is exported as text file
  std::string                     PrettyPrintFilePath()         const { return m_prettyPrintingFilePath;      } //!< Optional file path for pretty print
  PrettyPrinterOptions            PrettyPrintOptions()          const { return m_prettyPrintingOptions;       } //!< Defines what kind of model information pieces are reported in the textual model state report
  bool                            ReportManagerActivity()       const { return m_reportManagerActivity;       } //!< Enable/Disable logging and optionally reporting model state at specific point of manager activity
  std::string                     ManagerActivityFileBasePath() const { return m_managerActivityFileBasePath; } //!< Base file path where pretty print and/or GML files are saved
  ManagerMonitorOptions           ManagerActivityOptions()      const { return m_managerActivityOptions;      } //!< Defines what Mast manager main activities are reported in the log

  //! Sets command line parser object in charge for output messages
  //!
  //! @note This is useful for unit tests purpose
  void CommandLineParserOutput (std::shared_ptr<TCLAP::CmdLineOutput> parserOutput) { m_cmdLineOutput = parserOutput; };

  //! Sets whether parsing does automatic application exit when needed
  //!
  bool  AutomaticExit() const { return m_automaticExit; }

  //! Returns whether parsing does automatic application exit when needed
  //!
  void  AutomaticExit (bool automaticExit) { m_automaticExit = automaticExit; }

  //! Reports whether application should have been exited (when automaticExit is set to true)
  //!
  //! @note This is useful for unit tests purpose
  bool ShouldExit() const { return m_shouldExit; }

  // ---------------- Private Methods
  //
  std::string ExtractApplicationDirectoryPath (const std::string& applicationPath);
  void        ParseConfigurationFile          (const std::string& configurationFile);
  void        ParseYamlConfiguration          (const std::string& yamlConfiguration);

  // ---------------- Private Fields
  //
  private:


  std::string              m_applicationDirectoryPath;                                             //!< Application directory path (extracted from argv[0])
  std::string              m_sitFilePath;                                                          //!< SIT file used by the project
  std::string              m_configurationAlgorithm;                                               //!< One of [last_lazy, last_or_default, last_or_default_greedy] or one defined by a [plugin])
  std::string              m_accessInterfaceProtocol;                                              //!< When different to "SIT", it override the one define in [SIT] file
  std::vector<std::string> m_pluginDLLs;                                                           //!< List of plugins files paths
  std::vector<std::string> m_pluginDirectories;                                                    //!< List of plugins directories paths
  bool                     m_modelChecking               = false;                                  //!< Enable/Disable model checking (provided it has been parsed successfully)
  std::string              m_modelCheckingFilePath;                                                //!< Optional file path (logged when no path when logged is enabled)
  bool                     m_loggerEnabled               = false;                                  //!< Enable/Disable general logging
  mast::LoggerKind         m_loggerKind                  = mast::LoggerKind::Std;                  //!< Defines what kind of logger is used.
  std::string              m_loggerFilePath;                                                       //!< File path for logging (there is always a file logging when enabled)
  mast::LoggerShownItems   m_loggerShownItems            = mast::LoggerShownItems::Std;            //!< Kinds of element that are reported in the log
  mast::LoggerLevel        m_loggerLevel                 = mast::LoggerLevel::Info;                //!< Defines what level of information are logged (from error only to debug messages)
  bool                     m_gmlPrinting                 = false;                                  //!< Enable/Disable printing of GML representation of the model state
  mast::ReportMoments      m_gmlReportMoments            = mast::ReportMoments::AfterModelParsing; //!< Defines moment(s) in the vector generation for which model state is exported as GML file
  std::string              m_gmlFilePath;                                                          //!< Optional file path for GML graph printing
  std::string              m_gmlGraphName;                                                         //!< Optional graph name
  GmlPrinterOptions        m_gmlOptions                  = GmlPrinterOptions::Std;                 //!< Defines what kind of model information pieces are displayed in the model state report.
  bool                     m_prettyPrinting              = false;                                  //!< Enable/Disable printing of textual representation of the model state
  mast::ReportMoments      m_prettyPrintingReportMoments = mast::ReportMoments::AfterModelParsing; //!< Defines moment(s) in the vector generation for which model state is exported as text file
  std::string              m_prettyPrintingFilePath;                                               //!< Optional file path for pretty print
  PrettyPrinterOptions     m_prettyPrintingOptions       = PrettyPrinterOptions::Std;              //!< Defines what kind of model information pieces are reported in the textual model state report
  bool                     m_reportManagerActivity       = false;                                  //!< Enable/Disable logging and optionally reporting model state at specific point of manager activity
  std::string              m_managerActivityFileBasePath;                                          //!< Base file path where pretty print and/or GML files are saved
  ManagerMonitorOptions    m_managerActivityOptions      = ManagerMonitorOptions::Std;             //!< Defines what Mast manager main activities are reported in the log

  std::shared_ptr<TCLAP::CmdLineOutput> m_cmdLineOutput;         //!< Useful for unit tests (to catch command line parse output)
  bool                                  m_automaticExit = true;  //!< For unit tests context (to prevent unit tests exit)
  bool                                  m_shouldExit    = false; //!< Tells result of parsing in unit tests ctx
};
//
//  End of MastConfiguration class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined MASTCONFIGURATION_H__BCED33E8_29EF_4DA9_2F92_6B9AF0DC6FA5__INCLUDED_

//===========================================================================
// End of MastConfiguration.hpp
//===========================================================================
