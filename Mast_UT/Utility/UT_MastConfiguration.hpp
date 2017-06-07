//===========================================================================
//                           UT_MastConfiguration.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_MastConfiguration.hpp
//!
//! Declares test fixture class for testing MastConfiguration
//!
//===========================================================================


#ifndef UT_MASTCONFIGURATION_H__898FF0A_4F3B_4868_C9C_DC7FCBE17B47__INCLUDED_
  #define UT_MASTCONFIGURATION_H__898FF0A_4F3B_4868_C9C_DC7FCBE17B47__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class MastConfiguration
//!
//!
class UT_MastConfiguration : public CxxTest::TestSuite
{
  public:
   UT_MastConfiguration() = default;
  ~UT_MastConfiguration() = default;

  // ================ Test Methods ================

  void test_constructor ();

  void test_Update_NoArguments ();
  void test_Update_OnlyAppName ();
  void test_Update_BadArguments ();
  void test_Update_ShortSwitches ();
  void test_Update_LongSwitches ();
};
//
//  End of UT_MastConfiguration class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_MASTCONFIGURATION_H__898FF0A_4F3B_4868_C9C_DC7FCBE17B47__INCLUDED_
//===========================================================================
// End of UT_MastConfiguration.hpp
//===========================================================================
