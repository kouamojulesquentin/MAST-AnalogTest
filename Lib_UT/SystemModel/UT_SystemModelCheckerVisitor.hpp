//===========================================================================
//                           UT_SystemModelCheckerVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SystemModelCheckerVisitor.hpp
//!
//! Declares test fixture class for testing SystemModelCheckerVisitor
//!
//===========================================================================


#ifndef UT_SYSTEMMODELCHECKERVISITOR_H__7B7782B0_5B71_4ED4_F38E_2389907371B__INCLUDED_
  #define UT_SYSTEMMODELCHECKERVISITOR_H__7B7782B0_5B71_4ED4_F38E_2389907371B__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class SystemModelCheckerVisitor
//!
//!
class UT_SystemModelCheckerVisitor : public CxxTest::TestSuite
{
  public:
   UT_SystemModelCheckerVisitor() = default;
  ~UT_SystemModelCheckerVisitor() = default;

  // ================ Test Methods ================

  void test_Constructor_From_EmptyModel ();
  void test_Constructor_From_ModelWithTap ();

  void test_CheckIdentifiers_With_ModelWithTap ();
  void test_CheckIdentifiers_With_UnusedIdentifier ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_SystemModelCheckerVisitor class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_SYSTEMMODELCHECKERVISITOR_H__7B7782B0_5B71_4ED4_F38E_2389907371B__INCLUDED_
//===========================================================================
// End of UT_SystemModelCheckerVisitor.hpp
//===========================================================================
