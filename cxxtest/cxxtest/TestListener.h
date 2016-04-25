//-------------------------------------------------------------------------
// CxxTest: A lightweight C++ unit testing library.
// Copyright (c) 2008 Sandia Corporation.
// This software is distributed under the LGPL License v3
// For more information, see the COPYING file in the top CxxTest directory.
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//-------------------------------------------------------------------------


#ifndef __cxxtest__TestListener_h__
#define __cxxtest__TestListener_h__

//
// TestListener is the base class for all "listeners",
// i.e. classes that receive notifications of the
// testing process.
//
// The names of the parameters are in comments to avoid
// "unused parameter" warnings.
//

#include <cxxtest/Descriptions.h>
#if defined(_CXXTEST_HAVE_STD)
#endif
#include <cstring>

namespace CxxTest
{
struct DifferenceInfo;

class TestListener
{
public:
    TestListener() : _pLastWorldDescription(0) {}
    virtual ~TestListener() {}
    virtual void process_commandline(int& /*argc*/, char** /*argv*/) {}

    virtual void enterSuite          (const SuiteDescription& /*desc*/)         {}
    virtual void enterTest           (const TestDescription&  /*desc*/)         {}
    virtual void enterDataDrivenTest (const TestDescription&  /*desc*/)         {}
    virtual void enterWorld          (const WorldDescription& worldDescription) { _pLastWorldDescription = &worldDescription; }

    virtual void leaveSuite          (const SuiteDescription&  /*desc*/) {}
    virtual void leaveDataDrivenTest (const TestDescription&   /*desc*/) {}
    virtual void leaveTest           (const TestDescription&   /*desc*/) {}
    virtual void leaveWorld          (const WorldDescription&) { _pLastWorldDescription = 0; }

    virtual void unexpectExceptionFromTest     (const char* /*exceptionType*/, const char* /*exceptionValue*/, const char* /*file*/, int /*line*/) = 0;
    virtual void trace                         (const char* /*message*/, const char* /*file*/, int /*line*/) = 0;
    virtual void warning                       (const char* /*message*/, const char* /*file*/, int /*line*/) = 0;
    virtual void skippedTest                   (const char* /*message*/, const char* /*file*/, int /*line*/) = 0;
    virtual void failedTest                    (const char* /*message*/, const char* /*file*/, int /*line*/) = 0;
    virtual void failedTestInternal            (const char* /*message*/, const char* /*file*/, int /*line*/) = 0;
    virtual void notSupportedAssert            (const char* /*message*/, const char* /*file*/, int /*line*/, const char* /*notSunotSupportedAssertNamepportedAssert*/, const char* /*requiredCondition*/) = 0;

    virtual void failedAssertTrue              (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*expression*/) = 0;
    virtual void failedAssertFalse             (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*expression*/) = 0;
    virtual void failedAssertNotNullptr        (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*expression*/) = 0;
    virtual void failedAssertNullptr           (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*expression*/) = 0;
    virtual void failedAssertEquals            (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*xStr*/, const char* /*yStr*/, const char* /*x*/, const char* /*y*/) = 0;
    virtual void failedAssertSameData          (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*xStr*/, const char* /*yStr*/, const char* /*sizeStr*/, const void * /*x*/, const void * /*y*/, unsigned /*size*/) = 0;
    virtual void failedAssertFileContent       (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*checkedFile*/, const char* /*expectedContent*/, const char* /*expectedContentExpr*/, const DifferenceInfo& /*diffInfo*/) = 0;
    virtual void failedAssertFileExists        (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*checkedFile*/) = 0;
    virtual void failedAssertFileNotExists     (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*checkedFile*/) = 0;
    virtual void failedAssertFileSize          (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*checkedFile*/, int /*expectedSize*/, int /*effectiveSize*/) = 0;
    virtual void failedAssertSameFiles         (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char*, const char*, const char*) = 0;
    virtual void failedAssertDelta             (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*xStr*/, const char* /*yStr*/, const char* /*dStr*/, const char* /*x*/, const char* /*y*/, const char* /*d*/) = 0;
    virtual void failedAssertDiffers           (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*xStr*/, const char* /*yStr*/, const char* /*value*/) = 0;
    virtual void failedAssertGreaterThan       (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*xStr*/, const char* /*yStr*/, const char* /*x*/, const char* /*y*/) = 0;
    virtual void failedAssertGreaterThanEquals (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*xStr*/, const char* /*yStr*/, const char* /*x*/, const char* /*y*/) = 0;
    virtual void failedAssertLessThan          (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*xStr*/, const char* /*yStr*/, const char* /*x*/, const char* /*y*/) = 0;
    virtual void failedAssertLessThanEquals    (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*xStr*/, const char* /*yStr*/, const char* /*x*/, const char* /*y*/) = 0;
    virtual void failedAssertPredicate         (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*predicate*/, const char* /*xStr*/, const char* /*x*/) = 0;
    virtual void failedAssertRelation          (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*relation*/, const char* /*xStr*/, const char* /*yStr*/, const char* /*x*/, const char* /*y*/) = 0;
    virtual void failedAssertThrows            (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*expression*/, const char* /*type*/, bool /*otherThrown*/) = 0;
    virtual void failedAssertThrowsNothing     (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*expression*/, const char* /*exceptionMessage*/) = 0;
    virtual void failedAssertThrowsAnything    (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*expression*/) = 0;


