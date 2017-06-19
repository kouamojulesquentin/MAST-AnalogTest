//-------------------------------------------------------------------------
// CxxTest: A lightweight C++ unit testing library.
// Copyright (c) 2008 Sandia Corporation.
// This software is distributed under the LGPL License v3
// For more information, see the COPYING file in the top CxxTest directory.
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//-------------------------------------------------------------------------

#ifndef __cxxtest__ErrorFormatter_h__
#define __cxxtest__ErrorFormatter_h__

//
// The ErrorFormatter is a TestListener that
// prints reports of the errors to an output
// stream.  Since we cannot rely on the standard
// iostreams, this header defines a base class
// analog to std::ostream.
//

#include <cxxtest/TestRunner.h>
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/ValueTraits.h>
#include <cxxtest/DifferenceInfo.h>
#include <cstdio>

#if (__cplusplus >= 201103L)  // To print properly test duration
#include <sstream>
#include <iomanip>
#endif

namespace CxxTest
{
class OutputStream
{
public:
    virtual ~OutputStream() {}
    virtual void flush() {};
    virtual OutputStream &operator<<(unsigned /*number*/) { return *this; }
    virtual OutputStream &operator<<(const char*  /*string*/) { return *this; }

    typedef void (*Manipulator)(OutputStream &);

    virtual OutputStream &operator<<(Manipulator m) { m(*this); return *this; }
    static void endl(OutputStream &o) { (o << "\n").flush(); }
};

class ErrorFormatter : public TestListener
{
public:
    ErrorFormatter(OutputStream *o,
                   const char   *preLine       = ":",
                   const char   *postLine      = "",
                   const char   *errorString   = "Error",
                   const char   *warningString = "Warning",
                   const char   *successString = "Success")
    : _lastAssertsCount         (0)
    , _dotting                  (true)
    , _reported                 (false)
    , _previousDataDrivenTestId (-1)
    , _o                        (o)
    , _preLine                  (preLine)
    , _postLine                 (postLine)
    , _preExpr                  ("(")
    , _postExpr                 (")")
    , _preStr                   ("\"")
    , _postStr                  ("\"")
    , _differingString          ("Differing from:")
    , _errorString              (errorString)
    , _warningString            (warningString)
    , _successString            (successString)
    {
    }

    int run()
    {
        TestRunner::runAllTests(*this);
        return tracker().failedTests();
    }


    static void streamTestStr(OutputStream& os, unsigned total)
    {
        bool severalTest = (total > 1);
        os << (severalTest ? " tests" : " test");
    }

    static void streamTestCount(OutputStream& os, unsigned total)
    {
        os << total;
        streamTestStr(os, total);
    }

    static void totalTests(OutputStream& os)
    {
        const WorldDescription& wd = tracker().world();
        streamTestCount(os, wd.numTotalTests());
    }



    static void streamDataDrivenTests(OutputStream& os)
    {
      unsigned dataDrivenTests        = tracker().dataDrivenTests();
      unsigned dataDrivenTestsSamples = tracker().dataDrivenTestsSamples();

      if (dataDrivenTests != 0)
      {
        os << " including " << dataDrivenTestsSamples << " 'sub'";
        streamTestStr(os, dataDrivenTestsSamples);
        os << " from " << dataDrivenTests << " data driven";
        streamTestStr(os, dataDrivenTests);
      }
    }

    virtual void enterWorld(const WorldDescription& desc)
    {
        TestListener::enterWorld(desc);
        (*_o) << "Running " << desc.worldName() << " tests (" << totalTests << ")" << endl;
        _o->flush();

        _dotting                  = false;
        _reported                 = false;
        _previousDataDrivenTestId = -1;
    }

    virtual void enterSuite(const SuiteDescription &)
    {
      _reported                 = false;
      _previousDataDrivenTestId = -1;
    }

    virtual void enterTest(const TestDescription &)
    {
      _lastAssertsCount         = 0;
      _reported                 = false;
      _previousDataDrivenTestId = -1; // Valid test ids start from zero
    }


    virtual void enterDataDrivenTest(const TestDescription &)
    {
      _lastAssertsCount = 0;
    }

