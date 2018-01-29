//===========================================================================
//                           UT_AST_BasedNumber.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_AST_BasedNumber.hpp
//!
//! Declares test fixture class for testing AST_BasedNumber sub-classes
//!
//===========================================================================

#ifndef UT_AST_BASEDNUMBER_H__8E339426_E958_4E24_1EA5_57D437CC2555__INCLUDED_
  #define UT_AST_BASEDNUMBER_H__8E339426_E958_4E24_1EA5_57D437CC2555__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class AST_BasedNumber
//!
class UT_AST_BasedNumber : public CxxTest::TestSuite
{
  public:
   UT_AST_BasedNumber() = default;
  ~UT_AST_BasedNumber() = default;

  // ================ Test Methods ================

  // ---------------- Constructors
  //
  void test_Constructor_valid ();
  void test_Constructor_not_valid ();

  // ---------------- SizeExpr
  //
  void test_SizeExpr ();

  // ---------------- AsText
  //
  void test_AsText_unsized ();
  void test_AsText_sized ();

  // ---------------- AsBinaryVector
  //
  void test_AsBinaryVector_unsized ();
  void test_AsBinaryVector_sized ();
  void test_AsBinaryVector_target_size ();
  void test_AsBinaryVector_target_size_bad_truncation ();
};
//
//  End of UT_AST_BasedNumber class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_AST_BASEDNUMBER_H__8E339426_E958_4E24_1EA5_57D437CC2555__INCLUDED_
//===========================================================================
// End of UT_AST_BasedNumber.hpp
//===========================================================================
