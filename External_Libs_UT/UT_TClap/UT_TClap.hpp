//===========================================================================
//                           UT_TClap.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_TClap.hpp
//!
//! Declares test fixture class for testing tclap
//!
//===========================================================================


#ifndef UT_TCLAP_H__93703B79_F5EC_468C_14BD_45B4B2E482B__INCLUDED_
  #define UT_TCLAP_H__93703B79_F5EC_468C_14BD_45B4B2E482B__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for tclap library
//!
//! @note Does only partial tests because this is an external library that is supposed to be already tested
//!
class UT_TClap : public CxxTest::TestSuite
{
  public:
   UT_TClap() = default;
  ~UT_TClap() = default;

  // ================ Test Methods ================

  // ---------------- CmdLine
  //
  void test_CmdLine_Constructor_Minimal ();
  void test_CmdLine_Constructor_all_Params ();

  void test_CmdLine_setOutput ();

  void test_CmdLine_parse_no_Parameters ();
  void test_CmdLine_parse_only_ExeName ();
  void test_CmdLine_parse_Help ();
  void test_CmdLine_parse_Version ();
  void test_CmdLine_parse_UnregisteredSwitch ();
  void test_CmdLine_parse_UnregisteredParam ();

  void test_CmdLine_parse_all_Parameters ();
  void test_CmdLine_parse_missing_Required ();
};
//
//  End of UT_TClap class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_TCLAP_H__93703B79_F5EC_468C_14BD_45B4B2E482B__INCLUDED_
//===========================================================================
// End of UT_TClap.hpp
//===========================================================================
