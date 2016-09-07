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

#include "BinaryVector_Traits.hpp"
#include "CPP_API_Traits.hpp"

#include <algorithm>
#include <string>
#include <experimental/string_view>
#include <tuple>

using std::tuple;
using std::make_tuple;
using std::string;
using std::experimental::string_view;

using namespace mast;
using namespace test;

namespace
{
//! Creates a test case for testing 'C' interface
//!
//! @note Mast library must be initialized prior to calling this function
//!
void Create_TestCase_MIB_Multichain_Pre (bool reportGml = false, uint32_t regsBitsCount = DYNAMIC_TDR_LEN)
{
  auto sm = Startup::GetSystemModel();

  TestModelBuilder builder(*sm);

  auto tap = builder.Create_TestCase_MIB_Multichain_Pre("TAP", 4u, regsBitsCount);

  auto regDyn_0  = sm->RegisterWithId(6u);
  auto regDyn_1  = sm->RegisterWithId(7u);
  auto regDyn_2  = sm->RegisterWithId(8u);
  auto regDyn_3  = sm->RegisterWithId(9u);

  regDyn_0->SetToSut   (BinaryVector(regsBitsCount, 0x60));
  regDyn_1->SetToSut   (BinaryVector(regsBitsCount, 0x61));
  regDyn_2->SetToSut   (BinaryVector(regsBitsCount, 0x62));
  regDyn_3->SetToSut   (BinaryVector(regsBitsCount, 0x63));

  regDyn_0->SetBypass  (BinaryVector(regsBitsCount, 0x40));
  regDyn_1->SetBypass  (BinaryVector(regsBitsCount, 0x41));
  regDyn_2->SetBypass  (BinaryVector(regsBitsCount, 0x42));
  regDyn_3->SetBypass  (BinaryVector(regsBitsCount, 0x43));

  regDyn_0->SetPendingForRead(); regDyn_0->SetFromSut  (BinaryVector(regsBitsCount, 0x50));
  regDyn_1->SetPendingForRead(); regDyn_1->SetFromSut  (BinaryVector(regsBitsCount, 0x51));
  regDyn_2->SetPendingForRead(); regDyn_2->SetFromSut  (BinaryVector(regsBitsCount, 0x52));
  regDyn_3->SetPendingForRead(); regDyn_3->SetFromSut  (BinaryVector(regsBitsCount, 0x53));

  regDyn_0->SetHoldValue(false);
  regDyn_1->SetHoldValue(false);
  regDyn_2->SetHoldValue(false);
  regDyn_3->SetHoldValue(false);

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


//! Checks SystemModelManager::iGetRefresh()
//!
//! @param writtenValue   Value that is written in an iWrite prior to iGetRefresh
//! @param expected       Value of bypass register expected to be gotten from iGetRefresh
//!
template<typename T> void Check_iGetRefresh (T writtenValue, T expected)
{
  // ---------------- Setup
  //
  Session session;
  Create_TestCase_MIB_Multichain_Pre(false, 64u);

  auto regName = "dynamic_1";
  iPrefix("TAP_DR_Mux.MIB_mux");
  iWrite(regName, writtenValue);
  iApply();

  // ---------------- Exercise
  //
  auto readData = iGetRefresh<T>(regName);  // Get bypass value (because register is not configured to hold its value)

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (readData, expected);
}

//! Checks SystemModelManager::iGetRefresh()
//!
template<typename T> void Check_iGetRefresh (T value, StringType stringType, string_view expected)
{
  // ---------------- Setup
  //
  Session session;
  Create_TestCase_MIB_Multichain_Pre(false, 64u);

  auto regName = "dynamic_1";
  iPrefix("TAP_DR_Mux.MIB_mux");
  iWrite(regName, value);
  iApply();

  string readData;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (iGetRefresh(regName, readData, stringType));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (readData, expected);
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

//! Checks SystemModelManager::iRead()
//!
//! @param iReadValue         The value passed to iRead
//! @param iWriteValue        The value passed to iWrite
//! @param expectedMismatch   Expected mismatch (checked with 40 bits register)
//!
template<typename T> void Check_iRead (T iReadValue, T iWriteValue, string_view expectedMismatch)
{
  // ---------------- Setup
  //
  Session session;
  Create_TestCase_MIB_Multichain_Pre(false, 40u);

  auto regPath = "TAP_DR_Mux.MIB_mux.dynamic_2";

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (iRead(regPath, iReadValue));

  // ---------------- Verify
  //
  iWrite (regPath, iWriteValue); // Loopback (default protocol) will force FromSut to be updated
  iApply();

  auto xorResult = iGetMiscompares    (regPath, StringType::Hex);
  auto status    = iGetRegisterStatus (regPath, false);

  TS_ASSERT_EQUALS (status,    1u);
  TS_ASSERT_EQUALS (xorResult, expectedMismatch);   // 40 bits reg
}

} // End of unnamed namespace



//! Initializes test (called for each test)
void UT_PDL_Adapter_CPP::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped
  CxxTest::setDisplayUnsignedAsHex(true);
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


void UT_PDL_Adapter_CPP::test_iGet_uint8  () { Check_iGet<uint8_t>  (0x51);       }
void UT_PDL_Adapter_CPP::test_iGet_uint16 () { Check_iGet<uint16_t> (0x5151);     }
void UT_PDL_Adapter_CPP::test_iGet_uint32 () { Check_iGet<uint32_t> (0x51515151); }
void UT_PDL_Adapter_CPP::test_iGet_uint64 () { Check_iGet<uint64_t> (0x51515151); }
void UT_PDL_Adapter_CPP::test_iGet_int8   () { Check_iGet<int8_t>   (0x51);       }
void UT_PDL_Adapter_CPP::test_iGet_int16  () { Check_iGet<int16_t>  (0x5151);     }
void UT_PDL_Adapter_CPP::test_iGet_int32  () { Check_iGet<int32_t>  (0x51515151); }
void UT_PDL_Adapter_CPP::test_iGet_int64  () { Check_iGet<int64_t>  (0x51515151); }

//! Checks the syntactically more pleasant way to use iGet (for integral types)
//!
void UT_PDL_Adapter_CPP::test_iGet_Sugar ()
{
  // ---------------- Setup
  //
  Session session;
  Create_TestCase_MIB_Multichain_Pre(false, 64u);

  // ---------------- Exercise
  //
  auto readData = iGet<uint32_t>("TAP_DR_Mux.MIB_mux.dynamic_1");

  // ---------------- Verify
  //
  auto expected = uint32_t(0x51515151);
  TS_ASSERT_EQUALS (readData, expected);
}



void UT_PDL_Adapter_CPP::test_iGet_String ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto stringType = std::get<0>(data);
    auto expected   = std::get<1>(data);

    Session session;
    Create_TestCase_MIB_Multichain_Pre();

    string readData;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (iGet("TAP_DR_Mux.MIB_mux.dynamic_1", readData, stringType));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (readData, expected);
  };

