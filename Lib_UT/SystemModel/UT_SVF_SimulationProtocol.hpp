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


#ifndef UT_SVF_SIMULATIONPROTOCOL_H__1252752F_98CD_466E_ACBF_7835604010A5__INCLUDED_
  #define UT_SVF_SIMULATIONPROTOCOL_H__1252752F_98CD_466E_ACBF_7835604010A5__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class SVF_SimulationProtocol
//!
//!
class UT_SVF_SimulationProtocol : public CxxTest::TestSuite
{
  public:
   UT_SVF_SimulationProtocol() = default;
  ~UT_SVF_SimulationProtocol() = default;

  // ================ Test Methods ================

  void test_DefaultConstructor ();
  void test_Constructor ();

  void test_DoAction_Bad_ToSutPath ();
  void test_DoAction_Bad_FromSutPath ();
  void test_DoAction_Sync ();
  void test_DoAction_ASync ();


  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_SVF_SimulationProtocol class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_SVF_SIMULATIONPROTOCOL_H__1252752F_98CD_466E_ACBF_7835604010A5__INCLUDED_
//===========================================================================
// End of UT_SVF_SimulationProtocol.hpp
//===========================================================================
