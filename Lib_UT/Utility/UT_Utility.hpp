//===========================================================================
//                           UT_Utility.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_Utility.hpp
//!
//! Declares test fixture class for testing Utility
//!
//===========================================================================


#ifndef UT_UTILITY_H__B9B9EA0E_60C_4800_C782_585F8D7ACE9E__INCLUDED_
  #define UT_UTILITY_H__B9B9EA0E_60C_4800_C782_585F8D7ACE9E__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class Utility
//!
//!
class UT_Utility : public CxxTest::TestSuite
{
  public:
   UT_Utility() = default;
  ~UT_Utility() = default;

  // ================ Test Methods ================

  void test_MinimalBitsForValue ();

  void test_StringView_Utility_Split ();
  void test_StringView_Utility_TrimLeft ();
  void test_StringView_Utility_TrimRight ();
  void test_StringView_Utility_TrimBoth ();


};
//
//  End of UT_Utility class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_UTILITY_H__B9B9EA0E_60C_4800_C782_585F8D7ACE9E__INCLUDED_
//===========================================================================
// End of UT_Utility.hpp
//===========================================================================
