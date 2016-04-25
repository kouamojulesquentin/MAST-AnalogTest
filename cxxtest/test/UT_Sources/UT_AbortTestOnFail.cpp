//===========================================================================
//                           UT_AbortTestOnFail.cpp
//===========================================================================
// Copyright © 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_AbortTestOnFail.cpp
//!
//! Implements test fixture for testing abort on failure
//!
//===========================================================================


#include <stdexcept>

#include "UT_AbortTestOnFail.h"
#include <cxxtest/ValueTraits.h>


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

} // End of unnamed namespace


UT_AbortTestOnFail::UT_AbortTestOnFail ()
{
}



UT_AbortTestOnFail::~UT_AbortTestOnFail ()
{
}


//! Does nothing at all
//!
void UT_AbortTestOnFail::doNothing ()
{
}


//! Throws an exception (in fact just assert)
//!
void UT_AbortTestOnFail::throwException ()
{
  throw std::exception();
}


//! Runs TS_ASSERT with an expression that evaluates to false and AbortTestOnFail is false
//!
void UT_AbortTestOnFail::test_ASSERT_AbortTestOnFail_True ()
{
    CxxTest::setAbortTestOnFail(true);
    int lhs = 2;
    int rhs = 1;

    TS_ASSERT(lhs == rhs);
    TS_WARN("This message must not be in test report");
}

//! Runs TS_ASSERT_FALSE with an expression that evaluates to false and AbortTestOnFail is false
//!
void UT_AbortTestOnFail::test_ASSERT_FALSE_AbortTestOnFail_True ()
{
    CxxTest::setAbortTestOnFail(true);
    int lhs = 2;
    int rhs = 2;

    TS_ASSERT_FALSE(lhs == rhs);
    TS_WARN("This message must not be in test report");
}

//! Runs TS_ASSERT_EQUALS with parameters that make the assertion false and AbortTestOnFail is true
//!
void UT_AbortTestOnFail::test_ASSERT_EQUALS_AbortTestOnFail_True ()
{
    CxxTest::setAbortTestOnFail(true);
    int lhs = 2;
    int rhs = 1;

    TS_ASSERT_EQUALS(lhs, rhs);

    TS_WARN("This message must not be in test report");
}


//! Runs TS_ASSERT_DIFFERS with parameters that make the assertion false and AbortTestOnFail is true
//!
void UT_AbortTestOnFail::test_ASSERT_DIFFERS_AbortTestOnFail_True ()
{
    CxxTest::setAbortTestOnFail(true);
    int lhs = 2;
    int rhs = 2;

    TS_ASSERT_DIFFERS(lhs, rhs);

    TS_WARN("This message must not be in test report");
}




//! Runs TS_ASSERT_GREATER_THAN with first parameter less than second one and AbortTestOnFail is true
//!
void UT_AbortTestOnFail::test_ASSERT_GREATER_THAN_AbortTestOnFail_True ()
{
    CxxTest::setAbortTestOnFail(true);
    int sutValue      = 3;
    int expectedValue = 10;

    TS_ASSERT_GREATER_THAN(sutValue, expectedValue);

    TS_WARN("This message must not be in test report !!!");
}

//! Runs TS_ASSERT_GREATER_THAN_EQUALS with first parameter less than second one and AbortTestOnFail is true
//!
void UT_AbortTestOnFail::test_ASSERT_GREATER_THAN_EQUAL_AbortTestOnFail_True ()
{
    CxxTest::setAbortTestOnFail(true);
    int sutValue      = 4;
    int expectedValue = 10;

    TS_ASSERT_GREATER_THAN_EQUALS(sutValue, expectedValue);

    TS_WARN("This message must not be in test report !!!");
}



//! Runs TS_ASSERT_LESS_THAN with first parameter is greater than second one and AbortTestOnFail is true
//!
void UT_AbortTestOnFail::test_ASSERT_LESS_THAN_AbortTestOnFail_True ()
{
    CxxTest::setAbortTestOnFail(true);
    int sutValue      = 3;
    int expectedValue = 2;

    TS_ASSERT_LESS_THAN(sutValue, expectedValue);

    TS_WARN("This message must not be in test report");
}

