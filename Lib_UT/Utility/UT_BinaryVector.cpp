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
#include "BinaryVector.hpp"
#include "BinaryVector_Traits.hpp"

#include <vector>
#include <memory>
#include <tuple>
#include <sstream>
#include <stdexcept>
#include <experimental/string_view>

using std::vector;
using std::tuple;
using std::make_tuple;
using std::initializer_list;
using std::ostringstream;
using std::string;
using std::experimental::string_view;

using namespace mast;


//! Checks BinaryVector defaut constructor
//!
void UT_BinaryVector::test_Constructor_Default ()
{
  // ---------------- Exercise
  //
  BinaryVector sut;

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS  (sut.BitsCount(),  0);
  TS_ASSERT_EQUALS  (sut.BytesCount(), 0);
  TS_ASSERT_TRUE    (sut.IsEmpty());
  TS_ASSERT_FALSE   (sut.HasFixedSize());
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
  TS_ASSERT_EQUALS  (sut.BitsCount(),  0);
  TS_ASSERT_EQUALS  (sut.BytesCount(), 0);
  TS_ASSERT_TRUE    (sut.IsEmpty());
  TS_ASSERT_FALSE   (sut.HasFixedSize());
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
  TS_ASSERT_EQUALS  (sut.BitsCount(),  0);
  TS_ASSERT_EQUALS  (sut.BytesCount(), 0);
  TS_ASSERT_TRUE    (sut.IsEmpty());
  TS_ASSERT_FALSE   (sut.HasFixedSize());
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

    TS_ASSERT_EQUALS (sut.BitsCount(),  expectedBitsCount);
    TS_ASSERT_EQUALS (sut.BytesCount(), expectedBytesCount);
    TS_ASSERT_EQUALS (sut.IsEmpty(),    expectedBitsCount == 0);
    TS_ASSERT_FALSE  (sut.HasFixedSize());

    if (expectedBytesCount != 0u)
    {
      const uint8_t* pData = sut.Data();

      TS_ASSERT_GREATER_THAN_EQUALS (expectedContent.size(), expectedBytesCount);

      CxxTest::setAbortTestOnFail(true);
      TS_ASSERT_NOT_NULLPTR (pData);
      ostringstream os;
      for (uint32_t ii = 0 ; ii < expectedBytesCount ; ++ii)
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
    "0b1001:0110:1100",  // 18
    "0B1001:0110:1100",  // 19
    "1001:0110:\\b1100", // 20
    "1001:0110:\\B1100", // 21
    "1001:0110:/b1100",  // 22
    "1001:0110:/B1100",  // 23
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
    TExpected(12, 2, {0b10010110, 0b11000000}),  // 18
    TExpected(12, 2, {0b10010110, 0b11000000}),  // 19
    TExpected(12, 2, {0b10010110, 0b11000000}),  // 20
    TExpected(12, 2, {0b10010110, 0b11000000}),  // 21
    TExpected(12, 2, {0b10010110, 0b11000000}),  // 22
    TExpected(12, 2, {0b10010110, 0b11000000}),  // 23
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs, expected);
}


//! Checks BinaryVector::CreateFromBinaryString() when invalid characters are used
//!
void UT_BinaryVector::test_CreateFromBinaryString_InvalidChars ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](string_view bits)
  {
    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (BinaryVector::CreateFromBinaryString(bits), std::exception);
  };

  auto inputs =
  {
    "b",      // 00
    "B",      // 01
    "01b01",  // 02
    "01201",  // 03
    "01B01",  // 04
    "01/x01", // 05
    "01/X01", // 06
    "10/bx0", // 07
    "10I0",   // 08
    "10i0",   // 09
    "1(00",   // 10
    "100)0",  // 11
    "1[00",   // 12
    "100]1",  // 13
    "@101",   // 14
    "@b101",  // 15
    "#101",   // 16
    "#b101",  // 17
    "10&1",   // 18
    "~101",   // 19
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs);
}


//! Checks BinaryVector::CreateFromHexString()
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

    if (expectedBytesCount != 0u)
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
    "A",             // 15
    "5",             // 16
    "0xCAFE",        // 17
    "0XCAFE",        // 18
    "CA\\xFE",       // 19
    "CA\\XFE",       // 20
    "CA/xFE",        // 21
    "CA/XFE",        // 22
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
    "1010",                                              // 15
    "0101",                                              // 16
    "1100_1010:1111_1110",                               // 17
    "1100_1010:1111_1110",                               // 18
    "1100_1010:1111_1110",                               // 19
    "1100_1010:1111_1110",                               // 20
    "1100_1010:1111_1110",                               // 21
    "1100_1010:1111_1110",                               // 22
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs, expected);
}

//! Checks BinaryVector::CreateFromHexString() when invalid characters are used
//!
void UT_BinaryVector::test_CreateFromHexString_InvalidChars ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](string_view bits)
  {
    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (BinaryVector::CreateFromHexString(bits), std::exception);
  };

  auto inputs =
  {
    "x",      // 00
    "X",      // 01
    "2Ax27",  // 02
    "2AX27",  // 03
    "2A0x27", // 04
    "2A0X27", // 05
    "A2/xx2", // 06
    "A2G2",   // 07
    "A2I2",   // 08
    "A2i2",   // 09
    "A(22",   // 10
    "A22)2",  // 11
    "A[22",   // 12
    "A22]7",  // 13
    "@A27",   // 14
    "@x727",  // 15
    "#A27",   // 16
    "#x72B",  // 17
    "A2&5",   // 18
    "~A2C",   // 19
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs);
}


//! Checks BinaryVector::CreateFromString()
//!
//! @note Suppose that CreateFromString is working properly
//!
void UT_BinaryVector::test_CreateFromString ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto text            = std::get<0>(data);
    auto expectedBinBits = std::get<1>(data);

    // ---------------- Exercise
    //
    auto sut = BinaryVector::CreateFromString(text);

    // ---------------- Verify
    //
    const auto expectedBinaryVector = BinaryVector::CreateFromBinaryString(expectedBinBits);

    uint32_t expectedBitsCount  = expectedBinaryVector.BitsCount();
    uint32_t expectedBytesCount = expectedBinaryVector.BytesCount();

    TS_ASSERT_EQUALS (sut.BitsCount(),  expectedBitsCount);
    TS_ASSERT_EQUALS (sut.BytesCount(), expectedBytesCount);

    if (expectedBytesCount != 0u)
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

  auto data =
  {
    make_tuple(" ",                   ""),                                        // 00
    make_tuple("0b01",                "01"),                                      // 01
    make_tuple("/b101",               "101"),                                     // 02
    make_tuple("0x01234567",          "0000_0001:0010_0011:0100_0101:0110_0111"), // 03
    make_tuple("/x89AbcDeF",          "1000_1001:1010_1011:1100_1101:1110_1111"), // 04
    make_tuple("/xab/b01",            "1010_1011:01"),                            // 05
    make_tuple("/b01/x01",            "01:0000_0001"),                            // 06
    make_tuple("0xBAD\\b101",         "1011_1010_1101:101"),                      // 07
    make_tuple("0XA /B1",             "1010:1"),                                  // 08
    make_tuple("0B1101:/XA",          "1101:1010"),                               // 09
    make_tuple("0xCA/X",              "1100_1010"),                               // 10
    make_tuple("0XFE/b",              "1111_1110"),                               // 11
    make_tuple("  0x5A/xFE  /b1",     "0101_1010:1111_1110:1"),                   // 12
    make_tuple("  0b00010   /x  7",   "0001_0:0111"),                             // 13
    make_tuple("  0b00010   /x  7/",  "0001_0:0111"),                             // 14
    make_tuple("  0b00010   /x  7\\", "0001_0:0111"),                             // 15
    make_tuple("  0b00010   /x  7/b", "0001_0:0111"),                             // 16
    make_tuple("  0b00010   /x  7/x", "0001_0:0111"),                             // 17
    make_tuple("/x89Ab/XcDeF",        "1000_1001:1010_1011:1100_1101:1110_1111"), // 18
    make_tuple("/xab/b01/b11/x2",     "1010_1011:01:11:0010"),                    // 19
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector::CreateFromString when passing invalid characters
//!
void UT_BinaryVector::test_CreateFromString_InvalidChars ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](string_view bits)
  {
    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (BinaryVector::CreateFromString(bits), std::exception);
  };

  auto inputs =
  {
    "x",          // 00
    "X",          // 01
    "x/xAF",      // 02
    "b/b01",      // 03
    "2Ax27",      // 04
    "2AX27",      // 05
    "2A0x27",     // 06
    "2A0X27",     // 07
    "A2/xx2",     // 08
    "/xx2",       // 09
    "/x2x/b01",   // 10
    "/x2/bb01",   // 11
    "/x2/b01b",   // 12
    "/x2/b01x",   // 13
    "/x2AG/b01x", // 14
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs);
}


//! Checks BinaryVector::DataAsBinaryString()
//!
void UT_BinaryVector::test_DataAsBinaryString_Without_NewLine ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    string_view sutBits  = std::get<0>(data);
    string      expected = std::get<1>(data);

    auto sut   = BinaryVector::CreateFromBinaryString(sutBits);

    // ---------------- Exercise
    //
    auto sutAsString = sut.DataAsBinaryString(", ", "-");

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sutAsString, expected);
  };

  auto data =
  {
    //   Bits: sut,            result
    make_tuple("0",                            "0"),                              // 00
    make_tuple("1",                            "1"),                              // 01
    make_tuple("10",                           "10"),                             // 02
    make_tuple("11",                           "11"),                             // 03
    make_tuple("100",                          "100"),                            // 04
    make_tuple("101",                          "101"),                            // 05
    make_tuple("110",                          "110"),                            // 06
    make_tuple("111",                          "111"),                            // 07
    make_tuple("1000",                         "1000"),                           // 08
    make_tuple("1001",                         "1001"),                           // 09
    make_tuple("1010",                         "1010"),                           // 10
    make_tuple("1011",                         "1011"),                           // 11
    make_tuple("1100",                         "1100"),                           // 12
    make_tuple("1101",                         "1101"),                           // 13
    make_tuple("1110",                         "1110"),                           // 14
    make_tuple("1111",                         "1111"),                           // 15
    make_tuple("1011_0",                       "1011-0"),                         // 16
    make_tuple("1011_1",                       "1011-1"),                         // 17
    make_tuple("1011_10",                      "1011-10"),                        // 18
    make_tuple("1011_11",                      "1011-11"),                        // 19
    make_tuple("1011_100",                     "1011-100"),                       // 20
    make_tuple("1011_101",                     "1011-101"),                       // 21
    make_tuple("1011_110",                     "1011-110"),                       // 22
    make_tuple("1011_111",                     "1011-111"),                       // 23
    make_tuple("1011_1000",                    "1011-1000"),                      // 24
    make_tuple("1011_1001",                    "1011-1001"),                      // 25
    make_tuple("1011_1010",                    "1011-1010"),                      // 26
    make_tuple("1011_1011",                    "1011-1011"),                      // 27
    make_tuple("1011_1100",                    "1011-1100"),                      // 28
    make_tuple("1011_1101",                    "1011-1101"),                      // 29
    make_tuple("1011_1110",                    "1011-1110"),                      // 30
    make_tuple("1011_1111",                    "1011-1111"),                      // 31
    make_tuple("1110_0000:1",                  "1110-0000, 1"),                   // 32
    make_tuple("1110_0000:11",                 "1110-0000, 11"),                  // 33
    make_tuple("1110_0000:1100_1011:1010",     "1110-0000, 1100-1011, 1010"),     // 34
    make_tuple("1110_0000:1100_1011:1010_1",   "1110-0000, 1100-1011, 1010-1"),   // 35
    make_tuple("1110_0000:1100_1011:1010_101", "1110-0000, 1100-1011, 1010-101"), // 36
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector::DataAsBinaryString() with nibble and bytes separators and new lines every N bytes
//!
void UT_BinaryVector::test_DataAsBinaryString_With_NewLine ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    string_view sutBits  = std::get<0>(data);
    string      expected = std::get<1>(data);

    auto sut   = BinaryVector::CreateFromBinaryString(sutBits);

    // ---------------- Exercise
    //
    auto sutAsString = sut.DataAsBinaryString(", ", "-", 3, ";");

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sutAsString, expected);
  };

  auto data =
  {
    //   Bits: sut,            result
    make_tuple("0",                                     "0"),                                         // 00
    make_tuple("01",                                    "01"),                                        // 01
    make_tuple("1001",                                  "1001"),                                      // 02
    make_tuple("1011_1",                                "1011-1"),                                    // 03
    make_tuple("0110_01",                               "0110-01"),                                   // 04
    make_tuple("0110_011",                              "0110-011"),                                  // 05
    make_tuple("0110_0111",                             "0110-0111"),                                 // 06
    make_tuple("1110_0000:1",                           "1110-0000, 1"),                              // 07
    make_tuple("1110_0000:1100_1011:1010",              "1110-0000, 1100-1011, 1010"),                // 08
    make_tuple("1110_0000:1100_1011:1010_1010",         "1110-0000, 1100-1011, 1010-1010"),           // 09
    make_tuple("1110_0000:1100_1011:1010_1010:1",       "1110-0000, 1100-1011, 1010-1010;\n1"),       // 10
    make_tuple("1110_0000:1100_1011:1010_1010:1100_01", "1110-0000, 1100-1011, 1010-1010;\n1100-01"), // 11
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector::DataAsBinaryString()
//!
void UT_BinaryVector::test_DataAsBinaryString_Without_Separators ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    string_view sutBits  = std::get<0>(data);
    string      expected = std::get<1>(data);

    auto sut   = BinaryVector::CreateFromBinaryString(sutBits);

    // ---------------- Exercise
    //
    auto sutAsString = sut.DataAsBinaryString("", "");

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sutAsString, expected);
  };

  auto data =
  {
    //   Bits: sut,            result
    make_tuple("0",                            "0"),                       // 00
    make_tuple("1",                            "1"),                       // 01
    make_tuple("11",                           "11"),                      // 02
    make_tuple("01",                           "01"),                      // 03
    make_tuple("1001",                         "1001"),                    // 04
    make_tuple("1011_1",                       "10111"),                   // 05
    make_tuple("1011",                         "1011"),                    // 06
    make_tuple("0110_0",                       "01100"),                   // 07
    make_tuple("0110_01",                      "011001"),                  // 08
    make_tuple("0110_011",                     "0110011"),                 // 09
    make_tuple("0110_0111",                    "01100111"),                // 10
    make_tuple("1110_0000:1",                  "111000001"),               // 11
    make_tuple("1110_0000:11",                 "1110000011"),              // 12
    make_tuple("1110_0000:1100_1011:1010",     "11100000110010111010"),    // 13
    make_tuple("1110_0000:1100_1011:1010_1",   "111000001100101110101"),   // 14
    make_tuple("1110_0000:1100_1011:1010_101", "11100000110010111010101"), // 15
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
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


//! Checks HexVector::DataAsHexString() with 16 and 32 bits separators
//!
void UT_BinaryVector::test_DataAsHexString_Without_NewLine ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    string_view sutBits  = std::get<0>(data);
    string      expected = std::get<1>(data);

    auto sut   = BinaryVector::CreateFromBinaryString(sutBits);

    // ---------------- Exercise
    //
    auto sutAsString = sut.DataAsHexString(", ", "-");

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sutAsString, expected);
  };

  auto data =
  {
    //   Bits: sut,            result
    make_tuple("0000",                                         "0"),            // 00
    make_tuple("0001",                                         "1"),            // 01
    make_tuple("0010",                                         "2"),            // 02
    make_tuple("0011",                                         "3"),            // 03
    make_tuple("0100",                                         "4"),            // 04
    make_tuple("0101",                                         "5"),            // 05
    make_tuple("0110",                                         "6"),            // 06
    make_tuple("0111",                                         "7"),            // 07
    make_tuple("1000",                                         "8"),            // 08
    make_tuple("1001",                                         "9"),            // 09
    make_tuple("1010",                                         "A"),            // 10
    make_tuple("1011",                                         "B"),            // 11
    make_tuple("1100",                                         "C"),            // 12
    make_tuple("1101",                                         "D"),            // 13
    make_tuple("1110",                                         "E"),            // 14
    make_tuple("1111",                                         "F"),            // 15
    make_tuple("0",                                            "0"),            // 16
    make_tuple("1",                                            "8"),            // 17
    make_tuple("10",                                           "8"),            // 18
    make_tuple("11",                                           "C"),            // 19
    make_tuple("100",                                          "8"),            // 20
    make_tuple("101",                                          "A"),            // 21
    make_tuple("110",                                          "C"),            // 22
    make_tuple("111",                                          "E"),            // 23
    make_tuple("1011_0",                                       "B0"),           // 24
    make_tuple("1011_1",                                       "B8"),           // 25
    make_tuple("1011_10",                                      "B8"),           // 26
    make_tuple("1011_11",                                      "BC"),           // 27
    make_tuple("1011_100",                                     "B8"),           // 28
    make_tuple("1011_101",                                     "BA"),           // 29
    make_tuple("1011_110",                                     "BC"),           // 30
    make_tuple("1011_111",                                     "BE"),           // 31
    make_tuple("1011_1000",                                    "B8"),           // 32
    make_tuple("1011_1001",                                    "B9"),           // 33
    make_tuple("1011_1010",                                    "BA"),           // 34
    make_tuple("1011_1011",                                    "BB"),           // 35
    make_tuple("1011_1100",                                    "BC"),           // 36
    make_tuple("1011_1101",                                    "BD"),           // 37
    make_tuple("1011_1110",                                    "BE"),           // 38
    make_tuple("1011_1111",                                    "BF"),           // 39
    make_tuple("1110_0000:1",                                  "E08"),          // 40
    make_tuple("1110_0000:11",                                 "E0C"),          // 41
    make_tuple("1110_0000:1100_1011:1010",                     "E0CB-A"),       // 42
    make_tuple("1110_0000:1100_1011:1010_1",                   "E0CB-A8"),      // 43
    make_tuple("1110_0000:1100_1011:1010_101",                 "E0CB-AA"),      // 44
    make_tuple("1110_0000:1100_1011:1010_1011:1010_0101",      "E0CB-ABA5"),    // 45
    make_tuple("1110_0000:1100_1011:1010_1011:1010_0101:1100", "E0CB-ABA5, C"), // 46
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector::DataAsHexString() with 16 and 32 bits separators and new lines every N bytes
//!
void UT_BinaryVector::test_DataAsHexString_With_NewLine ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    string_view sutBits  = std::get<0>(data);
    string      expected = std::get<1>(data);

    auto sut   = BinaryVector::CreateFromBinaryString(sutBits);

    // ---------------- Exercise
    //
    auto sutAsString = sut.DataAsHexString(", ", "-", 4, ";");

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sutAsString, expected);
  };

  auto data =
  {
    //   Bits: sut,            result
    make_tuple("0",                                                                                 "0"),                         // 00
    make_tuple("01",                                                                                "4"),                         // 01
    make_tuple("1001",                                                                              "9"),                         // 02
    make_tuple("1011_1",                                                                            "B8"),                        // 03
    make_tuple("0110_01",                                                                           "64"),                        // 04
    make_tuple("0110_011",                                                                          "66"),                        // 05
    make_tuple("0110_0111",                                                                         "67"),                        // 06
    make_tuple("1110_0000:1",                                                                       "E08"),                       // 07
    make_tuple("1110_0000:1100_1011:1010",                                                          "E0CB-A"),                    // 08
    make_tuple("1110_0000:1100_1011:1010_1010",                                                     "E0CB-AA"),                   // 09
    make_tuple("1110_0000:1100_1011:1010_1010:1",                                                   "E0CB-AA8"),                  // 10
    make_tuple("1110_0000:1100_1011:1010_1010:1100_01",                                             "E0CB-AAC4"),                 // 11
    make_tuple("1110_0000:1100_1011:1010_1010:1100_0111:1110_0000:1100_1011:1010_1010:1100_0111:1", "E0CB-AAC7;\nE0CB-AAC7;\n8"), // 12
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector::DataAsHexString()
//!
void UT_BinaryVector::test_DataAsHexString_Without_Separators ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    string_view sutBits  = std::get<0>(data);
    string      expected = std::get<1>(data);

    auto sut   = BinaryVector::CreateFromBinaryString(sutBits);

    // ---------------- Exercise
    //
    auto sutAsString = sut.DataAsHexString("", "");

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sutAsString, expected);
  };

  auto data =
  {
    //   Bits: sut,                                           result
    make_tuple("1110_0000:1",                                 "E08"),       // 00
    make_tuple("1110_0000:1100_111",                          "E0CE"),      // 01
    make_tuple("1110_0000:1100_1011:1010",                    "E0CBA"),     // 02
    make_tuple("1110_0000:1100_1011:1010_1",                  "E0CBA8"),    // 03
    make_tuple("1110_0000:1100_1011:1010_0011:0010_0001:000", "E0CBA3210"), // 04
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
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
    "",                           // 00
    "0",                          // 01
    "1",                          // 02
    "01",                         // 03
    "10",                         // 04
    "1011",                       // 05
    "0110_0101",                  // 06
    "1110_0000:1",                // 07
    "1110_0010:10",               // 08
    "1110_0010:1001_1110:1010_1", // 09
  };

  auto rhsBits =
  {
    "0",                          // 00
    "1",                          // 01
    "11",                         // 02
    "001",                        // 03
    "",                           // 04
    "1010",                       // 05
    "0110_010",                   // 06
    "1110_0000:0",                // 07
    "1110_0110:10",               // 08
    "1110_0010:1001_1100:1010_1", // 09
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



//! Checks BinaryVector constructor with a count and pattern
//!
//! @note Suppose that operator== is working properly
void UT_BinaryVector::test_Constructor_With_Count_and_Pattern ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    uint32_t    bitsCount    = std::get<0>(data);
    uint8_t     pattern      = std::get<1>(data);
    string_view expectedBits = std::get<2>(data);

    // ---------------- Exercise
    //
    BinaryVector sut(bitsCount, pattern);

    // ---------------- Verify
    //
    auto expected = BinaryVector::CreateFromBinaryString(expectedBits);

    TS_ASSERT_EQUALS (sut, expected);
  };

  auto data =
  {
    //     bitsCount, pattern, expected
    make_tuple(0,  0b00000000, ""),                           // 00
    make_tuple(1,  0b00000000, "0"),                          // 01
    make_tuple(2,  0b00000000, "00"),                         // 02
    make_tuple(3,  0b00000000, "000"),                        // 03
    make_tuple(4,  0b00000000, "0000"),                       // 04
    make_tuple(5,  0b00000000, "0000_0"),                     // 05
    make_tuple(6,  0b00000000, "0000_00"),                    // 06
    make_tuple(7,  0b00000000, "0000_000"),                   // 07
    make_tuple(8,  0b00000000, "0000_0000"),                  // 08
    make_tuple(9,  0b00000000, "0000_0000_0"),                // 09
    make_tuple(0,  0b11111111, ""),                           // 10
    make_tuple(1,  0b11111111, "1"),                          // 11
    make_tuple(2,  0b11111111, "11"),                         // 12
    make_tuple(3,  0b11111111, "111"),                        // 13
    make_tuple(4,  0b11111111, "1111"),                       // 14
    make_tuple(5,  0b11111111, "1111_1"),                     // 15
    make_tuple(6,  0b11111111, "1111_11"),                    // 16
    make_tuple(7,  0b11111111, "1111_111"),                   // 17
    make_tuple(8,  0b11111111, "1111_1111"),                  // 18
    make_tuple(9,  0b11111111, "1111_1111_1"),                // 19
    make_tuple(10, 0b11001101, "1100_1101:11"),               // 20
    make_tuple(20, 0b11001101, "1100_1101:1100_1101:1100"),   // 21
    make_tuple(21, 0b11001101, "1100_1101:1100_1101:1100_1"), // 22
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector copy constructor
//!
//! @note Suppose that operator== is working properly
void UT_BinaryVector::test_Constructor_Copy ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto sourceBits)
  {
    // ---------------- Setup
    //
    auto source = BinaryVector::CreateFromBinaryString(sourceBits);

    // ---------------- Exercise
    //
    BinaryVector sut(source);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, source);
  };

  auto data =
  {
    string_view(""),             // 00
    string_view("0"),            // 01
    string_view("1"),            // 02
    string_view("01"),           // 03
    string_view("0111_00"),      // 04
    string_view("0111_001"),     // 05
    string_view("0111_0011"),    // 06
    string_view("0111_0011:0"),  // 07
    string_view("0111_0011:01"), // 08
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}

