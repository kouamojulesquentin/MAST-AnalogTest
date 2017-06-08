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
#include "TestUtilities.hpp"

#include <tclap/ArgException.h>
#include <tclap/StreamOutput.h>

#include <cxxtest/ValueTraits.h>
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

using namespace mast;


#define TS_ASSERT_STR_CONTAINS(text, sub_text)                                                              \
    if ((text).find((sub_text)) == std::string::npos)                                                       \
    {                                                                                                       \
      TS_FAIL(string(#text).append(": ").append((text)).append(", does not contain: ").append((sub_text)));\
    }

    #define TS_ASSERT_STR_EMPTY(text)                                           \
        if (!(text).empty())                                                    \
        {                                                                       \
          TS_FAIL(string(#text).append(" is not empty. Got: ").append((text))); \
        }


namespace
{
  void Check_DefaultConfiguration(const MastConfiguration& sut)
  {
    TS_ASSERT_FALSE (sut.LoggerEnabled());
    TS_ASSERT_FALSE (sut.ModelChecking());
    TS_ASSERT_FALSE (sut.GmlPrinting());
    TS_ASSERT_FALSE (sut.PrettyPrinting());
    TS_ASSERT_FALSE (sut.ReportManagerActivity());

    TS_ASSERT_EQUALS (sut.SitFilePath(),                 "DUT.sit");
    TS_ASSERT_EQUALS (sut.AccessInterfaceProtocol(),     "");
    TS_ASSERT_EQUALS (sut.ConfigurationAlgorithm(),      "last_or_default");
    TS_ASSERT_EQUALS (sut.GmlFilePath(),                 "MastModel.gml");
    TS_ASSERT_EQUALS (sut.GmlGraphName(),                "DUT");
    TS_ASSERT_EQUALS (sut.LoggerFilePath(),              "Mast.log");
    TS_ASSERT_EQUALS (sut.ManagerActivityFileBasePath(), "DUT");
    TS_ASSERT_EQUALS (sut.ModelCheckingFilePath(),       "");
    TS_ASSERT_EQUALS (sut.PrettyPrintFilePath(),         "MastModel.txt");

    TS_ASSERT_EQUALS (sut.GmlOptions(),                GmlPrinterOptions::Std);
    TS_ASSERT_EQUALS (sut.ManagerActivityOptions(),    ManagerMonitorOptions::Std);
    TS_ASSERT_EQUALS (sut.PrettyPrintOptions(),        PrettyPrinterOptions::Std);
    TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::Std);
    TS_ASSERT_EQUALS (sut.LoggerLevel(),               mast::LoggerLevel::Info);
    TS_ASSERT_EQUALS (sut.LoggerShownItems(),          mast::LoggerShownItems::Std);
    TS_ASSERT_EQUALS (sut.GmlReportMoments(),          mast::ReportMoments::AfterModelParsing);
    TS_ASSERT_EQUALS (sut.PrettyPrintReportMoments(),  mast::ReportMoments::AfterModelParsing);

    CxxTest::setAbortTestOnFail(true);
    const auto& plugins = sut.PluginDLLs();
    TS_ASSERT_EQUALS (plugins.size(),  1u);
    TS_ASSERT_EQUALS (plugins.front(), PLUGINS_DIRECTORY_NAME);
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
              "  Plugin_DLLs:  \n"
              "    - Plugins\n"
              "    - myPlugin.so\n"
              "  Debug:\n"
              "    Logging:\n"
              "      Enable: true\n"
              "      Logger_Kind: copy_errors_on_cerr\n"
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
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocol(),     "");
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
  TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::CopyErrorsOnCerr);
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
  TS_ASSERT_EQUALS (plugins.size(),  2u);
  TS_ASSERT_EQUALS (plugins[0], "Plugins");
  TS_ASSERT_EQUALS (plugins[1], "myPlugin.so");
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
              "  Access_interface_protocol: myProtocol\n"                                  // 04
              "  Plugin_DLLs:  [Plugins, myPlugin.dll]\n"                                  // 05
              "  Model_checking: \n"                                                       // 06
              "    Enable: true\n"                                                         // 07
              "    File_path: modelCheck.txt\n"                                            // 08
              "  Debug:\n"                                                                 // 09
              "    Logging:\n"                                                             // 10
              "      Enable: true\n"                                                       // 11
              "      Logger_Kind: copy_errors_on_cerr\n"                                   // 12
              "      File_path:   myProject.log\n"                                         // 13
              "      Shown_items: [microseconds, level, thread_id, function_name]\n"       // 14
              "      Level: warning\n"                                                     // 15
              "    Model_GML_printing: \n"                                                 // 16
              "      Enable: true\n"                                                       // 17
              "      Moments:\n"                                                           // 18
              "        - after_model_parsing\n"                                            // 19
              "        - after_configuration\n"                                            // 20
              "      File_path: ./myproject\n"                                             // 21
              "      Graph_name: Foo\n"                                                    // 22
              "      Options: std\n"                                                       // 23
              "    Model_textual_print: \n"                                                // 24
              "      Enable: true\n"                                                       // 25
              "      Moments:\n"                                                           // 26
              "        - after_model_parsing\n"                                            // 27
              "        - before_configuration\n"                                           // 28
              "      File_path: ../Tests/Bar\n"                                            // 29
              "      Options:         \n"                                                  // 30
              "         - verbose \n"                                                      // 31
              "         - auto_value \n"                                                   // 32
              "         - protocol_name \n"                                                // 33
              "         - selection_state \n"                                              // 34
              "         - selection_value \n"                                              // 35
              "         - selector_properties \n"                                          // 36
              "         - ignored_nodes  \n"                                               // 37
              "    Manager_activity:  \n"                                                  // 38
              "      Enable: true\n"                                                       // 39
              "      File_base_name: managerActivity\n"                                    // 40
              "      Options: [verbose, app_thread_creation, PDL_commands, data_cycles]\n" // 41
              "Plugins_Options: ""\n"                                                      // 42
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
  TS_ASSERT_TRUE  (sut.ReportManagerActivity());

  TS_ASSERT_EQUALS (sut.SitFilePath(),                 "myProject.sit");
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocol(),     "myProtocol");
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
  TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::CopyErrorsOnCerr);
  TS_ASSERT_EQUALS (sut.LoggerLevel(),               mast::LoggerLevel::Warning);
  TS_ASSERT_EQUALS (sut.LoggerShownItems(),          mast::LoggerShownItems::Microseconds |
                                                     mast::LoggerShownItems::Level        |
                                                     mast::LoggerShownItems::ThreadId     |
                                                     mast::LoggerShownItems::FunctionName);
  TS_ASSERT_EQUALS (sut.GmlReportMoments(),          mast::ReportMoments::AfterModelParsing | mast::ReportMoments::AfterConfiguration);
  TS_ASSERT_EQUALS (sut.PrettyPrintReportMoments(),  mast::ReportMoments::AfterModelParsing | mast::ReportMoments::BeforeConfiguration);

  CxxTest::setAbortTestOnFail(true);
  const auto& plugins = sut.PluginDLLs();
  TS_ASSERT_EQUALS (plugins.size(),  2u);
  TS_ASSERT_EQUALS (plugins[0], "Plugins");
  TS_ASSERT_EQUALS (plugins[1], "myPlugin.dll");
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
  auto              filePath = test::GetTestFilePath("UT_MastConfiguration_empty.yml");

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.ParseConfigurationFile(filePath));

  // ---------------- Verify
  //
  Check_DefaultConfiguration(sut);
}


