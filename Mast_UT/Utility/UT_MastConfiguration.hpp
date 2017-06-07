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

  // ---------------- Constructor
  //
  void test_constructor ();

  // ---------------- Update
  //
  void test_Update_NoArguments ();
  void test_Update_OnlyAppName ();
  void test_Update_BadArguments ();
  void test_Update_ShortSwitches ();
  void test_Update_LongSwitches ();

  // ---------------- ParseYamlConfiguration
  //
  void test_ParseYamlConfiguration_Empty ();
  void test_ParseYamlConfiguration_SyntaxErrors ();
  void test_ParseYamlConfiguration_PartialConfiguration ();
  void test_ParseYamlConfiguration_FullConfiguration ();
};
//
//  End of UT_MastConfiguration class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_MASTCONFIGURATION_H__898FF0A_4F3B_4868_C9C_DC7FCBE17B47__INCLUDED_
//===========================================================================
// End of UT_MastConfiguration.hpp
//===========================================================================
