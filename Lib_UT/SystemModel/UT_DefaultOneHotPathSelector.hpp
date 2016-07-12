//===========================================================================
//                           UT_DefaultOneHotPathSelector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_DefaultOneHotPathSelector.hpp
//!
//! Declares test fixture class for testingDefaultOneHotPathSelector
//!
//===========================================================================


#ifndef UT_DEFAULTONEHOTPATHSELECTOR_H__DB0D69F1_F361_4423_3295_BB7ACB274B64__INCLUDED_
  #define UT_DEFAULTONEHOTPATHSELECTOR_H__DB0D69F1_F361_4423_3295_BB7ACB274B64__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class DefaultOneHotPathSelector
//!
//!
class UT_DefaultOneHotPathSelector : public CxxTest::TestSuite
{
  public:
   UT_DefaultOneHotPathSelector() = default;
  ~UT_DefaultOneHotPathSelector() = default;


  // ================ Test Methods ================

  // ---------------- Constructor(s)
  //
  void test_Constructor ();
  void test_Constructor_Inverted ();
  void test_Constructor_ReverseOrder ();
  void test_Constructor_InvertedReverseOrder ();
  void test_Constructor_TooSmallRegister ();

  // ---------------- Select
  //
  void test_Select ();
  void test_Select_Inverted ();
  void test_Select_ReverseOrder ();
  void test_Select_InvertedReverseOrder ();
  void test_Select_Path_Zero ();
  void test_Select_OutOfRange ();

  void test_Deselect ();
  void test_Deselect_OutOfRange ();
};
//
//  End of UT_DefaultOneHotPathSelector class declaration
//---------------------------------------------------------------------------

#endif  // not defined UT_DEFAULTONEHOTPATHSELECTOR_H__DB0D69F1_F361_4423_3295_BB7ACB274B64__INCLUDED_
//===========================================================================
// End of UT_DefaultOneHotPathSelector.hpp
//===========================================================================
