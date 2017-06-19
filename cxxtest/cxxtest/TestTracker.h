//-------------------------------------------------------------------------
// CxxTest: A lightweight C++ unit testing library.
// Copyright (c) 2008 Sandia Corporation.
// This software is distributed under the LGPL License v3
// For more information, see the COPYING file in the top CxxTest directory.
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//-------------------------------------------------------------------------

#ifndef __cxxtest__TestTracker_h__
#define __cxxtest__TestTracker_h__

//
// The TestTracker tracks running tests
// The actual work is done in CountingListenerProxy,
// but this way avoids cyclic references TestListener<->CountingListenerProxy
//

#include <cxxtest/TestListener.h>
#include <cxxtest/DummyDescriptions.h>
#include <cxxtest/DummyListener.h>

#if (__cplusplus >= 201103L)
#include <chrono>
#endif

namespace CxxTest
{
struct DifferenceInfo;

class TestTracker
{
public:
    virtual ~TestTracker();

    static TestTracker &tracker();
    static bool print_tracing;
    static bool display_test_duration;


          TestDescription&  test()        { return *fixTest(_test);  }  // Make sure that no reference from NULL pointer is returned !
    const TestDescription&  test()  const { return *fixTest(_test);  }  // Make sure that no reference from NULL pointer is returned !
    const SuiteDescription& suite() const { return *_suite; }
    const WorldDescription& world() const { return *_world; }


    bool masterTestHadAssertion() const { return _hadAsserts;         }       //!< Returns whether a test (including data driven test or not had execucted at least one assertion).
    bool testHasNoAssertion()     const { return _testHasNoAssertion; }       //!< Returns whether a single test (from data driven test or not) had execucted at least one assertion).
    bool testSkipped()            const { return _testSkipped;        }       //!< Returns the number of skipped test so far
    bool testFailed()             const { return (testFailedAsserts() > 0); } //!< Returns whether a single test (from data driven test or not) had a failed assertion
    bool suiteFailed()            const { return (suiteFailedTests()  > 0); } //!< Returns whether the suited had at least one test in failure
    bool worldFailed()            const { return (failedSuites()      > 0); } //!< Returns whether the world had at least one test in failure

    unsigned warnings()               const { return _warnings;                }
    unsigned skippedTests()           const { return _skippedTests;            }
    unsigned failedTests()            const { return _failedTests;             }
    unsigned testAsserts()            const { return _testAsserts;             }
    unsigned testFailedAsserts()      const { return _testFailedAsserts;       }
    unsigned suiteFailedTests()       const { return _suiteFailedTests;        }
    unsigned failedSuites()           const { return _failedSuites;            }
    unsigned dataDrivenTests()        const { return _dataDrivenTests;        }
    unsigned dataDrivenTestsSamples() const { return _overallDataDrivenTestsSamples; }

    virtual void enterWorld           (const WorldDescription &wd);
    virtual void enterSuite           (const SuiteDescription &sd);
    virtual void enterTest            (TestDescription  &td);
    virtual void enterDataDrivenTest  (int testId, const char* inputSample);
    virtual void leaveTest            (const TestDescription  &td);
    virtual void leaveDataDrivenTest  ();
    virtual void leaveDataDrivenTests ();
    virtual void leaveSuite           (const SuiteDescription &sd);
    virtual void leaveWorld           (const WorldDescription &wd);

    void countAsserts();                          //!< Public to be called from outside to really count only user assertions
    void countDataDrivenTests(unsigned samples);  //!< Tallies the number of tests that are in fact defining (dynamically) data driven tests along with samples used to run data driven tests
    bool isLastDataDrivenTest(const TestDescription &td) const;

    const char* lastAssertFile() const { return _lastAssertFile; };
    int         lastAssertLine() const { return _lastAssertLine; };

    virtual void trace                      (const char* message, const char* file, int line);
    virtual void warning                    (const char* message, const char* file, int line);
    virtual void skippedTest                (const char* message, const char* file, int line);
    virtual void notSupportedAssert         (const char* message, const char* file, int line, const char* notSupportedAssertName, const char* requiredCondition);

    virtual void unexpectExceptionFromTest  (const char* exceptionType, const char* exceptionValue, const char* file, int line);

