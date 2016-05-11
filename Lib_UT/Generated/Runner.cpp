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
#include "D:\Projects\Tima\NewMast\Lib_UT\Utility\UT_BinaryVector.hpp"

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

static class TestDescription_suite_UT_BinaryVector_test_CreateFromBinaryString : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_CreateFromBinaryString() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 40, "test_CreateFromBinaryString" ) {}
 void runTest() { suite_UT_BinaryVector.test_CreateFromBinaryString(); }
} testDescription_suite_UT_BinaryVector_test_CreateFromBinaryString;

static class TestDescription_suite_UT_BinaryVector_test_CreateFromHexString : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_CreateFromHexString() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 41, "test_CreateFromHexString" ) {}
 void runTest() { suite_UT_BinaryVector.test_CreateFromHexString(); }
} testDescription_suite_UT_BinaryVector_test_CreateFromHexString;

static class TestDescription_suite_UT_BinaryVector_test_DataAsBinaryString_Without_NewLine : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_DataAsBinaryString_Without_NewLine() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 45, "test_DataAsBinaryString_Without_NewLine" ) {}
 void runTest() { suite_UT_BinaryVector.test_DataAsBinaryString_Without_NewLine(); }
} testDescription_suite_UT_BinaryVector_test_DataAsBinaryString_Without_NewLine;

static class TestDescription_suite_UT_BinaryVector_test_DataAsBinaryString_With_NewLine : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_DataAsBinaryString_With_NewLine() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 46, "test_DataAsBinaryString_With_NewLine" ) {}
 void runTest() { suite_UT_BinaryVector.test_DataAsBinaryString_With_NewLine(); }
} testDescription_suite_UT_BinaryVector_test_DataAsBinaryString_With_NewLine;

static class TestDescription_suite_UT_BinaryVector_test_DataAsBinaryString_Without_Separators : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_DataAsBinaryString_Without_Separators() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 47, "test_DataAsBinaryString_Without_Separators" ) {}
 void runTest() { suite_UT_BinaryVector.test_DataAsBinaryString_Without_Separators(); }
} testDescription_suite_UT_BinaryVector_test_DataAsBinaryString_Without_Separators;

static class TestDescription_suite_UT_BinaryVector_test_DataAsHexString_Without_NewLine : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_DataAsHexString_Without_NewLine() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 48, "test_DataAsHexString_Without_NewLine" ) {}
 void runTest() { suite_UT_BinaryVector.test_DataAsHexString_Without_NewLine(); }
} testDescription_suite_UT_BinaryVector_test_DataAsHexString_Without_NewLine;

static class TestDescription_suite_UT_BinaryVector_test_DataAsHexString_With_NewLine : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_DataAsHexString_With_NewLine() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 49, "test_DataAsHexString_With_NewLine" ) {}
 void runTest() { suite_UT_BinaryVector.test_DataAsHexString_With_NewLine(); }
} testDescription_suite_UT_BinaryVector_test_DataAsHexString_With_NewLine;

static class TestDescription_suite_UT_BinaryVector_test_DataAsHexString_Without_Separators : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_DataAsHexString_Without_Separators() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 50, "test_DataAsHexString_Without_Separators" ) {}
 void runTest() { suite_UT_BinaryVector.test_DataAsHexString_Without_Separators(); }
} testDescription_suite_UT_BinaryVector_test_DataAsHexString_Without_Separators;

static class TestDescription_suite_UT_BinaryVector_test_operator_eq_With_Self : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_operator_eq_With_Self() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 54, "test_operator_eq_With_Self" ) {}
 void runTest() { suite_UT_BinaryVector.test_operator_eq_With_Self(); }
} testDescription_suite_UT_BinaryVector_test_operator_eq_With_Self;

static class TestDescription_suite_UT_BinaryVector_test_operator_eq_When_Equal : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_operator_eq_When_Equal() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 55, "test_operator_eq_When_Equal" ) {}
 void runTest() { suite_UT_BinaryVector.test_operator_eq_When_Equal(); }
} testDescription_suite_UT_BinaryVector_test_operator_eq_When_Equal;

