//===========================================================================
//                           UT_PDL_Adapter_C.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_PDL_Adapter_C.hpp
//!
//! Declares test fixture class for testing PDL_Adapter_C
//!
//===========================================================================

#ifndef UT_PDL_ADAPTER_C_H__5BB4DECD_614B_4F3F_B39A_BB15F4612574__INCLUDED_
  #define UT_PDL_ADAPTER_C_H__5BB4DECD_614B_4F3F_B39A_BB15F4612574__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for "C" API PDL_Adapter_C functions
//!
class UT_PDL_Adapter_C : public CxxTest::TestSuite
{
  public:
   UT_PDL_Adapter_C() = default;
  ~UT_PDL_Adapter_C() = default;

  // ================ Test Methods ================

  // ---------------- iPrefix
  //
  void test_iPrefix_NotInitialized ();

  // ---------------- iGet_xx
  //
  void test_iGet_String_NotInitialized ();
  void test_iGet_uint8_NotInitialized ();
  void test_iGet_uint16_NotInitialized ();
  void test_iGet_uint32_NotInitialized ();
  void test_iGet_uint64_NotInitialized ();
  void test_iGet_int8_NotInitialized ();
  void test_iGet_int16_NotInitialized ();
  void test_iGet_int32_NotInitialized ();
  void test_iGet_int64_NotInitialized ();

  void test_iGet_String_Nullptr_Path ();
  void test_iGet_uint8_Nullptr_Path ();
  void test_iGet_uint16_Nullptr_Path ();
  void test_iGet_uint32_Nullptr_Path ();
  void test_iGet_uint64_Nullptr_Path ();
  void test_iGet_int8_Nullptr_Path ();
  void test_iGet_int16_Nullptr_Path ();
  void test_iGet_int32_Nullptr_Path ();
  void test_iGet_int64_Nullptr_Path ();

  void test_iGet_String_Nullptr_ReadData ();
  void test_iGet_uint8_Nullptr_ReadData ();
  void test_iGet_uint16_Nullptr_ReadData ();
  void test_iGet_uint32_Nullptr_ReadData ();
  void test_iGet_uint64_Nullptr_ReadData ();
  void test_iGet_int8_Nullptr_ReadData ();
  void test_iGet_int16_Nullptr_ReadData ();
  void test_iGet_int32_Nullptr_ReadData ();
  void test_iGet_int64_Nullptr_ReadData ();

  void test_iGet_String_Hex ();
  void test_iGet_String_Bin ();
  void test_iGet_uint8 ();
  void test_iGet_uint16 ();
  void test_iGet_uint32 ();
  void test_iGet_uint64 ();
  void test_iGet_int8 ();
  void test_iGet_int16 ();
  void test_iGet_int32 ();
  void test_iGet_int64 ();

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

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)
};
//
//  End of UT_PDL_Adapter_C class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_PDL_ADAPTER_C_H__5BB4DECD_614B_4F3F_B39A_BB15F4612574__INCLUDED_
//===========================================================================
// End of UT_PDL_Adapter_C.hpp
//===========================================================================
