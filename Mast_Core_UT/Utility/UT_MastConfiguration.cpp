//===========================================================================
//                           UT_MastConfiguration.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_MastConfiguration.cpp
//!
//! Implements test fixture for testing MastConfiguration
//!
//===========================================================================

#include "UT_MastConfiguration.hpp"
#include "MastConfiguration.hpp"
#include "MastConfig.hpp"
#include "Utility.hpp"
#include "EnumsUtility.hpp"
#include "TestUtilities.hpp"

#include <tclap/ArgException.h>
#include <tclap/StreamOutput.h>

#include <cxxtest/ValueTraits.h>
#include <cxxtest/traits/STL11_Traits.h>
#include <vector>
#include <string>
#include <experimental/string_view>
#include <sstream>
#include <memory>
#include <initializer_list>

using std::vector;
using std::string;
using std::experimental::string_view;
using std::ostringstream;
using std::shared_ptr;
using std::make_shared;
using std::initializer_list;

using namespace std::chrono_literals;
using namespace mast;


namespace
{
  //! Returns path of a file accessible by test runner
  //!
  //! @param fileName   File name used by test
  //!
  string GetTestFilePath (string_view fileName, bool checkExists = true)
  {
    return test::GetTestFilePath({"Mast_Core"s, "UT_TestFiles"s, string(fileName)}, checkExists);
  }
  //
  //  End of: GetTestFilePath
  //---------------------------------------------------------------------------

  void Check_DefaultConfiguration(const MastConfiguration& sut)
  {
    TS_ASSERT_FALSE (sut.LoggerEnabled());
    TS_ASSERT_FALSE (sut.ModelChecking());
    TS_ASSERT_FALSE (sut.GmlPrinting());
    TS_ASSERT_FALSE (sut.PrettyPrinting());
    TS_ASSERT_FALSE (sut.ReportManagerActivity());

    TS_ASSERT_EQUALS (sut.SitFilePath(),                       "");
    TS_ASSERT_EQUALS (sut.AccessInterfaceProtocolName(),       "");
    TS_ASSERT_EQUALS (sut.AccessInterfaceProtocolParameters(), "");
    TS_ASSERT_EQUALS (sut.ConfigurationAlgorithm(),            "last_or_default");
    TS_ASSERT_EQUALS (sut.GmlFilePath(),                       "MastModel.gml");
    TS_ASSERT_EQUALS (sut.GmlGraphName(),                      "DUT");
    TS_ASSERT_EQUALS (sut.LoggerFilePath(),                    "Mast.log");
    TS_ASSERT_EQUALS (sut.ManagerActivityFileBasePath(),       "DUT");
    TS_ASSERT_EQUALS (sut.ModelCheckingFilePath(),             "");
    TS_ASSERT_EQUALS (sut.PrettyPrintFilePath(),               "MastModel.txt");

    TS_ASSERT_EQUALS (sut.GmlOptions(),                GmlPrinterOptions::Std);
    TS_ASSERT_EQUALS (sut.ManagerActivityOptions(),    ManagerMonitorOptions::Std);
    TS_ASSERT_EQUALS (sut.PrettyPrintOptions(),        PrettyPrinterOptions::Std);
    TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::File);
    TS_ASSERT_EQUALS (sut.LoggerLevel(),               mast::LoggerLevel::Info);
    TS_ASSERT_EQUALS (sut.LoggerShownItems(),          mast::LoggerShownItems::Std_Less);
    TS_ASSERT_EQUALS (sut.GmlReportMoments(),          mast::ReportMoments::AfterModelParsing);
    TS_ASSERT_EQUALS (sut.PrettyPrintReportMoments(),  mast::ReportMoments::AfterModelParsing);

    CxxTest::setAbortTestOnFail(true);
    const auto& pluginDirs = sut.PluginDirectories();
    TS_ASSERT_EQUALS (pluginDirs.size(),  1u);
    TS_ASSERT_EQUALS (pluginDirs.front(), PLUGINS_DIRECTORY_NAME);
  }
} // End of unnamed namespace

//! Checks MastConfiguration::constructor()
//!
void UT_MastConfiguration::test_constructor ()
{
  // ---------------- Exercise
  //
  MastConfiguration sut;

  // ---------------- Verify
  //
  Check_DefaultConfiguration(sut);

  TS_ASSERT_FALSE (sut.ShouldExit());
}



//! Checks MastConfiguration::ParseYamlConfiguration() when yaml is empty
//!
void UT_MastConfiguration::test_ParseYamlConfiguration_Empty ()
{
  // ---------------- Setup
  //
  MastConfiguration sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.ParseYamlConfiguration(""));

  // ---------------- Verify
  //
  Check_DefaultConfiguration(sut);
}


//! Checks MastConfiguration::ParseYamlConfiguration() when there is syntax error(s)
//!
void UT_MastConfiguration::test_ParseYamlConfiguration_SyntaxErrors ()
{
  // ---------------- Setup
  //
  MastConfiguration sut;
  string yaml("Mast_Options:\n"
              "  Foo: false\n"
              "    Bar: true\n" // yaml-cpp: error at line 3, column 14: illegal map value.
             );

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.ParseYamlConfiguration(yaml), std::exception);
}


//! Checks MastConfiguration::ParseYamlConfiguration() when there is unsupported flags
//!
void UT_MastConfiguration::test_ParseYamlConfiguration_UnsupportedFlags ()
{
  // ---------------- Setup
  //
  MastConfiguration sut;
  string yaml("Mast_Options:\n"                                   // 01
              "  Debug:\n"                                        // 02
              "    Logging:\n"                                    // 03
              "      Shown_items: [function_name, picoseconds]\n" // 04  ==> There is no picoseconds flag
             );

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.ParseYamlConfiguration(yaml));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.LoggerShownItems(), mast::LoggerShownItems::FunctionName);
}