static class TestDescription_suite_UT_BinaryVector_test_operator_eq_When_NotEqual : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_operator_eq_When_NotEqual() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 56, "test_operator_eq_When_NotEqual" ) {}
 void runTest() { suite_UT_BinaryVector.test_operator_eq_When_NotEqual(); }
} testDescription_suite_UT_BinaryVector_test_operator_eq_When_NotEqual;

static class TestDescription_suite_UT_BinaryVector_test_operator_neq_With_Self : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_operator_neq_With_Self() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 60, "test_operator_neq_With_Self" ) {}
 void runTest() { suite_UT_BinaryVector.test_operator_neq_With_Self(); }
} testDescription_suite_UT_BinaryVector_test_operator_neq_With_Self;

static class TestDescription_suite_UT_BinaryVector_test_operator_neq_When_Equal : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_operator_neq_When_Equal() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 61, "test_operator_neq_When_Equal" ) {}
 void runTest() { suite_UT_BinaryVector.test_operator_neq_When_Equal(); }
} testDescription_suite_UT_BinaryVector_test_operator_neq_When_Equal;

static class TestDescription_suite_UT_BinaryVector_test_operator_neq_When_NotEqual : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_operator_neq_When_NotEqual() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 62, "test_operator_neq_When_NotEqual" ) {}
 void runTest() { suite_UT_BinaryVector.test_operator_neq_When_NotEqual(); }
} testDescription_suite_UT_BinaryVector_test_operator_neq_When_NotEqual;

static class TestDescription_suite_UT_BinaryVector_test_Constructor_With_Count_and_Pattern : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Constructor_With_Count_and_Pattern() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 66, "test_Constructor_With_Count_and_Pattern" ) {}
 void runTest() { suite_UT_BinaryVector.test_Constructor_With_Count_and_Pattern(); }
} testDescription_suite_UT_BinaryVector_test_Constructor_With_Count_and_Pattern;

static class TestDescription_suite_UT_BinaryVector_test_Constructor_Copy : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Constructor_Copy() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 67, "test_Constructor_Copy" ) {}
 void runTest() { suite_UT_BinaryVector.test_Constructor_Copy(); }
} testDescription_suite_UT_BinaryVector_test_Constructor_Copy;

static class TestDescription_suite_UT_BinaryVector_test_Constructor_Move : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Constructor_Move() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 68, "test_Constructor_Move" ) {}
 void runTest() { suite_UT_BinaryVector.test_Constructor_Move(); }
} testDescription_suite_UT_BinaryVector_test_Constructor_Move;

static class TestDescription_suite_UT_BinaryVector_test_Append_8_bits_When_Empty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_8_bits_When_Empty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 72, "test_Append_8_bits_When_Empty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_8_bits_When_Empty(); }
} testDescription_suite_UT_BinaryVector_test_Append_8_bits_When_Empty;

static class TestDescription_suite_UT_BinaryVector_test_Append_16_bits_When_Empty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_16_bits_When_Empty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 73, "test_Append_16_bits_When_Empty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_16_bits_When_Empty(); }
} testDescription_suite_UT_BinaryVector_test_Append_16_bits_When_Empty;

static class TestDescription_suite_UT_BinaryVector_test_Append_32_bits_When_Empty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_32_bits_When_Empty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 74, "test_Append_32_bits_When_Empty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_32_bits_When_Empty(); }
} testDescription_suite_UT_BinaryVector_test_Append_32_bits_When_Empty;

static class TestDescription_suite_UT_BinaryVector_test_Append_64_bits_When_Empty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_64_bits_When_Empty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 75, "test_Append_64_bits_When_Empty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_64_bits_When_Empty(); }
} testDescription_suite_UT_BinaryVector_test_Append_64_bits_When_Empty;

static class TestDescription_suite_UT_BinaryVector_test_Append_8_bits_When_NotEmpty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_8_bits_When_NotEmpty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 77, "test_Append_8_bits_When_NotEmpty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_8_bits_When_NotEmpty(); }
} testDescription_suite_UT_BinaryVector_test_Append_8_bits_When_NotEmpty;

