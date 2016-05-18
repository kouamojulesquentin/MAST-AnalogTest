//===========================================================================
//                           UT_PrettyPrinterVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_PrettyPrinterVisitor.hpp
//!
//! Declares test fixture class for testing PrettyPrinterVisitor
//!
//===========================================================================


#ifndef UT_PRETTYPRINTERVISITOR_H__81EABFFB_2B16_465D_E396_1C6C22642AB8__INCLUDED_
  #define UT_PRETTYPRINTERVISITOR_H__81EABFFB_2B16_465D_E396_1C6C22642AB8__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class PrettyPrinterVisitor
//!
class UT_PrettyPrinterVisitor : public CxxTest::TestSuite
{
  public:
   UT_PrettyPrinterVisitor() = default;
  ~UT_PrettyPrinterVisitor() = default;


  // ================ Test Methods ================

  // ---------------- Constructor
  //
  void test_Constructor ();

  // ---------------- Visit - Single element
  //
  void test_VisitAccessInterface ();
  void test_VisitAccessInterface_Verbose ();
  void test_VisitChain ();
  void test_VisitChain_Verbose ();
  void test_VisitLinker ();
  void test_VisitLinker_Verbose ();
  void test_VisitRegister ();
  void test_VisitRegister_Verbose ();

  // ---------------- Visit - Two nodes
  //
  void test_VisitChain_with_Register ();

  // ---------------- Visit - Several nodes
  //
  void test_VisitChain_with_Registers ();
  void test_VisitChain_with_Registers_Verbose ();
  void test_VisitLinker_with_Child ();
  void test_VisitLinker_with_Child_Verbose ();
  void test_VisitAccessInterface_with_Child ();
  void test_VisitAccessInterface_Hex_Format ();
  void test_VisitTap ();
  void test_VisitTap_With_SubNodes ();

  // ---------------- Private  Methods
  //
  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_PrettyPrinterVisitor class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_PRETTYPRINTERVISITOR_H__81EABFFB_2B16_465D_E396_1C6C22642AB8__INCLUDED_
//===========================================================================
// End of UT_PrettyPrinterVisitor.hpp
//===========================================================================
