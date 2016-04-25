//===========================================================================
//                           UT_ValueTraits.h
//===========================================================================
// Copyright (C) 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_ValueTraits.h
//!
//! Declares test fixture for testing 'standard' value traits
//!
//===========================================================================

#ifndef UT_VALUETRAITS_HPP
#define UT_VALUETRAITS_HPP

#include <cxxtest/TestSuite.h>
#include <cxxtest/ValueTraits.h>

//! Unit tests for CxxTest framework ValueTraits<T> class
//!
//! @note   Runs cxxtest asserts that failed for value types for which their is a ValueTraits specialization.
//!
//! This is intended to check the report with some "gold" results
//!
class UT_ValueTraits : public CxxTest::TestSuite
{
    public:
     UT_ValueTraits();
    ~UT_ValueTraits();

    // #========================================================================================================
    // # Test Methods
    // #========================================================================================================

    // ---------------- char variants
    //
    void test_ValueTraits_With_char ();
    void test_ValueTraits_With_const_char ();
    void test_ValueTraits_With_signed_char ();
    void test_ValueTraits_With_const_signed_char ();
    void test_ValueTraits_With_unsigned_char ();
    void test_ValueTraits_With_const_unsigned_char ();

    // ---------------- char* variants
    //
    void test_ValueTraits_With_char_Pointer ();
    void test_ValueTraits_With_const_char_Pointer ();
    void test_ValueTraits_With_signed_char_Pointer ();
    void test_ValueTraits_With_const_signed_char_Pointer ();
    void test_ValueTraits_With_unsigned_char_Pointer ();
    void test_ValueTraits_With_const_unsigned_char_Pointer ();

    // ---------------- int variants
    //
    void test_ValueTraits_With_int ();
    void test_ValueTraits_With_const_int ();
    void test_ValueTraits_With_signed_int ();
    void test_ValueTraits_With_const_signed_int ();
    void test_ValueTraits_With_unsigned_int ();
    void test_ValueTraits_With_const_unsigned_int ();

    // ---------------- int* variants
    //
    void test_ValueTraits_With_int_Pointer ();
    void test_ValueTraits_With_const_int_Pointer ();
    void test_ValueTraits_With_signed_int_Pointer ();
    void test_ValueTraits_With_const_signed_int_Pointer ();
    void test_ValueTraits_With_unsigned_int_Pointer ();
    void test_ValueTraits_With_const_unsigned_int_Pointer ();

    // ---------------- long int variants
    //
    void test_ValueTraits_With_long_int ();
    void test_ValueTraits_With_const_long_int ();
    void test_ValueTraits_With_signed_long_int ();
    void test_ValueTraits_With_const_signed_long_int ();
    void test_ValueTraits_With_unsigned_long_int ();
    void test_ValueTraits_With_const_unsigned_long_int ();

    // ---------------- long int* variants
    //
    void test_ValueTraits_With_long_int_Pointer ();
    void test_ValueTraits_With_const_long_int_Pointer ();
    void test_ValueTraits_With_signed_long_int_Pointer ();
    void test_ValueTraits_With_const_signed_long_int_Pointer ();
    void test_ValueTraits_With_unsigned_long_int_Pointer ();
    void test_ValueTraits_With_const_unsigned_long_int_Pointer ();

    // ---------------- long long variants
    //
    void test_ValueTraits_With_long_long ();
    void test_ValueTraits_With_const_long_long ();
    void test_ValueTraits_With_signed_long_long ();
    void test_ValueTraits_With_const_signed_long_long ();
    void test_ValueTraits_With_unsigned_long_long ();
    void test_ValueTraits_With_const_unsigned_long_long ();

    // ---------------- long long* variants
    //
    void test_ValueTraits_With_long_long_Pointer ();
    void test_ValueTraits_With_const_long_long_Pointer ();
    void test_ValueTraits_With_signed_long_long_Pointer ();
    void test_ValueTraits_With_const_signed_long_long_Pointer ();
    void test_ValueTraits_With_unsigned_long_long_Pointer ();
    void test_ValueTraits_With_const_unsigned_long_long_Pointer ();

    // ---------------- bool variants
    //
    void test_ValueTraits_With_bool ();
    void test_ValueTraits_With_const_bool ();
    void test_ValueTraits_With_const_nonconst_bool ();

    // ---------------- bool* variants
    //
    void test_ValueTraits_With_bool_Pointer ();
    void test_ValueTraits_With_const_bool_Pointer ();


    // ---------------- float variants
    //
    void test_ValueTraits_With_float ();
    void test_ValueTraits_With_const_float ();
    void test_ValueTraits_With_const_nonconst_float ();

    // ---------------- float* variants
    //
    void test_ValueTraits_With_float_Pointer ();
    void test_ValueTraits_With_const_float_Pointer ();


    // ---------------- double variants
    //
    void test_ValueTraits_With_double ();
    void test_ValueTraits_With_const_double ();

    // ---------------- double* variants
    //
    void test_ValueTraits_With_double_Pointer ();
    void test_ValueTraits_With_const_double_Pointer ();


};
#endif  // UT_VALUETRAITS_HPP

//===========================================================================
// End of UT_ValueTraits.h
//===========================================================================
