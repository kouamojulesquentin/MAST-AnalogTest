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

// Licensed under the LGPL, see http://www.gnu.org/licenses/lgpl.html

#ifndef __CXXTEST__XMLFORMATTER_H
#define __CXXTEST__XMLFORMATTER_H

//
// The XmlFormatter is a TestListener that
// prints reports of the errors to an output
// stream in the form of an XML document.
//

// The following definitions are used if stack trace support is enabled,
// to give the traces an easily-parsable XML format.  If stack tracing is
// not enabled, then these definitions will be ignored.
#define CXXTEST_STACK_TRACE_ESCAPE_AS_XML
#define CXXTEST_STACK_TRACE_NO_ESCAPE_FILELINE_AFFIXES

#define CXXTEST_STACK_TRACE_INITIAL_PREFIX "<stack-frame function=\""
#define CXXTEST_STACK_TRACE_INITIAL_SUFFIX "\"/>\n"
#define CXXTEST_STACK_TRACE_OTHER_PREFIX CXXTEST_STACK_TRACE_INITIAL_PREFIX
#define CXXTEST_STACK_TRACE_OTHER_SUFFIX CXXTEST_STACK_TRACE_INITIAL_SUFFIX
#define CXXTEST_STACK_TRACE_ELLIDED_MESSAGE ""
#define CXXTEST_STACK_TRACE_FILELINE_PREFIX "\" location=\""
#define CXXTEST_STACK_TRACE_FILELINE_SUFFIX ""


#include <cxxtest/TestRunner.h>
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/ValueTraits.h>
#include <cxxtest/ErrorFormatter.h>
#include <cxxtest/StdHeaders.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <ctime>

namespace CxxTest
{
struct DifferenceInfo;

class TeeOutputStreams
{
private:
    class teebuffer : public std::basic_streambuf<char>
    {
        typedef std::basic_streambuf<char> streambuf_t;
    public:
        teebuffer(streambuf_t * buf1, streambuf_t * buf2)
            : buffer1(buf1), buffer2(buf2)
        {}

        virtual int overflow(int c)
        {
            if (c == EOF)
            {
                return !EOF;
            }
            else
            {
                int const ans1 = buffer1->sputc(c);
                int const ans2 = buffer2->sputc(c);
                return ans1 == EOF || ans2 == EOF ? EOF : c;
            }
        }

        virtual int sync()
        {
            int ans1 = buffer1->pubsync();
            int ans2 = buffer2->pubsync();
            return ans1 || ans2 ? -1 : 0;
        }

        streambuf_t * buffer1;
        streambuf_t * buffer2;
    };

public:
    TeeOutputStreams(std::ostream& _cout, std::ostream& _cerr)
        : out(),
          err(),
          orig_cout(_cout),
          orig_cerr(_cerr),
          tee_out(out.rdbuf(), _cout.rdbuf()),
          tee_err(err.rdbuf(), _cerr.rdbuf())
    {
        orig_cout.rdbuf(&tee_out);
        orig_cerr.rdbuf(&tee_err);
    }

    ~TeeOutputStreams()
    {
        orig_cout.rdbuf(tee_out.buffer2);
        orig_cerr.rdbuf(tee_err.buffer2);
    }

    std::stringstream out;
    std::stringstream err;

private:
    std::ostream&  orig_cout;
    std::ostream&  orig_cerr;
    teebuffer      tee_out;
    teebuffer      tee_err;
};

class ElementInfo
{
public:
    std::string name;
    std::stringstream value;
    std::map<std::string, std::string> attribute;

    ElementInfo()
        : name(), value(), attribute()
    {}

    ElementInfo(const ElementInfo& rhs)
        : name(rhs.name), value(rhs.value.str()), attribute(rhs.attribute)
    {}

    ElementInfo& operator=(const ElementInfo& rhs)
    {
        name = rhs.name;
        value.str(rhs.value.str());
        attribute = rhs.attribute;
        return *this;
    }

    template <class Type>
    void add(const std::string& name_, Type& value_)
    {
        std::ostringstream os;
        os << value_;
        attribute[name_] = os.str();
    }

