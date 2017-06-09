//===========================================================================
//                           UT_MastEnvironment.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_MastEnvironment.hpp
//!
//! Declares test fixture class for testing MastEnvironment
//!
//===========================================================================


#ifndef UT_MASTENVIRONMENT_H__FEAFF4E5_5F01_4168_589D_FF4A37021EA2__INCLUDED_
  #define UT_MASTENVIRONMENT_H__FEAFF4E5_5F01_4168_589D_FF4A37021EA2__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class MastEnvironment
//!
//!
class UT_MastEnvironment : public CxxTest::TestSuite
{
  public:
   UT_MastEnvironment() = default;
  ~UT_MastEnvironment() = default;

  // ================ Test Methods ================

  // ---------------- Constructor
  //
  void test_Constructor_UnitTest ();

  // ---------------- ParseOptions
  //
  void test_ParseOptions_NoOptions ();

  // ---------------- LoadPlugins
  //
  void test_LoadPlugins_BeforeParsingOptions ();

  // ---------------- CreateSystemModel
  //

  // ---------------- Start
  //

  // ================ Other Methods ================

};
//
//  End of UT_MastEnvironment class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_MASTENVIRONMENT_H__FEAFF4E5_5F01_4168_589D_FF4A37021EA2__INCLUDED_
//===========================================================================
// End of UT_MastEnvironment.hpp
//===========================================================================
