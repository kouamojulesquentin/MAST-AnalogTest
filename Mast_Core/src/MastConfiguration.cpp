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
using std::make_shared;

using namespace mast;
using namespace std::chrono_literals;

namespace
{



// ---------------- Maps logger kind arguments to internal value
//
static const map<string, mast::LoggerKind>  loggerKindMapping
{
  {"file",                mast::LoggerKind::File},
  {"cout",                mast::LoggerKind::Cout},
  {"only_errors_on_cerr", mast::LoggerKind::OnlyErrorsOnCerr},
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
  {"std_less",      mast::LoggerShownItems::Std_Less},
  {"std",           mast::LoggerShownItems::Std},
  {"std_more",      mast::LoggerShownItems::Std_More},
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
  {"default",             PrettyPrinterOptions::Default},
  {"verbose",             PrettyPrinterOptions::Verbose},
  {"auto_value",          PrettyPrinterOptions::DisplayValueAuto},
  {"protocol_name",       PrettyPrinterOptions::ShowProtocol},
  {"selection_state",     PrettyPrinterOptions::ShowSelectionState},
  {"selector_properties", PrettyPrinterOptions::ShowSelectorProperties},
  {"selection_value",     PrettyPrinterOptions::ShowSelectionValue},
  {"ignored_nodes",       PrettyPrinterOptions::ShowNodeIsIgnored},
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
  : m_minTimeBetweenCycles        (10ms)
  , m_maxTimeBetweenCycles        (1s)
  , m_sitFilePath                 ("")
  , m_configurationAlgorithm      ("last_or_default")
  , m_aiProtocolName              ("")
  , m_aiProtocolParameters        ("")
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


//! Extracts from current application path (usually got from argv[0]), the, parent, directory path
//!
//! @note This may not work properly if the application path is reduced to application name
//!
//! @param applicationPath  The application path
//!
//! @return Extracted path or "." when no path has been found
string MastConfiguration::ExtractApplicationDirectoryPath (const string& applicationPath)
{
  return Utility::ExtractDirectoryPath(applicationPath);
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
  CHECK_PARAMETER_NOT_EMPTY (configurationFile, "Configuration file path cannot be empty");
  CHECK_FILE_EXISTS_EX      (configurationFile, "Configuration file: '");

  auto yaml = Utility::ReadTextFile(configurationFile);
  CHECK_VALUE_NOT_EMPTY(yaml, "Empty configuration file: \"" + configurationFile + "\"" );

  ParseYamlConfiguration(yaml);
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
      if (ok && !gotString.empty())
      {
        option = gotString;
      }
    };

    // Enum flags updater
    auto updateEnum = [&yaml, &gotString, &makePath](auto& option, const auto& mapping, initializer_list<const char*> pathItems)
    {
      auto ok = false;
      tie(ok, gotString) = yaml.TryGetAsString (makePath(pathItems));
      if (ok && !gotString.empty())
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
    auto updateFlags = [&yaml, &gotFlags, &makePath, &updateEnum]
                       (auto& option, const auto& mapping, initializer_list<const char*> pathItems)
    {
      auto ok = false;
      tie(ok, gotFlags) = yaml.TryGetAsStringVector (makePath(pathItems));
      if (ok)
      {
        ForceValue(option, 0);

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
      else  // Try with plain string (single value)
      {
        updateEnum(option, mapping, pathItems);
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

    // Duration updater
    auto updateDuration = [&yaml, &gotString, &makePath](auto& option, initializer_list<const char*> pathItems)
    {
      auto ok = false;
      tie(ok, gotString) = yaml.TryGetAsString (makePath(pathItems));
      if (ok && !gotString.empty())
      {
        option = Utility::ToMilliseconds(gotString);
      }
    };


    updateString (m_sitFilePath,                 {"SIT_file_path"});
    updateString (m_aiProtocolName,              {"Access_interface_protocol", "Name"});
    updateString (m_aiProtocolParameters,        {"Access_interface_protocol", "Parameters"});
    updateString (m_configurationAlgorithm,      {"Configuration_algorithm"});
    updateString (m_modelCheckingFilePath,       {"Model_checking",            "File_path"});
    updateString (m_gmlFilePath,                 {"Debug",                     "Model_GML_printing",  "File_path"});
    updateString (m_gmlGraphName,                {"Debug",                     "Model_GML_printing",  "Graph_name"});
    updateString (m_prettyPrintingFilePath,      {"Debug",                     "Model_textual_print", "File_path"});
    updateString (m_loggerFilePath,              {"Debug",                     "Logging",             "File_path"});
    updateString (m_managerActivityFileBasePath, {"Debug",                     "Manager_activity",    "File_base_name"});
    updateBool   (m_modelChecking,               {"Model_checking",            "Enable"});
    updateBool   (m_loggerEnabled,               {"Debug",                     "Logging",             "Enable"});
    updateBool   (m_gmlPrinting,                 {"Debug",                     "Model_GML_printing",  "Enable"});
    updateBool   (m_prettyPrinting,              {"Debug",                     "Model_textual_print", "Enable"});
    updateBool   (m_reportManagerActivity,       {"Debug",                     "Manager_activity",    "Enable"});

    updateEnum  (m_loggerLevel,                 loggerLevelMapping,            {"Debug", "Logging",             "Level"});
    updateFlags (m_loggerKind,                  loggerKindMapping,             {"Debug", "Logging",             "Kind"});
    updateFlags (m_loggerShownItems,            loggerShownItemMapping,        {"Debug", "Logging",             "Shown_items"});
    updateFlags (m_gmlOptions,                  gmlPrinterOptionsMapping,      {"Debug", "Model_GML_printing",  "Options"});
    updateFlags (m_gmlReportMoments,            reportMomentsMapping,          {"Debug", "Model_GML_printing",  "Moments"});
    updateFlags (m_prettyPrintingOptions,       prettyPrinterOptionsMapping,   {"Debug", "Model_textual_print", "Options"});
    updateFlags (m_prettyPrintingReportMoments, reportMomentsMapping,          {"Debug", "Model_textual_print", "Moments"});
    updateFlags (m_managerActivityOptions,      managerActivityOptionsMapping, {"Debug", "Manager_activity",    "Options"});

    updateSequence (m_pluginDLLs,        {"Plugins", "Files"});
    updateSequence (m_pluginDirectories, {"Plugins", "Directories"});

    updateDuration (m_minTimeBetweenCycles, {"Manager", "Min_time_between_cycles"});
    updateDuration (m_maxTimeBetweenCycles, {"Manager", "Max_time_between_cycles"});
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

    if (!m_cmdLineOutput)
    {
      m_cmdLineOutput = make_shared<TCLAP::StdOutput>(160u);
    }
    cmdLine.setOutput(m_cmdLineOutput.get());
    cmdLine.automaticExit(m_automaticExit);

    // ---------------- Prepare accepted arguments
    //
    auto makeAllowedSet = [](const auto& mapper)
    {
      vector<string> allowed;
      for (const auto& element : mapper)
      {
        allowed.emplace_back(element.first);
      }
      return allowed;
    };

    TCLAP::ValuesConstraint<string> logLevelConstraint(makeAllowedSet(loggerLevelMapping));
    TCLAP::ValuesConstraint<string> logKindConstraint (makeAllowedSet(loggerKindMapping));
    TCLAP::ValuesConstraint<string> logShowConstraint (makeAllowedSet(loggerShownItemMapping));

    // Insert in reverse order of the USAGE print()
    TCLAP::ValueArg<std::string> aiProtocolNameArg       ("",  "protocol_name",       "Overrides access interface protocol defined in SIT file",                       false,   "", "Protocol name",         cmdLine);
    TCLAP::ValueArg<std::string> aiProtocolParametersArg ("",  "protocol_parameters", "Optional access interface protocol parameters",                                 false,   "", "Protocol parameter(s)", cmdLine);
    TCLAP::ValueArg<std::string> minCycleArg             ("",  "min_cycle",           "Minimal time between two I/O cycles",                                                                                                                  false,   "1ms", "Positive integer with 'ms' or 's' suffix", cmdLine);
    TCLAP::ValueArg<std::string> maxCycleArg             ("",  "max_cycle",           "Maximal time between I/O cycles. It corresponds to the maximal delay between an iApply assertion and its execution",                                   false,   "1s",  "Positive integer with 'ms' or 's' suffix", cmdLine);
    TCLAP::ValueArg<std::string> configurationAlgoArg    ("a", "config_algo",         "Name of configuration algorithm used to select linker  (mux) path",             false,   "last_or_default",   "last_lazy|last_or_default|last_or_default_greedy| \"name defined by a plugin\"", cmdLine);
    TCLAP::ValueArg<std::string> checkModelFileArg       ("",  "check_file",          "Defines result of model checking (it is always logged when logger is enabled)", false,   "mast_check.txt",    "File path", cmdLine);
    TCLAP::SwitchArg             checkModelArg           ("",  "check",               "Enables model checking (resulting from parsing SIT file)",                       cmdLine, false);
    TCLAP::MultiArg<std::string> logKindArg              ("",  "log_kind",            "Defines logger kind",                                                            false,   &logKindConstraint,  cmdLine);
    TCLAP::MultiArg<std::string> logLevelArg             ("",  "log_level",           "Defines log level",                                                              false,   &logLevelConstraint, cmdLine);
    TCLAP::MultiArg<std::string> logShowArg              ("",  "log_show",            "Defines log shown items",                                                        false,   &logShowConstraint,  cmdLine);
    TCLAP::ValueArg<std::string> logFileArg              ("",  "log_file",            "Defines logger file path",                                                       false,   "mast.log",          "File path", cmdLine);
    TCLAP::SwitchArg             logEnabledArg           ("l", "log",                 "Enables logger",                                                                 cmdLine, false);
    TCLAP::MultiArg<std::string> pluginFilesArg          ("",  "plugin",              "Defines a plugin file to load",                                                  false,   "File path",         cmdLine);
    TCLAP::MultiArg<std::string> pluginDirsArg           ("",  "plugin_dir",          "Defines a plugin directory (all plugins in it are loaded)",                      false,   "Directory path",    cmdLine);
    TCLAP::ValueArg<std::string> sitFilePathArg          ("s", "sit",                 "Defines SIT that specified SUT model",                                           false,   "project.sit",       "File path", cmdLine);
    TCLAP::ValueArg<std::string> configurationFileArg    ("c", "conf",                "Defines configuration file",                                                     false,   "mast.cfg",          "File path", cmdLine);

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

      auto setDurationOption = [](auto& option, const auto& arg)
      {
        if (arg.isSet())
        {
          option = Utility::ToMilliseconds(arg.getValue());
        }
      };


      auto setMappedOptionFlags = [](auto& option, const auto& arg, const auto& mapping)
      {
        if (arg.isSet())
        {
          ForceValue(option, 0);

          const auto& options = arg.getValue();

          for (const auto& optionString : options)
          {
            auto pos = mapping.find(optionString);
            if (pos != mapping.cend())  // ==> Should be always true unless mapping is not complete !
            {
              option |= pos->second;
            }
            else
            {
              LOG(INFO) << "Internal error; no mapping for flag: " << optionString;
            }
          }
        }
      };

      // ---------------- Save parsed option
      //
      // Begin with configuration file (it has a lower priority relative to command line)
      auto configurationFile = ""s;
      setOption(configurationFile, configurationFileArg);
      if (!configurationFile.empty())
      {
        ParseConfigurationFile(configurationFile);
      }

      setOption(m_sitFilePath,            sitFilePathArg);
      setOption(m_configurationAlgorithm, configurationAlgoArg);
      setOption(m_pluginDLLs,             pluginFilesArg);
      setOption(m_pluginDirectories,      pluginDirsArg);
      setOption(m_modelChecking,          checkModelArg);
      setOption(m_modelCheckingFilePath,  checkModelFileArg);
      setOption(m_loggerEnabled,          logEnabledArg);
      setOption(m_loggerFilePath,         logFileArg);
      setOption(m_aiProtocolName,         aiProtocolNameArg);
      setOption(m_aiProtocolParameters,   aiProtocolParametersArg);

      setDurationOption(m_minTimeBetweenCycles, minCycleArg);
      setDurationOption(m_maxTimeBetweenCycles, maxCycleArg);

      setMappedOptionFlags(m_loggerKind,       logKindArg,  loggerKindMapping);
      setMappedOptionFlags(m_loggerLevel,      logLevelArg, loggerLevelMapping);
      setMappedOptionFlags(m_loggerShownItems, logShowArg,  loggerShownItemMapping);
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