//! Checks copy constructor when other has its size fixed
//!
void UT_BinaryVector::test_Constructor_Copy_FixedSize ()
{
  // ---------------- Setup
  //
  BinaryVector sut   (3, 0x00);
  BinaryVector other (4, 0xFF, SizeProperty::Fixed);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut = other);

  // ---------------- Verify
  //
  TS_ASSERT_FALSE  (sut.HasFixedSize());
  TS_ASSERT_EQUALS (sut, other);
}


//! Checks copy constructor when other has its size fixed property that must be copied
//!
void UT_BinaryVector::test_Constructor_Copy_FixedSizeOnCopy ()
{
  // ---------------- Setup
  //
  BinaryVector sut   (3, 0x00);
  BinaryVector other (4, 0xFF, SizeProperty::FixedOnCopy);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut = other);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.HasFixedSize());
  TS_ASSERT_EQUALS (sut, other);
}


//! Checks move constructor when other has its size fixed property that must NOT be copied
//!
void UT_BinaryVector::test_Constructor_Move_FixedSize ()
{
  // ---------------- Setup
  //
  BinaryVector other (4, 0xFF, SizeProperty::Fixed);

  // ---------------- Exercise
  //
  BinaryVector sut(std::move(other));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE  (sut.HasFixedSize());
  TS_ASSERT_EQUALS (sut, BinaryVector(4, 0xFF));
}


//! Checks move constructor when other has its size fixed property that must be copied
//!
void UT_BinaryVector::test_Constructor_Move_FixedSizeOnCopy ()
{
  // ---------------- Setup
  //
  BinaryVector other (4, 0xFF, SizeProperty::FixedOnCopy);

  // ---------------- Exercise
  //
  BinaryVector sut(std::move(other));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.HasFixedSize());
  TS_ASSERT_EQUALS (sut, BinaryVector(4, 0xFF));
}


//! Checks BinaryVector move constructor
//!
//! @note Suppose that operator== is working properly
void UT_BinaryVector::test_Constructor_Move ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto sourceBits)
  {
    // ---------------- Setup
    //
    auto source = BinaryVector::CreateFromBinaryString(sourceBits);

    // ---------------- Exercise
    //
    BinaryVector sut(std::move(source));

    // ---------------- Verify
    //
    auto expected = BinaryVector::CreateFromBinaryString(sourceBits);

    TS_ASSERT_EQUALS  (sut, expected);
    TS_ASSERT_TRUE    (source.IsEmpty());
  };

  auto data =
  {
    string_view(""),             // 00
    string_view("0"),            // 01
    string_view("1"),            // 02
    string_view("01"),           // 03
    string_view("0111_00"),      // 04
    string_view("0111_001"),     // 05
    string_view("0111_0011"),    // 06
    string_view("0111_0011:0"),  // 07
    string_view("0111_0011:01"), // 08
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
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
  TS_ASSERT_EQUALS      (sut.BitsCount(),  8);
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
    auto         expectedBitsCount  = 0u;
    auto         expectedBytesCount = 0u;

    for (uint8_t input : inputs)
    {
      // ---------------- Exercise
      //
      sut.Append(input);

      // ---------------- Verify
      //
      expectedBitsCount  += 8u;
      expectedBytesCount += 1u;

      TS_ASSERT_EQUALS      (sut.BitsCount(),   expectedBitsCount);
      TS_ASSERT_EQUALS      (sut.BytesCount(), expectedBytesCount);

      const uint8_t* pData = sut.Data();

      TS_ASSERT_GREATER_THAN_EQUALS (expectedContent.size(), expectedBytesCount);
      TS_ASSERT_NOT_NULLPTR (pData);
      for (uint32_t ii = 0 ; ii < expectedBytesCount ; ++ii)
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
    auto         expectedBitsCount  = 0u;
    auto         expectedBytesCount = 0u;

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
      for (uint32_t ii = 0 ; ii < expectedBytesCount ; ++ii)
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
    auto         expectedBitsCount  = 0u;
    auto         expectedBytesCount = 0u;

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
      for (uint32_t ii = 0 ; ii < expectedBytesCount ; ++ii)
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
    auto         expectedBitsCount  = 0u;
    auto         expectedBytesCount = 0u;

    for (uint64_t input : inputs)
    {
      // ---------------- Exercise
      //
      sut.Append(input);

      // ---------------- Verify
      //
      expectedBitsCount  += 64u;
      expectedBytesCount += sizeof(uint64_t);

      TS_ASSERT_EQUALS (sut.BitsCount(),   expectedBitsCount);
      TS_ASSERT_EQUALS (sut.BytesCount(), expectedBytesCount);

      const uint8_t* pData = sut.Data();

      TS_ASSERT_GREATER_THAN_EQUALS (expectedContent.size(), expectedBytesCount);
      TS_ASSERT_NOT_NULLPTR (pData);
      for (uint32_t ii = 0 ; ii < expectedBytesCount ; ++ii)
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


//! Checks Append when sut size has been fixed (at construction)
//!
void UT_BinaryVector::test_Append_8_bits_When_FixedSize ()
{
  // ---------------- Setup
  //
  const uint8_t value = 0xA5;
  BinaryVector sut(12, 0, SizeProperty::Fixed);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Append(value), std::exception);
}

//! Checks Append when sut size has been fixed (at construction)
//!
void UT_BinaryVector::test_Append_16_bits_When_FixedSize ()
{
  // ---------------- Setup
  //
  const uint16_t value = 0xCAFE;
  BinaryVector sut(12, 0, SizeProperty::Fixed);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Append(value), std::exception);
}

//! Checks Append when sut size has been fixed (at construction)
//!
void UT_BinaryVector::test_Append_32_bits_When_FixedSize ()
{
  // ---------------- Setup
  //
  const uint32_t value = 0xFACEDEAD;
  BinaryVector sut(12, 0, SizeProperty::Fixed);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Append(value), std::exception);
}

//! Checks Append when sut size has been fixed (at construction)
//!
void UT_BinaryVector::test_Append_64_bits_When_FixedSize ()
{
  // ---------------- Setup
  //
  const uint64_t value = 0xA51234578B;
  BinaryVector sut(12, 0, SizeProperty::Fixed);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Append(value), std::exception);
}


//! Checks Append when sut is empty and adding from 1 to 8 bits from uint8_t (right aligned)
//!
//! @note Each time a new BinaryVector is used
void UT_BinaryVector::test_Append_1_to_8_bits_When_Empty_Right_Aligned ()
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
    TS_ASSERT_THROWS_NOTHING (sut.Append(value, numberOfBits, BitsAlignment::Right));

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
    for (uint32_t ii = 0 ; ii < expectedBytesCount ; ++ii)
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

//! Checks Append when sut is empty and adding from 1 to 8 bits from uint8_t (left aligned)
//!
//! @note Each time a new BinaryVector is used
void UT_BinaryVector::test_Append_1_to_8_bits_When_Empty_Left_Aligned ()
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
    TS_ASSERT_THROWS_NOTHING (sut.Append(value, numberOfBits, BitsAlignment::Left));

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
    for (uint32_t ii = 0 ; ii < expectedBytesCount ; ++ii)
    {
      TS_ASSERT_EQUALS (pData[ii], expectedContent[ii]);
    }
  };

  const vector<TInput> inputs =
  {   // Value, bits
    TInput(0x7F, 1),    // 00
    TInput(0x80, 1),    // 01
    TInput(0xB0, 2),    // 02
    TInput(0xC0, 2),    // 03
    TInput(0x7F, 3),    // 04
    TInput(0x9F, 4),    // 05
    TInput(0x8F, 5),    // 06
    TInput(0x4B, 6),    // 07
    TInput(0x94, 7),    // 08
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


//! Checks Append when sut is not empty (from 1 to 8 bits) and adding from 1 to 8 bits (right aliqned) from uint8_t
//!
//! @note Each time a new BinaryVector is used
void UT_BinaryVector::test_Append_1_to_8_bits_When_NotEmpty_Right_Aligned ()
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
    TS_ASSERT_THROWS_NOTHING (sut.Append(value_2, numberOfBits_2, BitsAlignment::Right));

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
    for (uint32_t ii = 0 ; ii < expectedBytesCount ; ++ii)
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


//! Checks Append when sut is not empty (from 1 to 8 bits) and adding from 1 to 8 bits (left aliqned) from uint8_t
//!
//! @note Each time a new BinaryVector is used
void UT_BinaryVector::test_Append_1_to_8_bits_When_NotEmpty_Left_Aligned ()
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
    sut.Append(value_1, numberOfBits_1, BitsAlignment::Right);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(value_2, numberOfBits_2, BitsAlignment::Left));

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
    for (uint32_t ii = 0 ; ii < expectedBytesCount ; ++ii)
    {
      os.str("");
      os << "pData[" << ii << "]";
      auto msg = os.str().c_str();
      TSM_ASSERT_EQUALS (msg, pData[ii], expectedContent[ii]);
    }
  };

  const vector<TInput> inputs =
  {   // Value, bits(right aligned), value, bits (left aligned)
    TInput(0x00, 1, 0b10000000, 1), // 00
    TInput(0x01, 1, 0b01111111, 1), // 01
    TInput(0x02, 2, 0b11000000, 2), // 02
    TInput(0x03, 2, 0b10000010, 7), // 03
    TInput(0x03, 3, 0b00101011, 5), // 04
    TInput(0x09, 4, 0b10000001, 8), // 05
    TInput(0x11, 5, 0b01110111, 8), // 06
    TInput(0x12, 6, 0b10100100, 6), // 07
    TInput(0x4A, 7, 0b01001010, 8), // 08
    TInput(0x7B, 8, 0b10100000, 3), // 09
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


//! Checks Append when sut is empty and adding from 1 to 16 bits from uint16_t (right aligned)
//!
//! @note Each time a new BinaryVector is used
void UT_BinaryVector::test_Append_1_to_16_bits_When_Empty_Right_Aligned ()
{
  // ---------------- DDT Setup
  //
  using TInput = tuple<uint16_t, uint8_t> ; // Value, Number of bits (taken from LSB)

  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto& input    = std::get<0>(data);
    const auto& expected = std::get<1>(data) ;
    auto value           = std::get<0>(input);
    auto numberOfBits    = std::get<1>(input);

    BinaryVector sut;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(value, numberOfBits, BitsAlignment::Right));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, expected);
  };

  auto data =
  {             // Value, bits
    make_tuple(TInput(0x0000, 1),  BinaryVector::CreateFromBinaryString("0")),                   // 00
    make_tuple(TInput(0x0001, 1),  BinaryVector::CreateFromBinaryString("1")),                   // 01
    make_tuple(TInput(0x0002, 2),  BinaryVector::CreateFromBinaryString("10")),                  // 02
    make_tuple(TInput(0x0003, 2),  BinaryVector::CreateFromBinaryString("11")),                  // 03
    make_tuple(TInput(0x0003, 3),  BinaryVector::CreateFromBinaryString("011")),                 // 04
    make_tuple(TInput(0x0009, 4),  BinaryVector::CreateFromBinaryString("1001")),                // 05
    make_tuple(TInput(0x0011, 5),  BinaryVector::CreateFromBinaryString("1000_1")),              // 06
    make_tuple(TInput(0x0012, 6),  BinaryVector::CreateFromBinaryString("0100_10")),             // 07
    make_tuple(TInput(0x004A, 7),  BinaryVector::CreateFromBinaryString("1001_010")),            // 08
    make_tuple(TInput(0x007B, 8),  BinaryVector::CreateFromBinaryString("0111_1011")),           // 09
    make_tuple(TInput(0x017B, 9),  BinaryVector::CreateFromBinaryString("1:0111_1011")),         // 10
    make_tuple(TInput(0x037B, 11), BinaryVector::CreateFromBinaryString("011:0111_1011")),       // 11
    make_tuple(TInput(0x5ACE, 15), BinaryVector::CreateFromBinaryString("101:1010_1100:1110")),  // 12
    make_tuple(TInput(0xFACE, 16), BinaryVector::CreateFromBinaryString("1111_1010:1100_1110")), // 13
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}