//! Checks MastConfiguration::ParseConfigurationFile() from "default" yaml file
//!
void UT_MastConfiguration::test_ParseConfigurationFile_Default ()
{
  // ---------------- Setup
  //
  MastConfiguration sut;
  auto              filePath = test::GetTestFilePath("UT_MastConfiguration_default.yml");

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

  auto filePath = test::GetTestFilePath("UT_MastConfiguration_good.yml");

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

  TS_ASSERT_EQUALS (sut.SitFilePath(),                 "myProject.sit");
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocol(),     "myProtocol");
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
  TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::CopyErrorsOnCerr);
  TS_ASSERT_EQUALS (sut.LoggerLevel(),               mast::LoggerLevel::Warning);
  TS_ASSERT_EQUALS (sut.LoggerShownItems(),          mast::LoggerShownItems::Microseconds |
                                                     mast::LoggerShownItems::Level        |
                                                     mast::LoggerShownItems::ThreadId     |
                                                     mast::LoggerShownItems::FunctionName);
  TS_ASSERT_EQUALS (sut.GmlReportMoments(),          mast::ReportMoments::AfterModelParsing | mast::ReportMoments::AfterConfiguration);
  TS_ASSERT_EQUALS (sut.PrettyPrintReportMoments(),  mast::ReportMoments::AfterModelParsing | mast::ReportMoments::BeforeConfiguration);

  CxxTest::setAbortTestOnFail(true);
  const auto& plugins = sut.PluginDLLs();
  TS_ASSERT_EQUALS (plugins.size(),  2u);
  TS_ASSERT_EQUALS (plugins[0], "Plugins");
  TS_ASSERT_EQUALS (plugins[1], "myPlugin.dll");
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

  TS_ASSERT_STR_CONTAINS (stdMessage, "USAGE:")
  TS_ASSERT_STR_CONTAINS (errMessage, "PARSE ERROR: Argument:")
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

  TS_ASSERT_STR_CONTAINS (stdMessage, "USAGE:")
  TS_ASSERT_STR_CONTAINS (errMessage, "PARSE ERROR: Argument:")
}