//! Checks MastConfiguration::ParseYamlConfiguration() when yaml provides only settings for some options
//!
void UT_MastConfiguration::test_ParseYamlConfiguration_PartialConfiguration ()
{
  // ---------------- Setup
  //
  MastConfiguration sut;
  string yaml(
              "Mast_Options:\n"
              "  SIT_file_path: myDesign.sit\n"
              "  Configuration_algorithm: last_lazy\n"
              "  Plugins:  \n"
              "    Files:       [myPlugin.so] \n"
              "    Directories: [Plugins_1, Plugins_2] \n"
              "  Debug:\n"
              "    Logging:\n"
              "      Enable: true\n"
              "      Kind:   [only_errors_on_cerr]\n"
              "      File_path:   myProject.log\n"
              "      Shown_items: [date, time, microseconds, level, thread_id]\n"
              "      Level: debug\n"
             );  // There must be a space after the colon

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.ParseYamlConfiguration(yaml));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.ShouldExit());
  TS_ASSERT_TRUE  (sut.LoggerEnabled());
  TS_ASSERT_FALSE (sut.ModelChecking());
  TS_ASSERT_FALSE (sut.GmlPrinting());
  TS_ASSERT_FALSE (sut.PrettyPrinting());
  TS_ASSERT_FALSE (sut.ReportManagerActivity());

  TS_ASSERT_EQUALS (sut.SitFilePath(),                 "myDesign.sit");
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocolName(),     "");
  TS_ASSERT_EQUALS (sut.ConfigurationAlgorithm(),      "last_lazy");
  TS_ASSERT_EQUALS (sut.GmlFilePath(),                 "MastModel.gml");
  TS_ASSERT_EQUALS (sut.GmlGraphName(),                "DUT");
  TS_ASSERT_EQUALS (sut.LoggerFilePath(),              "myProject.log");
  TS_ASSERT_EQUALS (sut.ManagerActivityFileBasePath(), "DUT");
  TS_ASSERT_EQUALS (sut.ModelCheckingFilePath(),       "");
  TS_ASSERT_EQUALS (sut.PrettyPrintFilePath(),         "MastModel.txt");

  TS_ASSERT_EQUALS (sut.GmlOptions(),                GmlPrinterOptions::Std);
  TS_ASSERT_EQUALS (sut.ManagerActivityOptions(),    ManagerMonitorOptions::Std);
  TS_ASSERT_EQUALS (sut.PrettyPrintOptions(),        PrettyPrinterOptions::Std);
  TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::OnlyErrorsOnCerr);
  TS_ASSERT_EQUALS (sut.LoggerLevel(),               mast::LoggerLevel::Debug);
  TS_ASSERT_EQUALS (sut.LoggerShownItems(),          mast::LoggerShownItems::Date
                                                   | mast::LoggerShownItems::Time
                                                   | mast::LoggerShownItems::Microseconds
                                                   | mast::LoggerShownItems::Level
                                                   | mast::LoggerShownItems::ThreadId);

  TS_ASSERT_EQUALS (sut.GmlReportMoments(),          mast::ReportMoments::AfterModelParsing);
  TS_ASSERT_EQUALS (sut.PrettyPrintReportMoments(),  mast::ReportMoments::AfterModelParsing);

  CxxTest::setAbortTestOnFail(true);
  const auto& plugins = sut.PluginDLLs();
  TS_ASSERT_EQUALS (plugins.size(),  1u);
  TS_ASSERT_EQUALS (plugins[0], "myPlugin.so");

  const auto& pluginDirs = sut.PluginDirectories();
  TS_ASSERT_EQUALS (pluginDirs.size(),  2u);
  TS_ASSERT_EQUALS (pluginDirs[0], "Plugins_1");
  TS_ASSERT_EQUALS (pluginDirs[1], "Plugins_2");
}


