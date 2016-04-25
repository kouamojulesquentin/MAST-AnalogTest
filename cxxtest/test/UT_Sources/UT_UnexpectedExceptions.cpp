//===========================================================================
//                           UT_UnexpectedExceptions.cpp
//===========================================================================
// Copyright © 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_UnexpectedExceptions.cpp
//!
//! Implements test fixture for testing unexpected exceptions
//===========================================================================


#include "UT_UnexpectedExceptions.h"
#include <stdexcept>

namespace
{
    //! Compares a single value without throwing an exception
    //!
    class NotThrowingPredicate
    {
        public: bool operator()(int x) const
        {
            return x % 2 == 1;
        }
    };

    //! Compares two values without throwing an exception
    //!
    class NotThrowingRelation
    {
        public: bool operator()(int x, int y) const
        {
            return x == y;
        }
    };


    //! Throws and exception instead of comparing a value to something
    //!
    class ThrowingPredicate
    {
        public: bool operator()(int x) const
        {
            throw std::exception();
            return x % 2 == 1;
        }
    };

    //! Throws and exception instead of comparing two values
    //!
    class ThrowingRelation
    {
        public: bool operator()(int x, int y) const
        {
            throw std::exception();
            return x == y;
        }
    };

    void ThrowSomething()
    {
      throw std::invalid_argument("Invalid argument in 'ThrowSomething'");
    }
} // End of unnamed namespace


UT_UnexpectedExceptions::UT_UnexpectedExceptions ()
{
}



UT_UnexpectedExceptions::~UT_UnexpectedExceptions ()
{
}


//! Initializes test (called for each test)
//!
//!
void UT_UnexpectedExceptions::setUp ()
{
}


//! Cleanups test (called for each test)
//!
//!
void UT_UnexpectedExceptions::tearDown ()
{
}

//! Throws and exception instead of returning a double
//!
double UT_UnexpectedExceptions::getDouble_Throwing ()
{
    throw std::exception();
    return 4321.1234;
}

//! Throws and exception instead of returning an int
//!
int UT_UnexpectedExceptions::getInt_Throwing ()
{
    throw std::invalid_argument("You are dead !!!");
    return 0xDEAD;
}

//! Throws an exception instead of returning a pointer to int
//!
const int* UT_UnexpectedExceptions::getIntPointer_Throwing ()
{
    static const int value = 0;
    throw std::invalid_argument("How boy");
    return &value;
}

//! Throws and exception instead of returning a string
//!
const char* UT_UnexpectedExceptions::getString_Throwing ()
{
    throw std::exception();
    static const char text[] = "Hello test";
    return text;
}


//! Runs TS_ASSERT while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_ASSERT ()
{
    int value = 1;
    TS_ASSERT(getInt_Throwing() == value);
}

//! Runs TS_ASSERT_FALSE while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_ASSERT_FALSE ()
{
    int value = 45;
    TS_ASSERT_FALSE(getInt_Throwing() == value);
}

//! Runs TS_ASSERT_EQUALS while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_ASSERT_EQUALS ()
{
    TS_ASSERT_EQUALS(getInt_Throwing(), 1);
}

//! Runs TSA_ASSERT_EQUALS with, non pointer, value while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_ASSERT_EQUALS_AUTO_Value ()
{
    TSA_ASSERT_EQUALS(getInt_Throwing(), 1);
}

//! Runs TSA_ASSERT_EQUALS with, pointer value while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_ASSERT_EQUALS_AUTO_Pointer ()
{
    int value = 0;
    TSA_ASSERT_EQUALS(getIntPointer_Throwing(), &value);
}


//! Runs TS_ASSERT_DIFFERS while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_ASSERT_DIFFERS ()
{
    int value = 2;
    TS_ASSERT_DIFFERS(getInt_Throwing(), value);
}


//! Runs TS_ASSERT_GREATER_THAN while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_ASSERT_GREATER_THAN ()
{
    int value = 0;
    TS_ASSERT_GREATER_THAN(getInt_Throwing(), value);
}

//! Runs TS_ASSERT_GREATER_THAN_EQUALS while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_ASSERT_GREATER_THAN_EQUAL ()
{
    int value = 1;
    TS_ASSERT_GREATER_THAN_EQUALS(getInt_Throwing(), value);
}



//! Runs TS_ASSERT_LESS_THAN while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_ASSERT_LESS_THAN ()
{
    int value = 2;
    TS_ASSERT_LESS_THAN(getInt_Throwing(), value);
}

