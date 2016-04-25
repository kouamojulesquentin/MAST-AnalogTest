//-------------------------------------------------------------------------
// CxxTest: A lightweight C++ unit testing library.
// Copyright (c) 2008 Sandia Corporation.
// This software is distributed under the LGPL License v3
// For more information, see the COPYING file in the top CxxTest directory.
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//-------------------------------------------------------------------------

#ifndef __cxxtest__TestTracker_cpp__
#define __cxxtest__TestTracker_cpp__

#include <cxxtest/TestTracker.h>

namespace CxxTest
{
bool TestTracker::_created      = false;
bool TestTracker::print_tracing = true;         // We want tracing by default

TestTracker::TestTracker()
{
    if (!_created)
    {
        initialize();
        setListener(0);
        _created = true;
    }
}

TestTracker::~TestTracker()
{
}

TestTracker & TestTracker::tracker()
{
    static TestTracker theTracker;
    return theTracker;
}

void TestTracker::initialize()
{
    _warnings                      = 0;
    _skippedTests                  = 0;
    _testSkipped                   = false;
    _testHasNoAssertion            = false;
    _failedTests                   = 0;
    _testAsserts                   = 0;
    _testFailedAsserts             = 0;
    _suiteFailedTests              = 0;
    _failedSuites                  = 0;
    _world                         = 0;
    _suite                         = 0;
    _test                          = 0;
    _lastAssertFile                = NULL;
    _lastAssertLine                = 0;
    _dataDrivenTests               = 0;
    _currentDataDrivenTestsSamples = 0;
    _overallDataDrivenTestsSamples = 0;
    _withinDataDrivenTest          = false;
}

//! Returns input parameter if not NULL, address of "dummy" test otherwise
//!
//! @note In both cases, a pointer on non const is returned to be able to update data driven tests related fields (when applies)
//!
TestDescription* TestTracker::fixTest(TestDescription *td) const
{
    return td ? td : const_cast<TestDescription*>(&dummyTest());  // Dummy test never change (even when call with non const methods)
}

const SuiteDescription *TestTracker::fixSuite(const SuiteDescription *d) const
{
    return d ? d : &dummySuite();
}

const WorldDescription *TestTracker::fixWorld(const WorldDescription *d) const
{
    return d ? d : &dummyWorld();
}

const TestDescription& TestTracker::dummyTest() const
{
    return dummySuite().testDescription(0);
}

const SuiteDescription &TestTracker::dummySuite() const
{
    return dummyWorld().suiteDescription(0);
}

const WorldDescription &TestTracker::dummyWorld() const
{
    return _dummyWorld;
}

void TestTracker::setListener(TestListener *testListener)
{
    m_pListener = testListener ? testListener : &_dummyListener;
}

void TestTracker::enterWorld(const WorldDescription &wd)
{
    setWorld(&wd);
    _warnings          = 0;
    _skippedTests      = 0;
    _failedTests       = 0;
    _testAsserts       = 0;
    _testFailedAsserts = 0;
    _suiteFailedTests  = 0;
    _failedSuites      = 0;
    _testSkipped       = false;
    m_pListener->enterWorld(wd);
}

void TestTracker::enterSuite(const SuiteDescription &sd)
{
  setSuite(&sd);
  _testFailedAsserts       = 0;
  _suiteFailedTests        = 0;
  _testSkipped             = false;
  _withinDataDrivenTest   = false;
  m_pListener->enterSuite(sd);
}

//! Manages start of a single test within a data driven "master" test
//!
void TestTracker::enterDataDrivenTest(int testId, const char* inputSample)
{
  _testAsserts          = 0;
  _testFailedAsserts    = 0;
  _testSkipped          = false;
  _withinDataDrivenTest = true;

  _test->isDataDriven(true);
  _test->currentDataDrivenTestId(testId);
  _test->inputSampleAsText(inputSample);
  m_pListener->enterDataDrivenTest(*_test);
}

void TestTracker::enterTest(TestDescription& td)
{
  setTest(&td);

  _hadAsserts           = false;
  _testAsserts          = 0;
  _testFailedAsserts    = 0;
  _testSkipped          = false;
  _lastAssertFile       = NULL;
  _lastAssertLine       = 0;
  _withinDataDrivenTest = false;
  m_pListener->enterTest(td);
}


//! Returns true if currently processing last "sub" test of a series of data driven tests
//!
bool TestTracker::isLastDataDrivenTest (const TestDescription &td) const
{
  if (_currentDataDrivenTestsSamples == 0)
  {
    return false;
  }
  return (td.currentDataDrivenTestId() + 1) >= static_cast<int>(_currentDataDrivenTestsSamples);
}
//
//  End of fct_TestTracker::isLastDataDrivenTest
//---------------------------------------------------------------------------



//! Updates member _testHasNoAssertion (normally at end of test)
//!
void TestTracker::updateTestHasNoAssertion ()
{
  _testHasNoAssertion =     (_testAsserts == 0)
                        &&  !testFailed()
                        &&  !testSkipped();
}
//
//  End of fct_TestTracker::updateTestHasNoAssertion
//---------------------------------------------------------------------------


//! Manages end of a test
//!
void TestTracker::leaveTest(const TestDescription &td)
{
    updateTestHasNoAssertion();
    m_pListener->leaveTest(td);

    _lastAssertFile       = NULL;
    _lastAssertLine       = 0;
    _withinDataDrivenTest = false;
    setTest(0);
}

//! Manages end of a single test within a data driven "master" test
//!
void TestTracker::leaveDataDrivenTest()
{
  updateTestHasNoAssertion();
  m_pListener->leaveDataDrivenTest(test());
}

//! Manages end of a all tests within a data driven "master" test
//!
void TestTracker::leaveDataDrivenTests()
{
  _test->isDataDriven(false);
}


void TestTracker::leaveSuite(const SuiteDescription &sd)
{
    m_pListener->leaveSuite(sd);
    setSuite(0);
}

void TestTracker::leaveWorld(const WorldDescription &wd)
{
    m_pListener->leaveWorld(wd);
    setWorld(0);
}

void TestTracker::trace(const char *message, const char *file, int line)
{
    m_pListener->trace(message, file, line);
}

void TestTracker::warning(const char *message, const char *file, int line)
{
    countWarning();
    m_pListener->warning(message, file, line);
}

void TestTracker::skippedTest(const char *message, const char *file, int line)
{
    countSkipped();
    _testSkipped = true;
    m_pListener->skippedTest(message, file, line);
}

void TestTracker::failedTest(const char *message, const char *file, int line)
{
    countFailure();
    m_pListener->failedTest(message, file, line);
}

void TestTracker::failedTestInternal(const char *message, const char *file, int line)
{
    countFailure();
    m_pListener->failedTestInternal(message, file, line);
}

void TestTracker::notSupportedAssert(const char* message, const char* file, int line, const char* notSupportedAssertName, const char* requiredCondition)
{
    countFailure();
    m_pListener->notSupportedAssert(message, file, line, notSupportedAssertName, requiredCondition);
    _lastAssertFile = file;
    _lastAssertLine = line;
}


void TestTracker::unexpectExceptionFromTest(const char* exceptionType, const char* exceptionValue, const char* file, int line)
{
    countFailure();
    m_pListener->unexpectExceptionFromTest(exceptionType, exceptionValue, file, line);
    _lastAssertFile = file;   // Save file/line if another exception occurs after this managed one
    _lastAssertLine = line;
}

void TestTracker::failedAssertTrue(const char *message, const char *file, int line, const char *expression)
{
    countFailure();
    m_pListener->failedAssertTrue(message, file, line, expression);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::failedAssertFalse(const char *message, const char *file, int line, const char *expression)
{
    countFailure();
    m_pListener->failedAssertFalse(message, file, line, expression);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::failedAssertNullptr(const char *message, const char *file, int line, const char *expression)
{
    countFailure();
    m_pListener->failedAssertNullptr(message, file, line, expression);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::failedAssertNotNullptr(const char *message, const char *file, int line, const char *expression)
{
    countFailure();
    m_pListener->failedAssertNotNullptr(message, file, line, expression);
    _lastAssertFile = file;
    _lastAssertLine = line;
}


void TestTracker::failedAssertEquals(const char *message, const char *file, int line, const char *xStr, const char *yStr, const char *x, const char *y)
{
    countFailure();
    m_pListener->failedAssertEquals(message, file, line, xStr, yStr, x, y);
    _lastAssertFile = file;
    _lastAssertLine = line;
}


void TestTracker::failedAssertSameData(const char *message, const char *file, int line, const char *xStr, const char *yStr, const char *sizeStr, const void *x, const void *y, unsigned size)
{
    countFailure();
    m_pListener->failedAssertSameData(message, file, line, xStr, yStr, sizeStr, x, y, size);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::failedAssertDelta(const char *message, const char *file, int line, const char *xStr, const char *yStr, const char *dStr, const char *x, const char *y, const char *d)
{
    countFailure();
    m_pListener->failedAssertDelta(message, file, line, xStr, yStr, dStr, x, y, d);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::failedAssertDiffers(const char *message, const char *file, int line, const char *xStr, const char *yStr, const char *value)
{
    countFailure();
    m_pListener->failedAssertDiffers(message, file, line, xStr, yStr, value);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::failedAssertFileContent(const char* message, const char* file, int line, const char* checkedFile, const char* expectedContent, const char* expectedContentExpr, const DifferenceInfo& diffInfo)
{
    countFailure();
    m_pListener->failedAssertFileContent(message, file, line, checkedFile, expectedContent, expectedContentExpr, diffInfo);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::failedAssertFileExists(const char* message, const char* file, int line, const char* checkedFile)
{
    countFailure();
    m_pListener->failedAssertFileExists(message, file, line, checkedFile);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::failedAssertFileNotExists(const char* message, const char* file, int line, const char* checkedFile)
{
    countFailure();
    m_pListener->failedAssertFileNotExists(message, file, line, checkedFile);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::failedAssertFileSize(const char* message, const char* file, int line, const char* checkedFile, int expectedSize, int effectiveSize)
{
    countFailure();
    m_pListener->failedAssertFileSize(message, file, line, checkedFile, expectedSize, effectiveSize);
    _lastAssertFile = file;
    _lastAssertLine = line;
}


void TestTracker::failedAssertGreaterThan(const char *message, const char *file, int line, const char *xStr, const char *yStr, const char *x, const char *y)
{
    countFailure();
    m_pListener->failedAssertGreaterThan(message, file, line, xStr, yStr, x, y);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::failedAssertGreaterThanEquals(const char *message, const char *file, int line, const char *xStr, const char *yStr, const char *x, const char *y)
{
    countFailure();
    m_pListener->failedAssertGreaterThanEquals(message, file, line, xStr, yStr, x, y);
    _lastAssertFile = file;
    _lastAssertLine = line;
}


void TestTracker::failedAssertLessThan(const char *message, const char *file, int line, const char *xStr, const char *yStr, const char *x, const char *y)
{
    countFailure();
    m_pListener->failedAssertLessThan(message, file, line, xStr, yStr, x, y);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::failedAssertLessThanEquals(const char *message, const char *file, int line, const char *xStr, const char *yStr, const char *x, const char *y)
{
    countFailure();
    m_pListener->failedAssertLessThanEquals(message, file, line, xStr, yStr, x, y);
    _lastAssertFile = file;
    _lastAssertLine = line;
}


void TestTracker::failedAssertPredicate(const char *message, const char *file, int line, const char *predicate, const char *xStr, const char *x)
{
    countFailure();
    m_pListener->failedAssertPredicate(message, file, line, predicate, xStr, x);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::failedAssertRelation(const char *message, const char *file, int line, const char *relation, const char *xStr, const char *yStr, const char *x, const char *y)
{
    countFailure();
    m_pListener->failedAssertRelation(message, file, line, relation, xStr, yStr, x, y);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::failedAssertThrows(const char *message, const char *file, int line, const char *expression, const char *type, bool otherThrown)
{
    countFailure();
    m_pListener->failedAssertThrows(message, file, line, expression, type, otherThrown);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::failedAssertThrowsNothing(const char *message, const char *file, int line, const char *expression, const char* exceptionMessage)
{
    countFailure();
    m_pListener->failedAssertThrowsNothing(message, file, line, expression, exceptionMessage);
    _lastAssertFile = file;
    _lastAssertLine = line;
}


void TestTracker::failedAssertThrowsAnything(const char *message, const char *file, int line, const char *expression)
{
    countFailure();
    m_pListener->failedAssertThrowsAnything(message, file, line, expression);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::failedAssertSameFiles(const char *message, const char *file, int line, const char *file1, const char* file2, const char* explanation)
{
    countFailure();
    m_pListener->failedAssertSameFiles(message, file, line, file1, file2, explanation);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::setWorld(const WorldDescription *w)
{
    _world = fixWorld(w);
    setSuite(0);
}

void TestTracker::setSuite(const SuiteDescription *s)
{
    _suite = fixSuite(s);
    setTest(0);
}

void TestTracker::setTest(TestDescription *t)
{
    _test = fixTest(t);
}

//! Reports a successful "assert true"
//!
void TestTracker::succeededAssertTrue(const char *message, const char *file, int line, const char *expression)
{
    m_pListener->succeededAssertTrue(message, file, line, expression);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

//! Reports a successful "assert false"
//!
void TestTracker::succeededAssertFalse(const char *message, const char *file, int line, const char *expression)
{
    m_pListener->succeededAssertFalse(message, file, line, expression);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

//! Reports a successful "assert nullptr"
//!
void TestTracker::succeededAssertNullptr(const char *message, const char *file, int line, const char *expression)
{
    m_pListener->succeededAssertNullptr(message, file, line, expression);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

//! Reports a successful "assert not nullptr"
//!
void TestTracker::succeededAssertNotNullptr(const char *message, const char *file, int line, const char *expression)
{
    m_pListener->succeededAssertNotNullptr(message, file, line, expression);
    _lastAssertFile = file;
    _lastAssertLine = line;
}



//! Reports a successful "assert delta"
//!
void TestTracker::succeededAssertDelta(const char *message, const char *file, int line, const char *xStr, const char *yStr, const char *dStr, const char *x, const char *y, const char *d)
{
    m_pListener->succeededAssertDelta(message, file, line, xStr, yStr, dStr, x, y, d);
    _lastAssertFile = file;
    _lastAssertLine = line;
}


//! Reports a successful "assert differ"
//!
void TestTracker::succeededAssertDiffers(const char *message, const char *file, int line, const char *xStr, const char *yStr, const char *x, const char *y)
{
    m_pListener->succeededAssertDiffers(message, file, line, xStr, yStr, x, y);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

//! Reports a successful "assert equal"
//!
void TestTracker::succeededAssertEquals(const char *message, const char *file, int line, const char *xStr, const char *yStr, const char *x, const char *y)
{
    m_pListener->succeededAssertEquals(message, file, line, xStr, yStr, x, y);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::succeededAssertFileContent(const char* message, const char* file, int line, const char* checkedFile, const char* expectedContent, const char* expectedContentExpr, const DifferenceInfo& diffInfo)
{
    m_pListener->succeededAssertFileContent(message, file, line, checkedFile, expectedContent, expectedContentExpr, diffInfo);
    _lastAssertFile = file;
    _lastAssertLine = line;
}


void TestTracker::succeededAssertFileExists(const char* message, const char* file, int line, const char* checkedFile)
{
    m_pListener->succeededAssertFileExists(message, file, line, checkedFile);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::succeededAssertFileNotExists(const char* message, const char* file, int line, const char* checkedFile)
{
    m_pListener->succeededAssertFileNotExists(message, file, line, checkedFile);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

void TestTracker::succeededAssertFileSize(const char* message, const char* file, int line, const char* checkedFile, int expectedSize, int effectiveSize)
{
    m_pListener->succeededAssertFileSize(message, file, line, checkedFile, expectedSize, effectiveSize);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

//! Reports a successful "assert less than"
//!
void TestTracker::succeededAssertLessThan(const char *message, const char *file, int line, const char *xStr, const char *yStr, const char *x, const char *y)
{
    m_pListener->succeededAssertLessThan(message, file, line, xStr, yStr, x, y);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

//! Reports a successful "assert less than or equal"
//!
void TestTracker::succeededAssertLessThanEquals(const char *message, const char *file, int line, const char *xStr, const char *yStr, const char *x, const char *y)
{
    m_pListener->succeededAssertLessThanEquals(message, file, line, xStr, yStr, x, y);
    _lastAssertFile = file;
    _lastAssertLine = line;
}


//! Reports a successful "assert greater than"
//!
void TestTracker::succeededAssertGreaterThan(const char *message, const char *file, int line, const char *xStr, const char *yStr, const char *x, const char *y)
{
    m_pListener->succeededAssertGreaterThan(message, file, line, xStr, yStr, x, y);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

//! Reports a successful "assert greater than or equal"
//!
void TestTracker::succeededAssertGreaterThanEquals(const char *message, const char *file, int line, const char *xStr, const char *yStr, const char *x, const char *y)
{
    m_pListener->succeededAssertGreaterThanEquals(message, file, line, xStr, yStr, x, y);
    _lastAssertFile = file;
    _lastAssertLine = line;
}



//! Reports a successful "assert predicate"
//!
void TestTracker::succeededAssertPredicate(const char *message, const char *file, int line, const char *pStr, const char *xStr, const char *x)
{
    m_pListener->succeededAssertPredicate(message, file, line, pStr, xStr, x);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

//! Reports a successful "assert relation"
//!
void TestTracker::succeededAssertRelation(const char *message, const char *file, int line, const char *rStr, const char *xStr, const char *yStr, const char *x, const char *y)
{
    m_pListener->succeededAssertRelation(message, file, line, rStr, xStr, yStr, x, y);
    _lastAssertFile = file;
    _lastAssertLine = line;
}


//! Reports a successful "assert relation"
//!
void TestTracker::succeededAssertSameData(const char *message, const char *file, int line, const char *xStr, const char *yStr, const char *sizeStr, const void *x, const void *y, unsigned size)
{
    m_pListener->succeededAssertSameData(message, file, line, xStr, yStr, sizeStr, x, y, size);
    _lastAssertFile = file;
    _lastAssertLine = line;
}


//! Reports a successful "assert same files"
//!
void TestTracker::succeededAssertSameFiles(const char *message, const char *file, int line, const char *file1, const char* file2)
{
    m_pListener->succeededAssertSameFiles(message, file, line, file1, file2);
    _lastAssertFile = file;
    _lastAssertLine = line;
}


//! Reports a successful "assert throws"
//!
void TestTracker::succeededAssertThrows(const char* message, const char* file, int line, const char* expr, const char* type, const char* exceptionMessage)
{
    m_pListener->succeededAssertThrows(message, file, line, expr, type, exceptionMessage);
    _lastAssertFile = file;
    _lastAssertLine = line;
}

//! Reports a successful "assert throws"
//!
void TestTracker::succeededAssertThrowsNothing(const char *message, const char *file, int line, const char *expr)
{
    m_pListener->succeededAssertThrowsNothing(message, file, line, expr);
    _lastAssertFile = file;
    _lastAssertLine = line;
}


//! Reports a successful "assert throws anything"
//!
void TestTracker::succeededAssertThrowsAnything(const char *message, const char *file, int line, const char *expr)
{
    m_pListener->succeededAssertThrowsAnything(message, file, line, expr);
    _lastAssertFile = file;
    _lastAssertLine = line;
}



//! Counts the number of asserts within a test
//!
//! @note When called we are in place where exception are managed (catched) so we can forget last assertions file and line
void TestTracker::countAsserts()
{
    ++_testAsserts;
    _hadAsserts     = true;
    _lastAssertFile = NULL;
    _lastAssertLine = 0;
}


//! Tallies the number of tests that are in fact defining (dynamically)
//! data driven tests along with samples used to run data driven tests
//!
//! @param samples  The number of samples for that data driven test
//!
void TestTracker::countDataDrivenTests(unsigned samples)
{
    ++_dataDrivenTests;
    _currentDataDrivenTestsSamples  = samples;
    _overallDataDrivenTestsSamples += samples;
    _withinDataDrivenTest = true;
    _testFailedAsserts    = 0;
}



void TestTracker::countWarning()
{
    ++_warnings;
}

void TestTracker::countSkipped()
{
    ++_skippedTests;
}

void TestTracker::countFailure()
{
    ++_testFailedAsserts;

    if (_testFailedAsserts == 1)
    {
        ++_failedTests;
        if (++_suiteFailedTests == 1)
        {
            ++_failedSuites;
        }
    }
}
}

#endif // __cxxtest__TestTracker_cpp__