static class TestDescription_suite_UT_BinaryVector_test_Append_16_bits_When_NotEmpty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_16_bits_When_NotEmpty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 78, "test_Append_16_bits_When_NotEmpty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_16_bits_When_NotEmpty(); }
} testDescription_suite_UT_BinaryVector_test_Append_16_bits_When_NotEmpty;

static class TestDescription_suite_UT_BinaryVector_test_Append_32_bits_When_NotEmpty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_32_bits_When_NotEmpty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 79, "test_Append_32_bits_When_NotEmpty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_32_bits_When_NotEmpty(); }
} testDescription_suite_UT_BinaryVector_test_Append_32_bits_When_NotEmpty;

static class TestDescription_suite_UT_BinaryVector_test_Append_64_bits_When_NotEmpty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_64_bits_When_NotEmpty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 80, "test_Append_64_bits_When_NotEmpty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_64_bits_When_NotEmpty(); }
} testDescription_suite_UT_BinaryVector_test_Append_64_bits_When_NotEmpty;

static class TestDescription_suite_UT_BinaryVector_test_Append_1_to_8_bits_When_Empty_Right_Aligned : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_1_to_8_bits_When_Empty_Right_Aligned() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 82, "test_Append_1_to_8_bits_When_Empty_Right_Aligned" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_1_to_8_bits_When_Empty_Right_Aligned(); }
} testDescription_suite_UT_BinaryVector_test_Append_1_to_8_bits_When_Empty_Right_Aligned;

static class TestDescription_suite_UT_BinaryVector_test_Append_1_to_8_bits_When_Empty_Left_Aligned : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_1_to_8_bits_When_Empty_Left_Aligned() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 83, "test_Append_1_to_8_bits_When_Empty_Left_Aligned" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_1_to_8_bits_When_Empty_Left_Aligned(); }
} testDescription_suite_UT_BinaryVector_test_Append_1_to_8_bits_When_Empty_Left_Aligned;

static class TestDescription_suite_UT_BinaryVector_test_Append_1_to_8_bits_When_NotEmpty_Right_Aligned : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_1_to_8_bits_When_NotEmpty_Right_Aligned() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 84, "test_Append_1_to_8_bits_When_NotEmpty_Right_Aligned" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_1_to_8_bits_When_NotEmpty_Right_Aligned(); }
} testDescription_suite_UT_BinaryVector_test_Append_1_to_8_bits_When_NotEmpty_Right_Aligned;

static class TestDescription_suite_UT_BinaryVector_test_Append_1_to_8_bits_When_NotEmpty_Left_Aligned : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_1_to_8_bits_When_NotEmpty_Left_Aligned() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 85, "test_Append_1_to_8_bits_When_NotEmpty_Left_Aligned" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_1_to_8_bits_When_NotEmpty_Left_Aligned(); }
} testDescription_suite_UT_BinaryVector_test_Append_1_to_8_bits_When_NotEmpty_Left_Aligned;

static class TestDescription_suite_UT_BinaryVector_test_Append_Other_When_Empty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_Other_When_Empty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 87, "test_Append_Other_When_Empty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_Other_When_Empty(); }
} testDescription_suite_UT_BinaryVector_test_Append_Other_When_Empty;

static class TestDescription_suite_UT_BinaryVector_test_Append_Other_When_NotEmpty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Append_Other_When_NotEmpty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 88, "test_Append_Other_When_NotEmpty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Append_Other_When_NotEmpty(); }
} testDescription_suite_UT_BinaryVector_test_Append_Other_When_NotEmpty;

static class TestDescription_suite_UT_BinaryVector_test_Operator_Shift_When_Empty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Operator_Shift_When_Empty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 92, "test_Operator_Shift_When_Empty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Operator_Shift_When_Empty(); }
} testDescription_suite_UT_BinaryVector_test_Operator_Shift_When_Empty;

static class TestDescription_suite_UT_BinaryVector_test_Operator_Shift_When_NotEmpty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Operator_Shift_When_NotEmpty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 93, "test_Operator_Shift_When_NotEmpty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Operator_Shift_When_NotEmpty(); }
} testDescription_suite_UT_BinaryVector_test_Operator_Shift_When_NotEmpty;