//! Checks MastConfiguration::Update() giving only short version of command line arguments
//!
void UT_MastConfiguration::test_Update_ShortSwitches ()
{
  // ---------------- Setup
  //
  vector<string>    arguments{ "MyAppWithLongName", "-s=myDesign.sit", "-a=last_lazy", "-l", "-c=myConf.yml" };
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
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocol(),     "");
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
  TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::Std);
  TS_ASSERT_EQUALS (sut.LoggerLevel(),               mast::LoggerLevel::Info);
  TS_ASSERT_EQUALS (sut.LoggerShownItems(),          mast::LoggerShownItems::Std);
  TS_ASSERT_EQUALS (sut.GmlReportMoments(),          mast::ReportMoments::AfterModelParsing);
  TS_ASSERT_EQUALS (sut.PrettyPrintReportMoments(),  mast::ReportMoments::AfterModelParsing);

  CxxTest::setAbortTestOnFail(true);
  const auto& plugins = sut.PluginDLLs();
  TS_ASSERT_EQUALS (plugins.size(),  1u);
  TS_ASSERT_EQUALS (plugins.front(), PLUGINS_DIRECTORY_NAME);

  TS_ASSERT_STR_EMPTY (stdStream.str());
  TS_ASSERT_STR_EMPTY (errStream.str());
}


//! Checks MastConfiguration::Update() giving only long version of command line arguments
//!
void UT_MastConfiguration::test_Update_LongSwitches ()
{
  // ---------------- Setup
  //
  vector<string>    arguments{
                                "MyAppWithLongName",
                                "--sit=myDesign.sit",
                                "--protocol=myProtocol",
                                "--config_algo=last_lazy",
                                "--plugins=MyPlugins",
                                "--plugins=customPlugins.dll",
                                "--log",
                                "--log_file=myProject.log",
                                "--log_level=debug",
                                "--log_kind=copy_all_on_cout",
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

  TS_ASSERT_EQUALS (sut.SitFilePath(),                 "myDesign.sit");
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocol(),     "myProtocol");
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
  TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::CopyAllOnCout);
  TS_ASSERT_EQUALS (sut.LoggerLevel(),               mast::LoggerLevel::Debug);
  TS_ASSERT_EQUALS (sut.LoggerShownItems(),          mast::LoggerShownItems::Std);
  TS_ASSERT_EQUALS (sut.GmlReportMoments(),          mast::ReportMoments::AfterModelParsing);
  TS_ASSERT_EQUALS (sut.PrettyPrintReportMoments(),  mast::ReportMoments::AfterModelParsing);

  TS_ASSERT_STR_EMPTY (stdStream.str());
  TS_ASSERT_STR_EMPTY (errStream.str());

  CxxTest::setAbortTestOnFail(true);
  const auto& plugins = sut.PluginDLLs();
  TS_ASSERT_EQUALS (plugins.size(),  2u);
  TS_ASSERT_EQUALS (plugins[0], "MyPlugins");
  TS_ASSERT_EQUALS (plugins[1], "customPlugins.dll");
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
                                "--protocol=myProtocol",       // 02
                                "--config_algo=last_lazy",     // 03
                                "--plugins=MyPlugins",         // 04
                                "--plugins=customPlugins.dll", // 05
                                "--log",                       // 06
                                "--log_file=myProject.log",    // 07
                                "--log_level=debug",           // 08
                                "--log_kind=copy_all_on_cout", // 09
                                "--check",                     // 10
                                "--check_file=modelCheck.txt", // 11
                            };

  ostringstream     stdStream;
  ostringstream     errStream;
  auto              streamOutput = make_shared<TCLAP::StreamOutput>(stdStream, errStream);
  MastConfiguration sut;

  sut.CommandLineParserOutput(streamOutput);
  sut.AutomaticExit(false);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Update(12, arguments));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.ShouldExit());
  TS_ASSERT_TRUE  (sut.LoggerEnabled());
  TS_ASSERT_TRUE  (sut.ModelChecking());
  TS_ASSERT_FALSE (sut.GmlPrinting());
  TS_ASSERT_FALSE (sut.PrettyPrinting());
  TS_ASSERT_FALSE (sut.ReportManagerActivity());

  TS_ASSERT_EQUALS (sut.SitFilePath(),                 "myDesign.sit");
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocol(),     "myProtocol");
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
  TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::CopyAllOnCout);
  TS_ASSERT_EQUALS (sut.LoggerLevel(),               mast::LoggerLevel::Debug);
  TS_ASSERT_EQUALS (sut.LoggerShownItems(),          mast::LoggerShownItems::Std);
  TS_ASSERT_EQUALS (sut.GmlReportMoments(),          mast::ReportMoments::AfterModelParsing);
  TS_ASSERT_EQUALS (sut.PrettyPrintReportMoments(),  mast::ReportMoments::AfterModelParsing);

  TS_ASSERT_STR_EMPTY (stdStream.str());
  TS_ASSERT_STR_EMPTY (errStream.str());

  CxxTest::setAbortTestOnFail(true);
  const auto& plugins = sut.PluginDLLs();
  TS_ASSERT_EQUALS (plugins.size(),  2u);
  TS_ASSERT_EQUALS (plugins[0], "MyPlugins");
  TS_ASSERT_EQUALS (plugins[1], "customPlugins.dll");
}


