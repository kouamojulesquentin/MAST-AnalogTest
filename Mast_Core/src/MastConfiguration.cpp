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
#include "Utility.hpp"
#include "YamlFile.hpp"

#include <tclap/CmdLine.h>
#include <tclap/StreamOutput.h>

#include <map>
#include <tuple>
#include <initializer_list>
#include <type_traits>

using std::vector;
using std::string;
using std::map;
using std::tie;
using std::initializer_list;

using namespace mast;

namespace
{
// ---------------- Maps logger kind arguments to internal value
//
static const map<string, mast::LoggerKind>  loggerKindMapping
{
  {"std",                 mast::LoggerKind::Std},
  {"copy_all_on_cout",    mast::LoggerKind::CopyAllOnCout},
  {"copy_errors_on_cerr", mast::LoggerKind::CopyErrorsOnCerr},
};

// ---------------- Maps logger level arguments to internal value
//
static const map<string, mast::LoggerLevel> loggerLevelMapping
{
  {"debug",   mast::LoggerLevel::Debug},
  {"info",    mast::LoggerLevel::Info},
  {"warning", mast::LoggerLevel::Warning},
  {"error",   mast::LoggerLevel::Error},
};

// ---------------- Maps logger Show_Item option to internal value
//
static const map<string, mast::LoggerShownItems> loggerShownItemMapping
{
  {"date",          mast::LoggerShownItems::Date},
  {"time",          mast::LoggerShownItems::Time},
  {"microseconds",  mast::LoggerShownItems::Microseconds},
  {"level",         mast::LoggerShownItems::Level},
  {"thread_id",     mast::LoggerShownItems::ThreadId},
  {"file_name",     mast::LoggerShownItems::FileName},
  {"line_number",   mast::LoggerShownItems::LineNumber},
  {"function_name", mast::LoggerShownItems::FunctionName},
  {"std",           mast::LoggerShownItems::Std},
  {"all",           mast::LoggerShownItems::All},
};

// ---------------- Maps GML printer options to internal value
//
static const map<string, mast::GmlPrinterOptions> gmlPrinterOptionsMapping
{
  {"default",             GmlPrinterOptions::Default},
  {"identifiers",         GmlPrinterOptions::DisplayIdentifiers},
  {"register_value",      GmlPrinterOptions::DisplayRegisterValue},
  {"auto_value",          GmlPrinterOptions::DisplayValueAuto},
  {"protocol",            GmlPrinterOptions::ShowProtocol},
  {"selector_with_edge",  GmlPrinterOptions::ShowSelectorWithEdge},
  {"selector_properties", GmlPrinterOptions::ShowSelectorProperties},
  {"selector_tables",     GmlPrinterOptions::ShowSelectorTables},
  {"selection_value",     GmlPrinterOptions::ShowSelectionValues},
  {"std",                 GmlPrinterOptions::Std},
  {"all",                 GmlPrinterOptions::All},
};

// ---------------- Maps Pretty printer options to internal value
//
static const map<string, mast::PrettyPrinterOptions> prettyPrinterOptionsMapping
{
  {"verbose",             PrettyPrinterOptions::Verbose},
  {"auto_value",          PrettyPrinterOptions::DisplayValueAuto},
  {"protocol_name",       PrettyPrinterOptions::ShowProtocol},
  {"selection_state",     PrettyPrinterOptions::ShowSelectionState},
  {"selector_properties", PrettyPrinterOptions::ShowSelectorProperties},
  {"selection_value",     PrettyPrinterOptions::ShowSelectionValue},
  {"ignored_nodes",       PrettyPrinterOptions::ShowNodeIsIgnored},
  {"default",             PrettyPrinterOptions::Default},
  {"none",                PrettyPrinterOptions::None},
  {"std",                 PrettyPrinterOptions::Std},
  {"all",                 PrettyPrinterOptions::All},
  {"parser_debug",        PrettyPrinterOptions::Parser_debug},
};

// ---------------- Maps manager activity options to internal value
//
static const map<string, mast::ManagerMonitorOptions> managerActivityOptionsMapping
{
  {"default",             ManagerMonitorOptions::Default},
  {"verbose",             ManagerMonitorOptions::Verbose},
  {"app_thread_creation", ManagerMonitorOptions::AppThreadCreation},
  {"PDL_commands",        ManagerMonitorOptions::PDLCommands},
  {"data_cycles",         ManagerMonitorOptions::DataCycles},
  {"internal_debug",      ManagerMonitorOptions::InternalDebug},
  {"std",                 ManagerMonitorOptions::Std},
  {"all",                 ManagerMonitorOptions::All},
};

// ---------------- Maps report moment options to internal value
//
static const map<string, mast::ReportMoments> reportMomentsMapping
{
  {"after_model_parsing",  ReportMoments::AfterModelParsing},
  {"before_configuration", ReportMoments::BeforeConfiguration},
  {"after_configuration",  ReportMoments::AfterConfiguration},
};
} // End of unnamed namespace



