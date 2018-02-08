//===========================================================================
//                           UT_Register.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_Register.cpp
//!
//! Implements test fixture for testing Register
//!
//===========================================================================


#include "UT_Register.hpp"
#include "Register.hpp"
#include "BinaryVector_Traits.hpp"

#include <tuple>

using std::tuple;
using std::make_tuple;

using namespace mast;
namespace
{
template<typename T, typename U> void Check_LastFromSut(U data)
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString(std::get<0>(data));
  const auto expected = std::get<1>(data);

  Register sut("Reg", initial, true);
  sut.SetToSut(0u);
  sut.SetExpectedFromSut(0u);
  sut.SetBypass(BinaryVector(initial.BitsCount()));

  T gotValue;

  // ---------------- Exercise
  //
  sut.LastFromSut(gotValue);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (gotValue, expected);
}

//! Checks Register::SetExpectedFromSut() with integral type and don't care mask
//!
template<typename T>
void Check_SetExpectedFromSut_DontCare (T expected, T mask)
{
  static_assert(std::is_integral<T>::value, "");

  // ---------------- Setup
  //
  Register sut("Reg", BinaryVector::CreateFromString("0xF0A5B6C7D8E9"), true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.SetExpectedFromSut(expected, mask));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.ExpectedFromSut().Get<T>(), expected);
  TS_ASSERT_EQUALS (sut.DontCareMask().Get<T>(),    mask);
}

} // End of unnamed namespace



//! Initializes test (called for each test)
void UT_Register::setUp ()
{
  SystemModelNode::ResetNodeIdentifier(); // To start each test with node identifier equal 0
}


//! Pseudo test of sizeof(Register)
//! @note This is mainly used as an information on real size of Register
//!       and to detect/validate changes.
//!       This is the minimal memory usage (when it is empty)
void UT_Register::test_Sizeof ()
{
  TS_ASSERT_LESS_THAN_EQUALS (sizeof(Register), 368u);     // On Windows (32 bits), it is 208, even though in both case sizeof(int) is 4!
}


//! Checks Register:: constructor
//!
//! @note SystemModelNode parts have been test elsewhere
void UT_Register::test_Constructor ()
{
  // ---------------- Setup
  //
  auto name           = "A name";
  auto bypassSequence = BinaryVector::CreateFromBinaryString("1111_1111:0");

  // ---------------- Exercise
  //
  Register sut(name, bypassSequence);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_EQUALS (sut.TypeName(),            "Register");
  TS_ASSERT_EQUALS (sut.Identifier(),          0u);                    // ==> First create SystemModelNode
  TS_ASSERT_EQUALS (sut.Identifiers().size(),  1u);
  TS_ASSERT_EQUALS (sut.Identifiers().front(), 0u);
  TS_ASSERT_FALSE  (sut.HoldValue());
  TS_ASSERT_FALSE  (sut.MustCheckExpected());
  TS_ASSERT_EQUALS (sut.Mismatches(),          0U);
  TS_ASSERT_EQUALS (sut.BitsCount(),           9U);
  TS_ASSERT_EQUALS (sut.PendingCount(),        0U);
  TS_ASSERT_FALSE  (sut.IsPendingForRead());
  TS_ASSERT_FALSE  (sut.IsPending());
  TS_ASSERT_EQUALS (sut.BypassSequence(),      bypassSequence);
  TS_ASSERT_EQUALS (sut.NextToSut(),           bypassSequence);
  TS_ASSERT_EQUALS (sut.LastToSut(),           bypassSequence);
  TS_ASSERT_EQUALS (sut.ExpectedFromSut(),     bypassSequence);
  TS_ASSERT_EQUALS (sut.LastFromSut(),         bypassSequence);
  TS_ASSERT_EQUALS (sut.LastReadFromSut(),     BinaryVector(9u));
  TS_ASSERT_EQUALS (sut.LastCompareResult(),   sut.ExpectedFromSut()); // No value actually read yet

  uint8_t  last_uint8  = 0x55;  sut.LastReadFromSut(last_uint8);
  uint16_t last_uint16 = 0x55;  sut.LastReadFromSut(last_uint16);
  uint32_t last_uint32 = 0x55;  sut.LastReadFromSut(last_uint32);
  uint64_t last_uint64 = 0x55;  sut.LastReadFromSut(last_uint64);
  int8_t   last_int8   = 0x55;  sut.LastReadFromSut(last_int8);
  int16_t  last_int16  = 0x55;  sut.LastReadFromSut(last_int16);
  int32_t  last_int32  = 0x55;  sut.LastReadFromSut(last_int32);
  int64_t  last_int64  = 0x55;  sut.LastReadFromSut(last_int64);

  TS_ASSERT_EQUALS (last_uint8,  0U);
  TS_ASSERT_EQUALS (last_uint16, 0U);
  TS_ASSERT_EQUALS (last_uint32, 0UL);
  TS_ASSERT_EQUALS (last_uint64, 0ULL);
  TS_ASSERT_EQUALS (last_int8,   0);
  TS_ASSERT_EQUALS (last_int16,  0);
  TS_ASSERT_EQUALS (last_int32,  0L);
  TS_ASSERT_EQUALS (last_int64,  0LL);
}




