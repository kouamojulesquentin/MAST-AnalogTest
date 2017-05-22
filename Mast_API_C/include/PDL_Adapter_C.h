//===========================================================================
//                           PDL_Adapter_C.h
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PDL_Adapter_C.h
//!
//! Declares C language API to PDL
//!
//===========================================================================

#ifndef PDL_ADAPTER_C_H__71981A37_3B9D_480F_F88B_CD06588A949E__INCLUDED_
  #define PDL_ADAPTER_C_H__71981A37_3B9D_480F_F88B_CD06588A949E__INCLUDED_

#include "C_API_export.h"
#include "SystemModelAdapter_C.h"

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
  C_API_EXPORT ErrorCode iApply (); //!< Waits for all queued operations to be executed

  // ---------------- iGet
  //
  C_API_EXPORT ErrorCode iGet_String   (const char* registerPath, char*     readDataBuffer, size_t* readDataBufferLength, StringType stringType); //!< Returns last Register value read from SUT as binary string
  C_API_EXPORT ErrorCode iGet_uint8_t  (const char* registerPath, uint8_t*  readData); //!< Returns last Register value read from SUT as uint8_t
  C_API_EXPORT ErrorCode iGet_uint16_t (const char* registerPath, uint16_t* readData); //!< Returns last Register value read from SUT as uint16_t
  C_API_EXPORT ErrorCode iGet_uint32_t (const char* registerPath, uint32_t* readData); //!< Returns last Register value read from SUT as uint32_t
  C_API_EXPORT ErrorCode iGet_uint64_t (const char* registerPath, uint64_t* readData); //!< Returns last Register value read from SUT as uint64_t
  C_API_EXPORT ErrorCode iGet_int8_t   (const char* registerPath, int8_t*   readData); //!< Returns last Register value read from SUT as int8_t
  C_API_EXPORT ErrorCode iGet_int16_t  (const char* registerPath, int16_t*  readData); //!< Returns last Register value read from SUT as int16_t
  C_API_EXPORT ErrorCode iGet_int32_t  (const char* registerPath, int32_t*  readData); //!< Returns last Register value read from SUT as int32_t
  C_API_EXPORT ErrorCode iGet_int64_t  (const char* registerPath, int64_t*  readData); //!< Returns last Register value read from SUT as int64_t

  // ---------------- iGetMiscompares
  //
  C_API_EXPORT ErrorCode iGetMiscompares (const char* registerPath, char* buffer, size_t* bufferLength, StringType stringType); //!< Returns XOR of the last value read from SUT and the expected value

  // ---------------- iGetRefresh
  //
  C_API_EXPORT ErrorCode iGetRefresh_String   (const char* registerPath, char*     readDataBuffer, size_t* readDataBufferLength, StringType stringType); //!< Requests register value to be read from SUT and wait till it can be return (as string)
  C_API_EXPORT ErrorCode iGetRefresh_uint8_t  (const char* registerPath, uint8_t*  readData); //!< Requests register value to be read from SUT and wait till it can be return as uint8_t
  C_API_EXPORT ErrorCode iGetRefresh_uint16_t (const char* registerPath, uint16_t* readData); //!< Requests register value to be read from SUT and wait till it can be return as uint16_t
  C_API_EXPORT ErrorCode iGetRefresh_uint32_t (const char* registerPath, uint32_t* readData); //!< Requests register value to be read from SUT and wait till it can be return as uint32_t
  C_API_EXPORT ErrorCode iGetRefresh_uint64_t (const char* registerPath, uint64_t* readData); //!< Requests register value to be read from SUT and wait till it can be return as uint64_t
  C_API_EXPORT ErrorCode iGetRefresh_int8_t   (const char* registerPath, int8_t*   readData); //!< Requests register value to be read from SUT and wait till it can be return as int8_t
  C_API_EXPORT ErrorCode iGetRefresh_int16_t  (const char* registerPath, int16_t*  readData); //!< Requests register value to be read from SUT and wait till it can be return as int16_t
  C_API_EXPORT ErrorCode iGetRefresh_int32_t  (const char* registerPath, int32_t*  readData); //!< Requests register value to be read from SUT and wait till it can be return as int32_t
  C_API_EXPORT ErrorCode iGetRefresh_int64_t  (const char* registerPath, int64_t*  readData); //!< Requests register value to be read from SUT and wait till it can be return as int64_t

  // ---------------- iGetStatus
  //
  C_API_EXPORT ErrorCode iGetNodeStatus  (const char* nodePath, uint32_t* failureCount, bool clearCounter); //!< Returns the number of expected read failure for a single node sub-tree (or just Register)
  C_API_EXPORT ErrorCode iGetStatus      (uint32_t* failureCount, bool clearCounter); //!< Returns the number of expected read failure for all SystemModel tree (from root)

  // ---------------- iPrefix
  //
  C_API_EXPORT ErrorCode iPrefix (const char* registerPath); //!< Changes path prefix

  // ---------------- iRead
  //
  C_API_EXPORT ErrorCode iRead_BinaryVector (const char* registerPath, const char* expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected binary string value

  C_API_EXPORT ErrorCode iRead_uint8_t  (const char* registerPath, uint8_t  expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected uint8_t value
  C_API_EXPORT ErrorCode iRead_uint16_t (const char* registerPath, uint16_t expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected uint16_t value
  C_API_EXPORT ErrorCode iRead_uint32_t (const char* registerPath, uint32_t expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected uint32_t value
  C_API_EXPORT ErrorCode iRead_uint64_t (const char* registerPath, uint64_t expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected uint64_t value
  C_API_EXPORT ErrorCode iRead_int8_t   (const char* registerPath, int8_t   expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected int8_t value
  C_API_EXPORT ErrorCode iRead_int16_t  (const char* registerPath, int16_t  expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected int16_t value
  C_API_EXPORT ErrorCode iRead_int32_t  (const char* registerPath, int32_t  expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected int32_t value
  C_API_EXPORT ErrorCode iRead_int64_t  (const char* registerPath, int64_t  expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected int64_t value

  C_API_EXPORT ErrorCode iRead_with_DontCare_uint8_t  (const char* registerPath, uint8_t  expectedValue, uint8_t  dontCareMask); //!< Queues a request to (re-)read register value from SUT giving an expected uint8_t value and don't care mask
  C_API_EXPORT ErrorCode iRead_with_DontCare_uint16_t (const char* registerPath, uint16_t expectedValue, uint16_t dontCareMask); //!< Queues a request to (re-)read register value from SUT giving an expected uint16_t value and don't care mask
  C_API_EXPORT ErrorCode iRead_with_DontCare_uint32_t (const char* registerPath, uint32_t expectedValue, uint32_t dontCareMask); //!< Queues a request to (re-)read register value from SUT giving an expected uint32_t value and don't care mask
  C_API_EXPORT ErrorCode iRead_with_DontCare_uint64_t (const char* registerPath, uint64_t expectedValue, uint64_t dontCareMask); //!< Queues a request to (re-)read register value from SUT giving an expected uint64_t value and don't care mask
  C_API_EXPORT ErrorCode iRead_with_DontCare_int8_t   (const char* registerPath, int8_t   expectedValue, int8_t   dontCareMask); //!< Queues a request to (re-)read register value from SUT giving an expected int8_t value and don't care mask
  C_API_EXPORT ErrorCode iRead_with_DontCare_int16_t  (const char* registerPath, int16_t  expectedValue, int16_t  dontCareMask); //!< Queues a request to (re-)read register value from SUT giving an expected int16_t value and don't care mask
  C_API_EXPORT ErrorCode iRead_with_DontCare_int32_t  (const char* registerPath, int32_t  expectedValue, int32_t  dontCareMask); //!< Queues a request to (re-)read register value from SUT giving an expected int32_t value and don't care mask
  C_API_EXPORT ErrorCode iRead_with_DontCare_int64_t  (const char* registerPath, int64_t  expectedValue, int64_t  dontCareMask); //!< Queues a request to (re-)read register value from SUT giving an expected int64_t value and don't care mask

  // ---------------- iRefresh
  //
  C_API_EXPORT ErrorCode iRefresh (const char* registerPath); //!< Queues a request to (re-)read register value from SUT

  //! Forces the ResetPort to be asserted on the target module and reset SystemModel accordingly
  //!
  C_API_EXPORT ErrorCode iReset(bool doSynchronousReset);

  // ---------------- iWrite
  //
  C_API_EXPORT ErrorCode iWrite_BinaryVector (const char* registerPath, const char* value); //!< Requires write to register from binary string
  C_API_EXPORT ErrorCode iWrite_uint8_t      (const char* registerPath, uint8_t     value); //!< Requires write to register from uint8_t
  C_API_EXPORT ErrorCode iWrite_uint16_t     (const char* registerPath, uint16_t    value); //!< Requires write to register from uint16_t
  C_API_EXPORT ErrorCode iWrite_uint32_t     (const char* registerPath, uint32_t    value); //!< Requires write to register from uint32_t
  C_API_EXPORT ErrorCode iWrite_uint64_t     (const char* registerPath, uint64_t    value); //!< Requires write to register from uint64_t
  C_API_EXPORT ErrorCode iWrite_int8_t       (const char* registerPath, int8_t      value); //!< Requires write to register from int8_t
  C_API_EXPORT ErrorCode iWrite_int16_t      (const char* registerPath, int16_t     value); //!< Requires write to register from int16_t
  C_API_EXPORT ErrorCode iWrite_int32_t      (const char* registerPath, int32_t     value); //!< Requires write to register from int32_t
  C_API_EXPORT ErrorCode iWrite_int64_t      (const char* registerPath, int64_t     value); //!< Requires write to register from int64_t
}


#endif  // not defined PDL_ADAPTER_C_H__71981A37_3B9D_480F_F88B_CD06588A949E__INCLUDED_
//===========================================================================
// End of PDL_Adapter_C.h
//===========================================================================



