//===========================================================================
//                           UT_ConfigureAlgorithmFactory.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_ConfigureAlgorithmFactory.hpp
//!
//! Declares test fixture class for testing ConfigureAlgorithmFactory
//!
//===========================================================================


#ifndef UT_CONFIGUREALGORITHMFACTORY_H__C804DCB3_954D_47F0_D9A8_DFAFFBC65336__INCLUDED_
  #define UT_CONFIGUREALGORITHMFACTORY_H__C804DCB3_954D_47F0_D9A8_DFAFFBC65336__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class ConfigureAlgorithmFactory
//!
class UT_ConfigureAlgorithmFactory : public CxxTest::TestSuite
{
  public:
           UT_ConfigureAlgorithmFactory() = default;
  virtual ~UT_ConfigureAlgorithmFactory() = default;

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
//  End of UT_ConfigureAlgorithmFactory class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_CONFIGUREALGORITHMFACTORY_H__C804DCB3_954D_47F0_D9A8_DFAFFBC65336__INCLUDED_
//===========================================================================
// End of UT_ConfigureAlgorithmFactory.hpp
//===========================================================================
