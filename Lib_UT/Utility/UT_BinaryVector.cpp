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

#include <cxxtest/StdValueTraits.h>
#include <vector>
#include <memory>
#include <tuple>
#include <sstream>
#include <experimental/string_view>

using std::vector;
using std::tuple;
using std::make_tuple;
using std::initializer_list;
using std::ostringstream;
using std::experimental::string_view;

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
  TS_ASSERT_EQUALS  (sut.BitsCount(),   0);
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
  TS_ASSERT_EQUALS  (sut.BitsCount(),   0);
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
  TS_ASSERT_EQUALS  (sut.BitsCount(),   0);
  TS_ASSERT_EQUALS  (sut.BytesCount(), 0);
  TS_ASSERT_NULLPTR (sut.Data());
}



//! Checks BinaryVector::CreateFromBinaryString()
//!
void UT_BinaryVector::test_CreateFromBinaryString ()
{
  // ---------------- DDT Setup
  //
  using TExpected = tuple<uint32_t, uint32_t, vector<uint8_t>>; // Expected bits count, bytes count and bytes

  auto checker = [](string_view bits, const auto& expected)
  {
    // ---------------- Exercise
    //
    auto sut = BinaryVector::CreateFromBinaryString(bits);

    // ---------------- Verify
    //
    const auto  expectedBitsCount  = std::get<0>(expected);
    const auto  expectedBytesCount = std::get<1>(expected);
    const auto& expectedContent    = std::get<2>(expected);

    TS_ASSERT_EQUALS (sut.BitsCount(),   expectedBitsCount);
    TS_ASSERT_EQUALS (sut.BytesCount(), expectedBytesCount);

    if (expectedBytesCount != 0)
    {
      const uint8_t* pData = sut.Data();

      TS_ASSERT_GREATER_THAN_EQUALS (expectedContent.size(), expectedBytesCount);

      CxxTest::setAbortTestOnFail(true);
      TS_ASSERT_NOT_NULLPTR (pData);
      ostringstream os;
      for (int ii = 0 ; ii < expectedBytesCount ; ++ii)
      {
        os.str("");
        os << "pData[" << ii << "]";
        auto msg = os.str().c_str();
        TSM_ASSERT_EQUALS (msg, pData[ii], expectedContent[ii]);
      }
    }
  };

  auto inputs =
  {
    "",                  // 00
    " ",                 // 01
    " - ",               // 02
    " -:_,\t'",          // 03
    "10 -:_,\t'01",      // 04
    "01",                // 05
    "10",                // 06
    "1011",              // 07
    "111000001",         // 08
    "01100101",          // 09
    "10011000  0001",    // 10
    "10001011, 10111",   // 11
    "01001010'1001",     // 12
    "10010100 ,1001010", // 13
    "01111011\t101",     // 14
    "1001_0110_1100",    // 15
    "1001-0110-1100",    // 16
    "1001:0110:1100",    // 17
  };

  const vector<TExpected> expected =
  {     // Bits, Bytes, Values
    TExpected(0,  0, {}),                        // 00
    TExpected(0,  0, {}),                        // 01
    TExpected(0,  0, {}),                        // 02
    TExpected(0,  0, {}),                        // 03
    TExpected(4,  1, {0b10010000}),              // 04
    TExpected(2,  1, {0b01000000}),              // 05
    TExpected(2,  1, {0b10000000}),              // 06
    TExpected(4,  1, {0b10110000}),              // 07
    TExpected(9,  2, {0b11100000, 0b10000000}),  // 08
    TExpected(8,  1, {0b01100101}),              // 09
    TExpected(12, 2, {0b10011000, 0b00010000}),  // 10
    TExpected(13, 2, {0b10001011, 0b10111000}),  // 11
    TExpected(12, 2, {0b01001010, 0b10010000}),  // 12
    TExpected(15, 2, {0b10010100, 0b10010100}),  // 13
    TExpected(11, 2, {0b01111011, 0b10100000}),  // 14
    TExpected(12, 2, {0b10010110, 0b11000000}),  // 15
    TExpected(12, 2, {0b10010110, 0b11000000}),  // 16
    TExpected(12, 2, {0b10010110, 0b11000000}),  // 17
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs, expected);
}


