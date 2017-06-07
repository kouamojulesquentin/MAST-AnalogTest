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


//===========================================================================
// End of UT_MastConfiguration.cpp
//===========================================================================