//! Checks MastConfiguration::ParseYamlConfiguration() when yaml provides settings for all options
//!
void UT_MastConfiguration::test_ParseYamlConfiguration_FullConfiguration ()
{
  // ---------------- Setup
  //
  MastConfiguration sut;
  string yaml(
              "Mast_Options:\n"                                                            // 01
              "  SIT_file_path: myProject.sit\n"                                           // 02
              "  Configuration_algorithm: myConfiguration\n"                               // 03
              "  Access_interface_protocol: \n"                                            // 04
              "    Name: myProtocol\n"                                                     // 05
              "    Parameters: myProtocolParams\n"                                         // 06
              "  Plugins:  \n"                                                             // 07
              "    Files:       [myPlugin.dll] \n"                                         // 08
              "    Directories: [Plugins] \n"                                              // 09
              "  Manager: \n"                                                              // 10
              "    Min_time_between_cycles: 5ms\n"                                         // 11
              "    Max_time_between_cycles: 2s\n"                                          // 12
              "  Model_checking: \n"                                                       // 13
              "    Enable: true\n"                                                         // 14
              "    File_path: modelCheck.txt\n"                                            // 15
              "  Debug:\n"                                                                 // 16
              "    Logging:\n"                                                             // 17
              "      Enable: true\n"                                                       // 18
              "      Kind: only_errors_on_cerr\n"                                          // 19
              "      File_path:   myProject.log\n"                                         // 20
              "      Shown_items: [microseconds, level, thread_id, function_name]\n"       // 21
              "      Level: warning\n"                                                     // 22
              "    Model_GML_printing: \n"                                                 // 23
              "      Enable: true\n"                                                       // 24
              "      Moments:\n"                                                           // 25
              "        - after_model_parsing\n"                                            // 26
              "        - after_configuration\n"                                            // 27
              "      File_path: ./myproject\n"                                             // 28
              "      Graph_name: Foo\n"                                                    // 29
              "      Options: std\n"                                                       // 30
              "    Model_textual_print: \n"                                                // 31
              "      Enable: true\n"                                                       // 32
              "      Moments:\n"                                                           // 33
              "        - after_model_parsing\n"                                            // 34
              "        - before_configuration\n"                                           // 35
              "      File_path: ../Tests/Bar\n"                                            // 36
              "      Options:         \n"                                                  // 37
              "         - verbose \n"                                                      // 38
              "         - auto_value \n"                                                   // 39
              "         - protocol_name \n"                                                // 40
              "         - selection_state \n"                                              // 41
              "         - selection_value \n"                                              // 42
              "         - selector_properties \n"                                          // 43
              "         - ignored_nodes  \n"                                               // 44
              "    Model_SIT_export: \n"                                                   // 45
              "      Enable: true\n"                                                       // 46
              "      File_path: export.sit\n"                                              // 47
              "    Manager_activity:  \n"                                                  // 48
              "      Enable: true\n"                                                       // 49
              "      File_base_name: managerActivity\n"                                    // 50
              "      Options: [verbose, app_thread_creation, PDL_commands, data_cycles]\n" // 51
              "Plugins_Options: ""\n"                                                      // 52
             );

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.ParseYamlConfiguration(yaml));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.ShouldExit());
  TS_ASSERT_TRUE  (sut.LoggerEnabled());
  TS_ASSERT_TRUE  (sut.ModelChecking());
  TS_ASSERT_TRUE  (sut.GmlPrinting());
  TS_ASSERT_TRUE  (sut.PrettyPrinting());
  TS_ASSERT_TRUE  (sut.SitExport());
  TS_ASSERT_TRUE  (sut.ReportManagerActivity());

  TS_ASSERT_EQUALS (sut.MinTimeBetweenCycles(),              5ms);
  TS_ASSERT_EQUALS (sut.MaxTimeBetweenCycles(),              2000ms);

  TS_ASSERT_EQUALS (sut.SitFilePath(),                       "myProject.sit");
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocolName(),       "myProtocol");
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocolParameters(), "myProtocolParams");
  TS_ASSERT_EQUALS (sut.ConfigurationAlgorithm(),            "myConfiguration");
  TS_ASSERT_EQUALS (sut.SitExportFilePath(),                 "export.sit");
  TS_ASSERT_EQUALS (sut.GmlFilePath(),                       "./myproject");
  TS_ASSERT_EQUALS (sut.GmlGraphName(),                      "Foo");
  TS_ASSERT_EQUALS (sut.LoggerFilePath(),                    "myProject.log");
  TS_ASSERT_EQUALS (sut.ManagerActivityFileBasePath(),       "managerActivity");
  TS_ASSERT_EQUALS (sut.ModelCheckingFilePath(),             "modelCheck.txt");
  TS_ASSERT_EQUALS (sut.PrettyPrintFilePath(),               "../Tests/Bar");

  TS_ASSERT_EQUALS (sut.GmlOptions(),                GmlPrinterOptions::Std);
  TS_ASSERT_EQUALS (sut.ManagerActivityOptions(),    ManagerMonitorOptions::Verbose           |
                                                     ManagerMonitorOptions::AppThreadCreation |
                                                     ManagerMonitorOptions::PDLCommands       |
                                                     ManagerMonitorOptions::DataCycles);
  TS_ASSERT_EQUALS (sut.PrettyPrintOptions(),        PrettyPrinterOptions::Verbose                |
                                                     PrettyPrinterOptions::DisplayValueAuto       |
                                                     PrettyPrinterOptions::ShowProtocol           |
                                                     PrettyPrinterOptions::ShowSelectionState     |
                                                     PrettyPrinterOptions::ShowSelectionValue     |
                                                     PrettyPrinterOptions::ShowSelectorProperties |
                                                     PrettyPrinterOptions::ShowNodeIsIgnored);
  TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::OnlyErrorsOnCerr);
  TS_ASSERT_EQUALS (sut.LoggerLevel(),               mast::LoggerLevel::Warning);
  TS_ASSERT_EQUALS (sut.LoggerShownItems(),          mast::LoggerShownItems::Microseconds |
                                                     mast::LoggerShownItems::Level        |
                                                     mast::LoggerShownItems::ThreadId     |
                                                     mast::LoggerShownItems::FunctionName);
  TS_ASSERT_EQUALS (sut.GmlReportMoments(),          mast::ReportMoments::AfterModelParsing | mast::ReportMoments::AfterConfiguration);
  TS_ASSERT_EQUALS (sut.PrettyPrintReportMoments(),  mast::ReportMoments::AfterModelParsing | mast::ReportMoments::BeforeConfiguration);

  CxxTest::setAbortTestOnFail(true);
  const auto& plugins = sut.PluginDLLs();
  TS_ASSERT_EQUALS (plugins.size(),  1u);
  TS_ASSERT_EQUALS (plugins[0], "myPlugin.dll");

  const auto& pluginDirs = sut.PluginDirectories();
  TS_ASSERT_EQUALS (pluginDirs.size(),  1u);
  TS_ASSERT_EQUALS (pluginDirs[0], "Plugins");
}


