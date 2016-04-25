//===========================================================================
//                           UT_DataDriven_Failures.h
//===========================================================================
// Copyright (C) 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_DataDriven_Failures.h
//!
//! Declares test fixture class for testing DataDriven
//!
//===========================================================================
// Modifications history:
// Created by JFC on September/16/2015
//===========================================================================


#ifndef UT_DATADRIVEN_FAILURES_H__6E32CCC8_9332_4B3C_E0B6_A218C4671B8E__INCLUDED_
  #define UT_DATADRIVEN_FAILURES_H__6E32CCC8_9332_4B3C_E0B6_A218C4671B8E__INCLUDED_

#include <cxxtest/TestSuite.h>


//! Unit tests for class data driven tests
//!
class UT_DataDriven_Failures : public CxxTest::TestSuite
{
  public:
   UT_DataDriven_Failures() = default;
  ~UT_DataDriven_Failures() = default;


  // ================ Test Methods ================
  //

  // ---------------- Asserts with Input and Output data
  //
  void test_ParameterCountMismatch ();
  void test_Array_Int_Int       ();
  void test_Initializer_Int_Int ();
  void test_Vector_Int_Int      ();
  void test_StdString_Int ();
  void test_StdString_Compound ();
  void test_Standalone_Function ();
  void test_StaticMember_Function ();
  void test_ASSERT ();
  void test_ASSERT_TRUE ();
  void test_ASSERT_FALSE ();
  void test_ASSERT_EQUALS_AUTO_Value ();
  void test_ASSERT_EQUALS_AUTO_Pointer ();
  void test_ASSERT_DIFFERS ();
  void test_ASSERT_GREATER_THAN_Equal ();
  void test_ASSERT_GREATER_THAN_EQUAL_GreaterThan ();
  void test_ASSERT_LESS_THAN_Equal ();
  void test_ASSERT_LESS_THAN_EQUAL_GreaterThan ();
  void test_ASSERT_DELTA ();
  void test_ASSERT_SAME_DATA ();
  void test_setAbortDataDrivenTestsOnFailure_True ();


  // ---------------- Exceptions with input and output data
  //
  void test_Expected_Exception ();
  void test_Throw_Nothing ();
  void test_Unexpected_UnknownType_Exception ();
  void test_Unexpected_Bool_Exception ();
  void test_Unexpected_Int_Exception ();
  void test_Unexpected_Std_Exception ();


  // ---------------- Asserts with only single set of data
  //
  void test_Int ();
  void test_StdString ();
  void test_ASSERT_PREDICATE ();
  void test_ASSERT_RELATION ();

  // ---------------- Exceptions with only single set of data
  //
  void test_SingleData_Expected_Exception ();
  void test_SingleData_Throw_Nothing ();
  void test_SingleData_Unexpected_UnknownType_Exception ();
  void test_SingleData_Unexpected_Std_Exception ();

  // ---------------- Asserts with mixed data (input and expected in same collection but with binary test function)
  //
  void test_Mixed_StdString_Int ();
  void test_Mixed_StdString_Compound ();

  // ---------------- With not assert macros (in same "master" test)
  //
  void test_WithNoAssert_Before ();
  void test_WithNoAssert_After ();

  // ---------------- Missing assertion for some some test(s)
  //
  void test_MissingAssertion_First ();
  void test_MissingAssertion_Middle ();
  void test_MissingAssertion_Last ();

  // ================ Other Methods ================
  //
  static void StaticMemberTestFunction (int input, int expected);
};
//
//  End of UT_DataDriven_Failures class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_DATADRIVEN_FAILURES_H__6E32CCC8_9332_4B3C_E0B6_A218C4671B8E__INCLUDED_
//===========================================================================
// End of UT_DataDriven_Failures.h
//===========================================================================
