//===========================================================================
//                           UT_ConfigureVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_ConfigureVisitor.hpp
//!
//! Declares test fixture class for testing ConfigureVisitor
//!
//===========================================================================


#ifndef UT_CONFIGUREVISITOR_H__51ACB5EC_D94E_49EC_E599_44FC3CE1DC60__INCLUDED_
  #define UT_CONFIGUREVISITOR_H__51ACB5EC_D94E_49EC_E599_44FC3CE1DC60__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class ConfigureVisitor
//!
//!
class UT_ConfigureVisitor : public CxxTest::TestSuite
{
  public:
   UT_ConfigureVisitor() = default;
  ~UT_ConfigureVisitor() = default;

  // ================ Test Methods ================

  // ---------------- Constructor
  //
  void test_Constructor ();

  // ---------------- Accept
  //
  void test_Accept_Testcase_1500_Nothing_Pending ();
  void test_Accept_Testcase_1500_1_Pending ();
  void test_Accept_Testcase_1500_2_Pending ();
  void test_Accept_Testcase_1500_1_PendingRead ();
  void test_Accept_Testcase_AI_Pending_Reset ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_ConfigureVisitor class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_CONFIGUREVISITOR_H__51ACB5EC_D94E_49EC_E599_44FC3CE1DC60__INCLUDED_
//===========================================================================
// End of UT_ConfigureVisitor.hpp
//===========================================================================
