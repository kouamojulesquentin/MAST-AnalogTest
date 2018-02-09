//===========================================================================
//                           UT_AliasRepository.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_AliasRepository.hpp
//!
//! Declares test fixture class for testing AliasRepository
//!
//===========================================================================

#ifndef UT_ALIASREPOSITORY_H__7B38D945_A671_47B8_E4AE_3F88B51911CB__INCLUDED_
  #define UT_ALIASREPOSITORY_H__7B38D945_A671_47B8_E4AE_3F88B51911CB__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class AliasRepository
//!
class UT_AliasRepository : public CxxTest::TestSuite
{
  public:
   UT_AliasRepository() = default;
  ~UT_AliasRepository() = default;

  // ================ Test Methods ================

  void test_Constructor ();

  void test_Append_first ();
  void test_Append_second ();

  void test_FindRegisterAlias ();
  void test_FindRegisterAlias_not_exist ();

  void test_FindRegister ();
  void test_FindRegister_not_exist ();
};
//
//  End of UT_AliasRepository class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_ALIASREPOSITORY_H__7B38D945_A671_47B8_E4AE_3F88B51911CB__INCLUDED_
//===========================================================================
// End of UT_AliasRepository.hpp
//===========================================================================
