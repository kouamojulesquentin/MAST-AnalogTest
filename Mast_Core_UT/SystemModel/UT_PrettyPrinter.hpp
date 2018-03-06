//===========================================================================
//                           UT_PrettyPrinter.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_PrettyPrinter.hpp
//!
//! Declares test fixture class for testing PrettyPrinter
//!
//===========================================================================

#ifndef UT_PRETTYPRINTER_H__1B3FB709_2AA6_4CA6_8281_45577186605__INCLUDED_
  #define UT_PRETTYPRINTER_H__1B3FB709_2AA6_4CA6_8281_45577186605__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class PrettyPrinter
//!
class UT_PrettyPrinter : public CxxTest::TestSuite
{
  public:
   UT_PrettyPrinter() = default;
  ~UT_PrettyPrinter() = default;


  // ================ Test Methods ================

  // ---------------- Constructor
  //
  void test_Constructor ();

  // ---------------- Visit - Single element
  //
  void test_VisitAccessInterface ();
  void test_VisitAccessInterface_Verbose ();
  void test_VisitAccessInterface_ShowProtocol ();
  void test_VisitAccessInterfaceTranslator ();
  void test_VisitAccessInterfaceTranslator_ShowProtocol();
  void test_VisitChain ();
  void test_VisitChain_Verbose ();
  void test_VisitLinker_SingleRegSelector ();
  void test_VisitLinker_SingleRegSelector_Verbose ();
  void test_VisitLinker_MultipleRegSelector ();
  void test_VisitLinker_MultiplePartialRegSelector ();
  void test_VisitRegister ();
  void test_VisitRegister_Verbose ();

  // ---------------- Visit - Two nodes
  //
  void test_VisitChain_with_Register ();

  // ---------------- Visit - Several nodes
  //
  void test_VisitChain_with_Registers ();
  void test_VisitChain_with_Registers_Verbose ();
  void test_VisitChain_with_Aliases ();
  void test_VisitLinker_with_SingleRegSelector ();
  void test_VisitLinker_with_SingleRegSelector_Verbose ();
  void test_VisitAccessInterface_with_Child ();
  void test_VisitAccessInterface_AutoFormat ();
  void test_VisitTap ();
  void test_VisitTap_With_SubNodes ();

  // ---------------- PrettyPrint
  //
  void test_PrettyPrint ();
  void test_PrettyPrint_Verbose ();
  void test_PrettyPrint_AutoFormat ();
  void test_PrettyPrint_Std ();
  void test_PrettyPrint_SelectionValue ();
  void test_PrettyPrint_SelectionState ();
  void test_PrettyPrint_SelectionTables ();
  void test_PrettyPrint_AllOptions ();

  // ---------------- Private  Methods
  //
  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_PrettyPrinter class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_PRETTYPRINTER_H__1B3FB709_2AA6_4CA6_8281_45577186605__INCLUDED_
//===========================================================================
// End of UT_PrettyPrinter.hpp
//===========================================================================
