/*
-------------------------------------------------------------------------
 CxxTest: A lightweight C++ unit testing library.
 Copyright (c) 2008 Sandia Corporation.
 This software is distributed under the LGPL License v3
 For more information, see the COPYING file in the top CxxTest directory.
 Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
 the U.S. Government retains certain rights in this software.
-------------------------------------------------------------------------
*/

#ifndef __cxxtest__RealDescriptions_h__
#define __cxxtest__RealDescriptions_h__

//
// The "real" description classes
//

#include <cxxtest/Descriptions.h>
#include <cxxtest/TestSuite.h>
#include <cxxtest/GlobalFixture.h>

namespace CxxTest
{
class RealTestDescription : public TestDescription
{
public:
    RealTestDescription();
    RealTestDescription (List &argList, SuiteDescription &argSuite, unsigned argLine, const char* argTestName);
    void initialize     (List &argList, SuiteDescription &argSuite, unsigned argLine, const char* argTestName);

    virtual const char* file() const;
    virtual int         line() const;
    virtual const char* testName() const;
    virtual const char* suiteName() const;

    virtual bool        isDataDriven()            const { return _isDataDriven;      }; //!< True when test is in fact a "master" data driven test
    virtual int         currentDataDrivenTestId() const { return _testId;            }; //!< When a test is in fact a "master" data driven test, this gives index of input sample the test is run with
    virtual const char* inputSampleAsText()       const { return _inputSampleAsText; }; //!< Textual representation of input of a data driven sub-test

    virtual void isDataDriven            (bool        dataDriven)  { _isDataDriven      = dataDriven;  };
    virtual void currentDataDrivenTestId (int         testId)      { _testId            = testId;      };
    virtual void inputSampleAsText       (const char* inputSample) { _inputSampleAsText = inputSample; };

    TestDescription*       next();
    const TestDescription* next() const;

    TestSuite* suite() const;

    bool setUp();
    void run();
    bool tearDown();

private:
    RealTestDescription(const RealTestDescription &);
    RealTestDescription &operator=(const RealTestDescription &);

    virtual void runTest() = 0;

    SuiteDescription* _suite;
    int               _line;
    const char*       _testName;
    bool              _isDataDriven;
    int               _testId;
    const char*       _inputSampleAsText;
};

class RealSuiteDescription : public SuiteDescription
{
public:
    RealSuiteDescription();
    RealSuiteDescription(const char *argFile, unsigned argLine, const char *argSuiteName, List &argTests);

    void initialize(const char *argFile, unsigned argLine, const char *argSuiteName, List &argTests);

    const char *file() const;
    int line() const;
    const char *suiteName() const;

    TestDescription *firstTest();
    const TestDescription *firstTest() const;
    SuiteDescription *next();
    const SuiteDescription *next() const;

    unsigned numTests() const;
    const TestDescription &testDescription(unsigned i) const;

    void activateAllTests();
    bool leaveOnly(const char *testName);

private:
    RealSuiteDescription(const RealSuiteDescription &);
    RealSuiteDescription &operator=(const RealSuiteDescription &);

    const char *_file;
    int _line;
    const char *_suiteName;
    List *_tests;

    static List _suites;
    friend class RealWorldDescription;
};

class StaticSuiteDescription : public RealSuiteDescription
{
public:
    StaticSuiteDescription();
    StaticSuiteDescription(const char *argFile, unsigned argLine,
                           const char *argSuiteName, TestSuite &argSuite,
                           List &argTests);

    void initialize(const char *argFile, unsigned argLine,
                    const char *argSuiteName, TestSuite &argSuite,
                    List &argTests);
    TestSuite *suite() const;

    bool setUp();
    bool tearDown();

private:
    StaticSuiteDescription(const StaticSuiteDescription &);
    StaticSuiteDescription &operator=(const StaticSuiteDescription &);

    void doInitialize(TestSuite &argSuite);

    TestSuite *_suite;
};

class CommonDynamicSuiteDescription : public RealSuiteDescription
{
public:
    CommonDynamicSuiteDescription();
    CommonDynamicSuiteDescription(const char *argFile, unsigned argLine,
                                  const char *argSuiteName, List &argTests,
                                  unsigned argCreateLine, unsigned argDestroyLine);

