//===========================================================================
//                           UT_PDL_Adapter_C.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_PDL_Adapter_C.cpp
//!
//! Implements test fixture for testing PDL_Adapter_C
//!
//===========================================================================

#include "UT_PDL_Adapter_C.hpp"
#include "PDL_Adapter_C.h"
#include "BinaryVector.hpp"
#include "Startup.hpp"
#include "SystemModel.hpp"
#include "TestModelBuilder.hpp"
#include "GmlPrinterVisitor.hpp"

#include "C_API_Traits.hpp"
#include "BinaryVector_Traits.hpp"

#include <algorithm>
#include <experimental/string_view>

using std::experimental::string_view;

using namespace mast;
using namespace test;

namespace
{
  //! Creates a test case for testing 'C' interface
  //!
  //! @note Mast library must be initialized prior to calling this functin
  //!
  void Create_TestCase_MIB_Multichain_Pre (bool reportGml = false)
  {
    auto sm = Startup::GetSystemModel();

    TestModelBuilder builder(*sm);

    auto tap = builder.Create_TestCase_MIB_Multichain_Pre("TAP", 4u);

    auto regDyn_0  = sm->RegisterWithId(6u);
    auto regDyn_1  = sm->RegisterWithId(7u);
    auto regDyn_2  = sm->RegisterWithId(8u);
    auto regDyn_3  = sm->RegisterWithId(9u);

    regDyn_0->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x60));
    regDyn_1->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x61));
    regDyn_2->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x62));
    regDyn_3->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x63));

    regDyn_0->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x40));
    regDyn_1->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x41));
    regDyn_2->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x42));
    regDyn_3->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x43));

    regDyn_0->SetPendingForRead(); regDyn_0->SetFromSut  (BinaryVector(DYNAMIC_TDR_LEN, 0x50));
    regDyn_1->SetPendingForRead(); regDyn_1->SetFromSut  (BinaryVector(DYNAMIC_TDR_LEN, 0x51));
    regDyn_2->SetPendingForRead(); regDyn_2->SetFromSut  (BinaryVector(DYNAMIC_TDR_LEN, 0x52));
    regDyn_3->SetPendingForRead(); regDyn_3->SetFromSut  (BinaryVector(DYNAMIC_TDR_LEN, 0x53));

    if (reportGml)
    {
      TS_TRACE (GmlPrinterVisitor::Graph(tap, "MIB_Multichain_Pre"));
    }
  }
  //
  //  End of: Create_TestCase_MIB_Multichain_Pre
  //---------------------------------------------------------------------------


  template<typename T> using iWrite_functor = std::function<ErrorCode(const char*, T)>;
  template<typename T> using iGet_functor   = std::function<ErrorCode(const char*, T*)>;

  template <typename T> struct Traits
  {
    static T  Default () { return 0; }
//+    static T* Pointer () { return &value; }

//+    static T value;
  };

