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

#ifndef UNIX_ERROR_PRINTER_H_N4C6JUX4
#define UNIX_ERROR_PRINTER_H_N4C6JUX4

#ifndef _CXXTEST_HAVE_STD
#   define _CXXTEST_HAVE_STD
#endif // _CXXTEST_HAVE_STD

#include <cxxtest/Flags.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/ValueTraits.h>
#include <cxxtest/StdValueTraits.h>
#include <cxxtest/ErrorFormatter.h> // CxxTest::OutputStream

namespace CxxTest
{
struct DifferenceInfo;

class UNIXErrorFormatter : public TestListener
{
public:
    UNIXErrorFormatter(OutputStream *o, const char *preLine = ":", const char *postLine = "") :
        _reported(false),
        _o(o),
        _preLine(preLine),
        _postLine(postLine)
    {
    }

    int run()
    {
        TestRunner::runAllTests(*this);
        return tracker().failedTests();
    }

    static void totalTests(OutputStream &o)
    {
        char s[WorldDescription::MAX_STRLEN_TOTAL_TESTS];
        const WorldDescription &wd = tracker().world();
        o << wd.strTotalTests(s) << (wd.numTotalTests() == 1 ? " test" : " tests");
    }

    virtual void enterTest(const TestDescription &)
    {
        _reported = false;
    }

    virtual void leaveWorld(const WorldDescription &desc)
    {
        if (tracker().failedTests())
        {
            (*_o) << "Failed " << tracker().failedTests() << " of " << totalTests << endl;
            unsigned numPassed = desc.numTotalTests() - tracker().failedTests() - tracer().skippedTests();
            unsigned numTotal = desc.numTotalTests() - tracker().skippedTests();
            if (numTotal == 0)
            {
                (*_o) << "Success rate: 100%" << endl;
            }
            else
            {
                (*_o) << "Success rate: " << (unsigned)(numPassed * 100.0 / numTotal) << "%" << endl;
            }
        }
    }

    virtual void trace(const char* message, const char *file, int line)
    {
        stop(file, line) << "Trace: " <<
                         expression << endl;
    }

    virtual void warning(const char* message, const char *file, int line)
    {
        stop(file, line) << "Warning: " <<
                         expression << endl;
    }

    virtual void skippedTest(const char* message, const char *file, int line)
    {
        stop(file, line) << "Warning: Test skipped: " <<
                         expression << endl;
    }

    virtual void failedTest(const char* message, const char *file, int line)
    {
        stop(file, line) << "Error: Test failed: " << expression << endl;
    }

    virtual void failedTestInternal(const char* message, const char *file, int line)
    {
        stop(file, line) << "Message" << message<< endl;
    }

    virtual void notSupportedAssert(const char* message, const char* file, int line, const char* notSupportedAssertName, const char* requiredCondition)
    {
      OutputStream& os = stop(message, file, line);

      os << notSupportedAssertName;
      if (isNullOrEmpty(requiredCondition))
      {
        os << " is only supported when " << requiredCondition;
      }
      else
      {
        os << " is not supported yet";
      }
    }


    virtual void unexpectExceptionFromTest(const char* message, const char *file, int line)
    {
        stop(file, line) << "Message" << message<< endl;
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

        stop(file, line) << merger.get()
    }


    virtual void failedAssertTrue(const char* message, const char *file, int line)
    {
        OutputStream& os = stop(message, file, line);
        os << "Error: Expected '" << (expression ? expression : "???") << "' to be true, but it is false";
        os << endl;
    }

    virtual void failedAssertFalse(const char* message, const char *file, int line, const char* expression)
    {
        OutputStream& os = stop(message, file, line);
        os << "Error: Expected '" << (expression ? expression : "???") << "' to be false, but it is true";
        os << endl;
    }

    virtual void failedAssertNullptr(const char* message, const char *file, int line, const char* expression)
    {
        OutputStream& os = stop(message, file, line);
        os << "Error: Expected '" << (expression ? expression : "???") << "' to be nullptr, but it is not nullptr";
        os << endl;
    }

    virtual void failedAssertNotNullptr(const char* message, const char *file, int line, const char* expression)
    {
        OutputStream& os = stop(message, file, line);
        os << "Error: Expected '" << (expression ? expression : "???") << "' to not be nullptr, but it is nullptr";
        os << endl;
    }

    virtual void failedAssertEmpty(const char* message, const char* file, int line, const char* containerExpr, const char* containerContent)
    {
      stop(file, line) << "Error: Expected (" << containerExpr << ") to be empty, but it is not. Its content is: " << containerContent << endl;
    }

