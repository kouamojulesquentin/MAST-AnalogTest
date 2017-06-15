//===========================================================================
//                           UT_Asserts_Failures.h
//===========================================================================
// Copyright (C) 2015. All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_Asserts_Failures.h
//!
//! Declares test fixture for testing asserts macros that should report failure
//===========================================================================


#ifndef UT_ASSERTS_FAILURES_HPP
#define UT_ASSERTS_FAILURES_HPP

#include <cxxtest/TestSuite.h>
#include <cxxtest/ValueTraits.h>

//! Runs cxxtest, not exception related, asserts macros that should report failure
//!
//! This is intended to check the report with some "gold" results
//!
class UT_Asserts_Failures : public CxxTest::TestSuite
{
    public:
     UT_Asserts_Failures();
    ~UT_Asserts_Failures();

    // #========================================================================================================
    // # Test Methods
    // #========================================================================================================

    // ---------------- Assertions with failure (without message)
    //
    void test_ASSERT ();
    void test_ASSERT_FALSE ();
    void test_ASSERT_EQUALS ();
    void test_ASSERT_EQUALS_CString ();
    void test_ASSERT_EQUALS_Char_Pointers_const_const ();
    void test_ASSERT_EQUALS_Char_Pointers_const_notconst ();
    void test_ASSERT_EQUALS_Char_Pointers_notconst_const ();
    void test_ASSERT_EQUALS_Char_Pointers_notconst_notconst ();
    void test_ASSERT_EQUALS_AUTO_Value ();
    void test_ASSERT_EQUALS_AUTO_Pointer ();
    void test_ASSERT_DIFFERS ();
    void test_ASSERT_GREATER_THAN_Equal ();
    void test_ASSERT_GREATER_THAN_EQUAL_GreaterThan ();
    void test_ASSERT_LESS_THAN_Equal ();
    void test_ASSERT_LESS_THAN_EQUAL_GreaterThan ();
    void test_ASSERT_DELTA ();
    void test_ASSERT_PREDICATE ();
    void test_ASSERT_RELATION ();
    void test_ASSERT_SAME_DATA ();

    void test_ASSERT_EMPTY_CharArray ();
    void test_ASSERT_EMPTY_C_String ();
    void test_ASSERT_EMPTY_String ();
    void test_ASSERT_EMPTY_Vector ();
    void test_ASSERT_EMPTY_Map ();

    // ---------------- Assertions with failure (with message)
    //
    void test_ASSERT_MESSAGE ();
    void test_ASSERT_FALSE_MESSAGE ();
    void test_ASSERT_MESSAGE_EQUALS ();
    void test_ASSERT_MESSAGE_EQUALS_CString ();
    void test_ASSERT_MESSAGE_EQUALS_AUTO_Value ();
    void test_ASSERT_MESSAGE_EQUALS_AUTO_Pointer ();
    void test_ASSERT_MESSAGE_DIFFERS ();
    void test_ASSERT_MESSAGE_GREATER_THAN_Equal ();
    void test_ASSERT_MESSAGE_GREATER_THAN_EQUAL_GreaterThan ();
    void test_ASSERT_MESSAGE_LESS_THAN_Equal ();
    void test_ASSERT_MESSAGE_LESS_THAN_EQUAL_GreaterThan ();
    void test_ASSERT_MESSAGE_DELTA ();
    void test_ASSERT_MESSAGE_PREDICATE ();
    void test_ASSERT_MESSAGE_RELATION ();
    void test_ASSERT_MESSAGE_SAME_DATA ();


    // ---------------- Assertions checking exceptions with failure Assertions with failure (without message)
    //
    void test_ASSERT_THROWS ();
    void test_ASSERT_THROWS_ANYTHING ();
    void test_ASSERT_THROWS_NOTHING ();

    // ---------------- Assertions checking exceptions with failure Assertions with failure (withmessage)
    //
    void test_ASSERT_MESSAGE_THROWS ();
    void test_ASSERT_MESSAGE_THROWS_ANYTHING ();
    void test_ASSERT_MESSAGE_THROWS_NOTHING ();

    void test_ASSERT_MESSAGE_EMPTY ();

    //      TS_ASSERT_THROWS_ASSERT(expr,arg,assertion)
    //      TS_ASSERT_THROWS_EQUALS(expr,arg,x,y)
    //      TS_ASSERT_THROWS_IS_INFINITE(expr,arg,x)
    //      TS_ASSERT_THROWS_IS_NAN(expr,arg,x)


    //! Initializes test (called for each test)
    //!
    void setUp();

    //! Cleanups test (called for each test)
    //!
    void tearDown();

    // #========================================================================================================
    // # Private  Methods
    // #========================================================================================================
    private:
    int  incrementIntCounter();
    void throwException();

    // #========================================================================================================
    // # Private  Fields
    // #========================================================================================================
    private:
    int m_counter_int;      // This is for testing side effects of macros (should be as if it were a function)
};
#endif  // UT_ASSERTS_FAILURES_HPP

//===========================================================================
// End of UT_Asserts_Failures.h
//===========================================================================
