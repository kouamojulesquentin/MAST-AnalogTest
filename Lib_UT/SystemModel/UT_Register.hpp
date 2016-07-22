//===========================================================================
//                           UT_Register.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_Register.hpp
//!
//! Declares test fixture class for testing Register
//!
//===========================================================================


#ifndef UT_REGISTER_H__78C4B8C9_4565_49BE_2D84_20A2F5A162DD__INCLUDED_
  #define UT_REGISTER_H__78C4B8C9_4565_49BE_2D84_20A2F5A162DD__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class Register
//!
class UT_Register : public CxxTest::TestSuite
{
  public:
   UT_Register() = default;
  ~UT_Register() = default;


  // ================ Test Methods ================

  // ---------------- Constructor
  //
  void test_Constructor ();

  // ---------------- BinaryVector setters
  //
  void test_SetBypass ();
  void test_SetBypass_TooSmall ();
  void test_SetBypass_TooLarge ();

  void test_SetToSut ();
  void test_SetToSut_DifferentSize ();


  void test_SetExpectedFromSut ();
  void test_SetExpectedFromSut_DifferentSize ();

  void test_SetFromSut_DifferingFromExpected_WithoutCheck ();
  void test_SetFromSut_DifferingFromExpected_WithCheck ();
  void test_SetFromSut_DifferentSize ();

  void test_UpdateLastToSut ();

  void test_HoldValue ();

  // ---------------- Other setters
  //
  void test_SetToSut_uint8 ();
  void test_SetToSut_uint16 ();
  void test_SetToSut_uint32 ();
  void test_SetToSut_uint64 ();
  void test_SetToSut_int8 ();
  void test_SetToSut_int16 ();
  void test_SetToSut_int32 ();
  void test_SetToSut_int64 ();

  void test_SetExpectedFromSut_uint8 ();
  void test_SetExpectedFromSut_uint16 ();
  void test_SetExpectedFromSut_uint32 ();
  void test_SetExpectedFromSut_uint64 ();
  void test_SetExpectedFromSut_int8 ();
  void test_SetExpectedFromSut_int16 ();
  void test_SetExpectedFromSut_int32 ();
  void test_SetExpectedFromSut_int64 ();

  // ---------------- Other getters
  //
  void test_LastFromSut_uint8 ();
  void test_LastFromSut_uint16 ();
  void test_LastFromSut_uint32 ();
  void test_LastFromSut_uint64 ();
  void test_LastFromSut_int8 ();
  void test_LastFromSut_int16 ();
  void test_LastFromSut_int32 ();
  void test_LastFromSut_int64 ();

  // ---------------- Mismatches
  //
  void test_ResetMismatches ();
};
//
//  End of UT_Register class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_REGISTER_H__78C4B8C9_4565_49BE_2D84_20A2F5A162DD__INCLUDED_
//===========================================================================
// End of UT_Register.hpp
//===========================================================================