    virtual void failedAssertEmpty(const char* message, const char* file, int line, const char* containerExpr)
    {
      stop(file, line) << "Error: Expected (" << containerExpr << ") to be not empty, found it is" << endl;
    }


    virtual void failedAssertEquals(const char* message, const char *file, int line,
                            const char *xStr, const char *yStr,
                            const char *x, const char *y)
    {
        stop(file, line) << "Error: Expected (" <<
                         xStr << " == " << yStr << "), found (" <<
                         x << " != " << y << ")" << endl;
    }

    virtual void failedAssertSameData(const char* message, const char *file, int line,
                              const char *xStr, const char *yStr,
                              const char *sizeStr, const void *x,
                              const void *y, unsigned size)
    {
        stop(file, line) << "Error: Expected " << sizeStr << " (" << size << ") bytes to be equal at (" <<
                         xStr << ") and (" << yStr << "), found:" << endl;
        dump(x, size);
        (*_o) << "     differs from" << endl;
        dump(y, size);
    }

    virtual void failedAssertFileContent(const char* message, const char* file, int line, const char* checkedFile, const char* /*expectedContent*/, const char* expectedContentExpr, const DifferenceInfo& diffInfo)
    {
      static const char* _preStr  = "\"";
      static const char* _postStr = "\"";

      std::ostringstream checkMessage;
      std::ostringstream resultMessage;

      checkMessage  << _preStr << checkedFile << _postStr << " file content to be equal to (" << expectedContentExpr << ")";
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

      stop(file, line) << "Error: Expected " << checkMessage << ", found " << resultMessage;
    }

    virtual void failedAssertFileExists(const char* message, const char* file, int line, const char* checkedFile)
    {
        stop(file, line) << "Error: file'" << checkedFile << "' does not exist" << endl;
    }

    virtual void failedAssertFileNotExists(const char* message, const char* file, int line, const char* checkedFile)
    {
        stop(file, line) << "Error: file'" << checkedFile << "' does exist" << endl;
    }

    virtual void failedAssertFileSize(const char* message, const char* file, int line, const char* checkedFile, int expectedSize, int effectiveSize)
    {
        stop(file, line) << "Error: Expected file '" << checkedFile << "' size to be:" << expectedSize << ", found size: " << effectiveSize;
    }

    virtual void failedAssertSameFiles(const char* message, const char* file, int line,
                               const char*, const char*,
                               const char* explanation
                              )
    {
        stop(file, line) << "Error: " << explanation << endl;
    }

    virtual void failedAssertDelta(const char* message, const char *file, int line,
                           const char *xStr, const char *yStr, const char *dStr,
                           const char *x, const char *y, const char *d)
    {
        stop(file, line) << "Error: Expected (" <<
                         xStr << " == " << yStr << ") up to " << dStr << " (" << d << "), found (" <<
                         x << " != " << y << ")" << endl;
    }

    virtual void failedAssertDiffers(const char* message, const char *file, int line,
                             const char *xStr, const char *yStr,
                             const char *value)
    {
        stop(file, line) << "Error: Expected (" <<
                         xStr << " != " << yStr << "), found (" <<
                         value << ")" << endl;
    }

    virtual void failedAssertLessThan(const char* message, const char *file, int line,
                              const char *xStr, const char *yStr,
                              const char *x, const char *y)
    {
        stop(file, line) << "Error: Expected (" <<
                         xStr << " < " << yStr << "), found (" <<
                         x << " >= " << y << ")" << endl;
    }

    virtual void failedAssertLessThanEquals(const char* message, const char *file, int line,
                                    const char *xStr, const char *yStr,
                                    const char *x, const char *y)
    {
        stop(file, line) << "Error: Expected (" <<
                         xStr << " <= " << yStr << "), found (" <<
                         x << " > " << y << ")" << endl;
    }

    virtual void failedAssertGreaterThan(const char* message, const char *file, int line,
                              const char *xStr, const char *yStr,
                              const char *x, const char *y)
    {
        stop(file, line) << "Error: Expected (" <<
                         xStr << " > " << yStr << "), found (" <<
                         x << " <= " << y << ")" << endl;
    }

    virtual void failedAssertGreaterThanEquals(const char* message, const char *file, int line,
                                    const char *xStr, const char *yStr,
                                    const char *x, const char *y)
    {
        stop(file, line) << "Error: Expected (" <<
                         xStr << " >= " << yStr << "), found (" <<
                         x << " < " << y << ")" << endl;
    }

