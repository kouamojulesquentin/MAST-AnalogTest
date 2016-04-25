//===========================================================================
//                           UT_NotAssertMacros.cpp
//===========================================================================
// Copyright © 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_NotAssertMacros.cpp
//!
//! Implements test fixture for testing not assertions macros
//!
//===========================================================================

#include "UT_NotAssertMacros.h"

UT_NotAssertMacros::UT_NotAssertMacros ()
{
}



UT_NotAssertMacros::~UT_NotAssertMacros ()
{
}


//! Runs TS_WARN
//!
void UT_NotAssertMacros::test_WARN ()
{
    TS_WARN("'This is expected message from test_WARN'");
}

//! Runs TS_TRACE
//!
void UT_NotAssertMacros::test_TRACE ()
{
    TS_TRACE("'This is expected message from test_TRACE'");
}

//! Runs TS_SKIP
//!
void UT_NotAssertMacros::test_SKIP ()
{
    TS_SKIP("'This is expected message from test_SKIP'");
}

//! Runs TS_SKIP without message
//!
void UT_NotAssertMacros::test_SKIP_Empty_Message ()
{
    TS_SKIP("");
}


//! Runs TS_FAIL
//!
void UT_NotAssertMacros::test_FAIL ()
{
    TS_FAIL("'This is TS_FAIL message'");
}



//! Checks that a test method that does not use any Cxxtest macro is remport as such.
//!
void UT_NotAssertMacros::test_EmptyTest ()
{
    // Nothing there deliberately
}


//===========================================================================
// End of UT_NotAssertMacros.cpp
//===========================================================================