  auto data =
  {
    make_tuple(StringType::Hex,    "0x51515151"),
    make_tuple(StringType::Binary, "0b01010001010100010101000101010001"),
    //! @todo [JFC]-[September/06/2016]: In test_iGet_String(): Add checking of decimal mode (when available)
    //!
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


void UT_PDL_Adapter_CPP::test_iGet_String_Sugar ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto stringType = std::get<0>(data);
    auto expected   = std::get<1>(data);

    Session session;
    Create_TestCase_MIB_Multichain_Pre();

    // ---------------- Exercise
    //
    auto readData = iGet("TAP_DR_Mux.MIB_mux.dynamic_1", stringType);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (readData, expected);
  };

  auto data =
  {
    make_tuple(StringType::Hex,    "0x51515151"),
    make_tuple(StringType::Binary, "0b01010001010100010101000101010001"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}

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

void UT_PDL_Adapter_CPP::test_iGetRefresh_uint8  () { Check_iGetRefresh<uint8_t>  (0x51,                  0x41); }
void UT_PDL_Adapter_CPP::test_iGetRefresh_uint16 () { Check_iGetRefresh<uint16_t> (0x5141,                0x4141); }
void UT_PDL_Adapter_CPP::test_iGetRefresh_uint32 () { Check_iGetRefresh<uint32_t> (0x51413121UL,          0x41414141); }
void UT_PDL_Adapter_CPP::test_iGetRefresh_uint64 () { Check_iGetRefresh<uint64_t> (0x0171615141312111ULL, 0x4141414141414141); }
void UT_PDL_Adapter_CPP::test_iGetRefresh_int8   () { Check_iGetRefresh<int8_t>   (0x51,                  0x41); }
void UT_PDL_Adapter_CPP::test_iGetRefresh_int16  () { Check_iGetRefresh<int16_t>  (0x5141,                0x4141); }
void UT_PDL_Adapter_CPP::test_iGetRefresh_int32  () { Check_iGetRefresh<int32_t>  (0x51413121UL,          0x41414141); }
void UT_PDL_Adapter_CPP::test_iGetRefresh_int64  () { Check_iGetRefresh<int64_t>  (0x8171615141312111ULL, 0x4141414141414141); }

//! Checks the syntactically more pleasant way to use iGetRefresh (for integral types)
//!
void UT_PDL_Adapter_CPP::test_iGetRefresh_Sugar ()
{
  // ---------------- Setup
  //
  Session session;
  Create_TestCase_MIB_Multichain_Pre(false, 64u);

  auto value   = uint32_t(0x51413121UL);
  auto regName = "dynamic_1";

  iPrefix("TAP_DR_Mux.MIB_mux");
  iWrite(regName, value);
  iApply();

  // ---------------- Exercise
  //
  auto readData = iGetRefresh<uint32_t>(regName);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (readData, 0x41414141);  // The loopback protocol implies to get bypass value
}

void UT_PDL_Adapter_CPP::test_iGetRefresh_String ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto value      = std::get<0>(data);
    auto stringType = std::get<1>(data);
    auto expected   = std::get<2>(data);

    Session session;
    Create_TestCase_MIB_Multichain_Pre(false, 64u);

    auto regName = "dynamic_1";
    iPrefix("TAP_DR_Mux.MIB_mux");
    iWrite(regName, value);
    iApply();

    string readData;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (iGetRefresh(regName, readData, stringType));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (readData, expected);
  };

