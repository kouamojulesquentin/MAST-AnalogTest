//===========================================================================
//                           UT_SystemModelBuilder.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SystemModelBuilder.hpp
//!
//! Declares test fixture class for testing SystemModelBuilder
//!
//===========================================================================


#ifndef UT_SYSTEMMODELBUILDER_H__5E88D2CE_47B9_49A1_3281_B96164DF3552__INCLUDED_
  #define UT_SYSTEMMODELBUILDER_H__5E88D2CE_47B9_49A1_3281_B96164DF3552__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class SystemModelBuilder
//!
//!
class UT_SystemModelBuilder : public CxxTest::TestSuite
{
  public:
   UT_SystemModelBuilder() = default;
  ~UT_SystemModelBuilder() = default;

  // ================ Test Methods ================

  void test_Create_JTAG_TAP ();

  // ---------------- Create_PathSelector
  //
  void test_Create_PathSelector_Binary ();
  void test_Create_PathSelector_OneHot_CannotSelectNone ();
  void test_Create_PathSelector_OneHot_CanSelectNone ();
  void test_Create_PathSelector_NHot_CannotSelectNone ();
  void test_Create_PathSelector_NHot_CanSelectNone ();

  // ---------------- Create_MIB
  //
  void test_Create_MIB_1_Derivation ();
  void test_Create_MIB_4_Derivations ();

  // ---------------- DaisyChain_JTAG_TAPS
  //
  void test_DaisyChain_JTAG_TAPS ();
  void test_DaisyChain_JTAG_TAPS_NoName ();
  void test_DaisyChain_JTAG_TAPS_Append ();
  void test_DaisyChain_JTAG_TAPS_3rdTap ();
  void test_DaisyChain_JTAG_TAPS_4xTap ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_SystemModelBuilder class declaration
//---------------------------------------------------------------------------

#endif  // not defined UT_SYSTEMMODELBUILDER_H__5E88D2CE_47B9_49A1_3281_B96164DF3552__INCLUDED_
//===========================================================================
// End of UT_SystemModelBuilder.hpp
//===========================================================================
