//===========================================================================
//                           UT_DefaultNHotPathSelector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_DefaultNHotPathSelector.hpp
//!
//! Declares test fixture class for testingDefaultNHotPathSelector
//!
//===========================================================================


#ifndef UT_DEFAULTNHOTPATHSELECTOR_H__A72FF5E4_6B6E_42BA_E783_43341DCE4263__INCLUDED_
  #define UT_DEFAULTNHOTPATHSELECTOR_H__A72FF5E4_6B6E_42BA_E783_43341DCE4263__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class DefaultNHotPathSelector
//!
//!
class UT_DefaultNHotPathSelector : public CxxTest::TestSuite
{
  public:
   UT_DefaultNHotPathSelector() = default;
  ~UT_DefaultNHotPathSelector() = default;


  // ================ Test Methods ================

  // ---------------- Constructor(s)
  //
  void test_Constructor ();
  void test_Constructor_Inverted ();
  void test_Constructor_ReverseOrder ();
  void test_Constructor_InvertedReverseOrder ();
  void test_Constructor_Multiple ();
  void test_Constructor_Multiple_Inverted ();
  void test_Constructor_Multiple_ReverseOrder ();
  void test_Constructor_Multiple_InvertedReverseOrder ();
  void test_Constructor_TooSmallRegister ();
  void test_Constructor_nullptr_Register ();

  // ---------------- Select
  //
  void test_Select ();
  void test_Select_Inverted ();
  void test_Select_ReverseOrder ();
  void test_Select_InvertedReverseOrder ();
  void test_Select_Path_Zero ();
  void test_Select_Multiple ();
  void test_Select_Multiple_Inverted ();
  void test_Select_Multiple_ReverseOrder ();
  void test_Select_Multiple_InvertedReverseOrder ();
  void test_Select_OutOfRange ();

  // ---------------- Deselect
  //
  void test_Deselect ();
  void test_Deselect_Multiple ();
  void test_Deselect_Multiple_InvertedReverseOrder ();
  void test_Deselect_CannotSelectNone ();
  void test_Deselect_CannotSelectNone_LastOne ();
  void test_Deselect_OutOfRange ();
};
//
//  End of UT_DefaultNHotPathSelector class declaration
//---------------------------------------------------------------------------

#endif  // not defined UT_DEFAULTNHOTPATHSELECTOR_H__A72FF5E4_6B6E_42BA_E783_43341DCE4263__INCLUDED_

//===========================================================================
// End of UT_DefaultNHotPathSelector.hpp
//===========================================================================