//! Runs TS_ASSERT_LESS_THAN_EQUALS while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_ASSERT_LESS_THAN_EQUAL ()
{
    int value = 2;
    TS_ASSERT_LESS_THAN_EQUALS(getInt_Throwing(), value);
}

//! Runs TS_ASSERT_DELTA while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_ASSERT_DELTA ()
{
    double value         = 2.0;
    double acceptedDelta = 1e-7;
    TS_ASSERT_DELTA(getDouble_Throwing(), value, acceptedDelta);
}



//! Runs  TS_ASSERT_PREDICATE while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_ASSERT_PREDICATE ()
{
    int value = 29;
    TS_ASSERT_PREDICATE(ThrowingPredicate(), value);
}

//! Runs  TS_ASSERT_RELATION while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_ASSERT_RELATION ()
{
    int value = 2;
    TS_ASSERT_RELATION(ThrowingRelation(), 1, value);
}

//! Runs  TS_ASSERT_RELATION while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_ASSERT_SAME_DATA ()
{
    // ---------------- Setup
    //
    char lhs[] = "Hello work";

    // ---------------- Exercise
    //
    TS_ASSERT_SAME_DATA(lhs, getString_Throwing(), sizeof(lhs));
}


//! Runs TS_ASSERT while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_Message_ASSERT ()
{
    int value = 1;
    TSM_ASSERT("Message from TSM_ASSERT", getInt_Throwing() == value);
}

//! Runs TS_ASSERT_FALSE while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_Message_ASSERT_FALSE ()
{
    int value = 3;
    TSM_ASSERT_FALSE("Message from TSM_ASSERT", getInt_Throwing() == value);
}


//! Runs TS_ASSERT_EQUALS while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_Message_ASSERT_EQUALS ()
{
    int value = 1;
    TSM_ASSERT_EQUALS("Message from TSM_ASSERT_EQUALS", getInt_Throwing(), value);
}

//! Runs TS_ASSERT_EQUALS with non pointer value, while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_Message_ASSERT_EQUALS_AUTO_Value ()
{
    int value = 1;
    TSMA_ASSERT_EQUALS("Message from TSMA_ASSERT_EQUALS", getInt_Throwing(), value);
}

//! Runs TS_ASSERT_EQUALS with pointer value, while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_Message_ASSERT_EQUALS_AUTO_Pointer ()
{
    int value = 0;
    TSMA_ASSERT_EQUALS("Message from TSMA_ASSERT_EQUALS with", getIntPointer_Throwing(), &value);
}



//! Runs TS_ASSERT_DIFFERS while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_Message_ASSERT_DIFFERS ()
{
    int value = 2;
    TSM_ASSERT_DIFFERS("Message from TSM_ASSERT_DIFFERS", getInt_Throwing(), value);
}


//! Runs TS_ASSERT_GREATER_THAN while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_Message_ASSERT_GREATER_THAN ()
{
    int value = 0;
    TSM_ASSERT_GREATER_THAN("Message from TSM_ASSERT_GREATER_THAN", getInt_Throwing(), value);
}

//! Runs TS_ASSERT_GREATER_THAN_EQUALS while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_Message_ASSERT_GREATER_THAN_EQUAL ()
{
    int value = 1;
    TSM_ASSERT_GREATER_THAN_EQUALS("Message from TSM_ASSERT_GREATER_THAN_EQUALS", getInt_Throwing(), value);
}


//! Runs TS_ASSERT_LESS_THAN while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_Message_ASSERT_LESS_THAN ()
{
    int value = 2;
    TSM_ASSERT_LESS_THAN("Message from TSM_ASSERT_LESS_THAN", getInt_Throwing(), value);
}

//! Runs TS_ASSERT_LESS_THAN_EQUALS while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_Message_ASSERT_LESS_THAN_EQUAL ()
{
    int value = 2;
    TSM_ASSERT_LESS_THAN_EQUALS("Message from TSM_ASSERT_LESS_THAN_EQUALS", getInt_Throwing(), value);
}

//! Runs TS_ASSERT_DELTA while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_Message_ASSERT_DELTA ()
{
    double value         = 2.0;
    double acceptedDelta = 1e-7;
    TSM_ASSERT_DELTA("Message from TSM_ASSERT_DELTA", getDouble_Throwing(), value, acceptedDelta);
}


