//===========================================================================
//                           UT_AccessInterfaceProtocolFactories.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_AccessInterfaceProtocolFactories.hpp
//!
//! Declares test fixture class for testing AccessInterfaceProtocolFactories
//!
//===========================================================================


#ifndef UT_ACCESSINTERFACEPROTOCOLFACTORIES_H__AB34E0F4_45E3_4903_56B3_B11678D1A08A__INCLUDED_
  #define UT_ACCESSINTERFACEPROTOCOLFACTORIES_H__AB34E0F4_45E3_4903_56B3_B11678D1A08A__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class AccessInterfaceProtocolFactories
//!
//!
class UT_AccessInterfaceProtocolFactories : public CxxTest::TestSuite
{
  public:
  ~UT_AccessInterfaceProtocolFactories() = default;
   UT_AccessInterfaceProtocolFactories() = default;

  // ================ Test Methods ================

  void test_Instance ();
  void test_Create_Success ();
  void test_Create_Error ();

  // ---------------- Protocols Management
  //
  void test_Clear ();
  void test_InitializeWithDefaults ();

  void test_RegisterCreator_NewOne_when_Defaults ();
  void test_RegisterCreator_NewOne_when_None ();
  void test_RegisterCreator_Replace_Default ();
};
//
//  End of UT_AccessInterfaceProtocolFactories class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_ACCESSINTERFACEPROTOCOLFACTORIES_H__AB34E0F4_45E3_4903_56B3_B11678D1A08A__INCLUDED_
//===========================================================================
// End of UT_AccessInterfaceProtocolFactories.hpp
//===========================================================================