static class TestDescription_suite_UT_BinaryVector_test_Operator_Shift_When_Cascaded : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Operator_Shift_When_Cascaded() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 94, "test_Operator_Shift_When_Cascaded" ) {}
 void runTest() { suite_UT_BinaryVector.test_Operator_Shift_When_Cascaded(); }
} testDescription_suite_UT_BinaryVector_test_Operator_Shift_When_Cascaded;

static class TestDescription_suite_UT_BinaryVector_test_Operator_Plus_When_Empty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Operator_Plus_When_Empty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 98, "test_Operator_Plus_When_Empty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Operator_Plus_When_Empty(); }
} testDescription_suite_UT_BinaryVector_test_Operator_Plus_When_Empty;

static class TestDescription_suite_UT_BinaryVector_test_Operator_Plus_When_NotEmpty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Operator_Plus_When_NotEmpty() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 99, "test_Operator_Plus_When_NotEmpty" ) {}
 void runTest() { suite_UT_BinaryVector.test_Operator_Plus_When_NotEmpty(); }
} testDescription_suite_UT_BinaryVector_test_Operator_Plus_When_NotEmpty;

static class TestDescription_suite_UT_BinaryVector_test_ToggleBits : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_ToggleBits() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 103, "test_ToggleBits" ) {}
 void runTest() { suite_UT_BinaryVector.test_ToggleBits(); }
} testDescription_suite_UT_BinaryVector_test_ToggleBits;

static class TestDescription_suite_UT_BinaryVector_test_ToggleBits_Using_HexString : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_ToggleBits_Using_HexString() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 104, "test_ToggleBits_Using_HexString" ) {}
 void runTest() { suite_UT_BinaryVector.test_ToggleBits_Using_HexString(); }
} testDescription_suite_UT_BinaryVector_test_ToggleBits_Using_HexString;

static class TestDescription_suite_UT_BinaryVector_test_Operator_Tilde : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Operator_Tilde() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 105, "test_Operator_Tilde" ) {}
 void runTest() { suite_UT_BinaryVector.test_Operator_Tilde(); }
} testDescription_suite_UT_BinaryVector_test_Operator_Tilde;

static class TestDescription_suite_UT_BinaryVector_test_Slice : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Slice() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 109, "test_Slice" ) {}
 void runTest() { suite_UT_BinaryVector.test_Slice(); }
} testDescription_suite_UT_BinaryVector_test_Slice;

static class TestDescription_suite_UT_BinaryVector_test_Slice_When_Exceeding_Capacity : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_BinaryVector_test_Slice_When_Exceeding_Capacity() : CxxTest::RealTestDescription( Tests_UT_BinaryVector, suiteDescription_UT_BinaryVector, 110, "test_Slice_When_Exceeding_Capacity" ) {}
 void runTest() { suite_UT_BinaryVector.test_Slice_When_Exceeding_Capacity(); }
} testDescription_suite_UT_BinaryVector_test_Slice_When_Exceeding_Capacity;

#include "D:\Projects\Tima\NewMast\Lib_UT\SystemModel\UT_DefaultBinaryPathSelector.hpp"

static UT_DefaultBinaryPathSelector suite_UT_DefaultBinaryPathSelector;

static CxxTest::List Tests_UT_DefaultBinaryPathSelector = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_UT_DefaultBinaryPathSelector( ".//SystemModel/UT_DefaultBinaryPathSelector.hpp", 23, "UT_DefaultBinaryPathSelector", suite_UT_DefaultBinaryPathSelector, Tests_UT_DefaultBinaryPathSelector );

static class TestDescription_suite_UT_DefaultBinaryPathSelector_test_Constructor : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_DefaultBinaryPathSelector_test_Constructor() : CxxTest::RealTestDescription( Tests_UT_DefaultBinaryPathSelector, suiteDescription_UT_DefaultBinaryPathSelector, 34, "test_Constructor" ) {}
 void runTest() { suite_UT_DefaultBinaryPathSelector.test_Constructor(); }
} testDescription_suite_UT_DefaultBinaryPathSelector_test_Constructor;

