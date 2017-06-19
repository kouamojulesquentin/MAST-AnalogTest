//===========================================================================
//                           DummyListener.h
//===========================================================================
// Copyright (C) 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file DummyListener.h
//!
//! Declares class that implements Listener but do absolutly nothing
//!
//===========================================================================


#ifndef DUMMYLISTENER_H__8F69C2F2_E24F_4AA3_B199_A04EEEF42FC9__INCLUDED_
  #define DUMMYLISTENER_H__8F69C2F2_E24F_4AA3_B199_A04EEEF42FC9__INCLUDED_


// The names of the parameters are in comments to avoid
// "unused parameter" warnings.
//

#include <cxxtest/Descriptions.h>
#include <cstring>

namespace CxxTest
{
struct DifferenceInfo;

//! This is a place holder when there is no effective TestListener yet but we don't want to test on every path
//!
class DummyListener : public TestListener
{
public:
    DummyListener() {}
    virtual ~DummyListener() {}
    virtual void enterSuite (const SuiteDescription& /*desc*/) {}
    virtual void enterTest  (const TestDescription&  /*desc*/) {}
    virtual void enterWorld (const WorldDescription& /*desc*/) {}

    virtual void leaveSuite (const SuiteDescription&  /*desc*/) {}
    virtual void leaveTest  (const TestDescription&   /*desc*/) {}
    virtual void leaveWorld (const WorldDescription&  /*desc*/) {}

    virtual void unexpectExceptionFromTest (const char* /*exceptionType*/, const char* /*exceptionValue*/, const char* /*file*/, int /*line*/) {}

    virtual void trace                     (const char* /*message*/, const char* /*file*/, int /*line*/) {}
    virtual void warning                   (const char* /*message*/, const char* /*file*/, int /*line*/) {}
    virtual void skippedTest               (const char* /*message*/, const char* /*file*/, int /*line*/) {}
    virtual void failedTest                (const char* /*message*/, const char* /*file*/, int /*line*/) {}
    virtual void failedTestInternal        (const char* /*message*/, const char* /*file*/, int /*line*/) {}
    virtual void notSupportedAssert        (const char* /*message*/, const char* /*file*/, int /*line*/, const char* /*notSupportedAssert*/, const char* /*requiredCondition*/) {};

    virtual void failedAssertTrue              (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*expression*/) {}
    virtual void failedAssertFalse             (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*expression*/) {}
    virtual void failedAssertNotNullptr        (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*expression*/) {}
    virtual void failedAssertNullptr           (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*expression*/) {}
    virtual void failedAssertContains          (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*containerExpr*/, const char* /*valueExpr*/, const char* /*containerContent*/, const char* /*value*/) {}
    virtual void failedAssertNotContains       (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*containerExpr*/, const char* /*valueExpr*/, const char* /*containerContent*/, const char* /*value*/) {}
    virtual void failedAssertEmpty             (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*containerExpr*/, const char* /*containerContent*/) {}
    virtual void failedAssertNotEmpty          (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*containerExpr*/) {}
    virtual void failedAssertEquals            (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*xStr*/, const char* /*yStr*/, const char* /*x*/, const char* /*y*/) {}
    virtual void failedAssertSameData          (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*xStr*/, const char* /*yStr*/, const char* /*sizeStr*/, const void* /*x*/, const void* /*y*/, unsigned /*size*/) {}
    virtual void failedAssertDelta             (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*xStr*/, const char* /*yStr*/, const char* /*dStr*/, const char* /*x*/, const char* /*y*/, const char* /*d*/) {}
    virtual void failedAssertDiffers           (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*xStr*/, const char* /*yStr*/, const char* /*value*/) {}
    virtual void failedAssertFileContent       (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*checkedFile*/, const char* /*expectedContent*/, const char* /*expectedContentExpr*/, const DifferenceInfo& /*diffInfo*/) {}
    virtual void failedAssertFileNotExists     (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*checkedFile*/) {}
    virtual void failedAssertFileExists        (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*checkedFile*/) {}
    virtual void failedAssertFileSize          (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*checkedFile*/, int /*expectedSize*/, int /*effectiveSize*/) {}
    virtual void failedAssertSameFiles         (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* , const char*, const char*) {}
    virtual void failedAssertGreaterThan       (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*xStr*/, const char* /*yStr*/, const char* /*x*/, const char* /*y*/) {}
    virtual void failedAssertGreaterThanEquals (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*xStr*/, const char* /*yStr*/, const char* /*x*/, const char* /*y*/) {}
    virtual void failedAssertLessThan          (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*xStr*/, const char* /*yStr*/, const char* /*x*/, const char* /*y*/) {}
    virtual void failedAssertLessThanEquals    (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*xStr*/, const char* /*yStr*/, const char* /*x*/, const char* /*y*/) {}
    virtual void failedAssertPredicate         (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*predicate*/, const char* /*xStr*/, const char* /*x*/) {}
    virtual void failedAssertRelation          (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*relation*/, const char* /*xStr*/, const char* /*yStr*/, const char* /*x*/, const char* /*y*/) {}
    virtual void failedAssertThrows            (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*expression*/, const char* /*exceptionMessage*/, const char* /*type*/, bool /*otherThrown*/) {}
    virtual void failedAssertThrowsNothing     (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*expression*/, const char* /*exceptionMessage*/) {}
    virtual void failedAssertThrowsAnything    (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*expression*/) {}

    IGNORE_SUCCEEDED_ASSERTIONS
};
} // End of namespace CxxTest


#endif  // not defined DUMMYLISTENER_H__8F69C2F2_E24F_4AA3_B199_A04EEEF42FC9__INCLUDED_
//===========================================================================
// End of DummyListener.h
//===========================================================================


