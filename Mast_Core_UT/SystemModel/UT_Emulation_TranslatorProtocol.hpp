//===========================================================================
//                           UT_Emulation_Translator_SimulationProtocol.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_Emulation_Translator_SimulationProtocol.hpp
//!
//! Declares test fixture class for testing Emulation_Translator_SimulationProtocol
//!
//===========================================================================


#ifndef UT_Emulation_Translator_EMULATIONPROTOCOL_H__1252752F_98CD_466E_ACBF_7835604010A5__INCLUDED_
  #define UT_Emulation_Translator_EMULATIONPROTOCOL_H__1252752F_98CD_466E_ACBF_7835604010A5__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class Emulation_Translator_SimulationProtocol
//!
//!
class UT_Emulation_TranslatorProtocol : public CxxTest::TestSuite
{
  public:
   UT_Emulation_TranslatorProtocol() = default;
  ~UT_Emulation_TranslatorProtocol() = default;

  // ================ Test Methods ================

 void test_Constructor ();
 void test_TransformationCallback ();
 void test_SVF_RawPlayer_TransformationCallback();
 

};
//
//  End of UT_Emulation_Translator_EmulationProtocol class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_Emulation_Translator_EMULATIONPROTOCOL_H__1252752F_98CD_466E_ACBF_7835604010A5__INCLUDED_
//===========================================================================
// End of UT_Emulation_Translator_EmulationProtocol.hpp
//===========================================================================