#include "D:\Projects\Tima\NewMast\Lib_UT\SystemModel\UT_GmlPrinterVisitor.hpp"

static UT_GmlPrinterVisitor suite_UT_GmlPrinterVisitor;

static CxxTest::List Tests_UT_GmlPrinterVisitor = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_UT_GmlPrinterVisitor( ".//SystemModel/UT_GmlPrinterVisitor.hpp", 23, "UT_GmlPrinterVisitor", suite_UT_GmlPrinterVisitor, Tests_UT_GmlPrinterVisitor );

static class TestDescription_suite_UT_GmlPrinterVisitor_test_Constructor : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_GmlPrinterVisitor_test_Constructor() : CxxTest::RealTestDescription( Tests_UT_GmlPrinterVisitor, suiteDescription_UT_GmlPrinterVisitor, 31, "test_Constructor" ) {}
 void runTest() { suite_UT_GmlPrinterVisitor.test_Constructor(); }
} testDescription_suite_UT_GmlPrinterVisitor_test_Constructor;

static class TestDescription_suite_UT_GmlPrinterVisitor_test_VisitAccessInterface_with_Child : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_GmlPrinterVisitor_test_VisitAccessInterface_with_Child() : CxxTest::RealTestDescription( Tests_UT_GmlPrinterVisitor, suiteDescription_UT_GmlPrinterVisitor, 35, "test_VisitAccessInterface_with_Child" ) {}
 void runTest() { suite_UT_GmlPrinterVisitor.test_VisitAccessInterface_with_Child(); }
} testDescription_suite_UT_GmlPrinterVisitor_test_VisitAccessInterface_with_Child;

static class TestDescription_suite_UT_GmlPrinterVisitor_test_Visit_After_GetGraph : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_GmlPrinterVisitor_test_Visit_After_GetGraph() : CxxTest::RealTestDescription( Tests_UT_GmlPrinterVisitor, suiteDescription_UT_GmlPrinterVisitor, 36, "test_Visit_After_GetGraph" ) {}
 void runTest() { suite_UT_GmlPrinterVisitor.test_Visit_After_GetGraph(); }
} testDescription_suite_UT_GmlPrinterVisitor_test_Visit_After_GetGraph;

#include "D:\Projects\Tima\NewMast\Lib_UT\SystemModel\UT_PrettyPrinterVisitor.hpp"

static UT_PrettyPrinterVisitor suite_UT_PrettyPrinterVisitor;

static CxxTest::List Tests_UT_PrettyPrinterVisitor = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_UT_PrettyPrinterVisitor( ".//SystemModel/UT_PrettyPrinterVisitor.hpp", 22, "UT_PrettyPrinterVisitor", suite_UT_PrettyPrinterVisitor, Tests_UT_PrettyPrinterVisitor );

static class TestDescription_suite_UT_PrettyPrinterVisitor_test_Constructor : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_PrettyPrinterVisitor_test_Constructor() : CxxTest::RealTestDescription( Tests_UT_PrettyPrinterVisitor, suiteDescription_UT_PrettyPrinterVisitor, 33, "test_Constructor" ) {}
 void runTest() { suite_UT_PrettyPrinterVisitor.test_Constructor(); }
} testDescription_suite_UT_PrettyPrinterVisitor_test_Constructor;

static class TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitAccessInterface : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitAccessInterface() : CxxTest::RealTestDescription( Tests_UT_PrettyPrinterVisitor, suiteDescription_UT_PrettyPrinterVisitor, 37, "test_VisitAccessInterface" ) {}
 void runTest() { suite_UT_PrettyPrinterVisitor.test_VisitAccessInterface(); }
} testDescription_suite_UT_PrettyPrinterVisitor_test_VisitAccessInterface;

