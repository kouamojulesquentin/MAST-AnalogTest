//===========================================================================
//                           UT_SIT_Printer.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SIT_Printer.hpp
//!
//! Declares test fixture class for testing SIT_Printer
//!
//===========================================================================

#ifndef UT_SIT_PRINTER_H__72E0F48A_E251_4280_8F8C_8B90405BB524__INCLUDED_
  #define UT_SIT_PRINTER_H__72E0F48A_E251_4280_8F8C_8B90405BB524__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class SIT_Printer
//!
class UT_SIT_Printer : public CxxTest::TestSuite
{
  public:
   UT_SIT_Printer() = default;
  ~UT_SIT_Printer() = default;


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

  // ---------------- Visit - Two nodes
  //
  void test_VisitChain_with_Register ();

  // ---------------- Visit - Several nodes
  //
  void test_VisitChain_with_Registers ();
  void test_VisitLinker_with_Child ();
  void test_VisitAccessInterface_with_Child ();
  void test_VisitTap ();
  void test_VisitTap_With_SubNodes ();

  // ---------------- MakeSIT
  //
  void test_MakeSIT ();

  void test_Warnings ();

  // ---------------- Private  Methods
  //
  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_SIT_Printer class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_SIT_PRINTER_H__72E0F48A_E251_4280_8F8C_8B90405BB524__INCLUDED_
//===========================================================================
// End of UT_SIT_Printer.hpp
//===========================================================================