  auto data =
  {
    make_tuple(uint64_t(0xCAFE), StringType::Hex,    "0x4141414141414141"),  // iGetRefresh get bypass value
    make_tuple(uint64_t(0xFADE), StringType::Binary, "0b0100000101000001010000010100000101000001010000010100000101000001"),
    //! @todo [JFC]-[September/06/2016]: In test_iGetRefresh_String(): Add checking of decimal mode (when available)
    //!
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks the syntactically more pleasant way to use iGetRefresh (for string)
//!
void UT_PDL_Adapter_CPP::test_iGetRefresh_String_Sugar ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto value      = std::get<0>(data);
    auto stringType = std::get<1>(data);
    auto expected   = std::get<2>(data);

    Session session;
    Create_TestCase_MIB_Multichain_Pre(false, 64u);

    auto regName = "dynamic_1";
    iPrefix("TAP_DR_Mux.MIB_mux");
    iWrite(regName, value);
    iApply();

    // ---------------- Exercise
    //
    auto readData = iGetRefresh(regName, stringType);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (readData, expected);
  };

  auto data =
  {
    make_tuple(uint64_t(0x8171615141312111ULL), StringType::Hex,    "0x4141414141414141"),
    make_tuple(uint64_t(0x8171615141312111ULL), StringType::Binary, "0b0100000101000001010000010100000101000001010000010100000101000001"),
    //! @todo [JFC]-[September/06/2016]: In test_iGetRefresh_String_Sugar(): Add checking of decimal mode (when available)
    //!
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks iGetMiscompares expecting zero but got not zero
//!
void UT_PDL_Adapter_CPP::test_iGetMiscompares_Expecting_Zero ()
{
  // ---------------- Setup
  //
  Session session;
  Create_TestCase_MIB_Multichain_Pre();

  auto regPath = "TAP_DR_Mux.MIB_mux.dynamic_2";
  auto fromSut = "0xFADE5555";

  iRead  (regPath, "0x00000000");
  iWrite (regPath, fromSut);        // Loopback (default protocol) will force FromSut to be updated with that value
  iApply();

  string xorResult;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (iGetMiscompares(regPath, xorResult, StringType::Hex));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (xorResult, fromSut);
}


//! Checks iGetMiscompares expecting a value and got another one
//!
void UT_PDL_Adapter_CPP::test_iGetMiscompares ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto stringType = std::get<0>(data);
    auto expected   = std::get<1>(data);

    Session session;
    Create_TestCase_MIB_Multichain_Pre();

    auto regPath = "TAP_DR_Mux.MIB_mux.dynamic_2";
    iRead  (regPath, "0xFADE6666");
    iWrite (regPath, "0xFADE5555"); // Loopback (default protocol) will force FromSut to be updated
    iApply();

    string xorResult;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (iGetMiscompares(regPath, xorResult, stringType));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (xorResult, expected);
  };

