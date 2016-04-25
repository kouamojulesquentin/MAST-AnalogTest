//===========================================================================
//                           UT_Predicates.cpp
//===========================================================================
// Copyright © 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_Predicates.cpp
//!
//! Implements test fixture for testing Predicates
//!
//===========================================================================
// Modifications history:
// Created by JFC on September/25/2015
//===========================================================================


#include <functional>
#include <tuple>

using std::make_tuple;

#include "UT_Predicates.h"
#include <cxxtest/ValueTraits.h>


namespace
{
  //!
  class IsOddPredicate
  {
      public: bool operator()(int x) const
      {
          return x % 2 != 0;
      }
  };

  //! Unary function that returns true when the value is odd
  //!
  bool IsOddStandalonePredicate(int x)
  {
      return x % 2 != 0;
  }

  //! Binary function that returns true when lhs > rhs
  //!
  bool IsGreaterThanStandaloneRelation(double lhs, double rhs)
  {
    return lhs > rhs;
  }

}


//! A member function that may be used, indirectly, as a predicate for TS_ASSERT_PREDICATE
//!
bool UT_Predicates::IsOddInstanceMemberPredicate (int value) const
{
  return (value % 2) != 0;
}

//! Binary static member that returns true when lhs > rhs
//!
bool UT_Predicates::IsGreaterThanStaticMemberRelation(double lhs, double rhs)
{
  return lhs > rhs;
}


//! A static member function that may be used, indirectly, as a predicate for TS_ASSERT_PREDICATE
//!
bool UT_Predicates::IsOddStaticMemberPredicate (int value)
{
  return (value % 2) != 0;
}



//! Tests TS_ASSERT_PREDICATE using a predicate functor instance
//!
void UT_Predicates::test_ASSERT_PREDICATE_PredicateInstance ()
{
  TS_ASSERT_PREDICATE(IsOddPredicate(), 5);
}


//! Tests TS_ASSERT_PREDICATE using a lambda expression reference as predicate
//!
void UT_Predicates::test_ASSERT_PREDICATE_LambdaExpression_Ref ()
{
  auto IsOddLambda = [](int value) -> bool { return (value % 2) != 0; };

  TS_ASSERT_PREDICATE(IsOddLambda, 5);
}

//! Tests TS_ASSERT_PREDICATE using a lambda expression predicate
//!
void UT_Predicates::test_ASSERT_PREDICATE_LambdaExpression ()
{
  TS_ASSERT_PREDICATE([](int v) -> bool { return (v % 2) != 0; }, 5);
}

//! Tests TS_ASSERT_PREDICATE using an instance method (wrapped within a lambda) for the predicate
//!
void UT_Predicates::test_ASSERT_PREDICATE_InstanceMemberMethod ()
{
  auto IsOddMemberWithinLambda = [this](int v) -> bool { return IsOddInstanceMemberPredicate(v); };

  TS_ASSERT_PREDICATE(IsOddMemberWithinLambda, 5);
}


//! Tests TS_ASSERT_PREDICATE using a static method for the predicate
//!
void UT_Predicates::test_ASSERT_PREDICATE_StaticMemberMethod ()
{
  TS_ASSERT_PREDICATE(IsOddStaticMemberPredicate, 5);
}

//! Tests TS_ASSERT_PREDICATE using a standalone function for the predicate
//!
void UT_Predicates::test_ASSERT_PREDICATE_Standalone ()
{
  TS_ASSERT_PREDICATE(IsOddStandalonePredicate, 5);
}

//! Tests TS_ASSERT_THROWS_PREDICATE using a lambda expression predicate
//!
void UT_Predicates::test_ASSERT_THROWS_PREDICATE_LambdaExpression ()
{
  auto exceptionValueCheck = [](int x) { return x == 5; };
  TS_ASSERT_THROWS_PREDICATE( { throw 5; }, int exc, exceptionValueCheck, exc);
}