//! Checks Append when sut is empty and adding from 1 to 16 bits from uint16_t (left aligned)
//!
//! @note Each time a new BinaryVector is used
void UT_BinaryVector::test_Append_1_to_16_bits_When_Empty_Left_Aligned ()
{
  // ---------------- DDT Setup
  //
  using TInput = tuple<uint16_t, uint8_t> ; // Value, Number of bits (taken from LSB)

  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto& input    = std::get<0>(data);
    const auto& expected = std::get<1>(data) ;
    auto value           = std::get<0>(input);
    auto numberOfBits    = std::get<1>(input);

    BinaryVector sut;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(value, numberOfBits, BitsAlignment::Left));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, expected);
  };

  auto data =     // Value, bits, Expected
  {
    make_tuple(TInput(0x0000, 1),  BinaryVector::CreateFromBinaryString("0")),                   // 00
    make_tuple(TInput(0x0001, 1),  BinaryVector::CreateFromBinaryString("0")),                   // 01
    make_tuple(TInput(0x4000, 1),  BinaryVector::CreateFromBinaryString("0")),                   // 02
    make_tuple(TInput(0x8000, 1),  BinaryVector::CreateFromBinaryString("1")),                   // 03
    make_tuple(TInput(0xFFFF, 1),  BinaryVector::CreateFromBinaryString("1")),                   // 04
    make_tuple(TInput(0x8000, 2),  BinaryVector::CreateFromBinaryString("10")),                  // 05
    make_tuple(TInput(0xC000, 2),  BinaryVector::CreateFromBinaryString("11")),                  // 06
    make_tuple(TInput(0x7000, 3),  BinaryVector::CreateFromBinaryString("011")),                 // 07
    make_tuple(TInput(0x9000, 4),  BinaryVector::CreateFromBinaryString("1001")),                // 08
    make_tuple(TInput(0x8800, 5),  BinaryVector::CreateFromBinaryString("1000_1")),              // 09
    make_tuple(TInput(0x4800, 6),  BinaryVector::CreateFromBinaryString("0100_10")),             // 10
    make_tuple(TInput(0x9400, 7),  BinaryVector::CreateFromBinaryString("1001_010")),            // 11
    make_tuple(TInput(0x7B00, 8),  BinaryVector::CreateFromBinaryString("0111_1011")),           // 12
    make_tuple(TInput(0xBD80, 9),  BinaryVector::CreateFromBinaryString("1011_1101:1")),         // 13
    make_tuple(TInput(0x6F60, 11), BinaryVector::CreateFromBinaryString("0110_1111:011")),       // 14
    make_tuple(TInput(0xB59C, 15), BinaryVector::CreateFromBinaryString("1011_0101:1001_110")),  // 15
    make_tuple(TInput(0xFACE, 16), BinaryVector::CreateFromBinaryString("1111_1010:1100_1110")), // 16
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks Append when sut is not empty (from 1 to 16 bits) and adding from 1 to 16 bits (right aliqned) from uint16_t
//!
//! @note Each time a new BinaryVector is used
void UT_BinaryVector::test_Append_1_to_16_bits_When_NotEmpty_Right_Aligned ()
{
  // ---------------- DDT Setup
  //
  using TInput = tuple<BinaryVector, uint16_t, uint8_t> ; // Value, Number of bits (taken from LSB)

  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto& input    = std::get<0>(data);
    const auto& expected = std::get<1>(data);
    auto sut             = std::get<0>(input);
    auto value           = std::get<1>(input);
    auto numberOfBits    = std::get<2>(input);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(value, numberOfBits, BitsAlignment::Right));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, expected);
  };

  auto bbv = [](string_view asBinaryString) { return BinaryVector::CreateFromBinaryString(asBinaryString); };

  auto data =
  {                   // initial, Value, bits
    make_tuple(TInput(bbv("1"),        0x0000, 1),  bbv("1:0")),                         // 00
    make_tuple(TInput(bbv("0"),        0x0001, 1),  bbv("0:1")),                         // 01
    make_tuple(TInput(bbv("01"),       0x0002, 2),  bbv("01:10")),                       // 02
    make_tuple(TInput(bbv("00"),       0x0003, 2),  bbv("00:11")),                       // 03
    make_tuple(TInput(bbv("100"),      0x0003, 3),  bbv("100:011")),                     // 04
    make_tuple(TInput(bbv("101"),      0x0009, 4),  bbv("101:1001")),                    // 05
    make_tuple(TInput(bbv("110"),      0x0011, 5),  bbv("110:1_0001")),                  // 06
    make_tuple(TInput(bbv("111"),      0x0012, 6),  bbv("111:01_0010")),                 // 07
    make_tuple(TInput(bbv("1000"),     0x004A, 7),  bbv("1000:100_1010")),               // 08
    make_tuple(TInput(bbv("1000_0"),   0x007B, 8),  bbv("1_0000:0111_1011")),            // 09
    make_tuple(TInput(bbv("1010_11"),  0x037B, 11), bbv("1010_11:011:0111_1011")),       // 10
    make_tuple(TInput(bbv("1111_001"), 0x5ACE, 15), bbv("1111_001:101:1010_1100:1110")), // 11
    make_tuple(TInput(bbv("1001"),     0xFACE, 16), bbv("1001:1111_1010:1100_1110")),    // 12
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks Append when sut is not empty (from 1 to 16 bits) and adding from 1 to 16 bits (left aliqned) from uint16_t
//!
//! @note Each time a new BinaryVector is used
void UT_BinaryVector::test_Append_1_to_16_bits_When_NotEmpty_Left_Aligned ()
{
  // ---------------- DDT Setup
  //
  using TInput = tuple<BinaryVector, uint16_t, uint8_t> ; // Value, Number of bits (taken from LSB)

  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto& input    = std::get<0>(data);
    const auto& expected = std::get<1>(data);
    auto sut             = std::get<0>(input);
    auto value           = std::get<1>(input);
    auto numberOfBits    = std::get<2>(input);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(value, numberOfBits, BitsAlignment::Left));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, expected);
  };

  auto bbv = [](string_view asBinaryString) { return BinaryVector::CreateFromBinaryString(asBinaryString); };

  auto data =         // Initial, Value, bits
  {
    make_tuple(TInput(bbv("1"),         0x0000, 1),  bbv("1:0")),                           // 00
    make_tuple(TInput(bbv("0"),         0x0001, 1),  bbv("0:0")),                           // 01
    make_tuple(TInput(bbv("01"),        0x4000, 2),  bbv("01:01")),                         // 02
    make_tuple(TInput(bbv("00"),        0x8000, 2),  bbv("00:10")),                         // 03
    make_tuple(TInput(bbv("100"),       0xFFFF, 3),  bbv("100:111")),                       // 04
    make_tuple(TInput(bbv("101"),       0x8000, 4),  bbv("101:1000")),                      // 05
    make_tuple(TInput(bbv("110"),       0xC000, 5),  bbv("110:1100_0")),                    // 06
    make_tuple(TInput(bbv("111"),       0x7000, 6),  bbv("111:0111_00")),                   // 07
    make_tuple(TInput(bbv("1000"),      0x9400, 7),  bbv("1000:1001_010")),                 // 08
    make_tuple(TInput(bbv("1000_0"),    0x8800, 8),  bbv("1_0000:1000_1000")),              // 09
    make_tuple(TInput(bbv("1001_1"),    0xBD80, 9),  bbv("1001_1:1011_1101:1")),            // 10
    make_tuple(TInput(bbv("1001_10"),   0x6F60, 10), bbv("1001_10:0110_1111:01")),          // 11
    make_tuple(TInput(bbv("1010_11"),   0x4800, 11), bbv("1010_11:0100_1000:000")),         // 12
    make_tuple(TInput(bbv("1001_111"),  0xB59C, 12), bbv("1001_111:1011_0101:1001")),       // 13
    make_tuple(TInput(bbv("1001_111"),  0xB59C, 13), bbv("1001_111:1011_0101:1001_1")),     // 14
    make_tuple(TInput(bbv("1001"),      0x7B02, 14), bbv("1001:0111_1011:0000_00")),        // 15
    make_tuple(TInput(bbv("1111_001"),  0x940A, 15), bbv("1111_001:1001_0100:0000_101")),   // 16
    make_tuple(TInput(bbv("1001_111"),  0xFACE, 16), bbv("1001_111:1111_1010:1100_1110")),  // 17
    make_tuple(TInput(bbv("1001_1111"), 0xFACE, 16), bbv("1001_1111:1111_1010:1100_1110")), // 18
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks Append when sut is empty and adding from 1 to 32 bits from uint32_t (right aligned)
//!
//! @note Each time a new BinaryVector is used
void UT_BinaryVector::test_Append_1_to_32_bits_When_Empty_Right_Aligned ()
{
  // ---------------- DDT Setup
  //

  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto& input    = std::get<0>(data);
    const auto& expected = std::get<1>(data) ;
    auto value           = std::get<0>(input);
    auto numberOfBits    = std::get<1>(input);

    BinaryVector sut;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(value, numberOfBits, BitsAlignment::Right));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, expected);
  };

  using TInput = tuple<uint32_t, uint8_t> ; // Value, Number of bits (taken from LSB)
  auto data =   // Value, bits
  {
    make_tuple(TInput(0x00000000, 1),  BinaryVector::CreateFromBinaryString("0")),                                       // 00
    make_tuple(TInput(0x00000001, 1),  BinaryVector::CreateFromBinaryString("1")),                                       // 01
    make_tuple(TInput(0x00000002, 2),  BinaryVector::CreateFromBinaryString("10")),                                      // 02
    make_tuple(TInput(0x00000003, 2),  BinaryVector::CreateFromBinaryString("11")),                                      // 03
    make_tuple(TInput(0x00000003, 3),  BinaryVector::CreateFromBinaryString("011")),                                     // 04
    make_tuple(TInput(0x00000009, 4),  BinaryVector::CreateFromBinaryString("1001")),                                    // 05
    make_tuple(TInput(0x00000011, 5),  BinaryVector::CreateFromBinaryString("1000_1")),                                  // 06
    make_tuple(TInput(0x00000012, 6),  BinaryVector::CreateFromBinaryString("0100_10")),                                 // 07
    make_tuple(TInput(0x0000004A, 7),  BinaryVector::CreateFromBinaryString("1001_010")),                                // 08
    make_tuple(TInput(0x0000007B, 8),  BinaryVector::CreateFromBinaryString("0111_1011")),                               // 09
    make_tuple(TInput(0x0000017B, 9),  BinaryVector::CreateFromBinaryString("1:0111_1011")),                             // 10
    make_tuple(TInput(0x0000017B, 10), BinaryVector::CreateFromBinaryString("01:0111_1011")),                            // 11
    make_tuple(TInput(0x0000037B, 11), BinaryVector::CreateFromBinaryString("011:0111_1011")),                           // 12
    make_tuple(TInput(0x0000037B, 12), BinaryVector::CreateFromBinaryString("0011:0111_1011")),                          // 13
    make_tuple(TInput(0x00005ACE, 13), BinaryVector::CreateFromBinaryString("1:1010_1100:1110")),                        // 14
    make_tuple(TInput(0x00005ACE, 14), BinaryVector::CreateFromBinaryString("01:1010_1100:1110")),                       // 15
    make_tuple(TInput(0x00005ACE, 15), BinaryVector::CreateFromBinaryString("101:1010_1100:1110")),                      // 16
    make_tuple(TInput(0x0000FACE, 16), BinaryVector::CreateFromBinaryString("1111_1010:1100_1110")),                     // 17
    make_tuple(TInput(0x8164BACE, 17), BinaryVector::CreateFromBinaryString("0:1011_1010:1100_1110")),                   // 18
    make_tuple(TInput(0x8164BACE, 18), BinaryVector::CreateFromBinaryString("00:1011_1010:1100_1110")),                  // 19
    make_tuple(TInput(0x8164BACE, 19), BinaryVector::CreateFromBinaryString("100:1011_1010:1100_1110")),                 // 20
    make_tuple(TInput(0x8164BACE, 20), BinaryVector::CreateFromBinaryString("0100:1011_1010:1100_1110")),                // 21
    make_tuple(TInput(0x8164BACE, 21), BinaryVector::CreateFromBinaryString("0_0100:1011_1010:1100_1110")),              // 22
    make_tuple(TInput(0x8164BACE, 22), BinaryVector::CreateFromBinaryString("10_0100:1011_1010:1100_1110")),             // 23
    make_tuple(TInput(0x8164BACE, 23), BinaryVector::CreateFromBinaryString("110_0100:1011_1010:1100_1110")),            // 24
    make_tuple(TInput(0x8164BACE, 24), BinaryVector::CreateFromBinaryString("0110_0100:1011_1010:1100_1110")),           // 25
    make_tuple(TInput(0x8164BACE, 25), BinaryVector::CreateFromBinaryString("1:0110_0100:1011_1010:1100_1110")),         // 26
    make_tuple(TInput(0x8164BACE, 26), BinaryVector::CreateFromBinaryString("01:0110_0100:1011_1010:1100_1110")),        // 27
    make_tuple(TInput(0x8164BACE, 27), BinaryVector::CreateFromBinaryString("001:0110_0100:1011_1010:1100_1110")),       // 28
    make_tuple(TInput(0x8164BACE, 28), BinaryVector::CreateFromBinaryString("0001:0110_0100:1011_1010:1100_1110")),      // 29
    make_tuple(TInput(0x8164BACE, 29), BinaryVector::CreateFromBinaryString("0_0001:0110_0100:1011_1010:1100_1110")),    // 30
    make_tuple(TInput(0x8164BACE, 30), BinaryVector::CreateFromBinaryString("00_0001:0110_0100:1011_1010:1100_1110")),   // 31
    make_tuple(TInput(0x8164BACE, 31), BinaryVector::CreateFromBinaryString("000_0001:0110_0100:1011_1010:1100_1110")),  // 32
    make_tuple(TInput(0x8164BACE, 32), BinaryVector::CreateFromBinaryString("1000_0001:0110_0100:1011_1010:1100_1110")), // 33
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}