  auto data =
  {
    make_tuple(StringType::Hex,    "0x00003333"),
    make_tuple(StringType::Binary, "0b00000000000000000011001100110011"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks iGetMiscompares, easier to use version, expecting a value and got another one
//!
void UT_PDL_Adapter_CPP::test_iGetMiscompares_Sugar ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto stringType = std::get<0>(data);
    auto expected   = std::get<1>(data);

    Session session;
    Create_TestCase_MIB_Multichain_Pre();

    auto regPath = "TAP_DR_Mux.MIB_mux.dynamic_2";
    iRead  (regPath, "0xFADE6666");
    iWrite (regPath, "0xFADE5555"); // Loopback (default protocol) will force FromSut to be updated
    iApply();

    // ---------------- Exercise
    //
    auto xorResult = iGetMiscompares(regPath, stringType);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (xorResult, expected);
  };

  auto data =
  {
    make_tuple(StringType::Hex,    "0x00003333"),
    make_tuple(StringType::Binary, "0b00000000000000000011001100110011"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}

//! Checks iGetRegisterStatus requesting to reset the mismatched count
//!
void UT_PDL_Adapter_CPP::test_iGetRegisterStatus_with_Reset ()
{
  // ---------------- Setup
  //
  Session session;
  Create_TestCase_MIB_Multichain_Pre();

  auto regPath = "TAP_DR_Mux.MIB_mux.dynamic_2";

  iRead  (regPath, "0x00000000");  // Expect a value different of what we will get
  iWrite (regPath, "0xFADE5555");  // Loopback (default protocol) will force FromSut to be updated with that value
  iApply();

  // ---------------- Exercise
  //
  auto status = iGetRegisterStatus(regPath, true);

  // ---------------- Verify
  //
  auto newStatus = iGetRegisterStatus(regPath, false);

  TS_ASSERT_EQUALS (status,    1u);
  TS_ASSERT_EQUALS (newStatus, 0u);
}


//! Checks iGetRegisterStatus without requesting to reset the mismatched count
//!
void UT_PDL_Adapter_CPP::test_iGetRegisterStatus_without_Reset ()
{
  // ---------------- Setup
  //
  Session session;
  Create_TestCase_MIB_Multichain_Pre();

  auto regPath = "TAP_DR_Mux.MIB_mux.dynamic_2";

  iRead  (regPath, "0x00000000");  // Expect a value different of what we will get
  iWrite (regPath, "0xFADE5555");  // Loopback (default protocol) will force FromSut to be updated with that value
  iApply();

  // ---------------- Exercise
  //
  auto status = iGetRegisterStatus(regPath, false);

  // ---------------- Verify
  //
  auto newStatus = iGetRegisterStatus(regPath, false);

  TS_ASSERT_EQUALS (status,    1u);
  TS_ASSERT_EQUALS (newStatus, 1u);
}


//! Checks iRefresh
//!
void UT_PDL_Adapter_CPP::test_iRefresh ()
{
  // ---------------- Setup
  //
  Session session;
  Create_TestCase_MIB_Multichain_Pre();

  auto regPath = "TAP_DR_Mux.MIB_mux.dynamic_2";

  iWrite (regPath, "0xFADE5555");  // Loopback (default protocol) will force FromSut to be updated with that value
  iApply();

  auto readCheck = iGet<uint32_t>(regPath);
  TS_ASSERT_EQUALS (readCheck, 0xFADE5555);

  // ---------------- Exercise
  //
  iRefresh(regPath);

  // ---------------- Verify
  //
  iApply();                                   // Queued refresh must be applied to be effective
  auto readValue = iGet<uint32_t>(regPath);
  TS_ASSERT_EQUALS (readValue, 0x42424242u);  // Bypass value has been read because of Loopback protocol
}
                                                                  //  (iReadVal, iWriteVal, expectedMismatch)
// ---------------- Check iRead operation
//
void UT_PDL_Adapter_CPP::test_iRead_uint8  () { Check_iRead<uint8_t>  (0xF0,        0xAC,        "0x000000005C"); }
void UT_PDL_Adapter_CPP::test_iRead_uint16 () { Check_iRead<uint16_t> (0x0FF0,      0xACDE,      "0x000000A32E"); }
void UT_PDL_Adapter_CPP::test_iRead_uint32 () { Check_iRead<uint32_t> (0xFF00FF00,  0x89ABCDEF,  "0x0076AB32EF"); }
void UT_PDL_Adapter_CPP::test_iRead_uint64 () { Check_iRead<uint64_t> (0xCFF00FF00, 0xD89ABCDEF, "0x0176AB32EF"); }
void UT_PDL_Adapter_CPP::test_iRead_int8   () { Check_iRead<int8_t>   (0x70,        0x6C,        "0x000000001C"); }
void UT_PDL_Adapter_CPP::test_iRead_int16  () { Check_iRead<int16_t>  (0x0FF0,      0x3CDE,      "0x000000332E"); }
void UT_PDL_Adapter_CPP::test_iRead_int32  () { Check_iRead<int32_t>  (0x2F00FF00,  0x49ABCDEF,  "0x0066AB32EF"); }
void UT_PDL_Adapter_CPP::test_iRead_int64  () { Check_iRead<int64_t>  (0xCFF00FF00, 0xD89ABCDEF, "0x0176AB32EF"); }
void UT_PDL_Adapter_CPP::test_iRead_String ()
{
  // ---------------- Setup
  //
  Session session;
  Create_TestCase_MIB_Multichain_Pre(false, 40u);

  auto regPath = "TAP_DR_Mux.MIB_mux.dynamic_2";

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (iRead(regPath, "0x0CFF00FF00"));

  // ---------------- Verify
  //
  iWrite (regPath, 0xD89ABCDEF); // Loopback (default protocol) will force FromSut to be updated
  iApply();

  auto xorResult = iGetMiscompares    (regPath, StringType::Hex);
  auto status    = iGetRegisterStatus (regPath, false);

  TS_ASSERT_EQUALS (status,    1u);
  TS_ASSERT_EQUALS (xorResult, "0x0176AB32EF");   // 40 bits reg
}

//===========================================================================
// End of UT_PDL_Adapter_CPP.cpp
//===========================================================================
