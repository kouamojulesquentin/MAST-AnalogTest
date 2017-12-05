//===========================================================================
//                           UT_SVF_RawPlayer.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SVF_RawPlayer.hpp
//!
//! Declares test fixture class for testing SVF__RawPlayer
//!
//===========================================================================


#ifndef UT_SVF_RAWPLAYER_H__1252752F_98CD_466E_ACBF_7835604010A5__INCLUDED_
  #define UT_SVF_RAWPLAYER_H__1252752F_98CD_466E_ACBF_7835604010A5__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class UT_SVF_RawPlayer
//!
//!
class UT_SVF_RawPlayer : public CxxTest::TestSuite
{
  public:
   UT_SVF_RawPlayer() = default;
  ~UT_SVF_RawPlayer() = default;

  // ================ Test Methods ================

  void test_empty_Constructor ();
 
  void test_CallbackIds ();

  void test_doReset ();
  void test_Callbacks ();
  void test_Callbacks_multithread ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_SVF_RawPlayer class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_SVF_RAWPLAYER_H__1252752F_98CD_466E_ACBF_7835604010A5__INCLUDED_
//===========================================================================
// End of UT_SVF_SimulationProtocol.hpp
//===========================================================================
