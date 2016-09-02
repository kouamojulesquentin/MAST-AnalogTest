//===========================================================================
//                           UT_FromSutUpdater.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_FromSutUpdater.hpp
//!
//! Declares test fixture class for testing FromSutUpdater
//!
//===========================================================================


#ifndef UT_FROMSUTUPDATER_H__DDFB189C_B6E2_407B_A4AF_360AA92FBB__INCLUDED_
  #define UT_FROMSUTUPDATER_H__DDFB189C_B6E2_407B_A4AF_360AA92FBB__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class FromSutUpdater
//!
//!
class UT_FromSutUpdater : public CxxTest::TestSuite
{
  public:
   UT_FromSutUpdater() = default;
  ~UT_FromSutUpdater() = default;

  // ================ Test Methods ================

  void test_Constructor ();

  void test_UpdateRegisters_0 ();
  void test_UpdateRegisters_1 ();
  void test_UpdateRegisters_2 ();
  void test_UpdateRegisters_3 ();

  void test_UpdateRegisters_Vector_TooSmall ();
  void test_UpdateRegisters_Vector_TooLarge ();
  void test_UpdateRegisters_Vector_WrongOrder ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_FromSutUpdater class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_FROMSUTUPDATER_H__DDFB189C_B6E2_407B_A4AF_360AA92FBB__INCLUDED_

//===========================================================================
// End of UT_FromSutUpdater.hpp
//===========================================================================
