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

  // ---------------- string_view
  //
  void test_StringView_Utility_StartsWith ();
  void test_StringView_Utility_EndsWith ();
  void test_StringView_Utility_Split ();
  void test_StringView_Utility_TrimLeft ();
  void test_StringView_Utility_TrimRight ();
  void test_StringView_Utility_TrimBoth ();

  // ---------------- BackToken
  //
  void test_BackToken ();

  // ---------------- FrontToken
  //
  void test_FrontTokens ();

  // ---------------- TokensCount
  //
  void test_TokensCount ();

  // ---------------- StartsWith
  //
  void test_StartsWith  ();

  // ---------------- EndsWith
  //
  void test_EndsWith ();

  // ---------------- Contains
  //
  void test_Contains ();

  // ---------------- IsElementOf
  //
  void test_IsElementOf ();


  // ---------------- ToUInt32
  //
  void test_ToUInt32_Success ();
  void test_ToUInt32_Failure_OutOfRange ();
  void test_ToUInt32_Failure_Invalid ();

  // ---------------- SingleQuote
  //
  void test_SingleQuote ();

  // ---------------- IfNotEmpty_SingleQuoteAndSuffixWithSpace
  //
  void test_IfNotEmpty_SingleQuoteAndSuffixWithSpace ();

  // ---------------- ExtractDirectoryPath
  //
  void test_ExtractDirectoryPath ();

  // ---------------- chrono::duration
  //
  void test_ToMilliseconds_BadStr ();
  void test_ToMilliseconds_SupportedStr ();

  // ---------------- Assertion Macros
  //
  void test_CHECK_VALUE_EQ_Success ();
  void test_CHECK_VALUE_EQ_Failure ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_Utility class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_UTILITY_H__B9B9EA0E_60C_4800_C782_585F8D7ACE9E__INCLUDED_
//===========================================================================
// End of UT_Utility.hpp
//===========================================================================
