//===========================================================================
//                           UT_PDL_Adapter_CPP.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_PDL_Adapter_CPP.cpp
//!
//! Implements test fixture for testing PDL_Adapter_CPP
//!
//===========================================================================

#include "UT_PDL_Adapter_CPP.hpp"
#include "BinaryVector.hpp"
#include "PDL_Adapter_CPP.hpp"
#include "Startup.hpp"
#include "Session.hpp"
#include "SystemModel.hpp"
#include "TestModelBuilder.hpp"
#include "GmlPrinterVisitor.hpp"

//+#include "SystemModelAdapter_C.hpp"
#include "BinaryVector_Traits.hpp"

#include <algorithm>
#include <string>
#include <experimental/string_view>

using std::string;
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



//! Checks SystemModelManager::iGet() when Mast library is not yet initialized
//!
template<typename T> void Check_iGet_NotInitialized ()
{
  // ---------------- Setup
  //
  T readData;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (iGet("", readData), std::exception);
}


//! Checks SystemModelManager::iGet_xxx() when path is empty
//!
template<typename T> void Check_iGet_Empty_Path ()
{
  // ---------------- Setup
  //
  Session     session;
  string_view emptyPath = "";
  T           readData;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (iGet(emptyPath, readData), std::exception);
}


//! Checks SystemModelManager::iGet()
//!
template<typename T> void Check_iGet (T expectedData)
{
  // ---------------- Setup
  //
  Session session;
  Create_TestCase_MIB_Multichain_Pre();
  T readData;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (iGet("TAP_DR_Mux.MIB_mux.dynamic_1", readData));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (readData, expectedData);
}


//! Checks SystemModelManager::iWrite_xxx() using same thread as SystemModelManager
//!
template<typename T> void Check_iWrite_NotInitialized ()
{
  // ---------------- Setup
  //
  auto value = T();

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (iWrite("", value), std::exception);
}


//! Checks SystemModelManager::iWrite_xxx() using same thread as SystemModelManager
//!
template<typename T> void Check_iWrite_SingleThread (T value, string_view expected)
{
  // ---------------- Setup
  //
  Session session;

  Create_TestCase_MIB_Multichain_Pre();

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (iWrite("TAP_DR_Mux.MIB_mux.dynamic_1", value));

  // ---------------- Verify
  //
  TS_ASSERT_THROWS_NOTHING (iApply()); // iWrite does nothing visible without iApply

  auto reg               = Startup::GetSystemModel()->RegisterWithId(7u);
  auto expectedNextToSut = BinaryVector::CreateFromHexString(expected);
  TS_ASSERT_EQUALS (reg->NextToSut(), expectedNextToSut);
}
//
//  End of: Check_iWrite_SingleThread
//---------------------------------------------------------------------------

} // End of unnamed namespace



//! Initializes test (called for each test)
void UT_PDL_Adapter_CPP::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}



//! Checks SystemModelManager::iPrefix() when Mast library is not initialized
//!
void UT_PDL_Adapter_CPP::test_iPrefix_NotInitialized ()
{
  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (iPrefix("Reg"), std::exception);
}


void UT_PDL_Adapter_CPP::test_iGet_uint8_NotInitialized  () { Check_iGet_NotInitialized<uint8_t> (); }
void UT_PDL_Adapter_CPP::test_iGet_uint16_NotInitialized () { Check_iGet_NotInitialized<uint16_t>(); }
void UT_PDL_Adapter_CPP::test_iGet_uint32_NotInitialized () { Check_iGet_NotInitialized<uint32_t>(); }
void UT_PDL_Adapter_CPP::test_iGet_uint64_NotInitialized () { Check_iGet_NotInitialized<uint64_t>(); }
void UT_PDL_Adapter_CPP::test_iGet_int8_NotInitialized   () { Check_iGet_NotInitialized<int8_t>  (); }
void UT_PDL_Adapter_CPP::test_iGet_int16_NotInitialized  () { Check_iGet_NotInitialized<int16_t> (); }
void UT_PDL_Adapter_CPP::test_iGet_int32_NotInitialized  () { Check_iGet_NotInitialized<int32_t> (); }
void UT_PDL_Adapter_CPP::test_iGet_int64_NotInitialized  () { Check_iGet_NotInitialized<int64_t> (); }
void UT_PDL_Adapter_CPP::test_iGet_String_NotInitialized ()
{
  // ---------------- Setup
  //
  string readData;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (iGet("", readData, StringType::Binary), std::exception);
}


void UT_PDL_Adapter_CPP::test_iGet_uint8_Empty_Path  () { Check_iGet_Empty_Path<uint8_t> (); }
void UT_PDL_Adapter_CPP::test_iGet_uint16_Empty_Path () { Check_iGet_Empty_Path<uint16_t>(); }
void UT_PDL_Adapter_CPP::test_iGet_uint32_Empty_Path () { Check_iGet_Empty_Path<uint32_t>(); }
void UT_PDL_Adapter_CPP::test_iGet_uint64_Empty_Path () { Check_iGet_Empty_Path<uint64_t>(); }
void UT_PDL_Adapter_CPP::test_iGet_int8_Empty_Path   () { Check_iGet_Empty_Path<int8_t>  (); }
void UT_PDL_Adapter_CPP::test_iGet_int16_Empty_Path  () { Check_iGet_Empty_Path<int16_t> (); }
void UT_PDL_Adapter_CPP::test_iGet_int32_Empty_Path  () { Check_iGet_Empty_Path<int32_t> (); }
void UT_PDL_Adapter_CPP::test_iGet_int64_Empty_Path  () { Check_iGet_Empty_Path<int64_t> (); }
void UT_PDL_Adapter_CPP::test_iGet_String_Empty_Path ()
{
  // ---------------- Setup
  //
  Session session;
  string readData;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (iGet("", readData, StringType::Binary), std::exception);
}


