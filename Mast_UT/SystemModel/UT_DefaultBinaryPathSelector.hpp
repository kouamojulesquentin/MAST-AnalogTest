//===========================================================================
//                           UT_DefaultBinaryPathSelector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_DefaultBinaryPathSelector.hpp
//!
//! Declares test fixture class for testingDefaultBinaryPathSelector
//!
//===========================================================================


#ifndef UT_DEFAULTBINARYPATHSELECTOR_H__F912C833_68B1_42E0_B282_B309583C44AC__INCLUDED_
  #define UT_DEFAULTBINARYPATHSELECTOR_H__F912C833_68B1_42E0_B282_B309583C44AC__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class DefaultBinaryPathSelector
//!
//!
class UT_DefaultBinaryPathSelector : public CxxTest::TestSuite
{
  public:
   UT_DefaultBinaryPathSelector() = default;
  ~UT_DefaultBinaryPathSelector() = default;


  // ================ Test Methods ================

  // ---------------- Constructor(s)
  //
  void test_Constructor_CannotSelectNone ();
  void test_Constructor_CanSelectNone ();
  void test_Constructor_nullptr_Register ();

  // ---------------- Select
  //
  void test_Select_CannotSelectNone ();
  void test_Select_CanSelectNone ();
  void test_Select_CanSelectNoneInverted ();
  void test_Select_Path_Zero ();

  void test_Deselect ();
};
//
//  End of UT_DefaultBinaryPathSelector class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_DEFAULTBINARYPATHSELECTOR_H__F912C833_68B1_42E0_B282_B309583C44AC__INCLUDED_
//===========================================================================
// End of UT_DefaultBinaryPathSelector.hpp
//===========================================================================
