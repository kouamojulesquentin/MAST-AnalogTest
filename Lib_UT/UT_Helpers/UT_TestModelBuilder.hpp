//===========================================================================
//                           UT_TestModelBuilder.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_TestModelBuilder.hpp
//!
//! Declares test fixture class for testing TestModelBuilder
//!
//===========================================================================


#ifndef UT_TESTMODELBUILDER_H__59AFCE9C_983E_4989_D392_181352EF685D__INCLUDED_
  #define UT_TESTMODELBUILDER_H__59AFCE9C_983E_4989_D392_181352EF685D__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class TestModelBuilder
//!
class UT_TestModelBuilder : public CxxTest::TestSuite
{
  public:
   UT_TestModelBuilder() = default;
  ~UT_TestModelBuilder() = default;

  // ================ Test Methods ================

  void test_Create_UnitTestCase_6_Levels ();
  void test_Create_TestCase_AccessInterface ();
  void test_Create_TestCase_MIB_4_Chains ();
  void test_Create_TestCase_1687 ();
  void test_Create_TestCase_1500_3_Chains ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_TestModelBuilder class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_TESTMODELBUILDER_H__59AFCE9C_983E_4989_D392_181352EF685D__INCLUDED_
//===========================================================================
// End of UT_TestModelBuilder.hpp
//===========================================================================
