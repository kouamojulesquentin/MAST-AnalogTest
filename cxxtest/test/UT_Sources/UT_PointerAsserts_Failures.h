//===========================================================================
//                           UT_PointerAsserts_Failures.h
//===========================================================================
// Copyright (C) 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_PointerAsserts_Failures.h
//!
//! Declares test fixture class for testing PointerAsserts_Failures
//!
//===========================================================================
// Modifications history:
// Created by JFC on September/28/2015
//===========================================================================

#ifndef UT_POINTERASSERTS_FAILURES_H__CC0578DB_D159_492A_F6B5_BC79880FFE70__INCLUDED_
  #define UT_POINTERASSERTS_FAILURES_H__CC0578DB_D159_492A_F6B5_BC79880FFE70__INCLUDED_

#include <cxxtest/TestSuite.h>


//! Unit tests for new assertions (not from Cxxtest "distribution")
//!
class UT_PointerAsserts_Failures : public CxxTest::TestSuite
{
  public:
   UT_PointerAsserts_Failures();
  ~UT_PointerAsserts_Failures();


  // ================ Test Methods ================

  // ---------------- TS_ASSERT_NOT_NULLPTR
  //
  void test_TS_ASSERT_NULLPTR ();
  void test_TS_ASSERT_NULLPTR_shared_ptr ();
  void test_TS_ASSERT_NULLPTR_unique_ptr ();
  void test_TS_ASSERT_NULLPTR_Exception_inside ();
  void test_TS_ASSERT_NULLPTR_Exception_before ();
  void test_TS_ASSERT_NULLPTR_Exception_after ();

  // ---------------- TS_ASSERT_NOT_NULLPTR
  //
  void test_TS_ASSERT_NOT_NULLPTR ();
  void test_TS_ASSERT_NOT_NULLPTR_shared_ptr ();
  void test_TS_ASSERT_NOT_NULLPTR_unique_ptr ();
  void test_TS_ASSERT_NOT_NULLPTR_Exception_inside ();
  void test_TS_ASSERT_NOT_NULLPTR_Exception_before ();
  void test_TS_ASSERT_NOT_NULLPTR_Exception_after ();

  // ---------------- TS_ASSERT_EQUALS_PTR
  //
  void test_TS_ASSERT_EQUALS_PTR_When_First_NullPtr ();
  void test_TS_ASSERT_EQUALS_PTR_When_First_Not_NullPtr ();
  void test_TS_ASSERT_EQUALS_PTR_Exception_inside ();
  void test_TS_ASSERT_EQUALS_PTR_Exception_before ();
  void test_TS_ASSERT_EQUALS_PTR_Exception_after ();

  // ---------------- TS_ASSERT_DIFFERS_PTR
  //
  void test_TS_ASSERT_DIFFERS_PTR_When_Both_NullPtr ();
  void test_TS_ASSERT_DIFFERS_PTR_When_Both_Not_NullPtr ();
  void test_TS_ASSERT_DIFFERS_PTR_Exception_inside ();
  void test_TS_ASSERT_DIFFERS_PTR_Exception_before ();
  void test_TS_ASSERT_DIFFERS_PTR_Exception_after ();
};
//
//  End of UT_PointerAsserts_Failures class declaration
//---------------------------------------------------------------------------




#endif  // not defined UT_POINTERASSERTS_FAILURES_H__CC0578DB_D159_492A_F6B5_BC79880FFE70__INCLUDED_
//===========================================================================
// End of UT_PointerAsserts_Failures.h
//===========================================================================
