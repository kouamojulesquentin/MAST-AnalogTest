//===========================================================================
//                           UT_Asserts_Successes.cpp
//===========================================================================
// Copyright © 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_Asserts_Successes.cpp
//!
//! Implements test fixture for testing asserts macros that should be successful
//===========================================================================

#include "UT_Asserts_Successes.h"

#include <functional>
#include <stdexcept>
#include <vector>
#include <string>
#include <map>

using std::vector;
using std::string;
using std::map;


namespace
{
    //! Unary functor that returns true when the value is odd
    //!
    class IsOddPredicate
    {
        public: bool operator()(int x) const
        {
            return x % 2 == 1;
        }
    };

    //! Binary functor that returns true when two values are equal
    //!
    class AreEqualRelation
    {
        public: bool operator()(int x, int y) const
        {
            return x == y;
        }
    };

    //! Returns a char pointer on string litteral
    //!
    const char* getString()
    {
      return "Test string";
    }

    //! Returns a char pointer on non const string
    //!
    char* getNotConstString()
    {
      static char str[] = "Test string";
      char* pStr = str;
      return pStr;
    }


} // End of unnamed namespace


UT_Asserts_Successes::UT_Asserts_Successes ()
    :  m_counter_int (0)
{
}



UT_Asserts_Successes::~UT_Asserts_Successes ()
{
}


//! Initializes test (called for each test)
//!
//!
void UT_Asserts_Successes::setUp ()
{
    m_counter_int = 0;
}


//! Cleanups test (called for each test)
//!
void UT_Asserts_Successes::tearDown ()
{
}



//! Increments m_counter_int once and return the new value
//!
int UT_Asserts_Successes::incrementIntCounter ()
{
    return ++m_counter_int;
}


//! Throws an exception
//!
void UT_Asserts_Successes::throwException ()
{
    throw std::range_error("Thrown from 'throwException'");
}




//! Runs TS_ASSERT with an expression that evaluates to true
//!
void UT_Asserts_Successes::test_ASSERT ()
{
    int value = 1;
    TS_ASSERT(incrementIntCounter() == value);
}

//! Runs TS_ASSERT_FALSE with an expression that evaluates to true
//!
void UT_Asserts_Successes::test_ASSERT_FALSE ()
{
    int value = 0;
    TS_ASSERT_FALSE(incrementIntCounter() == value);
}

//! Runs TS_ASSERT_EQUALS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS ()
{
    int value = 1;
    TS_ASSERT_EQUALS(incrementIntCounter(), value);
}

//! Runs TS_ASSERT_EQUALS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Int_Signed_Unsigned ()
{
    int          signedValue   = 123;
    unsigned int unsignedValue = 123U;

    TS_ASSERT_EQUALS(signedValue, unsignedValue);
}

//! Runs TS_ASSERT_EQUALS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Int_Unsigned_unsigned ()
{
    int          signedValue   = 123;
    unsigned int unsignedValue = 123U;

    TS_ASSERT_EQUALS(unsignedValue, signedValue);
}

//! Runs TS_ASSERT_EQUALS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Short_Signed_Unsigned ()
{
    short          signedValue   = 123;
    unsigned short unsignedValue = 123U;

    TS_ASSERT_EQUALS(signedValue, unsignedValue);
}

//! Runs TS_ASSERT_EQUALS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Short_Unsigned_unsigned ()
{
    short          signedValue   = 123;
    unsigned short unsignedValue = 123U;

    TS_ASSERT_EQUALS(unsignedValue, signedValue);
}



//! Runs TS_ASSERT_EQUALS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Char_Plain_Unsigned ()
{
    char          plainValue    = 123;
    unsigned char unsignedValue = 123U;

    TS_ASSERT_EQUALS(plainValue, unsignedValue);
}

//! Runs TS_ASSERT_EQUALS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Char_Unsigned_Plain ()
{
    char          plainValue    = 123;
    unsigned char unsignedValue = 123U;

    TS_ASSERT_EQUALS(unsignedValue, plainValue);
}

//! Runs TS_ASSERT_EQUALS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Char_Signed_Unsigned ()
{
    signed   char signedValue   = 123;
    unsigned char unsignedValue = 123U;

    TS_ASSERT_EQUALS(signedValue, unsignedValue);
}


//! Runs TS_ASSERT_EQUALS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Char_Unsigned_Signed ()
{
    char          signedValue   = 123;
    unsigned char unsignedValue = 123U;

    TS_ASSERT_EQUALS(unsignedValue, signedValue);
}


