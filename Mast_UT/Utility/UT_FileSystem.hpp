//===========================================================================
//                           UT_FileSystem.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_FileSystem.hpp
//!
//! Declares test fixture class for testing FileSystem
//!
//===========================================================================


#ifndef UT_FILESYSTEM_H__4FCD76C0_A43B_4B1A_2D98_4CE0A1CEB62__INCLUDED_
  #define UT_FILESYSTEM_H__4FCD76C0_A43B_4B1A_2D98_4CE0A1CEB62__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class FileSystem
//!
class UT_FileSystem : public CxxTest::TestSuite
{
  public:
   UT_FileSystem() = default;
  ~UT_FileSystem() = default;

  // ================ Test Methods ================

  // ---------------- IsDirectory
  //
  void test_IsDirectory_when_ItIsNot ();
  void test_IsDirectory_when_ItIs ();
};
//
//  End of UT_FileSystem class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_FILESYSTEM_H__4FCD76C0_A43B_4B1A_2D98_4CE0A1CEB62__INCLUDED_
//===========================================================================
// End of UT_FileSystem.hpp
//===========================================================================
