//===========================================================================
//                           UT_BinaryVector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_BinaryVector.cpp
//!
//! Implements test fixture for testing BinaryVector
//!
//===========================================================================

#include "UT_BinaryVector.hpp"
#include "ScanVectors.hpp"

#include <cxxtest/ValueTraits.h>
#include <vector>
#include <memory>
#include <tuple>

using std::vector;
using std::tuple;
using std::make_tuple;
//+using std::pair;
//+using std::make_pair;
using std::initializer_list;

using mast::BinaryVector;



//! Checks BinaryVector defaut constructor
//!
void UT_BinaryVector::test_Constructor_Default ()
{
  // ---------------- Exercise
  //
  BinaryVector sut;

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS  (sut.BitCount(),   0);
  TS_ASSERT_EQUALS  (sut.BytesCount(), 0);
  TS_ASSERT_NULLPTR (sut.Data());
}


//! Checks BinaryVector copyt constructor when the source BinaryVector is empty
//!
void UT_BinaryVector::test_Constructor_Copy_When_SrcIsEmpty ()
{
  // ---------------- Setup
  //
  const BinaryVector srce;

  // ---------------- Exercise
  //
  BinaryVector sut(srce);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS  (sut.BitCount(),   0);
  TS_ASSERT_EQUALS  (sut.BytesCount(), 0);
  TS_ASSERT_NULLPTR (sut.Data());
}

//! Checks BinaryVector move constructor when the source BinaryVector is empty
//!
void UT_BinaryVector::test_Constructor_Move_When_SrcIsEmpty ()
{
  // ---------------- Setup
  //
  BinaryVector srce;

  // ---------------- Exercise
  //
  BinaryVector sut(std::move(srce));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS  (sut.BitCount(),   0);
  TS_ASSERT_EQUALS  (sut.BytesCount(), 0);
  TS_ASSERT_NULLPTR (sut.Data());
}


//! Checks Append when sut is empty and adding 8 bits from uint8_t
//!
void UT_BinaryVector::test_Append_8_bits_When_Empty ()
{
  // ---------------- Setup
  //
  const uint8_t value = 0xA5;
  BinaryVector sut;

  // ---------------- Exercise
  //
  sut.Append(value);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS      (sut.BitCount(),   8);
  TS_ASSERT_EQUALS      (sut.BytesCount(), 1);

  CxxTest::setAbortTestOnFail(true);
  const uint8_t*         pData = sut.Data();

  TS_ASSERT_NOT_NULLPTR (pData);
  TS_ASSERT_EQUALS      (pData[0], 0xA5);
}

//! Checks Append when sut is empty and adding 16 bits from uint16_t
//!
void UT_BinaryVector::test_Append_16_bits_When_Empty ()
{
  // ---------------- Setup
  //
  const uint16_t value = 0xFACE;
  BinaryVector  sut;

  // ---------------- Exercise
  //
  sut.Append(value);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS      (sut.BitCount(),   16);
  TS_ASSERT_EQUALS      (sut.BytesCount(), 2);

  CxxTest::setAbortTestOnFail(true);
  const uint8_t*        pData = sut.Data();

  TS_ASSERT_NOT_NULLPTR (pData);
  TS_ASSERT_EQUALS      (pData[0], 0xFA);
  TS_ASSERT_EQUALS      (pData[1], 0xCE);
}


//! Checks Append when sut is empty and adding 32 bits from uint32_t
//!
void UT_BinaryVector::test_Append_32_bits_When_Empty ()
{
  // ---------------- Setup
  //
  const uint32_t value = 0xFACEDEAD;
  BinaryVector  sut;

  // ---------------- Exercise
  //
  sut.Append(value);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS      (sut.BitCount(),  32);
  TS_ASSERT_EQUALS      (sut.BytesCount(), 4);

  CxxTest::setAbortTestOnFail(true);
  const uint8_t*        pData = sut.Data();

  TS_ASSERT_NOT_NULLPTR (pData);
  TS_ASSERT_EQUALS      (pData[0], 0xFA);
  TS_ASSERT_EQUALS      (pData[1], 0xCE);
  TS_ASSERT_EQUALS      (pData[2], 0xDE);
  TS_ASSERT_EQUALS      (pData[3], 0xAD);
}


//! Checks Append when sut is empty and adding 64 bits from uint64_t
//!
void UT_BinaryVector::test_Append_64_bits_When_Empty ()
{
  // ---------------- Setup
  //
  const uint64_t value = 0xFACEDEAD'BEEF0080;
  BinaryVector  sut;

  // ---------------- Exercise
  //
  sut.Append(value);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS      (sut.BitCount(),  64);
  TS_ASSERT_EQUALS      (sut.BytesCount(), 8);

  CxxTest::setAbortTestOnFail(true);
  const uint8_t*        pData = sut.Data();

  TS_ASSERT_NOT_NULLPTR (pData);
  TS_ASSERT_EQUALS      (pData[0], 0xFA);
  TS_ASSERT_EQUALS      (pData[1], 0xCE);
  TS_ASSERT_EQUALS      (pData[2], 0xDE);
  TS_ASSERT_EQUALS      (pData[3], 0xAD);
  TS_ASSERT_EQUALS      (pData[4], 0xBE);
  TS_ASSERT_EQUALS      (pData[5], 0xEF);
  TS_ASSERT_EQUALS      (pData[6], 0x00);
  TS_ASSERT_EQUALS      (pData[7], 0x80);
}


