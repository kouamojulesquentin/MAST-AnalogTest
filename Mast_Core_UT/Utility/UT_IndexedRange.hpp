//===========================================================================
//                           UT_IndexedRange.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_IndexedRange.hpp
//!
//! Declares test fixture class for testing IndexedRange
//!
//===========================================================================


#ifndef UT_INDEXEDRANGE_H__D88F41A_2C26_4107_26AF_1F06B952F8FB__INCLUDED_
  #define UT_INDEXEDRANGE_H__D88F41A_2C26_4107_26AF_1F06B952F8FB__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class IndexedRange
//!
class UT_IndexedRange : public CxxTest::TestSuite
{
  public:
   UT_IndexedRange() = default;
  ~UT_IndexedRange() = default;

  // ================ Test Methods ================

  // ---------------- Constructors
  //
  void test_Constructor_Default ();
  void test_Constructor_SingleIndex ();
  void test_Constructor_DualIndex ();
  void test_CopyConstructor ();

  // ---------------- Width
  //
  void test_Width_TooLarge ();

  // ---------------- Operators
  //
  void test_Operator_EQ_when_Equal ();
  void test_Operator_EQ_when_Not_Equal ();

  void test_Operator_NEQ_when_Equal ();
  void test_Operator_NEQ_when_Not_Equal ();

};
//
//  End of UT_IndexedRange class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_INDEXEDRANGE_H__D88F41A_2C26_4107_26AF_1F06B952F8FB__INCLUDED_
//===========================================================================
// End of UT_IndexedRange.hpp
//===========================================================================