    virtual void failedTest                    (const char* message, const char* file, int line);
    virtual void failedTestInternal            (const char* message, const char* file, int line);
    virtual void failedAssertTrue              (const char* message, const char* file, int line, const char* expression);
    virtual void failedAssertFalse             (const char* message, const char* file, int line, const char* expression);
    virtual void failedAssertNullptr           (const char* message, const char* file, int line, const char* expression);
    virtual void failedAssertNotNullptr        (const char* message, const char* file, int line, const char* expression);
    virtual void failedAssertContains          (const char* message, const char* file, int line, const char* containerExpr, const char* valueExpr, const char* containerContent, const char* value);
    virtual void failedAssertNotContains       (const char* message, const char* file, int line, const char* containerExpr, const char* valueExpr, const char* containerContent, const char* value);
    virtual void failedAssertEmpty             (const char* message, const char* file, int line, const char* containerExpr, const char* containerContent);
    virtual void failedAssertNotEmpty          (const char* message, const char* file, int line, const char* containerExpr);
    virtual void failedAssertEquals            (const char* message, const char* file, int line, const char* xStr, const char* yStr, const char* x,       const char* y);
    virtual void failedAssertSameData          (const char* message, const char* file, int line, const char* xStr, const char* yStr, const char* sizeStr, const void* x, const void* y, unsigned    size);
    virtual void failedAssertDelta             (const char* message, const char* file, int line, const char* xStr, const char* yStr, const char* dStr,    const char* x, const char* y, const char* d);
    virtual void failedAssertDiffers           (const char* message, const char* file, int line, const char* xStr, const char* yStr, const char* value);
    virtual void failedAssertFileContent       (const char* message, const char* file, int line, const char* checkedFile, const char* expectedContent, const char* expectedContentExpr, const DifferenceInfo& diffInfo);
    virtual void failedAssertFileExists        (const char* message, const char* file, int line, const char* checkedFile);
    virtual void failedAssertFileNotExists     (const char* message, const char* file, int line, const char* checkedFile);
    virtual void failedAssertFileSize          (const char* message, const char* file, int line, const char* checkedFile, int expectedSize, int effectiveSize);
    virtual void failedAssertLessThan          (const char* message, const char* file, int line, const char* xStr,        const char* yStr, const char* x, const char* y);
    virtual void failedAssertLessThanEquals    (const char* message, const char* file, int line, const char* xStr,        const char* yStr, const char* x, const char* y);
    virtual void failedAssertGreaterThan       (const char* message, const char* file, int line, const char* xStr,        const char* yStr, const char* x, const char* y);
    virtual void failedAssertGreaterThanEquals (const char* message, const char* file, int line, const char* xStr,        const char* yStr, const char* x, const char* y);
    virtual void failedAssertPredicate         (const char* message, const char* file, int line, const char* predicate,   const char* xStr, const char* x);
    virtual void failedAssertRelation          (const char* message, const char* file, int line, const char* relation,    const char* xStr, const char* yStr, const char* x, const char* y);
    virtual void failedAssertThrows            (const char* message, const char* file, int line, const char* expression,  const char* exceptionMessage, const char* type, bool otherThrown);
    virtual void failedAssertThrowsNothing     (const char* message, const char* file, int line, const char* expression,  const char* exceptionMessage);
    virtual void failedAssertThrowsAnything    (const char* message, const char* file, int line, const char* expression);
    virtual void failedAssertSameFiles         (const char* message, const char* file, int line, const char* file1, const char* file2, const char* explanation);


    virtual void succeededAssertTrue              (const char* message, const char* file, int line, const char* expression);
    virtual void succeededAssertFalse             (const char* message, const char* file, int line, const char* expression);
    virtual void succeededAssertNotNullptr        (const char* message, const char* file, int line, const char* expression);
    virtual void succeededAssertNullptr           (const char* message, const char* file, int line, const char* expression);
    virtual void succeededAssertDelta             (const char* message, const char* file, int line, const char* xStr, const char* yStr, const char* dStr,    const char* x, const char* y, const char* d);
    virtual void succeededAssertDiffers           (const char* message, const char* file, int line, const char* xStr, const char* yStr, const char* x,       const char* y);
    virtual void succeededAssertEquals            (const char* message, const char* file, int line, const char* xStr, const char* yStr, const char* x,       const char* y);
    virtual void succeededAssertContains          (const char* message, const char* file, int line, const char* containerExpr, const char* valueExpr, const char* containerContent, const char* value);
    virtual void succeededAssertNotContains       (const char* message, const char* file, int line, const char* containerExpr, const char* valueExpr, const char* containerContent, const char* value);
    virtual void succeededAssertEmpty             (const char* message, const char* file, int line, const char* containerExpr);
    virtual void succeededAssertNotEmpty          (const char* message, const char* file, int line, const char* containerExpr, const char* containerContent);
    virtual void succeededAssertFileContent       (const char* message, const char* file, int line, const char* checkedFile, const char* expectedContent, const char* expectedContentExpr, const DifferenceInfo& diffInfo);
    virtual void succeededAssertFileExists        (const char* message, const char* file, int line, const char* checkedFile);
    virtual void succeededAssertFileNotExists     (const char* message, const char* file, int line, const char* checkedFile);
    virtual void succeededAssertFileSize          (const char* message, const char* file, int line, const char* checkedFile, int expectedSize, int effectiveSize);
    virtual void succeededAssertLessThan          (const char* message, const char* file, int line, const char* xStr, const char* yStr, const char* x,       const char* y);
    virtual void succeededAssertLessThanEquals    (const char* message, const char* file, int line, const char* xStr, const char* yStr, const char* x,       const char* y);
    virtual void succeededAssertGreaterThan       (const char* message, const char* file, int line, const char* xStr, const char* yStr, const char* x,       const char* y);
    virtual void succeededAssertGreaterThanEquals (const char* message, const char* file, int line, const char* xStr, const char* yStr, const char* x,       const char* y);
    virtual void succeededAssertPredicate         (const char* message, const char* file, int line, const char* pStr, const char* xStr, const char* x);
    virtual void succeededAssertRelation          (const char* message, const char* file, int line, const char* rStr, const char* xStr, const char* yStr,    const char* x, const char* y);
    virtual void succeededAssertSameData          (const char* message, const char* file, int line, const char* xStr, const char* yStr, const char* sizeStr, const void* x, const void* y, unsigned size);
    virtual void succeededAssertThrows            (const char* message, const char* file, int line, const char* expr, const char* exceptionMessage, const char* type);
    virtual void succeededAssertThrowsNothing     (const char* message, const char* file, int line, const char* expr);
    virtual void succeededAssertThrowsAnything    (const char* message, const char* file, int line, const char* expr);
    virtual void succeededAssertSameFiles         (const char* message, const char* file, int line, const char* file1, const char* file2);

