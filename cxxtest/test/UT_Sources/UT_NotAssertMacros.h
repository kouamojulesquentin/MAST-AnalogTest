//===========================================================================
//                           UT_NotAssertMacros.h
//===========================================================================
// Copyright (C) 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_NotAssertMacros.h
//!
//! Declares test fixture for testing not assertions macros
//!
//===========================================================================


#ifndef UT_NOTASSERTMACROS_HPP
#define UT_NOTASSERTMACROS_HPP

#include <cxxtest/TestSuite.h>
#include <cxxtest/ValueTraits.h>

//! Runs cxxtest, not asserts, macros
//!
//! This is intended to check the report with some "gold" results
//!
class UT_NotAssertMacros : public CxxTest::TestSuite
{
    public:
     UT_NotAssertMacros();
    ~UT_NotAssertMacros();

    // #========================================================================================================
    // # Test Methods
    // #========================================================================================================

    // ---------------- Macros without assertion
    //
    void test_WARN ();
    void test_TRACE ();
    void test_SKIP ();
    void test_SKIP_Empty_Message ();
    void test_FAIL ();

    // ---------------- No Cxxtest macro
    //
    void test_EmptyTest ();

};
#endif  // UT_NOTASSERTMACROS_HPP


//===========================================================================
// End of UT_NotAssertMacros.h
//===========================================================================