//! Checks BinaryVector::CreateFromBinaryString()
//!
//! @note Suppose that CreateFromBinaryString is working properly
//!
void UT_BinaryVector::test_CreateFromHexString ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](string_view hexBits, string_view expectedBinBits)
  {
    // ---------------- Exercise
    //
    auto sut = BinaryVector::CreateFromHexString(hexBits);

    // ---------------- Verify
    //
    const auto expectedBinaryVector = BinaryVector::CreateFromBinaryString(expectedBinBits);

    uint32_t expectedBitsCount  = expectedBinaryVector.BitsCount();
    uint32_t expectedBytesCount = expectedBinaryVector.BytesCount();

    TS_ASSERT_EQUALS (sut.BitsCount(),  expectedBitsCount);
    TS_ASSERT_EQUALS (sut.BytesCount(), expectedBytesCount);

    if (expectedBytesCount != 0)
    {
      const uint8_t* pSutData      = sut.Data();
      const uint8_t* pExpectedData = expectedBinaryVector.Data();

      CxxTest::setAbortTestOnFail(true);
      TS_ASSERT_NOT_NULLPTR (pSutData);
      TS_ASSERT_NOT_NULLPTR (pExpectedData);

      ostringstream os;
      for (uint32_t ii = 0 ; ii < expectedBytesCount ; ++ii)
      {
        os.str("");
        os << "pSutData[" << ii << "]";
        auto msg = os.str().c_str();
        TSM_ASSERT_EQUALS (msg, pSutData[ii], pExpectedData[ii]);
      }
    }
  };

  auto inputs =
  {
    "",              // 00
    " ",             // 01
    ":",             // 02
    ",",             // 03
    " -:_,\t'",      // 04
    "Be -:_,\t'ef",  // 05
    "01",            // 06
    "10",            // 07
    "0123456789",    // 08
    "abcdef",        // 09
    "ABCDEF",        // 10
    "abcdef_123",    // 11
    "ABCDEF_456",    // 12
    "F-A",           // 13
    "'BAD",          // 14
  };

  auto expected =
  {
    "",                                                  // 00
    "",                                                  // 01
    "",                                                  // 02
    "",                                                  // 03
    "",                                                  // 04
    "1011_1110:1110_1111",                               // 05
    "0000_0001",                                         // 06
    "0001_0000",                                         // 07
    "0000_0001-0010_0011-0100_0101-0110_0111-1000_1001", // 08
    "1010_1011:1100_1101:1110_1111",                     // 09
    "1010_1011:1100_1101:1110_1111",                     // 10
    "1010_1011:1100_1101:1110_1111:0001_0010:0011",      // 11
    "1010_1011:1100_1101:1110_1111:0100_0101:0110",      // 12
    "1111_1010",                                         // 13
    "1011_1010:1101",                                    // 14
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs, expected);
}


//! Checks BinaryVector::operator== when comparing with same instance
//!
void UT_BinaryVector::test_operator_eq_With_Self ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](string_view bits)
  {
    // ---------------- Setup
    //
    auto sut = BinaryVector::CreateFromBinaryString(bits);

    // ---------------- Exercise
    //
    bool areEqual = sut == sut ;

    // ---------------- Verify
    //
    TS_ASSERT_TRUE (areEqual);
  };

  auto inputs =
  {
    "",            // 00
    "0",           // 01
    "1",           // 02
    "01",          // 03
    "1110_0000:1", // 04
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs);
}

//! Checks BinaryVector::operator== when both are equal
//!
void UT_BinaryVector::test_operator_eq_When_Equal ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](string_view bits)
  {
    // ---------------- Setup
    //
    auto sut = BinaryVector::CreateFromBinaryString(bits);
    auto rhs = BinaryVector::CreateFromBinaryString(bits);

    // ---------------- Exercise
    //
    bool areEqual = sut == rhs;

    // ---------------- Verify
    //
    TS_ASSERT_TRUE (areEqual);
  };

  auto inputs =
  {
    "",            // 00
    "0",           // 01
    "1",           // 02
    "01",          // 03
    "10",          // 04
    "1011",        // 05
    "0110_0101",   // 06
    "1110_0000:1", // 07
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs);
}


