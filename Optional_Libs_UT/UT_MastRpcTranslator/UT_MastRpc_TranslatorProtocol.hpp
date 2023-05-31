//===========================================================================
//                           UT_MastRpc_Translator_SimulationProtocol.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_MastRpc_Translator_SimulationProtocol.hpp
//!
//! Declares test fixture class for testing MastRpc_Translator_SimulationProtocol
//!
//===========================================================================


#ifndef UT_MastRpc_Translator_MastRpcPROTOCOL_H__1252752F_98CD_466E_ACBF_7835604010A5__INCLUDED_
  #define UT_MastRpc_Translator_MastRpcPROTOCOL_H__1252752F_98CD_466E_ACBF_7835604010A5__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class MastRpc_Translator_SimulationProtocol
//!
//!
class UT_Remote_Loopback_TranslatorProtocol : public CxxTest::TestSuite
{
  public:
   UT_Remote_Loopback_TranslatorProtocol() = default;
  ~UT_Remote_Loopback_TranslatorProtocol() = default;

  // ================ Test Methods ================

 void test_Constructor ();
 void test_TransformationCallback ();
 void test_SVF_RawPlayer_TransformationCallback();
 

};
//
//  End of UT_MastRpc_Translator_MastRpcProtocol class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_MastRpc_Translator_MastRpcPROTOCOL_H__1252752F_98CD_466E_ACBF_7835604010A5__INCLUDED_
//===========================================================================
// End of UT_MastRpc_Translator_MastRpcProtocol.hpp
//===========================================================================
