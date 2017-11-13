//===========================================================================
//                           UT_I2C_SimulationProtocol.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_I2C_SimulationProtocol.hpp
//!
//! Declares test fixture class for testing I2C_SimulationProtocol
//!
//===========================================================================


#ifndef UT_I2C_EMULATIONPROTOCOL_H__1252752F_98CD_466E_ACBF_7835604010A5__INCLUDED_
  #define UT_I2C_EMULATIONPROTOCOL_H__1252752F_98CD_466E_ACBF_7835604010A5__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class I2C_SimulationProtocol
//!
//!
class UT_I2C_EmulationProtocol : public CxxTest::TestSuite
{
  public:
   UT_I2C_EmulationProtocol() = default;
  ~UT_I2C_EmulationProtocol() = default;

  // ================ Test Methods ================

  void test_Constructor_Integer ();
  void test_Constructor_Integer_Error ();
  void test_MaxSupportedEndPoints ();
  void test_Constructor_String ();
  void test_Constructor_String_Error ();
  void test_DoCallback ();
  void test_DoCallback_String ();
};
//
//  End of UT_I2C_EmulationProtocol class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_I2C_EMULATIONPROTOCOL_H__1252752F_98CD_466E_ACBF_7835604010A5__INCLUDED_
//===========================================================================
// End of UT_I2C_EmulationProtocol.hpp
//===========================================================================