//! Initializes with default configurations
//!
MastConfiguration::MastConfiguration ()
  : m_sitFilePath                 ("DUT.sit")
  , m_configurationAlgorithm      ("last_or_default")
  , m_accessInterfaceProtocol     ("")
  , m_pluginDirectories           ({PLUGINS_DIRECTORY_NAME})
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


//! Extracts for current application path (usually got from argv[0]), the directory path
//!
//! @note This is not work properly if the application path is reduced to application name
//!
//! @param applicationPath  The application path
//!
//! @return Extracted path or "." when no path has been found
string MastConfiguration::ExtractApplicationDirectoryPath (const string& applicationPath)
{
  // ---------------- Set dirPath with only directory path of runner application
  //
  auto dirPath = applicationPath;
  auto sepPos  = dirPath.rfind("/"); // Search for last Linux directory separator
  if (sepPos == std::string::npos)
  {
    sepPos  = dirPath.rfind("\\");   // Search for last Windows directory separator
  }

  if (sepPos != std::string::npos)
  {
    dirPath.erase(sepPos);           // Remove exe name (keeping only directory path)
  }
  else
  {
    dirPath = ".";                   // When no separator ==> there is only application name ==> this is current directory
  }
  return dirPath;
}
//
//  End of: MastConfiguration::ExtractApplicationDirectoryPath
//---------------------------------------------------------------------------



//! Parses MAST options in configuration file
//!
//! @note If the file does not exist, this is logged as an info
//!
//! @param configurationFile  Path to file to parse
//!
void MastConfiguration::ParseConfigurationFile (const string& configurationFile)
{
  CHECK_PARAMETER_NOT_EMPTY(configurationFile, "Cannot parse file with empty path");

  if (!Utility::FileExists(configurationFile))
  {
    LOG(INFO) << "Cannot find configuration file: " << configurationFile;
    return;
  }

  auto yaml = Utility::ReadTextFile(configurationFile);
  ParseYamlConfiguration(yaml);

  if (yaml.empty())
  {
    LOG(INFO) << "Empty configuration file";
  }
}
//
//  End of: MastConfiguration::ParseConfigurationFile
//---------------------------------------------------------------------------


