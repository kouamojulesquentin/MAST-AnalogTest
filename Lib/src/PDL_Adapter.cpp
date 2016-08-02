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

using std::shared_ptr;
using std::string;

using namespace std::string_literals;
using namespace mast;

namespace
{
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

ErrorCode iGet_uint8_t  (const char* registerPath, uint8_t*  readData) { return iGet_impl(registerPath, readData); }
ErrorCode iGet_uint16_t (const char* registerPath, uint16_t* readData) { return iGet_impl(registerPath, readData); }
ErrorCode iGet_uint32_t (const char* registerPath, uint32_t* readData) { return iGet_impl(registerPath, readData); }
ErrorCode iGet_uint64_t (const char* registerPath, uint64_t* readData) { return iGet_impl(registerPath, readData); }
ErrorCode iGet_int8_t   (const char* registerPath, int8_t*   readData) { return iGet_impl(registerPath, readData); }
ErrorCode iGet_int16_t  (const char* registerPath, int16_t*  readData) { return iGet_impl(registerPath, readData); }
ErrorCode iGet_int32_t  (const char* registerPath, int32_t*  readData) { return iGet_impl(registerPath, readData); }
ErrorCode iGet_int64_t  (const char* registerPath, int64_t*  readData) { return iGet_impl(registerPath, readData); }



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