//+  template<> uint8_t  Traits<uint8_t>::value  = 0;
//+  template<> uint16_t Traits<uint16_t>::value = 0;
//+  template<> uint32_t Traits<uint32_t>::value = 0;
//+  template<> uint64_t Traits<uint64_t>::value = 0;
//+  template<> int8_t   Traits<int8_t>::value   = 0;
//+  template<> int16_t  Traits<int16_t>::value  = 0;
//+  template<> int32_t  Traits<int32_t>::value  = 0;
//+  template<> int64_t  Traits<int64_t>::value  = 0;


  template<> struct Traits<const char*>
  {
    static const char* Default()  { return ""; }
//+    static char*       Pointer () { return String; }

//+    static char String[200];
  };


  //! Checks SystemModelManager::iGet_xxx() when Mast library is not yet initialized
  //!
  template<typename T> void Check_iGet_NotInitialized (iGet_functor<T> fct)
  {
    static_assert(std::is_integral<T>::value, "This version only supports integral types");

    // ---------------- Setup
    //
    auto retCode = ErrorCode::Ok;

    // ---------------- Exercise
    //
    T readData;
    TS_ASSERT_THROWS_NOTHING (retCode = fct("", &readData));

    // ---------------- Verify
    //
    auto errorMsg = ErrorMessage();

    TS_ASSERT_EQUALS  (retCode,  ErrorCode::RuntimeError);
    TS_ASSERT_DIFFERS (errorMsg, "");
  }


  //! Checks SystemModelManager::iGet_xxx() when register path is nullptr
  //!
  template<typename T> void Check_iGet_Nullptr_Path (iGet_functor<T> fct)
  {
    static_assert(std::is_integral<T>::value, "This version only supports integral types");

    // ---------------- Setup
    //
    InitializeMast(nullptr);
    auto retCode = ErrorCode::Ok;

    // ---------------- Exercise
    //
    T readData;
    TS_ASSERT_THROWS_NOTHING (retCode = fct(nullptr, &readData));

    // ---------------- Verify
    //
    auto errorMsg = ErrorMessage();

    TS_ASSERT_EQUALS  (retCode,  ErrorCode::InvalidArgument);
    TS_ASSERT_DIFFERS (errorMsg, "");
  }


  //! Checks SystemModelManager::iGet_xxx()
  //!
  template<typename T> void Check_iGet (iGet_functor<T> fct, T expectedData)
  {
    static_assert(std::is_integral<T>::value, "This version only supports integral types");

    // ---------------- Setup
    //
    InitializeMast(nullptr);
    Create_TestCase_MIB_Multichain_Pre();

    auto retCode = ErrorCode::Ok;

    // ---------------- Exercise
    //
    T readData;
    TS_ASSERT_THROWS_NOTHING (retCode = fct("TAP_DR_Mux.MIB_mux.dynamic_1", &readData));

    // ---------------- Verify
    //
    auto errorMsg = ErrorMessage();

    TS_ASSERT_EQUALS (retCode,  ErrorCode::Ok);
    TS_ASSERT_EQUALS (errorMsg, "");
    TS_ASSERT_EQUALS (readData, expectedData);
  }



  //! Checks SystemModelManager::iGet_xxx() when register path is nullptr
  //!
  template<typename T> void Check_iGet_Nullptr_ReadData (iGet_functor<T> fct)
  {
    static_assert(std::is_integral<T>::value, "This version only supports integral types");

    // ---------------- Setup
    //
    InitializeMast(nullptr);
    auto retCode = ErrorCode::Ok;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (retCode = fct("", nullptr));

    // ---------------- Verify
    //
    auto errorMsg = ErrorMessage();

    TS_ASSERT_EQUALS  (retCode,  ErrorCode::InvalidArgument);
    TS_ASSERT_DIFFERS (errorMsg, "");
  }

  //! Checks SystemModelManager::iWrite_xxx() using same thread as SystemModelManager
  //!
  template<typename T> void Check_iWrite_NotInitialized (iWrite_functor<T> fct)
  {
    // ---------------- Setup
    //
    auto retCode = ErrorCode::Ok;

    // ---------------- Exercise
    //
    auto value = Traits<T>::Default();
    TS_ASSERT_THROWS_NOTHING (retCode = fct("", value));

    // ---------------- Verify
    //
    auto errorMsg = ErrorMessage();

    TS_ASSERT_EQUALS  (retCode,  ErrorCode::RuntimeError);
    TS_ASSERT_DIFFERS (errorMsg, "");
  }


  //! Checks SystemModelManager::iWrite_xxx() using same thread as SystemModelManager
  //!
  template<typename T> void Check_iWrite_SingleThread (std::function<ErrorCode(const char*, T)> fct,
                                                       T           value,
                                                       string_view expected)
  {
    // ---------------- Setup
    //
    InitializeMast(nullptr);

    Create_TestCase_MIB_Multichain_Pre();

    auto retCode = ErrorCode::Ok;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (retCode = fct("TAP_DR_Mux.MIB_mux.dynamic_1", value));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS         (retCode, ErrorCode::Ok);
    TS_ASSERT_THROWS_NOTHING (retCode  = iApply()); // iWrite does nothing visible without iApply
    TS_ASSERT_EQUALS         (retCode, ErrorCode::Ok);

    auto errorMsg = ErrorMessage();
    TS_ASSERT_EQUALS (errorMsg, "");

    auto reg               = Startup::GetSystemModel()->RegisterWithId(7u);
    auto expectedNextToSut = BinaryVector::CreateFromHexString(expected);
    TS_ASSERT_EQUALS (reg->NextToSut(), expectedNextToSut);
  }
  //
  //  End of: Check_iWrite_SingleThread
  //---------------------------------------------------------------------------

} // End of unnamed namespace

//! Initializes test (called for each test)
void UT_PDL_Adapter_C::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  CleanupMast();
  SystemModelNode::ResetNodeIdentifier();
}

//! Checks SystemModelManager::iPrefix() when Mast library is not initialized
//!
void UT_PDL_Adapter_C::test_iPrefix_NotInitialized ()
{
  // ---------------- Setup
  //
  auto retCode = ErrorCode::Ok;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (retCode = iPrefix("Reg"));

  // ---------------- Verify
  //
  auto errorMsg = ErrorMessage();

  TS_ASSERT_EQUALS  (retCode,  ErrorCode::RuntimeError);
  TS_ASSERT_DIFFERS (errorMsg, "");
}