//! Checks Register::SetPendingForWrite
//!
void UT_Register::test_SetPendingForWrite ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("1111_1111:0");

  Register sut("", bypassSequence);

  // ---------------- Exercise
  //
  sut.SetPendingForWrite(true);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.IsPending());
  TS_ASSERT_FALSE  (sut.IsPendingForRead());
  TS_ASSERT_EQUALS (sut.PendingCount(), 1U);
}


//! Checks Register::ResetPendingWrite
//!
void UT_Register::test_ResetPendingWrite ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("1111_1111:0");

  Register sut("", bypassSequence);

  sut.SetPendingForWrite(true);

  // ---------------- Exercise
  //
  sut.ResetPendingWrite();

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.IsPending());
  TS_ASSERT_FALSE  (sut.IsPendingForRead());
  TS_ASSERT_EQUALS (sut.PendingCount(), 0U);
}


//! Checks Register::SetPendingForWrite
//!
void UT_Register::test_SetPendingForRead ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("1111_1111:0");

  Register sut("", bypassSequence);

  // ---------------- Exercise
  //
  sut.SetPendingForRead(true);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.IsPending());
  TS_ASSERT_TRUE   (sut.IsPendingForRead());
  TS_ASSERT_EQUALS (sut.PendingCount(), 1U);
}

//! Checks Register when it is both pending for read and for write
//!
void UT_Register::test_SetBothPending ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("1111_1111:0");

  Register sut("", bypassSequence);

  // ---------------- Exercise
  //
  sut.SetPendingForRead();
  sut.SetPendingForWrite();

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.IsPending());
  TS_ASSERT_TRUE   (sut.IsPendingForRead());
  TS_ASSERT_EQUALS (sut.PendingCount(), 1U);
}

//! Checks Register::ResetPending
//!
void UT_Register::test_ResetPending ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("1111_1111:0");

  Register sut("", bypassSequence);

  sut.SetPendingForRead(true);
  sut.SetPendingForWrite(true);

  // ---------------- Exercise
  //
  sut.ResetPending();

  // ---------------- Verify
  //
  TS_ASSERT_FALSE  (sut.IsPending());
  TS_ASSERT_FALSE  (sut.IsPendingForRead());
  TS_ASSERT_EQUALS (sut.PendingCount(), 0U);
}


//! Checks Register::SetBypass() with a proper value
//!
void UT_Register::test_SetBypass ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:0");

  Register sut("Reg", initial);

  // ---------------- Exercise
  //
  sut.SetBypass(newValue);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.BypassSequence(), newValue);
}

//! Checks Register::SetBypass() with a too small value
//!
void UT_Register::test_SetBypass_TooSmall ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111");

  Register sut("Reg", initial);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.SetBypass(newValue), std::exception);
}


//! Checks Register::SetBypass() with a too large value
//!
void UT_Register::test_SetBypass_TooLarge ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:01");

  Register sut("Reg", initial);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.SetBypass(newValue), std::exception);
}

//! Checks Register::SetToSut() with a proper value
//!
void UT_Register::test_SetToSut ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:0");

  Register sut("Reg", initial);

  // ---------------- Exercise
  //
  sut.SetToSut(newValue);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.NextToSut(), newValue);
  TS_ASSERT_EQUALS (sut.LastToSut(), initial);
}

//! Checks Register::SetToSut() with a value of different size
//!
void UT_Register::test_SetToSut_DifferentSize ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111");

  Register sut("Reg", initial);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.SetToSut(newValue), std::exception);
}



//! Checks Register::SetExpectedFromSut() with a proper value
//!
void UT_Register::test_SetExpectedFromSut ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:0");

  Register sut("Reg", initial);

  // ---------------- Exercise
  //
  sut.SetExpectedFromSut(newValue);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.ExpectedFromSut(),   newValue);
  TS_ASSERT_EQUALS (sut.LastCompareResult(), sut.ExpectedFromSut());  // No value actually read yet
}

//! Checks Register::SetExpectedFromSut() with a value of different size
//!
void UT_Register::test_SetExpectedFromSut_DifferentSize ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111");

  Register sut("Reg", initial);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.SetExpectedFromSut(newValue), std::exception);
}


//! Checks Register::SetExpectedFromSut() with a proper value
//!
void UT_Register::test_SetExpectedFromSut_DontCare ()
{
  // ---------------- Setup
  //
  const auto initial         = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto expectedFromSut = BinaryVector::CreateFromBinaryString("0000_0101:0");
  const auto dontCareMask    = BinaryVector::CreateFromBinaryString("1000_0111:1");

  Register sut("Reg", initial);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.SetExpectedFromSut(expectedFromSut, dontCareMask));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.ExpectedFromSut(), expectedFromSut);
  TS_ASSERT_EQUALS (sut.DontCareMask(),    dontCareMask);
}


