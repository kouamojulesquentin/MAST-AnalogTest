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

#ifndef __CXXTEST__GUI_H
#define __CXXTEST__GUI_H

//
// GuiListener is a simple base class for the differes GUIs
// GuiTuiRunner<GuiT, TuiT> combines a GUI with a text-mode error formatter
//

#include <cxxtest/TeeListener.h>

namespace CxxTest
{
  struct DifferenceInfo;

class GuiListener : public TestListener
{
public:
    GuiListener() : _state(GREEN_BAR) {}
    virtual ~GuiListener() {}

    virtual void runGui(int &argc, char **argv, TestListener &listener)
    {
        enterGui(argc, argv);
        TestRunner::runAllTests(listener);
        leaveGui();
    }

    virtual void enterGui(int & /*argc*/, char ** /*argv*/) {}
    virtual void leaveGui() {}

    //
    // The easy way is to implement these functions:
    //
    virtual void guiEnterWorld(unsigned /*numTotalTests*/) {}
    virtual void guiEnterSuite(const char * /*suiteName*/) {}
    virtual void guiEnterTest(const char * /*suiteName*/, const char * /*testName*/) {}
    virtual void yellowBar() {}
    virtual void redBar() {}

    //
    // The hard way is this:
    //
    virtual void enterWorld(const WorldDescription &desc) { TestListener::enterWorld(desc);  guiEnterWorld(desc.numTotalTests ()); }
    virtual void enterSuite(const SuiteDescription &desc) { guiEnterSuite (desc.suiteName()); }
    virtual void enterTest (const TestDescription  &desc) { guiEnterTest  (desc.suiteName(), desc.testName()); }
    virtual void leaveTest (const TestDescription  &) { if (tracker().testHasNoAssertion()) yellowBarSafe(); }
    virtual void leaveSuite(const SuiteDescription &) {}
    virtual void leaveWorld(const WorldDescription &) {}

    virtual void trace(const char* /* message */, const char * /*file*/, int /*line*/)
    {
    }

    virtual void warning(const char* /* message */, const char * /*file*/, int /*line*/)
    {
        yellowBarSafe();
    }

    virtual void skippedTest(const char* /* message */, const char * /*file*/, int /*line*/)
    {
        yellowBarSafe();
    }

    virtual void failedTest(const char* /* message */, const char * /*file*/, int /*line*/)
    {
        redBarSafe();
    }

    virtual void failedTestInternal(const char* /* message */, const char * /*file*/, int /*line*/)
    {
        redBarSafe();
    }

    virtual void notSupportedAssert (const char* /*message*/, const char* /*file*/, int /*line*/, const char* /*notSupportedAssertName*/, const char* /*requiredCondition*/)
    {
        redBarSafe();
    }

    virtual void unexpectExceptionFromTest (const char* /*exceptionType*/, const char* /*exceptionValue*/, const char* /*file*/, int /*line*/)
    {
        redBarSafe();
    }


    virtual void failedAssertTrue(const char* /* message */, const char * /*file*/, int /*line*/, const char * /*expression*/)
    {
        redBarSafe();
    }

    virtual void failedAssertFalse(const char* /* message */, const char * /*file*/, int /*line*/, const char * /*expression*/)
    {
        redBarSafe();
    }

    virtual void failedAssertNullptr(const char* /* message */, const char * /*file*/, int /*line*/, const char * /*expression*/)
    {
        redBarSafe();
    }

    virtual void failedAssertNotNullptr(const char* /* message */, const char * /*file*/, int /*line*/, const char * /*expression*/)
    {
        redBarSafe();
    }

    virtual void failedAssertEmpty (const char* /*message*/, const char* /*file*/, int /*line*/, const char* /*containerExpr*/, const char* /*containerContent*/)
    {
      redBarSafe();
    }


    virtual void failedAssertEquals(const char* /* message */, const char * /*file*/, int /*line*/,
                            const char * /*xStr*/, const char * /*yStr*/,
                            const char * /*x*/, const char * /*y*/)
    {
        redBarSafe();
    }

    virtual void failedAssertSameData(const char* /* message */, const char * /*file*/, int /*line*/,
                              const char * /*xStr*/, const char * /*yStr*/,
                              const char * /*sizeStr*/, const void * /*x*/,
                              const void * /*y*/, unsigned /*size*/)
    {
        redBarSafe();
    }

    virtual void failedAssertSameFiles(const char* /* message */, const char * /*file*/, int /*line*/,
                              const char * /*xStr*/, const char * /*yStr*/,
                              const char * /*sizeStr*/, const void * /*x*/,
                              const void * /*y*/, unsigned /*size*/)
    {
        redBarSafe();
    }