//! Runs  TS_ASSERT_IS_INFINITE while throwing an exception
//!
//+void UT_UnexpectedExceptions::test_Unexpected_Within_Message_ASSERT_IS_INFINITE ()
//+{
//+    TSM_ASSERT_IS_INFINITE("Message from TSM_ASSERT_IS_INFINITE", getDouble_Throwing() / 0.0);
//+}


//! Runs  TS_ASSERT_IS_NAN while throwing an exception
//!
//+void UT_UnexpectedExceptions::test_Unexpected_Within_Message_ASSERT_IS_NAN ()
//+{
//+    TSM_ASSERT_IS_NAN("Message from TSM_ASSERT_IS_NAN", getDouble_Throwing() / 0.0);
//+}


//! Runs  TS_ASSERT_PREDICATE while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_Message_ASSERT_PREDICATE ()
{
    int value = 29;
    TSM_ASSERT_PREDICATE("Message from TSM_ASSERT_PREDICATE", ThrowingPredicate(), value);
}

//! Runs  TS_ASSERT_RELATION while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_Message_ASSERT_RELATION ()
{
    int lhs = 1;
    int rhs = 2;
    TSM_ASSERT_RELATION("Message from TSM_ASSERT_RELATION", ThrowingRelation(), lhs, rhs);
}

//! Runs  TS_ASSERT_RELATION while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_Message_ASSERT_SAME_DATA ()
{
    // ---------------- Setup
    //
    char lhs[] = "Hello work";

    // ---------------- Exercise
    //
    TSM_ASSERT_SAME_DATA("Message from TSM_ASSERT_SAME_DATA", lhs, getString_Throwing(), sizeof(lhs));
}


//! Runs TS_ASSERT after throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Before_ASSERT ()
{
    int value    = 1;
    int expected = getInt_Throwing();
    TS_ASSERT(expected == value);
}

//! Runs TS_ASSERT_FALSE after throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Before_ASSERT_FALSE ()
{
    int value    = 9;
    int expected = getInt_Throwing();
    TS_ASSERT_FALSE(expected == value);
}

//! Runs TS_ASSERT_EQUALS after throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Before_ASSERT_EQUALS ()
{
    int value    = 1;
    int expected = getInt_Throwing();

    TS_ASSERT_EQUALS(expected, value);
}

//! Runs TSA_ASSERT_EQUALS with non pointer value, after throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Before_ASSERT_EQUALS_AUTO_Value ()
{
    int value    = 1;
    int expected = getInt_Throwing();

    TSA_ASSERT_EQUALS(expected, value);
}

//! Runs TSA_ASSERT_EQUALS with pointer value, after throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Before_ASSERT_EQUALS_AUTO_Pointer()
{
    const int* pData = getIntPointer_Throwing();
    TSA_ASSERT_EQUALS(pData, pData);
}




//! Runs TS_ASSERT_DIFFERS after throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Before_ASSERT_DIFFERS ()
{
    int value    = 2;
    int expected = getInt_Throwing();
    TS_ASSERT_DIFFERS(expected, value);
}


//! Runs TS_ASSERT_GREATER_THAN after throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Before_ASSERT_GREATER_THAN ()
{
    int value    = 0;
    int expected = getInt_Throwing();
    TS_ASSERT_GREATER_THAN(expected, value);
}

//! Runs TS_ASSERT_GREATER_THAN_EQUALS after throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Before_ASSERT_GREATER_THAN_EQUAL ()
{
    int value    = 1;
    int expected = getInt_Throwing();
    TS_ASSERT_GREATER_THAN_EQUALS(expected, value);
}



//! Runs TS_ASSERT_LESS_THAN after throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Before_ASSERT_LESS_THAN ()
{
    int value    = 2;
    int expected = getInt_Throwing();
    TS_ASSERT_LESS_THAN(expected, value);
}

//! Runs TS_ASSERT_LESS_THAN_EQUALS after throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Before_ASSERT_LESS_THAN_EQUAL ()
{
    int value    = 2;
    int expected = getInt_Throwing();
    TS_ASSERT_LESS_THAN_EQUALS(expected, value);
}

//! Runs TS_ASSERT_DELTA after throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Before_ASSERT_DELTA ()
{
    double value         = 2.0;
    double acceptedDelta = 1e-7;
    double expected = getDouble_Throwing();
    TS_ASSERT_DELTA(expected, value, acceptedDelta);
}




//! Runs TS_ASSERT_PREDICATE after throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Before_ASSERT_PREDICATE ()
{
    throw std::exception();
    int value = 29;
    TS_ASSERT_PREDICATE(NotThrowingPredicate(), value);
}