//! Runs TS_ASSERT_EQUALS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Char_Plain_Short_Unsigned ()
{
    char            plainValue    = 123;
    unsigned short  unsignedValue = 123U;

    TS_ASSERT_EQUALS(plainValue, unsignedValue);
}

//! Runs TS_ASSERT_EQUALS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Short_Signed_Char_Plain ()
{
    signed short signedValue = 123;
    char         plainValue  = 123U;

    TS_ASSERT_EQUALS(signedValue, plainValue);
}


//! Runs TS_ASSERT_EQUALS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Char_Signed_Short_Unsigned ()
{
    signed   char  signedValue   = 123;
    unsigned short unsignedValue = 123U;

    TS_ASSERT_EQUALS(signedValue, unsignedValue);
}

//! Runs TS_ASSERT_EQUALS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Short_Signed_Char_Unsigned ()
{
    signed short  signedValue   = 123;
    unsigned char unsignedValue = 123U;

    TS_ASSERT_EQUALS(signedValue, unsignedValue);
}



//! Runs TS_ASSERT_EQUALS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Char_Plain_Int_Unsigned ()
{
    char          plainValue    = 123;
    unsigned int  unsignedValue = 123U;

    TS_ASSERT_EQUALS(plainValue, unsignedValue);
}

//! Runs TS_ASSERT_EQUALS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Int_Signed_Char_Plain ()
{
    signed int    signedValue = 123;
    char          plainValue  = 123U;

    TS_ASSERT_EQUALS(signedValue, plainValue);
}


//! Runs TS_ASSERT_EQUALS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Char_Signed_Int_Unsigned ()
{
    signed   char signedValue   = 123;
    unsigned int  unsignedValue = 123U;

    TS_ASSERT_EQUALS(signedValue, unsignedValue);
}

//! Runs TS_ASSERT_EQUALS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Int_Signed_Char_Unsigned ()
{
    signed int    signedValue   = 123;
    unsigned char unsignedValue = 123U;

    TS_ASSERT_EQUALS(signedValue, unsignedValue);
}


//! Runs TS_ASSERT_EQUALS with 2 different but equal strings (as array of chars)
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Char_Arrays ()
{
    char str1[] = "Test string";
    char str2[] = "Test string";
    TS_ASSERT_EQUALS(str1, str2);
}

//! Runs TS_ASSERT_EQUALS with 2 different but equal strings (as pointers to chars)
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Char_Pointers_const_const ()
{
    const char* pStr1 = getString();
    const char* pStr2 = "Test string";

    TS_ASSERT_EQUALS(pStr1, pStr2);
}

//! Runs TS_ASSERT_EQUALS with 2 different but equal strings (as pointers to chars)
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Char_Pointers_const_notconst ()
{
    char        str[] = "Test string";
    const char* pStr1 = getString();
    char*       pStr2 = str;

    TS_ASSERT_EQUALS(pStr1, pStr2);
}


//! Runs TS_ASSERT_EQUALS with 2 different but equal strings (as pointers to chars)
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Char_Pointers_notconst_const ()
{
    char*       pStr1 = getNotConstString();
    const char* pStr2 = "Test string";

    TS_ASSERT_EQUALS(pStr1, pStr2);
}

//! Runs TS_ASSERT_EQUALS with 2 different but equal strings (as pointers to chars)
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Char_Pointers_notconst_notconst ()
{
    char  str[] = "Test string";
    char* pStr1 = str;
    char* pStr2 = getNotConstString();

    TS_ASSERT_EQUALS(pStr1, pStr2);
}


//! Runs TS_ASSERT_EQUALS with 2 different but equal strings (as array and pointer to chars)
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Char_ArrayPtr ()
{
    char        str1[] = "Test string";
    const char* pStr2  = getString();

    TS_ASSERT_EQUALS(str1, pStr2);
}

//! Runs TS_ASSERT_EQUALS with 2 different but equal strings (as pointer to chars and array of chars)
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_Char_PtrArray ()
{
    const char* pStr1  = getString();
    char        str2[] = "Test string";

    TS_ASSERT_EQUALS(pStr1, str2);
}

//! Runs TSA_ASSERT_EQUALS with, non pointer, value parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_AUTO_Value ()
{
    int value = 1;
    TSA_ASSERT_EQUALS(incrementIntCounter(), value);
}

//! Runs TSA_ASSERT_EQUALS with, pointer value parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_EQUALS_AUTO_Pointer ()
{
    int value = 0;

    TSA_ASSERT_EQUALS(&value, &value);
}


