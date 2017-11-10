//===========================================================================
//                           UT_NamesChecker.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_NamesChecker.hpp
//!
//! Declares test fixture class for testing NamesChecker
//!
//===========================================================================


#ifndef UT_NAMESCHECKER_H__3C02F05F_EEB5_4493_9194_2DE6CD51E4B9__INCLUDED_
  #define UT_NAMESCHECKER_H__3C02F05F_EEB5_4493_9194_2DE6CD51E4B9__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class NamesChecker
//!
class UT_NamesChecker : public CxxTest::TestSuite
{
  public:
   UT_NamesChecker() = default;
  ~UT_NamesChecker() = default;

  // ================ Test Methods ================

  // ---------------- Names at single hierarchy level
  //
  void test_Check_SameNames_Only_Regs ();
  void test_Check_SameNames_Only_Parents ();
  void test_Check_SameNames_Mix_Regs_and_Parents ();

  void test_Check_SameNames_Only_Ignored ();
  void test_Check_SameNames_Some_Ignored_1 ();
  void test_Check_SameNames_Some_Ignored_2 ();
  void test_Check_SameNames_Mix_Regs_and_Ignored_1 ();
  void test_Check_SameNames_Mix_Regs_and_Ignored_2 ();

  void test_Check_NoName_Reg ();
  void test_Check_NoName_Parent ();
  void test_Check_NoName_Ignored ();

  // ---------------- Names paths (traversing hierarchy levels)
  //
  void test_Check_SamePath_Downward_2_Regs ();
  void test_Check_SamePath_Downward_3_Regs ();
  void test_Check_SamePath_Sideward_2_Regs ();
  void test_Check_SamePath_Sideward_3_Regs ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_NamesChecker class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_NAMESCHECKER_H__3C02F05F_EEB5_4493_9194_2DE6CD51E4B9__INCLUDED_
//===========================================================================
// End of UT_NamesChecker.hpp
//===========================================================================
