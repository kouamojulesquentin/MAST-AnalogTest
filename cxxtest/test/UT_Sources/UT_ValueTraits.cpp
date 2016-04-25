//===========================================================================
//                           UT_ValueTraits.cpp
//===========================================================================
// Copyright © 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_ValueTraits.cpp
//!
//! Implements test fixture for testing 'standard' value traits
//===========================================================================


#define _CXXTEST_LONGLONG long long

#include "UT_ValueTraits.h"



UT_ValueTraits::UT_ValueTraits ()
{
}



UT_ValueTraits::~UT_ValueTraits ()
{
}


//! Exercises ValueTraits<char>
//!
void UT_ValueTraits::test_ValueTraits_With_char ()
{
    // ---------------- Setup
    //
    char lhs = 'A';
    char rhs = 'z';

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const char>
//!
void UT_ValueTraits::test_ValueTraits_With_const_char ()
{
    // ---------------- Setup
    //
    const char lhs = 'A';
    const char rhs = 'z';

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}



//! Exercises ValueTraits<signed char>
//!
void UT_ValueTraits::test_ValueTraits_With_signed_char ()
{
    // ---------------- Setup
    //
    signed char lhs = -12;
    signed char rhs = -13;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const signed char>
//!
void UT_ValueTraits::test_ValueTraits_With_const_signed_char ()
{
    // ---------------- Setup
    //
    const signed char lhs = -12;
    const signed char rhs = -13;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<unsigned char>
//!
void UT_ValueTraits::test_ValueTraits_With_unsigned_char ()
{
    // ---------------- Setup
    //
    unsigned char lhs = 45U;
    unsigned char rhs = 54U;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const unsigned char>
//!
void UT_ValueTraits::test_ValueTraits_With_const_unsigned_char ()
{
    // ---------------- Setup
    //
    const unsigned char lhs = 45U;
    const unsigned char rhs = 54U;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}


//! Exercises ValueTraits<char*>
//!
void UT_ValueTraits::test_ValueTraits_With_char_Pointer ()
{
    // ---------------- Setup
    //
    char lhs[] = "Hello";
    char rhs[] = "World";

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const char*>
//!
void UT_ValueTraits::test_ValueTraits_With_const_char_Pointer ()
{
    // ---------------- Setup
    //
    const char lhs[] = "Hello";
    const char rhs[] = "World";

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}



//! Exercises ValueTraits<signed char*>
//!
void UT_ValueTraits::test_ValueTraits_With_signed_char_Pointer ()
{
    // ---------------- Setup
    //
    signed char* lhs = reinterpret_cast<signed char*>(0x78563412);
    signed char* rhs = reinterpret_cast<signed char*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const signed char*>
//!
void UT_ValueTraits::test_ValueTraits_With_const_signed_char_Pointer ()
{
    // ---------------- Setup
    //
    const signed char* lhs = reinterpret_cast<const signed char*>(0x78563412);
    const signed char* rhs = reinterpret_cast<const signed char*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<unsigned char*>
//!
void UT_ValueTraits::test_ValueTraits_With_unsigned_char_Pointer ()
{
    // ---------------- Setup
    //
    unsigned char* lhs = reinterpret_cast<unsigned char*>(0x78563412);
    unsigned char* rhs = reinterpret_cast<unsigned char*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const unsigned char*>
//!
void UT_ValueTraits::test_ValueTraits_With_const_unsigned_char_Pointer ()
{
    // ---------------- Setup
    //
    const unsigned char* lhs = reinterpret_cast<const unsigned char*>(0x78563412);
    const unsigned char* rhs = reinterpret_cast<const unsigned char*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<int>
//!
void UT_ValueTraits::test_ValueTraits_With_int ()
{
    // ---------------- Setup
    //
    int lhs = 99;
    int rhs = 55;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const int>
//!
void UT_ValueTraits::test_ValueTraits_With_const_int ()
{
    // ---------------- Setup
    //
    const int lhs = 199;
    const int rhs = 155;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}



//! Exercises ValueTraits<signed int>
//!
void UT_ValueTraits::test_ValueTraits_With_signed_int ()
{
    // ---------------- Setup
    //
    signed int lhs = -12;
    signed int rhs = -13;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const signed int>
//!
void UT_ValueTraits::test_ValueTraits_With_const_signed_int ()
{
    // ---------------- Setup
    //
    const signed int lhs = -12;
    const signed int rhs = -13;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<unsigned int>
//!
void UT_ValueTraits::test_ValueTraits_With_unsigned_int ()
{
    // ---------------- Setup
    //
    unsigned int lhs = 4567U;
    unsigned int rhs = 5432U;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const unsigned int>
//!
void UT_ValueTraits::test_ValueTraits_With_const_unsigned_int ()
{
    // ---------------- Setup
    //
    const unsigned int lhs = 4567U;
    const unsigned int rhs = 5432U;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}


//! Exercises ValueTraits<int*>
//!
void UT_ValueTraits::test_ValueTraits_With_int_Pointer ()
{
    // ---------------- Setup
    //
    int* lhs = reinterpret_cast<signed int*>(0x78563412);
    int* rhs = reinterpret_cast<signed int*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const int*>
//!
void UT_ValueTraits::test_ValueTraits_With_const_int_Pointer ()
{
    // ---------------- Setup
    //
    const int* lhs = reinterpret_cast<signed int*>(0x78563412);
    const int* rhs = reinterpret_cast<signed int*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}



//! Exercises ValueTraits<signed int*>
//!
void UT_ValueTraits::test_ValueTraits_With_signed_int_Pointer ()
{
    // ---------------- Setup
    //
    signed int* lhs = reinterpret_cast<signed int*>(0x78563412);
    signed int* rhs = reinterpret_cast<signed int*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const signed int*>
//!
void UT_ValueTraits::test_ValueTraits_With_const_signed_int_Pointer ()
{
    // ---------------- Setup
    //
    const signed int* lhs = reinterpret_cast<const signed int*>(0x78563412);
    const signed int* rhs = reinterpret_cast<const signed int*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<unsigned int*>
//!
void UT_ValueTraits::test_ValueTraits_With_unsigned_int_Pointer ()
{
    // ---------------- Setup
    //
    unsigned int* lhs = reinterpret_cast<unsigned int*>(0x78563412);
    unsigned int* rhs = reinterpret_cast<unsigned int*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const unsigned int*>
//!
void UT_ValueTraits::test_ValueTraits_With_const_unsigned_int_Pointer ()
{
    // ---------------- Setup
    //
    const unsigned int* lhs = reinterpret_cast<const unsigned int*>(0x78563412);
    const unsigned int* rhs = reinterpret_cast<const unsigned int*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}


//! Exercises ValueTraits<long int>
//!
void UT_ValueTraits::test_ValueTraits_With_long_int ()
{
    // ---------------- Setup
    //
    long int lhs = 1234567890;
    long int rhs = 1234567899;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const long int>
//!
void UT_ValueTraits::test_ValueTraits_With_const_long_int ()
{
    // ---------------- Setup
    //
    const long int lhs = 1234567890;
    const long int rhs = 1234567899;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}



//! Exercises ValueTraits<signed long int>
//!
void UT_ValueTraits::test_ValueTraits_With_signed_long_int ()
{
    // ---------------- Setup
    //
    signed long int lhs = -1234567890;
    signed long int rhs = -1234567899;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const signed long int>
//!
void UT_ValueTraits::test_ValueTraits_With_const_signed_long_int ()
{
    // ---------------- Setup
    //
    const signed long int lhs = -1234567890;
    const signed long int rhs = -1234567899;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<unsigned long int>
//!
void UT_ValueTraits::test_ValueTraits_With_unsigned_long_int ()
{
    // ---------------- Setup
    //
    unsigned long int lhs = 1234567890U;
    unsigned long int rhs = 1234567899U;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const unsigned long int>
//!
void UT_ValueTraits::test_ValueTraits_With_const_unsigned_long_int ()
{
    // ---------------- Setup
    //
    const unsigned long int lhs = 1234567890U;
    const unsigned long int rhs = 1234567899U;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}


//! Exercises ValueTraits<long int*>
//!
void UT_ValueTraits::test_ValueTraits_With_long_int_Pointer ()
{
    // ---------------- Setup
    //
    long int* lhs = reinterpret_cast<signed long int*>(0x78563412);
    long int* rhs = reinterpret_cast<signed long int*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const long int*>
//!
void UT_ValueTraits::test_ValueTraits_With_const_long_int_Pointer ()
{
    // ---------------- Setup
    //
    const long int* lhs = reinterpret_cast<signed long int*>(0x78563412);
    const long int* rhs = reinterpret_cast<signed long int*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}



//! Exercises ValueTraits<signed long int*>
//!
void UT_ValueTraits::test_ValueTraits_With_signed_long_int_Pointer ()
{
    // ---------------- Setup
    //
    signed long int* lhs = reinterpret_cast<signed long int*>(0x78563412);
    signed long int* rhs = reinterpret_cast<signed long int*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const signed long int*>
//!
void UT_ValueTraits::test_ValueTraits_With_const_signed_long_int_Pointer ()
{
    // ---------------- Setup
    //
    const signed long int* lhs = reinterpret_cast<const signed long int*>(0x78563412);
    const signed long int* rhs = reinterpret_cast<const signed long int*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<unsigned long int*>
//!
void UT_ValueTraits::test_ValueTraits_With_unsigned_long_int_Pointer ()
{
    // ---------------- Setup
    //
    unsigned long int* lhs = reinterpret_cast<unsigned long int*>(0x78563412);
    unsigned long int* rhs = reinterpret_cast<unsigned long int*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const unsigned long int*>
//!
void UT_ValueTraits::test_ValueTraits_With_const_unsigned_long_int_Pointer ()
{
    // ---------------- Setup
    //
    const unsigned long int* lhs = reinterpret_cast<const unsigned long int*>(0x78563412);
    const unsigned long int* rhs = reinterpret_cast<const unsigned long int*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}


//! Exercises ValueTraits<long long>
//!
void UT_ValueTraits::test_ValueTraits_With_long_long ()
{
    // ---------------- Setup
    //
    long long lhs = 1234567890123456789LL;
    long long rhs = 3876543210987654321LL;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const long long>
//!
void UT_ValueTraits::test_ValueTraits_With_const_long_long ()
{
    // ---------------- Setup
    //
    const long long lhs = 1234567890123456789LL;
    const long long rhs = 3876543210987654321LL;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}



//! Exercises ValueTraits<signed long long>
//!
void UT_ValueTraits::test_ValueTraits_With_signed_long_long ()
{
    // ---------------- Setup
    //
    signed long long lhs = -1234567890123456789LL;
    signed long long rhs = -3876543210987654321LL;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const signed long long>
//!
void UT_ValueTraits::test_ValueTraits_With_const_signed_long_long ()
{
    // ---------------- Setup
    //
    const signed long long lhs = -1234567890123456789LL;
    const signed long long rhs = -3876543210987654321LL;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<unsigned long long>
//!
void UT_ValueTraits::test_ValueTraits_With_unsigned_long_long ()
{
    // ---------------- Setup
    //
    unsigned long long lhs = 8234567890123456789ULL;
    unsigned long long rhs = 8234567890123456788ULL;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const unsigned long long>
//!
void UT_ValueTraits::test_ValueTraits_With_const_unsigned_long_long ()
{
    // ---------------- Setup
    //
    const unsigned long long lhs = 8234567890123456789ULL;
    const unsigned long long rhs = 8234567890123456788ULL;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}


//! Exercises ValueTraits<long long*>
//!
void UT_ValueTraits::test_ValueTraits_With_long_long_Pointer ()
{
    // ---------------- Setup
    //
    long long* lhs = reinterpret_cast<signed long long*>(0x78563412);
    long long* rhs = reinterpret_cast<signed long long*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const long long*>
//!
void UT_ValueTraits::test_ValueTraits_With_const_long_long_Pointer ()
{
    // ---------------- Setup
    //
    const long long* lhs = reinterpret_cast<signed long long*>(0x78563412);
    const long long* rhs = reinterpret_cast<signed long long*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}



//! Exercises ValueTraits<signed long long*>
//!
void UT_ValueTraits::test_ValueTraits_With_signed_long_long_Pointer ()
{
    // ---------------- Setup
    //
    signed long long* lhs = reinterpret_cast<signed long long*>(0x78563412);
    signed long long* rhs = reinterpret_cast<signed long long*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const signed long long*>
//!
void UT_ValueTraits::test_ValueTraits_With_const_signed_long_long_Pointer ()
{
    // ---------------- Setup
    //
    const signed long long* lhs = reinterpret_cast<const signed long long*>(0x78563412);
    const signed long long* rhs = reinterpret_cast<const signed long long*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<unsigned long long*>
//!
void UT_ValueTraits::test_ValueTraits_With_unsigned_long_long_Pointer ()
{
    // ---------------- Setup
    //
    unsigned long long* lhs = reinterpret_cast<unsigned long long*>(0x78563412);
    unsigned long long* rhs = reinterpret_cast<unsigned long long*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const unsigned long long*>
//!
void UT_ValueTraits::test_ValueTraits_With_const_unsigned_long_long_Pointer ()
{
    // ---------------- Setup
    //
    const unsigned long long* lhs = reinterpret_cast<const unsigned long long*>(0x78563412);
    const unsigned long long* rhs = reinterpret_cast<const unsigned long long*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}


//! Exercises ValueTraits<bool>
//!
void UT_ValueTraits::test_ValueTraits_With_bool ()
{
    // ---------------- Setup
    //
    bool lhs = true;
    bool rhs = false;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const bool>
//!
void UT_ValueTraits::test_ValueTraits_With_const_bool ()
{
    // ---------------- Setup
    //
    const bool lhs = true;
    const bool rhs = false;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const bool> mixed with ValueTraits<bool>
//!
void UT_ValueTraits::test_ValueTraits_With_const_nonconst_bool ()
{
    // ---------------- Setup
    //
    const bool lhs = true;
          bool rhs = false;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<bool*>
//!
void UT_ValueTraits::test_ValueTraits_With_bool_Pointer ()
{
    // ---------------- Setup
    //
    bool* lhs = reinterpret_cast<bool*>(0x78563412);
    bool* rhs = reinterpret_cast<bool*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const bool*>
//!
void UT_ValueTraits::test_ValueTraits_With_const_bool_Pointer ()
{
    // ---------------- Setup
    //
    const bool* lhs = reinterpret_cast<const bool*>(0x78563412);
    const bool* rhs = reinterpret_cast<const bool*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}



//! Exercises ValueTraits<float>
//!
void UT_ValueTraits::test_ValueTraits_With_float ()
{
    // ---------------- Setup
    //
    float lhs = 99.9f;
    float rhs = 55.5f;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const float>
//!
void UT_ValueTraits::test_ValueTraits_With_const_float ()
{
    // ---------------- Setup
    //
    const float lhs = 19.9f;
    const float rhs = 15.5f;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const float> mixed with ValueTraits<float>
//!
void UT_ValueTraits::test_ValueTraits_With_const_nonconst_float ()
{
    // ---------------- Setup
    //
    const float lhs = 98.65f;
          float rhs = 55.44f;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}


//! Exercises ValueTraits<float*>
//!
void UT_ValueTraits::test_ValueTraits_With_float_Pointer ()
{
    // ---------------- Setup
    //
    float* lhs = reinterpret_cast<float*>(0x78563412);
    float* rhs = reinterpret_cast<float*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const float*>
//!
void UT_ValueTraits::test_ValueTraits_With_const_float_Pointer ()
{
    // ---------------- Setup
    //
    const float* lhs = reinterpret_cast<const float*>(0x78563412);
    const float* rhs = reinterpret_cast<const float*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<double>
//!
void UT_ValueTraits::test_ValueTraits_With_double ()
{
    // ---------------- Setup
    //
    double lhs = 99.9;
    double rhs = 55.5;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const double>
//!
void UT_ValueTraits::test_ValueTraits_With_const_double ()
{
    // ---------------- Setup
    //
    const double lhs = 19.9;
    const double rhs = 15.5;

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}


//! Exercises ValueTraits<double*>
//!
void UT_ValueTraits::test_ValueTraits_With_double_Pointer ()
{
    // ---------------- Setup
    //
    double* lhs = reinterpret_cast<double*>(0x78563412);
    double* rhs = reinterpret_cast<double*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}

//! Exercises ValueTraits<const double*>
//!
void UT_ValueTraits::test_ValueTraits_With_const_double_Pointer ()
{
    // ---------------- Setup
    //
    const double* lhs = reinterpret_cast<const double*>(0x78563412);
    const double* rhs = reinterpret_cast<const double*>(0x78563499);

    // ---------------- Exercise
    //
    TS_ASSERT_EQUALS(lhs, rhs);
}


//===========================================================================
// End of UT_ValueTraits.cpp
//===========================================================================