//! Checks BinaryVector::operator== when both are not equal
//!
void UT_BinaryVector::test_operator_eq_When_NotEqual ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](string_view lhsBits, string_view rhsBits)
  {
    // ---------------- Setup
    //
    auto sut = BinaryVector::CreateFromBinaryString(lhsBits);
    auto rhs = BinaryVector::CreateFromBinaryString(rhsBits);

    // ---------------- Exercise
    //
    bool areEqual = sut == rhs;

    // ---------------- Verify
    //
    TS_ASSERT_FALSE (areEqual);
  };

  auto sutBits =
  {
    "",            // 00
    "0",           // 01
    "1",           // 02
    "01",          // 03
    "10",          // 04
    "1011",        // 05
    "0110_0101",   // 06
    "1110_0000:1", // 07
  };

  auto rhsBits =
  {
    "0",           // 00
    "1",           // 01
    "11",          // 02
    "001",         // 03
    "",            // 04
    "1010",        // 05
    "0110_010",    // 06
    "1110_0000:0", // 07
  };


  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, sutBits, rhsBits);
}

//! Checks BinaryVector::operator!= when comparing with same instance
//!
void UT_BinaryVector::test_operator_neq_With_Self ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](string_view bits)
  {
    // ---------------- Setup
    //
    auto sut = BinaryVector::CreateFromBinaryString(bits);

    // ---------------- Exercise
    //
    bool areNotEqual = sut != sut ;

    // ---------------- Verify
    //
    TS_ASSERT_FALSE (areNotEqual);
  };

  auto inputs =
  {
    "",            // 00
    "0",           // 01
    "1",           // 02
    "01",          // 03
    "1110_0000:1", // 04
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs);
}


//! Checks BinaryVector::operator!= when both are equal
//!
void UT_BinaryVector::test_operator_neq_When_Equal ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](string_view bits)
  {
    // ---------------- Setup
    //
    auto sut = BinaryVector::CreateFromBinaryString(bits);
    auto rhs = BinaryVector::CreateFromBinaryString(bits);

    // ---------------- Exercise
    //
    bool areNotEqual = sut != rhs;

    // ---------------- Verify
    //
    TS_ASSERT_FALSE (areNotEqual);
  };

  auto inputs =
  {
    "",            // 00
    "0",           // 01
    "1",           // 02
    "01",          // 03
    "10",          // 04
    "1011",        // 05
    "0110_0101",   // 06
    "1110_0000:1", // 07
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs);
}


//! Checks BinaryVector::operator!= when both are not equal
//!
void UT_BinaryVector::test_operator_neq_When_NotEqual ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](string_view lhsBits, string_view rhsBits)
  {
    // ---------------- Setup
    //
    auto sut = BinaryVector::CreateFromBinaryString(lhsBits);
    auto rhs = BinaryVector::CreateFromBinaryString(rhsBits);

    // ---------------- Exercise
    //
    bool areNotEqual = sut != rhs;

    // ---------------- Verify
    //
    TS_ASSERT_TRUE (areNotEqual);
  };

  auto sutBits =
  {
    "",            // 00
    "0",           // 01
    "1",           // 02
    "01",          // 03
    "10",          // 04
    "1011",        // 05
    "0110_0101",   // 06
    "1110_0000:1", // 07
  };

  auto rhsBits =
  {
    "0",           // 00
    "1",           // 01
    "11",          // 02
    "001",         // 03
    "",            // 04
    "1010",        // 05
    "0110_010",    // 06
    "1110_0000:0", // 07
  };


  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, sutBits, rhsBits);
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
  TS_ASSERT_EQUALS      (sut.BitsCount(),   8);
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
  TS_ASSERT_EQUALS      (sut.BitsCount(),   16);
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
  TS_ASSERT_EQUALS      (sut.BitsCount(),  32);
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
  TS_ASSERT_EQUALS      (sut.BitsCount(),  64);
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

      TS_ASSERT_EQUALS      (sut.BitsCount(),   expectedBitsCount);
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

      TS_ASSERT_EQUALS (sut.BitsCount(),   expectedBitsCount);
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

      TS_ASSERT_EQUALS (sut.BitsCount(),   expectedBitsCount);
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

      TS_ASSERT_EQUALS (sut.BitsCount(),   expectedBitsCount);
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


