//===========================================================================
//                           UT_PointerAsserts_Successes.h
//===========================================================================
// Copyright (C) 2015 . All rights reserved.
//
// Project : Cxxtest
//
//! @file UT_PointerAsserts_Successes.h
//!
//! Declares test fixture class for testing PointerAsserts_Failures
//!
//===========================================================================
// Modifications history:
// Created by JFC on September/28/2015
//===========================================================================

#ifndef UT_POINTERASSERTS_SUCCESSES_H__7E655A3F_7556_4728_2B81_6643FA59DE__INCLUDED_
  #define UT_POINTERASSERTS_SUCCESSES_H__7E655A3F_7556_4728_2B81_6643FA59DE__INCLUDED_

#include <cxxtest/TestSuite.h>


//! Unit tests for new assertions (not from Cxxtest "distribution")
//!
class UT_PointerAsserts_Successes : public CxxTest::TestSuite
{
  public:
   UT_PointerAsserts_Successes();
  ~UT_PointerAsserts_Successes();


  // ================ Test Methods ================

  // ---------------- TS_ASSERT_NOT_NULLPTR
  //
  void test_TS_ASSERT_NULLPTR ();
  void test_TS_ASSERT_NULLPTR_shared_ptr ();
  void test_TS_ASSERT_NULLPTR_unique_ptr ();

  // ---------------- TS_ASSERT_NOT_NULLPTR
  //
  void test_TS_ASSERT_NOT_NULLPTR ();
  void test_TS_ASSERT_NOT_NULLPTR_shared_ptr ();
  void test_TS_ASSERT_NOT_NULLPTR_unique_ptr ();

  // ---------------- TS_ASSERT_EQUALS_PTR
  //
  void test_TS_ASSERT_EQUALS_PTR_When_Both_NullPtr ();
  void test_TS_ASSERT_EQUALS_PTR_When_Both_Not_NullPtr ();

  // ---------------- TS_ASSERT_DIFFERS_PTR
  //
  void test_TS_ASSERT_DIFFERS_PTR_When_First_NullPtr ();
  void test_TS_ASSERT_DIFFERS_PTR_When_First_Not_NullPtr ();
};
//
//  End of UT_PointerAsserts_Successes class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_POINTERASSERTS_SUCCESSES_H__7E655A3F_7556_4728_2B81_6643FA59DE__INCLUDED_
//===========================================================================
// End of UT_PointerAsserts_Successes.h
//===========================================================================
