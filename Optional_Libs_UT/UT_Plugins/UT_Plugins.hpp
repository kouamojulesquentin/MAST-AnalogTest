//===========================================================================
//                           UT_Plugins.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_Plugins.hpp
//!
//! Declares test fixture class for testing Plugins
//!
//===========================================================================


#ifndef UT_PLUGINS_H__C8FB0322_3EF7_4108_839F_17B78958FB7C__INCLUDED_
  #define UT_PLUGINS_H__C8FB0322_3EF7_4108_839F_17B78958FB7C__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class Plugins
//!
class UT_Plugins : public CxxTest::TestSuite
{
  public:
  ~UT_Plugins() = default;
   UT_Plugins() = default;

  // ================ Test Methods ================

  void test_GetPlugins ();
  void test_LoadPlugin ();
  void test_LoadPlugins (); // Must be after test_LoadPlugin because loaded DLL are not unloaded (March/28/2017)
};
//
//  End of UT_Plugins class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_PLUGINS_H__C8FB0322_3EF7_4108_839F_17B78958FB7C__INCLUDED_
//===========================================================================
// End of UT_Plugins.hpp
//===========================================================================
