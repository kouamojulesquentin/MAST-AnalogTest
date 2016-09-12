//===========================================================================
//                           UT_GmlPrinter.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_GmlPrinter.hpp
//!
//! Declares test fixture class for testing GmlPrinter
//!
//===========================================================================


#ifndef UT_GMLPRINTER_H__F9300B2B_ED20_45DF_8683_469A2DB684E7__INCLUDED_
  #define UT_GMLPRINTER_H__F9300B2B_ED20_45DF_8683_469A2DB684E7__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class GmlPrinter
//!
//!
class UT_GmlPrinter : public CxxTest::TestSuite
{
  public:
   UT_GmlPrinter() = default;
  ~UT_GmlPrinter() = default;

  // ---------------- Constructor
  //
  void test_Constructor ();

  // ---------------- Visit
  //
  void test_VisitAccessInterface_with_Child ();
  void test_Visit_After_Graph ();
  void test_VisitTap ();
  void test_VisitTap_With_SubNodes ();
  void test_VisitTap_ShowProtocol ();

  // ---------------- Options
  //
  void test_Visit_NoEdgeForSelector ();
  void test_Visit_SelectionValues ();
  void test_Visit_With_DisplayIdentifier_true ();
  void test_Visit_With_DisplayIdentifierAndRegister_true ();

  // ---------------- Private  Methods
  //
  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_GmlPrinter class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_GMLPRINTER_H__F9300B2B_ED20_45DF_8683_469A2DB684E7__INCLUDED_
//===========================================================================
// End of UT_GmlPrinter.hpp
//===========================================================================