//! Parses MAST options from YAML text
//!
//! @param yamlConfiguration  YAML configuration
//!
void MastConfiguration::ParseYamlConfiguration (const string& yamlConfiguration)
{
  if (!yamlConfiguration.empty())
  {
    // ---------------- Check configuration respects YAML syntax
    //
    auto ok           = false;
    auto errorMessage = ""s;

    tie(ok, errorMessage) = YamlFile::Check(yamlConfiguration);
    CHECK_TRUE(ok, errorMessage);

    // ---------------- Parse YAML configuration
    //
    YamlFile  yaml;
    yaml.Load(yamlConfiguration);

    // ---------------- Update MAST options from the configuration
    //
    string path;
    auto makePath = [&path](initializer_list<const char*> pathItems)
    {
      path.clear();
      path.append("Mast_Options");
      for (const auto& pathItem : pathItems)
      {
        path.append(YamlFile::PATH_SEPARATOR_S).append(pathItem);
      }
      return path;
    };

    // String updater
    string gotString;
    auto updateString = [&yaml, &gotString, &makePath](string& option, initializer_list<const char*> pathItems)
    {
      auto ok = false;
      tie(ok, gotString) = yaml.TryGetAsString (makePath(pathItems));
      if (ok)
      {
        option = gotString;
      }
    };

    // Enum flags updater
    auto updateEnum = [&yaml, &gotString, &makePath](auto& option, const auto& mapping, initializer_list<const char*> pathItems)
    {
      auto ok = false;
      tie(ok, gotString) = yaml.TryGetAsString (makePath(pathItems));
      if (ok)
      {
        auto pos = mapping.find(gotString);
        if (pos != mapping.cend())
        {
          option = pos->second;
        }
      }
    };

    // Enum flags updater
    vector<string> gotFlags;  // To be re-used for each flag option parsing
    auto updateFlags = [&yaml, &gotFlags, &makePath](auto& option, const auto& mapping, initializer_list<const char*> pathItems)
    {
      auto ok = false;
      tie(ok, gotFlags) = yaml.TryGetAsStringVector (makePath(pathItems));
      if (ok)
      {
        using enum_type = std::remove_reference_t<decltype(option)>;

        option = static_cast<enum_type>(0);

        for (const auto& flag : gotFlags)
        {
          auto pos = mapping.find(flag);
          if (pos != mapping.cend())
          {
            option = option | pos->second;
          }
          else
          {
            LOG(INFO) << "Invalid flag: " << flag;
          }
        }
      }
    };


    // Bool updater
    auto updateBool = [&yaml, &gotString, &makePath](bool& option, initializer_list<const char*> pathItems)
    {
      auto ok  = false;
      auto got = false;

      tie(ok, got) = yaml.TryGetAsBool (makePath(pathItems));
      if (ok)
      {
        option = got;
      }
    };

    // Enum flags updater
    auto updateSequence = [&yaml, &makePath](auto& option, initializer_list<const char*> pathItems)
    {
      auto           ok = false;
      vector<string> gotSequence;

      tie(ok, gotSequence) = yaml.TryGetAsStringVector (makePath(pathItems));
      if (ok)
      {
        option = std::move(gotSequence);
      }
    };

    updateString (m_sitFilePath,                 {"SIT_file_path"});
    updateString (m_accessInterfaceProtocol,     {"Access_interface_protocol"});
    updateString (m_configurationAlgorithm,      {"Configuration_algorithm"});
    updateString (m_modelCheckingFilePath,       {"Model_checking", "File_path"});
    updateString (m_gmlFilePath,                 {"Debug",          "Model_GML_printing",  "File_path"});
    updateString (m_gmlGraphName,                {"Debug",          "Model_GML_printing",  "Graph_name"});
    updateString (m_prettyPrintingFilePath,      {"Debug",          "Model_textual_print", "File_path"});
    updateString (m_loggerFilePath,              {"Debug",          "Logging",             "File_path"});
    updateString (m_managerActivityFileBasePath, {"Debug",          "Manager_activity",    "File_base_name"});
    updateBool   (m_modelChecking,               {"Model_checking", "Enable"});
    updateBool   (m_loggerEnabled,               {"Debug",          "Logging",             "Enable"});
    updateBool   (m_gmlPrinting,                 {"Debug",          "Model_GML_printing",  "Enable"});
    updateBool   (m_prettyPrinting,              {"Debug",          "Model_textual_print", "Enable"});
    updateBool   (m_reportManagerActivity,       {"Debug",          "Manager_activity",    "Enable"});

    updateEnum  (m_loggerKind,                  loggerKindMapping,             {"Debug", "Logging",             "Logger_Kind"});
    updateEnum  (m_loggerLevel,                 loggerLevelMapping,            {"Debug", "Logging",             "Level"});
    updateFlags (m_loggerShownItems,            loggerShownItemMapping,        {"Debug", "Logging",             "Shown_items"});
    updateFlags (m_gmlOptions,                  gmlPrinterOptionsMapping,      {"Debug", "Model_GML_printing",  "Options"});
    updateFlags (m_gmlReportMoments,            reportMomentsMapping,          {"Debug", "Model_GML_printing",  "Moments"});
    updateFlags (m_prettyPrintingOptions,       prettyPrinterOptionsMapping,   {"Debug", "Model_textual_print", "Options"});
    updateFlags (m_prettyPrintingReportMoments, reportMomentsMapping,          {"Debug", "Model_textual_print", "Moments"});
    updateFlags (m_managerActivityOptions,      managerActivityOptionsMapping, {"Debug", "Manager_activity",    "Options"});

    updateSequence (m_pluginDLLs,        {"Plugins", "Files"});
    updateSequence (m_pluginDirectories, {"Plugins", "Directories"});
  }
}
//
//  End of: MastConfiguration::ParseYamlConfiguration
//---------------------------------------------------------------------------


