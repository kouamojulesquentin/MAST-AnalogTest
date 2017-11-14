//===========================================================================
//                           UT_AST_Checker.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_AST_Checker.hpp
//!
//! Declares test fixture class for testing AST_Checker
//!
//===========================================================================


#ifndef UT_AST_CHECKER_H__35E59098_194B_406C_91AC_502F5B37DC1__INCLUDED_
  #define UT_AST_CHECKER_H__35E59098_194B_406C_91AC_502F5B37DC1__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class AST_Checker
//!
//!
class UT_AST_Checker : public CxxTest::TestSuite
{
  public:
   UT_AST_Checker() = default;
  ~UT_AST_Checker() = default;

  // ================ Test Methods ================

  // ---------------- Constructor
  //
  void test_Constructor ();
  void test_Constructor_Nullptr_Network ();

  // ---------------- Check
  //
  void test_Check_NoModule ();
  void test_Check_Module_Ok ();
  void test_Check_Module_NoScanOutPorts ();
};
//
//  End of UT_AST_Checker class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_AST_CHECKER_H__35E59098_194B_406C_91AC_502F5B37DC1__INCLUDED_
//===========================================================================
// End of UT_AST_Checker.hpp
//===========================================================================