//! Runs TS_ASSERT_RELATION after throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Before_ASSERT_RELATION ()
{
    throw std::exception();

    int lhs = 1;
    int rhs = 2;
    TS_ASSERT_RELATION(NotThrowingRelation(), lhs, rhs);
}

//! Runs TS_ASSERT_RELATION after throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Before_ASSERT_SAME_DATA ()
{
    // ---------------- Setup
    //
    char lhs[] = "Hello work";
    char rhs[] = "Hello work";

    throw std::exception();

    // ---------------- Exercise
    //
    TS_ASSERT_SAME_DATA(lhs, rhs, sizeof(lhs));
}


//! Runs TS_ASSERT before throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_After_ASSERT ()
{
    int lhs = 1;
    int rhs = 1;
    TS_ASSERT(lhs == rhs);
    throw std::exception();
}


//! Runs TS_ASSERT_FALSE before throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_After_ASSERT_FALSE ()
{
    int lhs = 1;
    int rhs = 9;
    TS_ASSERT_FALSE(lhs == rhs);
    throw std::exception();
}

//! Runs TS_ASSERT_EQUALS before throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_After_ASSERT_EQUALS ()
{
    int lhs = 1;
    int rhs = 1;
    TS_ASSERT_EQUALS(lhs, rhs);
    throw std::exception();
}

//! Runs TSA_ASSERT_EQUALS with non pointer value, before throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_After_ASSERT_EQUALS_AUTO_Value ()
{
    int lhs = 2;
    int rhs = 2;
    TSA_ASSERT_EQUALS(lhs, rhs);
    throw std::exception();
}

//! Runs TSA_ASSERT_EQUALS with pointer value, before throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_After_ASSERT_EQUALS_AUTO_Pointer()
{
    int value  = 0;
    TSA_ASSERT_EQUALS(&value, &value);
    throw std::exception();
}


//! Runs TS_ASSERT_DIFFERS before throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_After_ASSERT_DIFFERS ()
{
    int lhs = 3;
    int rhs = 2;
    TS_ASSERT_DIFFERS(lhs, rhs);
    throw std::exception();
}


//! Runs TS_ASSERT_GREATER_THAN before throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_After_ASSERT_GREATER_THAN ()
{
    int lhs = 8;
    int rhs = 2;
    TS_ASSERT_GREATER_THAN(lhs, rhs);
    throw std::exception();
}

//! Runs TS_ASSERT_GREATER_THAN_EQUALS before throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_After_ASSERT_GREATER_THAN_EQUAL ()
{
    int lhs = 7;
    int rhs = 2;
    TS_ASSERT_GREATER_THAN_EQUALS(lhs, rhs);
    throw std::exception();
}


//! Runs TS_ASSERT_LESS_THAN before throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_After_ASSERT_LESS_THAN ()
{
    int lhs = 1;
    int rhs = 2;
    TS_ASSERT_LESS_THAN(lhs, rhs);
    throw std::exception();
}

//! Runs TS_ASSERT_LESS_THAN_EQUALS before throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_After_ASSERT_LESS_THAN_EQUAL ()
{
    int lhs = 2;
    int rhs = 2;
    TS_ASSERT_LESS_THAN_EQUALS(lhs, rhs);
    throw std::exception();
}

//! Runs TS_ASSERT_DELTA before throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_After_ASSERT_DELTA ()
{
    double value         = 2.0001;
    double expected      = 2.0;
    double acceptedDelta = 1e-3;
    TS_ASSERT_DELTA(value, expected, acceptedDelta);
    throw std::exception();
}



//! Runs TS_ASSERT_PREDICATE before throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_After_ASSERT_PREDICATE ()
{
    int value_a = 29;
    TS_ASSERT_PREDICATE(NotThrowingPredicate(), value_a);
    throw std::exception();
    int value_b = 28;
    TS_ASSERT_PREDICATE(NotThrowingPredicate(), value_b);
}

//! Runs TS_ASSERT_RELATION before throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_After_ASSERT_RELATION ()
{
    int lhs = 2;
    int rhs = 2;
    TS_ASSERT_RELATION(NotThrowingRelation(), lhs, rhs);
    throw std::out_of_range("input expected to be [0, 100[");
}