//! Runs TS_ASSERT_LESS_THAN_EQUALS with first parameter is greater than second one and AbortTestOnFail is true
//!
void UT_AbortTestOnFail::test_ASSERT_LESS_THAN_EQUAL_AbortTestOnFail_True ()
{
    CxxTest::setAbortTestOnFail(true);
    int sutValue      = 3;
    int expectedValue = 2;

    TS_ASSERT_LESS_THAN_EQUALS(sutValue, expectedValue);

    TS_WARN("This message must not be in test report");
}

//! Runs TS_ASSERT_DELTA with parameters that make the assertion false and AbortTestOnFail is true
//!
void UT_AbortTestOnFail::test_ASSERT_DELTA_AbortTestOnFail_True ()
{
    CxxTest::setAbortTestOnFail(true);

    float lhs           = 2.0f;
    float rhs           = 2.0001f;
    float acceptedDelta = 1e-7f;

    TS_ASSERT_DELTA(lhs, rhs, acceptedDelta);

    TS_WARN("This message must not be in test report");
}



//! Runs TS_ASSERT_PREDICATE  with parameters that make the assertion false and AbortTestOnFail is true
//!
void UT_AbortTestOnFail::test_ASSERT_PREDICATE_AbortTestOnFail_True ()
{
    CxxTest::setAbortTestOnFail(true);

    int value = 44;

    TS_ASSERT_PREDICATE(IsOddPredicate(), value);

    TS_WARN("This message must not be in test report");
}

//! Runs TS_ASSERT_RELATION  with parameters that make the assertion false and AbortTestOnFail is true
//!
void UT_AbortTestOnFail::test_ASSERT_RELATION_AbortTestOnFail_True ()
{
    CxxTest::setAbortTestOnFail(true);

    int lhs = 999;
    int rhs = 1000;

    TS_ASSERT_RELATION(AreEqualRelation(), lhs, rhs);

    TS_WARN("This message must not be in test report");
}

//! Runs TS_ASSERT_RELATION  with parameters that make the assertion false and AbortTestOnFail is true
//!
void UT_AbortTestOnFail::test_ASSERT_SAME_DATA_AbortTestOnFail_True ()
{
    // ---------------- Setup
    //
    char lhs[] = "world";
    char rhs[] = "World";
    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise
    //
    TS_ASSERT_SAME_DATA(lhs, rhs, sizeof(lhs));

    TS_WARN("This message must not be in test report");
}

//! Runs TS_ASSERT_THROWS when no exception is thrown and AbortTestOnFail is true
//!
void UT_AbortTestOnFail::test_ASSERT_THROWS_AbortTestOnFail_True ()
{
    CxxTest::setAbortTestOnFail(true);

    TS_ASSERT_THROWS(doNothing(), std::exception);


    TS_WARN("This message must not be in test report");
}

//! Runs TS_ASSERT_THROWS_ANYTHING when no exception is thrown and AbortTestOnFail is true
//!
void UT_AbortTestOnFail::test_ASSERT_THROWS_ANYTHING_AbortTestOnFail_True ()
{
    CxxTest::setAbortTestOnFail(true);

    TS_ASSERT_THROWS_ANYTHING(doNothing());

    TS_WARN("This message must not be in test report");
}


//! Runs TS_ASSERT_THROWS_NOTHING when an exception is thrown and AbortTestOnFail is true
//!
void UT_AbortTestOnFail::test_ASSERT_THROWS_NOTHING_AbortTestOnFail_True ()
{
    CxxTest::setAbortTestOnFail(true);

    TS_ASSERT_THROWS_NOTHING(throwException());

    TS_WARN("This message must not be in test report");
}


//! Runs TS_ASSERT with an expression that evaluates to false and AbortTestOnFail is false
//!
void UT_AbortTestOnFail::test_ASSERT_AbortTestOnFail_False ()
{
    CxxTest::setAbortTestOnFail(false);

    int lhs = 2;
    int rhs = 1;

    TS_ASSERT(lhs == rhs);

    TS_WARN("This is expected message for test_ASSERT_AbortTestOnFail_False");
}