//! Checks Append when sut is empty and adding 8 bits from uint8_t
//!
void UT_BinaryVector::test_Append_8_bits_When_NotEmpty ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& inputs, const auto& expectedContent)
  {
    // ---------------- Check parameters
    //
    CxxTest::setAbortTestOnFail(true);
    TS_ASSERT_EQUALS (inputs.size(), expectedContent.size());

    // ---------------- Setup
    //
    BinaryVector sut;
    auto         expectedBitsCount  = 0;
    auto         expectedBytesCount = 0;

    for (uint8_t input : inputs)
    {
      // ---------------- Exercise
      //
      sut.Append(input);

      // ---------------- Verify
      //
      expectedBitsCount  += 8;
      expectedBytesCount += 1;

      TS_ASSERT_EQUALS      (sut.BitCount(),   expectedBitsCount);
      TS_ASSERT_EQUALS      (sut.BytesCount(), expectedBytesCount);

      const uint8_t* pData = sut.Data();

      TS_ASSERT_GREATER_THAN_EQUALS (expectedContent.size(), expectedBytesCount);
      TS_ASSERT_NOT_NULLPTR (pData);
      for (int ii = 0 ; ii < expectedBytesCount ; ++ii)
      {
        TS_ASSERT_EQUALS (pData[ii], expectedContent[ii]);
      }
    }
  };

  const vector<vector<uint8_t>> data =
  {
    vector<uint8_t>({0x11, 0x22}),
    vector<uint8_t>({0x11, 0x22, 0x33}),
    vector<uint8_t>({0x11, 0x22, 0x33, 0x44}),
    vector<uint8_t>({0x11, 0x22, 0x33, 0x44, 0x55}),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data, data);
}


//! Checks Append when sut is empty and adding 16 bits from uint16_t
//!
void UT_BinaryVector::test_Append_16_bits_When_NotEmpty ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const vector<uint16_t>& inputs, const vector<uint8_t>& expectedContent)
  {
    // ---------------- Check parameters
    //
    CxxTest::setAbortTestOnFail(true);
    TS_ASSERT_EQUALS (inputs.size() * sizeof(uint16_t), expectedContent.size());

    // ---------------- Setup
    //
    BinaryVector sut;
    auto         expectedBitsCount  = 0;
    auto         expectedBytesCount = 0;

    for (uint16_t input : inputs)
    {
      // ---------------- Exercise
      //
      sut.Append(input);

      // ---------------- Verify
      //
      expectedBitsCount  += 16;
      expectedBytesCount += sizeof(uint16_t);

      TS_ASSERT_EQUALS (sut.BitCount(),   expectedBitsCount);
      TS_ASSERT_EQUALS (sut.BytesCount(), expectedBytesCount);

      const uint8_t* pData = sut.Data();

      TS_ASSERT_GREATER_THAN_EQUALS (expectedContent.size(), expectedBytesCount);
      TS_ASSERT_NOT_NULLPTR (pData);
      for (int ii = 0 ; ii < expectedBytesCount ; ++ii)
      {
        TS_ASSERT_EQUALS (pData[ii], expectedContent[ii]);
      }
    }
  };

  const auto inputs =
  {
    vector<uint16_t>({0x1122}),
    vector<uint16_t>({0x1122, 0x3344}),
    vector<uint16_t>({0x1122, 0x3344, 0x5566}),
    vector<uint16_t>({0x1122, 0x3344, 0x5566, 0xBAFE}),
  };

  const auto expected =
  {
    vector<uint8_t>({0x11, 0x22}),
    vector<uint8_t>({0x11, 0x22, 0x33, 0x44}),
    vector<uint8_t>({0x11, 0x22, 0x33, 0x44, 0x55, 0x66}),
    vector<uint8_t>({0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0xBA, 0xFE}),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs, expected);
}


//! Checks Append when sut is empty and adding 32 bits from uint32_t
//!
void UT_BinaryVector::test_Append_32_bits_When_NotEmpty ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const vector<uint32_t>& inputs, const vector<uint8_t>& expectedContent)
  {
    // ---------------- Check parameters
    //
    CxxTest::setAbortTestOnFail(true);
    TS_ASSERT_EQUALS (inputs.size() * sizeof(uint32_t), expectedContent.size());

    // ---------------- Setup
    //
    BinaryVector sut;
    auto         expectedBitsCount  = 0;
    auto         expectedBytesCount = 0;

    for (uint32_t input : inputs)
    {
      // ---------------- Exercise
      //
      sut.Append(input);

      // ---------------- Verify
      //
      expectedBitsCount  += 32;
      expectedBytesCount += sizeof(uint32_t);

      TS_ASSERT_EQUALS (sut.BitCount(),   expectedBitsCount);
      TS_ASSERT_EQUALS (sut.BytesCount(), expectedBytesCount);

      const uint8_t* pData = sut.Data();

      TS_ASSERT_GREATER_THAN_EQUALS (expectedContent.size(), expectedBytesCount);
      TS_ASSERT_NOT_NULLPTR (pData);
      for (int ii = 0 ; ii < expectedBytesCount ; ++ii)
      {
        TS_ASSERT_EQUALS (pData[ii], expectedContent[ii]);
      }
    }
  };

  const auto inputs =
  {
    vector<uint32_t>({0x11223344}),
    vector<uint32_t>({0x11223344, 0x55667788}),
    vector<uint32_t>({0x11223344, 0x55667788, 0x99AABBCC}),
  };

  const auto expected =
  {
    vector<uint8_t>({0x11, 0x22, 0x33, 0x44}),
    vector<uint8_t>({0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}),
    vector<uint8_t>({0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC}),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs, expected);
}


