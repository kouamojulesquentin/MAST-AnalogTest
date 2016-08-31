//===========================================================================
//                           PDL_Adapter_CPP.h
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PDL_Adapter.h
//!
//! Declares C++ language API to PDL
//!
//===========================================================================


#ifndef PDL_ADAPTER_CPP_H__287AC6DE_2B0_4C1D_90A1_4BF822B1DC85__INCLUDED_
  #define PDL_ADAPTER_CPP_H__287AC6DE_2B0_4C1D_90A1_4BF822B1DC85__INCLUDED_

#include "Platform.hpp"
#include <experimental/string_view>


namespace mast
{
  //! Defines string representation of register values
  //!
  enum class StringType
  {
    Binary, //!< Binary format with prepended "0b"
    Hex,    //!< Hexadecimal format with prepended "0x"
    Decimal //!< Decimal format
  };

  // ---------------- iApply
  //
  DLL_EXPORT void iApply (); //!< Waits for all queued operations to be executed

  // ---------------- iGet
  //
  DLL_EXPORT void        iGet (std::experimental::string_view registerPath, uint8_t&     readData);                        //!< Returns last Register value read from SUT as uint8_t
  DLL_EXPORT void        iGet (std::experimental::string_view registerPath, uint16_t&    readData);                        //!< Returns last Register value read from SUT as uint16_t
  DLL_EXPORT void        iGet (std::experimental::string_view registerPath, uint32_t&    readData);                        //!< Returns last Register value read from SUT as uint32_t
  DLL_EXPORT void        iGet (std::experimental::string_view registerPath, uint64_t&    readData);                        //!< Returns last Register value read from SUT as uint64_t
  DLL_EXPORT void        iGet (std::experimental::string_view registerPath, int8_t&      readData);                        //!< Returns last Register value read from SUT as int8_t
  DLL_EXPORT void        iGet (std::experimental::string_view registerPath, int16_t&     readData);                        //!< Returns last Register value read from SUT as int16_t
  DLL_EXPORT void        iGet (std::experimental::string_view registerPath, int32_t&     readData);                        //!< Returns last Register value read from SUT as int32_t
  DLL_EXPORT void        iGet (std::experimental::string_view registerPath, int64_t&     readData);                        //!< Returns last Register value read from SUT as int64_t
  DLL_EXPORT void        iGet (std::experimental::string_view registerPath, std::string& readData, StringType stringType); //!< Returns last Register value read from SUT as string
  DLL_EXPORT std::string iGet (std::experimental::string_view registerPath, StringType   stringType);                      //!< Returns last Register value read from SUT as string

  //! Returns last Register value read from SUT as binary string
  //! This syntactic sugar can be used like:
  //! @code {.cpp}
  //!   auto var = iGet<int32_t>("Foo.Bar");
  //! @endcode
  template<typename T> T iGet (std::experimental::string_view registerPath)
  {
    T readData;
    iGet(registerPath, readData);
    return readData;
  }

  // ---------------- iGetMiscompares
  //
  DLL_EXPORT std::string iGetMiscompares (std::experimental::string_view registerPath, StringType   stringType);                       //!< Returns XOR of the last value read from SUT and the expected value
  DLL_EXPORT void        iGetMiscompares (std::experimental::string_view registerPath, std::string& xorResult, StringType stringType); //!< Returns XOR of the last value read from SUT and the expected value

  // ---------------- iGetRefresh
  //
  DLL_EXPORT void        iGetRefresh (std::experimental::string_view registerPath, uint8_t&     readData);                        //!< Requests register value to be read from SUT and wait till it can be return as uint8_t
  DLL_EXPORT void        iGetRefresh (std::experimental::string_view registerPath, uint16_t&    readData);                        //!< Requests register value to be read from SUT and wait till it can be return as uint16_t
  DLL_EXPORT void        iGetRefresh (std::experimental::string_view registerPath, uint32_t&    readData);                        //!< Requests register value to be read from SUT and wait till it can be return as uint32_t
  DLL_EXPORT void        iGetRefresh (std::experimental::string_view registerPath, uint64_t&    readData);                        //!< Requests register value to be read from SUT and wait till it can be return as uint64_t
  DLL_EXPORT void        iGetRefresh (std::experimental::string_view registerPath, int8_t&      readData);                        //!< Requests register value to be read from SUT and wait till it can be return as int8_t
  DLL_EXPORT void        iGetRefresh (std::experimental::string_view registerPath, int16_t&     readData);                        //!< Requests register value to be read from SUT and wait till it can be return as int16_t
  DLL_EXPORT void        iGetRefresh (std::experimental::string_view registerPath, int32_t&     readData);                        //!< Requests register value to be read from SUT and wait till it can be return as int32_t
  DLL_EXPORT void        iGetRefresh (std::experimental::string_view registerPath, int64_t&     readData);                        //!< Requests register value to be read from SUT and wait till it can be return as int64_t
  DLL_EXPORT void        iGetRefresh (std::experimental::string_view registerPath, std::string& readData, StringType stringType); //!< Requests register value to be read from SUT and wait till it can be return (as string)
  DLL_EXPORT std::string iGetRefresh (std::experimental::string_view registerPath, StringType   stringType);                      //!< Requests register value to be read from SUT and wait till it can be return (as string)