//! Runs TS_ASSERT_FALSE with an expression that evaluates to false and AbortTestOnFail is false
//!
void UT_AbortTestOnFail::test_ASSERT_FALSE_AbortTestOnFail_False ()
{
    CxxTest::setAbortTestOnFail(false);

    int lhs = 2;
    int rhs = 2;

    TS_ASSERT_FALSE(lhs == rhs);

    TS_WARN("This is expected message for test_ASSERT_AbortTestOnFail_False");
}


//! Runs TS_ASSERT_EQUALS with parameters that make the assertion false and AbortTestOnFail is false
//!
void UT_AbortTestOnFail::test_ASSERT_EQUALS_AbortTestOnFail_False ()
{
    CxxTest::setAbortTestOnFail(false);

    int lhs = 2;
    int rhs = 1;

    TS_ASSERT_EQUALS(lhs, rhs);

    TS_WARN("This is expected message for test_ASSERT_EQUALS_AbortTestOnFail_False");
}


//! Runs TS_ASSERT_DIFFERS with parameters that make the assertion false and AbortTestOnFail is false
//!
void UT_AbortTestOnFail::test_ASSERT_DIFFERS_AbortTestOnFail_False ()
{
    CxxTest::setAbortTestOnFail(false);

    int lhs = 2;
    int rhs = 2;

    TS_ASSERT_DIFFERS(lhs, rhs);

    TS_WARN("This is expected message for test_ASSERT_DIFFERS_AbortTestOnFail_False");
}


//! Runs TS_ASSERT_GREATER_THAN with first parameter is greater than second one and AbortTestOnFail is false
//!
void UT_AbortTestOnFail::test_ASSERT_GREATER_THAN_AbortTestOnFail_False ()
{
    CxxTest::setAbortTestOnFail(false);
    int sutValue      = 3;
    int expectedValue = 20;

    TS_ASSERT_GREATER_THAN(sutValue, expectedValue);

    TS_WARN("This is expected message for test_ASSERT_GREATER_THAN_AbortTestOnFail_False");
}

//! Runs TS_ASSERT_GREATER_THAN_EQUALS with first parameter is greater than second one and AbortTestOnFail is false
//!
void UT_AbortTestOnFail::test_ASSERT_GREATER_THAN_EQUAL_AbortTestOnFail_False ()
{
    CxxTest::setAbortTestOnFail(false);
    int sutValue      = 8;
    int expectedValue = 20;

    TS_ASSERT_GREATER_THAN_EQUALS(sutValue, expectedValue);

    TS_WARN("This is expected message for test_ASSERT_GREATER_THAN_EQUAL_AbortTestOnFail_False");
}




//! Runs TS_ASSERT_LESS_THAN with first parameter is greater than second one and AbortTestOnFail is false
//!
void UT_AbortTestOnFail::test_ASSERT_LESS_THAN_AbortTestOnFail_False ()
{
    CxxTest::setAbortTestOnFail(false);
    int sutValue      = 3;
    int expectedValue = 2;

    TS_ASSERT_LESS_THAN(sutValue, expectedValue);

    TS_WARN("This is expected message for test_ASSERT_LESS_THAN_AbortTestOnFail_False");
}

//! Runs TS_ASSERT_LESS_THAN_EQUALS with first parameter is greater than second one and AbortTestOnFail is false
//!
void UT_AbortTestOnFail::test_ASSERT_LESS_THAN_EQUAL_AbortTestOnFail_False ()
{
    CxxTest::setAbortTestOnFail(false);
    int sutValue      = 3;
    int expectedValue = 2;

    TS_ASSERT_LESS_THAN_EQUALS(sutValue, expectedValue);

    TS_WARN("This is expected message for test_ASSERT_LESS_THAN_EQUAL_AbortTestOnFail_False");
}

//! Runs TS_ASSERT_DELTA with parameters that make the assertion false and AbortTestOnFail is false
//!
void UT_AbortTestOnFail::test_ASSERT_DELTA_AbortTestOnFail_False ()
{
    CxxTest::setAbortTestOnFail(false);

    float lhs           = 2.0f;
    float rhs           = 2.0001f;
    float acceptedDelta = 1e-7f;

    TS_ASSERT_DELTA(lhs, rhs, acceptedDelta);

    TS_WARN("This is expected message for test_ASSERT_DELTA_AbortTestOnFail_False");
}