//! Checks MastConfiguration::ParseConfigurationFile() giving empty path
//!
void UT_MastConfiguration::test_ParseConfigurationFile_EmptyPath ()
{
  // ---------------- Setup
  //
  MastConfiguration sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.ParseConfigurationFile(""), std::exception);
}

//! Checks MastConfiguration::ParseConfigurationFile() from empty yaml file
//!
void UT_MastConfiguration::test_ParseConfigurationFile_Empty ()
{
  // ---------------- Setup
  //
  MastConfiguration sut;
  auto              filePath = GetTestFilePath("UT_MastConfiguration_empty.yml");

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.ParseConfigurationFile(filePath), std::runtime_error);
}


//! Checks MastConfiguration::ParseConfigurationFile() from "default" yaml file
//!
void UT_MastConfiguration::test_ParseConfigurationFile_Default ()
{
  // ---------------- Setup
  //
  MastConfiguration sut;
  auto              filePath = GetTestFilePath("UT_MastConfiguration_default.yml");

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.ParseConfigurationFile(filePath));

  // ---------------- Verify
  //
  Check_DefaultConfiguration(sut);
}


//! Checks MastConfiguration::ParseConfigurationFile() from "good" yaml file
//!
void UT_MastConfiguration::test_ParseConfigurationFile_Good ()
{
  // ---------------- Setup
  //
  MastConfiguration sut;

  auto filePath = GetTestFilePath("UT_MastConfiguration_good.yml");

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.ParseConfigurationFile(filePath));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.ShouldExit());
  TS_ASSERT_TRUE  (sut.LoggerEnabled());
  TS_ASSERT_TRUE  (sut.ModelChecking());
  TS_ASSERT_TRUE  (sut.GmlPrinting());
  TS_ASSERT_TRUE  (sut.PrettyPrinting());
  TS_ASSERT_TRUE  (sut.ReportManagerActivity());

  TS_ASSERT_EQUALS (sut.SitFilePath(),                       "myProject.sit");
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocolName(),       "myProtocol");
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocolParameters(), "myProtocolParams");
  TS_ASSERT_EQUALS (sut.ConfigurationAlgorithm(),            "myConfiguration");
  TS_ASSERT_EQUALS (sut.GmlFilePath(),                       "./myproject");
  TS_ASSERT_EQUALS (sut.GmlGraphName(),                      "Foo");
  TS_ASSERT_EQUALS (sut.LoggerFilePath(),                    "myProject.log");
  TS_ASSERT_EQUALS (sut.ManagerActivityFileBasePath(),       "managerActivity");
  TS_ASSERT_EQUALS (sut.ModelCheckingFilePath(),             "modelCheck.txt");
  TS_ASSERT_EQUALS (sut.PrettyPrintFilePath(),               "../Tests/Bar");

  TS_ASSERT_EQUALS (sut.GmlOptions(),                GmlPrinterOptions::Std);
  TS_ASSERT_EQUALS (sut.ManagerActivityOptions(),    ManagerMonitorOptions::Verbose           |
                                                     ManagerMonitorOptions::AppThreadCreation |
                                                     ManagerMonitorOptions::PDLCommands       |
                                                     ManagerMonitorOptions::DataCycles);
  TS_ASSERT_EQUALS (sut.PrettyPrintOptions(),        PrettyPrinterOptions::Verbose                |
                                                     PrettyPrinterOptions::DisplayValueAuto       |
                                                     PrettyPrinterOptions::ShowProtocol           |
                                                     PrettyPrinterOptions::ShowSelectionState     |
                                                     PrettyPrinterOptions::ShowSelectionValue     |
                                                     PrettyPrinterOptions::ShowSelectorProperties |
                                                     PrettyPrinterOptions::ShowNodeIsIgnored);
  TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::OnlyErrorsOnCerr);
  TS_ASSERT_EQUALS (sut.LoggerLevel(),               mast::LoggerLevel::Warning);
  TS_ASSERT_EQUALS (sut.LoggerShownItems(),          mast::LoggerShownItems::Microseconds |
                                                     mast::LoggerShownItems::Level        |
                                                     mast::LoggerShownItems::ThreadId     |
                                                     mast::LoggerShownItems::FunctionName);
  TS_ASSERT_EQUALS (sut.GmlReportMoments(),          mast::ReportMoments::AfterModelParsing | mast::ReportMoments::AfterConfiguration);
  TS_ASSERT_EQUALS (sut.PrettyPrintReportMoments(),  mast::ReportMoments::AfterModelParsing | mast::ReportMoments::BeforeConfiguration);

  CxxTest::setAbortTestOnFail(true);
  const auto& plugins = sut.PluginDLLs();
  TS_ASSERT_EQUALS (plugins.size(),  1u);
  TS_ASSERT_EQUALS (plugins[0], "myOwnPlugin.dll");

  const auto& pluginDirs = sut.PluginDirectories();
  TS_ASSERT_EQUALS (pluginDirs.size(), 1u);
  TS_ASSERT_EQUALS (pluginDirs[0], "Plugins");
}



//! Checks MastConfiguration::Update() giving empty vector for the command line arguments
//!
void UT_MastConfiguration::test_Update_NoArguments ()
{
  // ---------------- Setup
  //
  vector<string>    arguments;
  MastConfiguration sut;
  sut.AutomaticExit(false);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS (sut.Update(arguments), TCLAP::CmdLineParseException);

  // ---------------- Verify
  //
  Check_DefaultConfiguration(sut);

  TS_ASSERT_TRUE (sut.ShouldExit());
}


