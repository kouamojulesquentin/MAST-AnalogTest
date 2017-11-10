//===========================================================================
//                           UT_DefaultTableBasedPathSelector.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_DefaultTableBasedPathSelector.hpp
//!
//! Declares test fixture class for testingDefaultTableBasedPathSelector
//!
//===========================================================================

#ifndef UT_DEFAULTTABLEBASEDPATHSELECTOR_H__3E8D480C_4539_45AF_E28D_3568208F62AA__INCLUDED_
  #define UT_DEFAULTTABLEBASEDPATHSELECTOR_H__3E8D480C_4539_45AF_E28D_3568208F62AA__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class DefaultTableBasedPathSelector
//!
class UT_DefaultTableBasedPathSelector : public CxxTest::TestSuite
{
  public:
   UT_DefaultTableBasedPathSelector() = default;
  ~UT_DefaultTableBasedPathSelector() = default;


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
  void test_Select_Path_Zero ();

  void test_Deselect ();

  void test_DebugSelectorInfo ();
};
//
//  End of UT_DefaultTableBasedPathSelector class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_DEFAULTTABLEBASEDPATHSELECTOR_H__3E8D480C_4539_45AF_E28D_3568208F62AA__INCLUDED_
//===========================================================================
// End of UT_DefaultTableBasedPathSelector.hpp
//===========================================================================
