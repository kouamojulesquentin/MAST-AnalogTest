//===========================================================================
//                           UT_SVFVector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SVFVector.cpp
//!
//! Implements test fixture for testing SVFVector
//!
//===========================================================================

#include "UT_SVFVector.hpp"
#include "SVFVector.hpp"
#include "BinaryVector.hpp"
#include "BinaryVector_Traits.hpp"

#include <cxxtest/ValueTraits.h>
#include <string>
#include <tuple>

using std::string;
using std::make_tuple;
using mast::SVFVector;
using mast::BinaryVector;


//! Checks SVFVector default constructor
//!
void UT_SVFVector::test_Constructor_Default ()
{
  // ---------------- Exercise
  //
  SVFVector sut;

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.BitsCount(), 0);
  TS_ASSERT_TRUE   (sut.IsEmpty());
  TS_ASSERT_EQUALS (sut.Data(), string());
}


//! Checks SVFVector constructor from SVF string (hexadecimal)
//!
void UT_SVFVector::test_Constructor_FromString ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto   bitsCount         = std::get<0>(data);
    auto   input             = std::get<1>(data);
    auto   expectedBitsCount = std::get<2>(data);
    string expectedData      = std::get<3>(data);

    // ---------------- Exercise
    //
    SVFVector sut(input, bitsCount);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.BitsCount(), expectedBitsCount);
    TS_ASSERT_EQUALS (sut.Data(),      expectedData);
  };


  auto data =
  {           // input, bitsCount, expected bitsCount, expected data
    make_tuple(0,  "",        0,  ""),       // 00
    make_tuple(1,  "0",       1,  "00"),     // 01
    make_tuple(1,  "01",      1,  "01"),     // 02
    make_tuple(2,  "2",       2,  "02"),     // 03
    make_tuple(3,  "05",      3,  "05"),     // 04
    make_tuple(4,  "0",       4,  "00"),     // 05
    make_tuple(4,  "1",       4,  "01"),     // 06
    make_tuple(4,  "002",     4,  "02"),     // 07
    make_tuple(4,  "3",       4,  "03"),     // 08
    make_tuple(4,  "4",       4,  "04"),     // 09
    make_tuple(4,  "5",       4,  "05"),     // 10
    make_tuple(4,  "6",       4,  "06"),     // 11
    make_tuple(4,  "7",       4,  "07"),     // 12
    make_tuple(4,  "8",       4,  "08"),     // 13
    make_tuple(4,  "9",       4,  "09"),     // 14
    make_tuple(4,  "a",       4,  "0A"),     // 15
    make_tuple(4,  "b",       4,  "0B"),     // 16
    make_tuple(4,  "c",       4,  "0C"),     // 17
    make_tuple(4,  "d",       4,  "0D"),     // 18
    make_tuple(4,  "e",       4,  "0E"),     // 19
    make_tuple(4,  "f",       4,  "0F"),     // 20
    make_tuple(5,  "17",      5,  "17"),     // 21
    make_tuple(6,  "02B",     6,  "2B"),     // 22
    make_tuple(7,  "75",      7,  "75"),     // 23
    make_tuple(8,  "00",      8,  "00"),     // 24
    make_tuple(8,  "01",      8,  "01"),     // 25
    make_tuple(8,  "002",     8,  "02"),     // 26
    make_tuple(8,  "04",      8,  "04"),     // 27
    make_tuple(8,  "08",      8,  "08"),     // 28
    make_tuple(8,  "10",      8,  "10"),     // 29
    make_tuple(8,  "20",      8,  "20"),     // 30
    make_tuple(8,  "40",      8,  "40"),     // 31
    make_tuple(8,  "80",      8,  "80"),     // 32
    make_tuple(8,  "A5",      8,  "A5"),     // 33
    make_tuple(8,  "5A",      8,  "5A"),     // 34
    make_tuple(9,  "1BD",     9,  "01BD"),   // 35
    make_tuple(11, "06F5",    11, "06F5"),   // 36
    make_tuple(16, "030E",    16, "030E"),   // 37
    make_tuple(17, "0061CDE", 17, "061CDE"), // 38
    make_tuple(20, "5E123",   20, "05E123"), // 39
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks SVFVector constructor from SVF string (hexadecimal) with embedded separators
//!
void UT_SVFVector::test_Constructor_FromString_With_Separators ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto   bitsCount         = std::get<0>(data);
    auto   input             = std::get<1>(data);
    auto   expectedBitsCount = std::get<2>(data);
    string expectedData      = std::get<3>(data);

    // ---------------- Exercise
    //
    SVFVector sut(input, bitsCount);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.BitsCount(), expectedBitsCount);
    TS_ASSERT_EQUALS (sut.Data(),      expectedData);
  };


  auto data = // input, bitsCount, expected bitsCount, expected data
  {
    make_tuple(0,  " ",         0,  ""),       // 00
    make_tuple(1,  " 0 ",       1,  "00"),     // 01
    make_tuple(1,  "0 1",       1,  "01"),     // 02
    make_tuple(2,  " 2",        2,  "02"),     // 03
    make_tuple(3,  "05 ",       3,  "05"),     // 04
    make_tuple(6,  "02_B",      6,  "2B"),     // 05
    make_tuple(15, "75:98",     15, "7598"),   // 06
    make_tuple(15, "75-98",     15, "7598"),   // 07
    make_tuple(8,  "5,A",       8,  "5A"),     // 08
    make_tuple(9,  "1B'D",      9,  "01BD"),   // 09
    make_tuple(11, "06F5",      11, "06F5"),   // 10
    make_tuple(16, "030E",      16, "030E"),   // 11
    make_tuple(17, "0061\tCDE", 17, "061CDE"), // 12
    make_tuple(19, "071\nCDE",  19, "071CDE"), // 13
    make_tuple(19, "071\rCDE",  19, "071CDE"), // 14
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks SVFVector constructor from SVF string (hexadecimal) with not valid parameters
//!
void UT_SVFVector::test_Constructor_FromString_Not_Valid_Parameters ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto   bitsCount         = std::get<0>(data);
    auto   input             = std::get<1>(data);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (SVFVector sut(input, bitsCount), std::exception);
  };


  auto data = // input, bitsCount, expected bitsCount, expected data
  {
    // ---------------- Not enough charaters
    //
    make_tuple(1,  "  "),        // 00
    make_tuple(5,  "2"),         // 01
    make_tuple(5,  " 2"),        // 02
    make_tuple(15, "_98_"),      // 03
    make_tuple(25, "5-98"),      // 04

    // ---------------- Not valid characters
    //
    make_tuple(8,  "5?A"),        // 05
    make_tuple(9,  "0x1BD"),      // 06
    make_tuple(11, "0g1110111"),  // 07
    make_tuple(16, "03&0E"),      // 08
    make_tuple(17, "006\\CDE"),   // 09
    make_tuple(19, "(071CDE)"),   // 10
    make_tuple(19, "[071\rCDE]"), // 11
    make_tuple(19, "\"07CDE\""),  // 12
    make_tuple(8,  "5OA"),        // 13
    make_tuple(8,  "5IA"),        // 14
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}





