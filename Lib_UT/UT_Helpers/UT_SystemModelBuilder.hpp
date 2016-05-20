//===========================================================================
//                           UT_SystemModelBuilder.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SystemModelBuilder.hpp
//!
//! Declares test fixture class for testing SystemModelBuilder
//!
//===========================================================================


#ifndef UT_SYSTEMMODELBUILDER_H__CEE2196B_B1C8_49A1_CC86_18C0A5DE84__INCLUDED_
  #define UT_SYSTEMMODELBUILDER_H__CEE2196B_B1C8_49A1_CC86_18C0A5DE84__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class SystemModelBuilder
//!
class UT_SystemModelBuilder : public CxxTest::TestSuite
{
  public:
   UT_SystemModelBuilder() = default;
  ~UT_SystemModelBuilder() = default;

  // ================ Test Methods ================

  void test_Create_TestCase_AccessInterface ();
  void test_Create_TestCase_1500_3_Chains ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_SystemModelBuilder class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_SYSTEMMODELBUILDER_H__CEE2196B_B1C8_49A1_CC86_18C0A5DE84__INCLUDED_
//===========================================================================
// End of UT_SystemModelBuilder.hpp
//===========================================================================