//! Updates from C-Style command line arguments
//!
//! @param argc   Number of arguments (first one is application name)
//! @param argv   Array of arguments
//!
void MastConfiguration::Update (int argc, const char* argv[])
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
  if (!arguments.empty())
  {
    m_applicationDirectoryPath = ExtractApplicationDirectoryPath(arguments.front());
    LOG(INFO) << "Application path:           " << arguments.front();
    LOG(INFO) << "Application directory path: " << m_applicationDirectoryPath;
  }

  try
  {
    // ---------------- Prepare the parser
    //
    TCLAP::CmdLine cmdLine("Mast: Manager for System On Chip Tests", '=', MAST_VERSION, false);

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

    // Insert in reverse order of the USAGE print()
    TCLAP::ValueArg<std::string> aiProtocolArg        ("",  "protocol",    "Override access interface protocol defined in SIT file",                        false, "",                       "Protocol name", cmdLine);
    TCLAP::ValueArg<std::string> configurationAlgoArg ("a", "config_algo", "Name of configuration algorithm used to select linker  (mux) path",             false, "last_or_default",        "last_lazy|last_or_default|last_or_default_greedy| \"name defined by a plugin\"", cmdLine);
    TCLAP::ValueArg<std::string> checkModelFileArg    ("",  "check_file",  "Defines result of model checking (it is always logged when logger is enabled)", false, "mast_check.txt",         "File path", cmdLine);
    TCLAP::SwitchArg             checkModelArg        ("",  "check",       "Enable model checking (resulting from parsing SIT file)",                       cmdLine, false);
    TCLAP::ValueArg<std::string> logKindArg           ("",  "log_kind",    "Define logger kind",                                                            false, "std",                    &logKindConstraint, cmdLine);
    TCLAP::ValueArg<std::string> logLevelArg          ("",  "log_level",   "Define log level",                                                              false, "info",                   &logLevelConstraint, cmdLine);
    TCLAP::ValueArg<std::string> logFileArg           ("",  "log_file",    "Define logger file path",                                                       false, "mast.log",               "File path", cmdLine);
    TCLAP::SwitchArg             logEnabledArg        ("l", "log",         "Enable logger",                                                                 cmdLine, false);
    TCLAP::MultiArg<std::string> pluginFilesArg       ("",  "plugin",      "Define a plugin file to load",                                                  false, "File path",      cmdLine);
    TCLAP::MultiArg<std::string> pluginDirsArg        ("",  "plugin_dir",  "Define a plugin directory (all plugins in it are loaded)",                      false, "Directory path", cmdLine);
    TCLAP::ValueArg<std::string> sitFilePathArg       ("s", "sit",         "Define SIT that specified SUT model",                                           false, "project.sit",    "File path", cmdLine);
    TCLAP::ValueArg<std::string> configurationFileArg ("c", "conf",        "Define configuration file",                                                     false, "mast.cfg",       "File path", cmdLine);

    // ---------------- Do parse command line arguments
    //
    cmdLine.parse(arguments);

    m_shouldExit = cmdLine.shouldExit();

    if (!m_shouldExit)
    {
      // ---------------- Get parsed values
      //
      auto setOption = [](auto& option, const auto& arg)
      {
        if (arg.isSet())
        {
          option = arg.getValue();
        }
      };

      auto setMappedOption = [](auto& option, const auto& arg, const auto& mapping)
      {
        if (arg.isSet())
        {
          auto pos = mapping.find(arg.getValue());
          if (pos != mapping.cend())  // ==> Should be always true unless mapping is not complete !
          {
            option = pos->second;
          }
          else
          {
            LOG(INFO) << "Internal error; no mapping for flag: " << arg.getValue();
          }
        }
      };

      // ---------------- Save parsed option
      //
      // Begin with configuration file (it has a lower priority relative to command line)
      auto configurationFile = "mast.cfg"s;
      setOption(configurationFile, configurationFileArg);
      ParseConfigurationFile(configurationFile);

      setOption(m_sitFilePath,             sitFilePathArg);
      setOption(m_configurationAlgorithm,  configurationAlgoArg);
      setOption(m_pluginDLLs,              pluginFilesArg);
      setOption(m_pluginDirectories,       pluginDirsArg);
      setOption(m_modelChecking,           checkModelArg);
      setOption(m_modelCheckingFilePath,   checkModelFileArg);
      setOption(m_loggerEnabled,           logEnabledArg);
      setOption(m_loggerFilePath,          logFileArg);
      setOption(m_accessInterfaceProtocol, aiProtocolArg);

      setMappedOption(m_loggerKind,  logKindArg,  loggerKindMapping);
      setMappedOption(m_loggerLevel, logLevelArg, loggerLevelMapping);
    }
  }
  catch(TCLAP::CmdLineParseException& exc)
  {
    LOG(ERROR_LVL) << "Failed to parse command line: " << exc.what();
    m_shouldExit = true;
    throw;
  }
  catch(TCLAP::SpecificationException& exc)
  {
    LOG(ERROR_LVL) << "Internal error: " << exc.what();
    m_shouldExit = true;
    throw;
  }
  catch(std::exception& exc)  // Catch C++ standard exceptions
  {
    LOG(ERROR_LVL) << "Failed to parse command line, getting unexpected exception: " << exc.what();
    m_shouldExit = true;
    throw;
  }
  catch (...)
  {
    LOG(ERROR_LVL) << "Caught unknown exception while parsing command line";
    m_shouldExit = true;
    throw;
  }
}
//
//  End of: MastConfiguration::Update
//---------------------------------------------------------------------------





//===========================================================================
// End of MastConfiguration.cpp
//===========================================================================
