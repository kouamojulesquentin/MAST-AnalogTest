//-------------------------------------------------------------------------
// CxxTest: A lightweight C++ unit testing library.
// Copyright (c) 2008 Sandia Corporation.
// This software is distributed under the LGPL License v3
// For more information, see the COPYING file in the top CxxTest directory.
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//-------------------------------------------------------------------------

#ifndef __cxxtest__Descriptions_h__
#define __cxxtest__Descriptions_h__

//
// TestDescription, SuiteDescription and WorldDescription
// hold information about tests so they can be run and reported.
//

#include <cxxtest/LinkedList.h>

namespace CxxTest
{
class TestSuite;

class TestDescription : public Link
{
public:
    virtual ~TestDescription();

    virtual const char* file() const = 0;
    virtual int         line() const = 0;
    virtual const char* testName() const = 0;
    virtual const char* suiteName() const = 0;


    virtual bool        isDataDriven() const = 0;            //!< True when test is in fact a "master" data driven test
    virtual int         currentDataDrivenTestId() const = 0; //!< When a test is in fact a "master" data driven test, this gives index of input sample the test is run with
    virtual const char* inputSampleAsText () const = 0;       //!< Textual representation of input of a data driven sub-test

    virtual void isDataDriven            (bool isDataDriven) = 0;
    virtual void currentDataDrivenTestId (int  testId)       = 0;
    virtual void inputSampleAsText (const char* inputSample) = 0;

    virtual void run() = 0;
    virtual bool setUp() = 0;
    virtual bool tearDown() = 0;

    virtual const TestDescription* next() const = 0;
    virtual TestDescription*       next() = 0;
};

class SuiteDescription : public Link
{
public:
    virtual ~SuiteDescription();

    virtual const char *file() const = 0;
    virtual int line() const = 0;
    virtual const char *suiteName() const = 0;
    virtual TestSuite *suite() const = 0;

    virtual unsigned numTests() const = 0;
    virtual const TestDescription &testDescription(unsigned /*i*/) const = 0;

    virtual TestDescription *firstTest() = 0;
    virtual const TestDescription *firstTest() const = 0;
    virtual SuiteDescription *next() = 0;
    virtual const SuiteDescription *next() const = 0;

    virtual void activateAllTests() = 0;
    virtual bool leaveOnly(const char * /*testName*/) = 0;

    virtual bool setUp() = 0;
    virtual bool tearDown() = 0;
};

class WorldDescription : public Link
{
public:
    virtual ~WorldDescription();

    virtual bool                    displayFilePath  () const { return false;}   //!< Tells whether files name should be reported with their full path or just the file name
    virtual bool                    displaySuccess   () const { return false;}   //!< Tells whether successful assertion details are added into the test report
    virtual const char*             worldName        () const { return "cxxtest"; }
    virtual unsigned                numSuites        (void) const = 0;
    virtual unsigned                numTotalTests    (void) const = 0;
    virtual const SuiteDescription& suiteDescription (unsigned /*i*/) const = 0;

    enum { MAX_STRLEN_TOTAL_TESTS = 32 };
    char *strTotalTests(char * /*buffer*/) const;

    virtual SuiteDescription*       firstSuite() = 0;
    virtual const SuiteDescription* firstSuite() const = 0;

    virtual void activateAllTests() = 0;
    virtual bool leaveOnly(const char * /*suiteName*/, const char * /*testName*/ = 0) = 0;
};
}

#endif // __cxxtest__Descriptions_h__