//! Checks Append when sut is empty and adding from 1 to 32 bits from uint32_t (left aligned)
//!
//! @note Each time a new BinaryVector is used
void UT_BinaryVector::test_Append_1_to_32_bits_When_Empty_Left_Aligned ()
{
  // ---------------- DDT Setup
  //
  using TInput = tuple<uint32_t, uint8_t> ; // Value, Number of bits (taken from LSB)

  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto& input    = std::get<0>(data);
    const auto& expected = std::get<1>(data) ;
    auto value           = std::get<0>(input);
    auto numberOfBits    = std::get<1>(input);

    BinaryVector sut;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(value, numberOfBits, BitsAlignment::Left));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, expected);
  };

  auto data =     // Value, bits, Expected
  {
    make_tuple(TInput(0x00000000, 1),  BinaryVector::CreateFromBinaryString("0")),                                       // 00
    make_tuple(TInput(0x80000000, 1),  BinaryVector::CreateFromBinaryString("1")),                                       // 01
    make_tuple(TInput(0x80000000, 2),  BinaryVector::CreateFromBinaryString("10")),                                      // 02
    make_tuple(TInput(0xC0000000, 2),  BinaryVector::CreateFromBinaryString("11")),                                      // 03
    make_tuple(TInput(0x70000000, 3),  BinaryVector::CreateFromBinaryString("011")),                                     // 04
    make_tuple(TInput(0x90000000, 4),  BinaryVector::CreateFromBinaryString("1001")),                                    // 05
    make_tuple(TInput(0x88000000, 5),  BinaryVector::CreateFromBinaryString("1000_1")),                                  // 06
    make_tuple(TInput(0x48000000, 6),  BinaryVector::CreateFromBinaryString("0100_10")),                                 // 07
    make_tuple(TInput(0x94000000, 7),  BinaryVector::CreateFromBinaryString("1001_010")),                                // 08
    make_tuple(TInput(0x7B000000, 8),  BinaryVector::CreateFromBinaryString("0111_1011")),                               // 09
    make_tuple(TInput(0xBD800000, 9),  BinaryVector::CreateFromBinaryString("1011_1101:1")),                             // 10
    make_tuple(TInput(0x5EC00000, 10), BinaryVector::CreateFromBinaryString("0101_1110:11")),                            // 11
    make_tuple(TInput(0x6F600000, 11), BinaryVector::CreateFromBinaryString("0110_1111:011")),                           // 12
    make_tuple(TInput(0x37B00000, 12), BinaryVector::CreateFromBinaryString("0011_0111:1011")),                          // 13
    make_tuple(TInput(0xD6700000, 13), BinaryVector::CreateFromBinaryString("1101_0110:0111_0")),                        // 14
    make_tuple(TInput(0x6B380000, 14), BinaryVector::CreateFromBinaryString("0110_1011:0011_10")),                       // 15
    make_tuple(TInput(0xB59D0000, 15), BinaryVector::CreateFromBinaryString("1011_0101:1001_110")),                      // 16
    make_tuple(TInput(0xFACE0000, 16), BinaryVector::CreateFromBinaryString("1111_1010:1100_1110")),                     // 17
    make_tuple(TInput(0x8164BACE, 17), BinaryVector::CreateFromBinaryString("1000_0001:0110_0100:1")),                   // 18
    make_tuple(TInput(0x8164BACE, 18), BinaryVector::CreateFromBinaryString("1000_0001:0110_0100:10")),                  // 19
    make_tuple(TInput(0x8164BACE, 19), BinaryVector::CreateFromBinaryString("1000_0001:0110_0100:101")),                 // 20
    make_tuple(TInput(0x8164BACE, 20), BinaryVector::CreateFromBinaryString("1000_0001:0110_0100:1011")),                // 21
    make_tuple(TInput(0x8164BACE, 21), BinaryVector::CreateFromBinaryString("1000_0001:0110_0100:1011_1")),              // 22
    make_tuple(TInput(0x8164BACE, 22), BinaryVector::CreateFromBinaryString("1000_0001:0110_0100:1011_10")),             // 23
    make_tuple(TInput(0x8164BACE, 23), BinaryVector::CreateFromBinaryString("1000_0001:0110_0100:1011_101")),            // 24
    make_tuple(TInput(0x8164BACE, 24), BinaryVector::CreateFromBinaryString("1000_0001:0110_0100:1011_1010")),           // 25
    make_tuple(TInput(0x8164BACE, 25), BinaryVector::CreateFromBinaryString("1000_0001:0110_0100:1011_1010:1")),         // 26
    make_tuple(TInput(0x8164BACE, 26), BinaryVector::CreateFromBinaryString("1000_0001:0110_0100:1011_1010:11")),        // 27
    make_tuple(TInput(0x8164BACE, 27), BinaryVector::CreateFromBinaryString("1000_0001:0110_0100:1011_1010:110")),       // 28
    make_tuple(TInput(0x8164BACE, 28), BinaryVector::CreateFromBinaryString("1000_0001:0110_0100:1011_1010:1100")),      // 29
    make_tuple(TInput(0x8164BACE, 29), BinaryVector::CreateFromBinaryString("1000_0001:0110_0100:1011_1010:1100_1")),    // 30
    make_tuple(TInput(0x8164BACE, 30), BinaryVector::CreateFromBinaryString("1000_0001:0110_0100:1011_1010:1100_11")),   // 31
    make_tuple(TInput(0x8164BACE, 31), BinaryVector::CreateFromBinaryString("1000_0001:0110_0100:1011_1010:1100_111")),  // 32
    make_tuple(TInput(0x8164BACE, 32), BinaryVector::CreateFromBinaryString("1000_0001:0110_0100:1011_1010:1100_1110")), // 33
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks Append when sut is not empty (from 1 to 32 bits) and adding from 1 to 32 bits (right aliqned) from uint32_t
//!
//! @note Each time a new BinaryVector is used
void UT_BinaryVector::test_Append_1_to_32_bits_When_NotEmpty_Right_Aligned ()
{
  // ---------------- DDT Setup
  //
  using TInput = tuple<BinaryVector, uint32_t, uint8_t> ; // Value, Number of bits (taken from LSB)

  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto& input    = std::get<0>(data);
    const auto& expected = std::get<1>(data);
    auto sut             = std::get<0>(input);
    auto value           = std::get<1>(input);
    auto numberOfBits    = std::get<2>(input);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(value, numberOfBits, BitsAlignment::Right));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, expected);
  };

  auto bbv = [](string_view asBinaryString) { return BinaryVector::CreateFromBinaryString(asBinaryString); };

  auto data =         // initial, Value, bits
  {
    make_tuple(TInput(bbv("1"),        0x00000000, 1),  bbv("1:0")),                                              // 00
    make_tuple(TInput(bbv("0"),        0x00000001, 1),  bbv("0:1")),                                              // 01
    make_tuple(TInput(bbv("01"),       0x00000002, 2),  bbv("01:10")),                                            // 02
    make_tuple(TInput(bbv("00"),       0x00000003, 2),  bbv("00:11")),                                            // 03
    make_tuple(TInput(bbv("100"),      0x00000003, 3),  bbv("100:011")),                                          // 04
    make_tuple(TInput(bbv("101"),      0x00000009, 4),  bbv("101:1001")),                                         // 05
    make_tuple(TInput(bbv("110"),      0x00000011, 5),  bbv("110:1000_1")),                                       // 06
    make_tuple(TInput(bbv("111"),      0x00000012, 6),  bbv("111:0100_10")),                                      // 07
    make_tuple(TInput(bbv("1000"),     0x0000004A, 7),  bbv("1000:1001_010")),                                    // 08
    make_tuple(TInput(bbv("1000_0"),   0x0000007B, 8),  bbv("1000_0:0111_1011")),                                 // 09
    make_tuple(TInput(bbv("1010_11"),  0x0000017B, 9),  bbv("1010_11:1:0111_1011")),                              // 10
    make_tuple(TInput(bbv("1111_001"), 0x0000017B, 10), bbv("1111_001:01:0111_1011")),                            // 11
    make_tuple(TInput(bbv("1001"),     0x0000037B, 11), bbv("1001:011:0111_1011")),                               // 12
    make_tuple(TInput(bbv("1"),        0x0000037B, 12), bbv("1:0011:0111_1011")),                                 // 13
    make_tuple(TInput(bbv("0"),        0x00005ACE, 13), bbv("0:1:1010_1100:1110")),                               // 14
    make_tuple(TInput(bbv("01"),       0x00005ACE, 14), bbv("01:01:1010_1100:1110")),                             // 15
    make_tuple(TInput(bbv("00"),       0x00005ACE, 15), bbv("00:101:1010_1100:1110")),                            // 16
    make_tuple(TInput(bbv("100"),      0x0000FACE, 16), bbv("100:1111_1010:1100_1110")),                          // 17
    make_tuple(TInput(bbv("101"),      0x8164BACE, 17), bbv("101:0:1011_1010:1100_1110")),                        // 18
    make_tuple(TInput(bbv("110"),      0x8164BACE, 18), bbv("110:00:1011_1010:1100_1110")),                       // 19
    make_tuple(TInput(bbv("111"),      0x8164BACE, 19), bbv("111:100:1011_1010:1100_1110")),                      // 20
    make_tuple(TInput(bbv("1000"),     0x8164BACE, 20), bbv("1000:0100:1011_1010:1100_1110")),                    // 21
    make_tuple(TInput(bbv("1000_0"),   0x8164BACE, 21), bbv("1000_0:0_0100:1011_1010:1100_1110")),                // 22
    make_tuple(TInput(bbv("1010_11"),  0x8164BACE, 22), bbv("1010_11:10_0100:1011_1010:1100_1110")),              // 23
    make_tuple(TInput(bbv("1111_001"), 0x8164BACE, 23), bbv("1111_001:110_0100:1011_1010:1100_1110")),            // 24
    make_tuple(TInput(bbv("1001"),     0x8164BACE, 24), bbv("1001:0110_0100:1011_1010:1100_1110")),               // 25
    make_tuple(TInput(bbv("110"),      0x8164BACE, 25), bbv("110:1:0110_0100:1011_1010:1100_1110")),              // 26
    make_tuple(TInput(bbv("111"),      0x8164BACE, 26), bbv("111:01:0110_0100:1011_1010:1100_1110")),             // 27
    make_tuple(TInput(bbv("1000"),     0x8164BACE, 27), bbv("1000:001:0110_0100:1011_1010:1100_1110")),           // 28
    make_tuple(TInput(bbv("1000_0"),   0x8164BACE, 28), bbv("1000_0:0001:0110_0100:1011_1010:1100_1110")),        // 29
    make_tuple(TInput(bbv("1010_11"),  0x8164BACE, 29), bbv("1010_11:0_0001:0110_0100:1011_1010:1100_1110")),     // 30
    make_tuple(TInput(bbv("1111_001"), 0x8164BACE, 30), bbv("1111_001:00_0001:0110_0100:1011_1010:1100_1110")),   // 31
    make_tuple(TInput(bbv("1010_11"),  0x8164BACE, 31), bbv("1010_11:000_0001:0110_0100:1011_1010:1100_1110")),   // 32
    make_tuple(TInput(bbv("1111_001"), 0x8164BACE, 32), bbv("1111_001:1000_0001:0110_0100:1011_1010:1100_1110")), // 33
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks Append when sut is not empty (from 1 to 312 bits) and adding from 1 to 32 bits (left aliqned) from uint32_t
//!
//! @note Each time a new BinaryVector is used
void UT_BinaryVector::test_Append_1_to_32_bits_When_NotEmpty_Left_Aligned ()
{
  // ---------------- DDT Setup
  //
  using TInput = tuple<BinaryVector, uint32_t, uint8_t> ; // Value, Number of bits (taken from LSB)

  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto& input    = std::get<0>(data);
    const auto& expected = std::get<1>(data);
    auto sut             = std::get<0>(input);
    auto value           = std::get<1>(input);
    auto numberOfBits    = std::get<2>(input);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(value, numberOfBits, BitsAlignment::Left));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, expected);
  };

  auto bbv = [](string_view asBinaryString) { return BinaryVector::CreateFromBinaryString(asBinaryString); };

  auto data =         // Initial, Value, bits
  {
    make_tuple(TInput(bbv("1"),        0x00000000, 1),  bbv("1:0")),                                              // 00
    make_tuple(TInput(bbv("0"),        0x80000000, 1),  bbv("0:1")),                                              // 01
    make_tuple(TInput(bbv("01"),       0x80000000, 2),  bbv("01:10")),                                            // 02
    make_tuple(TInput(bbv("00"),       0xC0000000, 2),  bbv("00:11")),                                            // 03
    make_tuple(TInput(bbv("100"),      0x70000000, 3),  bbv("100:011")),                                          // 04
    make_tuple(TInput(bbv("101"),      0x90000000, 4),  bbv("101:1001")),                                         // 05
    make_tuple(TInput(bbv("110"),      0x88000000, 5),  bbv("110:1000_1")),                                       // 06
    make_tuple(TInput(bbv("111"),      0x48000000, 6),  bbv("111:0100_10")),                                      // 07
    make_tuple(TInput(bbv("1000"),     0x94000000, 7),  bbv("1000:1001_010")),                                    // 08
    make_tuple(TInput(bbv("1000_0"),   0x7B000000, 8),  bbv("1000_0:0111_1011")),                                 // 09
    make_tuple(TInput(bbv("1010_11"),  0xBD800000, 9),  bbv("1010_11:1011_1101:1")),                              // 10
    make_tuple(TInput(bbv("1111_001"), 0x5EC00000, 10), bbv("1111_001:0101_1110:11")),                            // 11
    make_tuple(TInput(bbv("1001"),     0x6F600000, 11), bbv("1001:0110_1111:011")),                               // 12
    make_tuple(TInput(bbv("1"),        0x37B00000, 12), bbv("1:0011_0111:1011")),                                 // 13
    make_tuple(TInput(bbv("0"),        0xD6700000, 13), bbv("0:1101_0110:0111_0")),                               // 14
    make_tuple(TInput(bbv("01"),       0x6B380000, 14), bbv("01:0110_1011:0011_10")),                             // 15
    make_tuple(TInput(bbv("00"),       0xB59D0000, 15), bbv("00:1011_0101:1001_110")),                            // 16
    make_tuple(TInput(bbv("100"),      0xFACE0000, 16), bbv("100:1111_1010:1100_1110")),                          // 17
    make_tuple(TInput(bbv("101"),      0x8164BACE, 17), bbv("101:1000_0001:0110_0100:1")),                        // 18
    make_tuple(TInput(bbv("110"),      0x8164BACE, 18), bbv("110:1000_0001:0110_0100:10")),                       // 19
    make_tuple(TInput(bbv("111"),      0x8164BACE, 19), bbv("111:1000_0001:0110_0100:101")),                      // 20
    make_tuple(TInput(bbv("1000"),     0x8164BACE, 20), bbv("1000:1000_0001:0110_0100:1011")),                    // 21
    make_tuple(TInput(bbv("1000_0"),   0x8164BACE, 21), bbv("1000_0:1000_0001:0110_0100:1011_1")),                // 22
    make_tuple(TInput(bbv("1010_11"),  0x8164BACE, 22), bbv("1010_11:1000_0001:0110_0100:1011_10")),              // 23
    make_tuple(TInput(bbv("1111_001"), 0x8164BACE, 23), bbv("1111_001:1000_0001:0110_0100:1011_101")),            // 24
    make_tuple(TInput(bbv("1001"),     0x8164BACE, 24), bbv("1001:1000_0001:0110_0100:1011_1010")),               // 25
    make_tuple(TInput(bbv("110"),      0x8164BACE, 25), bbv("110:1000_0001:0110_0100:1011_1010:1")),              // 26
    make_tuple(TInput(bbv("111"),      0x8164BACE, 26), bbv("111:1000_0001:0110_0100:1011_1010:11")),             // 27
    make_tuple(TInput(bbv("1000"),     0x8164BACE, 27), bbv("1000:1000_0001:0110_0100:1011_1010:110")),           // 28
    make_tuple(TInput(bbv("1000_0"),   0x8164BACE, 28), bbv("1000_0:1000_0001:0110_0100:1011_1010:1100")),        // 29
    make_tuple(TInput(bbv("1010_11"),  0x8164BACE, 29), bbv("1010_11:1000_0001:0110_0100:1011_1010:1100_1")),     // 30
    make_tuple(TInput(bbv("1111_001"), 0x8164BACE, 30), bbv("1111_001:1000_0001:0110_0100:1011_1010:1100_11")),   // 31
    make_tuple(TInput(bbv("1010_11"),  0x8164BACE, 31), bbv("1010_11:1000_0001:0110_0100:1011_1010:1100_111")),   // 32
    make_tuple(TInput(bbv("1111_001"), 0x8164BACE, 32), bbv("1111_001:1000_0001:0110_0100:1011_1010:1100_1110")), // 33
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks Append when sut is empty and adding from 1 to 64 bits from uint64_t (right aligned)
//!
//! @note Each time a new BinaryVector is used
void UT_BinaryVector::test_Append_1_to_64_bits_When_Empty_Right_Aligned ()
{
  // ---------------- DDT Setup
  //

  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto& input    = std::get<0>(data);
    const auto& expected = std::get<1>(data) ;
    auto value           = std::get<0>(input);
    auto numberOfBits    = std::get<1>(input);

    BinaryVector sut;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(value, numberOfBits, BitsAlignment::Right));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, expected);
  };

  auto bbv     = [](string_view asBinaryString) { return BinaryVector::CreateFromBinaryString(asBinaryString); };
  using TInput = tuple<uint64_t, uint8_t> ; // Value, Number of bits (taken from LSB)

  auto data    =   // Value, bits
  {
    make_tuple(TInput(0x0000000000000000, 1),  bbv("0")),                                                                               // 00
    make_tuple(TInput(0x0000000000000001, 1),  bbv("1")),                                                                               // 01
    make_tuple(TInput(0x0000000000000002, 2),  bbv("10")),                                                                              // 02
    make_tuple(TInput(0x0000000000000003, 2),  bbv("11")),                                                                              // 03
    make_tuple(TInput(0x0000000000000003, 3),  bbv("011")),                                                                             // 04
    make_tuple(TInput(0x0000000000000009, 4),  bbv("1001")),                                                                            // 05
    make_tuple(TInput(0x0000000000000011, 5),  bbv("1000_1")),                                                                          // 06
    make_tuple(TInput(0x0000000000000012, 6),  bbv("0100_10")),                                                                         // 07
    make_tuple(TInput(0x000000000000004A, 7),  bbv("1001_010")),                                                                        // 08
    make_tuple(TInput(0x000000000000007B, 8),  bbv("0111_1011")),                                                                       // 09
    make_tuple(TInput(0x000000000000017B, 9),  bbv("1:0111_1011")),                                                                     // 10
    make_tuple(TInput(0x000000000000017B, 10), bbv("01:0111_1011")),                                                                    // 11
    make_tuple(TInput(0x000000000000037B, 11), bbv("011:0111_1011")),                                                                   // 12
    make_tuple(TInput(0x000000000000037B, 12), bbv("0011:0111_1011")),                                                                  // 13
    make_tuple(TInput(0x0000000000005ACE, 13), bbv("1:1010_1100:1110")),                                                                // 14
    make_tuple(TInput(0x0000000000005ACE, 14), bbv("01:1010_1100:1110")),                                                               // 15
    make_tuple(TInput(0x0000000000005ACE, 15), bbv("101:1010_1100:1110")),                                                              // 16
    make_tuple(TInput(0x000000000000FACE, 16), bbv("1111_1010:1100_1110")),                                                             // 17
    make_tuple(TInput(0x000000008164BACE, 17), bbv("0:1011_1010:1100_1110")),                                                           // 18
    make_tuple(TInput(0x000000008164BACE, 18), bbv("00:1011_1010:1100_1110")),                                                          // 19
    make_tuple(TInput(0x000000008164BACE, 19), bbv("100:1011_1010:1100_1110")),                                                         // 20
    make_tuple(TInput(0x000000008164BACE, 20), bbv("0100:1011_1010:1100_1110")),                                                        // 21
    make_tuple(TInput(0x000000008164BACE, 21), bbv("0_0100:1011_1010:1100_1110")),                                                      // 22
    make_tuple(TInput(0x000000008164BACE, 22), bbv("10_0100:1011_1010:1100_1110")),                                                     // 23
    make_tuple(TInput(0x000000008164BACE, 23), bbv("110_0100:1011_1010:1100_1110")),                                                    // 24
    make_tuple(TInput(0x000000008164BACE, 24), bbv("0110_0100:1011_1010:1100_1110")),                                                   // 25
    make_tuple(TInput(0x000000008164BACE, 25), bbv("1:0110_0100:1011_1010:1100_1110")),                                                 // 26
    make_tuple(TInput(0x000000008164BACE, 26), bbv("01:0110_0100:1011_1010:1100_1110")),                                                // 27
    make_tuple(TInput(0x000000008164BACE, 27), bbv("001:0110_0100:1011_1010:1100_1110")),                                               // 28
    make_tuple(TInput(0x000000008164BACE, 28), bbv("0001:0110_0100:1011_1010:1100_1110")),                                              // 29
    make_tuple(TInput(0x000000008164BACE, 29), bbv("0_0001:0110_0100:1011_1010:1100_1110")),                                            // 30
    make_tuple(TInput(0x000000008164BACE, 30), bbv("00_0001:0110_0100:1011_1010:1100_1110")),                                           // 31
    make_tuple(TInput(0x000000008164BACE, 31), bbv("000_0001:0110_0100:1011_1010:1100_1110")),                                          // 32
    make_tuple(TInput(0x000000008164BACE, 32), bbv("1000_0001:0110_0100:1011_1010:1100_1110")),                                         // 33
    make_tuple(TInput(0xFaceB01d98765432, 33), bbv("1:1001_1000:0111_0110:0101_0100:0011_0010")),                                       // 34
    make_tuple(TInput(0xFaceB01d98765432, 34), bbv("01:1001_1000:0111_0110:0101_0100:0011_0010")),                                      // 35
    make_tuple(TInput(0xFaceB01d98765432, 35), bbv("101:1001_1000:0111_0110:0101_0100:0011_0010")),                                     // 36
    make_tuple(TInput(0xFaceB01d98765432, 36), bbv("1101:1001_1000:0111_0110:0101_0100:0011_0010")),                                    // 37
    make_tuple(TInput(0xFaceB01d98765432, 37), bbv("1_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                                  // 38
    make_tuple(TInput(0xFaceB01d98765432, 38), bbv("01_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                                 // 39
    make_tuple(TInput(0xFaceB01d98765432, 39), bbv("001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                                // 40
    make_tuple(TInput(0xFaceB01d98765432, 40), bbv("0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                               // 41
    make_tuple(TInput(0xFaceB01d98765432, 41), bbv("0:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                             // 42
    make_tuple(TInput(0xFaceB01d98765432, 42), bbv("00:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                            // 43
    make_tuple(TInput(0xFaceB01d98765432, 43), bbv("000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                           // 44
    make_tuple(TInput(0xFaceB01d98765432, 44), bbv("0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                          // 45
    make_tuple(TInput(0xFaceB01d98765432, 45), bbv("1_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                        // 46
    make_tuple(TInput(0xFaceB01d98765432, 46), bbv("11_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                       // 47
    make_tuple(TInput(0xFaceB01d98765432, 47), bbv("011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                      // 48
    make_tuple(TInput(0xFaceB01d98765432, 48), bbv("1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                     // 49
    make_tuple(TInput(0xFaceB01d98765432, 49), bbv("0:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                   // 50
    make_tuple(TInput(0xFaceB01d98765432, 50), bbv("10:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                  // 51
    make_tuple(TInput(0xFaceB01d98765432, 51), bbv("110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                 // 52
    make_tuple(TInput(0xFaceB01d98765432, 52), bbv("1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                // 53
    make_tuple(TInput(0xFaceB01d98765432, 53), bbv("0_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),              // 54
    make_tuple(TInput(0xFaceB01d98765432, 54), bbv("00_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),             // 55
    make_tuple(TInput(0xFaceB01d98765432, 55), bbv("100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),            // 56
    make_tuple(TInput(0xFaceB01d98765432, 56), bbv("1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),           // 57
    make_tuple(TInput(0xFaceB01d98765432, 57), bbv("0:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),         // 58
    make_tuple(TInput(0xFaceB01d98765432, 58), bbv("10:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),        // 59
    make_tuple(TInput(0xFaceB01d98765432, 59), bbv("010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),       // 60
    make_tuple(TInput(0xFaceB01d98765432, 60), bbv("1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),      // 61
    make_tuple(TInput(0xFaceB01d98765432, 61), bbv("1_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),    // 62
    make_tuple(TInput(0xFaceB01d98765432, 62), bbv("11_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),   // 63
    make_tuple(TInput(0xFaceB01d98765432, 63), bbv("111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),  // 64
    make_tuple(TInput(0xFaceB01d98765432, 64), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")), // 65
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}

//! Checks Append when sut is empty and adding from 1 to 64 bits from uint64_t (left aligned)
//!
//! @note Each time a new BinaryVector is used
void UT_BinaryVector::test_Append_1_to_64_bits_When_Empty_Left_Aligned ()
{
  // ---------------- DDT Setup
  //
  using TInput = tuple<uint64_t, uint8_t> ; // Value, Number of bits (taken from LSB)

  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto& input    = std::get<0>(data);
    const auto& expected = std::get<1>(data) ;
    auto value           = std::get<0>(input);
    auto numberOfBits    = std::get<1>(input);

    BinaryVector sut;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(value, numberOfBits, BitsAlignment::Left));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, expected);
  };

  auto bbv     = [](string_view asBinaryString) { return BinaryVector::CreateFromBinaryString(asBinaryString); };
  auto data =     // Value, bits, Expected
  {
    make_tuple(TInput(0xFaceB01d98765432, 1),  bbv("1")),                                                                               // 00
    make_tuple(TInput(0xFaceB01d98765432, 2),  bbv("11")),                                                                              // 01
    make_tuple(TInput(0xFaceB01d98765432, 3),  bbv("111")),                                                                             // 02
    make_tuple(TInput(0xFaceB01d98765432, 4),  bbv("1111")),                                                                            // 03
    make_tuple(TInput(0xFaceB01d98765432, 5),  bbv("1111_1")),                                                                          // 04
    make_tuple(TInput(0xFaceB01d98765432, 6),  bbv("1111_10")),                                                                         // 05
    make_tuple(TInput(0xFaceB01d98765432, 7),  bbv("1111_101")),                                                                        // 06
    make_tuple(TInput(0xFaceB01d98765432, 8),  bbv("1111_1010")),                                                                       // 07
    make_tuple(TInput(0xFaceB01d98765432, 9),  bbv("1111_1010:1")),                                                                     // 08
    make_tuple(TInput(0xFaceB01d98765432, 10), bbv("1111_1010:11")),                                                                    // 09
    make_tuple(TInput(0xFaceB01d98765432, 11), bbv("1111_1010:110")),                                                                   // 10
    make_tuple(TInput(0xFaceB01d98765432, 12), bbv("1111_1010:1100")),                                                                  // 11
    make_tuple(TInput(0xFaceB01d98765432, 13), bbv("1111_1010:1100_1")),                                                                // 12
    make_tuple(TInput(0xFaceB01d98765432, 14), bbv("1111_1010:1100_11")),                                                               // 13
    make_tuple(TInput(0xFaceB01d98765432, 15), bbv("1111_1010:1100_111")),                                                              // 14
    make_tuple(TInput(0xFaceB01d98765432, 16), bbv("1111_1010:1100_1110")),                                                             // 15
    make_tuple(TInput(0xFaceB01d98765432, 17), bbv("1111_1010:1100_1110:1")),                                                           // 16
    make_tuple(TInput(0xFaceB01d98765432, 18), bbv("1111_1010:1100_1110:10")),                                                          // 17
    make_tuple(TInput(0xFaceB01d98765432, 19), bbv("1111_1010:1100_1110:101")),                                                         // 18
    make_tuple(TInput(0xFaceB01d98765432, 20), bbv("1111_1010:1100_1110:1011")),                                                        // 19
    make_tuple(TInput(0xFaceB01d98765432, 21), bbv("1111_1010:1100_1110:1011_0")),                                                      // 20
    make_tuple(TInput(0xFaceB01d98765432, 22), bbv("1111_1010:1100_1110:1011_00")),                                                     // 21
    make_tuple(TInput(0xFaceB01d98765432, 23), bbv("1111_1010:1100_1110:1011_000")),                                                    // 22
    make_tuple(TInput(0xFaceB01d98765432, 24), bbv("1111_1010:1100_1110:1011_0000")),                                                   // 23
    make_tuple(TInput(0xFaceB01d98765432, 25), bbv("1111_1010:1100_1110:1011_0000:0")),                                                 // 24
    make_tuple(TInput(0xFaceB01d98765432, 26), bbv("1111_1010:1100_1110:1011_0000:00")),                                                // 25
    make_tuple(TInput(0xFaceB01d98765432, 27), bbv("1111_1010:1100_1110:1011_0000:000")),                                               // 26
    make_tuple(TInput(0xFaceB01d98765432, 28), bbv("1111_1010:1100_1110:1011_0000:0001")),                                              // 27
    make_tuple(TInput(0xFaceB01d98765432, 29), bbv("1111_1010:1100_1110:1011_0000:0001_1")),                                            // 28
    make_tuple(TInput(0xFaceB01d98765432, 30), bbv("1111_1010:1100_1110:1011_0000:0001_11")),                                           // 29
    make_tuple(TInput(0xFaceB01d98765432, 31), bbv("1111_1010:1100_1110:1011_0000:0001_110")),                                          // 30
    make_tuple(TInput(0xFaceB01d98765432, 32), bbv("1111_1010:1100_1110:1011_0000:0001_1101")),                                         // 31
    make_tuple(TInput(0xFaceB01d98765432, 33), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1")),                                       // 32
    make_tuple(TInput(0xFaceB01d98765432, 34), bbv("1111_1010:1100_1110:1011_0000:0001_1101:10")),                                      // 33
    make_tuple(TInput(0xFaceB01d98765432, 35), bbv("1111_1010:1100_1110:1011_0000:0001_1101:100")),                                     // 34
    make_tuple(TInput(0xFaceB01d98765432, 36), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001")),                                    // 35
    make_tuple(TInput(0xFaceB01d98765432, 37), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1")),                                  // 36
    make_tuple(TInput(0xFaceB01d98765432, 38), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_10")),                                 // 37
    make_tuple(TInput(0xFaceB01d98765432, 39), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_100")),                                // 38
    make_tuple(TInput(0xFaceB01d98765432, 40), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000")),                               // 39
    make_tuple(TInput(0xFaceB01d98765432, 41), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0")),                             // 40
    make_tuple(TInput(0xFaceB01d98765432, 42), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:01")),                            // 41
    make_tuple(TInput(0xFaceB01d98765432, 43), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:011")),                           // 42
    make_tuple(TInput(0xFaceB01d98765432, 44), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111")),                          // 43
    make_tuple(TInput(0xFaceB01d98765432, 45), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0")),                        // 44
    make_tuple(TInput(0xFaceB01d98765432, 46), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_01")),                       // 45
    make_tuple(TInput(0xFaceB01d98765432, 47), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_011")),                      // 46
    make_tuple(TInput(0xFaceB01d98765432, 48), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110")),                     // 47
    make_tuple(TInput(0xFaceB01d98765432, 49), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0")),                   // 48
    make_tuple(TInput(0xFaceB01d98765432, 50), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:01")),                  // 49
    make_tuple(TInput(0xFaceB01d98765432, 51), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:010")),                 // 50
    make_tuple(TInput(0xFaceB01d98765432, 52), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101")),                // 51
    make_tuple(TInput(0xFaceB01d98765432, 53), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0")),              // 52
    make_tuple(TInput(0xFaceB01d98765432, 54), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_01")),             // 53
    make_tuple(TInput(0xFaceB01d98765432, 55), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_010")),            // 54
    make_tuple(TInput(0xFaceB01d98765432, 56), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:")),          // 55
    make_tuple(TInput(0xFaceB01d98765432, 57), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0")),         // 56
    make_tuple(TInput(0xFaceB01d98765432, 58), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:00")),        // 57
    make_tuple(TInput(0xFaceB01d98765432, 59), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:001")),       // 58
    make_tuple(TInput(0xFaceB01d98765432, 60), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011")),      // 59
    make_tuple(TInput(0xFaceB01d98765432, 61), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0")),    // 60
    make_tuple(TInput(0xFaceB01d98765432, 62), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_00")),   // 61
    make_tuple(TInput(0xFaceB01d98765432, 63), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_001")),  // 62
    make_tuple(TInput(0xFaceB01d98765432, 64), bbv("1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")), // 63
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks Append when sut is not empty (from 1 to 64 bits) and adding from 1 to 64 bits (right aliqned) from uint64_t
//!
//! @note Each time a new BinaryVector is used
void UT_BinaryVector::test_Append_1_to_64_bits_When_NotEmpty_Right_Aligned ()
{
  // ---------------- DDT Setup
  //
  using TInput = tuple<BinaryVector, uint64_t, uint8_t> ; // Value, Number of bits (taken from LSB)

  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto& input    = std::get<0>(data);
    const auto& expected = std::get<1>(data);
    auto sut             = std::get<0>(input);
    auto value           = std::get<1>(input);
    auto numberOfBits    = std::get<2>(input);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(value, numberOfBits, BitsAlignment::Right));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, expected);
  };

  auto bbv = [](string_view asBinaryString) { return BinaryVector::CreateFromBinaryString(asBinaryString); };

  auto data =         // initial, Value, bits
  {
    make_tuple(TInput(bbv("0"),                 0x0000000000000000, 1),  bbv("0:0")),                                                                                       // 00
    make_tuple(TInput(bbv("1"),                 0x0000000000000001, 1),  bbv("1:1")),                                                                                       // 01
    make_tuple(TInput(bbv("10"),                0x0000000000000002, 2),  bbv("10:10")),                                                                                     // 02
    make_tuple(TInput(bbv("11"),                0x0000000000000003, 2),  bbv("11:11")),                                                                                     // 03
    make_tuple(TInput(bbv("100"),               0x0000000000000003, 3),  bbv("100:011")),                                                                                   // 04
    make_tuple(TInput(bbv("101"),               0x0000000000000009, 4),  bbv("101:1001")),                                                                                  // 05
    make_tuple(TInput(bbv("110"),               0x0000000000000011, 5),  bbv("110:1000_1")),                                                                                // 06
    make_tuple(TInput(bbv("111"),               0x0000000000000012, 6),  bbv("111:0100_10")),                                                                               // 07
    make_tuple(TInput(bbv("1000"),              0x000000000000004A, 7),  bbv("1000:1001_010")),                                                                             // 08
    make_tuple(TInput(bbv("1000_1"),            0x000000000000007B, 8),  bbv("1000_1:0111_1011")),                                                                          // 09
    make_tuple(TInput(bbv("1000_01"),           0x000000000000017B, 9),  bbv("1000_01:1:0111_1011")),                                                                       // 10
    make_tuple(TInput(bbv("1000_001"),          0x000000000000017B, 10), bbv("1000_001:01:0111_1011")),                                                                     // 11
    make_tuple(TInput(bbv("1000_0001"),         0x000000000000037B, 11), bbv("1000_0001:011:0111_1011")),                                                                   // 12
    make_tuple(TInput(bbv("1000_0001:0"),       0x000000000000037B, 12), bbv("1000_0001:0:0011:0111_1011")),                                                                // 13
    make_tuple(TInput(bbv("1000_0001:01"),      0x0000000000005ACE, 13), bbv("1000_0001:01:1:1010_1100:1110")),                                                             // 14
    make_tuple(TInput(bbv("1000_0001:011"),     0x0000000000005ACE, 14), bbv("1000_0001:011:01:1010_1100:1110")),                                                           // 15
    make_tuple(TInput(bbv("1000_0001:0110"),    0x0000000000005ACE, 15), bbv("1000_0001:0110:101:1010_1100:1110")),                                                         // 16
    make_tuple(TInput(bbv("1000_0001:0110_1"),  0x000000000000FACE, 16), bbv("1000_0001:0110_1:1111_1010:1100_1110")),                                                      // 17
    make_tuple(TInput(bbv("1000_0001:0110_11"), 0x000000008164BACE, 17), bbv("1000_0001:0110_11:0:1011_1010:1100_1110")),                                                   // 18
    make_tuple(TInput(bbv("1"),                 0x000000008164BACE, 18), bbv("1:00:1011_1010:1100_1110")),                                                                  // 19
    make_tuple(TInput(bbv("0"),                 0x000000008164BACE, 19), bbv("0:100:1011_1010:1100_1110")),                                                                 // 20
    make_tuple(TInput(bbv("1"),                 0x000000008164BACE, 20), bbv("1:0100:1011_1010:1100_1110")),                                                                // 21
    make_tuple(TInput(bbv("10"),                0x000000008164BACE, 21), bbv("10:0_0100:1011_1010:1100_1110")),                                                             // 22
    make_tuple(TInput(bbv("11"),                0x000000008164BACE, 22), bbv("11:10_0100:1011_1010:1100_1110")),                                                            // 23
    make_tuple(TInput(bbv("100"),               0x000000008164BACE, 23), bbv("100:110_0100:1011_1010:1100_1110")),                                                          // 24
    make_tuple(TInput(bbv("101"),               0x000000008164BACE, 24), bbv("101:0110_0100:1011_1010:1100_1110")),                                                         // 25
    make_tuple(TInput(bbv("110"),               0x000000008164BACE, 25), bbv("110:1:0110_0100:1011_1010:1100_1110")),                                                       // 26
    make_tuple(TInput(bbv("111"),               0x000000008164BACE, 26), bbv("111:01:0110_0100:1011_1010:1100_1110")),                                                      // 27
    make_tuple(TInput(bbv("1000"),              0x000000008164BACE, 27), bbv("1000:001:0110_0100:1011_1010:1100_1110")),                                                    // 28
    make_tuple(TInput(bbv("1000_1"),            0x000000008164BACE, 28), bbv("1000_1:0001:0110_0100:1011_1010:1100_1110")),                                                 // 29
    make_tuple(TInput(bbv("1000_01"),           0x000000008164BACE, 29), bbv("1000_01:0_0001:0110_0100:1011_1010:1100_1110")),                                              // 30
    make_tuple(TInput(bbv("1000_001"),          0x000000008164BACE, 30), bbv("1000_001:00_0001:0110_0100:1011_1010:1100_1110")),                                            // 31
    make_tuple(TInput(bbv("1000_0001"),         0x000000008164BACE, 31), bbv("1000_0001:000_0001:0110_0100:1011_1010:1100_1110")),                                          // 32
    make_tuple(TInput(bbv("1000_0001:0"),       0x000000008164BACE, 32), bbv("1000_0001:0:1000_0001:0110_0100:1011_1010:1100_1110")),                                       // 33
    make_tuple(TInput(bbv("1000_0001:01"),      0xFaceB01d98765432, 33), bbv("1000_0001:01:1:1001_1000:0111_0110:0101_0100:0011_0010")),                                    // 34
    make_tuple(TInput(bbv("1000_0001:011"),     0xFaceB01d98765432, 34), bbv("1000_0001:011:01:1001_1000:0111_0110:0101_0100:0011_0010")),                                  // 35
    make_tuple(TInput(bbv("1000_0001:0110"),    0xFaceB01d98765432, 35), bbv("1000_0001:0110:101:1001_1000:0111_0110:0101_0100:0011_0010")),                                // 36
    make_tuple(TInput(bbv("1000_0001:0110_1"),  0xFaceB01d98765432, 36), bbv("1000_0001:0110_1:1101:1001_1000:0111_0110:0101_0100:0011_0010")),                             // 37
    make_tuple(TInput(bbv("1000_0001:0110_11"), 0xFaceB01d98765432, 37), bbv("1000_0001:0110_11:1_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                          // 38
    make_tuple(TInput(bbv("1"),                 0xFaceB01d98765432, 38), bbv("1:01_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                                         // 39
    make_tuple(TInput(bbv("0"),                 0xFaceB01d98765432, 39), bbv("0:001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                                        // 40
    make_tuple(TInput(bbv("1"),                 0xFaceB01d98765432, 40), bbv("1:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                                       // 41
    make_tuple(TInput(bbv("10"),                0xFaceB01d98765432, 41), bbv("10:0:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                                    // 42
    make_tuple(TInput(bbv("11"),                0xFaceB01d98765432, 42), bbv("11:00:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                                   // 43
    make_tuple(TInput(bbv("100"),               0xFaceB01d98765432, 43), bbv("100:000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                                 // 44
    make_tuple(TInput(bbv("101"),               0xFaceB01d98765432, 44), bbv("101:0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                                // 45
    make_tuple(TInput(bbv("110"),               0xFaceB01d98765432, 45), bbv("110:1_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                              // 46
    make_tuple(TInput(bbv("111"),               0xFaceB01d98765432, 46), bbv("111:11_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                             // 47
    make_tuple(TInput(bbv("1000"),              0xFaceB01d98765432, 47), bbv("1000:011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                           // 48
    make_tuple(TInput(bbv("1000_1"),            0xFaceB01d98765432, 48), bbv("1000_1:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                        // 49
    make_tuple(TInput(bbv("1000_01"),           0xFaceB01d98765432, 49), bbv("1000_01:0:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                     // 50
    make_tuple(TInput(bbv("1000_001"),          0xFaceB01d98765432, 50), bbv("1000_001:10:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                   // 51
    make_tuple(TInput(bbv("1000_0001"),         0xFaceB01d98765432, 51), bbv("1000_0001:110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                 // 52
    make_tuple(TInput(bbv("1000_0001:0"),       0xFaceB01d98765432, 52), bbv("1000_0001:0:1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),              // 53
    make_tuple(TInput(bbv("1000_0001:01"),      0xFaceB01d98765432, 53), bbv("1000_0001:01:0_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),           // 54
    make_tuple(TInput(bbv("1000_0001:011"),     0xFaceB01d98765432, 54), bbv("1000_0001:011:00_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),         // 55
    make_tuple(TInput(bbv("1000_0001:0110"),    0xFaceB01d98765432, 55), bbv("1000_0001:0110:100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),       // 56
    make_tuple(TInput(bbv("1000_0001:0110_1"),  0xFaceB01d98765432, 56), bbv("1000_0001:0110_1:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),    // 57
    make_tuple(TInput(bbv("1000_0001:0110_11"), 0xFaceB01d98765432, 57), bbv("1000_0001:0110_11:0:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")), // 58
    make_tuple(TInput(bbv("1"),                 0xFaceB01d98765432, 58), bbv("1:10:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),                // 59
    make_tuple(TInput(bbv("0"),                 0xFaceB01d98765432, 59), bbv("0:010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),               // 60
    make_tuple(TInput(bbv("1"),                 0xFaceB01d98765432, 60), bbv("1:1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),              // 61
    make_tuple(TInput(bbv("10"),                0xFaceB01d98765432, 61), bbv("10:1_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),           // 62
    make_tuple(TInput(bbv("11"),                0xFaceB01d98765432, 62), bbv("11:11_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),          // 63
    make_tuple(TInput(bbv("100"),               0xFaceB01d98765432, 63), bbv("100:111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),        // 64
    make_tuple(TInput(bbv("101"),               0xFaceB01d98765432, 64), bbv("101:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),       // 65
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks Append when sut is not empty (from 1 to 312 bits) and adding from 1 to 64 bits (left aliqned) from uint64_t
//!
//! @note Each time a new BinaryVector is used
void UT_BinaryVector::test_Append_1_to_64_bits_When_NotEmpty_Left_Aligned ()
{
  // ---------------- DDT Setup
  //
  using TInput = tuple<BinaryVector, uint64_t, uint8_t> ; // Value, Number of bits (taken from LSB)

  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto& input    = std::get<0>(data);
    const auto& expected = std::get<1>(data);
    auto sut             = std::get<0>(input);
    auto value           = std::get<1>(input);
    auto numberOfBits    = std::get<2>(input);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Append(value, numberOfBits, BitsAlignment::Left));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, expected);
  };

  auto bbv = [](string_view asBinaryString) { return BinaryVector::CreateFromBinaryString(asBinaryString); };

  auto data =         // Initial, Value, bits
  {
    make_tuple(TInput(bbv("0"),                 0xFaceB01d98765432, 1),  bbv("0:1")),                                                                                         // 00
    make_tuple(TInput(bbv("1"),                 0xFaceB01d98765432, 2),  bbv("1:11")),                                                                                        // 01
    make_tuple(TInput(bbv("10"),                0xFaceB01d98765432, 3),  bbv("10:111")),                                                                                      // 02
    make_tuple(TInput(bbv("11"),                0xFaceB01d98765432, 4),  bbv("11:1111")),                                                                                     // 03
    make_tuple(TInput(bbv("100"),               0xFaceB01d98765432, 5),  bbv("100:1111_1")),                                                                                  // 04
    make_tuple(TInput(bbv("101"),               0xFaceB01d98765432, 6),  bbv("101:1111_10")),                                                                                 // 05
    make_tuple(TInput(bbv("110"),               0xFaceB01d98765432, 7),  bbv("110:1111_101")),                                                                                // 06
    make_tuple(TInput(bbv("111"),               0xFaceB01d98765432, 8),  bbv("111:1111_1010")),                                                                               // 07
    make_tuple(TInput(bbv("1000"),              0xFaceB01d98765432, 9),  bbv("1000:1111_1010:1")),                                                                            // 08
    make_tuple(TInput(bbv("1000_1"),            0xFaceB01d98765432, 10), bbv("1000_1:1111_1010:11")),                                                                         // 09
    make_tuple(TInput(bbv("1000_01"),           0xFaceB01d98765432, 11), bbv("1000_01:1111_1010:110")),                                                                       // 10
    make_tuple(TInput(bbv("1000_001"),          0xFaceB01d98765432, 12), bbv("1000_001:1111_1010:1100")),                                                                     // 11
    make_tuple(TInput(bbv("1000_0001"),         0xFaceB01d98765432, 13), bbv("1000_0001:1111_1010:1100_1")),                                                                  // 12
    make_tuple(TInput(bbv("1000_0001:0"),       0xFaceB01d98765432, 14), bbv("1000_0001:0:1111_1010:1100_11")),                                                               // 13
    make_tuple(TInput(bbv("1000_0001:01"),      0xFaceB01d98765432, 15), bbv("1000_0001:01:1111_1010:1100_111")),                                                             // 14
    make_tuple(TInput(bbv("1000_0001:011"),     0xFaceB01d98765432, 16), bbv("1000_0001:011:1111_1010:1100_1110")),                                                           // 15
    make_tuple(TInput(bbv("1000_0001:0110"),    0xFaceB01d98765432, 17), bbv("1000_0001:0110:1111_1010:1100_1110:1")),                                                        // 16
    make_tuple(TInput(bbv("1000_0001:0110_1"),  0xFaceB01d98765432, 18), bbv("1000_0001:0110_1:1111_1010:1100_1110:10")),                                                     // 17
    make_tuple(TInput(bbv("1000_0001:0110_11"), 0xFaceB01d98765432, 19), bbv("1000_0001:0110_11:1111_1010:1100_1110:101")),                                                   // 18
    make_tuple(TInput(bbv("1"),                 0xFaceB01d98765432, 20), bbv("1:1111_1010:1100_1110:1011")),                                                                  // 19
    make_tuple(TInput(bbv("0"),                 0xFaceB01d98765432, 21), bbv("0:1111_1010:1100_1110:1011_0")),                                                                // 20
    make_tuple(TInput(bbv("1"),                 0xFaceB01d98765432, 22), bbv("1:1111_1010:1100_1110:1011_00")),                                                               // 21
    make_tuple(TInput(bbv("10"),                0xFaceB01d98765432, 23), bbv("10:1111_1010:1100_1110:1011_000")),                                                             // 22
    make_tuple(TInput(bbv("11"),                0xFaceB01d98765432, 24), bbv("11:1111_1010:1100_1110:1011_0000")),                                                            // 23
    make_tuple(TInput(bbv("100"),               0xFaceB01d98765432, 25), bbv("100:1111_1010:1100_1110:1011_0000:0")),                                                         // 24
    make_tuple(TInput(bbv("101"),               0xFaceB01d98765432, 26), bbv("101:1111_1010:1100_1110:1011_0000:00")),                                                        // 25
    make_tuple(TInput(bbv("110"),               0xFaceB01d98765432, 27), bbv("110:1111_1010:1100_1110:1011_0000:000")),                                                       // 26
    make_tuple(TInput(bbv("111"),               0xFaceB01d98765432, 28), bbv("111:1111_1010:1100_1110:1011_0000:0001")),                                                      // 27
    make_tuple(TInput(bbv("1000"),              0xFaceB01d98765432, 29), bbv("1000:1111_1010:1100_1110:1011_0000:0001_1")),                                                   // 28
    make_tuple(TInput(bbv("1000_1"),            0xFaceB01d98765432, 30), bbv("1000_1:1111_1010:1100_1110:1011_0000:0001_11")),                                                // 29
    make_tuple(TInput(bbv("1000_01"),           0xFaceB01d98765432, 31), bbv("1000_01:1111_1010:1100_1110:1011_0000:0001_110")),                                              // 30
    make_tuple(TInput(bbv("1000_001"),          0xFaceB01d98765432, 32), bbv("1000_001:1111_1010:1100_1110:1011_0000:0001_1101")),                                            // 31
    make_tuple(TInput(bbv("1000_0001"),         0xFaceB01d98765432, 33), bbv("1000_0001:1111_1010:1100_1110:1011_0000:0001_1101:1")),                                         // 32
    make_tuple(TInput(bbv("1000_0001:0"),       0xFaceB01d98765432, 34), bbv("1000_0001:0:1111_1010:1100_1110:1011_0000:0001_1101:10")),                                      // 33
    make_tuple(TInput(bbv("1000_0001:01"),      0xFaceB01d98765432, 35), bbv("1000_0001:01:1111_1010:1100_1110:1011_0000:0001_1101:100")),                                    // 34
    make_tuple(TInput(bbv("1000_0001:011"),     0xFaceB01d98765432, 36), bbv("1000_0001:011:1111_1010:1100_1110:1011_0000:0001_1101:1001")),                                  // 35
    make_tuple(TInput(bbv("1000_0001:0110"),    0xFaceB01d98765432, 37), bbv("1000_0001:0110:1111_1010:1100_1110:1011_0000:0001_1101:1001_1")),                               // 36
    make_tuple(TInput(bbv("1000_0001:0110_1"),  0xFaceB01d98765432, 38), bbv("1000_0001:0110_1:1111_1010:1100_1110:1011_0000:0001_1101:1001_10")),                            // 37
    make_tuple(TInput(bbv("1000_0001:0110_11"), 0xFaceB01d98765432, 39), bbv("1000_0001:0110_11:1111_1010:1100_1110:1011_0000:0001_1101:1001_100")),                          // 38
    make_tuple(TInput(bbv("1"),                 0xFaceB01d98765432, 40), bbv("1:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000")),                                         // 39
    make_tuple(TInput(bbv("0"),                 0xFaceB01d98765432, 41), bbv("0:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0")),                                       // 40
    make_tuple(TInput(bbv("1"),                 0xFaceB01d98765432, 42), bbv("1:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:01")),                                      // 41
    make_tuple(TInput(bbv("10"),                0xFaceB01d98765432, 43), bbv("10:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:011")),                                    // 42
    make_tuple(TInput(bbv("11"),                0xFaceB01d98765432, 44), bbv("11:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111")),                                   // 43
    make_tuple(TInput(bbv("100"),               0xFaceB01d98765432, 45), bbv("100:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0")),                                // 44
    make_tuple(TInput(bbv("101"),               0xFaceB01d98765432, 46), bbv("101:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_01")),                               // 45
    make_tuple(TInput(bbv("110"),               0xFaceB01d98765432, 47), bbv("110:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_011")),                              // 46
    make_tuple(TInput(bbv("111"),               0xFaceB01d98765432, 48), bbv("111:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110")),                             // 47
    make_tuple(TInput(bbv("1000"),              0xFaceB01d98765432, 49), bbv("1000:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0")),                          // 48
    make_tuple(TInput(bbv("1000_1"),            0xFaceB01d98765432, 50), bbv("1000_1:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:01")),                       // 49
    make_tuple(TInput(bbv("1000_01"),           0xFaceB01d98765432, 51), bbv("1000_01:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:010")),                     // 50
    make_tuple(TInput(bbv("1000_001"),          0xFaceB01d98765432, 52), bbv("1000_001:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101")),                   // 51
    make_tuple(TInput(bbv("1000_0001"),         0xFaceB01d98765432, 53), bbv("1000_0001:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0")),                // 52
    make_tuple(TInput(bbv("1000_0001:0"),       0xFaceB01d98765432, 54), bbv("1000_0001:0:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_01")),             // 53
    make_tuple(TInput(bbv("1000_0001:01"),      0xFaceB01d98765432, 55), bbv("1000_0001:01:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_010")),           // 54
    make_tuple(TInput(bbv("1000_0001:011"),     0xFaceB01d98765432, 56), bbv("1000_0001:011:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:")),        // 55
    make_tuple(TInput(bbv("1000_0001:0110"),    0xFaceB01d98765432, 57), bbv("1000_0001:0110:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0")),      // 56
    make_tuple(TInput(bbv("1000_0001:0110_1"),  0xFaceB01d98765432, 58), bbv("1000_0001:0110_1:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:00")),   // 57
    make_tuple(TInput(bbv("1000_0001:0110_11"), 0xFaceB01d98765432, 59), bbv("1000_0001:0110_11:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:001")), // 58
    make_tuple(TInput(bbv("1"),                 0xFaceB01d98765432, 60), bbv("1:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011")),                // 59
    make_tuple(TInput(bbv("0"),                 0xFaceB01d98765432, 61), bbv("0:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0")),              // 60
    make_tuple(TInput(bbv("1"),                 0xFaceB01d98765432, 62), bbv("1:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_00")),             // 61
    make_tuple(TInput(bbv("10"),                0xFaceB01d98765432, 63), bbv("10:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_001")),           // 62
    make_tuple(TInput(bbv("11"),                0xFaceB01d98765432, 64), bbv("11:1111_1010:1100_1110:1011_0000:0001_1101:1001_1000:0111_0110:0101_0100:0011_0010")),          // 63
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
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
    make_tuple("1",            "",              "1"),                      // 00
    make_tuple("0",            "",              "0"),                      // 01
    make_tuple("1",            "1",             "11"),                     // 02
    make_tuple("01",           "101",           "0110_1"),                 // 03
    make_tuple("10",           "1001",          "1010_01"),                // 04
    make_tuple("101",          "1100_11",       "1011_1001_1"),            // 05
    make_tuple("1011",         "0100",          "1011_0100"),              // 06
    make_tuple("0110_0",       "1010_1111",     "0110_0101:0111_1"),       // 07
    make_tuple("0110_01",      "1111_0000:111", "0110_0111:1100_0011:1"),  // 08
    make_tuple("0110_011",     "0110_1001:0",   "0110_0110:1101_0010"),    // 09
    make_tuple("0110_0111",    "1011_1101",     "0110_0111:1011_1101"),    // 10
    make_tuple("1110_0000:1",  "0001_0010:1",   "1110_0000:1000_1001:01"), // 11
    make_tuple("1110_0000:11", "0",             "1110_0000:110"),          // 12
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}

//! Checks Append when sut size has been fixed (at construction)
//!
void UT_BinaryVector::test_Append_Other_When_FixedSize ()
{
  // ---------------- Setup
  //
  BinaryVector sut   (12, 0x00, SizeProperty::Fixed);
  BinaryVector other (12, 0xFF);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Append(other), std::exception);
}



//! Checks BinaryVector::Operator<< when the sut is still empty
//!
void UT_BinaryVector::test_Operator_Shift_When_Empty ()
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
    auto& sutRef = sut << other;

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, other);
    TS_ASSERT_EQUALS (sut, sutRef);
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


//! Checks BinaryVector::Operator<< when the sut is not empty
//!
void UT_BinaryVector::test_Operator_Shift_When_NotEmpty ()
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
    TS_ASSERT_THROWS_NOTHING (sut << other);

    // ---------------- Verify
    //
    auto expected = BinaryVector::CreateFromBinaryString(expectedBits);

    TS_ASSERT_EQUALS (sut, expected);
  };

  auto data =
  {
    //   Bits: sut,            other,   result
    make_tuple("1",            "",              "1"),                      // 00
    make_tuple("0",            "",              "0"),                      // 01
    make_tuple("1",            "1",             "11"),                     // 02
    make_tuple("01",           "101",           "0110_1"),                 // 03
    make_tuple("10",           "1001",          "1010_01"),                // 04
    make_tuple("101",          "1100_11",       "1011_1001_1"),            // 05
    make_tuple("1011",         "0100",          "1011_0100"),              // 06
    make_tuple("0110_0",       "1010_1111",     "0110_0101:0111_1"),       // 07
    make_tuple("0110_01",      "1111_0000:111", "0110_0111:1100_0011:1"),  // 08
    make_tuple("0110_011",     "0110_1001:0",   "0110_0110:1101_0010"),    // 09
    make_tuple("0110_0111",    "1011_1101",     "0110_0111:1011_1101"),    // 10
    make_tuple("1110_0000:1",  "0001_0010:1",   "1110_0000:1000_1001:01"), // 11
    make_tuple("1110_0000:11", "0",             "1110_0000:110"),          // 12
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector::Operator<< when cascaded
//!
void UT_BinaryVector::test_Operator_Shift_When_Cascaded ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    string_view sutBits      = std::get<0>(data);
    string_view otherBits_1  = std::get<1>(data);
    string_view otherBits_2  = std::get<2>(data);
    string_view expectedBits = std::get<3>(data);

    auto sut     = BinaryVector::CreateFromBinaryString(sutBits);
    auto other_1 = BinaryVector::CreateFromBinaryString(otherBits_1);
    auto other_2 = BinaryVector::CreateFromBinaryString(otherBits_2);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut << other_1 << other_2);

    // ---------------- Verify
    //
    auto expected = BinaryVector::CreateFromBinaryString(expectedBits);

    TS_ASSERT_EQUALS (sut, expected);
  };

  auto data =
  {
    //   Bits: sut,            other_1,         other_2,         result
    make_tuple("1",            "",              "",              "1"),                          // 00
    make_tuple("0",            "",              "1",             "01"),                         // 01
    make_tuple("1",            "1",             "",              "11"),                         // 02
    make_tuple("01",           "101",           "01",            "0110_101"),                   // 03
    make_tuple("10",           "1001",          "001",           "1010_0100:1"),                // 04
    make_tuple("101",          "1100_11",       "11",            "1011_1001:111"),              // 05
    make_tuple("1011",         "0100",          "0",             "1011_0100:0"),                // 06
    make_tuple("0110_0",       "1010_1111",     "0000_1111",     "0110_0101:0111_1000:0111_1"), // 07
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}

//! Checks shift operator when sut size has been fixed (at construction)
//!
void UT_BinaryVector::test_Operator_Shift_When_FixedSize ()
{
  // ---------------- Setup
  //
  BinaryVector sut   (12, 0x00, SizeProperty::Fixed);
  BinaryVector other (12, 0xFF);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut << other, std::exception);
}

//! Checks BinaryVector::Operator+ when the sut is still empty
//!
void UT_BinaryVector::test_Operator_Plus_When_Empty ()
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
    auto result = sut + other;

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (result, other);
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


//! Checks BinaryVector::Operator+ when the sut is not empty
//!
void UT_BinaryVector::test_Operator_Plus_When_NotEmpty ()
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
    auto result = sut + other;

    // ---------------- Verify
    //
    auto expected = BinaryVector::CreateFromBinaryString(expectedBits);

    TS_ASSERT_EQUALS (result, expected);
  };

  auto data =
  {
    //   Bits: sut,                                     other,                                     result
    make_tuple("1",                                      "",                                       "1"),                                                                             // 00
    make_tuple("0",                                      "",                                       "0"),                                                                             // 01
    make_tuple("1",                                      "1",                                      "11"),                                                                            // 02
    make_tuple("01",                                     "101",                                    "0110_1"),                                                                        // 03
    make_tuple("10",                                     "1001",                                   "1010_01"),                                                                       // 04
    make_tuple("101",                                    "1100_11",                                "1011_1001_1"),                                                                   // 05
    make_tuple("1011",                                   "0100",                                   "1011_0100"),                                                                     // 06
    make_tuple("0110_0",                                 "1010_1111",                              "0110_0101:0111_1"),                                                              // 07
    make_tuple("0110_01",                                "1111_0000:111",                          "0110_0111:1100_0011:1"),                                                         // 08
    make_tuple("0110_011",                               "0110_1001:0",                            "0110_0110:1101_0010"),                                                           // 09
    make_tuple("0110_0111",                              "1011_1101",                              "0110_0111:1011_1101"),                                                           // 10
    make_tuple("1110_0000:1",                            "0001_0010:1",                            "1110_0000:1000_1001:01"),                                                        // 11
    make_tuple("1110_0000:11",                           "0",                                      "1110_0000:110"),                                                                 // 12
    make_tuple("1110_0000:0011_1",                       "1110_0000:1000_1001:01",                 "1110_0000:0011_1111:0000_0100:0100_101"),                                        // 13
    make_tuple("1110_0000:0011_1111:0000_0100:0100_101", "1110_0000:0011_1111:0000_0100:0100_101", "1110_0000:0011_1111:0000_0100:0100_101:1110_0000:0011_1111:0000_0100:0100_101"), // 14
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks addition operator when both operand have their size fixed (at construction)
//!
void UT_BinaryVector::test_Operator_Plus_When_FixedSize ()
{
  // ---------------- Setup
  //
  BinaryVector lhs (7, 0x00, SizeProperty::Fixed);
  BinaryVector rhs (3, 0xFF, SizeProperty::Fixed);

  // ---------------- Exercise
  //
  auto result = lhs + rhs;

  // ---------------- Verify
  //
  TS_ASSERT_FALSE  (result.HasFixedSize());

  auto expected = BinaryVector::CreateFromBinaryString("0000_0001:11");

  TS_ASSERT_EQUALS (result, expected);
}


//! Checks BinaryVector::FixSize() when it was initially not fixed
//!
void UT_BinaryVector::test_FixSize_When_WasNotFixed ()
{
  // ---------------- Setup
  //
  BinaryVector sut(12, 3);

  // ---------------- Exercise
  //
  sut.FixSize(true);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.HasFixedSize());
  TS_ASSERT_THROWS (sut.Set(8U), std::exception);
}

//! Checks BinaryVector::FixSize() when it was initially fixed
//!
void UT_BinaryVector::test_FixSize_When_WasFixed ()
{
  // ---------------- Setup
  //
  BinaryVector sut(12, 2, SizeProperty::Fixed);

  // ---------------- Exercise
  //
  sut.FixSize(false);

  // ---------------- Verify
  //
  TS_ASSERT_FALSE          (sut.HasFixedSize());
  TS_ASSERT_THROWS_NOTHING (sut.Set(8U));
}

//! Checks assigment operator
//!
void UT_BinaryVector::test_CopyAssignmentOperator ()
{
  // ---------------- Setup
  //
  BinaryVector sut   (3, 0x00);
  BinaryVector other (4, 0xFF);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut = other);

  // ---------------- Verify
  //
  TS_ASSERT_FALSE  (sut.HasFixedSize());
  TS_ASSERT_EQUALS (sut, other);
}


//! Checks assigment operator when sut has its size fixed and other has same size
//!
void UT_BinaryVector::test_CopyAssignmentOperator_When_FixedSize_SameSize ()
{
  // ---------------- Setup
  //
  BinaryVector sut   (3, 0x00, SizeProperty::Fixed);
  BinaryVector other (3, 0xFF);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut = other);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.HasFixedSize());
  TS_ASSERT_EQUALS (sut, other);
}

//! Checks assigment operator when sut has its size fixed (and other has not)
//!
void UT_BinaryVector::test_CopyAssignmentOperator_When_FixedSize_DifferentSize ()
{
  // ---------------- Setup
  //
  BinaryVector sut   (3, 0x00, SizeProperty::Fixed);
  BinaryVector other (2, 0xFF);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS (sut = other, std::exception);
}

//! Checks copy assigment operator when other has its size fixed
//!
void UT_BinaryVector::test_CopyAssignmentOperator_From_FixedSize ()
{
  // ---------------- Setup
  //
  BinaryVector sut   (3, 0x00);
  BinaryVector other (4, 0xFF, SizeProperty::Fixed);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut = other);

  // ---------------- Verify
  //
  TS_ASSERT_FALSE  (sut.HasFixedSize());
  TS_ASSERT_EQUALS (sut, other);
}


//! Checks copy assigment operator when other has its size fixed (the property must be copied)
//!
void UT_BinaryVector::test_CopyAssignmentOperator_From_FixedSizeOnCopy ()
{
  // ---------------- Setup
  //
  BinaryVector sut   (3, 0x00);
  BinaryVector other (4, 0xFF, SizeProperty::FixedOnCopy);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut = other);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.HasFixedSize());
  TS_ASSERT_EQUALS (sut, other);
}


//! Checks assigment operator
//!
void UT_BinaryVector::test_MoveAssignmentOperator ()
{
  // ---------------- Setup
  //
  BinaryVector sut (3, 0x00);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut = BinaryVector(4, 0xFF, SizeProperty::Fixed));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE  (sut.HasFixedSize());

  BinaryVector expected = BinaryVector(4, 0xFF);
  TS_ASSERT_EQUALS (sut, expected);
}


//! Checks move assigment operator when sut has its size fixed and other has same size
//!
void UT_BinaryVector::test_MoveAssignmentOperator_When_FixedSize_SameSize ()
{
  // ---------------- Setup
  //
  BinaryVector sut   (3, 0x00, SizeProperty::Fixed);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut = BinaryVector(3, 0xFF, SizeProperty::Fixed));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.HasFixedSize());

  BinaryVector expected = BinaryVector(3, 0xFF);
  TS_ASSERT_EQUALS (sut, expected);
}

//! Checks move assigment operator when sut has its size fixed (and other has not)
//!
void UT_BinaryVector::test_MoveAssignmentOperator_When_FixedSize_DifferentSize ()
{
  // ---------------- Setup
  //
  BinaryVector sut   (3, 0x00, SizeProperty::Fixed);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut = BinaryVector(4, 0xFF), std::exception);
}

//! Checks move assigment operator when other has its size fixed
//!
void UT_BinaryVector::test_MoveAssignmentOperator_From_FixedSize ()
{
  // ---------------- Setup
  //
  BinaryVector sut   (3, 0x00);
  BinaryVector other (4, 0xFF, SizeProperty::Fixed);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut = std::move(other));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE  (sut.HasFixedSize());

  BinaryVector expected = BinaryVector(4, 0xFF);
  TS_ASSERT_EQUALS (sut, expected);

  TS_ASSERT_TRUE (other.IsEmpty());
}


//! Checks move assigment operator when other has its size fixed that must be copied
//!
void UT_BinaryVector::test_MoveAssignmentOperator_From_FixedSizeOnCopy ()
{
  // ---------------- Setup
  //
  BinaryVector sut   (3, 0x00);
  BinaryVector other (4, 0xFF, SizeProperty::FixedOnCopy);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut = std::move(other));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE  (sut.HasFixedSize());

  BinaryVector expected = BinaryVector(4, 0xFF);
  TS_ASSERT_EQUALS (sut, expected);

  TS_ASSERT_TRUE (other.IsEmpty());
}



//! Checks BinaryVector::ToggleBits()
//!
//! @note Require DataAsBinaryString to work properly
//!
void UT_BinaryVector::test_ToggleBits ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    string_view sutBits  = std::get<0>(data);
    string      expected = std::get<1>(data);

    auto sut   = BinaryVector::CreateFromBinaryString(sutBits);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.ToggleBits());

    // ---------------- Verify
    //
    auto sutBit = sut.DataAsBinaryString();
    TS_ASSERT_EQUALS (sutBit, expected);
  };

  auto data =
  {
    //   Bits: sut,                            result
    make_tuple("1",                            "0"),                            // 00
    make_tuple("0",                            "1"),                            // 01
    make_tuple("11",                           "00"),                           // 02
    make_tuple("01",                           "10"),                           // 03
    make_tuple("1001",                         "0110"),                         // 04
    make_tuple("1011_1",                       "0100_0"),                       // 05
    make_tuple("1011",                         "0100"),                         // 06
    make_tuple("0110_0",                       "1001_1"),                       // 07
    make_tuple("0110_01",                      "1001_10"),                      // 08
    make_tuple("0110_011",                     "1001_100"),                     // 09
    make_tuple("0110_0111",                    "1001_1000"),                    // 10
    make_tuple("1110_0000:1",                  "0001_1111:0"),                  // 11
    make_tuple("1110_0000:11",                 "0001_1111:00"),                 // 12
    make_tuple("1110_0000:1100_1011:1010",     "0001_1111:0011_0100:0101"),     // 13
    make_tuple("1110_0000:1100_1011:1010_1",   "0001_1111:0011_0100:0101_0"),   // 14
    make_tuple("1110_0000:1100_1011:1010_101", "0001_1111:0011_0100:0101_010"), // 15
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector::ToggleBits()
//!
//! @note Require DataAsBinaryString to work properly
//!
void UT_BinaryVector::test_ToggleBits_Using_HexString ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    string_view sutBits  = std::get<0>(data);
    string      expected = std::get<1>(data);

    auto sut   = BinaryVector::CreateFromHexString(sutBits);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.ToggleBits());

    // ---------------- Verify
    //
    auto sutBit = sut.DataAsHexString();
    TS_ASSERT_EQUALS (sutBit, expected);
  };

  auto data =
  {
    //   Bits: sut,                    result
    make_tuple("0",                    "F"),                      // 00
    make_tuple("F",                    "0"),                      // 01
    make_tuple("81",                   "7E"),                     // 02
    make_tuple("5A2",                  "A5D"),                    // 03
    make_tuple("6B34",                 "94CB"),                   // 04
    make_tuple("7C89_0",               "8376_F"),                 // 05
    make_tuple("8D00_1E",              "72FF_E1"),                // 06
    make_tuple("0123456789:ABCDEF:A5", "FEDC_BA98:7654_3210:5A"), // 15
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}



//! Checks BinaryVector::ToggleBits()
//!
//! @note Require DataAsBinaryString to work properly
//!
void UT_BinaryVector::test_Operator_Tilde ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    string_view sutBits  = std::get<0>(data);
    string      expected = std::get<1>(data);

    auto sut   = BinaryVector::CreateFromBinaryString(sutBits);

    // ---------------- Exercise
    //
    auto result = ~sut;

    // ---------------- Verify
    //
    auto resultBit = result.DataAsBinaryString();
    TS_ASSERT_EQUALS (resultBit, expected);
  };

  auto data =
  {
    //   Bits: sut,                            result
    make_tuple("1",                            "0"),                            // 00
    make_tuple("0",                            "1"),                            // 01
    make_tuple("11",                           "00"),                           // 02
    make_tuple("01",                           "10"),                           // 03
    make_tuple("1001",                         "0110"),                         // 04
    make_tuple("1011_1",                       "0100_0"),                       // 05
    make_tuple("1011",                         "0100"),                         // 06
    make_tuple("0110_0",                       "1001_1"),                       // 07
    make_tuple("0110_01",                      "1001_10"),                      // 08
    make_tuple("0110_011",                     "1001_100"),                     // 09
    make_tuple("0110_0111",                    "1001_1000"),                    // 10
    make_tuple("1110_0000:1",                  "0001_1111:0"),                  // 11
    make_tuple("1110_0000:11",                 "0001_1111:00"),                 // 12
    make_tuple("1110_0000:1100_1011:1010",     "0001_1111:0011_0100:0101"),     // 13
    make_tuple("1110_0000:1100_1011:1010_1",   "0001_1111:0011_0100:0101_0"),   // 14
    make_tuple("1110_0000:1100_1011:1010_101", "0001_1111:0011_0100:0101_010"), // 15
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector::operator &=
//!
void UT_BinaryVector::test_Operator_Bitwise_And_Assignment ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto sut      = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    auto rhs      = BinaryVector::CreateFromBinaryString(std::get<1>(data));
    auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    // ---------------- Exercise
    //
    sut &= rhs;

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, expected);
  };

  auto data =
  {
    //   Bits: sut,                          other                         result
    make_tuple("",                           "",                           ""),                           // 00
    make_tuple("1",                          "1",                          "1"),                          // 01
    make_tuple("0",                          "0",                          "0"),                          // 02
    make_tuple("11",                         "10",                         "10"),                         // 03
    make_tuple("01",                         "10",                         "00"),                         // 04
    make_tuple("1001",                       "1111",                       "1001"),                       // 05
    make_tuple("1011_1",                     "0110_0",                     "0010_0"),                     // 06
    make_tuple("1110_0000:1",                "1010_1010:1",                "1010_0000:1"),                // 07
    make_tuple("1110_0000:1100_1011:1010",   "0001_1111:0011_0100:0101",   "0000_0000:0000_0000:0000"),   // 08
    make_tuple("1110_0000:1100_1011:1010_1", "1010_0111:1100_0011:1010_0", "1010_0000:1100_0011:1010_0"), // 09
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector::operator |=
//!
void UT_BinaryVector::test_Operator_Bitwise_Or_Assignment ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto sut      = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    auto rhs      = BinaryVector::CreateFromBinaryString(std::get<1>(data));
    auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    // ---------------- Exercise
    //
    sut |= rhs;

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, expected);
  };

  auto data =
  {
    //   Bits: sut,                          other                         result
    make_tuple("",                           "",                           ""),                           // 00
    make_tuple("1",                          "1",                          "1"),                          // 01
    make_tuple("0",                          "0",                          "0"),                          // 02
    make_tuple("11",                         "10",                         "11"),                         // 03
    make_tuple("01",                         "10",                         "11"),                         // 04
    make_tuple("1001",                       "1111",                       "1111"),                       // 05
    make_tuple("1011_0",                     "0110_0",                     "1111_0"),                     // 06
    make_tuple("1110_0000:1",                "1010_1010:1",                "1110_1010:1"),                // 07
    make_tuple("1110_0000:1100_1011:1010",   "0001_1111:0011_0100:0001",   "1111_1111:1111_1111:1011"),   // 08
    make_tuple("1110_0000:1100_1011:1010_1", "1010_0111:1100_0011:1010_0", "1110_0111:1100_1011:1010_1"), // 09
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}




//! Checks BinaryVector::operator ^=
//!
void UT_BinaryVector::test_Operator_Bitwise_Xor_Assignment ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto sut      = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    auto rhs      = BinaryVector::CreateFromBinaryString(std::get<1>(data));
    auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    // ---------------- Exercise
    //
    sut ^= rhs;

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, expected);
  };

  auto data =
  {
    //   Bits: sut,                          other                         result
    make_tuple("",                           "",                           ""),                           // 00
    make_tuple("1",                          "1",                          "0"),                          // 01
    make_tuple("0",                          "0",                          "0"),                          // 02
    make_tuple("11",                         "10",                         "01"),                         // 03
    make_tuple("01",                         "10",                         "11"),                         // 04
    make_tuple("1001",                       "1111",                       "0110"),                       // 05
    make_tuple("1011_0",                     "0110_0",                     "1101_0"),                     // 06
    make_tuple("1110_0000:1",                "1010_1010:1",                "0100_1010:0"),                // 07
    make_tuple("1110_0000:1100_1011:1010",   "0001_1111:0011_0100:0001",   "1111_1111:1111_1111:1011"),   // 08
    make_tuple("1110_0000:1100_1011:1010_1", "1010_0111:1100_0011:1010_0", "0100_0111:0000_1000:0000_1"), // 09
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector::operator &= when vectors have incompatible sizes
//!
void UT_BinaryVector::test_Operator_Bitwise_And_Assignment_SizeMismatch ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto sut = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    auto rhs = BinaryVector::CreateFromBinaryString(std::get<1>(data));

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut &= rhs, std::exception);
  };

  auto data =
  {
    //  Bits:  sut,  other
    make_tuple("",   "1"),   // 00
    make_tuple("1",  ""),    // 01
    make_tuple("01", "0"),   // 02
    make_tuple("11", "110"), // 03
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector::operator |= when vectors have incompatible sizes
//!
void UT_BinaryVector::test_Operator_Bitwise_Or_Assignment_SizeMismatch ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto sut = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    auto rhs = BinaryVector::CreateFromBinaryString(std::get<1>(data));

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut |= rhs, std::exception);
  };

  auto data =
  {
    //  Bits:  sut,  other
    make_tuple("",   "1"),   // 00
    make_tuple("1",  ""),    // 01
    make_tuple("01", "0"),   // 02
    make_tuple("11", "110"), // 03
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector::operator ^= when vectors have incompatible sizes
//!
void UT_BinaryVector::test_Operator_Bitwise_Xor_Assignment_SizeMismatch ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto sut = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    auto rhs = BinaryVector::CreateFromBinaryString(std::get<1>(data));

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut ^= rhs, std::exception);
  };

  auto data =
  {
    //  Bits:  sut,  other
    make_tuple("",   "1"),   // 00
    make_tuple("1",  ""),    // 01
    make_tuple("01", "0"),   // 02
    make_tuple("11", "110"), // 03
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector::operator &=
//!
void UT_BinaryVector::test_Operator_Bitwise_And ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto lhs      = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    auto rhs      = BinaryVector::CreateFromBinaryString(std::get<1>(data));
    auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    // ---------------- Exercise
    //
    auto result = lhs & rhs;

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (result, expected);
  };

  auto data =
  {
    //   Bits: lhs,                          other                         result
    make_tuple("",                           "",                           ""),                           // 00
    make_tuple("1",                          "1",                          "1"),                          // 01
    make_tuple("0",                          "0",                          "0"),                          // 02
    make_tuple("11",                         "10",                         "10"),                         // 03
    make_tuple("01",                         "10",                         "00"),                         // 04
    make_tuple("1001",                       "1111",                       "1001"),                       // 05
    make_tuple("1011_1",                     "0110_0",                     "0010_0"),                     // 06
    make_tuple("1110_0000:1",                "1010_1010:1",                "1010_0000:1"),                // 07
    make_tuple("1110_0000:1100_1011:1010",   "0001_1111:0011_0100:0101",   "0000_0000:0000_0000:0000"),   // 08
    make_tuple("1110_0000:1100_1011:1010_1", "1010_0111:1100_0011:1010_0", "1010_0000:1100_0011:1010_0"), // 09
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector::operator |=
//!
void UT_BinaryVector::test_Operator_Bitwise_Or ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto lhs      = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    auto rhs      = BinaryVector::CreateFromBinaryString(std::get<1>(data));
    auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    // ---------------- Exercise
    //
    auto result = lhs | rhs;

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (result, expected);
  };

  auto data =
  {
    //   Bits: lhs,                          other                         result
    make_tuple("",                           "",                           ""),                           // 00
    make_tuple("1",                          "1",                          "1"),                          // 01
    make_tuple("0",                          "0",                          "0"),                          // 02
    make_tuple("11",                         "10",                         "11"),                         // 03
    make_tuple("01",                         "10",                         "11"),                         // 04
    make_tuple("1001",                       "1111",                       "1111"),                       // 05
    make_tuple("1011_0",                     "0110_0",                     "1111_0"),                     // 06
    make_tuple("1110_0000:1",                "1010_1010:1",                "1110_1010:1"),                // 07
    make_tuple("1110_0000:1100_1011:1010",   "0001_1111:0011_0100:0001",   "1111_1111:1111_1111:1011"),   // 08
    make_tuple("1110_0000:1100_1011:1010_1", "1010_0111:1100_0011:1010_0", "1110_0111:1100_1011:1010_1"), // 09
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}




//! Checks BinaryVector::operator ^=
//!
void UT_BinaryVector::test_Operator_Bitwise_Xor ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto lhs      = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    auto rhs      = BinaryVector::CreateFromBinaryString(std::get<1>(data));
    auto expected = BinaryVector::CreateFromBinaryString(std::get<2>(data));

    // ---------------- Exercise
    //
    auto result = lhs ^ rhs;

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (result, expected);
  };

  auto data =
  {
    //   Bits: lhs,                          other                         result
    make_tuple("",                           "",                           ""),                           // 00
    make_tuple("1",                          "1",                          "0"),                          // 01
    make_tuple("0",                          "0",                          "0"),                          // 02
    make_tuple("11",                         "10",                         "01"),                         // 03
    make_tuple("01",                         "10",                         "11"),                         // 04
    make_tuple("1001",                       "1111",                       "0110"),                       // 05
    make_tuple("1011_0",                     "0110_0",                     "1101_0"),                     // 06
    make_tuple("1110_0000:1",                "1010_1010:1",                "0100_1010:0"),                // 07
    make_tuple("1110_0000:1100_1011:1010",   "0001_1111:0011_0100:0001",   "1111_1111:1111_1111:1011"),   // 08
    make_tuple("1110_0000:1100_1011:1010_1", "1010_0111:1100_0011:1010_0", "0100_0111:0000_1000:0000_1"), // 09
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector::operator &= when vectors have incompatible sizes
//!
void UT_BinaryVector::test_Operator_Bitwise_And_SizeMismatch ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto lhs = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    auto rhs = BinaryVector::CreateFromBinaryString(std::get<1>(data));

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (lhs & rhs, std::exception);
  };

  auto data =
  {
    //  Bits:  lhs,  other
    make_tuple("",   "1"),   // 00
    make_tuple("1",  ""),    // 01
    make_tuple("01", "0"),   // 02
    make_tuple("11", "110"), // 03
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector::operator |= when vectors have incompatible sizes
//!
void UT_BinaryVector::test_Operator_Bitwise_Or_SizeMismatch ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto lhs = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    auto rhs = BinaryVector::CreateFromBinaryString(std::get<1>(data));

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (lhs | rhs, std::exception);
  };

  auto data =
  {
    //  Bits:  lhs,  other
    make_tuple("",   "1"),   // 00
    make_tuple("1",  ""),    // 01
    make_tuple("01", "0"),   // 02
    make_tuple("11", "110"), // 03
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector::operator ^= when vectors have incompatible sizes
//!
void UT_BinaryVector::test_Operator_Bitwise_Xor_SizeMismatch ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto lhs = BinaryVector::CreateFromBinaryString(std::get<0>(data));
    auto rhs = BinaryVector::CreateFromBinaryString(std::get<1>(data));

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (lhs ^ rhs, std::exception);
  };

  auto data =
  {
    //  Bits:  lhs,  other
    make_tuple("",   "1"),   // 00
    make_tuple("1",  ""),    // 01
    make_tuple("01", "0"),   // 02
    make_tuple("11", "110"), // 03
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}



//! Checks BinaryVector::Slice()
//!
void UT_BinaryVector::test_Slice ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    string_view sutBits      = std::get<0>(data);
    uint32_t    firstBit     = std::get<1>(data);
    uint32_t    bitsCount    = std::get<2>(data);
    string_view expectedBits = std::get<3>(data);

    auto sut      = BinaryVector::CreateFromBinaryString(sutBits);
    auto expected = BinaryVector::CreateFromBinaryString(expectedBits);

    // ---------------- Exercise
    //
    auto result = sut.Slice(firstBit, bitsCount);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (result, expected);
  };

  auto data =
  {
    //   Bits: sut,                              firstBit, bitsCount, expected
    make_tuple("",                                            0,  0,  ""),                       // 00
    make_tuple("",                                            1,  0,  ""),                       // 01
    make_tuple("0",                                           0,  1,  "0"),                      // 02
    make_tuple("1",                                           0,  1,  "1"),                      // 03
    make_tuple("10",                                          0,  1,  "1"),                      // 04
    make_tuple("10",                                          0,  2,  "10"),                     // 05
    make_tuple("1011",                                        1,  1,  "0"),                      // 06
    make_tuple("1001",                                        1,  2,  "00"),                     // 07
    make_tuple("1011_0",                                      1,  3,  "011"),                    // 08
    make_tuple("1011_1",                                      1,  4,  "0111"),                   // 09
    make_tuple("1011_1101:101",                               1,  5,  "0111_1"),                 // 10
    make_tuple("1011_1101:101",                               1,  6,  "0111_10"),                // 11
    make_tuple("1011_1101:101",                               1,  7,  "0111_101"),               // 12
    make_tuple("1011_1101:101",                               1,  8,  "0111_1011"),              // 13
    make_tuple("1011_0101:101",                               2,  3,  "110"),                    // 14
    make_tuple("1011_1101:101",                               2,  5,  "1111_0"),                 // 15
    make_tuple("1011_1101:101",                               2,  6,  "1111_01"),                // 16
    make_tuple("1011_1101:101",                               2,  7,  "1111_011"),               // 17
    make_tuple("1011_1101:101",                               2,  8,  "1111_0110"),              // 18
    make_tuple("1011_1101:101",                               2,  9,  "1111_0110:1"),            // 19
    make_tuple("1011_1101:1011_1110:0101",                    3,  10, "1110_1101:11"),           // 20
    make_tuple("1011_1101:1011_1110:0101",                    4,  10, "1101_1011:11"),           // 21
    make_tuple("1011_1101:1011_1110:0101",                    5,  11, "1011_0111:110"),          // 22
    make_tuple("1011_1101:1011_1110:0101",                    6,  12, "0110_1111:1001"),         // 23
    make_tuple("1011_1101:1011_1110:0101",                    7,  12, "1101_1111:0010"),         // 24
    make_tuple("1011_1101:1011_1110:0101_1001",               8,  13, "1011_1110:0101_1"),       // 25
    make_tuple("1001_1010:1011_1100:1101_1110",               9,  14, "0111_1001:1011_11"),      // 26
    make_tuple("1011_1101:1011_1110:0101_1001:0101_1010:11",  14, 15, "1001_0110:0101_011"),     // 27
    make_tuple("1011_1101:1011_1110:0101_1001:0101_1010:11",  15, 16, "0010_1100:1010_1101"),    // 28
    make_tuple("1011_1101:1011_1110:0101_1001:0101_1010:11",  16, 17, "0101_1001:0101_1010:1"),  // 29
    make_tuple("1011_1101:1011_1110:0101_1001:0101_1010:110", 17, 18, "1011_0010:1011_0101:10"), // 30
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//! Checks BinaryVector::Slice() when required slice exceed BinaryVector capacity
//!
void UT_BinaryVector::test_Slice_When_Exceeding_Capacity ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    string_view sutBits   = std::get<0>(data);
    uint32_t    firstBit  = std::get<1>(data);
    uint32_t    bitsCount = std::get<2>(data);

    auto sut   = BinaryVector::CreateFromBinaryString(sutBits);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut.Slice(firstBit, bitsCount), std::invalid_argument);
  };

  auto data =
  {
    //   Bits: sut, firstBit, bitsCount
    make_tuple("",       0, 1), // 00
    make_tuple("1",      0, 2), // 01
    make_tuple("0",      0, 2), // 02
    make_tuple("11",     1, 2), // 03
    make_tuple("01",     2, 1), // 04
    make_tuple("1001",   4, 1), // 05
    make_tuple("1011_1", 0, 6), // 06
    make_tuple("1011",   1, 5), // 07
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, data);
}


//===========================================================================
// End of UT_BinaryVector.cpp
//===========================================================================