//! Checks MastConfiguration::Update() giving empty vector for the command line arguments
//!
void UT_MastConfiguration::test_Update_OnlyAppName ()
{
  // ---------------- Setup
  //
  vector<string>    arguments{ "MyApp" };
  MastConfiguration sut;
  sut.AutomaticExit(false);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Update(arguments));

  // ---------------- Verify
  //
  Check_DefaultConfiguration(sut);

  TS_ASSERT_FALSE (sut.ShouldExit());
}


//! Checks MastConfiguration::Update() providing bad argument(s)
//!
void UT_MastConfiguration::test_Update_BadArguments ()
{
  // ---------------- Setup
  //
  ostringstream     stdStream;
  ostringstream     errStream;
  auto              streamOutput = make_shared<TCLAP::StreamOutput>(stdStream, errStream);
  MastConfiguration sut;

  sut.CommandLineParserOutput(streamOutput);
  sut.AutomaticExit(false);

  vector<string> arguments{ "MyApp", "-s myDesign.sit" };

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Update(arguments));

  // ---------------- Verify
  //
  Check_DefaultConfiguration(sut);

  TS_ASSERT_TRUE (sut.ShouldExit());

  auto stdMessage = stdStream.str();
  auto errMessage = errStream.str();

  TS_ASSERT_CONTAINS (stdMessage, "Usage is:")
  TS_ASSERT_CONTAINS (errMessage, "Parsing ERROR: Argument:")
}


//! Checks MastConfiguration::Update() providing unsupported flag
//!
void UT_MastConfiguration::test_Update_UnsupportedFlags ()
{
  // ---------------- Setup
  //
  ostringstream     stdStream;
  ostringstream     errStream;
  auto              streamOutput = make_shared<TCLAP::StreamOutput>(stdStream, errStream);
  MastConfiguration sut;

  sut.CommandLineParserOutput(streamOutput);
  sut.AutomaticExit(false);

  vector<string> arguments{ "MyApp", "--log_level=fatal" }; // ==> fatal is not supported

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Update(arguments));

  // ---------------- Verify
  //
  Check_DefaultConfiguration(sut);

  TS_ASSERT_TRUE (sut.ShouldExit());

  auto stdMessage = stdStream.str();
  auto errMessage = errStream.str();

  TS_ASSERT_CONTAINS (stdMessage, "Usage is:")
  TS_ASSERT_CONTAINS (errMessage, "Parsing ERROR: Argument:")
}



//! Checks MastConfiguration::Update() giving only short version of command line arguments
//!
void UT_MastConfiguration::test_Update_ShortSwitches ()
{
  // ---------------- Setup
  //
  auto              configFilePath = GetTestFilePath("UT_MastConfiguration_minimal.yml");
  vector<string>    arguments{ "MyAppWithLongName", "-s=myDesign.sit", "-a=last_lazy", "-l", "-c=" + configFilePath };
  ostringstream     stdStream;
  ostringstream     errStream;
  auto              streamOutput = make_shared<TCLAP::StreamOutput>(stdStream, errStream);
  MastConfiguration sut;

  sut.CommandLineParserOutput(streamOutput);
  sut.AutomaticExit(false);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Update(arguments));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.ShouldExit());
  TS_ASSERT_TRUE  (sut.LoggerEnabled());
  TS_ASSERT_FALSE (sut.ModelChecking());
  TS_ASSERT_FALSE (sut.GmlPrinting());
  TS_ASSERT_FALSE (sut.PrettyPrinting());
  TS_ASSERT_FALSE (sut.ReportManagerActivity());

  TS_ASSERT_EQUALS (sut.SitFilePath(),                 "myDesign.sit");
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocolName(), "");
  TS_ASSERT_EQUALS (sut.ConfigurationAlgorithm(),      "last_lazy");
  TS_ASSERT_EQUALS (sut.GmlFilePath(),                 "MastModel.gml");
  TS_ASSERT_EQUALS (sut.GmlGraphName(),                "DUT");
  TS_ASSERT_EQUALS (sut.LoggerFilePath(),              "Mast.log");
  TS_ASSERT_EQUALS (sut.ManagerActivityFileBasePath(), "DUT");
  TS_ASSERT_EQUALS (sut.ModelCheckingFilePath(),       "");
  TS_ASSERT_EQUALS (sut.PrettyPrintFilePath(),         "MastModel.txt");

  TS_ASSERT_EQUALS (sut.GmlOptions(),                GmlPrinterOptions::Std);
  TS_ASSERT_EQUALS (sut.ManagerActivityOptions(),    ManagerMonitorOptions::Std);
  TS_ASSERT_EQUALS (sut.PrettyPrintOptions(),        PrettyPrinterOptions::Std);
  TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::File);
  TS_ASSERT_EQUALS (sut.LoggerLevel(),               mast::LoggerLevel::Info);
  TS_ASSERT_EQUALS (sut.LoggerShownItems(),          mast::LoggerShownItems::Std_Less);
  TS_ASSERT_EQUALS (sut.GmlReportMoments(),          mast::ReportMoments::AfterModelParsing);
  TS_ASSERT_EQUALS (sut.PrettyPrintReportMoments(),  mast::ReportMoments::AfterModelParsing);

  CxxTest::setAbortTestOnFail(true);
  const auto& pluginDirs = sut.PluginDirectories();
  TS_ASSERT_EQUALS (pluginDirs.size(),  1u);
  TS_ASSERT_EQUALS (pluginDirs.front(), PLUGINS_DIRECTORY_NAME);

  TS_ASSERT_EMPTY (stdStream.str());
  TS_ASSERT_EMPTY (errStream.str());
}