static class TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitAccessInterface_Verbose : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitAccessInterface_Verbose() : CxxTest::RealTestDescription( Tests_UT_PrettyPrinterVisitor, suiteDescription_UT_PrettyPrinterVisitor, 38, "test_VisitAccessInterface_Verbose" ) {}
 void runTest() { suite_UT_PrettyPrinterVisitor.test_VisitAccessInterface_Verbose(); }
} testDescription_suite_UT_PrettyPrinterVisitor_test_VisitAccessInterface_Verbose;

static class TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitChain : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitChain() : CxxTest::RealTestDescription( Tests_UT_PrettyPrinterVisitor, suiteDescription_UT_PrettyPrinterVisitor, 39, "test_VisitChain" ) {}
 void runTest() { suite_UT_PrettyPrinterVisitor.test_VisitChain(); }
} testDescription_suite_UT_PrettyPrinterVisitor_test_VisitChain;

static class TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitChain_Verbose : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitChain_Verbose() : CxxTest::RealTestDescription( Tests_UT_PrettyPrinterVisitor, suiteDescription_UT_PrettyPrinterVisitor, 40, "test_VisitChain_Verbose" ) {}
 void runTest() { suite_UT_PrettyPrinterVisitor.test_VisitChain_Verbose(); }
} testDescription_suite_UT_PrettyPrinterVisitor_test_VisitChain_Verbose;

static class TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitLinker : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitLinker() : CxxTest::RealTestDescription( Tests_UT_PrettyPrinterVisitor, suiteDescription_UT_PrettyPrinterVisitor, 41, "test_VisitLinker" ) {}
 void runTest() { suite_UT_PrettyPrinterVisitor.test_VisitLinker(); }
} testDescription_suite_UT_PrettyPrinterVisitor_test_VisitLinker;

static class TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitLinker_Verbose : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitLinker_Verbose() : CxxTest::RealTestDescription( Tests_UT_PrettyPrinterVisitor, suiteDescription_UT_PrettyPrinterVisitor, 42, "test_VisitLinker_Verbose" ) {}
 void runTest() { suite_UT_PrettyPrinterVisitor.test_VisitLinker_Verbose(); }
} testDescription_suite_UT_PrettyPrinterVisitor_test_VisitLinker_Verbose;

static class TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitRegister : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitRegister() : CxxTest::RealTestDescription( Tests_UT_PrettyPrinterVisitor, suiteDescription_UT_PrettyPrinterVisitor, 43, "test_VisitRegister" ) {}
 void runTest() { suite_UT_PrettyPrinterVisitor.test_VisitRegister(); }
} testDescription_suite_UT_PrettyPrinterVisitor_test_VisitRegister;

static class TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitRegister_Verbose : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitRegister_Verbose() : CxxTest::RealTestDescription( Tests_UT_PrettyPrinterVisitor, suiteDescription_UT_PrettyPrinterVisitor, 44, "test_VisitRegister_Verbose" ) {}
 void runTest() { suite_UT_PrettyPrinterVisitor.test_VisitRegister_Verbose(); }
} testDescription_suite_UT_PrettyPrinterVisitor_test_VisitRegister_Verbose;

static class TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitTap : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitTap() : CxxTest::RealTestDescription( Tests_UT_PrettyPrinterVisitor, suiteDescription_UT_PrettyPrinterVisitor, 45, "test_VisitTap" ) {}
 void runTest() { suite_UT_PrettyPrinterVisitor.test_VisitTap(); }
} testDescription_suite_UT_PrettyPrinterVisitor_test_VisitTap;

static class TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitTap_Verbose : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitTap_Verbose() : CxxTest::RealTestDescription( Tests_UT_PrettyPrinterVisitor, suiteDescription_UT_PrettyPrinterVisitor, 46, "test_VisitTap_Verbose" ) {}
 void runTest() { suite_UT_PrettyPrinterVisitor.test_VisitTap_Verbose(); }
} testDescription_suite_UT_PrettyPrinterVisitor_test_VisitTap_Verbose;

static class TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitChain_with_Register : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitChain_with_Register() : CxxTest::RealTestDescription( Tests_UT_PrettyPrinterVisitor, suiteDescription_UT_PrettyPrinterVisitor, 50, "test_VisitChain_with_Register" ) {}
 void runTest() { suite_UT_PrettyPrinterVisitor.test_VisitChain_with_Register(); }
} testDescription_suite_UT_PrettyPrinterVisitor_test_VisitChain_with_Register;