//! Checks PDL_Adapter::iGet_String() when MAST library has not been initialized yet
//!
void UT_PDL_Adapter_C::test_iGet_String_NotInitialized ()
{
  // ---------------- Setup
  //
  auto retCode            = ErrorCode::Ok;
  auto readDataBuffer     = std::array<char, 50>();
  auto readDataBufferSize = readDataBuffer.size();

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (retCode = iGet_String("", readDataBuffer.data(), &readDataBufferSize, Hex));

  // ---------------- Verify
  //
  auto errorMsg = ErrorMessage();

  TS_ASSERT_EQUALS  (retCode,  ErrorCode::RuntimeError);
  TS_ASSERT_DIFFERS (errorMsg, "");
}

void UT_PDL_Adapter_C::test_iGet_uint8_NotInitialized        () { Check_iGet_NotInitialized<uint8_t>(iGet_uint8_t);          }
void UT_PDL_Adapter_C::test_iGet_uint16_NotInitialized       () { Check_iGet_NotInitialized<uint16_t>(iGet_uint16_t);        }
void UT_PDL_Adapter_C::test_iGet_uint32_NotInitialized       () { Check_iGet_NotInitialized<uint32_t>(iGet_uint32_t);        }
void UT_PDL_Adapter_C::test_iGet_uint64_NotInitialized       () { Check_iGet_NotInitialized<uint64_t>(iGet_uint64_t);        }
void UT_PDL_Adapter_C::test_iGet_int8_NotInitialized         () { Check_iGet_NotInitialized<int8_t>(iGet_int8_t);            }
void UT_PDL_Adapter_C::test_iGet_int16_NotInitialized        () { Check_iGet_NotInitialized<int16_t>(iGet_int16_t);          }
void UT_PDL_Adapter_C::test_iGet_int32_NotInitialized        () { Check_iGet_NotInitialized<int32_t>(iGet_int32_t);          }
void UT_PDL_Adapter_C::test_iGet_int64_NotInitialized        () { Check_iGet_NotInitialized<int64_t>(iGet_int64_t);          }


//! Checks PDL_Adapter::iGet_String() when register path is nullptr
//!
void UT_PDL_Adapter_C::test_iGet_String_Nullptr_Path ()
{
  // ---------------- Setup
  //
  InitializeMast(nullptr);

  auto retCode            = ErrorCode::Ok;
  auto readDataBuffer     = std::array<char, 50>();
  auto readDataBufferSize = readDataBuffer.size();

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (retCode = iGet_String(nullptr, readDataBuffer.data(), &readDataBufferSize, Hex));

  // ---------------- Verify
  //
  auto errorMsg = ErrorMessage();

  TS_ASSERT_EQUALS  (retCode,  ErrorCode::InvalidArgument);
  TS_ASSERT_DIFFERS (errorMsg, "");
}

void UT_PDL_Adapter_C::test_iGet_uint8_Nullptr_Path        () { Check_iGet_Nullptr_Path<uint8_t>(iGet_uint8_t);        }
void UT_PDL_Adapter_C::test_iGet_uint16_Nullptr_Path       () { Check_iGet_Nullptr_Path<uint16_t>(iGet_uint16_t);        }
void UT_PDL_Adapter_C::test_iGet_uint32_Nullptr_Path       () { Check_iGet_Nullptr_Path<uint32_t>(iGet_uint32_t);        }
void UT_PDL_Adapter_C::test_iGet_uint64_Nullptr_Path       () { Check_iGet_Nullptr_Path<uint64_t>(iGet_uint64_t);        }
void UT_PDL_Adapter_C::test_iGet_int8_Nullptr_Path         () { Check_iGet_Nullptr_Path<int8_t>(iGet_int8_t);            }
void UT_PDL_Adapter_C::test_iGet_int16_Nullptr_Path        () { Check_iGet_Nullptr_Path<int16_t>(iGet_int16_t);          }
void UT_PDL_Adapter_C::test_iGet_int32_Nullptr_Path        () { Check_iGet_Nullptr_Path<int32_t>(iGet_int32_t);          }
void UT_PDL_Adapter_C::test_iGet_int64_Nullptr_Path        () { Check_iGet_Nullptr_Path<int64_t>(iGet_int64_t);          }