//! Checks MastConfiguration::Update() giving only long version of command line arguments
//!
void UT_MastConfiguration::test_Update_LongSwitches ()
{
  // ---------------- Setup
  //
  vector<string>    arguments{
                               "MyAppWithLongName",
                               "--min_cycle=5ms",
                               "--max_cycle=2s",
                               "--sit=myDesign.sit",
                               "--protocol_name=myProtocol",
                               "--protocol_parameters=myProtocolParameters",
                               "--config_algo=last_lazy",
                               "--plugin_dir=MyPlugins",
                               "--plugin=customPlugins.dll",
                               "--log",
                               "--log_file=myProject.log",
                               "--log_level=debug",
                               "--log_kind=file",
                               "--log_kind=cout",
                               "--check",
                               "--check_file=modelCheck.txt",
                             };
  ostringstream     stdStream;
  ostringstream     errStream;
  auto              streamOutput = make_shared<TCLAP::StreamOutput>(stdStream, errStream);
  MastConfiguration sut;

  sut.CommandLineParserOutput(streamOutput);
  sut.AutomaticExit(false);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Update(arguments));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.ShouldExit());
  TS_ASSERT_TRUE  (sut.LoggerEnabled());
  TS_ASSERT_TRUE  (sut.ModelChecking());
  TS_ASSERT_FALSE (sut.GmlPrinting());
  TS_ASSERT_FALSE (sut.PrettyPrinting());
  TS_ASSERT_FALSE (sut.ReportManagerActivity());

  TS_ASSERT_EQUALS (sut.MinTimeBetweenCycles(),              5ms);
  TS_ASSERT_EQUALS (sut.MaxTimeBetweenCycles(),              2000ms);

  TS_ASSERT_EQUALS (sut.SitFilePath(),                       "myDesign.sit");
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocolName(),       "myProtocol");
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocolParameters(), "myProtocolParameters");
  TS_ASSERT_EQUALS (sut.ConfigurationAlgorithm(),            "last_lazy");
  TS_ASSERT_EQUALS (sut.GmlFilePath(),                       "MastModel.gml");
  TS_ASSERT_EQUALS (sut.GmlGraphName(),                      "DUT");
  TS_ASSERT_EQUALS (sut.LoggerFilePath(),                    "myProject.log");
  TS_ASSERT_EQUALS (sut.ManagerActivityFileBasePath(),       "DUT");
  TS_ASSERT_EQUALS (sut.ModelCheckingFilePath(),             "modelCheck.txt");
  TS_ASSERT_EQUALS (sut.PrettyPrintFilePath(),               "MastModel.txt");

  TS_ASSERT_EQUALS (sut.GmlOptions(),                GmlPrinterOptions::Std);
  TS_ASSERT_EQUALS (sut.ManagerActivityOptions(),    ManagerMonitorOptions::Std);
  TS_ASSERT_EQUALS (sut.PrettyPrintOptions(),        PrettyPrinterOptions::Std);
  TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::File | mast::LoggerKind::Cout);
  TS_ASSERT_EQUALS (sut.LoggerLevel(),               mast::LoggerLevel::Debug);
  TS_ASSERT_EQUALS (sut.LoggerShownItems(),          mast::LoggerShownItems::Std_Less);
  TS_ASSERT_EQUALS (sut.GmlReportMoments(),          mast::ReportMoments::AfterModelParsing);
  TS_ASSERT_EQUALS (sut.PrettyPrintReportMoments(),  mast::ReportMoments::AfterModelParsing);

  TS_ASSERT_EMPTY (stdStream.str());
  TS_ASSERT_EMPTY (errStream.str());

  CxxTest::setAbortTestOnFail(true);
  const auto& plugins = sut.PluginDLLs();
  TS_ASSERT_EQUALS (plugins.size(),  1u);
  TS_ASSERT_EQUALS (plugins[0], "customPlugins.dll");

  const auto& pluginDirs = sut.PluginDirectories();
  TS_ASSERT_EQUALS (pluginDirs.size(),  1u);
  TS_ASSERT_EQUALS (pluginDirs[0], "MyPlugins");
}