    void write(OutputStream& os)
    {
        os << "        <" << name.c_str() << " ";
        std::map<std::string, std::string>::iterator curr = attribute.begin();
        std::map<std::string, std::string>::iterator end = attribute.end();
        while (curr != end)
        {
            os << curr->first.c_str()
               << "=\"" << curr->second.c_str() << "\" ";
            curr++;
        }
        if (value.str().empty())
        {
            os << "/>";
        }
        else
        {
            os << ">" << escape(value.str()).c_str();
            if (value.str().length() > 40)
            {
              os.endl(os);
            }
            os << "</" << name.c_str() << ">";
        }
        os.endl(os);
    }

    std::string escape(const std::string& str)
    {
        std::string escStr = "";
        for (size_t i = 0; i < str.length(); i++)
        {
            switch (str[i])
            {
            case '"':  escStr += "&quot;"; break;
            case '\'': escStr += "&apos;"; break;
            case '<':  escStr += "&lt;"; break;
            case '>':  escStr += "&gt;"; break;
            case '&':  escStr += "&amp;"; break;
            default:   escStr += str[i]; break;
            }
        }
        return escStr;
    }

};

class TestCaseInfo
{
public:

    TestCaseInfo() : fail(false), error(false), runtime(0.0) {}
    std::string className;
    std::string testName;
    std::string line;
    bool fail;
    bool error;
    double runtime;
    std::list<ElementInfo> elements;
    typedef std::list<ElementInfo>::iterator element_t;
    std::string world;

    element_t add_element(const std::string& name)
    {
        element_t elt = elements.insert(elements.end(), ElementInfo());
        elt->name = name;
        return elt;
    }

    element_t update_element(const std::string& name)
    {
        element_t elt = elements.begin();
        while (elt != elements.end())
        {
            if (elt->name == name)
            {
                return elt;
            }
            elt++;
        }
        return add_element(name);
    }

    void write(OutputStream &o)
    {
        o << "    <testcase classname=\"" << className.c_str()
          << "\" name=\"" << testName.c_str()
          << "\" line=\"" << line.c_str() << "\"";
        bool elts = false;
        element_t curr = elements.begin();
        element_t end  = elements.end();
        while (curr != end)
        {
            if (!elts)
            {
                o << ">";
                o.endl(o);
                elts = true;
            }
            curr->write(o);
            curr++;
        }
        if (elts)
        {
            o << "    </testcase>";
        }
        else
        {
            o << " />";
        }
        o.endl(o);
    }

};

class XmlFormatter : public TestListener
{
public:
    XmlFormatter(OutputStream *o, OutputStream *ostr, std::ostringstream *os)
        : _o(o), _ostr(ostr), _os(os), stream_redirect(NULL)
    {}

    std::list<TestCaseInfo> info;
    std::list<TestCaseInfo>::iterator testcase;
    typedef std::list<ElementInfo>::iterator element_t;
    std::string classname;
    int ntests;
    int nfail;
    int nerror;
    double totaltime;

    int run()
    {
        TestRunner::runAllTests(*this);
        return tracker().failedTests();
    }

    void enterWorld(const WorldDescription& desc)
    {
        TestListener::enterWorld(desc);
        ntests    = 0;
        nfail     = 0;
        nerror    = 0;
        totaltime = 0;
    }

    static void totalTests(OutputStream &o)
    {
        char s[WorldDescription::MAX_STRLEN_TOTAL_TESTS];
        const WorldDescription &wd = tracker().world();
        o << wd.strTotalTests(s)
          << (wd.numTotalTests() == 1 ? " test" : " tests");
    }

    virtual void enterSuite(const SuiteDescription& desc)
    {
        classname = desc.suiteName();
        // replace "::" namespace with java-style "."
        size_t pos = 0;
        while ((pos = classname.find("::", pos)) !=
                CXXTEST_STD(string::npos))
        {
            classname.replace(pos, 2, ".");
        }
        while (! classname.empty() && classname[0] == '.')
        {
            classname.erase(0, 1);
        }

        //CXXTEST_STD(cout) << "HERE " << desc.file() << " "
        //                  << classname << CXXTEST_STD(endl);

        //classname=desc.suiteName();
        //(*_o) << "file=\"" << desc.file() << "\" ";
        //(*_o) << "line=\"" << desc.line() << "\"";
        //_o->flush();
    }