//! Checks SVFVector constructor from BinaryVector
//!
void UT_SVFVector::test_Constructor_FromBinary ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto   input            = std::get<0>(data);
    auto   expectedBitCount = std::get<1>(data);
    string expectedData     = std::get<2>(data);
    auto   binaryVector     = BinaryVector::CreateFromBinaryString(input);

    // ---------------- Exercise
    //
    SVFVector sut(binaryVector);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.BitsCount(), expectedBitCount);
    TS_ASSERT_EQUALS (sut.Data(),      expectedData);
  };


  auto data =
  {           // input, expected bitsCount, expected data
    make_tuple("",                         0,  ""),       // 00
    make_tuple("0",                        1,  "00"),     // 01
    make_tuple("1",                        1,  "01"),     // 02
    make_tuple("10",                       2,  "02"),     // 03
    make_tuple("101",                      3,  "05"),     // 04
    make_tuple("0000",                     4,  "00"),     // 05
    make_tuple("0001",                     4,  "01"),     // 06
    make_tuple("0010",                     4,  "02"),     // 07
    make_tuple("0011",                     4,  "03"),     // 08
    make_tuple("0100",                     4,  "04"),     // 09
    make_tuple("0101",                     4,  "05"),     // 10
    make_tuple("0110",                     4,  "06"),     // 11
    make_tuple("0111",                     4,  "07"),     // 12
    make_tuple("1000",                     4,  "08"),     // 13
    make_tuple("1001",                     4,  "09"),     // 14
    make_tuple("1010",                     4,  "0A"),     // 15
    make_tuple("1011",                     4,  "0B"),     // 16
    make_tuple("1100",                     4,  "0C"),     // 17
    make_tuple("1101",                     4,  "0D"),     // 18
    make_tuple("1110",                     4,  "0E"),     // 19
    make_tuple("1111",                     4,  "0F"),     // 20
    make_tuple("1011_1",                   5,  "17"),     // 21
    make_tuple("1010_11",                  6,  "2B"),     // 22
    make_tuple("1110_101",                 7,  "75"),     // 23
    make_tuple("0000_0000",                8,  "00"),     // 24
    make_tuple("0000_0001",                8,  "01"),     // 25
    make_tuple("0000_0010",                8,  "02"),     // 26
    make_tuple("0000_0100",                8,  "04"),     // 27
    make_tuple("0000_1000",                8,  "08"),     // 28
    make_tuple("0001_0000",                8,  "10"),     // 29
    make_tuple("0010_0000",                8,  "20"),     // 30
    make_tuple("0100_0000",                8,  "40"),     // 31
    make_tuple("1000_0000",                8,  "80"),     // 32
    make_tuple("1010_0101",                8,  "A5"),     // 33
    make_tuple("0101_1010",                8,  "5A"),     // 34
    make_tuple("1101_1110:1",              9,  "01BD"),   // 35
    make_tuple("1101_1110:101",            11, "06F5"),   // 36
    make_tuple("0000_1110:0000_0011",      16, "0E03"),   // 37
    make_tuple("0000_1110:0000_0011:0",    17, "001C06"), // 38
    make_tuple("0000_1110:0000_0011:0101", 20, "00E035"), // 39

    make_tuple("1010_1000:1001_0110:0111_0100:0101_0010:0011_0000:0001", 44, "0A8967452301"), // 40
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks SVFVector::operator== when both are equal
//!
void UT_SVFVector::test_operator_eq_With_Self ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto svfString = std::get<0>(data);
    auto bitsCount = std::get<1>(data);
    auto sut = SVFVector(svfString, bitsCount);

    // ---------------- Exercise
    //
    bool areEqual = sut == sut;

    // ---------------- Verify
    //
    TS_ASSERT_TRUE (areEqual);
  };

  auto inputs =
  {
    make_tuple("",        0),  // 00
    make_tuple("0",       1),  // 01
    make_tuple("1",       1),  // 02
    make_tuple("1",       3),  // 03
    make_tuple("51",      7),  // 04
    make_tuple("1ABCDEF", 25), // 05
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs);
}



