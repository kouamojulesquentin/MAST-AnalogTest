//===========================================================================
//                           UT_MastEnvironment.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_MastEnvironment.hpp
//!
//! Declares test fixture class for testing MastEnvironment
//!
//===========================================================================


#ifndef UT_MASTENVIRONMENT_H__FEAFF4E5_5F01_4168_589D_FF4A37021EA2__INCLUDED_
  #define UT_MASTENVIRONMENT_H__FEAFF4E5_5F01_4168_589D_FF4A37021EA2__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class MastEnvironment
//!
//!
class UT_MastEnvironment : public CxxTest::TestSuite
{
  public:
   UT_MastEnvironment() = default;
  ~UT_MastEnvironment() = default;

  // ================ Test Methods ================

  // ---------------- Constructor
  //
  void test_Constructor_UnitTest ();

  // ---------------- ParseOptions
  //
  void test_ParseOptions_NoOptions ();
  void test_ParseOptions_bad_Args ();
  void test_ParseOptions_no_Yaml ();
  void test_ParseOptions_with_Yaml ();
  void test_ParseOptions_Argc_Argv ();

  // ---------------- LoadPlugins
  //
  void test_LoadPlugins_BeforeParsingOptions ();
  void test_LoadPlugins_AfterParsingOptions ();

  // ---------------- CreateSystemModel
  //
  void test_CreateSystemModel_BeforeLoadPlugins ();
  void test_CreateSystemModel_AfterLoadPlugins ();
  void test_CreateSystemModel_SIT_Error ();
  void test_CreateSystemModel_SIT_Error_no_CheckFile ();
  void test_CreateSystemModel_SIT_Model_Exports ();
  void test_CreateSystemModel_ChangeProtocol ();
  void test_CreateSystemModel_ChangeProtocol_TopIsChain ();

  // ---------------- CreateManager
  //
  void test_CreateManager_BeforeCreateSystemModel ();
  void test_CreateManager_AfterCreateSystemModel ();
  void test_CreateManager_Monitoring ();

  // ---------------- CreateApplications
  //
  void test_CreateApplications_BeforeCreateManager ();
  void test_CreateApplications_AfterCreateManager ();
  void test_CreateApplications_NotFoundApp ();

  // ---------------- Start
  //
  void test_Start_BeforeCreateApp ();
  void test_Start_AfterCreateApp ();
  void test_Start_RealPDLAlgo ();
  void test_Start_Arguments_RealPDLAlgo ();
  void test_Start_Argc_Argv_RealPDLAlgo ();

  // ---------------- Tests with translators
  //
  void test_Start_RealPDLAlgo_JTAG_Emu ();
  void test_Start_RealPDLAlgo_JTAG_2_I2C_Emu ();
  void test_Start_RealPDLAlgo_JTAG_BitBang ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_MastEnvironment class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_MASTENVIRONMENT_H__FEAFF4E5_5F01_4168_589D_FF4A37021EA2__INCLUDED_
//===========================================================================
// End of UT_MastEnvironment.hpp
//===========================================================================
