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

#ifndef __cxxtest__TeeListener_h__
#define __cxxtest__TeeListener_h__

//
// A TeeListener notifies two "regular" TestListeners
//

#include <cxxtest/TestListener.h>
#include <cxxtest/TestListener.h>
#include "DummyListener.h"

namespace CxxTest
{
struct DifferenceInfo;

class TeeListener : public TestListener
{
public:
    TeeListener()
    {
        setFirst(_dummy);
        setSecond(_dummy);
    }

    virtual ~TeeListener()
    {
    }

    void setFirst(TestListener &first)
    {
        _first = &first;
    }

    void setSecond(TestListener &second)
    {
        _second = &second;
    }

    virtual void enterWorld(const WorldDescription &desc)
    {
        TestListener::enterWorld(desc);
        _first->enterWorld(desc);
        _second->enterWorld(desc);
    }

    virtual void enterSuite(const SuiteDescription &d)
    {
        _first->enterSuite(d);
        _second->enterSuite(d);
    }

    virtual void enterTest(const TestDescription &d)
    {
        _first->enterTest(d);
        _second->enterTest(d);
    }

    virtual void trace(const char* message, const char *file, int line)
    {
        _first->trace(message, file, line);
        _second->trace(message, file, line);
    }

    virtual void warning(const char* message, const char *file, int line)
    {
        _first->warning(message, file, line);
        _second->warning(message, file, line);
    }

    virtual void skippedTest(const char* message, const char *file, int line)
    {
        _first->skippedTest(message, file, line);
        _second->skippedTest(message, file, line);
    }

    virtual void failedTest(const char* message, const char *file, int line)
    {
        _first->failedTest  (message, file, line);
        _second->failedTest (message, file, line);
    }

    virtual void failedTestInternal(const char* message, const char *file, int line)
    {
        _first->failedTestInternal  (message, file, line);
        _second->failedTestInternal (message, file, line);
    }

    virtual void notSupportedAssert(const char* message, const char* file, int line, const char* notSupportedAssertName, const char* requiredCondition)
    {
        _first->notSupportedAssert  (message, file, line, notSupportedAssertName, requiredCondition);
        _second->notSupportedAssert (message, file, line, notSupportedAssertName, requiredCondition);
    }

    virtual void unexpectExceptionFromTest(const char* exceptionType, const char* exceptionValue, const char* file, int line)
    {
        _first->unexpectExceptionFromTest  (exceptionType, exceptionValue, file, line);
        _second->unexpectExceptionFromTest (exceptionType, exceptionValue, file, line);
    }


    virtual void failedAssertTrue(const char* message, const char* file, int line, const char *expression)
    {
        _first->failedAssertTrue  (message, file, line, expression);
        _second->failedAssertTrue (message, file, line, expression);
    }

    virtual void failedAssertFalse(const char* message, const char* file, int line, const char *expression)
    {
        _first->failedAssertFalse  (message, file, line, expression);
        _second->failedAssertFalse (message, file, line, expression);
    }

    virtual void failedAssertNullptr(const char* message, const char* file, int line, const char *expression)
    {
        _first->failedAssertNullptr  (message, file, line, expression);
        _second->failedAssertNullptr (message, file, line, expression);
    }

    virtual void failedAssertNotNullptr(const char* message, const char* file, int line, const char *expression)
    {
        _first->failedAssertNotNullptr  (message, file, line, expression);
        _second->failedAssertNotNullptr (message, file, line, expression);
    }


    virtual void failedAssertEmpty    (const char* message, const char* file, int line, const char* containerExpr, const char* containerContent)
    {
      _first->failedAssertEmpty  (message, file, line, containerExpr, containerContent);
      _second->failedAssertEmpty (message, file, line, containerExpr, containerContent);
    }

    virtual void failedAssertNotEmpty    (const char* message, const char* file, int line, const char* containerExpr)
    {
      _first->failedAssertNotEmpty  (message, file, line, containerExpr);
      _second->failedAssertNotEmpty (message, file, line, containerExpr);
    }


    virtual void failedAssertEquals(const char* message, const char *file, int line,
                            const char *xStr, const char *yStr,
                            const char *x, const char *y)
    {
        _first->failedAssertEquals(message, file, line, xStr, yStr, x, y);
        _second->failedAssertEquals(message, file, line, xStr, yStr, x, y);
    }

    virtual void failedAssertSameData(const char* message, const char *file, int line,
                              const char *xStr, const char *yStr,
                              const char *sizeStr, const void *x,
                              const void *y, unsigned size)
    {
        _first->failedAssertSameData(message, file, line, xStr, yStr, sizeStr, x, y, size);
        _second->failedAssertSameData(message, file, line, xStr, yStr, sizeStr, x, y, size);
    }