//! Runs TS_ASSERT_DIFFERS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_DIFFERS ()
{
    int value = 2;

    TS_ASSERT_DIFFERS(incrementIntCounter(), value);
}

//! Runs TS_ASSERT_DIFFERS with 2 different strings (as array of chars)
//!
void UT_Asserts_Successes::test_ASSERT_DIFFERS_Char_Arrays ()
{
    char str1[] = "Test string";
    char str2[] = "Test spring";
    TS_ASSERT_DIFFERS(str1, str2);
}

//! Runs TS_ASSERT_DIFFERS with 2 different strings (as pointers to chars)
//!
void UT_Asserts_Successes::test_ASSERT_DIFFERS_Char_Pointers ()
{
    const char* pStr1 = getString();
    const char* pStr2 = "Test strong";

    TS_ASSERT_DIFFERS(pStr1, pStr2);
}

//! Runs TS_ASSERT_DIFFERS with 2 different strings (as array and pointer to chars)
//!
void UT_Asserts_Successes::test_ASSERT_DIFFERS_Char_ArrayPtr ()
{
    char        str1[] = "This local string differs from the one from getString()!";
    const char* pStr2  = getString();

    TS_ASSERT_DIFFERS(str1, pStr2);
}

//! Runs TS_ASSERT_DIFFERS with 2 different strings (as pointer to chars and array of chars)
//!
void UT_Asserts_Successes::test_ASSERT_DIFFERS_Char_PtrArray ()
{
    const char* pStr1  = getString();
    char        str2[] = "This local string differs from the one from getString()!";

    TS_ASSERT_DIFFERS(pStr1, str2);
}

//! Runs TS_ASSERT_GREATER_THAN with first parameter is equal to second one
//!
void UT_Asserts_Successes::test_ASSERT_GREATER_THAN_GreaterThan ()
{
    int value = 0;

    TS_ASSERT_GREATER_THAN(incrementIntCounter(), value);
}

//! Runs TS_ASSERT_GREATER_THAN_EQUALS with first parameter is eqless than second one
//!
void UT_Asserts_Successes::test_ASSERT_GREATER_THAN_EQUAL_GreaterThan ()
{
    int value = 0;

    TS_ASSERT_GREATER_THAN_EQUALS(incrementIntCounter(), value);
}

//! Runs TS_ASSERT_GREATER_THAN_EQUALS with first parameter is eqless than second one
//!
void UT_Asserts_Successes::test_ASSERT_GREATER_THAN_EQUAL_Equal ()
{
    int value = 1;

    TS_ASSERT_GREATER_THAN_EQUALS(incrementIntCounter(), value);
}


//! Runs TS_ASSERT_LESS_THAN with first parameter is less than second one
//!
void UT_Asserts_Successes::test_ASSERT_LESS_THAN_LessThan ()
{
    TS_ASSERT_LESS_THAN(incrementIntCounter(), 2);
}

//! Runs TS_ASSERT_LESS_THAN_EQUALS with first parameter is less than second one
//!
void UT_Asserts_Successes::test_ASSERT_LESS_THAN_EQUAL_LessThan ()
{
    int value = 2;

    TS_ASSERT_LESS_THAN(incrementIntCounter(), value);
}

//! Runs TS_ASSERT_LESS_THAN_EQUALS with first parameters equals second one
//!
void UT_Asserts_Successes::test_ASSERT_LESS_THAN_EQUAL_Equal ()
{
    int value = 1;

    TS_ASSERT_LESS_THAN_EQUALS(incrementIntCounter(), value);
}

//! Runs TS_ASSERT_DELTA with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_DELTA ()
{
    const float expectedValue     = 2.0f;
    const float expectedTolerance = 2e-11f;

    TS_ASSERT_DELTA(sqrt(4.0), expectedValue, expectedTolerance);
}



//! Runs  TS_ASSERT_PREDICATE  with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_PREDICATE ()
{
    int value = 29;

    TS_ASSERT_PREDICATE(IsOddPredicate(), value);
}

//! Runs  TS_ASSERT_RELATION  with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_RELATION ()
{
    int lhs = 500;
    int rhs = 500;

    TS_ASSERT_RELATION(AreEqualRelation(), lhs, rhs);
}

