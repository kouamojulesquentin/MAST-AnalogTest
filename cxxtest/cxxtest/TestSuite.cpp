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

#ifndef __cxxtest__TestSuite_cpp__
#define __cxxtest__TestSuite_cpp__

#include <cxxtest/TestSuite.h>
#if defined(_CXXTEST_HAVE_STD)
#include <cxxtest/DifferenceInfo.h>
#include <fstream>
#include <string>
#endif

namespace CxxTest
{

//
// Test-aborting stuff
//
static bool currentAbortTestOnFail               = false;
static bool currentAbortDataDrivenTestsOnFailure = CXXTEST_DEFAULT_DDT_ABORT;

bool CountAssertDisabler::disableAssertionCount = false;

//
// TestSuite members
//
TestSuite::~TestSuite() {}
void TestSuite::setUp() {}
void TestSuite::tearDown() {}

StringMerger::StringMerger(const char* str1, const char* str2)
: m_pNextChar (m_mergedStrings)
{
  append(str1);
  append(str2);
}


//! Appends a string to current one
//!
StringMerger& StringMerger::append (const char* str)
{
  if (str)
  {
    const char* endOfBuffer = &m_mergedStrings[StringMerger::BUFFER_SIZE];

    size_t remainingBytes = endOfBuffer - m_pNextChar;
    m_pNextChar = copyStringN(m_pNextChar, str, remainingBytes);
  }
  return *this;
}
//
//  End of fct_StringMerger::append
//---------------------------------------------------------------------------





bool abortTestOnFail()
{
    return currentAbortTestOnFail;
}

bool setAbortTestOnFail(bool value)
{
    bool previousValue = currentAbortTestOnFail;
    currentAbortTestOnFail = value;
    return previousValue;
}

//! Returns whether following data driven tests are ignored when one is in failure
//!
bool abortDataDrivenTestsOnFailure()
{
  return currentAbortDataDrivenTestsOnFailure;
}

//! Sets whether following data driven tests are ignored when one is in failure
//!
bool setAbortDataDrivenTestsOnFailure(bool value)
{
  bool previousValue = currentAbortDataDrivenTestsOnFailure;
  currentAbortDataDrivenTestsOnFailure = value;
  return previousValue;
}



//! Asks tracker to tally an additional assertion
//!
void countAssert ()
{
    if (!CountAssertDisabler::disableAssertionCount)
    {
      tracker().countAsserts();
    }
}


void doAbortTest()
{
#   if defined(_CXXTEST_HAVE_EH)
    if (currentAbortTestOnFail)
    {
        throw AbortTest();
    }
#   endif // _CXXTEST_HAVE_EH
}

//
// Max dump size
//
static unsigned currentMaxDumpSize = CXXTEST_MAX_DUMP_SIZE;

unsigned maxDumpSize()
{
    return currentMaxDumpSize;
}

void setMaxDumpSize(unsigned value)
{
    currentMaxDumpSize = value;
}

//
// Some non-template functions
//
void doTrace(const char*file, int line, const char*message)
{
    if (tracker().print_tracing)
    {
        tracker().trace(message, file, line);
    }
}

void doWarn(const char*file, int line, const char*message)
{
    tracker().warning(message, file, line);
}

#   if defined(_CXXTEST_HAVE_EH)
void doSkipTest(const char* file, int line, const char* message)
{
    tracker().skippedTest(message, file, line);
    throw SkipTest();
#   else
void doSkipTest(const char* file, int line, const char*)
{
    doWarn(file, line, "Test skipping is not supported without exception handling.");
#   endif
}

void doFailTest(const char* file, int line, const char* message)
{
    tracker().failedTest(message, file, line);
    TS_ABORT();
}

void doAssertTrue( const char* file, int line, const char* expression, const char* message, bool expressionValue)
{
    if (!expressionValue)
    {
        tracker().failedAssertTrue(message, file, line, expression );
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertTrue(message, file, line, expression);
    }
}

void doAssertFalse( const char* file, int line, const char* expression, const char* message, bool expressionValue)
{
    if (expressionValue)
    {
        tracker().failedAssertFalse(message, file, line, expression );
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertFalse(message, file, line, expression);
    }
}


void doAssertNullptr(const char* file, int line, const char* expression, const char* message, bool notNull)
{
    if (notNull)
    {
        tracker().failedAssertNullptr(message, file, line, expression);
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertNullptr(message, file, line, expression);
    }
}

//! Checks that ASCII text file as expected content
//!
void doAssertFileContent(const char* file, int line, const char* filePath, const char* expectedContentExpr, const char* expectedContent, const char* message)
{
#if defined(_CXXTEST_HAVE_STD)
    DifferenceInfo diff;
    if (!fileHasExpectedContent(filePath, expectedContent, diff))
    {
        tracker().failedAssertFileContent(message, file, line, filePath, expectedContent, expectedContentExpr, diff);
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertFileContent(message, file, line, filePath, expectedContent, expectedContentExpr, diff);
    }
#else
    tracker().notSupportedAssert(message, file, line, "TS_ASSERT_FILE_CONTENT", "--have-std is enabled");
    TS_ABORT();
#endif
}

void doAssertFileExists(const char* file, int line, const char* checkedFile, const char* message)
{
#if defined(_CXXTEST_HAVE_STD)
    if (!fileExists(checkedFile))
    {
        tracker().failedAssertFileExists(message, file, line, checkedFile);
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertFileExists(message, file, line, checkedFile);
    }
#else
    tracker().notSupportedAssert(message, file, line, "xxx_ASSERT_FILE_EXISTS", "--have-std is enabled");
    TS_ABORT();
#endif
}

void doAssertFileNotExists(const char* file, int line, const char* checkedFile, const char* message)
{
#if defined(_CXXTEST_HAVE_STD)
    if (fileExists(checkedFile))
    {
        tracker().failedAssertFileNotExists(message, file, line, checkedFile);
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertFileNotExists(message, file, line, checkedFile);
    }
#else
    tracker().notSupportedAssert(message, file, line, "xxx_ASSERT_FILE_NOT_EXISTS", "--have-std is enabled");
    TS_ABORT();
#endif
}


void doAssertFileSize(const char* file, int line, const char* checkedFile, int expecteSize, const char* message)
{
#if defined(_CXXTEST_HAVE_STD)
    int effectiveFileSize = fileSize(checkedFile);
    if (effectiveFileSize != expecteSize)
    {
      tracker().failedAssertFileSize(message, file, line, checkedFile, expecteSize, effectiveFileSize);
      TS_ABORT();
    }
    else
    {
      tracker().succeededAssertFileSize(message, file, line, checkedFile, expecteSize, effectiveFileSize);
    }
#else
    tracker().notSupportedAssert(message, file, line, "xxx_ASSERT_FILE_SIZE", "--have-std is enabled");
    TS_ABORT();
#endif
}


void doAssertNotNullptr(const char* file, int line, const char* expression, const char* message, bool notNull)
{
    if (!notNull)
    {
        tracker().failedAssertNotNullptr(message, file, line, expression);
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertNotNullptr(message, file, line, expression);
    }
}


bool sameData(const void * x, const void * y, unsigned size)
{
    if (size == 0)
    {
        return true;
    }

    if (x == y)
    {
        return true;
    }

    if (!x || !y)
    {
        return false;
    }

    const char*cx = (const char*)x;
    const char*cy = (const char*)y;
    while (size --)
    {
        if (*cx++ != *cy++)
        {
            return false;
        }
    }

    return true;
}

void doAssertSameData(const char* file, int line,
                      const char* xExpr, const void * x,
                      const char* yExpr, const void * y,
                      const char* sizeExpr, unsigned size,
                      const char* message)
{
    if (!sameData(x, y, size))
    {
        tracker().failedAssertSameData(message, file, line, xExpr, yExpr, sizeExpr, x, y, size);
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertSameData(message, file, line, xExpr, yExpr, sizeExpr, x, y, size);
    }
}


#if defined(_CXXTEST_HAVE_STD)

//! Returns true if a file exits (and can be opened)
//!
bool fileExists(const char* file)
{
    std::ifstream ifs(file);
    return ifs.is_open();
}

//! Returns file size if it can be opened, -1 otherwise
//!
//! @note Returned value is the file size as reported by the os (this is not the number of characters)
//!
int fileSize(const char* file)
{
    int effectiveFileSize = -1;

    std::ifstream ifs(file, std::ios_base::in | std::ios_base::binary);
    if (ifs.is_open())
    {
      ifs.seekg(0, std::ios_base::end);
      effectiveFileSize = ifs.tellg();
    }

    return effectiveFileSize;
}

#endif





#if defined(_CXXTEST_HAVE_STD)
bool fileHasExpectedContent(const char* filePath, const std::string& expectedContent, DifferenceInfo& diff)
{
    std::ifstream file;
    file.open(filePath);
    if (!file)
    {
        diff.reason = DifferenceInfo::LEFT_NOT_EXIST;
        return false;
    }

    std::istringstream expected(expectedContent);
    std::string        nextFileLine;
    std::string        nextExpectedLine;
    int                line = 0;

    while (file && expected)
    {
      std::getline(file,     nextFileLine);
      std::getline(expected, nextExpectedLine);

      diff.leftLine  = line;
      diff.rightLine = line;
      ++line;

      if (nextFileLine != nextExpectedLine)
      {

          if      (file && !expected)
          {
            diff.reason = DifferenceInfo::LEFT_LARGER;
            while (file)
            {
              std::getline(file, nextFileLine);
              ++diff.leftLine;
            }
            return false;
          }
          else if (!file && expected)
          {
            diff.reason = DifferenceInfo::LEFT_SHORTER;
            while (expected)
            {
              std::getline(expected, nextExpectedLine);
              ++diff.rightLine;
            }
            return false;
          }

          diff.reason             = DifferenceInfo::DIFFER_AT_LINE;
          diff.leftLine           = line;
          diff.rightLine          = line;
          diff.leftDifferingLine  = nextFileLine;
          diff.rightDifferingLine = nextExpectedLine;

          return false;
      }

      // ---------------- Clear just read line because if next line is empty, they are not cleared automatically
      //
      nextFileLine.clear();
      nextExpectedLine.clear();
    }

    return true;
}
#endif


#if defined(_CXXTEST_HAVE_STD)
bool sameFiles(const char* file1, const char* file2, std::ostringstream & explanation)
{
    std::string curr_line("[1]: '"); // prefix is for line number
    std::string prev_line;
    std::string pprev_line;
    std::string ppprev_line;

    std::ifstream is1;
    is1.open (file1);
    std::ifstream is2;
    is2.open(file2);
    if (!is1)
    {
        explanation << "File '" << file1 << "' does not exist!";
        return false;
    }
    if (!is2)
    {
        explanation << "File '" << file2 << "' does not exist!";
        return false;
    }

    int nline = 1;
    std::string line1;
    std::string line2;
    while (true)
    {
        std::getline(is1, line1);
        std::getline(is2, line2);

        if (!is1 && !is2)
        {
          return true;
        }

        if (!is1)
        {
            explanation << "File '" << file1 << "' ended before file '" << file2 << "' (line " << nline << ")";

            // Previous lines context
            if (nline > 3)  explanation << "\n= [" << nline - 3 << "]: '" << ppprev_line << "'";
            if (nline > 2)  explanation << "\n= [" << nline - 2 << "]: '" << pprev_line  << "'";
            if (nline > 1)  explanation << "\n= [" << nline - 1 << "]: '" << prev_line   << "'";

            // Left side file
            explanation << "\n< [" << nline << "]: '" << line1 << "'";

            // Right side file
            explanation << "\n> [" << nline << "]: '" << line2 << "'";

            return false;
        }

        if (!is2)
        {
            explanation << "File '" << file1 << "' ended after file '" << file2 << "' (line " << nline << ")";

            // Previous lines context
            if (nline > 3)  explanation << "\n= [" << nline - 3 << "]: '" << ppprev_line << "'";
            if (nline > 2)  explanation << "\n= [" << nline - 2 << "]: '" << pprev_line  << "'";
            if (nline > 1)  explanation << "\n= [" << nline - 1 << "]: '" << prev_line   << "'";

            // Left side file
            explanation << "\n< [" << nline << "]: '" << line1 << "'";

            // Right side file
            explanation << "\n> [" << nline << "]: '" << line2 << "'";

            return false;
        }

        size_t length1 = line1.size();
        if ((length1 != 0) && (line1[length1 -1u] == '\r'))
        {
          line1.resize(length1 - 1u);
        }

        size_t length2 = line2.size();
        if ((length2 != 0) && (line2[length2 -1u] == '\r'))
        {
          line2.resize(length2 - 1u);
        }

        if (line1 != line2)
        {
            int ncol = 1;
            int maxCompareCol = std::min(line1.size(), line2.size());
            while (ncol <= maxCompareCol)
            {
              if (line1[ncol - 1u] != line2[ncol - 1u])
              {
                break;
              }
              ++ncol;
            }

            //+ Todo (JFC) (June/21/2017) : find column of difference !!!!
            //+
            explanation << "Files '" << file1 << "' and '" << file2 << "' differ at line: " << nline << ", col: " << ncol;

            // Previous lines context
            if (nline > 3)  explanation << "\n= [" << nline - 3 << "]: '" << ppprev_line << "'";
            if (nline > 2)  explanation << "\n= [" << nline - 2 << "]: '" << pprev_line  << "'";
            if (nline > 1)  explanation << "\n= [" << nline - 1 << "]: '" << prev_line   << "'";

            // Left side file
            explanation << "\n< [" << nline << "]: '" << line1 << "'";

            // Right side file
            explanation << "\n> [" << nline << "]: '" << line2 << "'";

            return false;
        }

        ++nline;

        ppprev_line = pprev_line;
        pprev_line  = prev_line;
        prev_line   = line1;
    }
}
#endif

void doAssertSameFiles(const char* file, int line, const char* file1, const char* file2, const char* message)
{
#if defined(_CXXTEST_HAVE_STD)
    std::ostringstream explanation;
    if (!sameFiles(file1, file2, explanation))
    {
        tracker().failedAssertSameFiles(message, file, line, file1, file2, explanation.str().c_str());
        TS_ABORT();
    }
    else
    {
      tracker().succeededAssertSameFiles(message, file, line, file1, file2);
    }
#else
    tracker().notSupportedAssert(message, file, line, "xxx_ASSERT_SAME_FILES", "--have-std is enabled");
    TS_ABORT();
#endif
}


void doAssertThrows(const char* file, int line, const char* expression, const char* message, const char* expectedExceptionType, const char* exceptionMessage, bool hasThrown, bool otherThrown)
{
    if (!hasThrown || otherThrown)
    {
      tracker().failedAssertThrows(message, file, line, expression, exceptionMessage, expectedExceptionType, otherThrown);
      TS_ABORT();
    }
    else
    {
      tracker().succeededAssertThrows(message, file, line, expression, exceptionMessage, expectedExceptionType);
    }
}


void doAssertThrowsNothing(const char* file, int line, const char* expression, const char* message, const char* exceptionMessage, bool hasThrown)
{
    if (hasThrown)
    {
      tracker().failedAssertThrowsNothing(message, file, line, expression, exceptionMessage);
      TS_ABORT();
    }
    else
    {
      tracker().succeededAssertThrowsNothing(message, file, line, expression);
    }
}


void doAssertThrowsAnything(const char* file, int line, const char* expression, const char* message, bool hasThrown)
{
    if (!hasThrown)
    {
      tracker().failedAssertThrowsAnything(message, file, line, expression);
      TS_ABORT();
    }
    else
    {
      tracker().succeededAssertThrowsAnything(message, file, line, expression);
    }
}


} // End of namespace CxxTest

#endif // __cxxtest__TestSuite_cpp__

