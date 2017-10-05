/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#ifndef _CXXTEST_HAVE_EH
#define _CXXTEST_HAVE_EH
#endif
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>


#include "g3log/g3log.hpp"
#include "g3log/logworker.hpp"
#include "LogFormatter.h"
#include "LoggerSinks.h"
#include "CustomFileSink.h"
#include <memory>
#include <string>

std::string Exe_Dir_Path; // Mainly for mast configuration related code

int main( int argc, char *argv[] )
{
    // ---------------- Set Exe_Dir_Path with only directory path of runner application
    //
    Exe_Dir_Path = std::string(argv[0]);
    auto sepPos  = Exe_Dir_Path.rfind("/"); // Search for last Linux directory separator
    if (sepPos == std::string::npos)
    {
      sepPos  = Exe_Dir_Path.rfind("\\");   // Search for last Windows directory separator
    }

    if (sepPos != std::string::npos)
    {
      Exe_Dir_Path.erase(sepPos);           // Remove exe name (keeping only directory path)
    }
    else
    {
      Exe_Dir_Path = ".";                   // When no separator ==> there is only application name ==> this is current directory
    }

    // ---------------- Initialize logger
    //
    auto logworker  = g3::LogWorker::createLogWorker();

    // ---------------- Sink for logging errors to std::cerr
    //
    auto cerrSink       = std::make_unique<g3::ErrorsOnCerrLoggerSink>(true, g3::LogFormatterUsage::Ignore);
    auto cerrSinkHandle = logworker->addSink(std::move(cerrSink), &g3::ErrorsOnCerrLoggerSink::ReceiveLogMessage);

    // ---------------- Sink for capturing all messages in memory
    //
    g3::UnitTestsLoggerSink::TSinkHandle sinkHandle = logworker->addSink(std::make_unique<g3::UnitTestsLoggerSink>(false), &g3::UnitTestsLoggerSink::ReceiveLogMessage);
    g3::UnitTestsLoggerSink::SetSinkHandle(sinkHandle);    // This is to allow g3log unit tests to have an access point to log at the end of the chain

    auto logFormatter = g3::LogFormatter();
    logFormatter.ShowDate(false);
    logFormatter.ShowTime(false);
    logFormatter.ShowFileName(false);
    logFormatter.ShowFunctionName(false);
    logFormatter.ShowLineNumber(false);


    auto customSink = std::make_unique<g3::CustomFileSink>("UT_Readers_Log.txt", g3::CustomFileSink::FlushMode::AutoBackground, logFormatter);
    customSink->Clear();

    auto customSinkHandle = logworker->addSink(std::move(customSink), &g3::CustomFileSink::ReceiveLogUnformattedMessage);


    g3::initializeLogging(logworker.get());
    LOG(INFO) << "Start of Unit tests";
    g3::logEnabled(false);

    // ---------------- Start UT framework
    //
    CxxTest::RealWorldDescription::_worldName = "Mast_Readers";

    CxxTest::ErrorPrinter tmp;

    int status = CxxTest::Main<CxxTest::ErrorPrinter>(tmp, argc, argv);

    g3::logEnabled(true);
    LOG(INFO) << "End of Unit tests";
    g3::logEnabled(false);
    return status;
}

bool suite_UT_Fragments_init = false;
#include "D:\Projects\Tima\NewMast\Readers_UT\ICL\UT_Fragments.hpp"

static UT_Fragments suite_UT_Fragments;

static CxxTest::List Tests_UT_Fragments = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_UT_Fragments( "D:/Projects/Tima/NewMast/Readers_UT/ICL/UT_Fragments.hpp", 23, "UT_Fragments", suite_UT_Fragments, Tests_UT_Fragments );

static class TestDescription_suite_UT_Fragments_test_Parse_Parser_Fragments : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_Fragments_test_Parse_Parser_Fragments() : CxxTest::RealTestDescription( Tests_UT_Fragments, suiteDescription_UT_Fragments, 31, "test_Parse_Parser_Fragments" ) {}
 void runTest() { suite_UT_Fragments.test_Parse_Parser_Fragments(); }
} testDescription_suite_UT_Fragments_test_Parse_Parser_Fragments;

static class TestDescription_suite_UT_Fragments_test_Parse_Parser_Fragments_with_SyntaxErrors : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_Fragments_test_Parse_Parser_Fragments_with_SyntaxErrors() : CxxTest::RealTestDescription( Tests_UT_Fragments, suiteDescription_UT_Fragments, 32, "test_Parse_Parser_Fragments_with_SyntaxErrors" ) {}
 void runTest() { suite_UT_Fragments.test_Parse_Parser_Fragments_with_SyntaxErrors(); }
} testDescription_suite_UT_Fragments_test_Parse_Parser_Fragments_with_SyntaxErrors;

#include "D:\Projects\Tima\NewMast\Readers_UT\ICL\UT_IEEE_P1687.hpp"

static UT_IEEE_P1687 suite_UT_IEEE_P1687;

static CxxTest::List Tests_UT_IEEE_P1687 = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_UT_IEEE_P1687( "D:/Projects/Tima/NewMast/Readers_UT/ICL/UT_IEEE_P1687.hpp", 24, "UT_IEEE_P1687", suite_UT_IEEE_P1687, Tests_UT_IEEE_P1687 );

static class TestDescription_suite_UT_IEEE_P1687_test_Parse_Annex_E_Examples : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_IEEE_P1687_test_Parse_Annex_E_Examples() : CxxTest::RealTestDescription( Tests_UT_IEEE_P1687, suiteDescription_UT_IEEE_P1687, 32, "test_Parse_Annex_E_Examples" ) {}
 void runTest() { suite_UT_IEEE_P1687.test_Parse_Annex_E_Examples(); }
} testDescription_suite_UT_IEEE_P1687_test_Parse_Annex_E_Examples;

static class TestDescription_suite_UT_IEEE_P1687_test_Parse_Annex_F_Design_Guidance : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_IEEE_P1687_test_Parse_Annex_F_Design_Guidance() : CxxTest::RealTestDescription( Tests_UT_IEEE_P1687, suiteDescription_UT_IEEE_P1687, 33, "test_Parse_Annex_F_Design_Guidance" ) {}
 void runTest() { suite_UT_IEEE_P1687.test_Parse_Annex_F_Design_Guidance(); }
} testDescription_suite_UT_IEEE_P1687_test_Parse_Annex_F_Design_Guidance;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName      = "cxxtest";
bool CxxTest::RealWorldDescription::_displayFilePath = false;
