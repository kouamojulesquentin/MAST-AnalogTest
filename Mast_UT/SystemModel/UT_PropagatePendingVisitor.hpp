//===========================================================================
//                           UT_PropagatePendingVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_PropagatePendingVisitor.hpp
//!
//! Declares test fixture class for testing UT_PropagatePendingVisitor
//!
//===========================================================================


#ifndef PROPAGATEPENDINGVISITOR_H__2D134860_8B86_45B2_5094_10ECCC5630BD__INCLUDED_
  #define PROPAGATEPENDINGVISITOR_H__2D134860_8B86_45B2_5094_10ECCC5630BD__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class UT_PropagatePendingVisitor
//!
class UT_PropagatePendingVisitor : public CxxTest::TestSuite
{
  public:
   UT_PropagatePendingVisitor() = default;
  ~UT_PropagatePendingVisitor() = default;

  // ================ Test Methods ================

  void test_Constructor ();
  void test_Visit_xxx ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_PropagatePendingVisitor class declaration
//---------------------------------------------------------------------------


#endif  // not defined PROPAGATEPENDINGVISITOR_H__2D134860_8B86_45B2_5094_10ECCC5630BD__INCLUDED_
//===========================================================================
// End of UT_PropagatePendingVisitor.hpp
//===========================================================================