//! Checks SVFVector::operator== when both are equal
//!
void UT_SVFVector::test_operator_eq_When_Equal ()
{
// ---------------- DDT Setup
//
auto checker = [](auto data)
{
  // ---------------- Setup
  //
  auto svfString = std::get<0>(data);
  auto bitsCount = std::get<1>(data);
  auto sut = SVFVector(svfString, bitsCount);
  auto rhs = SVFVector(svfString, bitsCount);

  // ---------------- Exercise
  //
  bool areEqual = sut == rhs;

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (areEqual);
};

auto inputs =
{
  make_tuple("",        0),  // 00
  make_tuple("0",       1),  // 01
  make_tuple("1",       1),  // 02
  make_tuple("1",       3),  // 03
  make_tuple("51",      7),  // 04
  make_tuple("1ABCDEF", 25), // 05
};

// ---------------- DDT Exercise
//
TS_DATA_DRIVEN_TEST (checker, inputs);
}


//! Checks SVFVector::operator== when both are not equal
//!
void UT_SVFVector::test_operator_eq_When_NotEqual ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto sutSvfString = std::get<0>(data);
    auto sutBitsCount = std::get<1>(data);
    auto rhsSvfString = std::get<2>(data);
    auto rhsBitsCount = std::get<3>(data);

    auto sut = SVFVector(sutSvfString, sutBitsCount);
    auto rhs = SVFVector(rhsSvfString, rhsBitsCount);

    // ---------------- Exercise
    //
    bool areEqual = sut == rhs;

    // ---------------- Verify
    //
    TS_ASSERT_FALSE (areEqual);
  };

  auto inputs =
  {
    make_tuple("",        0,  "0",       1),  // 00
    make_tuple("0",       1,  "0",       2),  // 01
    make_tuple("0",       2,  "0",       1),  // 02
    make_tuple("0",       1,  "1",       1),  // 03
    make_tuple("1",       1,  "0",       1),  // 04
    make_tuple("1",       3,  "2",       3),  // 05
    make_tuple("51",      7,  "71",      7),  // 06
    make_tuple("1ABCDEF", 25, "1ABDDEF", 25), // 07
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs);
}

