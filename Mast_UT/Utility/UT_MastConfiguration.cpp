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

#include <tclap/ArgException.h>
#include <tclap/StreamOutput.h>

#include <cxxtest/ValueTraits.h>
#include <vector>
#include <string>
#include <sstream>
#include <memory>

using std::vector;
using std::string;
using std::ostringstream;
using std::shared_ptr;
using std::make_shared;

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
    TS_ASSERT_EQUALS (sut.ConfigurationAlgorithm(),      "LastOrDefault");
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


//! Checks MastConfiguration::ParseYamlConfiguration() when yaml there is syntax error(s)
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
              "Mast_Options:\n"                              // 01
              "  SIT_file_path: DUT.sit\n"                   // 02
              "  Configuration_algorithm: last_or_default\n" // 03
              "  Access_interface_protocol: SIT\n"           // 04
              "  Plugin_DLLs:  Plugins\n"                    // 05
              "  Model_checking: \n"                         // 06
              "    Enable: false\n"                          // 07
              "    File_path:\n"                             // 08
              "  Debug:\n"                                   // 09
              "    Logging:\n"                               // 10
              "      Enable: false\n"                        // 11
              "      Logger_Kind: std\n"                     // 12
              "      File_path:\n"                           // 13
              "      Shown_items: std\n"                     // 14
              "      Level: info\n"                          // 15
              "    Model_GML_printing: \n"                   // 16
              "      Enable: false\n"                        // 17
              "      Moments:\n"                             // 18
              "        - After_Parsing\n"                    // 19
              "      File_path:\n"                           // 20
              "      Graph_name:\n"                          // 21
              "      Options: std\n"                         // 22
              "    Model_textual_print: \n"                  // 23
              "      Enable: false\n"                        // 24
              "      Moments:\n"                             // 25
              "        - After_Parsing\n"                    // 26
              "      File_path:\n"                           // 27
              "      Options:         default\n"             // 28
              "    Manager_activity:  \n"                    // 29
              "      Enable: false\n"                        // 30
              "      File_base_name:\n"                      // 31
              "      Options:         PDL_commands\n"        // 32
              "Plugins_Options: ""\n"                        // 33
             );

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.ParseYamlConfiguration(yaml));

  // ---------------- Verify
  //
  TS_WARN ("Test not yet implemented");
}


//===========================================================================
// End of UT_MastConfiguration.cpp
//===========================================================================