//! Checks Register::SetExpectedFromSut() with a don't care maske of improper size
//!
void UT_Register::test_SetExpectedFromSut_DontCare_DifferentSize ()
{
  // ---------------- Setup
  //
  const auto initial         = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto expectedFromSut = BinaryVector::CreateFromBinaryString("0000_0101:0");
  const auto dontCareMask    = BinaryVector::CreateFromBinaryString("1000_0111");

  Register sut("Reg", initial);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.SetExpectedFromSut(expectedFromSut, dontCareMask), std::exception);
}


//! Checks Register::SetFromSut() with a value different from expected one and check disabled
//!
void UT_Register::test_SetFromSut_Differing_WithoutCheck ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:0");
  const auto expected = BinaryVector::CreateFromBinaryString("1110_0111:1");

  Register sut("Reg", initial);
  sut.SetExpectedFromSut(expected);

  // ---------------- Exercise
  //
  sut.SetFromSut(newValue);

  // ---------------- Verify
  //
  TS_ASSERT_FALSE  (sut.MustCheckExpected());
  TS_ASSERT_EQUALS (sut.LastFromSut(),     newValue);
  TS_ASSERT_EQUALS (sut.LastReadFromSut(), BinaryVector(initial.BitsCount()));
  TS_ASSERT_EQUALS (sut.Mismatches(),      0);
  TS_ASSERT_EQUALS (sut.LastCompareResult(), expected);   // Check was not activated (so LastCompareResult has not been updated)
}


//! Checks Register::SetFromSut() with a value different from expected one and check enabled
//!
void UT_Register::test_SetFromSut_Differing_WithCheck ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:0");
  const auto expected = BinaryVector::CreateFromBinaryString("1110_0111:1");

  Register sut("Reg", initial);
  sut.SetExpectedFromSut(expected);
  sut.SetPendingForRead(true);
  sut.SetCheckExpected(true);

  // ---------------- Exercise
  //
  sut.SetFromSut(newValue);

  // ---------------- Verify
  //
  auto expectedCompareResult = BinaryVector::CreateFromBinaryString("0000_0000:1");

  TS_ASSERT_TRUE   (sut.MustCheckExpected());
  TS_ASSERT_EQUALS (sut.LastFromSut(),       newValue);
  TS_ASSERT_EQUALS (sut.LastReadFromSut(),   newValue);
  TS_ASSERT_EQUALS (sut.Mismatches(),        1);
  TS_ASSERT_EQUALS (sut.LastCompareResult(), expectedCompareResult);
}


//! Checks Register::SetFromSut() with a value different from expected, check enabled and there is a mismatch
//!
void UT_Register::test_SetFromSut_Differing_DontCare_Mismatch ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:0");
  const auto expected = BinaryVector::CreateFromBinaryString("1010_1000:1");
  const auto dontcare = BinaryVector::CreateFromBinaryString("1011_1100:1");

  Register sut("Reg", initial);
  sut.SetExpectedFromSut(expected, dontcare);
  sut.SetPendingForRead(true);
  sut.SetCheckExpected(true);

  // ---------------- Exercise
  //
  sut.SetFromSut(newValue);

  // ---------------- Verify
  //
  auto expectedCompareResult = BinaryVector::CreateFromBinaryString("0000_1100:1");

  TS_ASSERT_TRUE   (sut.MustCheckExpected());
  TS_ASSERT_EQUALS (sut.LastFromSut(),       newValue);
  TS_ASSERT_EQUALS (sut.LastReadFromSut(),   newValue);
  TS_ASSERT_EQUALS (sut.Mismatches(),        1);
  TS_ASSERT_EQUALS (sut.LastCompareResult(), expectedCompareResult);
}


//! Checks Register::SetFromSut() with a value different from expected, check enabled and there is a no mismatch due to don't care
//!
void UT_Register::test_SetFromSut_Differing_DontCare_NoMismatch ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1010_1011:1");
  const auto expected = BinaryVector::CreateFromBinaryString("1010_1000:0");
  const auto dontcare = BinaryVector::CreateFromBinaryString("1111_1100:0");

  Register sut("Reg", initial);
  sut.SetExpectedFromSut(expected, dontcare);
  sut.SetPendingForRead(true);
  sut.SetCheckExpected(true);

  // ---------------- Exercise
  //
  sut.SetFromSut(newValue);

  // ---------------- Verify
  //
  auto expectedCompareResult = BinaryVector::CreateFromBinaryString("0000_0000:0");

  TS_ASSERT_TRUE   (sut.MustCheckExpected());
  TS_ASSERT_EQUALS (sut.LastFromSut(),       newValue);
  TS_ASSERT_EQUALS (sut.LastReadFromSut(),   newValue);
  TS_ASSERT_EQUALS (sut.Mismatches(),        0);
  TS_ASSERT_EQUALS (sut.LastCompareResult(), expectedCompareResult);
}

