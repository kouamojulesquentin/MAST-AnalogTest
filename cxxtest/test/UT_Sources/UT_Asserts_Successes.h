//===========================================================================
//                           UT_Asserts_Successes.h
//===========================================================================
// Copyright (C) 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_Asserts_Successes.h
//!
//! Declares test fixture for testing asserts macros that should be successful
//===========================================================================


#ifndef UT_ASSERTS_SUCCESSES_HPP
#define UT_ASSERTS_SUCCESSES_HPP

#include <cxxtest/TestSuite.h>
#include <cxxtest/ValueTraits.h>

//! Runs cxxtest, not exception related, asserts macros that should be successful
//!
//! This is intended to check the report with some "gold" results
//!
class UT_Asserts_Successes : public CxxTest::TestSuite
{
    public:
     UT_Asserts_Successes();
    ~UT_Asserts_Successes();

    // #========================================================================================================
    // # Test Methods
    // #========================================================================================================

    // ---------------- Assertions with success (without message)
    //
    void test_ASSERT ();
    void test_ASSERT_FALSE ();
    void test_ASSERT_EQUALS ();
    void test_ASSERT_EQUALS_Int_Signed_Unsigned ();
    void test_ASSERT_EQUALS_Int_Unsigned_unsigned ();
    void test_ASSERT_EQUALS_Short_Signed_Unsigned ();
    void test_ASSERT_EQUALS_Short_Unsigned_unsigned ();
    void test_ASSERT_EQUALS_Char_Plain_Unsigned ();
    void test_ASSERT_EQUALS_Char_Unsigned_Plain ();
    void test_ASSERT_EQUALS_Char_Signed_Unsigned ();
    void test_ASSERT_EQUALS_Char_Unsigned_Signed ();
    void test_ASSERT_EQUALS_Char_Plain_Short_Unsigned ();
    void test_ASSERT_EQUALS_Short_Signed_Char_Plain ();
    void test_ASSERT_EQUALS_Char_Signed_Short_Unsigned ();
    void test_ASSERT_EQUALS_Short_Signed_Char_Unsigned ();
    void test_ASSERT_EQUALS_Char_Plain_Int_Unsigned ();
    void test_ASSERT_EQUALS_Int_Signed_Char_Plain ();
    void test_ASSERT_EQUALS_Char_Signed_Int_Unsigned ();
    void test_ASSERT_EQUALS_Int_Signed_Char_Unsigned ();
    void test_ASSERT_EQUALS_Char_Arrays ();
    void test_ASSERT_EQUALS_Char_Pointers_const_const ();
    void test_ASSERT_EQUALS_Char_Pointers_const_notconst ();
    void test_ASSERT_EQUALS_Char_Pointers_notconst_const ();
    void test_ASSERT_EQUALS_Char_Pointers_notconst_notconst ();
    void test_ASSERT_EQUALS_Char_ArrayPtr ();
    void test_ASSERT_EQUALS_Char_PtrArray ();
    void test_ASSERT_EQUALS_AUTO_Value ();
    void test_ASSERT_EQUALS_AUTO_Pointer ();
    void test_ASSERT_DIFFERS ();
    void test_ASSERT_DIFFERS_Char_Arrays ();
    void test_ASSERT_DIFFERS_Char_Pointers ();
    void test_ASSERT_DIFFERS_Char_ArrayPtr ();
    void test_ASSERT_DIFFERS_Char_PtrArray ();
    void test_ASSERT_GREATER_THAN_GreaterThan ();
    void test_ASSERT_GREATER_THAN_EQUAL_GreaterThan ();
    void test_ASSERT_GREATER_THAN_EQUAL_Equal ();
    void test_ASSERT_LESS_THAN_LessThan ();
    void test_ASSERT_LESS_THAN_EQUAL_LessThan ();
    void test_ASSERT_LESS_THAN_EQUAL_Equal ();
    void test_ASSERT_DELTA ();
    void test_ASSERT_PREDICATE ();
    void test_ASSERT_RELATION ();
    void test_ASSERT_SAME_DATA ();

    void test_ASSERT_EMPTY_CharArray ();
    void test_ASSERT_EMPTY_C_String ();
    void test_ASSERT_EMPTY_NullPtr ();
    void test_ASSERT_EMPTY_String ();
    void test_ASSERT_EMPTY_Vector ();
    void test_ASSERT_EMPTY_Map ();

    void test_ASSERT_NOT_EMPTY_CharArray ();
    void test_ASSERT_NOT_EMPTY_C_String ();
    void test_ASSERT_NOT_EMPTY_String ();
    void test_ASSERT_NOT_EMPTY_Vector ();
    void test_ASSERT_NOT_EMPTY_Map ();

    // ---------------- Assertions with success (with message)
    //
    void test_ASSERT_MESSAGE ();
    void test_ASSERT_FALSE_MESSAGE ();
    void test_ASSERT_MESSAGE_EQUALS ();
    void test_ASSERT_MESSAGE_EQUALS_AUTO_Value ();
    void test_ASSERT_MESSAGE_EQUALS_AUTO_Pointer ();
    void test_ASSERT_MESSAGE_DIFFERS ();
    void test_ASSERT_MESSAGE_GREATER_THAN_GreaterThan ();
    void test_ASSERT_MESSAGE_GREATER_THAN_EQUAL_GreaterThan ();
    void test_ASSERT_MESSAGE_GREATER_THAN_EQUAL_Equal ();
    void test_ASSERT_MESSAGE_LESS_THAN_LessThan ();
    void test_ASSERT_MESSAGE_LESS_THAN_EQUAL_LessThan ();
    void test_ASSERT_MESSAGE_LESS_THAN_EQUAL_Equal ();
    void test_ASSERT_MESSAGE_DELTA ();
    void test_ASSERT_MESSAGE_PREDICATE ();
    void test_ASSERT_MESSAGE_RELATION ();
    void test_ASSERT_MESSAGE_SAME_DATA ();

    // ---------------- Assertions checking exceptions with success (without message)
    //
    void test_ASSERT_THROWS ();
    void test_ASSERT_THROWS_ANYTHING ();
    void test_ASSERT_THROWS_NOTHING ();

    // ---------------- Assertions checking exceptions with success (with message)
    //
    void test_ASSERT_MESSAGE_THROWS ();
    void test_ASSERT_MESSAGE_THROWS_ANYTHING ();
    void test_ASSERT_MESSAGE_THROWS_NOTHING ();

    void test_ASSERT_MESSAGE_EMPTY ();
    void test_ASSERT_MESSAGE_NOT_EMPTY ();

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
#endif  // UT_ASSERTS_SUCCESSES_HPP

//===========================================================================
// End of UT_Asserts_Successes.h
//===========================================================================
