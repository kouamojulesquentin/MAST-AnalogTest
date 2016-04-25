//===========================================================================
//                           UT_DataDriven_Successes.h
//===========================================================================
// Copyright (C) 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_DataDriven_Successes.h
//!
//! Declares test fixture class for testing DataDriven
//!
//===========================================================================
// Modifications history:
// Created by JFC on September/16/2015
//===========================================================================


#ifndef UT_TABLEDRIVEN_SUCCESSES_H__676ECDA5_BCEB_41E5_30B0_9A37DBAB834A__INCLUDED_
  #define UT_TABLEDRIVEN_SUCCESSES_H__676ECDA5_BCEB_41E5_30B0_9A37DBAB834A__INCLUDED_
#include <cxxtest/TestSuite.h>


//! Unit tests for class data driven tests when all tests are successful
//!
class UT_DataDriven_Successes : public CxxTest::TestSuite
{
  public:
   UT_DataDriven_Successes() = default;
  ~UT_DataDriven_Successes() = default;


  // ================ Test Methods ================
  //
  // ---------------- Asserts with input and output data
  //
  void test_Array_Int_Int       ();
  void test_Initializer_Int_Int ();
  void test_Vector_Int_Int      ();

  // ---------------- Exceptions with input and output data
  //
  void test_Expected_Exception  ();
  void test_Throw_Nothing       ();

  // ---------------- Setup/Teardown with input and output data
  //
  void test_Setup_Teardown ();

  // ---------------- Asserts with only single set of data
  //
  void test_Int ();
  void test_StdString ();

  // ---------------- Asserts with mixed data (input and expected in same collection but with binary test function)
  //
  void test_Mixed_StdString_Int ();
  void test_Mixed_StdString_Compound ();

  // ---------------- Exceptions with only single set of data
  //
  void test_SingleData_Expected_Exception ();
  void test_SingleData_Throw_Nothing ();

  // ---------------- Setup/Teardown with only single set of data
  //
  void test_SingleData_Setup_Teardown ();

  // ---------------- Mix With not assert macros
  //
  void test_WithNoAssert_Before ();
  void test_WithNoAssert_After ();

  // ---------------- Missing assertion for some some test(s)
  //
  void test_MissingAssertion_First ();
  void test_MissingAssertion_Middle ();
  void test_MissingAssertion_Last ();


  //! @todo [JFC]-[September/16/2015]: Remove "test_No_test_yet_for_Guard" method when all tests are implemented
  //!
  void test_No_test_yet_for_Guard ();

  // ---------------- Private  Methods
  //

  //! Initializes test (called for each test)
  //!
  void setUp();

  //! Cleanups test (called for each test)
  //!
  void tearDown();

  // ---------------- Private  Fields
  //
  int   m_setupCount    = 0;
  int   m_teardownCount = 0;
};
//
//  End of UT_DataDriven_Successes class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_TABLEDRIVEN_SUCCESSES_H__676ECDA5_BCEB_41E5_30B0_9A37DBAB834A__INCLUDED_
//===========================================================================
// End of UT_DataDriven_Successes.h
//===========================================================================