//! Runs TS_ASSERT_IS_INFINITE  with parameter that make the assertion false and AbortTestOnFail is false
//!
//+void UT_AbortTestOnFail::test_ASSERT_IS_INFINITE_AbortTestOnFail_False ()
//+{
//+    CxxTest::setAbortTestOnFail(false);
//+    TS_ASSERT_IS_INFINITE(1.0);
//+    TS_WARN("This is expected message for test_ASSERT_IS_INFINITE_AbortTestOnFail_False");
//+}


//! Runs TS_ASSERT_IS_NAN  with parameter that make the assertion false and AbortTestOnFail is false
//!
//+void UT_AbortTestOnFail::test_ASSERT_IS_NAN_AbortTestOnFail_False ()
//+{
//+    CxxTest::setAbortTestOnFail(false);
//+    TS_ASSERT_IS_NAN(0.0);
//+    TS_WARN("This is expected message for test_ASSERT_IS_NAN_AbortTestOnFail_False");
//+}


//! Runs TS_ASSERT_PREDICATE  with parameters that make the assertion false and AbortTestOnFail is false
//!
void UT_AbortTestOnFail::test_ASSERT_PREDICATE_AbortTestOnFail_False ()
{
    CxxTest::setAbortTestOnFail(false);
    int value = 22;

    TS_ASSERT_PREDICATE(IsOddPredicate(), value);

    TS_WARN("This is expected message for test_ASSERT_PREDICATE_AbortTestOnFail_False");
}

//! Runs TS_ASSERT_RELATION  with parameters that make the assertion false and AbortTestOnFail is false
//!
void UT_AbortTestOnFail::test_ASSERT_RELATION_AbortTestOnFail_False ()
{
    CxxTest::setAbortTestOnFail(false);

    int lhs = 999;
    int rhs = 1000;

    TS_ASSERT_RELATION(AreEqualRelation(), lhs, rhs);

    TS_WARN("This is expected message for test_ASSERT_RELATION_AbortTestOnFail_False");
}

//! Runs TS_ASSERT_RELATION  with parameters that make the assertion false and AbortTestOnFail is false
//!
void UT_AbortTestOnFail::test_ASSERT_SAME_DATA_AbortTestOnFail_False ()
{
    // ---------------- Setup
    //
    char lhs[] = "Hello world";
    char rhs[] = "Hello World";
    CxxTest::setAbortTestOnFail(false);

    // ---------------- Exercise
    //
    TS_ASSERT_SAME_DATA(lhs, rhs, sizeof(lhs));

    TS_WARN("This is expected message for test_ASSERT_SAME_DATA_AbortTestOnFail_False");
}

//! Runs TS_ASSERT_THROWS when no exception is thrown and AbortTestOnFail is false
//!
void UT_AbortTestOnFail::test_ASSERT_THROWS_AbortTestOnFail_False ()
{
    CxxTest::setAbortTestOnFail(false);

    TS_ASSERT_THROWS(doNothing(), std::exception);

    TS_WARN("This is expected message for test_ASSERT_THROWS_AbortTestOnFail_False");
}

//! Runs TS_ASSERT_THROWS_ANYTHING when no exception is thrown and AbortTestOnFail is false
//!
void UT_AbortTestOnFail::test_ASSERT_THROWS_ANYTHING_AbortTestOnFail_False ()
{
    CxxTest::setAbortTestOnFail(false);

    TS_ASSERT_THROWS_ANYTHING(doNothing());

    TS_WARN("This is expected message for test_ASSERT_THROWS_ANYTHING_AbortTestOnFail_False");
}


//! Runs TS_ASSERT_THROWS_NOTHING when an exception is thrown and AbortTestOnFail is false
//!
void UT_AbortTestOnFail::test_ASSERT_THROWS_NOTHING_AbortTestOnFail_False ()
{
    CxxTest::setAbortTestOnFail(false);

    TS_ASSERT_THROWS_NOTHING(throwException());

    TS_WARN("This is expected message for test_ASSERT_THROWS_ANYTHING_AbortTestOnFail_False");
}


//===========================================================================
// End of UT_AbortTestOnFail.cpp
//===========================================================================
