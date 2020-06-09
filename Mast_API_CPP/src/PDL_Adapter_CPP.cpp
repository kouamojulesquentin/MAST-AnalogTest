//===========================================================================
//                           PDL_Adapter_CPP.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PDL_Adapter_CPP.cpp
//!
//! Implements 'C++' language API to PDL
//===========================================================================

#include "PDL_Adapter_CPP.hpp"
#include "SystemModelManager.hpp"
#include "BinaryVector.hpp"
#include "Startup.hpp"
#include "Utility.hpp"

#include <memory>
#include <string>
#include <cstring>
#include <algorithm>

using std::shared_ptr;
using std::string;

using string_view = std::experimental::string_view;

using namespace std::string_literals;
using namespace mast;

#define CHECK_REGISTER_PATH(registerPath) CHECK_PARAMETER_NOT_EMPTY(registerPath, "Register path must be not empty")
#define CHECK_NODE_PATH(nodePath)         CHECK_PARAMETER_NOT_EMPTY(nodePath,     "Node path must be not empty")

namespace
{
//! Copies binary vector bits as a C-Style string
//!
//! @param binVector  A BinaryVector to "export" to C-Style string
//! @param StringType Base of representation (Hex, Binary or Decimal)
//!
string BinaryVectorAsString (const BinaryVector& binVector, StringType stringType)
{
  // ---------------- Convert BinaryVector to string
  //
  string asString;

  switch (stringType)
  {
    case StringType::Binary:
      asString = binVector.DataAsBinaryString("", "", 0, "", true);
      break;
    case StringType::Hex:
      asString = binVector.DataAsHexString("", "", 0, "", true);
      break;
    case StringType::Decimal:
//+      asString = binVector.DataAsDecimalString("", "", 0, "");
      THROW_INVALID_ARGUMENT("StringType::Decimal is not yet supported !!!");
      break;
    default:
      THROW_INVALID_ARGUMENT("stringType must be Binary, Hex or Decimal"s + std::to_string(static_cast<std::underlying_type_t<StringType>>(stringType)));
      break;
  }

  return asString;
}
//
//  End of: BinaryVectorAsString
//---------------------------------------------------------------------------

inline auto GetAndCheckManager()
{
  auto manager = Startup::GetManager();
  CHECK_VALUE_NOT_NULL(manager, "Mast library has not been properly initialized");\
  return manager;
}

template<typename T> void iGet_impl(string_view registerPath, T& readData)
{
  CHECK_REGISTER_PATH(registerPath);

  auto manager = GetAndCheckManager();
  manager->iGet(registerPath, readData);
}

template<typename T> void iGetRefresh_impl(string_view registerPath, T& readData)
{
  CHECK_REGISTER_PATH(registerPath);

  auto manager = GetAndCheckManager();
  manager->iGetRefresh(registerPath, readData);
}


template<typename T> void iRead_impl(string_view registerPath, T expectedValue)
{
  CHECK_REGISTER_PATH(registerPath);

  auto manager = GetAndCheckManager();
  manager->iRead(registerPath, std::move(expectedValue));
}

template<typename T> void iRead_impl(string_view registerPath, T expectedValue, T dontCareMask)
{
  CHECK_REGISTER_PATH(registerPath);

  auto manager = GetAndCheckManager();
  manager->iRead(registerPath, std::move(expectedValue), std::move(dontCareMask));
}



template<typename T> void iWrite_impl(string_view registerPath, T value)
{
  CHECK_REGISTER_PATH(registerPath);

  auto manager = GetAndCheckManager();
  manager->iWrite(registerPath, std::move(value));
}


template<typename T> void iScan_impl(string_view registerPath, T value)
{
  CHECK_REGISTER_PATH(registerPath);

  auto manager = GetAndCheckManager();
  manager->iScan(registerPath, std::move(value));
}


template<typename T> void iScan_impl(string_view registerPath, T value, T expectedValue)
{
  CHECK_REGISTER_PATH(registerPath);

  auto manager = GetAndCheckManager();
  manager->iScan(registerPath, std::move(value),std::move(expectedValue));
}

} // End of unnamed namespace


//! Waits for all queued operations to be executed
//!
void mast::iApply ()
{
  auto manager = GetAndCheckManager();
  manager->iApply();
}