    virtual void failedAssertFileContent(const char* message, const char* file, int line, const char* checkedFile, const char* expectedContent, const char* expectedContentExpr, const DifferenceInfo& diffInfo)
    {
      _first->failedAssertFileContent  (message, file, line, checkedFile, expectedContent, expectedContentExpr, diffInfo);
      _second->failedAssertFileContent (message, file, line, checkedFile, expectedContent, expectedContentExpr, diffInfo);
    }

    virtual void failedAssertFileExists(const char* message, const char* file, int line, const char* checkedFile)
    {
        _first->failedAssertFileExists  (message, file, line, checkedFile);
        _second->failedAssertFileExists (message, file, line, checkedFile);
    }

    virtual void failedAssertFileNotExists(const char* message, const char* file, int line, const char* checkedFile)
    {
        _first->failedAssertFileNotExists  (message, file, line, checkedFile);
        _second->failedAssertFileNotExists (message, file, line, checkedFile);
    }


    virtual void failedAssertFileSize(const char* message, const char* file, int line, const char* checkedFile, int expectedSize, int effectiveSize)
    {
        _first->failedAssertFileSize  (message, file, line, checkedFile, expectedSize, effectiveSize);
        _second->failedAssertFileSize (message, file, line, checkedFile, expectedSize, effectiveSize);
    }

    virtual void failedAssertSameFiles(const char* message, const char* file, int line, const char* file1, const char* file2, const char* explanation)
    {
        _first->failedAssertSameFiles(message, file, line, file1, file2, explanation);
        _second->failedAssertSameFiles(message, file, line, file1, file2, explanation);
    }

    virtual void failedAssertDelta(const char* message, const char *file, int line,
                           const char *xStr, const char *yStr, const char *dStr,
                           const char *x, const char *y, const char *d)
    {
        _first->failedAssertDelta(message, file, line, xStr, yStr, dStr, x, y, d);
        _second->failedAssertDelta(message, file, line, xStr, yStr, dStr, x, y, d);
    }

    virtual void failedAssertDiffers(const char* message, const char *file, int line,
                             const char *xStr, const char *yStr,
                             const char *value)
    {
        _first->failedAssertDiffers(message, file, line, xStr, yStr, value);
        _second->failedAssertDiffers(message, file, line, xStr, yStr, value);
    }

    virtual void failedAssertLessThan(const char* message, const char *file, int line,
                              const char *xStr, const char *yStr,
                              const char *x, const char *y)
    {
        _first->failedAssertLessThan(message, file, line, xStr, yStr, x, y);
        _second->failedAssertLessThan(message, file, line, xStr, yStr, x, y);
    }

    virtual void failedAssertLessThanEquals(const char* message, const char *file, int line,
                                    const char *xStr, const char *yStr,
                                    const char *x, const char *y)
    {
        _first->failedAssertLessThanEquals(message, file, line, xStr, yStr, x, y);
        _second->failedAssertLessThanEquals(message, file, line, xStr, yStr, x, y);
    }

    virtual void failedAssertGreaterThan(const char* message, const char *file, int line,
                              const char *xStr, const char *yStr,
                              const char *x, const char *y)
    {
        _first->failedAssertGreaterThan(message, file, line, xStr, yStr, x, y);
        _second->failedAssertGreaterThan(message, file, line, xStr, yStr, x, y);
    }

    virtual void failedAssertGreaterThanEquals(const char* message, const char *file, int line,
                                    const char *xStr, const char *yStr,
                                    const char *x, const char *y)
    {
        _first->failedAssertGreaterThanEquals(message, file, line, xStr, yStr, x, y);
        _second->failedAssertGreaterThanEquals(message, file, line, xStr, yStr, x, y);
    }


    virtual void failedAssertPredicate(const char* message, const char *file, int line,
                               const char *predicate, const char *xStr, const char *x)
    {
        _first->failedAssertPredicate(message, file, line, predicate, xStr, x);
        _second->failedAssertPredicate(message, file, line, predicate, xStr, x);
    }

    virtual void failedAssertRelation(const char* message, const char *file, int line,
                              const char *relation, const char *xStr, const char *yStr,
                              const char *x, const char *y)
    {
        _first->failedAssertRelation(message, file, line, relation, xStr, yStr, x, y);
        _second->failedAssertRelation(message, file, line, relation, xStr, yStr, x, y);
    }

    virtual void failedAssertThrows(const char* message, const char *file, int line, const char *expression, const char* exceptionMessage, const char *type, bool otherThrown)
    {
        _first->failedAssertThrows  (message, file, line, expression, exceptionMessage, type, otherThrown);
        _second->failedAssertThrows (message, file, line, expression, exceptionMessage, type, otherThrown);
    }

