//===========================================================================
//                           UT_SVF_SimulationProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SVF_SimulationProtocol.hpp
//!
//! Declares test fixture class for testing SVF_SimulationProtocol
//!
//===========================================================================


#ifndef UT_STIL_EMULATIONPROTOCOL_H__1252752F_98CD_466E_ACBF_7835604010A5__INCLUDED_
  #define UT_STIL_EMULATIONPROTOCOL_H__1252752F_98CD_466E_ACBF_7835604010A5__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class SVF_SimulationProtocol
//!
//!
class UT_STIL_EmulationProtocol : public CxxTest::TestSuite
{
  public:
   UT_STIL_EmulationProtocol() = default;
  ~UT_STIL_EmulationProtocol() = default;

  // ================ Test Methods ================

  void test_Constructor_Integer ();
  void test_MaxSupportedChannels ();
  void test_Constructor_String ();
  void test_Constructor_String_Error ();
  void test_SPY_Constructor ();
  void test_DoCallback ();

  // ================ Other Methods ================

};
//
//  End of UT_STIL_EmulationProtocol class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_STIL_EMULATIONPROTOCOL_H__1252752F_98CD_466E_ACBF_7835604010A5__INCLUDED_
//===========================================================================
// End of UT_STIL_EmulationProtocol.hpp
//===========================================================================
