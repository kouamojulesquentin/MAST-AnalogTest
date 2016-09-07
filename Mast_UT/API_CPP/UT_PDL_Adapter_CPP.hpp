//===========================================================================
//                           UT_PDL_Adapter_CPP.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_PDL_Adapter_CPP.hpp
//!
//! Declares test fixture class for testing PDL_Adapter_CPP
//!
//===========================================================================

#ifndef UT_PDL_ADAPTER_CPP_H__637F98DF_F67A_4CE7_DBA0_8A468CF82F89__INCLUDED_
  #define UT_PDL_ADAPTER_CPP_H__637F98DF_F67A_4CE7_DBA0_8A468CF82F89__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for "C++" API PDL_Adapter_CPP functions
//!
class UT_PDL_Adapter_CPP : public CxxTest::TestSuite
{
  public:
   UT_PDL_Adapter_CPP() = default;
  ~UT_PDL_Adapter_CPP() = default;

  // ================ Test Methods ================

  // ---------------- iPrefix
  //
  void test_iPrefix_NotInitialized ();

  // ---------------- iGet_xx
  //
  void test_iGet_uint8_NotInitialized ();
  void test_iGet_uint16_NotInitialized ();
  void test_iGet_uint32_NotInitialized ();
  void test_iGet_uint64_NotInitialized ();
  void test_iGet_int8_NotInitialized ();
  void test_iGet_int16_NotInitialized ();
  void test_iGet_int32_NotInitialized ();
  void test_iGet_int64_NotInitialized ();
  void test_iGet_String_NotInitialized ();

  void test_iGet_uint8_Empty_Path ();
  void test_iGet_uint16_Empty_Path ();
  void test_iGet_uint32_Empty_Path ();
  void test_iGet_uint64_Empty_Path ();
  void test_iGet_int8_Empty_Path ();
  void test_iGet_int16_Empty_Path ();
  void test_iGet_int32_Empty_Path ();
  void test_iGet_int64_Empty_Path ();
  void test_iGet_String_Empty_Path ();

  void test_iGet_uint8 ();
  void test_iGet_uint16 ();
  void test_iGet_uint32 ();
  void test_iGet_uint64 ();
  void test_iGet_int8 ();
  void test_iGet_int16 ();
  void test_iGet_int32 ();
  void test_iGet_int64 ();
  void test_iGet_Sugar ();
  void test_iGet_String ();
  void test_iGet_String_Sugar ();

  // ---------------- iWrite_xx
  //
  void test_iWrite_BinaryVector_NotInitialized ();
  void test_iWrite_uint8_NotInitialized ();
  void test_iWrite_uint16_NotInitialized ();
  void test_iWrite_uint32_NotInitialized ();
  void test_iWrite_uint64_NotInitialized ();
  void test_iWrite_int8_NotInitialized ();
  void test_iWrite_int16_NotInitialized ();
  void test_iWrite_int32_NotInitialized ();
  void test_iWrite_int64_NotInitialized ();

//+  void test_iWrite_BinaryVector_InvalidValue ();

  void test_iWrite_BinaryVector ();
  void test_iWrite_uint8 ();
  void test_iWrite_uint16 ();
  void test_iWrite_uint32 ();
  void test_iWrite_uint64 ();
  void test_iWrite_int8 ();
  void test_iWrite_int16 ();
  void test_iWrite_int32 ();
  void test_iWrite_int64 ();

  // ---------------- iGetRefresh
  //
  void test_iGetRefresh_uint8 ();
  void test_iGetRefresh_uint16 ();
  void test_iGetRefresh_uint32 ();
  void test_iGetRefresh_uint64 ();
  void test_iGetRefresh_int8 ();
  void test_iGetRefresh_int16 ();
  void test_iGetRefresh_int32 ();
  void test_iGetRefresh_int64 ();
  void test_iGetRefresh_Sugar ();
  void test_iGetRefresh_String ();
  void test_iGetRefresh_String_Sugar ();

  // ---------------- iGetMiscompares
  //
  void test_iGetMiscompares_Expecting_Zero ();
  void test_iGetMiscompares ();
  void test_iGetMiscompares_Sugar ();

  // ---------------- iGetRegisterStatus
  //
  void test_iGetRegisterStatus_with_Reset ();
  void test_iGetRegisterStatus_without_Reset ();

  // ---------------- iRefresh
  //
  void test_iRefresh ();


  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_PDL_Adapter_CPP class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_PDL_ADAPTER_CPP_H__637F98DF_F67A_4CE7_DBA0_8A468CF82F89__INCLUDED_
//===========================================================================
// End of UT_PDL_Adapter_CPP.hpp
//===========================================================================
