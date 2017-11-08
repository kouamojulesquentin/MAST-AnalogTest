//===========================================================================
//                           UT_ICL_Reader.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_ICL_Reader.hpp
//!
//! Declares test fixture class for testing ICL_Reader
//!
//===========================================================================


#ifndef UT_ICL_READER_H__EAD5D93F_6DD4_428F_43A3_345F9803BDEF__INCLUDED_
  #define UT_ICL_READER_H__EAD5D93F_6DD4_428F_43A3_345F9803BDEF__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class ICL_Reader
//!
class UT_ICL_Reader : public CxxTest::TestSuite
{
  public:
  UT_ICL_Reader() = default;
  virtual ~UT_ICL_Reader() = default;

  // ================ Test Methods ================

  // ---------------- UpdateAstFromIcl
  //
  void test_UpdateAstFromIcl_1_ScanRegister ();
  void test_UpdateAstFromIcl_3_ScanRegister ();
  void test_UpdateAstFromIcl_parameters_value ();
  void test_UpdateAstFromIcl_parameters_strings ();
  void test_UpdateAstFromIcl_parameters_param_ref ();
  void test_UpdateAstFromIcl_parameters_string_and_param_ref ();
  void test_UpdateAstFromIcl_parameters_param_ref_and_string ();
  void test_UpdateAstFromIcl_attributes_no_value ();
  void test_UpdateAstFromIcl_attributes_value ();
  void test_UpdateAstFromIcl_attributes_strings ();
  void test_UpdateAstFromIcl_attributes_param_ref ();
  void test_UpdateAstFromIcl_ScanMux_SIB ();
  void test_UpdateAstFromIcl_ScanMux_ValueList ();
  void test_UpdateAstFromIcl_Instance ();
  void test_UpdateAstFromIcl_Namespace_Def ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_ICL_Reader class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_ICL_READER_H__EAD5D93F_6DD4_428F_43A3_345F9803BDEF__INCLUDED_
//===========================================================================
// End of UT_ICL_Reader.hpp
//===========================================================================
