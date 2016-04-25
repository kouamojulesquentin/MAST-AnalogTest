//===========================================================================
//                           UT_UnexpectedExceptions.h
//===========================================================================
// Copyright (C) 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_UnexpectedExceptions.h
//!
//! Declares test fixture for testing unexpected exceptions
//===========================================================================

#ifndef UT_UNEXPECTEDEXCEPTIONS_HPP
#define UT_UNEXPECTEDEXCEPTIONS_HPP

#include <cxxtest/TestSuite.h>
#include <cxxtest/ValueTraits.h>

//! Runs cxxtest asserts macro while an unexpected exception occurs (inside or outside not exception related asserts)
//!
//! This is intended to check the report with some "gold" results
//!
class UT_UnexpectedExceptions : public CxxTest::TestSuite
{
    public:
     UT_UnexpectedExceptions();
    ~UT_UnexpectedExceptions();

    // #========================================================================================================
    // # Test Methods
    // #========================================================================================================

    // ---------------- Unexpected exception WITHIN ASSERTs
    //
    void test_Unexpected_Within_ASSERT ();
    void test_Unexpected_Within_ASSERT_FALSE ();
    void test_Unexpected_Within_ASSERT_EQUALS ();
    void test_Unexpected_Within_ASSERT_EQUALS_AUTO_Value ();
    void test_Unexpected_Within_ASSERT_EQUALS_AUTO_Pointer ();
    void test_Unexpected_Within_ASSERT_DIFFERS ();
    void test_Unexpected_Within_ASSERT_GREATER_THAN ();
    void test_Unexpected_Within_ASSERT_GREATER_THAN_EQUAL ();
    void test_Unexpected_Within_ASSERT_LESS_THAN ();
    void test_Unexpected_Within_ASSERT_LESS_THAN_EQUAL ();
    void test_Unexpected_Within_ASSERT_DELTA ();
    void test_Unexpected_Within_ASSERT_PREDICATE ();
    void test_Unexpected_Within_ASSERT_RELATION ();
    void test_Unexpected_Within_ASSERT_SAME_DATA ();

    // ---------------- Unexpected exception WITHIN message versions
    //
    void test_Unexpected_Within_Message_ASSERT ();
    void test_Unexpected_Within_Message_ASSERT_FALSE ();
    void test_Unexpected_Within_Message_ASSERT_EQUALS ();
    void test_Unexpected_Within_Message_ASSERT_EQUALS_AUTO_Value ();
    void test_Unexpected_Within_Message_ASSERT_EQUALS_AUTO_Pointer ();
    void test_Unexpected_Within_Message_ASSERT_DIFFERS ();
    void test_Unexpected_Within_Message_ASSERT_GREATER_THAN ();
    void test_Unexpected_Within_Message_ASSERT_GREATER_THAN_EQUAL ();
    void test_Unexpected_Within_Message_ASSERT_LESS_THAN ();
    void test_Unexpected_Within_Message_ASSERT_LESS_THAN_EQUAL ();
    void test_Unexpected_Within_Message_ASSERT_DELTA ();
    void test_Unexpected_Within_Message_ASSERT_PREDICATE ();
    void test_Unexpected_Within_Message_ASSERT_RELATION ();
    void test_Unexpected_Within_Message_ASSERT_SAME_DATA ();

    // ---------------- Unexpected exception BEFORE ASSERTs
    //
    void test_Unexpected_Before_ASSERT ();
    void test_Unexpected_Before_ASSERT_FALSE ();
    void test_Unexpected_Before_ASSERT_EQUALS ();
    void test_Unexpected_Before_ASSERT_EQUALS_AUTO_Value ();
    void test_Unexpected_Before_ASSERT_EQUALS_AUTO_Pointer ();
    void test_Unexpected_Before_ASSERT_DIFFERS ();
    void test_Unexpected_Before_ASSERT_GREATER_THAN ();
    void test_Unexpected_Before_ASSERT_GREATER_THAN_EQUAL ();
    void test_Unexpected_Before_ASSERT_LESS_THAN ();
    void test_Unexpected_Before_ASSERT_LESS_THAN_EQUAL ();
    void test_Unexpected_Before_ASSERT_DELTA ();
    void test_Unexpected_Before_ASSERT_PREDICATE ();
    void test_Unexpected_Before_ASSERT_RELATION ();
    void test_Unexpected_Before_ASSERT_SAME_DATA ();

    // ---------------- Unexpected exception AFTER ASSERTs
    //
    void test_Unexpected_After_ASSERT ();
    void test_Unexpected_After_ASSERT_FALSE ();
    void test_Unexpected_After_ASSERT_EQUALS ();
    void test_Unexpected_After_ASSERT_EQUALS_AUTO_Value ();
    void test_Unexpected_After_ASSERT_EQUALS_AUTO_Pointer ();
    void test_Unexpected_After_ASSERT_DIFFERS ();
    void test_Unexpected_After_ASSERT_GREATER_THAN ();
    void test_Unexpected_After_ASSERT_GREATER_THAN_EQUAL ();
    void test_Unexpected_After_ASSERT_LESS_THAN ();
    void test_Unexpected_After_ASSERT_LESS_THAN_EQUAL ();
    void test_Unexpected_After_ASSERT_DELTA ();
    void test_Unexpected_After_ASSERT_PREDICATE ();
    void test_Unexpected_After_ASSERT_RELATION ();
    void test_Unexpected_After_ASSERT_SAME_DATA ();

    // ---------------- Unexpected exception WITHIN message versions
    //
    void test_Unexpected_Within_and_After_ASSERT ();
    void test_Unexpected_Within_and_After_ASSERT_FALSE ();
    void test_Unexpected_Within_and_After_ASSERT_EQUALS ();
    void test_Unexpected_Within_and_After_ASSERT_EQUALS_AUTO_Value ();
    void test_Unexpected_Within_and_After_ASSERT_EQUALS_AUTO_Pointer ();
    void test_Unexpected_Within_and_After_ASSERT_DIFFERS ();
    void test_Unexpected_Within_and_After_ASSERT_GREATER_THAN ();
    void test_Unexpected_Within_and_After_ASSERT_GREATER_THAN_EQUAL ();
    void test_Unexpected_Within_and_After_ASSERT_LESS_THAN ();
    void test_Unexpected_Within_and_After_ASSERT_LESS_THAN_EQUAL ();
    void test_Unexpected_Within_and_After_ASSERT_DELTA ();
    void test_Unexpected_Within_and_After_ASSERT_PREDICATE ();
    void test_Unexpected_Within_and_After_ASSERT_RELATION ();
    void test_Unexpected_Within_and_After_ASSERT_SAME_DATA ();


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
    int         getInt_Throwing();
    const int*  getIntPointer_Throwing();
    double      getDouble_Throwing();
    const char* getString_Throwing();

    // #========================================================================================================
    // # Private  Fields
    // #========================================================================================================
    private:
};
#endif  // UT_UNEXPECTEDEXCEPTIONS_HPP

//===========================================================================
// End of UT_UnexpectedExceptions.h
//===========================================================================
