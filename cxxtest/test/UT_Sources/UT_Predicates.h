//===========================================================================
//                           UT_Predicates.h
//===========================================================================
// Copyright (C) 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_Predicates.h
//!
//! Declares test fixture class for testing Predicates
//!
//===========================================================================
// Modifications history:
// Created by JFC on September/25/2015
//===========================================================================

#ifndef UT_PREDICATES_H__CB1CFD92_C757_46DD_CFAB_696AB8275B11__INCLUDED_
  #define UT_PREDICATES_H__CB1CFD92_C757_46DD_CFAB_696AB8275B11__INCLUDED_

#include <cxxtest/TestSuite.h>


//! Unit tests for unary and binary predicates assertions
//!
//! @note Binary predicates are called Relation in CxxTest context
//! @not  Thos tests require at least C++11 because it uses lambda expressions
//!
class UT_Predicates : public CxxTest::TestSuite
{
  public:
   UT_Predicates() = default;
  ~UT_Predicates() = default;

  // ================ Test Methods ================

  // ---------------- Predicate
  //
  void test_ASSERT_PREDICATE_PredicateInstance ();
  void test_ASSERT_PREDICATE_LambdaExpression_Ref ();
  void test_ASSERT_PREDICATE_LambdaExpression ();
  void test_ASSERT_PREDICATE_InstanceMemberMethod ();
  void test_ASSERT_PREDICATE_StaticMemberMethod ();
  void test_ASSERT_PREDICATE_Standalone ();
  void test_ASSERT_THROWS_PREDICATE_LambdaExpression ();


  void test_ASSERT_PREDICATE_PredicateInstance_failure ();
  void test_ASSERT_PREDICATE_LambdaExpression_failure ();
  void test_ASSERT_PREDICATE_StaticMemberMethod_failure ();
  void test_ASSERT_PREDICATE_Standalone_failure ();
  void test_ASSERT_THROWS_PREDICATE_LambdaExpression_failure ();


  // ---------------- Relation
  //
  void test_ASSERT_RELATION_PredicateInstance ();
  void test_ASSERT_RELATION_LambdaExpression_Ref ();
  void test_ASSERT_RELATION_LambdaExpression ();
  void test_ASSERT_RELATION_StaticMemberMethod ();
  void test_ASSERT_RELATION_Standalone ();
  void test_ASSERT_THROWS_RELATION_LambdaExpression ();

  void test_ASSERT_THROWS_RELATION_LambdaExpression_failure ();

  // ================ Other Methods ================

  // ---------------- Private  Methods
  //
  bool        IsOddInstanceMemberPredicate      (int    value) const;
  static bool IsOddStaticMemberPredicate        (int    value);
  static bool IsGreaterThanStaticMemberRelation (double lhs, double rhs);
};
//
//  End of UT_Predicates class declaration
//---------------------------------------------------------------------------




#endif  // not defined UT_PREDICATES_H__CB1CFD92_C757_46DD_CFAB_696AB8275B11__INCLUDED_
//===========================================================================
// End of UT_Predicates.h
//===========================================================================