void mast::iGet (string_view registerPath, uint8_t&  readData) { iGet_impl(registerPath, readData); }
void mast::iGet (string_view registerPath, uint16_t& readData) { iGet_impl(registerPath, readData); }
void mast::iGet (string_view registerPath, uint32_t& readData) { iGet_impl(registerPath, readData); }
void mast::iGet (string_view registerPath, uint64_t& readData) { iGet_impl(registerPath, readData); }
void mast::iGet (string_view registerPath, int8_t&   readData) { iGet_impl(registerPath, readData); }
void mast::iGet (string_view registerPath, int16_t&  readData) { iGet_impl(registerPath, readData); }
void mast::iGet (string_view registerPath, int32_t&  readData) { iGet_impl(registerPath, readData); }
void mast::iGet (string_view registerPath, int64_t&  readData) { iGet_impl(registerPath, readData); }

std::string mast::iGet (string_view registerPath, StringType stringType)
{
  CHECK_REGISTER_PATH(registerPath);

  auto manager   = GetAndCheckManager();
  auto gotVector = manager->iGet(registerPath);
  return BinaryVectorAsString(gotVector, stringType);
}
void mast::iGet (string_view registerPath, std::string& readData, StringType stringType) { readData = iGet(registerPath, stringType); }


//! Returns XOR of the last value read from SUT and the expected value
//!
//! @note May contain x-values (for don't care)
//!
string mast::iGetMiscompares (string_view registerPath, StringType stringType)
{
  CHECK_REGISTER_PATH(registerPath);

  auto manager   = GetAndCheckManager();
  auto gotVector = manager->iGetMiscompares(registerPath);
  return BinaryVectorAsString(gotVector, stringType);
}
void mast::iGetMiscompares (string_view registerPath, std::string& xorResult, StringType stringType) { xorResult = mast::iGetMiscompares(registerPath, stringType); }


void mast::iGetRefresh (string_view registerPath, uint8_t&  readData) { return iGetRefresh_impl(registerPath, readData); }
void mast::iGetRefresh (string_view registerPath, uint16_t& readData) { return iGetRefresh_impl(registerPath, readData); }
void mast::iGetRefresh (string_view registerPath, uint32_t& readData) { return iGetRefresh_impl(registerPath, readData); }
void mast::iGetRefresh (string_view registerPath, uint64_t& readData) { return iGetRefresh_impl(registerPath, readData); }
void mast::iGetRefresh (string_view registerPath, int8_t&   readData) { return iGetRefresh_impl(registerPath, readData); }
void mast::iGetRefresh (string_view registerPath, int16_t&  readData) { return iGetRefresh_impl(registerPath, readData); }
void mast::iGetRefresh (string_view registerPath, int32_t&  readData) { return iGetRefresh_impl(registerPath, readData); }
void mast::iGetRefresh (string_view registerPath, int64_t&  readData) { return iGetRefresh_impl(registerPath, readData); }

string mast::iGetRefresh (string_view registerPath, StringType stringType)
{
  CHECK_REGISTER_PATH(registerPath);

  auto manager   = GetAndCheckManager();
  auto gotVector = manager->iGetRefresh(registerPath);

  return BinaryVectorAsString(gotVector, stringType);
}
void mast::iGetRefresh (string_view registerPath, std::string& readData, StringType stringType) { readData = mast::iGetRefresh(registerPath, stringType); }


uint32_t mast::iGetStatus (string_view nodePath, bool clearCounter)
{
  CHECK_NODE_PATH(nodePath);

  auto   manager = GetAndCheckManager();
  return manager->iGetStatus(nodePath, clearCounter);
}


uint32_t mast::iGetStatus (bool clearCounter)
{
  auto   manager = GetAndCheckManager();
  return manager->iGetStatus(clearCounter);
}


//! Changes path prefix
//!
void mast::iPrefix (string_view registerPath)
{
  CHECK_REGISTER_PATH(registerPath);

  auto manager = GetAndCheckManager();
  manager->iPrefix(string(registerPath));
}


