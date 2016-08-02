//===========================================================================
//                           UT_SystemModelManager.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SystemModelManager.hpp
//!
//! Declares test fixture class for testing SystemModelManager
//!
//===========================================================================


#ifndef UT_SYSTEMMODELMANAGER_H__344BA89D_474D_485A_218C_DC3C397F936B__INCLUDED_
  #define UT_SYSTEMMODELMANAGER_H__344BA89D_474D_485A_218C_DC3C397F936B__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class SystemModelManager
//!
class UT_SystemModelManager : public CxxTest::TestSuite
{
  public:
   UT_SystemModelManager() = default;
  ~UT_SystemModelManager() = default;

  // ================ Test Methods ================

  // ---------------- Constructor
  //
  void test_Constructor_EmptySystemModel ();
  void test_Constructor_SystemModel_Without_AI ();
  void test_Constructor_Root_is_AI ();
  void test_Constructor_Root_is_Chain_1_AI ();
  void test_Constructor_Root_is_Chain_3_AI ();

  // ---------------- DoDataCycles
  //
  void test_DoDataCycles_AccessInterface ();
  void test_DoDataCycles_1500 ();
  void test_DoDataCycles_1500_SVF ();
  void test_DoDataCycles_1500_I2C ();
  void test_DoDataCycles_1500_Generic ();
  void test_DoDataCycles_1500_Greedy ();
  void test_DoDataCycles_1500_Lazy ();

  void test_DoDataCycles_MIB_Multichain_Pre ();
  void test_DoDataCycles_MIB_Multichain_Pre_Greedy ();
  void test_DoDataCycles_MIB_Multichain_Pre_Lazy ();
  void test_DoDataCycles_MIB_Multichain_Post ();

  void test_CreateApplicationThread_1_App ();
  void test_CreateApplicationThread_2_App ();
  void test_CreateApplicationThread_Top_is_Nullptr ();

  void test_iPrefix_Thread_is_SystemModelManager ();
  void test_iPrefix_Thread_is_Known ();
  void test_iPrefix_Thread_is_Unknown ();

  // ---------------- iGet
  //
  void test_iGet_SingleThread_BinaryVector ();
  void test_iGet_SingleThread_uint8 ();
  void test_iGet_SingleThread_uint16 ();
  void test_iGet_SingleThread_uint32 ();
  void test_iGet_SingleThread_uint64 ();
  void test_iGet_SingleThread_int8 ();
  void test_iGet_SingleThread_int16 ();
  void test_iGet_SingleThread_int32 ();
  void test_iGet_SingleThread_int64 ();
  void test_iGet_Thread_is_Known ();
  void test_iGet_Thread_is_Unknown ();

  // ---------------- iWrite
  //
  void test_iWrite_SingleThread_BinaryVector ();
  void test_iWrite_SingleThread_uint8 ();
  void test_iWrite_SingleThread_uint16 ();
  void test_iWrite_SingleThread_uint32 ();
  void test_iWrite_SingleThread_uint64 ();
  void test_iWrite_SingleThread_int8 ();
  void test_iWrite_SingleThread_int16 ();
  void test_iWrite_SingleThread_int32 ();
  void test_iWrite_SingleThread_int64 ();

  void test_iWrite_Thread_is_Known ();
  void test_iWrite_Thread_is_Unknown ();

  // ---------------- iApply
  //
  void test_iApply_Thread_is_SystemModelManager ();
  void test_iApply_Thread_is_SystemModelManager_NoPending ();
  void test_iApply_Thread_is_Known ();
  void test_iApply_Thread_is_Known_NoPending ();
  void test_iApply_Thread_is_Unknown ();
  void test_iApply_DataCycleLoop_NotStarted ();
  void test_iApply_4_Threads_Once_SameReg ();
  void test_iApply_4_Threads_1_Write ();
  void test_iApply_4_Threads_N_Writes ();
  void test_iApply_4_Threads_N_Writes_TC_1500 ();

  // ---------------- iRefresh
  //
  void test_iRefresh ();
  void test_iRefresh_4_RegPerApp ();
  void test_iGetRefresh ();

  // ---------------- iGetStatus
  //
  void test_iGetStatus_Register_WithoutClear ();
  void test_iGetStatus_Register_WithClear ();
  void test_iGetStatus_Register_Multithread ();

  // ---------------- iGetMiscompares
  //
  void test_iGetMiscompares ();
  void test_iGetMiscompares_Multithread ();

  // ================ Other Methods ================

  void setUp();    //!< Initializes test (called for each test)
  void tearDown(); //!< Cleanups test (called for each test)
};
//
//  End of UT_SystemModelManager class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_SYSTEMMODELMANAGER_H__344BA89D_474D_485A_218C_DC3C397F936B__INCLUDED_
//===========================================================================
// End of UT_SystemModelManager.hpp
//===========================================================================