//! Checks Append when sut is empty and adding from 1 to 8 bits from uint8_t
//!
//! @note Each time a new BinaryVector is used
void UT_BinaryVector::test_Append_1_to_8_bits_When_Empty ()
{
  // ---------------- DDT Setup
  //
  using TInput    = tuple<uint8_t,  uint8_t>; // Value, Number of bits (taken from LSB)
  using TExpected = tuple<uint32_t, uint32_t, vector<uint8_t>>; // Expected bits count, bytes count, bytes

  auto checker = [](const auto& input, const auto& expected)
  {
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

    TS_ASSERT_EQUALS (sut.BitsCount(),   expectedBitsCount);
    TS_ASSERT_EQUALS (sut.BytesCount(), expectedBytesCount);

    const uint8_t* pData = sut.Data();

    TS_ASSERT_GREATER_THAN_EQUALS (expectedContent.size(), expectedBytesCount);
    CxxTest::setAbortTestOnFail(true);
    TS_ASSERT_NOT_NULLPTR (pData);
    for (int ii = 0 ; ii < expectedBytesCount ; ++ii)
    {
      TS_ASSERT_EQUALS (pData[ii], expectedContent[ii]);
    }
  };

  const vector<TInput> inputs =
  {   // Value, bits
    TInput(0x00, 1),    // 00
    TInput(0x01, 1),    // 01
    TInput(0x02, 2),    // 02
    TInput(0x03, 2),    // 03
    TInput(0x03, 3),    // 04
    TInput(0x09, 4),    // 05
    TInput(0x11, 5),    // 06
    TInput(0x12, 6),    // 07
    TInput(0x4A, 7),    // 08
    TInput(0x7B, 8),    // 09
  };

  const vector<TExpected> expected =
  {     // Bits, Bytes, Values
    TExpected(1, 1, {0b00000000}),   // 00
    TExpected(1, 1, {0b10000000}),   // 01
    TExpected(2, 1, {0b10000000}),   // 02
    TExpected(2, 1, {0b11000000}),   // 03
    TExpected(3, 1, {0b01100000}),   // 04
    TExpected(4, 1, {0b10010000}),   // 05
    TExpected(5, 1, {0b10001000}),   // 06
    TExpected(6, 1, {0b01001000}),   // 07
    TExpected(7, 1, {0b10010100}),   // 08
    TExpected(8, 1, {0b01111011}),   // 09
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs, expected);
}

