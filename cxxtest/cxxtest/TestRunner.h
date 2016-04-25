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

#ifndef __cxxtest_TestRunner_h__
#define __cxxtest_TestRunner_h__

//
// TestRunner is the class that runs all the tests.
// To use it, create an object that implements the TestListener
// interface and call TestRunner::runAllTests( myListener );
//

#include <cxxtest/TestListener.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestSuite.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestStateGuard.h>

namespace CxxTest
{
class TestRunner
{
public:

    static void setListener(TestListener* listener)
    {
        tracker().setListener(listener);
    }

    static void runAllTests(TestListener &listener)
    {
        tracker().setListener(&listener);
        _TS_TRY { TestRunner().runWorld(); }
        _TS_LAST_CATCH( { tracker().failedTestInternal("Exception thrown from world", __FILE__, __LINE__); });
        tracker().setListener(0);
    }

    static void runAllTests(TestListener *listener)
    {
        if (listener)
        {
            listener->warning("Deprecated; Use runAllTests( TestListener & )", __FILE__, __LINE__);
            runAllTests(*listener);
        }
    }

private:
    void runWorld()
    {
        RealWorldDescription wd;
        WorldGuard sg;

        tracker().enterWorld(wd);
        if (wd.setUp())
        {
            for (SuiteDescription *sd = wd.firstSuite(); sd; sd = sd->next())
            {
                if (sd->active())
                {
                    runSuite(*sd);
                }
            }

            wd.tearDown();
        }
        tracker().leaveWorld(wd);
    }

    void runSuite(SuiteDescription &sd)
    {
        TestStateGuard sg;

        tracker().enterSuite(sd);
        if (sd.setUp())
        {
            for (TestDescription *td = sd.firstTest(); td; td = td->next())
            {
                if (td->active())
                {
                    runTest(*td);
                }
            }

            sd.tearDown();
        }
        tracker().leaveSuite(sd);
    }

    void runTest(TestDescription &td)
    {
        TestStateGuard sg;

        tracker().enterTest(td);
        if (td.setUp())
        {
            _TS_TRY           { td.run(); }
            _TS_CATCH_SKIPPED ({})    // Occurs when TS_SKIP is executed
            _TS_CATCH_ABORT   ({})    // Occurs in case of test failure and abortTestOnFail() is true
            _TS_CATCH_UNEXPECTED(td.file(), td.line());
            td.tearDown();
        }
        tracker().leaveTest(td);
    }


    class WorldGuard : public TestStateGuard
    {
    public:
        WorldGuard() : TestStateGuard()
        {
#ifdef _CXXTEST_HAVE_EH
            setAbortTestOnFail(CXXTEST_DEFAULT_ABORT);
#endif // _CXXTEST_HAVE_EH
            setMaxDumpSize(CXXTEST_MAX_DUMP_SIZE);
        }
    };
};

//
// For --no-static-init
//
void initialize();
}


#endif // __cxxtest_TestRunner_h__
