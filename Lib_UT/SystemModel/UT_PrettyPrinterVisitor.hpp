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
  void test_VisitChain ();
  void test_VisitLinker ();
  void test_VisitRegister ();
  void test_VisitTap ();

  // ---------------- Visit - Two nodes
  //
  void test_VisitChain_with_Register ();

  // ---------------- Visit - Several nodes
  //
  void test_VisitChain_with_Registers ();

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