//! Runs  TS_ASSERT_RELATION  with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_SAME_DATA ()
{
    // ---------------- Setup
    //
    char lhs[] = "The quick brown fox ran over the lazy dog";
    char rhs[sizeof(lhs)];

    std::memcpy(rhs, lhs, sizeof(lhs));

    // ---------------- Exercise
    //
    TS_ASSERT_SAME_DATA(lhs, rhs, 26);
}


//! Runs TS_ASSERT_EMPTY with empty char[]
//!
void UT_Asserts_Successes::test_ASSERT_EMPTY_CharArray ()
{
    // ---------------- Setup
    //
    const char emptyCharArray[] = "";

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_EMPTY(emptyCharArray);
}


//! Runs TS_ASSERT_EMPTY with empty C-Style string
//!
void UT_Asserts_Successes::test_ASSERT_EMPTY_C_String ()
{
    // ---------------- Setup
    //
    const char* emptyC_String = "";

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_EMPTY(emptyC_String);
}


//! Runs TS_ASSERT_EMPTY with nullptr
//!
void UT_Asserts_Successes::test_ASSERT_EMPTY_NullPtr ()
{
    // ---------------- Setup
    //
    const char* nullPtr = NULL;

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_EMPTY(nullPtr);
}


//! Runs TS_ASSERT_EMPTY with empty std::string
//!
void UT_Asserts_Successes::test_ASSERT_EMPTY_String ()
{
    // ---------------- Setup
    //
    string emptyString;

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_EMPTY(emptyString);
}

//! Runs TS_ASSERT_EMPTY with empty std::vector
//!
void UT_Asserts_Successes::test_ASSERT_EMPTY_Vector ()
{
    // ---------------- Setup
    //
    vector<int> emptyVector;

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_EMPTY(emptyVector);
}


//! Runs TS_ASSERT_EMPTY with empty std::map
//!
void UT_Asserts_Successes::test_ASSERT_EMPTY_Map ()
{
    // ---------------- Setup
    //
    map<int, string> emptyMap;

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_EMPTY(emptyMap);
}

//! Runs TSM_ASSERT with an expression that evaluates to true
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE ()
{
    int value = 1;

    TSM_ASSERT("This is assertion message for TSM_ASSERT", incrementIntCounter() == value);
}

//! Runs TSM_ASSERT with an expression that evaluates to true
//!
void UT_Asserts_Successes::test_ASSERT_FALSE_MESSAGE ()
{
    int value = 10;

    TSM_ASSERT_FALSE("This is assertion message for TSM_ASSERT_FALSE", incrementIntCounter() == value);
}


//! Runs TSM_ASSERT_EQUALS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE_EQUALS ()
{
    int value = 1;

    TSM_ASSERT_EQUALS("This is assertion message for TSM_ASSERT_EQUALS", incrementIntCounter(), value);
}

//! Runs TSMA_ASSERT_EQUALS with, non pointer, value parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE_EQUALS_AUTO_Value ()
{
    int value = 1;

    TSMA_ASSERT_EQUALS("This is assertion message for TSMA_ASSERT_EQUALS", incrementIntCounter(), value);
}

//! Runs TSMA_ASSERT_EQUALS with, pointer value parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE_EQUALS_AUTO_Pointer ()
{
    int value = 0;

    TSMA_ASSERT_EQUALS("This is assertion message for TSMA_ASSERT_EQUALS with pointers", &value, &value);
}



//! Runs TSM_ASSERT_DIFFERS with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE_DIFFERS ()
{
    int value = 2;

    TSM_ASSERT_DIFFERS("This is assertion message for TSM_ASSERT_DIFFERS", incrementIntCounter(), value);
}


//! Runs TSM_ASSERT_GREATER_THAN with first parameter is greater than second one
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE_GREATER_THAN_GreaterThan ()
{
    int value = 0;

    TSM_ASSERT_GREATER_THAN("This is assertion message for TSM_ASSERT_GREATER_THAN", incrementIntCounter(), value);
}

//! Runs TSM_ASSERT_GREATER_THAN_EQUALS with first parameter is greater than second one
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE_GREATER_THAN_EQUAL_GreaterThan ()
{
    int value = 0;

    TSM_ASSERT_GREATER_THAN_EQUALS("This is assertion message for TSM_ASSERT_GREATER_THAN_EQUALS", incrementIntCounter(), value);
}

//! Runs TSM_ASSERT_GREATER_THAN_EQUALS with first parameter is greater than second one
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE_GREATER_THAN_EQUAL_Equal()
{
    int value = 1;

    TSM_ASSERT_GREATER_THAN_EQUALS("This is assertion message for TSM_ASSERT_GREATER_THAN_EQUALS", incrementIntCounter(), value);
}



