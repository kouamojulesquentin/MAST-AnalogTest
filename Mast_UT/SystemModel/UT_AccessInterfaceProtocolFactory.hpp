//===========================================================================
//                           UT_AccessInterfaceProtocolFactory.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_AccessInterfaceProtocolFactory.hpp
//!
//! Declares test fixture class for testing AccessInterfaceProtocolFactory
//!
//===========================================================================


#ifndef UT_ACCESSINTERFACEPROTOCOLFACTORY_H__906155E3_A05E_4CEA_6A7_755751180F__INCLUDED_
  #define UT_ACCESSINTERFACEPROTOCOLFACTORY_H__906155E3_A05E_4CEA_6A7_755751180F__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class AccessInterfaceProtocolFactory
//!
//!
class UT_AccessInterfaceProtocolFactory : public CxxTest::TestSuite
{
  public:
  ~UT_AccessInterfaceProtocolFactory() = default;
   UT_AccessInterfaceProtocolFactory() = default;

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
//  End of UT_AccessInterfaceProtocolFactory class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_ACCESSINTERFACEPROTOCOLFACTORY_H__906155E3_A05E_4CEA_6A7_755751180F__INCLUDED_
//===========================================================================
// End of UT_AccessInterfaceProtocolFactory.hpp
//===========================================================================