void mast::iRead (string_view registerPath, uint8_t  expectedValue) { return iRead_impl(registerPath, expectedValue); }
void mast::iRead (string_view registerPath, uint16_t expectedValue) { return iRead_impl(registerPath, expectedValue); }
void mast::iRead (string_view registerPath, uint32_t expectedValue) { return iRead_impl(registerPath, expectedValue); }
void mast::iRead (string_view registerPath, uint64_t expectedValue) { return iRead_impl(registerPath, expectedValue); }
void mast::iRead (string_view registerPath, int8_t   expectedValue) { return iRead_impl(registerPath, expectedValue); }
void mast::iRead (string_view registerPath, int16_t  expectedValue) { return iRead_impl(registerPath, expectedValue); }
void mast::iRead (string_view registerPath, int32_t  expectedValue) { return iRead_impl(registerPath, expectedValue); }
void mast::iRead (string_view registerPath, int64_t  expectedValue) { return iRead_impl(registerPath, expectedValue); }

void mast::iRead (string_view registerPath, uint8_t  expectedValue, uint8_t  dontCareMask) { return iRead_impl(registerPath, expectedValue, dontCareMask); }
void mast::iRead (string_view registerPath, uint16_t expectedValue, uint16_t dontCareMask) { return iRead_impl(registerPath, expectedValue, dontCareMask); }
void mast::iRead (string_view registerPath, uint32_t expectedValue, uint32_t dontCareMask) { return iRead_impl(registerPath, expectedValue, dontCareMask); }
void mast::iRead (string_view registerPath, uint64_t expectedValue, uint64_t dontCareMask) { return iRead_impl(registerPath, expectedValue, dontCareMask); }
void mast::iRead (string_view registerPath, int8_t   expectedValue, int8_t   dontCareMask) { return iRead_impl(registerPath, expectedValue, dontCareMask); }
void mast::iRead (string_view registerPath, int16_t  expectedValue, int16_t  dontCareMask) { return iRead_impl(registerPath, expectedValue, dontCareMask); }
void mast::iRead (string_view registerPath, int32_t  expectedValue, int32_t  dontCareMask) { return iRead_impl(registerPath, expectedValue, dontCareMask); }
void mast::iRead (string_view registerPath, int64_t  expectedValue, int64_t  dontCareMask) { return iRead_impl(registerPath, expectedValue, dontCareMask); }

void mast::iRead (string_view registerPath, string_view expectedValue) { return iRead_impl(registerPath, BinaryVector::CreateFromString(expectedValue)); }


//! Changes path prefix
//!
void mast::iRefresh (string_view registerPath)
{
  CHECK_REGISTER_PATH(registerPath);

  auto manager = GetAndCheckManager();
  manager->iRefresh(registerPath);
}



//! Forces the ResetPort to be asserted on the target module and reset SystemModel accordingly
//!
//! @param doSynchronousReset When true, reset will be done by issuing a synchronous reset sequence
//!
void mast::iReset (bool doSynchronousReset)
{
  auto manager = GetAndCheckManager();
  manager->iReset(doSynchronousReset);
}
//
//  End of: mast::iReset
//---------------------------------------------------------------------------




void mast::iWrite (string_view registerPath, uint8_t     value) { iWrite_impl(registerPath, value); }
void mast::iWrite (string_view registerPath, uint16_t    value) { iWrite_impl(registerPath, value); }
void mast::iWrite (string_view registerPath, uint32_t    value) { iWrite_impl(registerPath, value); }
void mast::iWrite (string_view registerPath, uint64_t    value) { iWrite_impl(registerPath, value); }
void mast::iWrite (string_view registerPath, int8_t      value) { iWrite_impl(registerPath, value); }
void mast::iWrite (string_view registerPath, int16_t     value) { iWrite_impl(registerPath, value); }
void mast::iWrite (string_view registerPath, int32_t     value) { iWrite_impl(registerPath, value); }
void mast::iWrite (string_view registerPath, int64_t     value) { iWrite_impl(registerPath, value); }
void mast::iWrite (string_view registerPath, string_view value) { iWrite_impl(registerPath, BinaryVector::CreateFromString(value)); }

//! iScan is like an iWrite+iRead, but can only be executed on a BlackBox
void mast::iScan (string_view registerPath, string_view value) { iScan_impl(registerPath, BinaryVector::CreateFromString(value)); }
void mast::iScan (string_view registerPath, string_view value,string_view expectedValue) { iScan_impl(registerPath, BinaryVector::CreateFromString(value), BinaryVector::CreateFromString(expectedValue)); }

//===========================================================================
// End of PDL_Adapter_CPP.cpp
//===========================================================================
