//===========================================================================
//                           UT_EnumsUtility.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_EnumsUtility.hpp
//!
//! Declares test fixture class for testing EnumsUtility helpers
//!
//===========================================================================
#ifndef UT_ENUMSUTILITY_H__9852236C_AB00_49C6_5E9D_9DB3D323DD6A__INCLUDED_
  #define UT_ENUMSUTILITY_H__9852236C_AB00_49C6_5E9D_9DB3D323DD6A__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for helpers in EnumsUtility.hpp
//!
class UT_EnumsUtility : public CxxTest::TestSuite
{
  public:
   UT_EnumsUtility() = default;
  ~UT_EnumsUtility() = default;

  // ================ Test Methods ================

  void test_ExtractedScope ();
  void test_RemovedScope ();

  void test_Enum_NameString ();
  void test_Enum_ScopeString ();
  void test_Enum_ScopedNameString ();

};
//
//  End of UT_EnumsUtility class declaration
//---------------------------------------------------------------------------

#endif  // not defined UT_ENUMSUTILITY_H__9852236C_AB00_49C6_5E9D_9DB3D323DD6A__INCLUDED_
//===========================================================================
// End of UT_EnumsUtility.hpp
//===========================================================================