//! Runs TSM_ASSERT_LESS_THAN with first parameter is less than second one
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE_LESS_THAN_LessThan ()
{
    int value = 2;

    TSM_ASSERT_LESS_THAN("This is assertion message for TSM_ASSERT_LESS_THAN", incrementIntCounter(), value);
}

//! Runs TSM_ASSERT_LESS_THAN_EQUALS with first parameter is less than second one
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE_LESS_THAN_EQUAL_LessThan ()
{
    TSM_ASSERT_LESS_THAN_EQUALS("This is assertion message for TSM_ASSERT_LESS_THAN_EQUALS", incrementIntCounter(), 2);
}

//! Runs TSM_ASSERT_LESS_THAN_EQUALS with first parameters equals second one
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE_LESS_THAN_EQUAL_Equal ()
{
    int value = 1;

    TSM_ASSERT_LESS_THAN_EQUALS("This is assertion message for TSM_ASSERT_LESS_THAN_EQUALS", incrementIntCounter(), value);
}

//! Runs TSM_ASSERT_DELTA with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE_DELTA ()
{
    const float expectedValue     = 2.00f;
    const float expectedTolerance = 1e-4f;

    TSM_ASSERT_DELTA("This is assertion message for TSM_ASSERT_DELTA", sqrt(4.0), expectedValue, expectedTolerance);
}



//! Runs  TSM_ASSERT_PREDICATE  with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE_PREDICATE ()
{
    int value = 29;

    TSM_ASSERT_PREDICATE("This is assertion message for TSM_ASSERT_PREDICATE", IsOddPredicate(), value);
}

//! Runs  TSM_ASSERT_RELATION  with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE_RELATION ()
{
    int lhs = 500;
    int rhs = 500;

    TSM_ASSERT_RELATION("This is assertion message for TSM_ASSERT_RELATION", AreEqualRelation(), lhs, rhs);
}

//! Runs  TSM_ASSERT_RELATION  with parameters that make the assertion true
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE_SAME_DATA ()
{
    // ---------------- Setup
    //
    char lhs[] = "The quick brown fox ran over the lazy dog";
    char rhs[sizeof(lhs)];

    std::memcpy(rhs, lhs, sizeof(lhs));

    // ---------------- Exercise
    //
    TSM_ASSERT_SAME_DATA("This is assertion message for TSM_ASSERT_SAME_DATA", lhs, rhs, 26);
}

//! Runs  TS_ASSERT_THROWS when the exception is thrown
//!
void UT_Asserts_Successes::test_ASSERT_THROWS ()
{
    TS_ASSERT_THROWS(throwException(), std::range_error);
}

//! Runs  TS_ASSERT_THROWS_ANYTHING when the exception is (considered) thrown
//!
void UT_Asserts_Successes::test_ASSERT_THROWS_ANYTHING ()
{
    TS_ASSERT_THROWS_ANYTHING(throwException());
}

//! Runs  TS_ASSERT_THROWS_NOTHING when no "exception"
//!
void UT_Asserts_Successes::test_ASSERT_THROWS_NOTHING ()
{
    TS_ASSERT_THROWS_NOTHING(incrementIntCounter());
}


//! Runs  TSM_ASSERT_THROWS when the exception is thrown
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE_THROWS ()
{
    TSM_ASSERT_THROWS("This is assertion message for TSM_ASSERT_THROWS", throwException(), std::exception);
}

//! Runs  TSM_ASSERT_THROWS_ANYTHING when the exception is (considered) thrown
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE_THROWS_ANYTHING ()
{
    TSM_ASSERT_THROWS_ANYTHING("This is assertion message for TSM_ASSERT_THROWS_ANYTHING", throwException());
}

//! Runs  TSM_ASSERT_THROWS_NOTHING when no "exception"
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE_THROWS_NOTHING ()
{
    TSM_ASSERT_THROWS_NOTHING("This is assertion message for TSM_ASSERT_THROWS_NOTHING", incrementIntCounter());
}


//! Runs TSM_ASSERT_EMPTY with empty std::string
//!
void UT_Asserts_Successes::test_ASSERT_MESSAGE_EMPTY ()
{
    // ---------------- Setup
    //
    string emptyString;

    // ---------------- Exercise & Verify
    //
    TSM_ASSERT_EMPTY("This is assertion message for TSM_ASSERT_EMPTY", emptyString);
}

//===========================================================================
// End of UT_Asserts_Successes.cpp
//===========================================================================