    virtual void failedAssertFileContent (const char* /*message*/, const char* /*file*/, int /*line*/, const char* /*checkedFile*/, const char* /*expectedContent*/, const char* /*expectedContentExpr*/, const DifferenceInfo& /*diffInfo*/)
    {
        redBarSafe();
    }

    virtual void failedAssertFileExists(const char* /* message */, const char* /*file*/, int /*line*/, const char* /*checkedFile*/)
    {
        redBarSafe();
    }


    virtual void failedAssertFileNotExists(const char* /* message */, const char* /*file*/, int /*line*/, const char* /*checkedFile*/)
    {
        redBarSafe();
    }

    virtual void failedAssertFileSize  (const char* /*message*/, const char* /*file*/, int /*line*/, const char* /*checkedFile*/, int /*expectedSize*/, int /*effectiveSize*/)
    {
        redBarSafe();
    }


    virtual void failedAssertSameFiles(const char* /* message */, const char * /*file*/, int /*line*/,
                               const char*, const char*,
                               const char* /* explanation */
                              )
    {
      redBarSafe();
    }



    virtual void failedAssertDelta(const char* /* message */, const char * /*file*/, int /*line*/,
                           const char * /*xStr*/, const char * /*yStr*/, const char * /*dStr*/,
                           const char * /*x*/, const char * /*y*/, const char * /*d*/)
    {
        redBarSafe();
    }

    virtual void failedAssertDiffers(const char* /* message */, const char * /*file*/, int /*line*/,
                             const char * /*xStr*/, const char * /*yStr*/,
                             const char * /*value*/)
    {
        redBarSafe();
    }

    virtual void failedAssertLessThan(const char* /* message */, const char * /*file*/, int /*line*/,
                              const char * /*xStr*/, const char * /*yStr*/,
                              const char * /*x*/, const char * /*y*/)
    {
        redBarSafe();
    }

    virtual void failedAssertLessThanEquals(const char* /* message */, const char * /*file*/, int /*line*/,
                                    const char * /*xStr*/, const char * /*yStr*/,
                                    const char * /*x*/, const char * /*y*/)
    {
        redBarSafe();
    }

    virtual void failedAssertGreaterThan(const char* /* message */, const char * /*file*/, int /*line*/,
                              const char * /*xStr*/, const char * /*yStr*/,
                              const char * /*x*/, const char * /*y*/)
    {
        redBarSafe();
    }

    virtual void failedAssertGreaterThanEquals(const char* /* message */, const char * /*file*/, int /*line*/,
                                    const char * /*xStr*/, const char * /*yStr*/,
                                    const char * /*x*/, const char * /*y*/)
    {
        redBarSafe();
    }


    virtual void failedAssertPredicate(const char* /* message */, const char * /*file*/, int /*line*/,
                               const char * /*predicate*/, const char * /*xStr*/, const char * /*x*/)
    {
        redBarSafe();
    }

    virtual void failedAssertRelation(const char* /* message */, const char * /*file*/, int /*line*/,
                              const char * /*relation*/, const char * /*xStr*/, const char * /*yStr*/,
                              const char * /*x*/, const char * /*y*/)
    {
        redBarSafe();
    }

    virtual void failedAssertThrows(const char* /* message */,
                                    const char* /*file*/,
                                    int         /*line*/,
                                    const char* /*expression*/,
                                    const char* /*exceptionMessage*/,
                                    const char* /*type*/,
                                    bool        /*otherThrown*/)
    {
        redBarSafe();
    }

    virtual void failedAssertThrowsNothing(const char* /* message */, const char * /*file*/, int /*line*/, const char * /*expression*/, const char* /*exceptionMessage*/)
    {
        redBarSafe();
    }

    virtual void failedAssertThrowsAnything(const char* /* message */, const char * /*file*/, int /*line*/, const char * /*expression*/)
    {
        redBarSafe();
    }


    IGNORE_SUCCEEDED_ASSERTIONS

protected:
    void yellowBarSafe()
    {
        if (_state < YELLOW_BAR)
        {
            yellowBar();
            _state = YELLOW_BAR;
        }
    }

    void redBarSafe()
    {
        if (_state < RED_BAR)
        {
            redBar();
            _state = RED_BAR;
        }
    }

private:
    enum { GREEN_BAR, YELLOW_BAR, RED_BAR } _state;
};

template<class GuiT, class TuiT>
class GuiTuiRunner : public TeeListener
{
    int* _argc;
    char **_argv;
    GuiT _gui;
    TuiT _tui;

public:
    GuiTuiRunner() : _argc(0), _argv(0) {}

    void process_commandline(int& argc, char** argv)
    {
        _argc = &argc;
        _argv = argv;
        setFirst(_gui);
        setSecond(_tui);
    }

    int run()
    {
        _gui.runGui(*_argc, _argv, *this);
        return tracker().failedTests();
    }
};
}

#endif //__CXXTEST__GUI_H