//! Checks Register::SetFromSut() while in pending read
//!
void UT_Register::test_SetFromSut_Differing_WithPendingRead ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111");
  const auto newValue = BinaryVector::CreateFromBinaryString("0010_0110");

  Register sut("Reg", initial);
  sut.SetExpectedFromSut(BinaryVector::CreateFromBinaryString("1100_0101"));
  sut.SetPendingForRead(true);

  // ---------------- Exercise
  //
  sut.SetFromSut(newValue);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.LastFromSut(),       newValue);
  TS_ASSERT_EQUALS (sut.LastReadFromSut(),   newValue);
  TS_ASSERT_EQUALS (sut.Mismatches(),        0);
  TS_ASSERT_EQUALS (sut.LastCompareResult(), BinaryVector::CreateFromBinaryString("1110_0011"));

  uint8_t  last_uint8  = 0x55U;   sut.LastReadFromSut(last_uint8);
  uint16_t last_uint16 = 0x55U;   sut.LastReadFromSut(last_uint16);
  uint32_t last_uint32 = 0x55UL;  sut.LastReadFromSut(last_uint32);
  uint64_t last_uint64 = 0x55ULL; sut.LastReadFromSut(last_uint64);
  int8_t   last_int8   = 0x55;    sut.LastReadFromSut(last_int8);
  int16_t  last_int16  = 0x55;    sut.LastReadFromSut(last_int16);
  int32_t  last_int32  = 0x55L;   sut.LastReadFromSut(last_int32);
  int64_t  last_int64  = 0x55LL;  sut.LastReadFromSut(last_int64);

  CxxTest::setDisplayUnsignedAsHex(true);
  TS_ASSERT_EQUALS (last_uint8,  0x26U);
  TS_ASSERT_EQUALS (last_uint16, 0x26U);
  TS_ASSERT_EQUALS (last_uint32, 0x26UL);
  TS_ASSERT_EQUALS (last_uint64, 0x26ULL);
  TS_ASSERT_EQUALS (last_int8,   0x26);
  TS_ASSERT_EQUALS (last_int16,  0x26);
  TS_ASSERT_EQUALS (last_int32,  0x26L);
  TS_ASSERT_EQUALS (last_int64,  0x26LL);
}



//! Checks Register::SetFromSut() with a value of different size
//!
void UT_Register::test_SetFromSut_DifferentSize ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111");

  Register sut("Reg", initial);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.SetFromSut(newValue), std::exception);
}



//! Checks Register::UpdateLastToSut()
//!
void UT_Register::test_UpdateLastToSut ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:1");

  Register sut("Reg", initial);
  sut.SetToSut(newValue);

  // ---------------- Exercise
  //
  sut.UpdateLastToSut();

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.LastToSut(), newValue);
}


//! Checks when Register::HoldValue() is true, "bypass sequence" is updated whenever "next to sut" is
//!
void UT_Register::test_HoldValue ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");

  Register sut("Reg", initial);

  // ---------------- Exercise
  //
  sut.SetHoldValue(true);

  // ---------------- Verify
  //
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:0");

  sut.SetToSut(newValue);

  TS_ASSERT_EQUALS (sut.NextToSut(),      newValue);
  TS_ASSERT_EQUALS (sut.BypassSequence(), newValue);
}

