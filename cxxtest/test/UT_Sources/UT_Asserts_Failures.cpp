//===========================================================================
//                           UT_Asserts_Failures.cpp
//===========================================================================
// Copyright © 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_Asserts_Failures.cpp
//!
//! Implements test fixture for testing asserts macros that should report failure
//===========================================================================

#include "UT_Asserts_Failures.h"
#include <stdexcept>
#include <functional>

namespace
{
    //! Unary functor that returns true when the value is odd
    //!
    class IsOddPredicate
    {
        public: bool operator()(int x) const { return x % 2 == 1; }
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


UT_Asserts_Failures::UT_Asserts_Failures ()
    :  m_counter_int (0)
{
}



UT_Asserts_Failures::~UT_Asserts_Failures ()
{
}


//! Initializes test (called for each test)
//!
//!
void UT_Asserts_Failures::setUp ()
{
    m_counter_int = 0;
}


//! Cleanups test (called for each test)
//!
void UT_Asserts_Failures::tearDown ()
{
}



//! Increments m_counter_int once and return the new value
//!
int UT_Asserts_Failures::incrementIntCounter ()
{
    return ++m_counter_int;
}


//! Asserts
//!
void UT_Asserts_Failures::throwException ()
{
  throw std::exception();
}


//! Runs TS_ASSERT with an expression that evaluates to false
//!
void UT_Asserts_Failures::test_ASSERT ()
{
    int value = 0;
    TS_ASSERT(incrementIntCounter() == value);
}


//! Runs TS_ASSERT_FALSE with an expression that evaluates to true
//!
void UT_Asserts_Failures::test_ASSERT_FALSE ()
{
    int value = 1;
    TS_ASSERT_FALSE(incrementIntCounter() == value);
}

//! Runs TS_ASSERT_EQUALS with parameters that make the assertion false
//!
void UT_Asserts_Failures::test_ASSERT_EQUALS ()
{
    int value = 0;
    TS_ASSERT_EQUALS(incrementIntCounter(), value);
}

//! Runs TS_ASSERT_EQUALS with parameters that make the assertion false
//!
void UT_Asserts_Failures::test_ASSERT_EQUALS_CString ()
{
    TS_ASSERT_EQUALS("Hello World", "Hella World");
}

//! Runs TS_ASSERT_EQUALS with 2 different but equal strings (as pointers to chars)
//!
void UT_Asserts_Failures::test_ASSERT_EQUALS_Char_Pointers_const_const ()
{
    const char* pStr1 = getString();
    const char* pStr2 = "Test strong";

    TS_ASSERT_EQUALS(pStr1, pStr2);
}

//! Runs TS_ASSERT_EQUALS with 2 different but equal strings (as pointers to chars)
//!
void UT_Asserts_Failures::test_ASSERT_EQUALS_Char_Pointers_const_notconst ()
{
    char        str[] = "Test strong";
    const char* pStr1 = getString();
    char*       pStr2 = str;

    TS_ASSERT_EQUALS(pStr1, pStr2);
}


//! Runs TS_ASSERT_EQUALS with 2 different but equal strings (as pointers to chars)
//!
void UT_Asserts_Failures::test_ASSERT_EQUALS_Char_Pointers_notconst_const ()
{
    char*       pStr1 = getNotConstString();
    const char* pStr2 = "Test strong";

    TS_ASSERT_EQUALS(pStr1, pStr2);
}

//! Runs TS_ASSERT_EQUALS with 2 different but equal strings (as pointers to chars)
//!
void UT_Asserts_Failures::test_ASSERT_EQUALS_Char_Pointers_notconst_notconst ()
{
    char  str[] = "Test strong";
    char* pStr1 = str;
    char* pStr2 = getNotConstString();

    TS_ASSERT_EQUALS(pStr1, pStr2);
}


//! Runs TSA_ASSERT_EQUALS with, non pointer, value parameters that make the assertion false
//!
void UT_Asserts_Failures::test_ASSERT_EQUALS_AUTO_Value ()
{
    int value = 0;
    TSA_ASSERT_EQUALS(incrementIntCounter(), value);
}

//! Runs TSA_ASSERT_EQUALS with, pointer value parameters that make the assertion false
//!
void UT_Asserts_Failures::test_ASSERT_EQUALS_AUTO_Pointer ()
{
    int value_a = 0;
    int value_b = 0;

    TSA_ASSERT_EQUALS(&value_a, &value_b);
}


//! Runs TS_ASSERT_DIFFERS with parameters that make the assertion false
//!
void UT_Asserts_Failures::test_ASSERT_DIFFERS ()
{
    int value = 1;

    TS_ASSERT_DIFFERS(incrementIntCounter(), value);
}



//! Runs TS_ASSERT_GREATER_THAN with first parameter is equal to second one
//!
void UT_Asserts_Failures::test_ASSERT_GREATER_THAN_Equal ()
{
    int value = 1;

    TS_ASSERT_GREATER_THAN(incrementIntCounter(), value);
}

//! Runs TS_ASSERT_GREATER_THAN_EQUALS with first parameter is less than second one
//!
void UT_Asserts_Failures::test_ASSERT_GREATER_THAN_EQUAL_GreaterThan ()
{
    int value = 2;

    TS_ASSERT_GREATER_THAN_EQUALS(incrementIntCounter(), value);
}


//! Runs TS_ASSERT_LESS_THAN with first parameter is equal to second one
//!
void UT_Asserts_Failures::test_ASSERT_LESS_THAN_Equal ()
{
    int value = 1;

    TS_ASSERT_LESS_THAN(incrementIntCounter(), value);
}

//! Runs TS_ASSERT_LESS_THAN_EQUALS with first parameter is greater than second one
//!
void UT_Asserts_Failures::test_ASSERT_LESS_THAN_EQUAL_GreaterThan ()
{
    int value = 0;

    TS_ASSERT_LESS_THAN_EQUALS(incrementIntCounter(), value);
}

//! Runs TS_ASSERT_DELTA with parameters that make the assertion false
//!
void UT_Asserts_Failures::test_ASSERT_DELTA ()
{
    const double expectedValue     = 2.01;
    const double expectedTolerance = 2e-11;

    TS_ASSERT_DELTA(sqrt(4.0), expectedValue, expectedTolerance);
}



//! Runs  TS_ASSERT_PREDICATE  with parameters that make the assertion false
//!
void UT_Asserts_Failures::test_ASSERT_PREDICATE ()
{
    int value = 28;

    TS_ASSERT_PREDICATE(IsOddPredicate(), value);
}

//! Runs  TS_ASSERT_RELATION  with parameters that make the assertion false
//!
void UT_Asserts_Failures::test_ASSERT_RELATION ()
{
    int lhs = 1000;
    int rhs = 1001;

    TS_ASSERT_RELATION(AreEqualRelation(), lhs, rhs);
}

//! Runs  TS_ASSERT_RELATION  with parameters that make the assertion false
//!
void UT_Asserts_Failures::test_ASSERT_SAME_DATA ()
{
    // ---------------- Setup
    //
    char lhs[] = "Lazy dog";
    char rhs[] = "Lady dog";

    // ---------------- Exercise
    //
    TS_ASSERT_SAME_DATA(lhs, rhs, sizeof(lhs));
}


//! Runs TSM_ASSERT with an expression that evaluates to false
//!
void UT_Asserts_Failures::test_ASSERT_MESSAGE ()
{
    int value = 0;

    TSM_ASSERT("This is assertion message for TSM_ASSERT", incrementIntCounter() == value);
}

//! Runs TSM_ASSERT_FALSE with an expression that evaluates to true
//!
void UT_Asserts_Failures::test_ASSERT_FALSE_MESSAGE ()
{
    int value = 1;
    TSM_ASSERT_FALSE("This is assertion message for TSM_ASSERT_FALSE", incrementIntCounter() == value);
}


//! Runs TSM_ASSERT_EQUALS with parameters that make the assertion false
//!
void UT_Asserts_Failures::test_ASSERT_MESSAGE_EQUALS ()
{
    int value = 0;

    TSM_ASSERT_EQUALS("This is assertion message for TSM_ASSERT_EQUALS", incrementIntCounter(), value);
}

//! Runs TSM_ASSERT_EQUALS with parameters that make the assertion false
//!
void UT_Asserts_Failures::test_ASSERT_MESSAGE_EQUALS_CString ()
{
    TSM_ASSERT_EQUALS("This is assertion message for TSM_ASSERT_EQUALS", "How are you?", "How are you.");
}

//! Runs TSMA_ASSERT_EQUALS with, non pointer, value parameters that make the assertion false
//!
void UT_Asserts_Failures::test_ASSERT_MESSAGE_EQUALS_AUTO_Value ()
{
    int value = 0;

    TSMA_ASSERT_EQUALS("This is assertion message for TSMA_ASSERT_EQUALS", incrementIntCounter(), value);
}

//! Runs TSMA_ASSERT_EQUALS with, pointer value parameters that make the assertion false
//!
void UT_Asserts_Failures::test_ASSERT_MESSAGE_EQUALS_AUTO_Pointer ()
{
    int value_a = 0;
    int value_b = 0;

    TSMA_ASSERT_EQUALS("This is assertion message for TSMA_ASSERT_EQUALS with pointers", &value_a, &value_b);
}


//! Runs TSM_ASSERT_DIFFERS with parameters that make the assertion false
//!
void UT_Asserts_Failures::test_ASSERT_MESSAGE_DIFFERS ()
{
    int value = 1;

    TSM_ASSERT_DIFFERS("This is assertion message for TSM_ASSERT_DIFFERS", incrementIntCounter(), value);
}

//! Runs TSM_ASSERT_GREATER_THAN with first parameter is equal to second one
//!
void UT_Asserts_Failures::test_ASSERT_MESSAGE_GREATER_THAN_Equal ()
{
    int value = 1;

    TSM_ASSERT_GREATER_THAN("This is assertion message for TSM_ASSERT_GREATER_THAN", incrementIntCounter(), value);
}

//! Runs TSM_ASSERT_GREATER_THAN_EQUALS with first parameter is less than second one
//!
void UT_Asserts_Failures::test_ASSERT_MESSAGE_GREATER_THAN_EQUAL_GreaterThan ()
{
    int value = 9;

    TSM_ASSERT_GREATER_THAN_EQUALS("This is assertion message for TSM_ASSERT_GREATER_THAN_EQUALS", incrementIntCounter(), value);
}


//! Runs TSM_ASSERT_LESS_THAN with first parameter is equal to second one
//!
void UT_Asserts_Failures::test_ASSERT_MESSAGE_LESS_THAN_Equal ()
{
    int value = 1;

    TSM_ASSERT_LESS_THAN("This is assertion message for TSM_ASSERT_LESS_THAN", incrementIntCounter(), value);
}

//! Runs TSM_ASSERT_LESS_THAN_EQUALS with first parameter is greater than second one
//!
void UT_Asserts_Failures::test_ASSERT_MESSAGE_LESS_THAN_EQUAL_GreaterThan ()
{
    int value = 0;

    TSM_ASSERT_LESS_THAN_EQUALS("This is assertion message for TSM_ASSERT_LESS_THAN_EQUALS", incrementIntCounter(), value);
}

//! Runs TSM_ASSERT_DELTA with parameters that make the assertion false
//!
void UT_Asserts_Failures::test_ASSERT_MESSAGE_DELTA ()
{
    const float expectedValue     = 2.01f;
    const float expectedTolerance = 1e-4f;

    TSM_ASSERT_DELTA("This is assertion message for TSM_ASSERT_DELTA", sqrt(4.0), expectedValue, expectedTolerance);
}



//! Runs  TSM_ASSERT_PREDICATE  with parameters that make the assertion false
//!
void UT_Asserts_Failures::test_ASSERT_MESSAGE_PREDICATE ()
{
    int value = 28;

    TSM_ASSERT_PREDICATE("This is assertion message for TSM_ASSERT_PREDICATE", IsOddPredicate(), value);
}

//! Runs  TSM_ASSERT_RELATION  with parameters that make the assertion false
//!
void UT_Asserts_Failures::test_ASSERT_MESSAGE_RELATION ()
{
    int lhs = 1000;
    int rhs = 1001;

    TSM_ASSERT_RELATION("This is assertion message for TSM_ASSERT_RELATION", AreEqualRelation(), lhs, rhs);
}

//! Runs  TSM_ASSERT_RELATION  with parameters that make the assertion false
//!
void UT_Asserts_Failures::test_ASSERT_MESSAGE_SAME_DATA ()
{
    // ---------------- Setup
    //
    char lhs[] = "Lazy dog";
    char rhs[] = "Lady dog";

    // ---------------- Exercise
    //
    TSM_ASSERT_SAME_DATA("This is assertion message for TSM_ASSERT_SAME_DATA", lhs, rhs, sizeof(lhs));
}


//! Runs  TS_ASSERT_THROWS when the exception is (considered) thrown
//!
void UT_Asserts_Failures::test_ASSERT_THROWS ()
{
    TS_ASSERT_THROWS(incrementIntCounter(), std::exception);
}

//! Runs  TS_ASSERT_THROWS_ANYTHING when the exception is (considered) thrown
//!
void UT_Asserts_Failures::test_ASSERT_THROWS_ANYTHING ()
{
    TS_ASSERT_THROWS_ANYTHING(incrementIntCounter());
}


//! Runs  TS_ASSERT_THROWS_NOTHING when no "exception"
//!
void UT_Asserts_Failures::test_ASSERT_THROWS_NOTHING ()
{
    TS_ASSERT_THROWS_NOTHING(throwException());
}

//! Runs  TSM_ASSERT_THROWS when the exception is (considered) thrown
//!
void UT_Asserts_Failures::test_ASSERT_MESSAGE_THROWS ()
{
    TSM_ASSERT_THROWS("This is assertion message for TSM_ASSERT_THROWS", incrementIntCounter(), std::exception);
}

//! Runs  TSM_ASSERT_THROWS_ANYTHING when the exception is (considered) thrown
//!
void UT_Asserts_Failures::test_ASSERT_MESSAGE_THROWS_ANYTHING ()
{
    TSM_ASSERT_THROWS_ANYTHING("This is assertion message for TSM_ASSERT_THROWS_ANYTHING", incrementIntCounter());
}


//! Runs  TSM_ASSERT_THROWS_NOTHING when no "exception"
//!
void UT_Asserts_Failures::test_ASSERT_MESSAGE_THROWS_NOTHING ()
{
    TSM_ASSERT_THROWS_NOTHING("This is assertion message for TSM_ASSERT_THROWS_NOTHING", throwException());
}


//===========================================================================
// End of UT_Asserts_Failures.cpp
//===========================================================================
