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

  // ---------------- Maps logger Show_Item option to internal value
  //
  static const map<string, mast::GmlPrinterOptions> gmlPrinterOptionsMapping
  {
    {"gml_printer_options",      GmlPrinterOptions::Default},
    {"display_identifiers",      GmlPrinterOptions::DisplayIdentifiers},
    {"display_register_value",   GmlPrinterOptions::DisplayRegisterValue},
    {"display_value_auto",       GmlPrinterOptions::DisplayValueAuto},
    {"show_protocol",            GmlPrinterOptions::ShowProtocol},
    {"show_selector_with_edge",  GmlPrinterOptions::ShowSelectorWithEdge},
    {"show_selector_properties", GmlPrinterOptions::ShowSelectorProperties},
    {"show_selector_tables",     GmlPrinterOptions::ShowSelectorTables},
    {"show_selection_values",    GmlPrinterOptions::ShowSelectionValues},
    {"std",                      GmlPrinterOptions::Std},
    {"all",                      GmlPrinterOptions::All},
  };

//+    ManagerMonitorOptions    m_managerActivityOptions
//+    PrettyPrinterOptions     m_prettyPrintingOptions
//+    mast::ReportMoments      m_gmlReportMoments
//+    mast::ReportMoments      m_prettyPrintingReportMoments

} // End of unnamed namespace



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
    updateString (m_managerActivityFileBasePath, {"Debug",          "Manager_activity",    "File_path"});
    updateBool   (m_modelChecking,               {"Model_checking", "Enable"});
    updateBool   (m_loggerEnabled,               {"Debug",          "Logging",             "Enable"});
    updateBool   (m_gmlPrinting,                 {"Debug",          "Model_GML_printing",  "Enable"});
    updateBool   (m_prettyPrinting,              {"Debug",          "Model_textual_print", "Enable"});
    updateBool   (m_reportManagerActivity,       {"Debug",          "Manager_activity",    "Enable"});

    updateEnum  (m_loggerKind,       loggerKindMapping,        {"Debug", "Logging",            "Logger_Kind"});
    updateEnum  (m_loggerLevel,      loggerLevelMapping,       {"Debug", "Logging",            "Level"});
    updateFlags (m_loggerShownItems, loggerShownItemMapping,   {"Debug", "Logging",            "Shown_items"});
    updateFlags (m_gmlOptions,       gmlPrinterOptionsMapping, {"Debug", "Model_GML_printing", "Options"});

//+    ManagerMonitorOptions    m_managerActivityOptions
//+    PrettyPrinterOptions     m_prettyPrintingOptions
//+    mast::ReportMoments      m_gmlReportMoments
//+    mast::ReportMoments      m_prettyPrintingReportMoments

    updateSequence (m_pluginDLLs, {"Plugin_DLLs"});
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
    TCLAP::MultiArg<std::string> pluginDLLsArg        ("",  "plugins",     "Define plugins to load (may be directory of file path)",                        false, "Directory or file path", cmdLine);
    TCLAP::ValueArg<std::string> sitFilePathArg       ("s", "sit",         "Define SIT that specified SUT model",                                           false, "project.sit",            "File path", cmdLine);
    TCLAP::ValueArg<std::string> configurationFileArg ("c", "conf",        "Define configuration file",                                                     false, "mast.cfg",               "File path", cmdLine);

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
          if (pos != mapping.cend())
          {
            option = pos->second;
          }
          else
          {
            LOG(INFO) << "Invalid flag: " << arg.getValue();
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
      setOption(m_pluginDLLs,              pluginDLLsArg);
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