    virtual void leaveTest(const TestDescription& testDescription)
    {
        TestTracker& testTracker = tracker();

        if (testTracker.testSkipped())
        {
            if (!_dotting)
            {
              (*_o) << endl;
            }
            (*_o) << "s";
            _o->flush();
            fflush(stdout);
            _dotting = true;
        }
        else if (testTracker.testHasNoAssertion())
        {
            if (testTracker.test().isDataDriven())
            {
                warning("No assertion found for this data driven test sample !!!", NULL, 0);
            }
            else if (!testTracker.masterTestHadAssertion())
            {
                warning("No assertion found in this test !!!", testDescription.file(), testDescription.line());
            }
        }
        else if (!testTracker.testFailed())
        {
            if (!_dotting)
            {
              (*_o) << endl;
            }

            if (testTracker.testAsserts() != _lastAssertsCount)
            {
              (*_o) << ".";
            }
            _o->flush();
            fflush(stdout);
            _dotting = true;
        }

        #if (__cplusplus >= 201103L)
        if (testTracker.display_test_duration && !testTracker.testHasNoAssertion())
        {

          constexpr auto millisecondsPerMicrosecond = 1000LL;
          constexpr auto secondsPerMicrosecond      = 1000000LL;
          constexpr auto secondsPerMinute           = 60LL;

          auto microseconds          = testTracker.testDuration();
          auto seconds               = microseconds          / secondsPerMicrosecond;
          auto remainingMicroseconds = microseconds          % secondsPerMicrosecond;
          auto milliseconds          = remainingMicroseconds / millisecondsPerMicrosecond;

          microseconds = remainingMicroseconds % millisecondsPerMicrosecond;

          const auto& test = testTracker.test();
          std::ostringstream os;
          os << "Duration for " << test.suiteName() << "::" << test.testName() << ": ";
          os << std::setfill('0');
          os << "[" << std::setw(2) << seconds % secondsPerMinute;
          os << ":" << std::setw(3) << milliseconds;
          os << "." << std::setw(3) << microseconds;
          os << " us]";
          (*_o) << os.str().c_str() << endl;
        }
        #endif
        _lastAssertsCount = testTracker.testAsserts();
    }

    //! Logs feedback of data driven test execution (if not failed)
    //!
    virtual void leaveDataDrivenTest(const TestDescription& testDescription)
    {
      leaveTest(testDescription);
    }


    virtual void leaveWorld(const WorldDescription &desc)
    {
        unsigned numTotal = desc.numTotalTests() - tracker().skippedTests();
        OutputStream& os = *_o;

        os << endl << endl;
        _dotting = false;

        unsigned effectiveTotalTests = desc.numTotalTests() + tracker().dataDrivenTestsSamples() - tracker().dataDrivenTests();

        if (!tracker().failedTests())
        {
            os << "OK! (" << effectiveTotalTests;
            streamTestStr(os, numTotal);
            os << streamDataDrivenTests << ")"<< endl;
        }
        else
        {

          os << "Failed " << tracker().failedTests() << " and Skipped " << tracker().skippedTests() << " of ";
          streamTestCount(os, effectiveTotalTests);
          os << streamDataDrivenTests << endl;

          if (numTotal == 0)
          {
              os << "Success rate: 100%" << endl;
          }
          else
          {
              unsigned runnableTests = effectiveTotalTests - tracker().skippedTests();
              unsigned numPassed     = runnableTests - tracker().failedTests() ;

              os << "Success rate: " << (unsigned)(numPassed * 100.0 / runnableTests) << "%" << endl;
          }
        }

    }

    virtual void trace(const char* message, const char* file, int line)
    {
        doMessage("Trace", message, file, line);
    }

    virtual void warning(const char* message, const char* file, int line)
    {
        doMessage(_warningString, message, file, line);
    }

    virtual void skippedTest(const char* message, const char* file, int line)
    {
        doMessage("Warning: Test skipped", message, file, line);
    }

    virtual void failedTest(const char* message, const char* file, int line)
    {
        doMessage("Error", message, file, line);
    }

    virtual void failedTestInternal(const char* message, const char* file, int line)
    {
        doMessage("Error", message, file, line);
    }

    virtual void notSupportedAssert(const char* message, const char* file, int line, const char* notSupportedAssertName, const char* requiredCondition)
    {
      OutputStream& os = stop(message, file, line);

      os << notSupportedAssertName << " assertion ";
      if (isNullOrEmpty(requiredCondition))
      {
        os << "is not supported yet";
      }
      else
      {
        os << "is only supported when " << requiredCondition;
      }
    }


    virtual void unexpectExceptionFromTest(const char* exceptionType, const char* exceptionValue, const char* file, int line)
    {
        StringMerger merger("Unexpected ");
        if (isNullOrEmpty(exceptionType))
        {
          merger.append("(unknown type)");
        }
        else
        {
          merger.append("'");
          merger.append(exceptionType);
          merger.append("'");
        }
        merger.append(" exception thrown from test");

        if (!isNullOrEmpty(exceptionValue))
        {
          merger.append(" with value: '");
          merger.append(exceptionValue);
          merger.append("'");
        }

        // ---------------- Try to better locate unexpected exception
        //
        const TestTracker& tracker = TestTracker::tracker();
        bool  gotExceptionAfterAssert = tracker.lastAssertFile() && (tracker.lastAssertLine() != 0);
        if (gotExceptionAfterAssert)
        {
          file = tracker.lastAssertFile();
          line = tracker.lastAssertLine();
          merger.append(" (After assertion at specified line)");
        }

        doMessage("Error", merger.get(), file, line);
    }