    virtual void initialize();

private:
    TestTracker();
    TestTracker(const TestTracker &);
    TestTracker &operator=(const TestTracker &);

    TestDescription*        fixTest  (TestDescription*        td) const;
    const SuiteDescription* fixSuite (const SuiteDescription* sd) const;
    const WorldDescription* fixWorld (const WorldDescription* wd) const;

    const TestDescription&  dummyTest()  const;
    const SuiteDescription& dummySuite() const;
    const WorldDescription& dummyWorld() const;

    void setWorld (const WorldDescription* wd);
    void setSuite (const SuiteDescription* sd);
    void setTest  (      TestDescription*  td);

    void countWarning();
    void countFailure();
    void countSkipped();

    //! Updates member _testHasNoAssertion (normally at end of test)
    //!
    void updateTestHasNoAssertion ();


    friend class TestRunner;

    void setListener(TestListener* listener);

    static bool             _created;
    DummyListener           _dummyListener; // Dummy instance to use when there no actual TestListener
    DummyWorldDescription   _dummyWorld;
    TestListener*           m_pListener;
    const WorldDescription* _world;
    const SuiteDescription* _suite;
    TestDescription*        _test;        // Not const to be able to manage, dynamically, data driven tests

    const char* _lastAssertFile;          //!< The file parameter of last assert (must outlive the assert scope). This is for better unexpected exception localization
    int         _lastAssertLine;          //!< The line parameter of last assert
    bool        _testSkipped;
    bool        _testHasNoAssertion;

    unsigned _warnings;                      //!< Counts warning in "world"
    unsigned _skippedTests;
    unsigned _failedTests;
    bool     _hadAsserts;                    //!< Saves the fact that for the overall test, there was at least one assertion execution (should only be reset for new test)
    unsigned _testAsserts;                   //!< Counts asserts within a single test (is reset for each data driven sample)
    unsigned _testFailedAsserts;             //!< Then number of assertion in failure in current test
    unsigned _suiteFailedTests;
    unsigned _failedSuites;
    unsigned _dataDrivenTests;               //!< The number of tests that forward the test per se to data driven tests
    unsigned _currentDataDrivenTestsSamples; //!< The number of samples used (as input and optionally expected result) by current data driven tests
    unsigned _overallDataDrivenTestsSamples; //!< The number of samples used (as input and optionally expected result) by ALL data driven tests
    bool     _withinDataDrivenTest;          //!< Memorizes that a data driven test sub-test is running

    #if (__cplusplus >= 201103L)
    public:
    int64_t testDuration() const { return _duration_microseconds; }             //!< Returns Nb of microseconds used to run last test
    private:
    std::chrono::time_point<std::chrono::steady_clock> _startTime;            //!< Current test start time
    std::chrono::time_point<std::chrono::steady_clock> _stopTime;             //!< Current test stop time
    int64_t                                            _duration_microseconds; //!< Time used by last run test
    #endif
};

inline TestTracker &tracker() { return TestTracker::tracker(); }
}

#endif // __cxxtest__TestTracker_h__
