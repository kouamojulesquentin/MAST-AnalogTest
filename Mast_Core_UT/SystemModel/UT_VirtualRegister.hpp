//===========================================================================
//                           UT_VirtualRegister.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_VirtualRegister.hpp
//!
//! Declares test fixture class for testing VirtualRegister
//!
//===========================================================================


#ifndef UT_VIRTUALREGISTER_H__624723AE_FE56_4639_7A84_3B53D70C9106__INCLUDED_
  #define UT_VIRTUALREGISTER_H__624723AE_FE56_4639_7A84_3B53D70C9106__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class VirtualRegister
//!
class UT_VirtualRegister : public CxxTest::TestSuite
{
  public:
   UT_VirtualRegister() = default;
  ~UT_VirtualRegister() = default;

  // ================ Test Methods ================

  // ---------------- Constructors
  //
  void test_Constructor_Default ();
  void test_Constructor_FromSingleRegister ();

  // ---------------- Append
  //
  void test_Append_First ();
  void test_Append_Second ();
  void test_Append_First_Invalid ();
  void test_Append_Second_Invalid ();

  // ---------------- SetPending
  //
  void test_SetPending ();

  // ---------------- Getters after append
  //
  void test_Getters_when_HasNoRegister ();
  void test_Getters_1_Register ();
  void test_Getters_2_Registers ();
  void test_LastFromSut_Unsigned_1_Register ();
  void test_LastFromSut_Signed_1_Register ();
  void test_LastFromSut_Unsigned_2_Registers ();
  void test_LastFromSut_Signed_2_Registers ();

  // ---------------- SetToSut
  //
  void test_SetToSut_Upto ();
  void test_SetToSut_Downto ();
  void test_SetToSut_when_HasNoRegister ();
  void test_SetToSut_when_Empty ();
//+  void test_SetToSut_when_InvalidBitsCount ();

  // ---------------- SetExpectedFromSut
  //
  void test_SetExpectedFromSut_Upto ();

  // ---------------- Iterators
  //
  void test_begin_end ();
  void test_cbegin_cend ();

  // ================ Other Methods ================
  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_VirtualRegister class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_VIRTUALREGISTER_H__624723AE_FE56_4639_7A84_3B53D70C9106__INCLUDED_
//===========================================================================
// End of UT_VirtualRegister.hpp
//===========================================================================
