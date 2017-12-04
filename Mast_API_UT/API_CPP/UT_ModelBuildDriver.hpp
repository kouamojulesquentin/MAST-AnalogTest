//===========================================================================
//                           UT_ModelBuildDriver.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_ModelBuildDriver.hpp
//!
//! Declares test fixture class for testing ModelBuildDriver
//!
//===========================================================================


#ifndef UT_MODELBUILDDRIVER_H__46B9D203_D70B_4A01_D18B_BCD3A8F3EB38__INCLUDED_
  #define UT_MODELBUILDDRIVER_H__46B9D203_D70B_4A01_D18B_BCD3A8F3EB38__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class ModelBuildDriver
//!
class UT_ModelBuildDriver : public CxxTest::TestSuite
{
  public:
   UT_ModelBuildDriver() = default;
  ~UT_ModelBuildDriver() = default;

  // ================ Test Methods ================

  // ---------------- constructor
  //
  void test_Constructor ();

  // ---------------- CreateModelFromSitFile
  //
  void test_CreateModelFromSitFile_EmptyPath ();
  void test_CreateModelFromSitFile_NotFoundFile ();
  void test_CreateModelFromSitFile_NoInstance ();
  void test_CreateModelFromSitFile_OneInstance ();
  void test_CreateModelFromSitFile_OneInstance_no_Ext ();
  void test_CreateModelFromSitFile_TwoInstances ();
  void test_CreateModelFromSitFile_Hierarchical ();
  void test_CreateModelFromSitFile_Hierarchical_SubDir ();
  void test_CreateModelFromSitFile_ParsingError ();

  void test_CreateModelFromSitFile_OneFactoryInstance ();
  void test_CreateModelFromSitFile_MixKindsInstances ();
  void test_CreateModelFromSitFile_MissingFactory ();

  void test_CreateModelFromSitFile_Examples ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_ModelBuildDriver class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_MODELBUILDDRIVER_H__46B9D203_D70B_4A01_D18B_BCD3A8F3EB38__INCLUDED_
//===========================================================================
// End of UT_ModelBuildDriver.hpp
//===========================================================================