//! Checks Append when sut is empty and adding 64 bits from uint64_t
//!
void UT_BinaryVector::test_Append_64_bits_When_NotEmpty ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const vector<uint64_t>& inputs, const vector<uint8_t>& expectedContent)
  {
    // ---------------- Check parameters
    //
    CxxTest::setAbortTestOnFail(true);
    TS_ASSERT_EQUALS (inputs.size() * sizeof(uint64_t), expectedContent.size());

    // ---------------- Setup
    //
    BinaryVector sut;
    auto         expectedBitsCount  = 0;
    auto         expectedBytesCount = 0;

    for (uint64_t input : inputs)
    {
      // ---------------- Exercise
      //
      sut.Append(input);

      // ---------------- Verify
      //
      expectedBitsCount  += 64;
      expectedBytesCount += sizeof(uint64_t);

      TS_ASSERT_EQUALS (sut.BitCount(),   expectedBitsCount);
      TS_ASSERT_EQUALS (sut.BytesCount(), expectedBytesCount);

      const uint8_t* pData = sut.Data();

      TS_ASSERT_GREATER_THAN_EQUALS (expectedContent.size(), expectedBytesCount);
      TS_ASSERT_NOT_NULLPTR (pData);
      for (int ii = 0 ; ii < expectedBytesCount ; ++ii)
      {
        TS_ASSERT_EQUALS (pData[ii], expectedContent[ii]);
      }
    }
  };

  const auto inputs =
  {
    vector<uint64_t>({0x11223344'55667788}),
    vector<uint64_t>({0x11223344'55667788, 0x99AABBCC'DDEEFF00}),
  };

  const auto expected =
  {
    vector<uint8_t>({0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}),
    vector<uint8_t>({0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00}),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs, expected);
}


//! Checks Append when sut is empty and adding from 1 to 7 bits from uint8_t
//!
void UT_BinaryVector::test_Append_1_to_7_bits_Empty ()
{
  // ---------------- DDT Setup
  //
  using TInput    = tuple<uint8_t,  uint8_t>; // Value, Number of bits (taken from LSB)
  using TExpected = tuple<uint32_t, uint32_t, vector<uint8_t>>; // Expected bits count, bytes count, bytes

  auto checker = [](const auto& input, const auto& expected)
  {
    // ---------------- Check parameters
    //
    CxxTest::setAbortTestOnFail(true);

    // ---------------- Setup
    //
    auto value        = std::get<0>(input);
    auto numberOfBits = std::get<1>(input);
    BinaryVector sut;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(value, numberOfBits));

    // ---------------- Verify
    //
    const auto  expectedBitsCount  = std::get<0>(expected);
    const auto  expectedBytesCount = std::get<1>(expected);
    const auto& expectedContent    = std::get<2>(expected);

    TS_ASSERT_EQUALS (sut.BitCount(),   expectedBitsCount);
    TS_ASSERT_EQUALS (sut.BytesCount(), expectedBytesCount);

    const uint8_t* pData = sut.Data();

    TS_ASSERT_GREATER_THAN_EQUALS (expectedContent.size(), expectedBytesCount);
    TS_ASSERT_NOT_NULLPTR (pData);
    for (int ii = 0 ; ii < expectedBytesCount ; ++ii)
    {
      TS_ASSERT_EQUALS (pData[ii], expectedContent[ii]);
    }
  };

  const vector<TInput> inputs =
  {
    TInput(0x0, 1),
    TInput(0x1, 1),
    TInput(0x2, 2),
    TInput(0x3, 2),
    TInput(0x3, 4),
  };

  const vector<TExpected> expected =
  {
    TExpected(1, 1, {0b0000'0000}),
    TExpected(1, 1, {0b1000'0000}),
    TExpected(2, 1, {0b0100'0000}),
    TExpected(2, 1, {0b1100'0000}),
    TExpected(4, 1, {0b0011'0000}),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs, expected);
}



//! @todo [JFC]-[April/25/2016]: Remove "No_test_yet_for_Guard" method when all tests are implemented
//!
void UT_BinaryVector::test_No_test_yet_for_Guard ()
{
  TS_WARN ("No test yet for: append with different width and bits count");
}




//===========================================================================
// End of UT_BinaryVector.cpp
//===========================================================================