    virtual void leaveSuite(const SuiteDescription &)
    {
        std::list<TestCaseInfo>::iterator curr = info.begin();
        std::list<TestCaseInfo>::iterator end  = info.end();
        while (curr != end)
        {
            if (curr->fail) { nfail++; }
            if (curr->error) { nerror++; }
            totaltime += curr->runtime;
            ntests++;
            curr++;
        }
        curr = info.begin();
        end  = info.end();
        while (curr != end)
        {
            (*curr).write(*_ostr);
            curr++;
        }
        info.clear();
    }

    virtual void enterTest(const TestDescription & desc)
    {
        testcase = info.insert(info.end(), TestCaseInfo());
        testcase->testName = desc.testName();
        testcase->className = classname;
        std::ostringstream os;
        os << desc.line();
        testcase->line = os.str();

        if (stream_redirect)
            CXXTEST_STD(cerr) << "ERROR: The stream_redirect != NULL"
                              << CXXTEST_STD(endl);

        stream_redirect =
            new TeeOutputStreams(CXXTEST_STD(cout), CXXTEST_STD(cerr));
    }

    virtual void leaveTest(const TestDescription& testDescription)
    {
        if (tracker().testHasNoAssertion())
        {
            warning("No assertion found in this test !!!", testDescription.file(), testDescription.line());
        }

        if (stream_redirect != NULL)
        {
            std::string out = stream_redirect->out.str();
            if (! out.empty())
            {
                // silently ignore the '.'
                if (out[0] != '.' || out.size() > 1)
                {
                    testcase->add_element("system-out")->value << out;
                }
            }
            if (! stream_redirect->err.str().empty())
            {
                testcase->add_element("system-err")->value << stream_redirect->err.str();
            }

            delete stream_redirect;
            stream_redirect = NULL;
        }
    }

    virtual void leaveWorld(const WorldDescription& desc)
    {
        std::ostringstream os;
        const time_t current_date(time(0));
        char current_date_string[27];
        const size_t n = strlen(ctime_r(&current_date,current_date_string));
        if (n) {
            current_date_string[n-1] = '\0'; // remove the ending \n
        } else {
            current_date_string[0] = '\0'; // just in case...
        }
        os << totaltime;
        (*_o) << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << endl;
        (*_o) << "<testsuite name=\"" << desc.worldName() << "\" ";
        (*_o) << "date=\"" << current_date_string;
        (*_o) << "\" tests=\"" << ntests
              << "\" errors=\"" << nerror
              << "\" failures=\"" << nfail
              << "\" time=\"" << os.str().c_str() << "\" >";
        _o->endl(*_o);
        (*_o) << _os->str().c_str();
        _os->clear();
        (*_o) << "</testsuite>" << endl;
        _o->flush();
    }

    virtual void trace(const char* message, const char* /*file*/, int line)
    {
        element_t elt = testcase->add_element("trace");
        elt->add("line", line);
        elt->value << message;
    }

    virtual void warning(const char* message, const char* /*file*/, int line)
    {
        element_t elt = testcase->add_element("warning");
        elt->add("line", line);
        elt->value << message;
    }

    virtual void skippedTest(const char* message, const char* file, int line)
    {
        testSkipped(file, line, "skipped") << "Test skipped: " << message;
    }

    virtual void failedTest(const char* message, const char* file, int line)
    {
        testFailure("", file, line, "failure") << "Test failed: " << message;
    }

    virtual void failedTestInternal(const char* message, const char* file, int line)
    {
        testFailure("", file, line, "failure") << "Test failed: " << message;
    }

    virtual void notSupportedAssert(const char* message, const char* file, int line, const char* notSupportedAssertName, const char* requiredCondition)
    {
      std::stringstream& os = testFailure(message, file, line, "failure");

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

        testFailure("", file, line, "failure") << merger.get();
    }

    virtual void failedAssertTrue(const char* message, const char *file, int line, const char *expression)
    {
        testFailure(message, file, line, "failedAssertTrue") << "Assertion failed: " << expression;
    }

    virtual void failedAssertFalse(const char* message, const char *file, int line, const char *expression)
    {
        testFailure(message, file, line, "failedAssertFalse") << "Assertion failed: " << expression;
    }

    virtual void failedAssertNullptr(const char* message, const char *file, int line, const char *expression)
    {
        testFailure(message, file, line, "failedAssertNullptr") << "Assertion failed: " << expression;
    }

