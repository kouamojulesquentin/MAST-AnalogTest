//===========================================================================
//                           UT_ToSutVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_ToSutVisitor.hpp
//!
//! Declares test fixture class for testing ToSutVisitor
//!
//===========================================================================


#ifndef UT_TOSUTVISITOR_H__28703448_D48A_4FEE_95A5_CF40543FECD1__INCLUDED_
  #define UT_TOSUTVISITOR_H__28703448_D48A_4FEE_95A5_CF40543FECD1__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class ToSutVisitor
//!
//!
class UT_ToSutVisitor : public CxxTest::TestSuite
{
  public:
   UT_ToSutVisitor() = default;
  ~UT_ToSutVisitor() = default;

  // ================ Test Methods ================

  void test_Constructor ();

  void test_Accept_Mib_Binary_CanSelectNone ();
  void test_Accept_Mib_Binary_CannotSelectNone ();
  void test_Accept_Testcase_1500_Nothing_Pending ();
  void test_Accept_Testcase_1500_BypassMode_IgnorePending ();
  void test_Accept_Testcase_AccessInterface_1_Pending_Step_1 ();
  void test_Accept_Testcase_AccessInterface_1_Pending_Step_2 ();
//+  void test_Accept_Testcase_1500_2_Pending ();

  void test_Reset ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_ToSutVisitor class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_TOSUTVISITOR_H__28703448_D48A_4FEE_95A5_CF40543FECD1__INCLUDED_
//===========================================================================
// End of UT_ToSutVisitor.hpp
//===========================================================================
