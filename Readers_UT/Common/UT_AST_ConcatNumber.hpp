//===========================================================================
//                           UT_AST_ConcatNumber.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_AST_ConcatNumber.hpp
//!
//! Declares test fixture class for testing AST_ConcatNumber
//!
//===========================================================================

#ifndef UT_AST_CONCATNUMBER_H__7DE9EF47_6F37_4AFA_EBA_3AB98BC8A01C__INCLUDED_
#define UT_AST_CONCATNUMBER_H__7DE9EF47_6F37_4AFA_EBA_3AB98BC8A01C__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class AST_ConcatNumber
//!
class UT_AST_ConcatNumber : public CxxTest::TestSuite
{
  public:
   UT_AST_ConcatNumber() = default;
  ~UT_AST_ConcatNumber() = default;

  // ================ Test Methods ================

  // ---------------- Constructor
  //
  void test_Constructor ();

  // ---------------- AsText
  //
  void test_AsText ();

  // ---------------- AsBinaryVector
  //
  void test_AsBinaryVector ();
  void test_AsBinaryVector_not_valid ();
};
//
//  End of UT_AST_ConcatNumber class declaration
//---------------------------------------------------------------------------

#endif  // not defined UT_AST_CONCATNUMBER_H__7DE9EF47_6F37_4AFA_EBA_3AB98BC8A01C__INCLUDED_

//===========================================================================
// End of UT_AST_ConcatNumber.hpp
//===========================================================================