//! Checks PDL_Adapter::iGet_String() when register path is nullptr
//!
void UT_PDL_Adapter_C::test_iGet_String_Nullptr_ReadData ()
{
  // ---------------- Setup
  //
  InitializeMast(nullptr);

  auto retCode            = ErrorCode::Ok;
  auto readDataBufferSize = size_t(0u);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (retCode = iGet_String("", nullptr, &readDataBufferSize, Hex));

  // ---------------- Verify
  //
  auto errorMsg = ErrorMessage();

  TS_ASSERT_EQUALS  (retCode,  ErrorCode::InvalidArgument);
  TS_ASSERT_DIFFERS (errorMsg, "");
}

void UT_PDL_Adapter_C::test_iGet_uint8_Nullptr_ReadData        () { Check_iGet_Nullptr_ReadData<uint8_t>(iGet_uint8_t);        }
void UT_PDL_Adapter_C::test_iGet_uint16_Nullptr_ReadData       () { Check_iGet_Nullptr_ReadData<uint16_t>(iGet_uint16_t);        }
void UT_PDL_Adapter_C::test_iGet_uint32_Nullptr_ReadData       () { Check_iGet_Nullptr_ReadData<uint32_t>(iGet_uint32_t);        }
void UT_PDL_Adapter_C::test_iGet_uint64_Nullptr_ReadData       () { Check_iGet_Nullptr_ReadData<uint64_t>(iGet_uint64_t);        }
void UT_PDL_Adapter_C::test_iGet_int8_Nullptr_ReadData         () { Check_iGet_Nullptr_ReadData<int8_t>(iGet_int8_t);            }
void UT_PDL_Adapter_C::test_iGet_int16_Nullptr_ReadData        () { Check_iGet_Nullptr_ReadData<int16_t>(iGet_int16_t);          }
void UT_PDL_Adapter_C::test_iGet_int32_Nullptr_ReadData        () { Check_iGet_Nullptr_ReadData<int32_t>(iGet_int32_t);          }
void UT_PDL_Adapter_C::test_iGet_int64_Nullptr_ReadData        () { Check_iGet_Nullptr_ReadData<int64_t>(iGet_int64_t);          }

//! Checks PDL_Adapter::iGet_String() in hexadecimal format
//!
void UT_PDL_Adapter_C::test_iGet_String_Hex ()
{
  // ---------------- Setup
  //
  InitializeMast(nullptr);
  Create_TestCase_MIB_Multichain_Pre();

  auto retCode            = ErrorCode::Ok;
  auto readDataBuffer     = std::array<char, 50>();
  auto readDataBufferSize = readDataBuffer.size();

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (retCode = iGet_String("TAP_DR_Mux.MIB_mux.dynamic_1", readDataBuffer.data(), &readDataBufferSize, Hex));

  // ---------------- Verify
  //
  auto errorMsg = ErrorMessage();

  TS_ASSERT_EQUALS (retCode,               ErrorCode::Ok);
  TS_ASSERT_EQUALS (errorMsg,              "");
  TS_ASSERT_EQUALS (readDataBufferSize,    8u);
  TS_ASSERT_EQUALS (readDataBuffer.data(), "51515151");
}


//! Checks PDL_Adapter::iGet_String() in binary format
//!
void UT_PDL_Adapter_C::test_iGet_String_Bin ()
{
  // ---------------- Setup
  //
  InitializeMast(nullptr);
  Create_TestCase_MIB_Multichain_Pre();

  auto retCode            = ErrorCode::Ok;
  auto readDataBuffer     = std::array<char, 50>();
  auto readDataBufferSize = readDataBuffer.size();

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (retCode = iGet_String("TAP_DR_Mux.MIB_mux.dynamic_1", readDataBuffer.data(), &readDataBufferSize, Binary));

  // ---------------- Verify
  //
  auto errorMsg = ErrorMessage();

  TS_ASSERT_EQUALS (retCode,               ErrorCode::Ok);
  TS_ASSERT_EQUALS (errorMsg,              "");
  TS_ASSERT_EQUALS (readDataBufferSize,    32u);
  TS_ASSERT_EQUALS (readDataBuffer.data(), "01010001010100010101000101010001");
}