//! Checks Register::SetToSut() with a proper value from uint8_t
//!
void UT_Register::test_SetToSut_uint8 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto initial  = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    const auto newValue = std::get<1>(data);
    const auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    Register sut("Reg", initial, true);

    // ---------------- Exercise
    //
    sut.SetToSut(newValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.NextToSut(), expected);
    TS_ASSERT_EQUALS (sut.LastToSut(), initial);
  };

  auto data =
  {
    make_tuple("0:0000",    uint8_t(5u),  "0:0101"),
    make_tuple("0:0000",    uint8_t(25u), "1:1001"),
    make_tuple("0000:0000", uint8_t(25u), "0001:1001"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetToSut() with a proper value from uint16_t
//!
void UT_Register::test_SetToSut_uint16 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto initial  = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    const auto newValue = std::get<1>(data);
    const auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    Register sut("Reg", initial, true);

    // ---------------- Exercise
    //
    sut.SetToSut(newValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.NextToSut(), expected);
    TS_ASSERT_EQUALS (sut.LastToSut(), initial);
  };

  auto data =
  {
    make_tuple("0:0000",                uint16_t(5u),         "0:0101"),
    make_tuple("1|1111:1111_1111:1111", uint16_t(25u),        "0|0000:0000_0001:1001"),
    make_tuple("0000:0000_0000:0000",   uint16_t(UINT16_MAX), "1111:1111_1111:1111"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetToSut() with a proper value from uint32_t
//!
void UT_Register::test_SetToSut_uint32 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto initial  = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    const auto newValue = std::get<1>(data);
    const auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    Register sut("Reg", initial, true);

    // ---------------- Exercise
    //
    sut.SetToSut(newValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.NextToSut(), expected);
    TS_ASSERT_EQUALS (sut.LastToSut(), initial);
  };

  auto data =
  {
    make_tuple("0:0000",    uint32_t(5u),  "0:0101"),
    make_tuple("0:0000",    uint32_t(25u), "1:1001"),
    make_tuple("111||1111:1111_1111:1111|111:1111_1111:1111", uint32_t(123456789u), "000||0000:1110_1011:0111|100:1101_0001:0101"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetToSut() with a proper value from uint64_t
//!
void UT_Register::test_SetToSut_uint64 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto initial  = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    const auto newValue = std::get<1>(data);
    const auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    Register sut("Reg", initial, true);

    // ---------------- Exercise
    //
    sut.SetToSut(newValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.NextToSut(), expected);
    TS_ASSERT_EQUALS (sut.LastToSut(), initial);
  };

  auto data =
  {
    make_tuple("0:0000",                                                      uint64_t(5u),                "0:0101"),
    make_tuple("0:0000",                                                      uint64_t(25u),               "1:1001"),
    make_tuple("0000:0000",                                                   uint64_t(25u),               "0001:1001"),
    make_tuple("1111:1111_1111:1111|1111:1111_1111:1111-1111:1111_1111:1111", uint64_t(33012345678933ULL), "0001:1110:0000:0110|0100:1001_0010:0011-0010:1100_0101:0101"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetToSut() with a proper value from int8_t
//!
void UT_Register::test_SetToSut_int8 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto initial  = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    const auto newValue = std::get<1>(data);
    const auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    Register sut("Reg", initial, true);

    // ---------------- Exercise
    //
    sut.SetToSut(newValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.NextToSut(), expected);
    TS_ASSERT_EQUALS (sut.LastToSut(), initial);
  };

  auto data =
  {
    make_tuple("0:0000",    int8_t(5),   "0:0101"),
    make_tuple("0:0000",    int8_t(25),  "1:1001"),
    make_tuple("0000:0000", int8_t(-25), "1110:0111"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetToSut() with a proper value from int16_t
//!
void UT_Register::test_SetToSut_int16 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto initial  = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    const auto newValue = std::get<1>(data);
    const auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    Register sut("Reg", initial, true);

    // ---------------- Exercise
    //
    sut.SetToSut(newValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.NextToSut(), expected);
    TS_ASSERT_EQUALS (sut.LastToSut(), initial);
  };

  auto data =
  {
    make_tuple("0:0000",                int16_t(5),         "0:0101"),
    make_tuple("1|1111:1111_1111:1111", int16_t(-25),       "1|1111:1111_1110:0111"),
    make_tuple("0000:0000_0000:0000",   int16_t(INT16_MIN), "1000:0000_0000:0000"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetToSut() with a proper value from int32_t
//!
void UT_Register::test_SetToSut_int32 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto initial  = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    const auto newValue = std::get<1>(data);
    const auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    Register sut("Reg", initial, true);

    // ---------------- Exercise
    //
    sut.SetToSut(newValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.NextToSut(), expected);
    TS_ASSERT_EQUALS (sut.LastToSut(), initial);
  };

  auto data =
  {
    make_tuple("0:0000",                                       int32_t(5L),          "0:0101"),
    make_tuple("00:0000",                                      int32_t(-25L),        "10:0111"),
    make_tuple("111||1111:1111_1111:1111|1111:1111_1111:1111", int32_t(-123456789L), "111||1111:1000_1010:0100|0011:0010_1110:1011"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetToSut() with a proper value from int64_t
//!
void UT_Register::test_SetToSut_int64 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto initial  = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    const auto newValue = std::get<1>(data);
    const auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    Register sut("Reg", initial, true);

    // ---------------- Exercise
    //
    sut.SetToSut(newValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.NextToSut(), expected);
    TS_ASSERT_EQUALS (sut.LastToSut(), initial);
  };

  auto data =
  {
    make_tuple("0:0000",                                                      int64_t(5LL),               "0:0101"),
    make_tuple("000:0000",                                                    int64_t(-25LL),             "110:0111"),
    make_tuple("1111:1111_1111:1111|1111:1111_1111:1111-1111:1111_1111:1111", int64_t(-33012345678933LL), "1110:0001_1111:1001|1011:0110_1101:1100-1101:0011_1010:1011"),

  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetExpectedFromSut() with a proper value from uint8_t
//!
void UT_Register::test_SetExpectedFromSut_uint8 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto initial  = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    const auto newValue = std::get<1>(data);
    const auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    Register sut("Reg", initial, true);

    // ---------------- Exercise
    //
    sut.SetExpectedFromSut(newValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ExpectedFromSut(), expected);
  };

  auto data =
  {
    make_tuple("0:0000",    uint8_t(5u),  "0:0101"),
    make_tuple("0:0000",    uint8_t(25u), "1:1001"),
    make_tuple("0000:0000", uint8_t(25u), "0001:1001"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetExpectedFromSut() with a proper value from uint16_t
//!
void UT_Register::test_SetExpectedFromSut_uint16 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto initial  = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    const auto newValue = std::get<1>(data);
    const auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    Register sut("Reg", initial, true);

    // ---------------- Exercise
    //
    sut.SetExpectedFromSut(newValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ExpectedFromSut(), expected);
  };

  auto data =
  {
    make_tuple("0:0000",                uint16_t(5u),         "0:0101"),
    make_tuple("1|1111:1111_1111:1111", uint16_t(25u),        "0|0000:0000_0001:1001"),
    make_tuple("0000:0000_0000:0000",   uint16_t(UINT16_MAX), "1111:1111_1111:1111"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetExpectedFromSut() with a proper value from uint32_t
//!
void UT_Register::test_SetExpectedFromSut_uint32 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto initial  = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    const auto newValue = std::get<1>(data);
    const auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    Register sut("Reg", initial, true);

    // ---------------- Exercise
    //
    sut.SetExpectedFromSut(newValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ExpectedFromSut(), expected);
  };

  auto data =
  {
    make_tuple("0:0000",    uint32_t(5u),  "0:0101"),
    make_tuple("0:0000",    uint32_t(25u), "1:1001"),
    make_tuple("111||1111:1111_1111:1111|111:1111_1111:1111", uint32_t(123456789u), "000||0000:1110_1011:0111|100:1101_0001:0101"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetExpectedFromSut() with a proper value from uint64_t
//!
void UT_Register::test_SetExpectedFromSut_uint64 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto initial  = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    const auto newValue = std::get<1>(data);
    const auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    Register sut("Reg", initial, true);

    // ---------------- Exercise
    //
    sut.SetExpectedFromSut(newValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ExpectedFromSut(), expected);
  };

  auto data =
  {
    make_tuple("0:0000",                                                      uint64_t(5u),                "0:0101"),
    make_tuple("0:0000",                                                      uint64_t(25u),               "1:1001"),
    make_tuple("0000:0000",                                                   uint64_t(25u),               "0001:1001"),
    make_tuple("1111:1111_1111:1111|1111:1111_1111:1111-1111:1111_1111:1111", uint64_t(33012345678933ULL), "0001:1110:0000:0110|0100:1001_0010:0011-0010:1100_0101:0101"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetExpectedFromSut() with a proper value from int8_t
//!
void UT_Register::test_SetExpectedFromSut_int8 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto initial  = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    const auto newValue = std::get<1>(data);
    const auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    Register sut("Reg", initial, true);

    // ---------------- Exercise
    //
    sut.SetExpectedFromSut(newValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ExpectedFromSut(), expected);
  };

  auto data =
  {
    make_tuple("0:0000",    int8_t(5),   "0:0101"),
    make_tuple("0:0000",    int8_t(25),  "1:1001"),
    make_tuple("0000:0000", int8_t(-25), "1110:0111"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetExpectedFromSut() with a proper value from int16_t
//!
void UT_Register::test_SetExpectedFromSut_int16 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto initial  = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    const auto newValue = std::get<1>(data);
    const auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    Register sut("Reg", initial, true);

    // ---------------- Exercise
    //
    sut.SetExpectedFromSut(newValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ExpectedFromSut(), expected);
  };

  auto data =
  {
    make_tuple("0:0000",                int16_t(5),         "0:0101"),
    make_tuple("1|1111:1111_1111:1111", int16_t(-25),       "1|1111:1111_1110:0111"),
    make_tuple("0000:0000_0000:0000",   int16_t(INT16_MIN), "1000:0000_0000:0000"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetExpectedFromSut() with a proper value from int32_t
//!
void UT_Register::test_SetExpectedFromSut_int32 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto initial  = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    const auto newValue = std::get<1>(data);
    const auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    Register sut("Reg", initial, true);

    // ---------------- Exercise
    //
    sut.SetExpectedFromSut(newValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ExpectedFromSut(), expected);
  };

  auto data =
  {
    make_tuple("0:0000",                                       int32_t(5L),          "0:0101"),
    make_tuple("00:0000",                                      int32_t(-25L),        "10:0111"),
    make_tuple("111||1111:1111_1111:1111|1111:1111_1111:1111", int32_t(-123456789L), "111||1111:1000_1010:0100|0011:0010_1110:1011"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetExpectedFromSut() with a proper value from int64_t
//!
void UT_Register::test_SetExpectedFromSut_int64 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto initial  = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    const auto newValue = std::get<1>(data);
    const auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    Register sut("Reg", initial, true);

    // ---------------- Exercise
    //
    sut.SetExpectedFromSut(newValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.ExpectedFromSut(), expected);
  };

  auto data =
  {
    make_tuple("0:0000",                                                      int64_t(5LL),               "0:0101"),
    make_tuple("000:0000",                                                    int64_t(-25LL),             "110:0111"),
    make_tuple("1111:1111_1111:1111|1111:1111_1111:1111-1111:1111_1111:1111", int64_t(-33012345678933LL), "1110:0001_1111:1001|1011:0110_1101:1100-1101:0011_1010:1011"),

  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


void UT_Register::test_SetExpectedFromSut_DontCare_uint8  () { Check_SetExpectedFromSut_DontCare<uint8_t>  (uint8_t  (0xFA),       uint8_t  (0xFE)       ); }
void UT_Register::test_SetExpectedFromSut_DontCare_uint16 () { Check_SetExpectedFromSut_DontCare<uint16_t> (uint16_t (0xFA12),     uint16_t (0xFEAA)     ); }
void UT_Register::test_SetExpectedFromSut_DontCare_uint32 () { Check_SetExpectedFromSut_DontCare<uint32_t> (uint32_t (0xFA1234),   uint32_t (0xFEAA55)   ); }
void UT_Register::test_SetExpectedFromSut_DontCare_uint64 () { Check_SetExpectedFromSut_DontCare<uint64_t> (uint64_t (0xFA123456), uint64_t (0xFEAA5566) ); }
void UT_Register::test_SetExpectedFromSut_DontCare_int8   () { Check_SetExpectedFromSut_DontCare<int8_t>   (int8_t   (0xFA),       int8_t   (0xFE)       ); }
void UT_Register::test_SetExpectedFromSut_DontCare_int16  () { Check_SetExpectedFromSut_DontCare<int16_t>  (int16_t  (0xFA12),     int16_t  (0xFEAA)     ); }
void UT_Register::test_SetExpectedFromSut_DontCare_int32  () { Check_SetExpectedFromSut_DontCare<int32_t>  (int32_t  (0xFA1234),   int32_t  (0xFEAA55)   ); }
void UT_Register::test_SetExpectedFromSut_DontCare_int64  () { Check_SetExpectedFromSut_DontCare<int64_t>  (int64_t  (0xFA123456), int64_t  (0xFEAA5566) ); }


//! Checks Register::SetToSut() with a proper value from uint8_t
//!
void UT_Register::test_LastFromSut_uint8 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    Check_LastFromSut<uint8_t>(data);
  };

  auto data =
  {
    make_tuple("0:0101",        uint8_t(5)),
    make_tuple("000|0111:1011", uint8_t(123)),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetToSut() with a proper value from uint16_t
//!
void UT_Register::test_LastFromSut_uint16 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    Check_LastFromSut<uint16_t>(data);
  };

  auto data =
  {
    make_tuple("0:0101",                   uint16_t(5)),
    make_tuple("000|0011:0000_0011:1001", uint16_t(12345)),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks Register::SetToSut() with a proper value from uint32_t
//!
void UT_Register::test_LastFromSut_uint32 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    Check_LastFromSut<uint32_t>(data);
  };

  auto data =
  {
    make_tuple("0:0101",                                uint32_t(5L)),
    make_tuple("00:0111_0101:1011|1100:1101_0001:0101", uint32_t(123456789L)),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetToSut() with a proper value from uint64_t
//!
void UT_Register::test_LastFromSut_uint64 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    Check_LastFromSut<uint64_t>(data);
  };

  auto data =
  {
    make_tuple("0:0101",                                                      uint64_t(5ULL)),
    make_tuple("0001:1110:0000:0110|0100:1001_0010:0011-0010:1100_0101:0101", uint64_t(33012345678933ULL)),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}





//! Checks Register::SetToSut() with a proper value from int8_t
//!
void UT_Register::test_LastFromSut_int8 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    Check_LastFromSut<int8_t>(data);
  };

  auto data =
  {
    make_tuple("0:0101",        int8_t(5)),
    make_tuple("110:0111",      int8_t(-25)),
    make_tuple("111|1000:0101", int8_t(-123)),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetToSut() with a proper value from int16_t
//!
void UT_Register::test_LastFromSut_int16 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    Check_LastFromSut<int16_t>(data);
  };

  auto data =
  {
    make_tuple("0:0101",                   int16_t(5)),
    make_tuple("110:0111",                 int16_t(-25)),
    make_tuple("111||1100:1111_1100:0111", int16_t(-12345)),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks Register::SetToSut() with a proper value from int32_t
//!
void UT_Register::test_LastFromSut_int32 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    Check_LastFromSut<int32_t>(data);
  };

  auto data =
  {
    make_tuple("0:0101",                                       int32_t(5L)),
    make_tuple("110:0111",                                     int32_t(-25L)),
    make_tuple("111||1111:1000_1010:0100|0011:0010_1110:1011", int32_t(-123456789L)),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Register::SetToSut() with a proper value from int64_t
//!
void UT_Register::test_LastFromSut_int64 ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    Check_LastFromSut<int64_t>(data);
  };

  auto data =
  {
    make_tuple("0:0101",                                                      int64_t(5LL)),
    make_tuple("110:0111",                                                    int64_t(-25LL)),
    make_tuple("1110:0001_1111:1001|1011:0110_1101:1100-1101:0011_1010:1011", int64_t(-33012345678933LL)),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks Register::ResetMismatches()
//!
void UT_Register::test_ResetMismatches ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:0");
  const auto expected = BinaryVector::CreateFromBinaryString("1110_0111:1");

  Register sut("Reg", initial);
  sut.SetExpectedFromSut(expected);
  sut.SetCheckExpected(true);
  sut.SetFromSut(newValue);

  // ---------------- Exercise
  //
  sut.ResetMismatches();

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.MustCheckExpected());
  TS_ASSERT_EQUALS (sut.Mismatches(), 0);
}


//! Checks Register::Reset() when there is a reset value
//!
void UT_Register::test_Reset ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromString("0x123456");
  const auto reset    = BinaryVector::CreateFromString("0xAA55AA");
  const auto newValue = BinaryVector::CreateFromString("0x789ABC");
  const auto expected = BinaryVector::CreateFromString("0x789ABC");
  const auto dontcare = BinaryVector::CreateFromString("0xF0F0FF");

  Register sut("Reg", initial, reset, true);
  sut.SetExpectedFromSut(expected, dontcare);
  sut.SetCheckExpected(true);
  sut.SetPendingForRead(true);
  sut.SetFromSut(newValue);

  // ---------------- Exercise
  //
  sut.Reset();

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.TypeName(),          "Register");
  TS_ASSERT_EQUALS (sut.BitsCount(),         24U);
  TS_ASSERT_EQUALS (sut.Mismatches(),        0U);
  TS_ASSERT_EQUALS (sut.PendingCount(),      0U);
  TS_ASSERT_FALSE  (sut.IsPendingForRead());
  TS_ASSERT_FALSE  (sut.IsPending());
  TS_ASSERT_TRUE   (sut.HoldValue());
  TS_ASSERT_FALSE  (sut.MustCheckExpected());
  TS_ASSERT_EQUALS (sut.BypassSequence(),    reset);
  TS_ASSERT_EQUALS (sut.NextToSut(),         reset);
  TS_ASSERT_EQUALS (sut.LastToSut(),         initial);
  TS_ASSERT_EQUALS (sut.ExpectedFromSut(),   expected);
  TS_ASSERT_EQUALS (sut.LastFromSut(),       newValue);
  TS_ASSERT_EQUALS (sut.LastReadFromSut(),   newValue);
  TS_ASSERT_EQUALS (sut.LastCompareResult(), BinaryVector::CreateFromString("0x000000"));
}


//! Checks Register::Reset() when there is a no reset value
//!
void UT_Register::test_Reset_NoValue ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromString("0x123456");
  const auto newValue = BinaryVector::CreateFromString("0x789ABC");
  const auto gotValue = BinaryVector::CreateFromString("0x55AA66");
  const auto expected = BinaryVector::CreateFromString("0x789ABF");
  const auto dontcare = BinaryVector::CreateFromString("0xF0F0FF");

  Register sut("Reg", initial, true);
  sut.SetExpectedFromSut(expected, dontcare);
  sut.SetCheckExpected(true);
  sut.SetPendingForRead(true);
  sut.SetToSut  (newValue);
  sut.SetFromSut(gotValue);

  // ---------------- Exercise
  //
  sut.Reset();

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.TypeName(),          "Register");
  TS_ASSERT_EQUALS (sut.BitsCount(),         24U);
  TS_ASSERT_TRUE   (sut.HoldValue());
  TS_ASSERT_EQUALS (sut.Mismatches(),        0U);
  TS_ASSERT_EQUALS (sut.PendingCount(),      0U);
  TS_ASSERT_FALSE  (sut.IsPendingForRead());
  TS_ASSERT_FALSE  (sut.IsPending());
  TS_ASSERT_FALSE  (sut.MustCheckExpected());
  TS_ASSERT_EQUALS (sut.BypassSequence(),    newValue);
  TS_ASSERT_EQUALS (sut.NextToSut(),         newValue);
  TS_ASSERT_EQUALS (sut.LastToSut(),         initial);
  TS_ASSERT_EQUALS (sut.ExpectedFromSut(),   expected);
  TS_ASSERT_EQUALS (sut.LastFromSut(),       gotValue);
  TS_ASSERT_EQUALS (sut.LastReadFromSut(),   gotValue);
  TS_ASSERT_EQUALS (sut.LastCompareResult(), BinaryVector::CreateFromString("0x2D30D9"));
}






//===========================================================================
// End of UT_Register.cpp
//===========================================================================
