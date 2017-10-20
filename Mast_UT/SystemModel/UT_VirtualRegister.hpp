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

  // ---------------- Append
  //
  void test_Append_First ();
  void test_Append_Second ();
  void test_Append_First_Invalid ();
  void test_Append_Second_Invalid ();

  // ---------------- SetPending
  //
  void test_SetPending ();

  // ---------------- SetToSut
  //
  void test_SetToSut ();

};
//
//  End of UT_VirtualRegister class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_VIRTUALREGISTER_H__624723AE_FE56_4639_7A84_3B53D70C9106__INCLUDED_
//===========================================================================
// End of UT_VirtualRegister.hpp
//===========================================================================
