//===========================================================================
//                           PDL_Adapter.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PDL_Adapter.cpp
//!
//! Implements 'C' language API to system model
//===========================================================================

#include "PDL_Adapter.h"
#include "SystemModelManager.hpp"
#include "Startup.hpp"
#include "C_API_Commons.hpp"
#include "Utility.hpp"

#include <memory>
#include <string>
#include <cstring>
#include <algorithm>

using std::shared_ptr;
using std::string;

using namespace std::string_literals;
using namespace mast;

namespace
{

//! Copies binary vector bits as a C-Style string
//!
//! @param binVector                        A BinaryVector to "export" to C-Style string
//! @param readDataBuffer                   A buffer to build the C-Style string
//! @param [in, out] readDataBufferLength   As input, point to input buffer length, as output this is same as strlen() for the return string
//!
void CopyBinaryVectorToCstr (const BinaryVector& binVector, char* readDataBuffer, size_t* readDataBufferLength, StringType stringType)
{
  // ---------------- Convert BinaryVector to string
  //
  string asString;

  switch (stringType)
  {
    case StringType::Binary:
      asString = binVector.DataAsBinaryString("", "", 0, "");
      break;
    case StringType::Hex:
      asString = binVector.DataAsHexString("", "", 0, "");
      break;
    case StringType::Decimal:
//+      asString = binVector.DataAsDecimalString("", "", 0, "");
      THROW_INVALID_ARGUMENT("StringType::Decimal is not yet supported !!!");
      break;
    default:
      THROW_INVALID_ARGUMENT("stringType must be Binary, Hex or Decimal"s + std::to_string(static_cast<std::underlying_type_t<StringType>>(stringType)));
      break;
  }

  // ---------------- Copy result
  //
  auto actualCount = std::min(*readDataBufferLength, asString.length() + 1u);
  std::strncpy(readDataBuffer, asString.c_str(), actualCount);

  // ---------------- Make sure it is null terminated
  //
  if (actualCount == *readDataBufferLength)
  {
    readDataBuffer[actualCount - 1u] = '\0';
  }

  *readDataBufferLength = actualCount - 1u; // Length does not count null terminating character
}
//
//  End of: CopyBinaryVectorToCstr
//---------------------------------------------------------------------------


  inline auto GetAndCheckManager()
  {
    auto manager = Startup::GetManager();
    CHECK_VALUE_NOT_NULL(manager, "Mast library has not been properly initialized");\
    return manager;
  }

  template<typename T> ErrorCode iGet_impl(const char* registerPath, T* readData)
  {
    auto retCode = ErrorCode::Ok;

    TRY_CATCH_ALL(retCode,
                  CHECK_PARAMETER_NOT_NULL(registerPath, "Register path must be not nullptr");
                  CHECK_PARAMETER_NOT_NULL(readData,     "Pointer to read data must be not nullptr");
                  auto manager = GetAndCheckManager();
                  manager->iGet(registerPath, *readData);
                 );

    return retCode;
  }

  template<typename T> ErrorCode iGetRefresh_impl(const char* registerPath, T* readData)
  {
    auto retCode = ErrorCode::Ok;

    TRY_CATCH_ALL(retCode,
                  CHECK_PARAMETER_NOT_NULL(registerPath, "Register path must be not nullptr");
                  CHECK_PARAMETER_NOT_NULL(readData,     "Pointer to read data must be not nullptr");
                  auto manager = GetAndCheckManager();
                  manager->iGetRefresh(registerPath, *readData);
                 );

    return retCode;
  }


  template<typename T> ErrorCode iRead_impl(const char* registerPath, T expectedValue)
  {
    auto retCode = ErrorCode::Ok;

    TRY_CATCH_ALL(retCode,
                  CHECK_PARAMETER_NOT_NULL(registerPath, "Register path must be not nullptr");
                  auto manager = GetAndCheckManager();
                  manager->iRead(registerPath, std::move(expectedValue));
                 );

    return retCode;
  }


  template<typename T> ErrorCode iWrite_impl(const char* registerPath, T value)
  {
    auto retCode = ErrorCode::Ok;

    TRY_CATCH_ALL(retCode,
                  CHECK_PARAMETER_NOT_NULL(registerPath, "Register path must be not nullptr");
                  auto manager = GetAndCheckManager();
                  manager->iWrite(registerPath, value);
                 );

    return retCode;
  }
} // End of unnamed namespace



//! Waits for all queued operations to be executed
//!
DLL_EXPORT ErrorCode iApply ()
{
  auto retCode = ErrorCode::Ok;

  TRY_CATCH_ALL(retCode,
                auto manager = GetAndCheckManager();
                manager->iApply();
               );

  return retCode;
}