    virtual void failedAssertNotNullptr(const char* message, const char *file, int line, const char *expression)
    {
        testFailure(message, file, line, "failedAssertNotNullptr") << "Assertion failed: " << expression;
    }


    virtual void failedAssertEquals(const char* message, const char *file, int line,
                            const char* xStr, const char* yStr,
                            const char *x, const char *y)
    {
        testFailure(message, file, line, "failedAssertEquals")
                << "Error: Expected ("
                << xStr << " == " << yStr << "), found ("
                << x << " != " << y << ")";
    }

    virtual void failedAssertSameData(const char* message, const char *file, int line,
                              const char *xStr, const char *yStr, const char *sizeStr,
                              const void* /*x*/, const void* /*y*/, unsigned size)
    {
        testFailure(message, file, line, "failedAssertSameData")
                << "Error: Expected " << sizeStr
                << " (" << size << ")  bytes to be equal at ("
                << xStr << ") and (" << yStr << "), found";
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

      testFailure(message, file, line, "failedAssertFileContent") << "Error: Expected " << checkMessage << ", found " << resultMessage;
    }


    virtual void failedAssertFileExists(const char* message, const char* file, int line, const char* checkedFile)
    {
        testFailure(message, file, line, "failedAssertFileExists") << "Error: file '" << checkedFile << "' does not exist";
    }


    virtual void failedAssertFileNotExists(const char* message, const char* file, int line, const char* checkedFile)
    {
        testFailure(message, file, line, "failedAssertFileNotExists") << "Error: file '" << checkedFile << "' does exist";
    }

    virtual void failedAssertFileSize(const char* message, const char* file, int line, const char* checkedFile, int expectedSize, int effectiveSize)
    {
        testFailure(message, file, line, "failedAssertFileSize") << "Error: Expected file '" << checkedFile << "' size to be:" << expectedSize << ", found size: " << effectiveSize;
    }

    virtual void failedAssertSameFiles(const char* message, const char *file, int line,
                               const char *, const char *,
                               const char* explanation
                              )
    {
        testFailure(message, file, line, "failedAssertSameFiles") << "Error: " << explanation;
    }

    virtual void failedAssertDelta(const char* message, const char *file, int line,
                           const char *xStr, const char *yStr, const char *dStr,
                           const char *x, const char *y, const char *d)
    {
        testFailure(message, file, line, "failedAssertDelta")
                << "Error: Expected ("
                << xStr << " == " << yStr << ") up to " << dStr
                << " (" << d << "), found ("
                << x << " != " << y << ")";
    }

    virtual void failedAssertDiffers(const char* message, const char *file, int line,
                             const char *xStr, const char *yStr,
                             const char *value)
    {
        testFailure(message, file, line, "failedAssertDiffers")
                << "Error: Expected ("
                << xStr << " != " << yStr << "), found ("
                << value << ")";
    }

    virtual void failedAssertLessThan(const char* message, const char *file, int line,
                              const char *xStr, const char *yStr,
                              const char *x, const char *y)
    {
        testFailure(message, file, line, "failedAssertLessThan")
                << "Error: Expected (" <<
                xStr << " < " << yStr << "), found (" <<
                x << " >= " << y << ")";
    }

    virtual void failedAssertLessThanEquals(const char* message, const char *file, int line,
                                    const char *xStr, const char *yStr,
                                    const char *x, const char *y)
    {
        testFailure(message, file, line, "failedAssertLessThanEquals")
                << "Error: Expected (" <<
                xStr << " <= " << yStr << "), found (" <<
                x << " > " << y << ")";
    }

    virtual void failedAssertGreaterThan(const char* message, const char *file, int line,
                              const char *xStr, const char *yStr,
                              const char *x, const char *y)
    {
        testFailure(message, file, line, "failedAssertGreaterThan")
                << "Error: Expected (" <<
                xStr << " > " << yStr << "), found (" <<
                x << " <= " << y << ")";
    }

    virtual void failedAssertGreaterThanEquals(const char* message, const char *file, int line,
                                    const char *xStr, const char *yStr,
                                    const char *x, const char *y)
    {
        testFailure(message, file, line, "failedAssertGreaterThanEquals")
                << "Error: Expected (" <<
                xStr << " >= " << yStr << "), found (" <<
                x << " < " << y << ")";
    }


