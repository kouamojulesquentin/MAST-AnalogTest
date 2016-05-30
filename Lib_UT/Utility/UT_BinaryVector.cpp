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
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs, expected);
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