//! Checks SVFVector::operator!= when comparing with same instance
//!
void UT_SVFVector::test_operator_neq_With_Self ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto svfString = std::get<0>(data);
    auto bitsCount = std::get<1>(data);
    auto sut = SVFVector(svfString, bitsCount);

    // ---------------- Exercise
    //
    bool areNotEqual = sut != sut;

    // ---------------- Verify
    //
    TS_ASSERT_FALSE (areNotEqual);
  };

  auto inputs =
  {
    make_tuple("",        0),  // 00
    make_tuple("0",       1),  // 01
    make_tuple("1",       1),  // 02
    make_tuple("1",       3),  // 03
    make_tuple("51",      7),  // 04
    make_tuple("1ABCDEF", 25), // 05
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs);
}


//! Checks SVFVector::operator!= when both are equal
//!
void UT_SVFVector::test_operator_neq_When_Equal ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto svfString = std::get<0>(data);
    auto bitsCount = std::get<1>(data);
    auto sut = SVFVector(svfString, bitsCount);
    auto rhs = SVFVector(svfString, bitsCount);

    // ---------------- Exercise
    //
    bool areNotEqual = sut != rhs;

    // ---------------- Verify
    //
    TS_ASSERT_FALSE (areNotEqual);
  };

  auto inputs =
  {
    make_tuple("",        0),  // 00
    make_tuple("0",       1),  // 01
    make_tuple("1",       1),  // 02
    make_tuple("1",       3),  // 03
    make_tuple("51",      7),  // 04
    make_tuple("1ABCDEF", 25), // 05
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs);
}


//! Checks SVFVector::operator!= when both are not equal
//!
void UT_SVFVector::test_operator_neq_When_NotEqual ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto sutSvfString = std::get<0>(data);
    auto sutBitsCount = std::get<1>(data);
    auto rhsSvfString = std::get<2>(data);
    auto rhsBitsCount = std::get<3>(data);

    auto sut = SVFVector(sutSvfString, sutBitsCount);
    auto rhs = SVFVector(rhsSvfString, rhsBitsCount);

    // ---------------- Exercise
    //
    bool areNotEqual = sut != rhs;

    // ---------------- Verify
    //
    TS_ASSERT_TRUE (areNotEqual);
  };

  auto inputs =
  {
    make_tuple("",        0,  "0",       1),  // 00
    make_tuple("0",       1,  "0",       2),  // 01
    make_tuple("0",       2,  "0",       1),  // 02
    make_tuple("0",       1,  "1",       1),  // 03
    make_tuple("1",       1,  "0",       1),  // 04
    make_tuple("1",       3,  "2",       3),  // 05
    make_tuple("51",      7,  "71",      7),  // 06
    make_tuple("1ABCDEF", 25, "1ABDDEF", 25), // 07
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, inputs);
}


