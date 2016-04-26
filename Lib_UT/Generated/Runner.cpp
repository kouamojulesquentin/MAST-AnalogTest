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


// #include "g3log/g3log.hpp"
// #include "g3log/logworker.hpp"
// #include "LogFormatter.h"
// #include "LoggerSinks.h"
// #include <memory>

int main( int argc, char *argv[] )
{
    // ---------------- Initialize logger (for discarding any log)
    //
//    auto logworker  = g3::LogWorker::createLogWorker();
//    g3::UnitTestsLoggerSink::TSinkHandle sinkHandle = logworker->addSink(std::make_unique<g3::UnitTestsLoggerSink>(false), &g3::UnitTestsLoggerSink::ReceiveLogMessage);
//
//    g3::UnitTestsLoggerSink::SetSinkHandle(sinkHandle);    // This is to allow g3log unit tests to have an access point to log at the end of the chain
//    g3::initializeLogging(logworker.get());
//    g3::logEnabled(true);

    // ---------------- Start UT framework
    //
    CxxTest::RealWorldDescription::_worldName = "Mast_Lib";

    CxxTest::ErrorPrinter tmp;

    int status = CxxTest::Main<CxxTest::ErrorPrinter>(tmp, argc, argv);
    return status;
}

bool suite_UT_BinaryVector_init = false;
#include "C:\Users\Jean-Francois\Documents\Tima\NewMast\Lib_UT\Utility\UT_BinaryVector.hpp"

static UT_BinaryVector suite_UT_BinaryVector;

static CxxTest::List Tests_UT_BinaryVector = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_UT_BinaryVector( ".//Utility/UT_BinaryVector.hpp", 23, "UT_BinaryVector", suite_UT_BinaryVector, Tests_UT_BinaryVector );

static class TestDescription_suite_UT_BinaryVector_test_Constructor_Default : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Constructor_Default() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 34, "test_Constructor_Default" ) {}
 void runTest() { suite_UT_BinaryVector.test_Constructor_Default(); }
} testDescription_suite_UT_BinaryVector_test_Constructor_Default;

static class TestDescription_suite_UT_BinaryVector_test_Constructor_Copy_When_SrcIsEmpty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Constructor_Copy_When_SrcIsEmpty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 35, "test_Constructor_Copy_When_SrcIsEmpty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Constructor_Copy_When_SrcIsEmpty(); }
} testDescription_suite_UT_BinaryVector_test_Constructor_Copy_When_SrcIsEmpty;

static class TestDescription_suite_UT_BinaryVector_test_Constructor_Move_When_SrcIsEmpty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Constructor_Move_When_SrcIsEmpty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 36, "test_Constructor_Move_When_SrcIsEmpty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Constructor_Move_When_SrcIsEmpty(); }
} testDescription_suite_UT_BinaryVector_test_Constructor_Move_When_SrcIsEmpty;

static class TestDescription_suite_UT_BinaryVector_test_Append_8_bits_When_Empty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_8_bits_When_Empty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 40, "test_Append_8_bits_When_Empty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_8_bits_When_Empty(); }
} testDescription_suite_UT_BinaryVector_test_Append_8_bits_When_Empty;

static class TestDescription_suite_UT_BinaryVector_test_Append_16_bits_When_Empty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_16_bits_When_Empty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 41, "test_Append_16_bits_When_Empty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_16_bits_When_Empty(); }
} testDescription_suite_UT_BinaryVector_test_Append_16_bits_When_Empty;

static class TestDescription_suite_UT_BinaryVector_test_Append_32_bits_When_Empty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_32_bits_When_Empty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 42, "test_Append_32_bits_When_Empty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_32_bits_When_Empty(); }
} testDescription_suite_UT_BinaryVector_test_Append_32_bits_When_Empty;

static class TestDescription_suite_UT_BinaryVector_test_Append_64_bits_When_Empty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_64_bits_When_Empty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 43, "test_Append_64_bits_When_Empty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_64_bits_When_Empty(); }
} testDescription_suite_UT_BinaryVector_test_Append_64_bits_When_Empty;

static class TestDescription_suite_UT_BinaryVector_test_Append_8_bits_When_NotEmpty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_8_bits_When_NotEmpty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 45, "test_Append_8_bits_When_NotEmpty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_8_bits_When_NotEmpty(); }
} testDescription_suite_UT_BinaryVector_test_Append_8_bits_When_NotEmpty;

static class TestDescription_suite_UT_BinaryVector_test_Append_16_bits_When_NotEmpty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_16_bits_When_NotEmpty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 46, "test_Append_16_bits_When_NotEmpty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_16_bits_When_NotEmpty(); }
} testDescription_suite_UT_BinaryVector_test_Append_16_bits_When_NotEmpty;

static class TestDescription_suite_UT_BinaryVector_test_Append_32_bits_When_NotEmpty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_32_bits_When_NotEmpty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 47, "test_Append_32_bits_When_NotEmpty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_32_bits_When_NotEmpty(); }
} testDescription_suite_UT_BinaryVector_test_Append_32_bits_When_NotEmpty;

static class TestDescription_suite_UT_BinaryVector_test_Append_64_bits_When_NotEmpty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_64_bits_When_NotEmpty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 48, "test_Append_64_bits_When_NotEmpty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_64_bits_When_NotEmpty(); }
} testDescription_suite_UT_BinaryVector_test_Append_64_bits_When_NotEmpty;

static class TestDescription_suite_UT_BinaryVector_test_Append_1_to_7_bits_Empty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_1_to_7_bits_Empty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 50, "test_Append_1_to_7_bits_Empty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_1_to_7_bits_Empty(); }
} testDescription_suite_UT_BinaryVector_test_Append_1_to_7_bits_Empty;

static class TestDescription_suite_UT_BinaryVector_test_No_test_yet_for_Guard : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_No_test_yet_for_Guard() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 54, "test_No_test_yet_for_Guard" ) {}
 void runTest() { suite_UT_BinaryVector.test_No_test_yet_for_Guard(); }
} testDescription_suite_UT_BinaryVector_test_No_test_yet_for_Guard;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName      = "cxxtest";
bool CxxTest::RealWorldDescription::_displayFilePath = false;