    virtual void failedAssertRelation(const char* message, const char *file, int line,
                              const char *relation, const char *xStr, const char *yStr,
                              const char *x, const char *y)
    {
        stop(file, line) << "Error: Expected " << relation << "( " <<
                         xStr << ", " << yStr << " ), found !" << relation << "( " << x << ", " << y << " )" << endl;
    }

    virtual void failedAssertPredicate(const char* message, const char *file, int line,
                               const char *predicate, const char *xStr, const char *x)
    {
        stop(file, line) << "Error: Expected " << predicate << "( " <<
                         xStr << " ), found !" << predicate << "( " << x << " )" << endl;
    }

    virtual void failedAssertThrows(const char* message,
                                    const char* file,
                                    int         line,
                                    const char* expression,
                                    const char* /*exceptionMessage*/,
                                    const char* type,
                                    bool        otherThrown)
    {
        stop(file, line) << "Error: Expected (" << expression << ") to throw (" <<
                         type << ") but it " << (otherThrown ? "threw something else" : "didn't throw") <<
                         endl;
    }

    virtual void failedAssertThrowsNothing(const char* message, const char *file, int line, const char *expression, const char* exceptionMessage)
    {
        OutputStream& os = stop(file, line);

        os << "Error: Expected (" << expression << ") not to throw, but it did throw"

        if (exceptionMessage && (strlen(exceptionMessage) != 0))
        {
          os << " '" << exceptionMessage << "'";
        }

        os << endl;
    }

    virtual void failedAssertThrowsAnything(const char* message, const char *file, int line, const char *expression)
    {
        stop(file, line) << "Error: Expected (" << expression << ") to throw anything, but it did not" << endl;
    }

    IGNORE_SUCCEEDED_ASSERTIONS

protected:
    OutputStream *outputStream() const
    {
        return _o;
    }

private:
    UNIXErrorFormatter(const UNIXErrorFormatter &);
    UNIXErrorFormatter &operator=(const UNIXErrorFormatter &);

    OutputStream& stop(const char *file, int line)
    {
        reportTest();
        return (*_o) << file << _preLine << line << _postLine << ": ";
    }

    void reportTest(void)
    {
        if (_reported)
        {
            return;
        }
        (*_o) << tracker().suite().file() << ": In " << tracker().suite().suiteName() << "::" << tracker().test().testName() << ":" << endl;
        _reported = true;
    }

    void dump(const void *buffer, unsigned size)
    {
        if (!buffer)
        {
            dumpNull();
        }
        else
        {
            dumpBuffer(buffer, size);
        }
    }

    void dumpNull()
    {
        (*_o) << "   (null)" << endl;
    }

    void dumpBuffer(const void *buffer, unsigned size)
    {
        unsigned dumpSize = size;
        if (maxDumpSize() && dumpSize > maxDumpSize())
        {
            dumpSize = maxDumpSize();
        }

        const unsigned char *p = (const unsigned char *)buffer;
        (*_o) << "   { ";
        for (unsigned i = 0; i < dumpSize; ++ i)
        {
            (*_o) << byteToHex(*p++) << " ";
        }
        if (dumpSize < size)
        {
            (*_o) << "... ";
        }
        (*_o) << "}" << endl;
    }

    static void endl(OutputStream &o)
    {
        OutputStream::endl(o);
    }

    bool _reported;
    OutputStream *_o;
    const char *_preLine;
    const char *_postLine;
};

// ========================================================================================
// = Actual runner is a subclass only because that is how the original ErrorPrinter works =
// ========================================================================================

class unix : public UNIXErrorFormatter
{
public:
    unix(CXXTEST_STD(ostream) &o = CXXTEST_STD(cerr), const char *preLine = ":", const char *postLine = "") :
        UNIXErrorFormatter(new Adapter(o), preLine, postLine) {}
    virtual ~unix() { delete outputStream(); }

private:
    class Adapter : public OutputStream
    {
        CXXTEST_STD(ostream) &_o;
    public:
        Adapter(CXXTEST_STD(ostream) &o) : _o(o) {}
        void flush() { _o.flush(); }
        OutputStream &operator<<(const char *s) { _o << s; return *this; }
        OutputStream &operator<<(Manipulator m) { return OutputStream::operator<<(m); }
        OutputStream &operator<<(unsigned i)
        {
            char s[1 + 3 * sizeof(unsigned)];
            numberToString(i, s);
            _o << s;
            return *this;
        }
    };
};
}

#endif /* end of include guard: UNIX_ERROR_PRINTER_H_N4C6JUX4 */

// Copyright 2008 Sandia Corporation. Under the terms of Contract
// DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government
// retains certain rights in this software.
