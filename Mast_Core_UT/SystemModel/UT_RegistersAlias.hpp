//===========================================================================
//                           UT_RegistersAlias.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_RegistersAlias.hpp
//!
//! Declares test fixture class for testing RegistersAlias
//!
//===========================================================================

#ifndef UT_REGISTERSALIAS_H__E70E11D9_2ED7_4412_41A0_A66C8EC5E765__INCLUDED_
  #define UT_REGISTERSALIAS_H__E70E11D9_2ED7_4412_41A0_A66C8EC5E765__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class RegistersAlias
//!
class UT_RegistersAlias : public CxxTest::TestSuite
{
  public:
   UT_RegistersAlias() = default;
  ~UT_RegistersAlias() = default;

  // ================ Test Methods ================

  void test_Constructor ();
  void test_Constructor_empty_name ();
  void test_Constructor_empty_VirtualRegister ();

};
//
//  End of UT_RegistersAlias class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_REGISTERSALIAS_H__E70E11D9_2ED7_4412_41A0_A66C8EC5E765__INCLUDED_
//===========================================================================
// End of UT_RegistersAlias.hpp
//===========================================================================
