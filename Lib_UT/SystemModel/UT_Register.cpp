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
} // End of unnamed namespace

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
  TS_ASSERT_EQUALS (sut.TypeName(), "Register");
  TS_ASSERT_FALSE  (sut.HoldValue());
  TS_ASSERT_FALSE  (sut.MustCheckExpected());
  TS_ASSERT_EQUALS (sut.Mismatches(),      0U);
  TS_ASSERT_EQUALS (sut.BitsCount(),       9U);
  TS_ASSERT_EQUALS (sut.BypassSequence(),  bypassSequence);
  TS_ASSERT_EQUALS (sut.NextToSut(),       bypassSequence);
  TS_ASSERT_EQUALS (sut.LastToSut(),       bypassSequence);
  TS_ASSERT_EQUALS (sut.ExpectedFromSut(), bypassSequence);
  TS_ASSERT_EQUALS (sut.LastFromSut(),     bypassSequence);
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
  TS_ASSERT_EQUALS (sut.ExpectedFromSut(), newValue);
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

//! Checks Register::SetFromSut() with a value different from expected one and check disabled
//!
void UT_Register::test_SetFromSut_DifferingFromExpected_WithoutCheck ()
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
  TS_ASSERT_EQUALS (sut.LastFromSut(), newValue);
  TS_ASSERT_EQUALS (sut.Mismatches(), 0);
}

//! Checks Register::SetFromSut() with a value different from expected one and check enabled
//!
void UT_Register::test_SetFromSut_DifferingFromExpected_WithCheck ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:0");
  const auto expected = BinaryVector::CreateFromBinaryString("1110_0111:1");

  Register sut("Reg", initial);
  sut.SetExpectedFromSut(expected);
  sut.SetCheckExpected(true);

  // ---------------- Exercise
  //
  sut.SetFromSut(newValue);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.MustCheckExpected());
  TS_ASSERT_EQUALS (sut.LastFromSut(), newValue);
  TS_ASSERT_EQUALS (sut.Mismatches(), 1);
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

//===========================================================================
// End of UT_Register.cpp
//===========================================================================