    virtual void failedAssertTrue(const char* message, const char* file, int line, const char* expression)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << _preExpr << safeExpression(expression) << _postExpr << " to be true";
        resultMessage  << "it is false";

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    virtual void failedAssertFalse(const char* message, const char* file, int line, const char* expression)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << _preExpr << safeExpression(expression) << _postExpr << " to be false";
        resultMessage  << "it is true";

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    virtual void failedAssertNullptr(const char* message, const char* file, int line, const char* expression)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << _preExpr << expression << _postExpr << " to be nullptr";
        resultMessage  << "it is not nullptr";

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    virtual void failedAssertNotNullptr(const char* message, const char* file, int line, const char* expression)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << _preExpr << expression << _postExpr << " to be not nullptr";
        resultMessage  << "it is nullptr";

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    //! Reports a failed "assert contains"
    //!
    virtual void failedAssertContains(const char* message, const char* file, int line, const char* containerExpr, const char* valueExpr, const char* containerContent, const char* value)
    {
      std::ostringstream checkMessage;
      std::ostringstream resultMessage;

      checkMessage  << _preExpr << containerExpr   << _postExpr << " to contain " << _preExpr << valueExpr << _postExpr;
      resultMessage <<             containerContent             << " does not contain "       << value;

      reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }


    //! Reports a failed "assert not contains"
    //!
    virtual void failedAssertNotContains(const char* message, const char* file, int line, const char* containerExpr, const char* valueExpr, const char* containerContent, const char* value)
    {
      std::ostringstream checkMessage;
      std::ostringstream resultMessage;

      checkMessage  << _preExpr << containerExpr   << _postExpr << " to not contain " << _preExpr << valueExpr << _postExpr;
      resultMessage <<             containerContent             << " contains "                   << value;

      reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    //! Reports a failed "assert empty"
    //!
    virtual void failedAssertEmpty(const char* message, const char* file, int line, const char* containerExpr, const char* containerContent)
    {
      std::ostringstream checkMessage;
      std::ostringstream resultMessage;

      checkMessage  << _preExpr << containerExpr   << _postExpr << " to be empty";
      resultMessage << "it contains: " << containerContent;

      reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    //! Reports a failed "assert not empty"
    //!
    virtual void failedAssertNotEmpty(const char* message, const char* file, int line, const char* containerExpr)
    {
      std::ostringstream checkMessage;
      std::ostringstream resultMessage;

      checkMessage  << _preExpr << containerExpr   << _postExpr << " to be not empty";
      resultMessage << "it is empty";

      reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }



    virtual void failedAssertEquals(const char* message, const char* file, int line, const char* xStr, const char* yStr, const char* x, const char* y)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage  << _preExpr << xStr << " == " << yStr << _postExpr;
        resultMessage << _preExpr << x    << " != " << y    << _postExpr;

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    virtual void failedAssertFileContent(const char* message, const char* file, int line, const char* checkedFile, const char* /*expectedContent*/, const char* expectedContentExpr, const DifferenceInfo& diffInfo)
    {
      std::ostringstream checkMessage;
      std::ostringstream resultMessage;

      checkMessage  << _preStr << safeExpression(checkedFile) << _postStr << " file content to be equal to " << _preExpr << safeExpression(expectedContentExpr) << _postExpr;
      switch (diffInfo.reason)
      {
        case DifferenceInfo::LEFT_NOT_EXIST:
          resultMessage << "Left file does not exist (or cannot be opened)";
          break;
        case DifferenceInfo::RIGHT_NOT_EXIST:
          resultMessage << "Right file does not exist (or cannot be opened)";
          break;
        case DifferenceInfo::LEFT_SHORTER:
          resultMessage << "file is shorter: (" << DifferenceInfo::lineStr(diffInfo.leftLine) << ") than expected content (" << DifferenceInfo::lineStr(diffInfo.rightLine) << ")";
          break;
        case DifferenceInfo::LEFT_LARGER:
          resultMessage << "file is larger: (" << DifferenceInfo::lineStr(diffInfo.leftLine) << ") than expected content (" << DifferenceInfo::lineStr(diffInfo.rightLine) << ")" ;
          break;
        case DifferenceInfo::DIFFER_AT_LINE:
          resultMessage << "file differ at line " << diffInfo.leftLine;
          resultMessage << std::endl << "Expected content at line " << diffInfo.leftLine << ": " << _preStr << diffInfo.rightDifferingLine << _postStr;
          resultMessage << std::endl << "File content at line     " << diffInfo.leftLine << ": " << _preStr << diffInfo.leftDifferingLine  << _postStr;
          break;
        case DifferenceInfo::ARE_EQUAL:
          resultMessage << "Houps, internal error, failedAssertFileContent should not be called when reason = DifferenceInfo::ARE_EQUAL";
          break;
        default:
          resultMessage << "Houps, internal error, reason is not valid";
          break;
      }

      reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    virtual void failedAssertFileExists(const char* message, const char* file, int line, const char* checkedFile)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage  << _preStr << safeExpression(checkedFile) << _postStr << " file to exist";
        resultMessage << "it does not exist";

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    virtual void failedAssertFileNotExists(const char* message, const char* file, int line, const char* checkedFile)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage  << _preStr << safeExpression(checkedFile) << _postStr << " file to not exist";
        resultMessage << "it does exist";

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    virtual void failedAssertFileSize(const char* message, const char* file, int line, const char* checkedFile, int expectedSize, int effectiveSize)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage  << _preStr << safeExpression(checkedFile) << _postStr << " file size = " << expectedSize;
        if (effectiveSize == -1)
        {
          resultMessage << "file cannot be opened";
        }
        else
        {
          resultMessage << "effective file size = " << effectiveSize;
        }

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    virtual void failedAssertSameData(const char* message, const char* file, int line,
                              const char* xStr, const char* yStr,
                              const char* sizeStr, const void *x,
                              const void *y, unsigned size)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage  << "first " << _preExpr << sizeStr << _postExpr << "  (" << size << ") bytes to be equal at " << _preExpr << xStr << _postExpr << "  and " << _preExpr << yStr << _postExpr;

        dump(resultMessage, x, size);
        resultMessage << std::endl << _differingString << std::endl;
        dump(resultMessage, y, size);

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str(), true);
    }

    virtual void failedAssertSameFiles(const char* message, const char* file, int line, const char*, const char*, const char* explanation)
    {
        stop(message, file, line) << _errorString << ": " << explanation;
    }



    virtual void failedAssertDelta(const char* message, const char* file, int line,
                           const char* xStr, const char* yStr, const char* dStr,
                           const char* x, const char* y, const char* d)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage  << _preExpr << xStr << " == " << yStr << _postExpr << " within delta: " << _preExpr << dStr << _postExpr;
        resultMessage << _preExpr << x    << " != " << y    << _postExpr << " and delta = "   << d;

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    virtual void failedAssertDiffers(const char* message, const char* file, int line,
                             const char* xStr, const char* yStr,
                             const char* value)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage  << _preExpr << xStr  << " != " << yStr  << _postExpr;
        resultMessage << _preExpr << value << " == " << value << _postExpr;

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    virtual void failedAssertLessThan(const char* message, const char* file, int line,
                              const char* xStr, const char* yStr,
                              const char* x, const char* y)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage  << _preExpr << xStr << " < "  << yStr << _postExpr;
        resultMessage << _preExpr << x    << " >= " << y    << _postExpr;

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    virtual void failedAssertLessThanEquals(const char* message, const char* file, int line,
                                    const char* xStr, const char* yStr,
                                    const char* x, const char* y)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage  << _preExpr << xStr << " <= " << yStr << _postExpr;
        resultMessage << _preExpr << x    << " > "  << y    << _postExpr;

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    virtual void failedAssertGreaterThan(const char* message, const char* file, int line,
                              const char* xStr, const char* yStr,
                              const char* x, const char* y)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage  << _preExpr << xStr << " > "  << yStr << _postExpr;
        resultMessage << _preExpr << x    << " <= " << y    << _postExpr;

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    virtual void failedAssertGreaterThanEquals(const char* message, const char* file, int line,
                                    const char* xStr, const char* yStr,
                                    const char* x, const char* y)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage  << _preExpr << xStr << " >= " << yStr << _postExpr;
        resultMessage << _preExpr << x    << " < "  << y    << _postExpr;

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }


    virtual void failedAssertRelation(const char* message, const char* file, int line,
                              const char* relation, const char* xStr, const char* yStr,
                              const char* x, const char* y)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage  << relation << "(" << xStr << ", " << yStr << ") to be true";
        resultMessage << relation << "(" << x    << ", " << y    << ") is false";

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    virtual void failedAssertPredicate(const char* message, const char* file, int line,
                               const char* predicate, const char* xStr, const char* x)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage  << predicate << "(" << xStr << ") to be true";
        resultMessage << predicate << "(" << x    << ") is false";

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    virtual void failedAssertThrows(const char* message, const char* file, int line, const char* expression, const char* exceptionMessage, const char* type, bool otherThrown)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage  << _preExpr << expression << _postExpr << " to throw " << _preExpr << type << _postExpr;

        if (otherThrown)
        {
          if (!isNullOrEmpty(exceptionMessage))
          {
            resultMessage << "it threw \"" << exceptionMessage << "\"";
          }
          else
          {
            resultMessage << "it threw something else";
          }
        }
        else
        {
          resultMessage << "it didn't throw";
        }
        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    virtual void failedAssertThrowsNothing(const char* message, const char* file, int line, const char* expression, const char* exceptionMessage)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage  << _preExpr << expression << _postExpr << " not to throw";
        resultMessage << "it did throw";
        if (!isNullOrEmpty(exceptionMessage))
        {
          resultMessage << " '" << exceptionMessage << "'";
        }

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    virtual void failedAssertThrowsAnything(const char* message, const char* file, int line, const char* expression)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage  << _preExpr << expression << _postExpr << "to throw anything";
        resultMessage << "it did not throw";

        reportFailedAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }




    //! Reports a successful "assert true"
    //!
    virtual void succeededAssertTrue(const char* assertionMessage, const char* file, int line, const char* expression)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << _preExpr << expression << _postExpr << " to be true";
        resultMessage  << "It is true";

        reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    //! Reports a successful "assert false"
    //!
    virtual void succeededAssertFalse(const char* assertionMessage, const char* file, int line, const char* expression)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << _preExpr << expression << _postExpr << " to be false";
        resultMessage  << "It is false";

        reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    //! Reports a successful "nullptr"
    //!
    virtual void succeededAssertNullptr(const char* assertionMessage, const char* file, int line, const char* expression)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << _preExpr << expression << _postExpr << " to be nullptr";
        resultMessage  << "It is nullptr";

        reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    //! Reports a successful "not nullptr"
    //!
    virtual void succeededAssertNotNullptr(const char* assertionMessage, const char* file, int line, const char* expression)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << _preExpr << expression << _postExpr << " to not be nullptr";
        resultMessage  << "It is not nullptr";

        reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    //! Reports a successful "assert contains"
    //!
    virtual void succeededAssertContains(const char* message, const char* file, int line, const char* containerExpr, const char* valueExpr, const char* containerContent, const char* value)
    {
      std::ostringstream checkMessage;
      std::ostringstream resultMessage;

      checkMessage  << _preExpr << containerExpr   << _postExpr << " to contain " << _preExpr << valueExpr << _postExpr;
      resultMessage <<             containerContent             << " contains "   << value;

      reportSuccessfulAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }


    //! Reports a successful "assert not contains"
    //!
    virtual void succeededAssertNotContains(const char* message, const char* file, int line, const char* containerExpr, const char* valueExpr, const char* containerContent, const char* value)
    {
      std::ostringstream checkMessage;
      std::ostringstream resultMessage;

      checkMessage  << _preExpr << containerExpr   << _postExpr << " to not contain " << _preExpr << valueExpr << _postExpr;
      resultMessage <<             containerContent             << " does not contain "           << value;

      reportSuccessfulAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }


    //! Reports a successful "assert delta"
    //!
    virtual void succeededAssertDelta(const char* assertionMessage, const char* file, int line, const char* xStr, const char*  yStr, const char* dStr, const char* x, const char* y, const char* d)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << _preExpr << xStr << " == " << yStr << _postExpr << "  with tolerance " << _preExpr << dStr << _postExpr;
        resultMessage  << _preExpr << x    << " == " << y    << _postExpr << "  with tolerance " << _preExpr << d    << _postExpr;

        reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }


    //! Reports a successful "assert differ"
    //!
    virtual void succeededAssertDiffers(const char* assertionMessage, const char* file, int line, const char* xStr, const char* yStr, const char* x, const char* y)
    {
        static const char* nullStr = "NULL";

        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << _preExpr << xStr              << " != " << yStr              << _postExpr;
        resultMessage  << _preExpr << (x ? x : nullStr) << " != " << (y ? y : nullStr) << _postExpr;    // Must support case when x or y represent a "NULL" string

        reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    //! Reports a successful "assert empty"
    //!
    virtual void succeededAssertEmpty (const char* message, const char* file, int line, const char* containerExpr)
    {
      std::ostringstream checkMessage;
      std::ostringstream resultMessage;

      checkMessage  << _preExpr << containerExpr << _postExpr << " to be empty";
      resultMessage << "it is empty";

      reportSuccessfulAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    //! Reports a successful "assert not empty"
    //!
    virtual void succeededAssertNotEmpty (const char* message, const char* file, int line, const char* containerExpr, const char* containerContent)
    {
      std::ostringstream checkMessage;
      std::ostringstream resultMessage;

      checkMessage  << _preExpr << containerExpr << _postExpr << " to be not empty";
      resultMessage << "it is not empty; it contains: " << containerContent;;

      reportSuccessfulAssert(message, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }



    //! Reports a successful "assert equal"
    //!
    virtual void succeededAssertEquals(const char* assertionMessage, const char* file, int line, const char* xStr, const char* yStr, const char* x, const char* y)
    {
        static const char* nullStr = "NULL";

        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << _preExpr << xStr              << " == " << yStr              << _postExpr;
        resultMessage  << _preExpr << (x ? x : nullStr) << " == " << (y ? y : nullStr) << _postExpr;    // Must support case when x or y represent a "NULL" string

        reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }


    //! Reports a successful "assert greater than"
    //!
    virtual void succeededAssertGreaterThan(const char* assertionMessage, const char* file, int line, const char* xStr, const char* yStr, const char* x, const char* y)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << _preExpr << xStr << " > " << yStr << _postExpr;
        resultMessage  << _preExpr << x    << " > " << y    << _postExpr;

        reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }


    //! Reports a successful "assert greater than or equal"
    //!
    virtual void succeededAssertGreaterThanEquals(const char* assertionMessage, const char* file, int line, const char* xStr, const char* yStr, const char* x, const char* y)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << _preExpr << xStr << " >= " << yStr << _postExpr;
        resultMessage  << _preExpr << x    << " >= " << y    << _postExpr;

        reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }


    //! Reports a successful "assert less than"
    //!
    virtual void succeededAssertLessThan(const char* assertionMessage, const char* file, int line, const char* xStr, const char* yStr, const char* x, const char* y)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << _preExpr << xStr << " < " << yStr << _postExpr;
        resultMessage  << _preExpr << x    << " < " << y    << _postExpr;

        reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }


    //! Reports a successful "assert less than or equal"
    //!
    virtual void succeededAssertLessThanEquals(const char* assertionMessage, const char* file, int line, const char* xStr, const char* yStr, const char* x, const char* y)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << _preExpr << xStr << " <= " << yStr << _postExpr;
        resultMessage  << _preExpr << x    << " <= " << y    << _postExpr;

        reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    //! Reports a successful "assert predicate"
    //!
    virtual void succeededAssertPredicate(const char* assertionMessage, const char* file, int line, const char* pStr, const char* xStr, const char* x)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << pStr << _preExpr << xStr << _postExpr << " to be true";
        resultMessage  << pStr << _preExpr << x    << _postExpr << " is true";

        reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }


    //! Reports a successful "assert relation"
    //!
    virtual void succeededAssertRelation(const char* assertionMessage, const char* file, int line, const char* rStr, const char* xStr, const char* yStr, const char* x, const char* y)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << rStr << _preExpr << xStr << ", " << yStr << _postExpr << " to be true";
        resultMessage  << rStr << _preExpr << x    << ", " << y    << _postExpr << " is true";

        reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }


    //! Reports a successful "assert same data"
    //!
    virtual void succeededAssertSameData(const char* assertionMessage, const char* file, int line, const char* xStr, const char* yStr, const char* sizeStr, const void* x, const void* /* y */, unsigned size)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage << "first " << _preExpr << sizeStr << _postExpr << " bytes to be equal at ";
        checkMessage << _preExpr << xStr << _postExpr;
        checkMessage << " and ";
        checkMessage << _preExpr << yStr << _postExpr;

        resultMessage << size  << " first bytes are equal: " << std::endl;
        dump(resultMessage, x, size);

        reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }


    //! Reports a successful "assert file content"
    //!
    virtual void succeededAssertFileContent(const char* assertionMessage, const char* file, int line, const char* checkedFile, const char* /*expectedContent*/, const char* expectedContentExpr, const DifferenceInfo& diffInfo)
    {
      std::ostringstream checkMessage;
      std::ostringstream resultMessage;

      checkMessage  << _preStr  << safeExpression(checkedFile)         << _postStr  << " file content (" << DifferenceInfo::lineStr(diffInfo.leftLine)  << ") to be equal to ";
      checkMessage  << _preExpr << safeExpression(expectedContentExpr) << _postExpr;
      resultMessage << "its content is as expected";

      reportSuccessfulAssert(assertionMessage, file, line, checkMessage, resultMessage);
    }


    //! Reports a successful "assert file exists"
    //!
    virtual void succeededAssertFileExists(const char* assertionMessage, const char* file, int line, const char* checkedFile)
    {
      std::ostringstream checkMessage;
      std::ostringstream resultMessage;

      checkMessage  << _preStr << safeExpression(checkedFile) << _postStr << " file to exist";
      resultMessage << "it exists";

      reportSuccessfulAssert(assertionMessage, file, line, checkMessage, resultMessage);
    }


    //! Reports a successful "assert file does not exist"
    //!
    virtual void succeededAssertFileNotExists(const char* assertionMessage, const char* file, int line, const char* checkedFile)
    {
      std::ostringstream checkMessage;
      std::ostringstream resultMessage;

      checkMessage  << _preStr << safeExpression(checkedFile) << _postStr << " file to not exist";
      resultMessage << "it does not exist";

      reportSuccessfulAssert(assertionMessage, file, line, checkMessage, resultMessage);
    }

    //! Reports a successful "assert file size"
    //!
    virtual void succeededAssertFileSize(const char* assertionMessage, const char* file, int line, const char* checkedFile, int expectedSize, int effectiveSize)
    {
      std::ostringstream checkMessage;
      std::ostringstream resultMessage;

      checkMessage  << _preStr << safeExpression(checkedFile) << _postStr << " file size = " << expectedSize;
      resultMessage << "effective file size = " << effectiveSize;

      reportSuccessfulAssert(assertionMessage, file, line, checkMessage, resultMessage);
    }

    //! Reports a successful "assert same file"
    //!
    virtual void succeededAssertSameFiles(const char* assertionMessage, const char* file, int line, const char* file1, const char* file2)
    {
      std::ostringstream checkMessage;
      std::ostringstream resultMessage;

      checkMessage   << " file: " << _preStr << file1 << _postStr << " to be equal to file " << _preStr << file2 << _postStr;
      resultMessage  << " files are equal";

      reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }


    //! Reports a successful "assert throws"
    //!
    virtual void succeededAssertThrows(const char* assertionMessage, const char* file, int line, const char* expr, const char* type, const char* exceptionMessage)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        if (std::strcmp(type, "...") == 0)
        {
            succeededAssertThrowsAnything(assertionMessage, file, line, expr);
        }
        else
        {
            checkMessage   << _preExpr << expr << _postExpr << " to throw " << _preExpr << type << _postExpr;
            resultMessage  << "It did throw "  << _preExpr  << (exceptionMessage ? exceptionMessage : type) << _postExpr;
        }

        reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }


    //! Reports a successful "assert not throws"
    //!
    virtual void succeededAssertThrowsNothing(const char* assertionMessage, const char* file, int line, const char* expr)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << _preExpr << expr << _postExpr << " to not throw any type of exceptions";
        resultMessage  << "It did not throw";

        reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    //! Reports a successful "assert not throws"
    //!
    virtual void succeededAssertThrowsAnything(const char* assertionMessage, const char* file, int line, const char* expr)
    {
        std::ostringstream checkMessage;
        std::ostringstream resultMessage;

        checkMessage   << _preExpr << expr << _postExpr << " to throw any type of exceptions";
        resultMessage  << "It did throw";

        reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }


protected:
    OutputStream *outputStream() const
    {
        return _o;
    }

private:
    ErrorFormatter(const ErrorFormatter &);
    ErrorFormatter &operator=(const ErrorFormatter &);


    OutputStream& doMessage(const char* context, const char* message, const char* file, int line)
    {
        reportTest();
        reportDataDrivenTest();

        (*_o) << endl;

        if (file)
        {
          (*_o) << adaptSourceFilePath(file) << _preLine << line << _postLine << ": ";
        }

        if (context)
        {
          (*_o) << context << ": ";
        }

        if (!isNullOrEmpty(message))
        {
          (*_o) << message;
        }
        else
        {
          (*_o) << "Missing message !!!";
        }

        _dotting  = false;
        return (*_o);
    }

    void reportFileAndLine(const char* file, int line)
    {
      (*_o) << endl << adaptSourceFilePath(file) << _preLine << line << _postLine << ": ";
    }

    void reportTest()
    {
        if (_reported)
        {
            return;
        }

        (*_o) << endl << endl << "In " << tracker().suite().suiteName() << "::" << tracker().test().testName() << ": ";
        _reported = true;
        _dotting  = false;
    }

    //! If current test is in fact a "sub" test from a data driven "master" test, it adds information to identify this test sample
    //!
    void reportDataDrivenTest()
    {
        TestTracker&           tracker         = TestTracker::tracker();
        const TestDescription& testDescription = tracker.test();

        int testId = testDescription.currentDataDrivenTestId();

        if (!testDescription.isDataDriven() || (testId == _previousDataDrivenTestId))
        {
          return;
        }

        (*_o) << endl << "==> In data driven test with id: " << testId;
        const char* inputSample = testDescription.inputSampleAsText();
        if (inputSample)
        {
          (*_o) << " and input sample: " << inputSample;
        }

        _previousDataDrivenTestId = testId;
        _dotting = false;
    }

    OutputStream& stop(const char* message, const char* file, int line)
    {
        reportTest();
        reportDataDrivenTest();
        reportFileAndLine(file, line);

        if (!isNullOrEmpty(message))
        {
          (*_o) << "Message: " << message;
          stop(0, file, line);
        }

        _dotting = false;
        return (*_o);
    }

    //! Prints messages related to failed assertion
    //!
    OutputStream& reportFailedAssert (const char* assertionMessage, const char* file, int line, const char* checkMessage, const char* resultMessage, bool resultOnNewLine = false)
    {
        OutputStream& os = stop(assertionMessage, file, line);

        os << _errorString;
        os << ": Expected "  << checkMessage;
        os << ",";
        if (resultOnNewLine)
        {
          os << endl << "But found: " << endl;
        }
        else
        {
          os << " but found ";
        }
        os << resultMessage;

        return os;
    }

    //! Prints messages related to successful assertion
    //!
    OutputStream& reportSuccessfulAssert (const char* assertionMessage, const char* file, int line, const std::ostringstream& checkMessage, const std::ostringstream& resultMessage)
    {
      return reportSuccessfulAssert(assertionMessage, file, line, checkMessage.str().c_str(), resultMessage.str().c_str());
    }

    //! Prints messages related to successful assertion
    //!
    OutputStream& reportSuccessfulAssert (const char* assertionMessage, const char* file, int line, const char* checkMessage, const char* resultMessage)
    {
        if (displaySuccess())
        {
            reportTest();
            reportDataDrivenTest();
            reportFileAndLine(file, line);

            if (!isNullOrEmpty(assertionMessage))
            {
              (*_o) << "Message: " << assertionMessage;
              reportSuccessfulAssert(0, file, line, checkMessage, resultMessage);
            }
            else
            {
              (*_o) << _successString;
              (*_o) << ": Expected " << checkMessage;
              (*_o) << ", Result: "  << resultMessage;
            }
        }

        return (*_o);
    }

    //! Returns a string (representing an expression) that is never nullptr
    //!
    const char* safeExpression(const char* expression)
    {
      return expression ? expression : "NULL";
    }

    template<typename TStream>
    void dump(TStream& stream, const void *buffer, unsigned size)
    {
        if (!buffer)
        {
            dumpNull(stream);
        }
        else
        {
            dumpBuffer(stream, buffer, size);
        }
    }

    template<typename TStream>
    void dumpNull(TStream& stream)
    {
        stream << "   (null)";
    }

    template<typename TStream>
    void dumpBuffer(TStream& stream, const void *buffer, unsigned size)
    {
        unsigned dumpSize = size;
        if (maxDumpSize() && (dumpSize > maxDumpSize()))
        {
            dumpSize = maxDumpSize();
        }

        const unsigned char *p = (const unsigned char *)buffer;
        char asHex[3];
        stream << "   { ";
        for (unsigned i = 0; i < dumpSize; ++ i)
        {
            byteToHex(*p++, asHex);
            stream << asHex << " ";
        }

        if (dumpSize < size)
        {
            stream << "... ";
        }
        stream << "}";
    }


    void dump(const void *buffer, unsigned size)
    {
        dump(*_o, buffer, size);
    }

    static void endl(OutputStream& os)
    {
        OutputStream::endl(os);
    }

    static void endl(std::ostringstream& os)
    {
        os << std::endl;
    }

    unsigned      _lastAssertsCount;          //!< This is to detect call to leaveTest from data driven last test and from the "master" test (to adapt messages, dotting...)
    bool          _dotting;
    bool          _reported;
    int           _previousDataDrivenTestId;  //!< Tracks the id of reported data driven tests
    OutputStream* _o;

    const char* const _preLine;
    const char* const _postLine;
    const char* const _preExpr;
    const char* const _postExpr;
    const char* const _preStr;    // Char(s) that appear before something that should be read as a string
    const char* const _postStr;   // Char(s) that appear after something that should be read as a string
    const char* const _differingString;
    const char* const _errorString;
    const char* const _warningString;
    const char* const _successString;
};
}

#endif // __cxxtest__ErrorFormatter_h__