//! Checks SVFVector::ToBinaryVector()
//!
//! @note This is a kind of roundtrip test that checks that both construction from BinaryVector and export to binary vector are compatible
//!       BinaryVector ==> SVFVector ==> BinaryVector
//!
void UT_SVFVector::test_ToBinaryVector ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto init                 = std::get<0>(data);
    auto expected             = std::get<1>(data);
    auto initBinaryVector     = BinaryVector::CreateFromBinaryString(init);
    auto expectedBinaryVector = BinaryVector::CreateFromBinaryString(expected);

    SVFVector sut(initBinaryVector);

    // ---------------- Exercise
    //
    auto result = sut.ToBinaryVector();

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (result, expectedBinaryVector);
  };


  auto data =
  {           // input,                    expected data
    make_tuple("1101_1110:1",              "1101_1110:1"),              // 35
    make_tuple("",                         ""),                         // 00
    make_tuple("0",                        "0"),                        // 01
    make_tuple("1",                        "1"),                        // 02
    make_tuple("10",                       "10"),                       // 03
    make_tuple("101",                      "101"),                      // 04
    make_tuple("0000",                     "0000"),                     // 05
    make_tuple("0001",                     "0001"),                     // 06
    make_tuple("0010",                     "0010"),                     // 07
    make_tuple("0011",                     "0011"),                     // 08
    make_tuple("0100",                     "0100"),                     // 09
    make_tuple("0101",                     "0101"),                     // 10
    make_tuple("0110",                     "0110"),                     // 11
    make_tuple("0111",                     "0111"),                     // 12
    make_tuple("1000",                     "1000"),                     // 13
    make_tuple("1001",                     "1001"),                     // 14
    make_tuple("1010",                     "1010"),                     // 15
    make_tuple("1011",                     "1011"),                     // 16
    make_tuple("1100",                     "1100"),                     // 17
    make_tuple("1101",                     "1101"),                     // 18
    make_tuple("1110",                     "1110"),                     // 19
    make_tuple("1111",                     "1111"),                     // 20
    make_tuple("1011_1",                   "1011_1"),                   // 21
    make_tuple("1010_11",                  "1010_11"),                  // 22
    make_tuple("1110_101",                 "1110_101"),                 // 23
    make_tuple("0000_0000",                "0000_0000"),                // 24
    make_tuple("0000_0001",                "0000_0001"),                // 25
    make_tuple("0000_0010",                "0000_0010"),                // 26
    make_tuple("0000_0100",                "0000_0100"),                // 27
    make_tuple("0000_1000",                "0000_1000"),                // 28
    make_tuple("0001_0000",                "0001_0000"),                // 29
    make_tuple("0010_0000",                "0010_0000"),                // 30
    make_tuple("0100_0000",                "0100_0000"),                // 31
    make_tuple("1000_0000",                "1000_0000"),                // 32
    make_tuple("1010_0101",                "1010_0101"),                // 33
    make_tuple("0101_1010",                "0101_1010"),                // 34
    make_tuple("1101_1110:101",            "1101_1110:101"),            // 36
    make_tuple("0000_1110:0000_0011",      "0000_1110:0000_0011"),      // 37
    make_tuple("0000_1110:0000_0011:0",    "0000_1110:0000_0011:0"),    // 38
    make_tuple("0000_1110:0000_0011:0101", "0000_1110:0000_0011:0101"), // 39

    make_tuple("1010_1000:1001_0110:0111_0100:0101_0010:0011_0000:0001", "1010_1000:1001_0110:0111_0100:0101_0010:0011_0000:0001"), // 40
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//===========================================================================
// End of UT_SVFVector.cpp
//===========================================================================