    virtual void failedAssertThrowsNothing(const char* message, const char *file, int line, const char *expression, const char* exceptionMessage)
    {
        _first->failedAssertThrowsNothing (message, file, line, expression, exceptionMessage);
        _second->failedAssertThrowsNothing(message, file, line, expression, exceptionMessage);
    }

    virtual void failedAssertThrowsAnything(const char* message, const char *file, int line, const char *expression)
    {
        _first->failedAssertThrowsAnything (message, file, line, expression);
        _second->failedAssertThrowsAnything(message, file, line, expression);
    }




    virtual void succeededAssertTrue              (const char* assertionMessage, const char* file, int  line, const char* expression)
    {
      _first  ->succeededAssertTrue(assertionMessage, file, line, expression);
      _second ->succeededAssertTrue(assertionMessage, file, line, expression);
    }
    virtual void succeededAssertFalse             (const char* assertionMessage, const char* file, int  line, const char* expression)
    {
      _first  ->succeededAssertFalse(assertionMessage, file, line, expression);
      _second ->succeededAssertFalse(assertionMessage, file, line, expression);
    }

    virtual void succeededAssertNotNullptr        (const char* assertionMessage, const char* file, int  line, const char* expression)
    {
      _first  ->succeededAssertNotNullptr(assertionMessage, file, line, expression);
      _second ->succeededAssertNotNullptr(assertionMessage, file, line, expression);
    }

    virtual void succeededAssertNullptr           (const char* assertionMessage, const char* file, int  line, const char* expression)
    {
      _first  ->succeededAssertNullptr(assertionMessage, file, line, expression);
      _second ->succeededAssertNullptr(assertionMessage, file, line, expression);
    }

    virtual void succeededAssertDelta             (const char* assertionMessage, const char* file, int  line, const char* xStr,  const char* yStr, const char* dStr, const char* x, const char* y, const char* d)
    {
      _first  ->succeededAssertDelta(assertionMessage, file, line, xStr, yStr, dStr, x, y, d);
      _second ->succeededAssertDelta(assertionMessage, file, line, xStr, yStr, dStr, x, y, d);
    }

    virtual void succeededAssertDiffers           (const char* assertionMessage, const char* file, int  line, const char* xStr,  const char* yStr, const char* x, const char* y)
    {
      _first  ->succeededAssertDiffers(assertionMessage, file, line, xStr, yStr, x, y);
      _second ->succeededAssertDiffers(assertionMessage, file, line, xStr, yStr, x, y);
    }

    virtual void succeededAssertEmpty (const char* message, const char* file, int line, const char* containerExpr)
    {
      _first->succeededAssertEmpty  (message, file, line, containerExpr);
      _second->succeededAssertEmpty (message, file, line, containerExpr);
    }

    virtual void succeededAssertNotEmpty (const char* message, const char* file, int line, const char* containerExpr, const char* containerContent)
    {
      _first->succeededAssertNotEmpty  (message, file, line, containerExpr, containerContent);
      _second->succeededAssertNotEmpty (message, file, line, containerExpr, containerContent);
    }


    virtual void succeededAssertEquals            (const char* assertionMessage, const char* file, int  line, const char* xStr,  const char* yStr, const char* x, const char* y)
    {
      _first  ->succeededAssertEquals(assertionMessage, file, line, xStr, yStr, x, y);
      _second ->succeededAssertEquals(assertionMessage, file, line, xStr, yStr, x, y);
    }

    virtual void succeededAssertFileContent       (const char* assertionMessage, const char* file, int line, const char* checkedFile, const char* expectedContent, const char* expectedContentExpr, const DifferenceInfo& diffInfo)
    {
      _first  ->succeededAssertFileContent(assertionMessage, file, line, checkedFile, expectedContent, expectedContentExpr, diffInfo);
      _second ->succeededAssertFileContent(assertionMessage, file, line, checkedFile, expectedContent, expectedContentExpr, diffInfo);
    }

    virtual void succeededAssertFileExists        (const char* assertionMessage, const char* file, int line, const char* checkedFile)
    {
      _first  ->succeededAssertFileExists(assertionMessage, file, line, checkedFile);
      _second ->succeededAssertFileExists(assertionMessage, file, line, checkedFile);
    }

    virtual void succeededAssertFileNotExists     (const char* assertionMessage, const char* file, int line, const char* checkedFile)
    {
      _first  ->succeededAssertFileNotExists(assertionMessage, file, line, checkedFile);
      _second ->succeededAssertFileNotExists(assertionMessage, file, line, checkedFile);
    }

    virtual void succeededAssertFileSize          (const char* assertionMessage, const char* file, int line, const char* checkedFile, int expectedSize, int effectiveSize)
    {
      _first  ->succeededAssertFileSize(assertionMessage, file, line, checkedFile, expectedSize, effectiveSize);
      _second ->succeededAssertFileSize(assertionMessage, file, line, checkedFile, expectedSize, effectiveSize);
    }