//! Checks MastConfiguration::Update() using version taking argc and argv
//!
void UT_MastConfiguration::test_Update_UsingArgv ()
{
  // ---------------- Setup
  //
  const char* arguments[] = {
                                "MyAppWithLongName",           // 00
                                "--sit=myDesign.sit",          // 01
                                "--protocol_name=myProtocol",  // 02
                                "--config_algo=last_lazy",     // 03
                                "--plugin_dir=MyPlugins",      // 04
                                "--plugin=customPlugins.dll",  // 05
                                "--log",                       // 06
                                "--log_file=myProject.log",    // 07
                                "--log_level=debug",           // 08
                                "--log_kind=cout",             // 09
                                "--log_kind=file",             // 10
                                "--check",                     // 11
                                "--check_file=modelCheck.txt", // 12
                            };

  ostringstream     stdStream;
  ostringstream     errStream;
  auto              streamOutput = make_shared<TCLAP::StreamOutput>(stdStream, errStream);
  auto              argc = std::extent<decltype(arguments)>::value;
  MastConfiguration sut;

  sut.CommandLineParserOutput(streamOutput);
  sut.AutomaticExit(false);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Update(argc, arguments));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.ShouldExit());
  TS_ASSERT_TRUE  (sut.LoggerEnabled());
  TS_ASSERT_TRUE  (sut.ModelChecking());
  TS_ASSERT_FALSE (sut.GmlPrinting());
  TS_ASSERT_FALSE (sut.PrettyPrinting());
  TS_ASSERT_FALSE (sut.ReportManagerActivity());

  TS_ASSERT_EQUALS (sut.SitFilePath(),                 "myDesign.sit");
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocolName(), "myProtocol");
  TS_ASSERT_EQUALS (sut.ConfigurationAlgorithm(),      "last_lazy");
  TS_ASSERT_EQUALS (sut.GmlFilePath(),                 "MastModel.gml");
  TS_ASSERT_EQUALS (sut.GmlGraphName(),                "DUT");
  TS_ASSERT_EQUALS (sut.LoggerFilePath(),              "myProject.log");
  TS_ASSERT_EQUALS (sut.ManagerActivityFileBasePath(), "DUT");
  TS_ASSERT_EQUALS (sut.ModelCheckingFilePath(),       "modelCheck.txt");
  TS_ASSERT_EQUALS (sut.PrettyPrintFilePath(),         "MastModel.txt");

  TS_ASSERT_EQUALS (sut.GmlOptions(),                GmlPrinterOptions::Std);
  TS_ASSERT_EQUALS (sut.ManagerActivityOptions(),    ManagerMonitorOptions::Std);
  TS_ASSERT_EQUALS (sut.PrettyPrintOptions(),        PrettyPrinterOptions::Std);
  TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::File | mast::LoggerKind::Cout);
  TS_ASSERT_EQUALS (sut.LoggerLevel(),               mast::LoggerLevel::Debug);
  TS_ASSERT_EQUALS (sut.LoggerShownItems(),          mast::LoggerShownItems::Std_Less);
  TS_ASSERT_EQUALS (sut.GmlReportMoments(),          mast::ReportMoments::AfterModelParsing);
  TS_ASSERT_EQUALS (sut.PrettyPrintReportMoments(),  mast::ReportMoments::AfterModelParsing);

  TS_ASSERT_EMPTY (stdStream.str());
  TS_ASSERT_EMPTY (errStream.str());

  CxxTest::setAbortTestOnFail(true);
  const auto& plugins = sut.PluginDLLs();
  TS_ASSERT_EQUALS (plugins.size(),  1u);
  TS_ASSERT_EQUALS (plugins[0], "customPlugins.dll");

  const auto& pluginDirs = sut.PluginDirectories();
  TS_ASSERT_EQUALS (pluginDirs.size(),  1u);
  TS_ASSERT_EQUALS (pluginDirs[0], "MyPlugins");
}


//! Checks MastConfiguration::Update() giving only configuration file path
//!
void UT_MastConfiguration::test_Update_OnlyYamlConfig ()
{
  // ---------------- Setup
  //
  vector<string> arguments{ "MyAppWithLongName" };
  arguments.emplace_back("--conf="s + GetTestFilePath("UT_MastConfiguration_good.yml"));

  ostringstream     stdStream;
  ostringstream     errStream;
  auto              streamOutput = make_shared<TCLAP::StreamOutput>(stdStream, errStream);
  MastConfiguration sut;

  sut.CommandLineParserOutput(streamOutput);
  sut.AutomaticExit(false);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Update(arguments));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.ShouldExit());
  TS_ASSERT_TRUE  (sut.LoggerEnabled());
  TS_ASSERT_TRUE  (sut.ModelChecking());
  TS_ASSERT_TRUE  (sut.GmlPrinting());
  TS_ASSERT_TRUE  (sut.PrettyPrinting());
  TS_ASSERT_TRUE  (sut.ReportManagerActivity());

  TS_ASSERT_EQUALS (sut.SitFilePath(),                 "myProject.sit");
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocolName(),     "myProtocol");
  TS_ASSERT_EQUALS (sut.ConfigurationAlgorithm(),      "myConfiguration");
  TS_ASSERT_EQUALS (sut.GmlFilePath(),                 "./myproject");
  TS_ASSERT_EQUALS (sut.GmlGraphName(),                "Foo");
  TS_ASSERT_EQUALS (sut.LoggerFilePath(),              "myProject.log");
  TS_ASSERT_EQUALS (sut.ManagerActivityFileBasePath(), "managerActivity");
  TS_ASSERT_EQUALS (sut.ModelCheckingFilePath(),       "modelCheck.txt");
  TS_ASSERT_EQUALS (sut.PrettyPrintFilePath(),         "../Tests/Bar");

  TS_ASSERT_EQUALS (sut.GmlOptions(),                GmlPrinterOptions::Std);
  TS_ASSERT_EQUALS (sut.ManagerActivityOptions(),    ManagerMonitorOptions::Verbose           |
                                                     ManagerMonitorOptions::AppThreadCreation |
                                                     ManagerMonitorOptions::PDLCommands       |
                                                     ManagerMonitorOptions::DataCycles);
  TS_ASSERT_EQUALS (sut.PrettyPrintOptions(),        PrettyPrinterOptions::Verbose                |
                                                     PrettyPrinterOptions::DisplayValueAuto       |
                                                     PrettyPrinterOptions::ShowProtocol           |
                                                     PrettyPrinterOptions::ShowSelectionState     |
                                                     PrettyPrinterOptions::ShowSelectionValue     |
                                                     PrettyPrinterOptions::ShowSelectorProperties |
                                                     PrettyPrinterOptions::ShowNodeIsIgnored);
  TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::OnlyErrorsOnCerr);
  TS_ASSERT_EQUALS (sut.LoggerLevel(),               mast::LoggerLevel::Warning);
  TS_ASSERT_EQUALS (sut.LoggerShownItems(),          mast::LoggerShownItems::Microseconds |
                                                     mast::LoggerShownItems::Level        |
                                                     mast::LoggerShownItems::ThreadId     |
                                                     mast::LoggerShownItems::FunctionName);
  TS_ASSERT_EQUALS (sut.GmlReportMoments(),          mast::ReportMoments::AfterModelParsing | mast::ReportMoments::AfterConfiguration);
  TS_ASSERT_EQUALS (sut.PrettyPrintReportMoments(),  mast::ReportMoments::AfterModelParsing | mast::ReportMoments::BeforeConfiguration);

  CxxTest::setAbortTestOnFail(true);
  const auto& plugins = sut.PluginDLLs();
  TS_ASSERT_EQUALS (plugins.size(),  1u);
  TS_ASSERT_EQUALS (plugins[0], "myOwnPlugin.dll");

  const auto& pluginDirs = sut.PluginDirectories();
  TS_ASSERT_EQUALS (pluginDirs.size(),  1u);
  TS_ASSERT_EQUALS (pluginDirs[0], "Plugins");
}