//! Checks PDL_Adapter_CPP::iGet_String() in hexadecimal format
//!
void UT_PDL_Adapter_CPP::test_iGet_String_Hex ()
{
  // ---------------- Setup
  //
  Session session;
  Create_TestCase_MIB_Multichain_Pre();

  string readData;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (iGet("TAP_DR_Mux.MIB_mux.dynamic_1", readData, StringType::Hex));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (readData, "51515151");
}


//! Checks PDL_Adapter_CPP::iGet_String() in binary format
//!
void UT_PDL_Adapter_CPP::test_iGet_String_Bin ()
{
  Session session;
  Create_TestCase_MIB_Multichain_Pre();

  string readData;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (iGet("TAP_DR_Mux.MIB_mux.dynamic_1", readData, StringType::Binary));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (readData, "01010001010100010101000101010001");
}


void UT_PDL_Adapter_CPP::test_iGet_uint8  () { Check_iGet<uint8_t>  (0x51);       }
void UT_PDL_Adapter_CPP::test_iGet_uint16 () { Check_iGet<uint16_t> (0x5151);     }
void UT_PDL_Adapter_CPP::test_iGet_uint32 () { Check_iGet<uint32_t> (0x51515151); }
void UT_PDL_Adapter_CPP::test_iGet_uint64 () { Check_iGet<uint64_t> (0x51515151); }
void UT_PDL_Adapter_CPP::test_iGet_int8   () { Check_iGet<int8_t>   (0x51);       }
void UT_PDL_Adapter_CPP::test_iGet_int16  () { Check_iGet<int16_t>  (0x5151);     }
void UT_PDL_Adapter_CPP::test_iGet_int32  () { Check_iGet<int32_t>  (0x51515151); }
void UT_PDL_Adapter_CPP::test_iGet_int64  () { Check_iGet<int64_t>  (0x51515151); }


//! Checks PDL_Adapter_CPP::iWrite() when MAST library has not been initialized yet
//!
void UT_PDL_Adapter_CPP::test_iWrite_BinaryVector_NotInitialized () { Check_iWrite_NotInitialized<string_view>(); }
void UT_PDL_Adapter_CPP::test_iWrite_uint8_NotInitialized        () { Check_iWrite_NotInitialized<uint8_t>    (); }
void UT_PDL_Adapter_CPP::test_iWrite_uint16_NotInitialized       () { Check_iWrite_NotInitialized<uint16_t>   (); }
void UT_PDL_Adapter_CPP::test_iWrite_uint32_NotInitialized       () { Check_iWrite_NotInitialized<uint32_t>   (); }
void UT_PDL_Adapter_CPP::test_iWrite_uint64_NotInitialized       () { Check_iWrite_NotInitialized<uint64_t>   (); }
void UT_PDL_Adapter_CPP::test_iWrite_int8_NotInitialized         () { Check_iWrite_NotInitialized<int8_t>     (); }
void UT_PDL_Adapter_CPP::test_iWrite_int16_NotInitialized        () { Check_iWrite_NotInitialized<int16_t>    (); }
void UT_PDL_Adapter_CPP::test_iWrite_int32_NotInitialized        () { Check_iWrite_NotInitialized<int32_t>    (); }
void UT_PDL_Adapter_CPP::test_iWrite_int64_NotInitialized        () { Check_iWrite_NotInitialized<int64_t>    (); }


//! Checks PDL_Adapter_CPP::iWrite()
//!
void UT_PDL_Adapter_CPP::test_iWrite_BinaryVector () { Check_iWrite_SingleThread<string_view> ("0b1010:1011_1100:1101|0100:0101_0110:0111", "ABCD_4567"); }

void UT_PDL_Adapter_CPP::test_iWrite_uint8  () { Check_iWrite_SingleThread<uint8_t>  (uint8_t(123),          "0000_007B"); }
void UT_PDL_Adapter_CPP::test_iWrite_uint16 () { Check_iWrite_SingleThread<uint16_t> (uint16_t(12345),       "0000_3039"); }
void UT_PDL_Adapter_CPP::test_iWrite_uint32 () { Check_iWrite_SingleThread<uint32_t> (uint32_t(1234567L),    "0012_D687"); }
void UT_PDL_Adapter_CPP::test_iWrite_uint64 () { Check_iWrite_SingleThread<uint64_t> (uint64_t(123456789LL), "075B_CD15"); }
void UT_PDL_Adapter_CPP::test_iWrite_int8   () { Check_iWrite_SingleThread<int8_t>   (int8_t(-123),          "FFFF_FF85"); }
void UT_PDL_Adapter_CPP::test_iWrite_int16  () { Check_iWrite_SingleThread<int16_t>  (int16_t(-12345),       "FFFF_CFC7"); }
void UT_PDL_Adapter_CPP::test_iWrite_int32  () { Check_iWrite_SingleThread<int32_t>  (int32_t(-1234567L),    "FFED_2979"); }
void UT_PDL_Adapter_CPP::test_iWrite_int64  () { Check_iWrite_SingleThread<int64_t>  (int64_t(-123456789LL), "F8A4_32EB"); }


//+void UT_PDL_Adapter_CPP::test_iWrite_BinaryVector_InvalidValue () { Check_iWrite_SingleThread<const char*>(iWrite_BinaryVector, "ABCD_4567",           "ABCD_4567"); }

//===========================================================================
// End of UT_PDL_Adapter_CPP.cpp
//===========================================================================
