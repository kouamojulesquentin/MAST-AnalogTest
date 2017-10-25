//===========================================================================
//                           UT_AST_PrettyPrinter.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_AST_PrettyPrinter.hpp
//!
//! Declares test fixture class for testing AST_PrettyPrinter
//!
//===========================================================================

#ifndef UT_AST_PRETTYPRINTER_H__41A07665_E05A_4CE3_96B5_24A5B0AE43E4__INCLUDED_
  #define UT_AST_PRETTYPRINTER_H__41A07665_E05A_4CE3_96B5_24A5B0AE43E4__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class AST_PrettyPrinter
//!
//!
class UT_AST_PrettyPrinter : public CxxTest::TestSuite
{
  public:
  ~UT_AST_PrettyPrinter() = default;
   UT_AST_PrettyPrinter() = default;

  // ================ Test Methods ================

  // ---------------- Visit_Module
  //
  void test_Visit_Module_empty ();

  // ---------------- Visit_ScanRegister
  //
  void test_Visit_ScanRegister_empty ();


  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_AST_PrettyPrinter class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_AST_PRETTYPRINTER_H__41A07665_E05A_4CE3_96B5_24A5B0AE43E4__INCLUDED_
//===========================================================================
// End of UT_AST_PrettyPrinter.hpp
//===========================================================================