static class TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitChain_with_Registers : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitChain_with_Registers() : CxxTest::RealTestDescription( Tests_UT_PrettyPrinterVisitor, suiteDescription_UT_PrettyPrinterVisitor, 54, "test_VisitChain_with_Registers" ) {}
 void runTest() { suite_UT_PrettyPrinterVisitor.test_VisitChain_with_Registers(); }
} testDescription_suite_UT_PrettyPrinterVisitor_test_VisitChain_with_Registers;

static class TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitChain_with_Registers_Verbose : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitChain_with_Registers_Verbose() : CxxTest::RealTestDescription( Tests_UT_PrettyPrinterVisitor, suiteDescription_UT_PrettyPrinterVisitor, 55, "test_VisitChain_with_Registers_Verbose" ) {}
 void runTest() { suite_UT_PrettyPrinterVisitor.test_VisitChain_with_Registers_Verbose(); }
} testDescription_suite_UT_PrettyPrinterVisitor_test_VisitChain_with_Registers_Verbose;

static class TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitLinker_with_Child : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitLinker_with_Child() : CxxTest::RealTestDescription( Tests_UT_PrettyPrinterVisitor, suiteDescription_UT_PrettyPrinterVisitor, 56, "test_VisitLinker_with_Child" ) {}
 void runTest() { suite_UT_PrettyPrinterVisitor.test_VisitLinker_with_Child(); }
} testDescription_suite_UT_PrettyPrinterVisitor_test_VisitLinker_with_Child;

static class TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitLinker_with_Child_Verbose : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitLinker_with_Child_Verbose() : CxxTest::RealTestDescription( Tests_UT_PrettyPrinterVisitor, suiteDescription_UT_PrettyPrinterVisitor, 57, "test_VisitLinker_with_Child_Verbose" ) {}
 void runTest() { suite_UT_PrettyPrinterVisitor.test_VisitLinker_with_Child_Verbose(); }
} testDescription_suite_UT_PrettyPrinterVisitor_test_VisitLinker_with_Child_Verbose;

static class TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitAccessInterface_with_Child : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitAccessInterface_with_Child() : CxxTest::RealTestDescription( Tests_UT_PrettyPrinterVisitor, suiteDescription_UT_PrettyPrinterVisitor, 58, "test_VisitAccessInterface_with_Child" ) {}
 void runTest() { suite_UT_PrettyPrinterVisitor.test_VisitAccessInterface_with_Child(); }
} testDescription_suite_UT_PrettyPrinterVisitor_test_VisitAccessInterface_with_Child;

static class TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitAccessInterface_Hex_Format : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_PrettyPrinterVisitor_test_VisitAccessInterface_Hex_Format() : CxxTest::RealTestDescription( Tests_UT_PrettyPrinterVisitor, suiteDescription_UT_PrettyPrinterVisitor, 59, "test_VisitAccessInterface_Hex_Format" ) {}
 void runTest() { suite_UT_PrettyPrinterVisitor.test_VisitAccessInterface_Hex_Format(); }
} testDescription_suite_UT_PrettyPrinterVisitor_test_VisitAccessInterface_Hex_Format;

#include "D:\Projects\Tima\NewMast\Lib_UT\SystemModel\UT_Register.hpp"

static UT_Register suite_UT_Register;

static CxxTest::List Tests_UT_Register = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_UT_Register( ".//SystemModel/UT_Register.hpp", 22, "UT_Register", suite_UT_Register, Tests_UT_Register );

static class TestDescription_suite_UT_Register_test_Constructor : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_UT_Register_test_Constructor() : CxxTest::RealTestDescription( Tests_UT_Register, suiteDescription_UT_Register, 33, "test_Constructor" ) {}
 void runTest() { suite_UT_Register.test_Constructor(); }
} testDescription_suite_UT_Register_test_Constructor;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName      = "cxxtest";
bool CxxTest::RealWorldDescription::_displayFilePath = false;
