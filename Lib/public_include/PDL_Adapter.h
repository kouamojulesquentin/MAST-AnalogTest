//===========================================================================
//                           PDL_Adapter.h
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PDL_Adapter.h
//!
//! Declares C language API to PDL
//!
//===========================================================================


#ifndef PDL_ADAPTER_H__91DB123F_F997_4574_50B6_54BF85F6D69A__INCLUDED_
  #define PDL_ADAPTER_H__91DB123F_F997_4574_50B6_54BF85F6D69A__INCLUDED_

#include "SystemModelAdapter.h"
#include "Platform.hpp"

#include <stddef.h>
#include <stdint.h>

extern "C"
{
  //! Defines string representation of register values
  //!
  enum  StringType
  {
    Binary, //!< Binary format with prepended "0b"
    Hex,    //!< Hexadecimal format with prepended "0x"
    Decimal //!< Decimal format
  };


  // ---------------- iApply
  //
  DLL_EXPORT ErrorCode iApply (); //!< Waits for all queued operations to be executed

  // ---------------- iGet
  //
  DLL_EXPORT ErrorCode iGet_String   (const char* registerPath, char*     readDataBuffer, size_t* readDataBufferLength, StringType stringType); //!< Returns last Register value read from SUT as binary string
  DLL_EXPORT ErrorCode iGet_uint8_t  (const char* registerPath, uint8_t*  readData); //!< Returns last Register value read from SUT as uint8_t
  DLL_EXPORT ErrorCode iGet_uint16_t (const char* registerPath, uint16_t* readData); //!< Returns last Register value read from SUT as uint16_t
  DLL_EXPORT ErrorCode iGet_uint32_t (const char* registerPath, uint32_t* readData); //!< Returns last Register value read from SUT as uint32_t
  DLL_EXPORT ErrorCode iGet_uint64_t (const char* registerPath, uint64_t* readData); //!< Returns last Register value read from SUT as uint64_t
  DLL_EXPORT ErrorCode iGet_int8_t   (const char* registerPath, int8_t*   readData); //!< Returns last Register value read from SUT as int8_t
  DLL_EXPORT ErrorCode iGet_int16_t  (const char* registerPath, int16_t*  readData); //!< Returns last Register value read from SUT as int16_t
  DLL_EXPORT ErrorCode iGet_int32_t  (const char* registerPath, int32_t*  readData); //!< Returns last Register value read from SUT as int32_t
  DLL_EXPORT ErrorCode iGet_int64_t  (const char* registerPath, int64_t*  readData); //!< Returns last Register value read from SUT as int64_t

  // ---------------- iGetMiscompares
  //
  DLL_EXPORT ErrorCode iGetMiscompares (const char* registerPath, char* buffer, size_t* bufferLength, StringType stringType); //!< Returns XOR of the last value read from SUT and the expected value

  // ---------------- iGetRefresh
  //
  DLL_EXPORT ErrorCode iGetRefresh_String   (const char* registerPath, char*     readDataBuffer, size_t* readDataBufferLength, StringType stringType); //!< Requests register value to be read from SUT and wait till it can be return (as string)
  DLL_EXPORT ErrorCode iGetRefresh_uint8_t  (const char* registerPath, uint8_t*  readData); //!< Requests register value to be read from SUT and wait till it can be return as uint8_t
  DLL_EXPORT ErrorCode iGetRefresh_uint16_t (const char* registerPath, uint16_t* readData); //!< Requests register value to be read from SUT and wait till it can be return as uint16_t
  DLL_EXPORT ErrorCode iGetRefresh_uint32_t (const char* registerPath, uint32_t* readData); //!< Requests register value to be read from SUT and wait till it can be return as uint32_t
  DLL_EXPORT ErrorCode iGetRefresh_uint64_t (const char* registerPath, uint64_t* readData); //!< Requests register value to be read from SUT and wait till it can be return as uint64_t
  DLL_EXPORT ErrorCode iGetRefresh_int8_t   (const char* registerPath, int8_t*   readData); //!< Requests register value to be read from SUT and wait till it can be return as int8_t
  DLL_EXPORT ErrorCode iGetRefresh_int16_t  (const char* registerPath, int16_t*  readData); //!< Requests register value to be read from SUT and wait till it can be return as int16_t
  DLL_EXPORT ErrorCode iGetRefresh_int32_t  (const char* registerPath, int32_t*  readData); //!< Requests register value to be read from SUT and wait till it can be return as int32_t
  DLL_EXPORT ErrorCode iGetRefresh_int64_t  (const char* registerPath, int64_t*  readData); //!< Requests register value to be read from SUT and wait till it can be return as int64_t

  // ---------------- iGetStatus
  //
  DLL_EXPORT ErrorCode iGetRegisterStatus  (const char* registerPath, uint32_t* failureCount, bool clearCounter); //!< Returns the number of expected read failure for a single Register

  // ---------------- iPrefix
  //
  DLL_EXPORT ErrorCode iPrefix (const char* registerPath); //!< Changes path prefix

  // ---------------- iRead
  //
  DLL_EXPORT ErrorCode iRead_BinaryVector (const char* registerPath, const char* expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected binary string value
  DLL_EXPORT ErrorCode iRead_uint8_t      (const char* registerPath, uint8_t     expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected uint8_t value
  DLL_EXPORT ErrorCode iRead_uint16_t     (const char* registerPath, uint16_t    expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected uint16_t value
  DLL_EXPORT ErrorCode iRead_uint32_t     (const char* registerPath, uint32_t    expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected uint32_t value
  DLL_EXPORT ErrorCode iRead_uint64_t     (const char* registerPath, uint64_t    expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected uint64_t value
  DLL_EXPORT ErrorCode iRead_int8_t       (const char* registerPath, int8_t      expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected int8_t value
  DLL_EXPORT ErrorCode iRead_int16_t      (const char* registerPath, int16_t     expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected int16_t value
  DLL_EXPORT ErrorCode iRead_int32_t      (const char* registerPath, int32_t     expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected int32_t value
  DLL_EXPORT ErrorCode iRead_int64_t      (const char* registerPath, int64_t     expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected int64_t value

  // ---------------- iRefresh
  //
  DLL_EXPORT ErrorCode iRefresh (const char* registerPath); //!< Queues a request to (re-)read register value from SUT

  // ---------------- iWrite
  //
  DLL_EXPORT ErrorCode iWrite_BinaryVector (const char* registerPath, const char* value); //!< Requires write to register from binary string
  DLL_EXPORT ErrorCode iWrite_uint8_t      (const char* registerPath, uint8_t     value); //!< Requires write to register from uint8_t
  DLL_EXPORT ErrorCode iWrite_uint16_t     (const char* registerPath, uint16_t    value); //!< Requires write to register from uint16_t
  DLL_EXPORT ErrorCode iWrite_uint32_t     (const char* registerPath, uint32_t    value); //!< Requires write to register from uint32_t
  DLL_EXPORT ErrorCode iWrite_uint64_t     (const char* registerPath, uint64_t    value); //!< Requires write to register from uint64_t
  DLL_EXPORT ErrorCode iWrite_int8_t       (const char* registerPath, int8_t      value); //!< Requires write to register from int8_t
  DLL_EXPORT ErrorCode iWrite_int16_t      (const char* registerPath, int16_t     value); //!< Requires write to register from int16_t
  DLL_EXPORT ErrorCode iWrite_int32_t      (const char* registerPath, int32_t     value); //!< Requires write to register from int32_t
  DLL_EXPORT ErrorCode iWrite_int64_t      (const char* registerPath, int64_t     value); //!< Requires write to register from int64_t
}

#endif  // not defined PDL_ADAPTER_H__91DB123F_F997_4574_50B6_54BF85F6D69A__INCLUDED_
//===========================================================================
// End of PDL_Adapter.h
//===========================================================================