//! Checks MastConfiguration::Update() giving only configuration file path
//!
void UT_MastConfiguration::test_Update_OnlyYamlConfig ()
{
  // ---------------- Setup
  //
  vector<string> arguments{ "MyAppWithLongName" };
  arguments.emplace_back("--conf="s + test::GetTestFilePath("UT_MastConfiguration_good.yml"));

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
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocol(),     "myProtocol");
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
  TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::CopyErrorsOnCerr);
  TS_ASSERT_EQUALS (sut.LoggerLevel(),               mast::LoggerLevel::Warning);
  TS_ASSERT_EQUALS (sut.LoggerShownItems(),          mast::LoggerShownItems::Microseconds |
                                                     mast::LoggerShownItems::Level        |
                                                     mast::LoggerShownItems::ThreadId     |
                                                     mast::LoggerShownItems::FunctionName);
  TS_ASSERT_EQUALS (sut.GmlReportMoments(),          mast::ReportMoments::AfterModelParsing | mast::ReportMoments::AfterConfiguration);
  TS_ASSERT_EQUALS (sut.PrettyPrintReportMoments(),  mast::ReportMoments::AfterModelParsing | mast::ReportMoments::BeforeConfiguration);

  CxxTest::setAbortTestOnFail(true);
  const auto& plugins = sut.PluginDLLs();
  TS_ASSERT_EQUALS (plugins.size(),  2u);
  TS_ASSERT_EQUALS (plugins[0], "Plugins");
  TS_ASSERT_EQUALS (plugins[1], "myPlugin.dll");
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
                                "--plugins=MyPlugins",
                                "--plugins=customPlugins.so",
                             };

  arguments.emplace_back("--conf="s + test::GetTestFilePath("UT_MastConfiguration_good.yml"));

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
  TS_ASSERT_EQUALS (sut.AccessInterfaceProtocol(),     "myProtocol");
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
  TS_ASSERT_EQUALS (sut.LoggerKind(),                mast::LoggerKind::CopyErrorsOnCerr);
  TS_ASSERT_EQUALS (sut.LoggerLevel(),               mast::LoggerLevel::Warning);
  TS_ASSERT_EQUALS (sut.LoggerShownItems(),          mast::LoggerShownItems::Microseconds |
                                                     mast::LoggerShownItems::Level        |
                                                     mast::LoggerShownItems::ThreadId     |
                                                     mast::LoggerShownItems::FunctionName);
  TS_ASSERT_EQUALS (sut.GmlReportMoments(),          mast::ReportMoments::AfterModelParsing | mast::ReportMoments::AfterConfiguration);
  TS_ASSERT_EQUALS (sut.PrettyPrintReportMoments(),  mast::ReportMoments::AfterModelParsing | mast::ReportMoments::BeforeConfiguration);

  CxxTest::setAbortTestOnFail(true);
  const auto& plugins = sut.PluginDLLs();
  TS_ASSERT_EQUALS (plugins.size(),  2u);
  TS_ASSERT_EQUALS (plugins[0], "MyPlugins");
  TS_ASSERT_EQUALS (plugins[1], "customPlugins.so");
}



//===========================================================================
// End of UT_MastConfiguration.cpp
//===========================================================================