//! Returns last Register value read from SUT as string
//!
ErrorCode iGet_String (const char* registerPath, char* readDataBuffer, size_t* readDataBufferLength, StringType stringType)
{
  auto retCode = ErrorCode::Ok;

  TRY_CATCH_ALL(retCode,
                CHECK_PARAMETER_NOT_NULL(registerPath,             "Register path must be not nullptr");
                CHECK_PARAMETER_NOT_NULL(readDataBuffer,           "Pointer to read data must be not nullptr");
                CHECK_PARAMETER_NOT_NULL(readDataBufferLength,     "Pointer to read data length");
                CHECK_PARAMETER_GT      (*readDataBufferLength, 1, "Buffer length must be > 1");    // At least 1 bit + null terminator

                auto manager   = GetAndCheckManager();
                auto gotVector = manager->iGet(registerPath);
                CopyBinaryVectorToCstr(gotVector, readDataBuffer, readDataBufferLength, stringType);
               );

  return retCode;
}

ErrorCode iGet_uint8_t  (const char* registerPath, uint8_t*  readData) { return iGet_impl(registerPath, readData); }
ErrorCode iGet_uint16_t (const char* registerPath, uint16_t* readData) { return iGet_impl(registerPath, readData); }
ErrorCode iGet_uint32_t (const char* registerPath, uint32_t* readData) { return iGet_impl(registerPath, readData); }
ErrorCode iGet_uint64_t (const char* registerPath, uint64_t* readData) { return iGet_impl(registerPath, readData); }
ErrorCode iGet_int8_t   (const char* registerPath, int8_t*   readData) { return iGet_impl(registerPath, readData); }
ErrorCode iGet_int16_t  (const char* registerPath, int16_t*  readData) { return iGet_impl(registerPath, readData); }
ErrorCode iGet_int32_t  (const char* registerPath, int32_t*  readData) { return iGet_impl(registerPath, readData); }
ErrorCode iGet_int64_t  (const char* registerPath, int64_t*  readData) { return iGet_impl(registerPath, readData); }



//! Returns XOR of the last value read from SUT and the expected value
//!
//! @note May contain x-values (for don't care)
//!
ErrorCode iGetMiscompares (const char* registerPath, char* buffer, size_t* bufferLength, StringType stringType)
{
  auto retCode = ErrorCode::Ok;

  //! @todo [JFC]-[August/02/2016]: In iGetMiscompares(): add support for don't care
  //!
  TRY_CATCH_ALL(retCode,
                CHECK_PARAMETER_NOT_NULL(registerPath,     "Register path must be not nullptr");
                CHECK_PARAMETER_NOT_NULL(buffer,           "Pointer tp data buffer must be not nullptr");
                CHECK_PARAMETER_NOT_NULL(bufferLength,     "Pointer to result length must be not nullptr");
                CHECK_PARAMETER_GT      (*bufferLength, 1, "Buffer length must be > 1");    // At least 1 bit + null terminator

                auto manager   = GetAndCheckManager();
                auto gotVector = manager->iGetMiscompares(registerPath);
                CopyBinaryVectorToCstr(gotVector, buffer, bufferLength, stringType);
               );

  return retCode;
}


//! Requests register value to be read from SUT and wait till it can be return (as string)
//!
ErrorCode iGetRefresh_String (const char* registerPath, char* readDataBuffer, size_t* readDataBufferLength, StringType stringType)
{
  auto retCode = ErrorCode::Ok;

  TRY_CATCH_ALL(retCode,
                CHECK_PARAMETER_NOT_NULL(registerPath,             "Register path must be not nullptr");
                CHECK_PARAMETER_NOT_NULL(readDataBuffer,           "Pointer to read data must be not nullptr");
                CHECK_PARAMETER_NOT_NULL(readDataBufferLength,     "Pointer to read data length");
                CHECK_PARAMETER_GT      (*readDataBufferLength, 1, "Buffer length must be > 1");    // At least 1 bit + null terminator

                auto manager   = GetAndCheckManager();
                auto gotVector = manager->iGetRefresh(registerPath);
                CopyBinaryVectorToCstr(gotVector, readDataBuffer, readDataBufferLength, stringType);
               );

  return retCode;
}

ErrorCode iGetRefresh_uint8_t  (const char* registerPath, uint8_t*  readData) { return iGetRefresh_impl(registerPath, readData); }
ErrorCode iGetRefresh_uint16_t (const char* registerPath, uint16_t* readData) { return iGetRefresh_impl(registerPath, readData); }
ErrorCode iGetRefresh_uint32_t (const char* registerPath, uint32_t* readData) { return iGetRefresh_impl(registerPath, readData); }
ErrorCode iGetRefresh_uint64_t (const char* registerPath, uint64_t* readData) { return iGetRefresh_impl(registerPath, readData); }
ErrorCode iGetRefresh_int8_t   (const char* registerPath, int8_t*   readData) { return iGetRefresh_impl(registerPath, readData); }
ErrorCode iGetRefresh_int16_t  (const char* registerPath, int16_t*  readData) { return iGetRefresh_impl(registerPath, readData); }
ErrorCode iGetRefresh_int32_t  (const char* registerPath, int32_t*  readData) { return iGetRefresh_impl(registerPath, readData); }
ErrorCode iGetRefresh_int64_t  (const char* registerPath, int64_t*  readData) { return iGetRefresh_impl(registerPath, readData); }