//! Checks MastConfiguration::Update() giving configuration file path and some arguments
//!
void UT_MastConfiguration::test_Update_Arguments_and_YamlConfig ()
{
  // ---------------- Setup
  //
  vector<string>    arguments{
                                "MyAppWithLongName",
                                "--sit=myDesign.sit",
                                "--plugin_dir=MyPlugins",
                                "--plugin=customPlugins.so",
                             };

  arguments.emplace_back("--conf="s + GetTestFilePath("UT_MastConfiguration_good.yml"));

  ostringstream     stdStream;
  ostringstream     errStream;
  auto              streamOutput = make_shared<TCLAP::StreamOutput>(stdStream, errStream);
  MastConfiguration sut;

  sut.CommandLineParserOutput(streamOutput);
  sut.AutomaticExit(false);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Update(arguments));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.ShouldExit());
  TS_ASSERT_TRUE  (sut.LoggerEnabled());
  TS_ASSERT_TRUE  (sut.ModelChecking());
  TS_ASSERT_TRUE  (sut.GmlPrinting());
  TS_ASSERT_TRUE  (sut.PrettyPrinting());
  TS_ASSERT_TRUE  (sut.ReportManagerActivity());

  TS_ASSERT_EQUALS (sut.SitFilePath(),                 "myDesign.sit");
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocolName(), "myProtocol");
  TS_ASSERT_EQUALS (sut.ConfigurationAlgorithm(),      "myConfiguration");
  TS_ASSERT_EQUALS (sut.GmlFilePath(),                 "./myproject");
  TS_ASSERT_EQUALS (sut.GmlGraphName(),                "Foo");
  TS_ASSERT_EQUALS (sut.LoggerFilePath(),              "myProject.log");
  TS_ASSERT_EQUALS (sut.ManagerActivityFileBasePath(), "managerActivity");
  TS_ASSERT_EQUALS (sut.ModelCheckingFilePath(),       "modelCheck.txt");
  TS_ASSERT_EQUALS (sut.PrettyPrintFilePath(),         "../Tests/Bar");

  TS_ASSERT_EQUALS (sut.GmlOptions(),                GmlPrinterOptions::Std);
  TS_ASSERT_EQUALS (sut.ManagerActivityOptions(),    ManagerMonitorOptions::Verbose           |
                                                     ManagerMonitorOptions::AppThreadCreation |
                                                     ManagerMonitorOptions::PDLCommands       |
                                                     ManagerMonitorOptions::DataCycles);
  TS_ASSERT_EQUALS (sut.PrettyPrintOptions(),        PrettyPrinterOptions::Verbose                |
                                                     PrettyPrinterOptions::DisplayValueAuto       |
                                                     PrettyPrinterOptions::ShowProtocol           |
                                                     PrettyPrinterOptions::ShowSelectionState     |
                                                     PrettyPrinterOptions::ShowSelectionValue     |
                                                     PrettyPrinterOptions::ShowSelectorProperties |
                                                     PrettyPrinterOptions::ShowNodeIsIgnored);
  TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::OnlyErrorsOnCerr);
  TS_ASSERT_EQUALS (sut.LoggerLevel(),               mast::LoggerLevel::Warning);
  TS_ASSERT_EQUALS (sut.LoggerShownItems(),          mast::LoggerShownItems::Microseconds |
                                                     mast::LoggerShownItems::Level        |
                                                     mast::LoggerShownItems::ThreadId     |
                                                     mast::LoggerShownItems::FunctionName);
  TS_ASSERT_EQUALS (sut.GmlReportMoments(),          mast::ReportMoments::AfterModelParsing | mast::ReportMoments::AfterConfiguration);
  TS_ASSERT_EQUALS (sut.PrettyPrintReportMoments(),  mast::ReportMoments::AfterModelParsing | mast::ReportMoments::BeforeConfiguration);

  CxxTest::setAbortTestOnFail(true);
  const auto& plugins = sut.PluginDLLs();
  TS_ASSERT_EQUALS (plugins.size(),  1u);
  TS_ASSERT_EQUALS (plugins[0], "customPlugins.so");

  const auto& pluginDirs = sut.PluginDirectories();
  TS_ASSERT_EQUALS (pluginDirs.size(),  1u);
  TS_ASSERT_EQUALS (pluginDirs[0], "MyPlugins");
}



//===========================================================================
// End of UT_MastConfiguration.cpp
//===========================================================================