//! Runs TS_ASSERT_RELATION before throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_After_ASSERT_SAME_DATA ()
{
    // ---------------- Setup
    //
    char lhs[] = "Hello work";
    char rhs[] = "Hello work";

    // ---------------- Exercise
    //
    TS_ASSERT_SAME_DATA(lhs, rhs, sizeof(lhs));

    throw std::invalid_argument("Invalid argument 'foo'");
}


//! Runs TS_ASSERT while throwing an exception within the macro and after that macro
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_and_After_ASSERT ()
{
    int value = 1;
    TS_ASSERT(getInt_Throwing() == value);
    ThrowSomething();
}

//! Runs TS_ASSERT_FALSE while throwing an exception within the macro and after that macro
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_and_After_ASSERT_FALSE ()
{
    int value = 3;
    TS_ASSERT_FALSE(getInt_Throwing() == value);
    ThrowSomething();
}


//! Runs TS_ASSERT_EQUALS while throwing an exception within the macro and after that macro
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_and_After_ASSERT_EQUALS ()
{
    int value = 1;
    TS_ASSERT_EQUALS(getInt_Throwing(), value);
    ThrowSomething();
}

//! Runs TS_ASSERT_EQUALS with non pointer value, while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_and_After_ASSERT_EQUALS_AUTO_Value ()
{
    int value = 1;
    TSA_ASSERT_EQUALS(getInt_Throwing(), value);
    ThrowSomething();
}

//! Runs TS_ASSERT_EQUALS with pointer value, while throwing an exception
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_and_After_ASSERT_EQUALS_AUTO_Pointer ()
{
    int value = 0;
    TSA_ASSERT_EQUALS(getIntPointer_Throwing(), &value);
    ThrowSomething();
}



//! Runs TS_ASSERT_DIFFERS while throwing an exception within the macro and after that macro
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_and_After_ASSERT_DIFFERS ()
{
    int value = 2;
    TS_ASSERT_DIFFERS(getInt_Throwing(), value);
    ThrowSomething();
}


//! Runs TS_ASSERT_GREATER_THAN while throwing an exception within the macro and after that macro
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_and_After_ASSERT_GREATER_THAN ()
{
    int value = 0;
    TS_ASSERT_GREATER_THAN(getInt_Throwing(), value);
    ThrowSomething();
}

//! Runs TS_ASSERT_GREATER_THAN_EQUALS while throwing an exception within the macro and after that macro
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_and_After_ASSERT_GREATER_THAN_EQUAL ()
{
    int value = 1;
    TS_ASSERT_GREATER_THAN_EQUALS(getInt_Throwing(), value);
    ThrowSomething();
}


//! Runs TS_ASSERT_LESS_THAN while throwing an exception within the macro and after that macro
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_and_After_ASSERT_LESS_THAN ()
{
    int value = 2;
    TS_ASSERT_LESS_THAN(getInt_Throwing(), value);
    ThrowSomething();
}

//! Runs TS_ASSERT_LESS_THAN_EQUALS while throwing an exception within the macro and after that macro
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_and_After_ASSERT_LESS_THAN_EQUAL ()
{
    int value = 2;
    TS_ASSERT_LESS_THAN_EQUALS(getInt_Throwing(), value);
    ThrowSomething();
}

//! Runs TS_ASSERT_DELTA while throwing an exception within the macro and after that macro
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_and_After_ASSERT_DELTA ()
{
    double value         = 2.0;
    double acceptedDelta = 1e-7;
    TS_ASSERT_DELTA(getDouble_Throwing(), value, acceptedDelta);
    ThrowSomething();
}


//! Runs  TS_ASSERT_PREDICATE while throwing an exception within the macro and after that macro
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_and_After_ASSERT_PREDICATE ()
{
    int value = 29;
    TS_ASSERT_PREDICATE(ThrowingPredicate(), value);
    ThrowSomething();
}

//! Runs  TS_ASSERT_RELATION while throwing an exception within the macro and after that macro
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_and_After_ASSERT_RELATION ()
{
    int lhs = 1;
    int rhs = 2;
    TS_ASSERT_RELATION(ThrowingRelation(), lhs, rhs);
    ThrowSomething();
}

//! Runs  TS_ASSERT_RELATION while throwing an exception within the macro and after that macro
//!
void UT_UnexpectedExceptions::test_Unexpected_Within_and_After_ASSERT_SAME_DATA ()
{
    char lhs[] = "Hello work";
    TS_ASSERT_SAME_DATA(lhs, getString_Throwing(), sizeof(lhs));
    ThrowSomething();
}


//===========================================================================
// End of UT_UnexpectedExceptions.cpp
//===========================================================================
