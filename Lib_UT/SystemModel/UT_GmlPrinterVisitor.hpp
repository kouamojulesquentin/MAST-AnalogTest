//===========================================================================
//                           UT_GmlPrinterVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_GmlPrinterVisitor.hpp
//!
//! Declares test fixture class for testing GmlPrinterVisitor
//!
//===========================================================================


#ifndef UT_GMLPRINTERVISITOR_H__EE13EE8E_BF72_4155_E7B9_E5465FA829B9__INCLUDED_
  #define UT_GMLPRINTERVISITOR_H__EE13EE8E_BF72_4155_E7B9_E5465FA829B9__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class GmlPrinterVisitor
//!
//!
class UT_GmlPrinterVisitor : public CxxTest::TestSuite
{
  public:
   UT_GmlPrinterVisitor() = default;
  ~UT_GmlPrinterVisitor() = default;

  // ---------------- Constructor
  //
  void test_Constructor ();

  // ---------------- Visit
  //
  void test_VisitAccessInterface_with_Child ();
  void test_Visit_After_GetGraph ();
  void test_VisitTap ();
  void test_VisitTap_With_SubNodes ();

  // ---------------- Options
  //
  void test_Visit_With_DisplayIdentifier_true ();
  void test_Visit_With_DisplayIdentifierAndRegister_true ();

  // ---------------- Private  Methods
  //
  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_GmlPrinterVisitor class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_GMLPRINTERVISITOR_H__EE13EE8E_BF72_4155_E7B9_E5465FA829B9__INCLUDED_
//===========================================================================
// End of UT_GmlPrinterVisitor.hpp
//===========================================================================