//! Returns the number of expected read failure for a single Register
//!
//! @param [in]  registerPath   Path to the register
//! @param [out] failureCount   Pointer to return the number of failures
//! @param [in]  clearCounter   When true, the mismatch counter is reset
//!
DLL_EXPORT ErrorCode iGetRegisterStatus (const char* registerPath, uint32_t* failureCount, bool clearCounter)
{
  auto retCode = ErrorCode::Ok;

  TRY_CATCH_ALL(retCode,
                CHECK_PARAMETER_NOT_NULL(registerPath, "Register path must be not nullptr");
                auto manager = GetAndCheckManager();
                *failureCount = manager->iGetStatus(registerPath, clearCounter);
               );

  return retCode;
}


//! Changes path prefix
//!
DLL_EXPORT ErrorCode iPrefix (const char* registerPath)
{
  auto retCode = ErrorCode::Ok;

  TRY_CATCH_ALL(retCode,
                CHECK_PARAMETER_NOT_NULL(registerPath, "Register path must be not nullptr");
                auto manager = GetAndCheckManager();
                manager->iPrefix(registerPath);
               );

  return retCode;
}

ErrorCode iRead_BinaryVector (const char* registerPath, const char*  value)
{
  auto retCode = ErrorCode::Ok;

  //! @todo [JFC]-[August/02/2016]: In iRead_BinaryVector(): Add support for don't care
  //!
  TRY_CATCH_ALL(retCode,
                CHECK_PARAMETER_NOT_NULL(registerPath, "Register path must be not nullptr");
                retCode = iRead_impl(registerPath, BinaryVector::CreateFromBinaryString(value))
               );

  return retCode;
}

ErrorCode iRead_uint8_t  (const char* registerPath, uint8_t  expectedValue) { return iRead_impl(registerPath, expectedValue); }
ErrorCode iRead_uint16_t (const char* registerPath, uint16_t expectedValue) { return iRead_impl(registerPath, expectedValue); }
ErrorCode iRead_uint32_t (const char* registerPath, uint32_t expectedValue) { return iRead_impl(registerPath, expectedValue); }
ErrorCode iRead_uint64_t (const char* registerPath, uint64_t expectedValue) { return iRead_impl(registerPath, expectedValue); }
ErrorCode iRead_int8_t   (const char* registerPath, int8_t   expectedValue) { return iRead_impl(registerPath, expectedValue); }
ErrorCode iRead_int16_t  (const char* registerPath, int16_t  expectedValue) { return iRead_impl(registerPath, expectedValue); }
ErrorCode iRead_int32_t  (const char* registerPath, int32_t  expectedValue) { return iRead_impl(registerPath, expectedValue); }
ErrorCode iRead_int64_t  (const char* registerPath, int64_t  expectedValue) { return iRead_impl(registerPath, expectedValue); }


//! Changes path prefix
//!
DLL_EXPORT ErrorCode iRefresh (const char* registerPath)
{
  auto retCode = ErrorCode::Ok;

  TRY_CATCH_ALL(retCode,
                CHECK_PARAMETER_NOT_NULL(registerPath, "Register path must not be nullptr");
                auto manager = GetAndCheckManager();
                manager->iRefresh(registerPath);
               );

  return retCode;
}


//! Requires write to register from binary string
//!
ErrorCode iWrite_BinaryVector (const char* registerPath, const char*  value)
{
  auto retCode = ErrorCode::Ok;

  TRY_CATCH_ALL(retCode,
                CHECK_PARAMETER_NOT_NULL(registerPath, "Register path must be not nullptr");
                retCode = iWrite_impl(registerPath, BinaryVector::CreateFromBinaryString(value))
               );

  return retCode;
}

ErrorCode iWrite_uint8_t  (const char* registerPath, uint8_t  value)  { return iWrite_impl(registerPath, value); }
ErrorCode iWrite_uint16_t (const char* registerPath, uint16_t value)  { return iWrite_impl(registerPath, value); }
ErrorCode iWrite_uint32_t (const char* registerPath, uint32_t value)  { return iWrite_impl(registerPath, value); }
ErrorCode iWrite_uint64_t (const char* registerPath, uint64_t value)  { return iWrite_impl(registerPath, value); }
ErrorCode iWrite_int8_t   (const char* registerPath, int8_t   value)  { return iWrite_impl(registerPath, value); }
ErrorCode iWrite_int16_t  (const char* registerPath, int16_t  value)  { return iWrite_impl(registerPath, value); }
ErrorCode iWrite_int32_t  (const char* registerPath, int32_t  value)  { return iWrite_impl(registerPath, value); }
ErrorCode iWrite_int64_t  (const char* registerPath, int64_t  value)  { return iWrite_impl(registerPath, value); }

//===========================================================================
// End of PDL_Adapter.cpp
//===========================================================================