    void initialize(const char *argFile, unsigned argLine,
                    const char *argSuiteName, List &argTests,
                    unsigned argCreateLine, unsigned argDestroyLine);

protected:
    unsigned _createLine, _destroyLine;

private:
    void doInitialize(unsigned argCreateLine, unsigned argDestroyLine);
};

template<class S>
class DynamicSuiteDescription : public CommonDynamicSuiteDescription
{
public:
    DynamicSuiteDescription() {}
    DynamicSuiteDescription(const char *argFile, unsigned argLine,
                            const char *argSuiteName, List &argTests,
                            S *&argSuite, unsigned argCreateLine,
                            unsigned argDestroyLine) :
        CommonDynamicSuiteDescription(argFile, argLine, argSuiteName, argTests, argCreateLine, argDestroyLine)
    {
        _suite = &argSuite;
    }

    void initialize(const char *argFile, unsigned argLine,
                    const char *argSuiteName, List &argTests,
                    S *&argSuite, unsigned argCreateLine,
                    unsigned argDestroyLine)
    {
        CommonDynamicSuiteDescription::initialize(argFile, argLine,
                argSuiteName, argTests,
                argCreateLine, argDestroyLine);
        _suite = &argSuite;
    }

    TestSuite *suite() const { return realSuite(); }

    bool setUp();
    bool tearDown();

private:
    S *realSuite() const { return *_suite; }
    void setSuite(S *s) { *_suite = s; }

    void createSuite()
    {
        setSuite(S::createSuite());
    }

    void destroySuite()
    {
        S *s = realSuite();
        setSuite(0);
        S::destroySuite(s);
    }

    S **_suite;
};

template<class S>
bool DynamicSuiteDescription<S>::setUp()
{
    _TS_TRY
    {
        createSuite();
        //! @todo [JFC]-[September/28/2015]: In setUp(): replace with failedAssertNotNullptr
        //!
        if (suite() == NULL)
        {
          tracker().failedAssertTrue("createSuite() failed", file(), _createLine, "suite() != NULL");
        }
    }
    _TS_CATCH_ABORT   ( { return false; })
    _TS_CATCH_SKIPPED ( { return false; })
    _TS_LAST_CATCH    ( { tracker().failedAssertThrowsNothing("", file(), _createLine, "createSuite()", NULL); return false; })

    return (suite() != 0);
}

template<class S>
bool DynamicSuiteDescription<S>::tearDown()
{
    if (!_suite)
    {
        return true;
    }

    _TS_TRY
    {
        _TS_ASSERT_THROWS_NOTHING(file(), _destroyLine, destroySuite());
    }
    _TS_CATCH_ABORT   ( { return false; })
    _TS_CATCH_SKIPPED ( { return false; });

    return true;
}

class RealWorldDescription : public WorldDescription
{
public:
    static List& suites ();

    virtual bool                    displayFilePath  () const { return _displayFilePath;}   //!< Tells whether files name should be reported with their full path or just the file name
    virtual bool                    displaySuccess   () const { return _displaySuccess;}    //!< Tells whether successful assertion details are added into the test report
    virtual const char*             worldName        () const { return _worldName;}
    virtual unsigned                numSuites        (void) const;
    virtual unsigned                numTotalTests    (void) const;
    virtual const SuiteDescription& suiteDescription (unsigned i) const;

    virtual SuiteDescription*       firstSuite       ();
    virtual const SuiteDescription* firstSuite       () const;
    virtual void                    activateAllTests ();
    virtual bool                    leaveOnly        (const char *suiteName, const char *testName = 0);

    bool setUp();
    bool tearDown();
    static void reportError(const char *message);

    static const char *_worldName;
    static bool        _displayFilePath;
    static bool        _displaySuccess;
};

void activateAllTests();
bool leaveOnly(const char *suiteName, const char *testName = 0);
}

#endif // __cxxtest__RealDescriptions_h__