    virtual void succeededAssertGreaterThan       (const char* assertionMessage, const char* file, int  line, const char* xStr,  const char* yStr, const char* x, const char* y)
    {
      _first  ->succeededAssertGreaterThan(assertionMessage, file, line, xStr, yStr, x, y);
      _second ->succeededAssertGreaterThan(assertionMessage, file, line, xStr, yStr, x, y);
    }

    virtual void succeededAssertGreaterThanEquals (const char* assertionMessage, const char* file, int  line, const char* xStr,  const char* yStr, const char* x, const char* y)
    {
      _first  ->succeededAssertGreaterThanEquals(assertionMessage, file, line, xStr, yStr, x, y);
      _second ->succeededAssertGreaterThanEquals(assertionMessage, file, line, xStr, yStr, x, y);
    }

    virtual void succeededAssertLessThan          (const char* assertionMessage, const char* file, int  line, const char* xStr,  const char* yStr, const char* x, const char* y)
    {
      _first  ->succeededAssertLessThan(assertionMessage, file, line, xStr, yStr, x, y);
      _second ->succeededAssertLessThan(assertionMessage, file, line, xStr, yStr, x, y);
    }

    virtual void succeededAssertLessThanEquals    (const char* assertionMessage, const char* file, int  line, const char* xStr,  const char* yStr, const char* x, const char* y)
    {
      _first  ->succeededAssertLessThanEquals(assertionMessage, file, line, xStr, yStr, x, y);
      _second ->succeededAssertLessThanEquals(assertionMessage, file, line, xStr, yStr, x, y);
    }

    virtual void succeededAssertPredicate         (const char* assertionMessage, const char* file, int  line, const char* pStr,  const char* xStr, const char* x)
    {
      _first  ->succeededAssertPredicate(assertionMessage, file, line, pStr, xStr, x);
      _second ->succeededAssertPredicate(assertionMessage, file, line, pStr, xStr, x);
    }

    virtual void succeededAssertRelation          (const char* assertionMessage, const char* file, int  line, const char* rStr,  const char* xStr, const char* yStr, const char* x, const char* y)
    {
      _first  ->succeededAssertRelation(assertionMessage, file, line, rStr, xStr, yStr, x, y);
      _second ->succeededAssertRelation(assertionMessage, file, line, rStr, xStr, yStr, x, y);
    }

    virtual void succeededAssertSameData          (const char* assertionMessage, const char* file, int  line, const char* xStr,  const char* yStr, const char* sizeStr, const void * x, const void * y, unsigned size)
    {
      _first  ->succeededAssertSameData(assertionMessage, file, line, xStr, yStr, sizeStr, x, y, size);
      _second ->succeededAssertSameData(assertionMessage, file, line, xStr, yStr, sizeStr, x, y, size);
    }

    virtual void succeededAssertSameFiles         (const char* assertionMessage, const char* file, int  line, const char* file1, const char* file2)
    {
      _first  ->succeededAssertSameFiles(assertionMessage, file, line, file1, file2);
      _second ->succeededAssertSameFiles(assertionMessage, file, line, file1, file2);
    }

    virtual void succeededAssertThrows            (const char* assertionMessage, const char* file, int  line, const char* expr,  const char* type, const char* exceptionMessage)
    {
      _first  ->succeededAssertThrows(assertionMessage, file, line, expr, type, exceptionMessage);
      _second ->succeededAssertThrows(assertionMessage, file, line, expr, type, exceptionMessage);
    }

    virtual void succeededAssertThrowsNothing     (const char* assertionMessage, const char* file, int  line, const char* expr)
    {
      _first  ->succeededAssertThrowsNothing(assertionMessage, file, line, expr);
      _second ->succeededAssertThrowsNothing(assertionMessage, file, line, expr);
    }

    virtual void succeededAssertThrowsAnything    (const char* assertionMessage, const char* file, int  line, const char* expr)
    {
      _first  ->succeededAssertThrowsAnything(assertionMessage, file, line, expr);
      _second ->succeededAssertThrowsAnything(assertionMessage, file, line, expr);
    }



    virtual void leaveTest(const TestDescription &d)
    {
        _first->leaveTest  (d);
        _second->leaveTest (d);
    }

    virtual void leaveSuite(const SuiteDescription &d)
    {
        _first->leaveSuite(d);
        _second->leaveSuite(d);
    }

    virtual void leaveWorld(const WorldDescription &d)
    {
        _first->leaveWorld(d);
        _second->leaveWorld(d);
    }

private:
    TestListener *_first, *_second;
    DummyListener _dummy;
};
}


#endif // __cxxtest__TeeListener_h__