    // ---------------- Processing of successful asserts
    //
    #define SUCCEEDED_ASSERTIONS(Intention)   \
    virtual void succeededAssertTrue              (const char* /*assertionMessage*/, const char* /*file*/, int  /*line*/, const char* /*expression*/) Intention \
    virtual void succeededAssertFalse             (const char* /*assertionMessage*/, const char* /*file*/, int  /*line*/, const char* /*expression*/) Intention \
    virtual void succeededAssertNotNullptr        (const char* /*assertionMessage*/, const char* /*file*/, int  /*line*/, const char* /*expression*/) Intention \
    virtual void succeededAssertNullptr           (const char* /*assertionMessage*/, const char* /*file*/, int  /*line*/, const char* /*expression*/) Intention \
    virtual void succeededAssertDelta             (const char* /*assertionMessage*/, const char* /*file*/, int  /*line*/, const char* /*xStr*/,  const char* /*yStr*/, const char* /*dStr*/, const char* /*x*/, const char* /*y*/, const char* /*d*/) Intention \
    virtual void succeededAssertDiffers           (const char* /*assertionMessage*/, const char* /*file*/, int  /*line*/, const char* /*xStr*/,  const char* /*yStr*/, const char* /*x*/, const char* /*y*/) Intention \
    virtual void succeededAssertEquals            (const char* /*assertionMessage*/, const char* /*file*/, int  /*line*/, const char* /*xStr*/,  const char* /*yStr*/, const char* /*x*/, const char* /*y*/) Intention \
    virtual void succeededAssertFileContent       (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*checkedFile*/, const char* /*expectedContent*/, const char* /*expectedContentExpr*/, const DifferenceInfo& /*diffInfo*/) Intention \
    virtual void succeededAssertFileExists        (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*checkedFile*/) Intention \
    virtual void succeededAssertFileNotExists     (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*checkedFile*/) Intention \
    virtual void succeededAssertFileSize          (const char* /*assertionMessage*/, const char* /*file*/, int /*line*/, const char* /*checkedFile*/, int /*expectedSize*/, int /*effectiveSize*/) Intention \
    virtual void succeededAssertGreaterThan       (const char* /*assertionMessage*/, const char* /*file*/, int  /*line*/, const char* /*xStr*/,  const char* /*yStr*/, const char* /*x*/, const char* /*y*/) Intention \
    virtual void succeededAssertGreaterThanEquals (const char* /*assertionMessage*/, const char* /*file*/, int  /*line*/, const char* /*xStr*/,  const char* /*yStr*/, const char* /*x*/, const char* /*y*/) Intention \
    virtual void succeededAssertLessThan          (const char* /*assertionMessage*/, const char* /*file*/, int  /*line*/, const char* /*xStr*/,  const char* /*yStr*/, const char* /*x*/, const char* /*y*/) Intention \
    virtual void succeededAssertLessThanEquals    (const char* /*assertionMessage*/, const char* /*file*/, int  /*line*/, const char* /*xStr*/,  const char* /*yStr*/, const char* /*x*/, const char* /*y*/) Intention \
    virtual void succeededAssertPredicate         (const char* /*assertionMessage*/, const char* /*file*/, int  /*line*/, const char* /*pStr*/,  const char* /*xStr*/, const char* /*x*/) Intention \
    virtual void succeededAssertRelation          (const char* /*assertionMessage*/, const char* /*file*/, int  /*line*/, const char* /*rStr*/,  const char* /*xStr*/, const char* /*yStr*/, const char* /*x*/, const char* /*y*/) Intention \
    virtual void succeededAssertSameData          (const char* /*assertionMessage*/, const char* /*file*/, int  /*line*/, const char* /*xStr*/,  const char* /*yStr*/, const char* /*sizeStr*/, const void * /*x*/, const void * /*y*/, unsigned /*size*/) Intention \
    virtual void succeededAssertSameFiles         (const char* /*assertionMessage*/, const char* /*file*/, int  /*line*/, const char* /*file1*/, const char* /*file2*/) Intention \
    virtual void succeededAssertThrows            (const char* /*assertionMessage*/, const char* /*file*/, int  /*line*/, const char* /*expr*/,  const char* /*type*/, const char* /*exceptionMessage*/) Intention \
    virtual void succeededAssertThrowsNothing     (const char* /*assertionMessage*/, const char* /*file*/, int  /*line*/, const char* /*expr*/) Intention \
    virtual void succeededAssertThrowsAnything    (const char* /*assertionMessage*/, const char* /*file*/, int  /*line*/, const char* /*expr*/) Intention

    SUCCEEDED_ASSERTIONS(= 0;)
    #define IGNORE_SUCCEEDED_ASSERTIONS  SUCCEEDED_ASSERTIONS({})

    protected:

    //! Returns whether details of successful assertion should be added in the report
    //!
    //! @note This is mostly for debug purposes
    bool displaySuccess() const
    {
      return _pLastWorldDescription ? _pLastWorldDescription->displaySuccess() : false;
    }

    //! Removes the path part of a file path unless option to keep it is enabled
    //!
    const char* adaptSourceFilePath (const char* filePath) const
    {
      if (_pLastWorldDescription && !_pLastWorldDescription->displayFilePath())
      {
        const char* lastBackSlash = strrchr(filePath, '\\');
        if (!lastBackSlash)
        {
          lastBackSlash = strrchr(filePath, '/');
        }

        if (lastBackSlash)
        {
          filePath = ++lastBackSlash;
        }
      }
      return filePath;
    }
    //
    //  End of fct_adaptSourceFilePath
    //---------------------------------------------------------------------------

    private:
    const WorldDescription* _pLastWorldDescription; // Capture world description for
};
} // End of namespace CxxTest


#endif // __cxxtest__TestListener_h__

