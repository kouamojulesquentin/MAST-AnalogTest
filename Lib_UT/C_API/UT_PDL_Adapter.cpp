//===========================================================================
//                           UT_PDL_Adapter.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_PDL_Adapter.cpp
//!
//! Implements test fixture for testing PDL_Adapter
//!
//===========================================================================

#include "UT_PDL_Adapter.hpp"
#include "PDL_Adapter.h"
#include "BinaryVector.hpp"

//+#include "SystemModelAdapter.hpp"
#include "C_API_Traits.hpp"
#include <algorithm>
#include <experimental/string_view>

using std::experimental::string_view;

using namespace mast;

namespace
{
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

//+    SystemModel sm;
//+    Create_TestCase_MIB_Multichain_Pre(sm);

//+    auto reg  = sm.RegisterWithId(7u);

//+    SystemModelManager sut(sm);

    //! @todo [JFC]-[July/25/2016]: In Check_iWrite_SingleThread(): Complete it to have a real test
    //!
    auto retCode = ErrorCode::Ok;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (retCode = fct("TAP_DR_Mux.MIB_mux.dynamic_1", value));

    // ---------------- Verify
    //
    auto expectedNextToSut = BinaryVector::CreateFromHexString(expected);
    auto errorMsg = ErrorMessage();

    TS_ASSERT_EQUALS (retCode,  ErrorCode::Ok);
    TS_ASSERT_EQUALS (errorMsg, "");

//+    TS_ASSERT_EQUALS (reg->NextToSut(), expectedNextToSut);
  }
  //
  //  End of: Check_iWrite_SingleThread
  //---------------------------------------------------------------------------

} // End of unnamed namespace

//! Initializes test (called for each test)
void UT_PDL_Adapter::setUp ()
{
}

//! Checks SystemModelManager::iPrefix() when Mast library is not initialized
//!
void UT_PDL_Adapter::test_iPrefix_NotInitialized ()
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


//! Checks PDL_Adapter::iGet_uintxx_t() when MAST library has not been initialized yet
//!
//+void UT_PDL_Adapter::test_iGet_BinaryVector_NotInitialized () { Check_iGet_NotInitialized<const char*>(iGet_BinaryVector); }
void UT_PDL_Adapter::test_iGet_uint8_NotInitialized        () { Check_iGet_NotInitialized<uint8_t>(iGet_uint8_t);          }
void UT_PDL_Adapter::test_iGet_uint16_NotInitialized       () { Check_iGet_NotInitialized<uint16_t>(iGet_uint16_t);        }
void UT_PDL_Adapter::test_iGet_uint32_NotInitialized       () { Check_iGet_NotInitialized<uint32_t>(iGet_uint32_t);        }
void UT_PDL_Adapter::test_iGet_uint64_NotInitialized       () { Check_iGet_NotInitialized<uint64_t>(iGet_uint64_t);        }
void UT_PDL_Adapter::test_iGet_int8_NotInitialized         () { Check_iGet_NotInitialized<int8_t>(iGet_int8_t);            }
void UT_PDL_Adapter::test_iGet_int16_NotInitialized        () { Check_iGet_NotInitialized<int16_t>(iGet_int16_t);          }
void UT_PDL_Adapter::test_iGet_int32_NotInitialized        () { Check_iGet_NotInitialized<int32_t>(iGet_int32_t);          }
void UT_PDL_Adapter::test_iGet_int64_NotInitialized        () { Check_iGet_NotInitialized<int64_t>(iGet_int64_t);          }


//! Checks PDL_Adapter::iWrite_uintxx_t() when MAST library has not been initialized yet
//!
void UT_PDL_Adapter::test_iWrite_BinaryVector_NotInitialized () { Check_iWrite_NotInitialized<const char*>(iWrite_BinaryVector); }
void UT_PDL_Adapter::test_iWrite_uint8_NotInitialized        () { Check_iWrite_NotInitialized<uint8_t>(iWrite_uint8_t);          }
void UT_PDL_Adapter::test_iWrite_uint16_NotInitialized       () { Check_iWrite_NotInitialized<uint16_t>(iWrite_uint16_t);        }
void UT_PDL_Adapter::test_iWrite_uint32_NotInitialized       () { Check_iWrite_NotInitialized<uint32_t>(iWrite_uint32_t);        }
void UT_PDL_Adapter::test_iWrite_uint64_NotInitialized       () { Check_iWrite_NotInitialized<uint64_t>(iWrite_uint64_t);        }
void UT_PDL_Adapter::test_iWrite_int8_NotInitialized         () { Check_iWrite_NotInitialized<int8_t>(iWrite_int8_t);            }
void UT_PDL_Adapter::test_iWrite_int16_NotInitialized        () { Check_iWrite_NotInitialized<int16_t>(iWrite_int16_t);          }
void UT_PDL_Adapter::test_iWrite_int32_NotInitialized        () { Check_iWrite_NotInitialized<int32_t>(iWrite_int32_t);          }
void UT_PDL_Adapter::test_iWrite_int64_NotInitialized        () { Check_iWrite_NotInitialized<int64_t>(iWrite_int64_t);          }


//! Checks PDL_Adapter::iWrite_uintxx_t()
//!
//+void UT_PDL_Adapter::test_iWrite_BinaryVector () { Check_iWrite_SingleThread<const char* > (iWrite_BinaryVector, "1010:1011_1100:1011|0100:0101_0110:0111", "ABCD_4567"); }

//+void UT_PDL_Adapter::test_iWrite_uint8        () { Check_iWrite_SingleThread<uint8_t>(iWrite_uint8_t,   uint8_t(123),          "0000_007B"); }
//+void UT_PDL_Adapter::test_iWrite_uint16       () { Check_iWrite_SingleThread<uint16_t>(iWrite_uint16_t, uint16_t(12345),       "0000_3039"); }
//+void UT_PDL_Adapter::test_iWrite_uint32       () { Check_iWrite_SingleThread<uint32_t>(iWrite_uint32_t, uint32_t(1234567L),    "0012_D687"); }
//+void UT_PDL_Adapter::test_iWrite_uint64       () { Check_iWrite_SingleThread<uint64_t>(iWrite_uint64_t, uint64_t(123456789LL), "075B_CD15"); }
//+void UT_PDL_Adapter::test_iWrite_int8         () { Check_iWrite_SingleThread<int8_t>(iWrite_int8_t,     int8_t(-123),          "FFFF_FF85"); }
//+void UT_PDL_Adapter::test_iWrite_int16        () { Check_iWrite_SingleThread<int16_t>(iWrite_int16_t,   int16_t(-12345),       "FFFF_CFC7"); }
//+void UT_PDL_Adapter::test_iWrite_int32        () { Check_iWrite_SingleThread<int32_t>(iWrite_int32_t,   int32_t(-1234567L),    "FFED_2979"); }
//+void UT_PDL_Adapter::test_iWrite_int64        () { Check_iWrite_SingleThread<int64_t>(iWrite_int64_t,   int64_t(-123456789LL), "F8A4_32EB"); }


//+void UT_PDL_Adapter::test_iWrite_BinaryVector_InvalidValue () { Check_iWrite_SingleThread<const char*>(iWrite_BinaryVector, "ABCD_4567",           "ABCD_4567"); }

//===========================================================================
// End of UT_PDL_Adapter.cpp
//===========================================================================