//! Tests TS_ASSERT_PREDICATE using a predicate functor instance
//!
void UT_Predicates::test_ASSERT_PREDICATE_PredicateInstance_failure ()
{
  int value = 2;
  TS_ASSERT_PREDICATE(IsOddPredicate(), value);
}

//! Tests TS_ASSERT_PREDICATE using a lambda expression predicate
//!
void UT_Predicates::test_ASSERT_PREDICATE_LambdaExpression_failure ()
{
  int value = 4;
  TS_ASSERT_PREDICATE([](int v) -> bool { return (v % 2) != 0; }, value);
}

//! Tests TS_ASSERT_PREDICATE using a static method (wrapped within a lambda) for the predicate
//!
void UT_Predicates::test_ASSERT_PREDICATE_StaticMemberMethod_failure ()
{
  int value = 6;
  TS_ASSERT_PREDICATE(IsOddStaticMemberPredicate, value);
}

//! Tests TS_ASSERT_PREDICATE using a standalone function for the predicate
//!
void UT_Predicates::test_ASSERT_PREDICATE_Standalone_failure ()
{
  int value = 8;
  TS_ASSERT_PREDICATE(IsOddStandalonePredicate, value);
}

//! Tests TS_ASSERT_THROWS_PREDICATE using a lambda expression predicate
//!
void UT_Predicates::test_ASSERT_THROWS_PREDICATE_LambdaExpression_failure ()
{
  auto exceptionValueCheck = [](int x) { return x == 5; };
  TS_ASSERT_THROWS_PREDICATE( { throw 3; }, int exc, exceptionValueCheck, exc);
}

//! Tests TS_ASSERT_RELATION using a binary function object instance
//!
void UT_Predicates::test_ASSERT_RELATION_PredicateInstance ()
{
  TS_ASSERT_RELATION(std::equal_to<double>(), 69.96, 69.96);
}


//! Tests TS_ASSERT_RELATION using a lambda expression reference as relation
//!
void UT_Predicates::test_ASSERT_RELATION_LambdaExpression_Ref ()
{
  auto areEqualRelation = [](auto lhs, auto rhs) -> bool { return lhs == rhs; };

  TS_ASSERT_RELATION(areEqualRelation, 69.96, 69.96);
}


//! Tests TS_ASSERT_RELATION using a lambda expression as relation
//!
void UT_Predicates::test_ASSERT_RELATION_LambdaExpression ()
{
  TS_ASSERT_RELATION([](auto lhs, auto rhs) -> bool { return lhs == rhs; }, 69.96, 69.96);
}

//! Tests TS_ASSERT_RELATION using a standalone function as relation
//!
void UT_Predicates::test_ASSERT_RELATION_StaticMemberMethod ()
{
  auto lhs = 69.96;
  auto rhs = 69.69;
  TS_ASSERT_RELATION(IsGreaterThanStaticMemberRelation, lhs, rhs);
}


//! Tests TS_ASSERT_RELATION using a standalone function as relation
//!
void UT_Predicates::test_ASSERT_RELATION_Standalone ()
{
  auto lhs = 69.96;
  auto rhs = 69.69;
  TS_ASSERT_RELATION(IsGreaterThanStandaloneRelation, lhs, rhs);
}


//! Tests TS_ASSERT_THROWS_RELATION using a lambda expression relation
//!
void UT_Predicates::test_ASSERT_THROWS_RELATION_LambdaExpression ()
{
  auto exceptionValueCheck = [](int x, int y) { return x == y; };
  TS_ASSERT_THROWS_RELATION( { throw 5; }, int exc, exceptionValueCheck, exc, 5);
}


//! Tests TS_ASSERT_THROWS_RELATION using a lambda expression relation
//!
void UT_Predicates::test_ASSERT_THROWS_RELATION_LambdaExpression_failure ()
{
  auto exceptionValueCheck = [](int x, int y) { return x == y; };
  TS_ASSERT_THROWS_RELATION( { throw 5; }, int exc, exceptionValueCheck, exc, 9);
}


//===========================================================================
// End of UT_Predicates.cpp
//===========================================================================