//! Checks Append when sut is not empty (from 1 to 8 bits) and adding from 1 to 8 bits from uint8_t
//!
//! @note Each time a new BinaryVector is used
void UT_BinaryVector::test_Append_1_to_8_bits_When_NotEmpty ()
{
  // ---------------- DDT Setup
  //
  using TInput    = tuple<uint8_t, uint8_t, uint8_t, uint8_t>;  // First value and number of bits, second value and number of bits
  using TExpected = tuple<uint32_t, uint32_t, vector<uint8_t>>; // Expected bits count, bytes count and bytes

  auto checker = [](const auto& input, const auto& expected)
  {
    // ---------------- Setup
    //
    auto value_1        = std::get<0>(input);
    auto numberOfBits_1 = std::get<1>(input);
    auto value_2        = std::get<2>(input);
    auto numberOfBits_2 = std::get<3>(input);

    BinaryVector sut;
    sut.Append(value_1, numberOfBits_1);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(value_2, numberOfBits_2));

    // ---------------- Verify
    //
    const auto  expectedBitsCount  = std::get<0>(expected);
    const auto  expectedBytesCount = std::get<1>(expected);
    const auto& expectedContent    = std::get<2>(expected);

    TS_ASSERT_EQUALS (sut.BitsCount(),  expectedBitsCount);
    TS_ASSERT_EQUALS (sut.BytesCount(), expectedBytesCount);

    const uint8_t* pData = sut.Data();

    TS_ASSERT_GREATER_THAN_EQUALS (expectedContent.size(), expectedBytesCount);
    CxxTest::setAbortTestOnFail(true);
    TS_ASSERT_NOT_NULLPTR (pData);
    ostringstream os;
    for (int ii = 0 ; ii < expectedBytesCount ; ++ii)
    {
      os.str("");
      os << "pData[" << ii << "]";
      auto msg = os.str().c_str();
      TSM_ASSERT_EQUALS (msg, pData[ii], expectedContent[ii]);
    }
  };

  const vector<TInput> inputs =
  {   // Value, bits, value, bits
    TInput(0x00, 1, 0x01, 1),    // 00
    TInput(0x01, 1, 0x00, 1),    // 01
    TInput(0x02, 2, 0x03, 2),    // 02
    TInput(0x03, 2, 0x41, 7),    // 03
    TInput(0x03, 3, 0x05, 5),    // 04
    TInput(0x09, 4, 0x81, 8),    // 05
    TInput(0x11, 5, 0x77, 8),    // 06
    TInput(0x12, 6, 0x29, 6),    // 07
    TInput(0x4A, 7, 0x4A, 8),    // 08
    TInput(0x7B, 8, 0x05, 3),    // 09
  };

  const vector<TExpected> expected =
  {     // Bits, Bytes, Values
    TExpected(2,  1, {0b01000000}),              // 00
    TExpected(2,  1, {0b10000000}),              // 01
    TExpected(4,  1, {0b10110000}),              // 02
    TExpected(9,  2, {0b11100000, 0b10000000}),  // 03
    TExpected(8,  1, {0b01100101}),              // 04
    TExpected(12, 2, {0b10011000, 0b00010000}),  // 05
    TExpected(13, 2, {0b10001011, 0b10111000}),  // 06
    TExpected(12, 2, {0b01001010, 0b10010000}),  // 07
    TExpected(15, 2, {0b10010100, 0b10010100}),  // 08
    TExpected(11, 2, {0b01111011, 0b10100000}),  // 09
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs, expected);
}

//! Checks Append with another BinaryVector when sut is empty
//!
void UT_BinaryVector::test_Append_Other_When_Empty ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](string_view otherBits)
  {
    // ---------------- Setup
    //
    auto sut   = BinaryVector();
    auto other = BinaryVector::CreateFromBinaryString(otherBits);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(other));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, other);
  };

  auto data =
  {
    "",             // 00
    "0",            // 01
    "1",            // 02
    "01",           // 03
    "10",           // 04
    "101",          // 05
    "1011",         // 06
    "0110_0",       // 07
    "0110_01",      // 08
    "0110_011",     // 09
    "0110_0111",    // 10
    "1110_0000:1",  // 11
    "1110_0000:11", // 12
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks Append with another BinaryVector when sut is not empty
//!
void UT_BinaryVector::test_Append_Other_When_NotEmpty ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    string_view sutBits      = std::get<0>(data);
    string_view otherBits    = std::get<1>(data);
    string_view expectedBits = std::get<2>(data);

    auto sut   = BinaryVector::CreateFromBinaryString(sutBits);
    auto other = BinaryVector::CreateFromBinaryString(otherBits);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(other));

    // ---------------- Verify
    //
    auto expected = BinaryVector::CreateFromBinaryString(expectedBits);

    TS_ASSERT_EQUALS (sut, expected);
  };

  auto data =
  {
    //   Bits: sut,            other,   result
    make_tuple("1",            "", ""), // 00
    make_tuple("0",            "", ""), // 01
    make_tuple("1",            "", ""), // 02
    make_tuple("01",           "", ""), // 03
    make_tuple("10",           "", ""), // 04
    make_tuple("101",          "", ""), // 05
    make_tuple("1011",         "", ""), // 06
    make_tuple("0110_0",       "", ""), // 07
    make_tuple("0110_01",      "", ""), // 08
    make_tuple("0110_011",     "", ""), // 09
    make_tuple("0110_0111",    "", ""), // 10
    make_tuple("1110_0000:1",  "", ""), // 11
    make_tuple("1110_0000:11", "", ""), // 12
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}



//! Checks BinaryVector::Operator<< when the sut is still empty
//!
void UT_BinaryVector::test_Operator_Shift_When_Empty ()
{
  // ---------------- Setup
  //

  // ---------------- Exercise
  //

  // ---------------- Verify
  //
  TS_WARN ("Test not yet implemented");
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