  //! Requests register value to be read from SUT and wait till it can be return
  //! This syntactic sugar can be used like:
  //! @code {.cpp}
  //!   auto var = iGetRefresh<int32_t>("Foo.Bar");
  //! @endcode
  template<typename T> T iGetRefresh (std::experimental::string_view registerPath)
  {
    T readData;
    iGetRefresh(registerPath, readData);
    return readData;
  }


  // ---------------- iGetStatus
  //
  DLL_EXPORT uint32_t iGetRegisterStatus  (std::experimental::string_view registerPath, bool clearCounter); //!< Returns the number of expected read failure for a single Register

  // ---------------- iPrefix
  //
  DLL_EXPORT void iPrefix (std::experimental::string_view registerPath); //!< Changes path prefix

  // ---------------- iRead
  //
  DLL_EXPORT void iRead (std::experimental::string_view registerPath, uint8_t     expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected uint8_t value
  DLL_EXPORT void iRead (std::experimental::string_view registerPath, uint16_t    expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected uint16_t value
  DLL_EXPORT void iRead (std::experimental::string_view registerPath, uint32_t    expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected uint32_t value
  DLL_EXPORT void iRead (std::experimental::string_view registerPath, uint64_t    expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected uint64_t value
  DLL_EXPORT void iRead (std::experimental::string_view registerPath, int8_t      expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected int8_t value
  DLL_EXPORT void iRead (std::experimental::string_view registerPath, int16_t     expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected int16_t value
  DLL_EXPORT void iRead (std::experimental::string_view registerPath, int32_t     expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected int32_t value
  DLL_EXPORT void iRead (std::experimental::string_view registerPath, int64_t     expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected int64_t value
  DLL_EXPORT void iRead (std::experimental::string_view registerPath, std::experimental::string_view expectedValue); //!< Queues a request to (re-)read register value from SUT giving an expected binary string value

  // ---------------- iRefresh
  //
  DLL_EXPORT void iRefresh (std::experimental::string_view registerPath); //!< Queues a request to (re-)read register value from SUT

  // ---------------- iWrite
  //
  DLL_EXPORT void iWrite (std::experimental::string_view registerPath, uint8_t     value); //!< Queues write request to register from uint8_t
  DLL_EXPORT void iWrite (std::experimental::string_view registerPath, uint16_t    value); //!< Queues write request to register from uint16_t
  DLL_EXPORT void iWrite (std::experimental::string_view registerPath, uint32_t    value); //!< Queues write request to register from uint32_t
  DLL_EXPORT void iWrite (std::experimental::string_view registerPath, uint64_t    value); //!< Queues write request to register from uint64_t
  DLL_EXPORT void iWrite (std::experimental::string_view registerPath, int8_t      value); //!< Queues write request to register from int8_t
  DLL_EXPORT void iWrite (std::experimental::string_view registerPath, int16_t     value); //!< Queues write request to register from int16_t
  DLL_EXPORT void iWrite (std::experimental::string_view registerPath, int32_t     value); //!< Queues write request to register from int32_t
  DLL_EXPORT void iWrite (std::experimental::string_view registerPath, int64_t     value); //!< Queues write request to register from int64_t
  DLL_EXPORT void iWrite (std::experimental::string_view registerPath, std::experimental::string_view value); //!< Queues write request to register from binary string
}

#endif  // not defined PDL_ADAPTER_CPP_H__287AC6DE_2B0_4C1D_90A1_4BF822B1DC85__INCLUDED_

//===========================================================================
// End of PDL_Adapter.h
//===========================================================================



