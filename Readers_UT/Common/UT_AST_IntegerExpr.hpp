//===========================================================================
//                           UT_AST_IntegerExpr.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_AST_IntegerExpr.hpp
//!
//! Declares test fixture class for testing AST_IntegerExpr sub-classes
//!
//===========================================================================


#ifndef UT_AST_INTEGEREXPR_H__6F301249_E88D_40BF_B989_92AB30141E__INCLUDED_
  #define UT_AST_INTEGEREXPR_H__6F301249_E88D_40BF_B989_92AB30141E__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for classes derived from AST_IntegerExpr
//!
class UT_AST_IntegerExpr : public CxxTest::TestSuite
{
  public:
  UT_AST_IntegerExpr() = default;
  virtual ~UT_AST_IntegerExpr() = default;

  // ================ Test Methods ================

  // ---------------- Constructors
  //
  void test_IntegerLiteral_Constructor_valid ();
  void test_IntegerLiteral_Constructor_not_valid ();
  void test_IntegerBinaryExpr_Constructor_valid ();
  void test_IntegerBinaryExpr_Constructor_not_valid ();

  // ---------------- Evaluate
  //
  void test_IntegerBinaryExpr_Evaluate_valid ();
  void test_IntegerBinaryExpr_Evaluate_not_valid ();
};
//
//  End of UT_AST_IntegerExpr class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_AST_INTEGEREXPR_H__6F301249_E88D_40BF_B989_92AB30141E__INCLUDED_
//===========================================================================
// End of UT_AST_IntegerExpr.hpp
//===========================================================================