void UT_PDL_Adapter_C::test_iGet_uint8  () { Check_iGet<uint8_t>  (iGet_uint8_t,  0x51);       }
void UT_PDL_Adapter_C::test_iGet_uint16 () { Check_iGet<uint16_t> (iGet_uint16_t, 0x5151);     }
void UT_PDL_Adapter_C::test_iGet_uint32 () { Check_iGet<uint32_t> (iGet_uint32_t, 0x51515151); }
void UT_PDL_Adapter_C::test_iGet_uint64 () { Check_iGet<uint64_t> (iGet_uint64_t, 0x51515151); }
void UT_PDL_Adapter_C::test_iGet_int8   () { Check_iGet<int8_t>   (iGet_int8_t,   0x51);       }
void UT_PDL_Adapter_C::test_iGet_int16  () { Check_iGet<int16_t>  (iGet_int16_t,  0x5151);     }
void UT_PDL_Adapter_C::test_iGet_int32  () { Check_iGet<int32_t>  (iGet_int32_t,  0x51515151); }
void UT_PDL_Adapter_C::test_iGet_int64  () { Check_iGet<int64_t>  (iGet_int64_t,  0x51515151); }


//! Checks PDL_Adapter::iWrite_uintxx_t() when MAST library has not been initialized yet
//!
void UT_PDL_Adapter_C::test_iWrite_BinaryVector_NotInitialized () { Check_iWrite_NotInitialized<const char*>(iWrite_BinaryVector); }
void UT_PDL_Adapter_C::test_iWrite_uint8_NotInitialized        () { Check_iWrite_NotInitialized<uint8_t>(iWrite_uint8_t);          }
void UT_PDL_Adapter_C::test_iWrite_uint16_NotInitialized       () { Check_iWrite_NotInitialized<uint16_t>(iWrite_uint16_t);        }
void UT_PDL_Adapter_C::test_iWrite_uint32_NotInitialized       () { Check_iWrite_NotInitialized<uint32_t>(iWrite_uint32_t);        }
void UT_PDL_Adapter_C::test_iWrite_uint64_NotInitialized       () { Check_iWrite_NotInitialized<uint64_t>(iWrite_uint64_t);        }
void UT_PDL_Adapter_C::test_iWrite_int8_NotInitialized         () { Check_iWrite_NotInitialized<int8_t>(iWrite_int8_t);            }
void UT_PDL_Adapter_C::test_iWrite_int16_NotInitialized        () { Check_iWrite_NotInitialized<int16_t>(iWrite_int16_t);          }
void UT_PDL_Adapter_C::test_iWrite_int32_NotInitialized        () { Check_iWrite_NotInitialized<int32_t>(iWrite_int32_t);          }
void UT_PDL_Adapter_C::test_iWrite_int64_NotInitialized        () { Check_iWrite_NotInitialized<int64_t>(iWrite_int64_t);          }


//! Checks PDL_Adapter::iWrite_uintxx_t()
//!
void UT_PDL_Adapter_C::test_iWrite_BinaryVector () { Check_iWrite_SingleThread<const char* > (iWrite_BinaryVector, "1010:1011_1100:1101|0100:0101_0110:0111", "ABCD_4567"); }

void UT_PDL_Adapter_C::test_iWrite_uint8        () { Check_iWrite_SingleThread<uint8_t>(iWrite_uint8_t,   uint8_t(123),          "0000_007B"); }
void UT_PDL_Adapter_C::test_iWrite_uint16       () { Check_iWrite_SingleThread<uint16_t>(iWrite_uint16_t, uint16_t(12345),       "0000_3039"); }
void UT_PDL_Adapter_C::test_iWrite_uint32       () { Check_iWrite_SingleThread<uint32_t>(iWrite_uint32_t, uint32_t(1234567L),    "0012_D687"); }
void UT_PDL_Adapter_C::test_iWrite_uint64       () { Check_iWrite_SingleThread<uint64_t>(iWrite_uint64_t, uint64_t(123456789LL), "075B_CD15"); }
void UT_PDL_Adapter_C::test_iWrite_int8         () { Check_iWrite_SingleThread<int8_t>(iWrite_int8_t,     int8_t(-123),          "FFFF_FF85"); }
void UT_PDL_Adapter_C::test_iWrite_int16        () { Check_iWrite_SingleThread<int16_t>(iWrite_int16_t,   int16_t(-12345),       "FFFF_CFC7"); }
void UT_PDL_Adapter_C::test_iWrite_int32        () { Check_iWrite_SingleThread<int32_t>(iWrite_int32_t,   int32_t(-1234567L),    "FFED_2979"); }
void UT_PDL_Adapter_C::test_iWrite_int64        () { Check_iWrite_SingleThread<int64_t>(iWrite_int64_t,   int64_t(-123456789LL), "F8A4_32EB"); }


//+void UT_PDL_Adapter_C::test_iWrite_BinaryVector_InvalidValue () { Check_iWrite_SingleThread<const char*>(iWrite_BinaryVector, "ABCD_4567",           "ABCD_4567"); }

//===========================================================================
// End of UT_PDL_Adapter_C.cpp
//===========================================================================