    virtual void failedAssertRelation(const char* message, const char *file, int line,
                              const char *relation, const char *xStr, const char *yStr,
                              const char *x, const char *y)
    {
        testFailure(message, file, line, "failedAssertRelation")
                << "Error: Expected " << relation << "( " <<
                xStr << ", " << yStr << " ), found !" << relation
                << "( " << x << ", " << y << " )";
    }

    virtual void failedAssertPredicate(const char* message, const char *file, int line,
                               const char *predicate, const char *xStr, const char *x)
    {
        testFailure(message, file, line, "failedAssertPredicate")
                << "Error: Expected " << predicate << "( " <<
                xStr << " ), found !" << predicate << "( " << x << " )";
    }

    virtual void failedAssertThrows(const char* message, const char *file, int line,
                            const char *expression, const char *type,
                            bool otherThrown)
    {
        testFailure(message, file, line, "failedAssertThrows")
                << "Error: Expected (" << expression << ") to throw ("  <<
                type << ") but it "
                << (otherThrown ? "threw something else" : "didn't throw");
    }

    virtual void failedAssertThrowsNothing(const char* message, const char *file, int line, const char *expression, const char* exceptionMessage)
    {
        std::stringstream& os = testFailure(message, file, line, "failedAssertThrowsNothing");
        os << "Error: Expected (" << expression;
        os << ") not to throw, but it did throw";
        if (exceptionMessage && (strlen(exceptionMessage) != 0))
        {
          os << " '" << exceptionMessage << "'";
        }
    }

    virtual void failedAssertThrowsAnything(const char* message, const char *file, int line, const char *expression)
    {
        std::stringstream& os = testFailure(message, file, line, "failedAssertThrowsAnything");
        os << "Error: Expected (" << expression;
        os << ") to throw anything, but it did not";
    }

    IGNORE_SUCCEEDED_ASSERTIONS
protected:

    OutputStream *outputStream() const
    {
        return _o;
    }

    OutputStream *outputFileStream() const
    {
        return _ostr;
    }

private:
    XmlFormatter(const XmlFormatter &);
    XmlFormatter &operator=(const XmlFormatter &);

    std::stringstream& testFailure(const char* message, const char* file, int line, const char *failureType)
    {
        testcase->fail    = true;
        element_t elt     = testcase->update_element("failure");
        bool hadEmptyBody = elt->value.str().empty();

        if (hadEmptyBody)
        {
            elt->add("type", failureType);
            elt->add("line", line);
            const char* adaptedFileName = TestListener::adaptSourceFilePath(file);
            elt->add("file", adaptedFileName);
        }

        if (message && (strlen(message) != 0))
        {
          elt->value << CXXTEST_STD(endl) << "Message: " << message << CXXTEST_STD(endl);
        }
        else if (!hadEmptyBody)
        {
          elt->value << CXXTEST_STD(endl);
        }

        return elt->value;
    }

    std::stringstream& testSkipped(const char* file, int line, const char *failureType)
    {
        //testcase->fail = true;
        element_t elt = testcase->update_element("skipped");
        if (elt->value.str().empty())
        {
            elt->add("type", failureType);
            elt->add("line", line);
            const char* adaptedFileName = TestListener::adaptSourceFilePath(file);
            elt->add("file", adaptedFileName);
        }
        else
        {
            elt->value << CXXTEST_STD(endl);
        }
        return elt->value;
    }

#if 0
    void attributeBinary(const char* name, const void *value, unsigned size)
    {
        (*_o) << name;
        (*_o) << "=\"";
        dump(value, size);
        (*_o) << "\" ";
    }

    void dump(const void *buffer, unsigned size)
    {
        if (!buffer) { return; }

        unsigned dumpSize = size;
        if (maxDumpSize() && dumpSize > maxDumpSize())
        {
            dumpSize = maxDumpSize();
        }

        const unsigned char *p = (const unsigned char *)buffer;
        for (unsigned i = 0; i < dumpSize; ++ i)
        {
            (*_o) << byteToHex(*p++) << " ";
        }
        if (dumpSize < size)
        {
            (*_o) << "... ";
        }
    }
#endif

    static void endl(OutputStream &o)
    {
        OutputStream::endl(o);
    }

    OutputStream *_o;
    OutputStream *_ostr;
    std::ostringstream *_os;

    TeeOutputStreams *stream_redirect;
};
}

#endif // __CXXTEST__XMLFORMATTER_H

