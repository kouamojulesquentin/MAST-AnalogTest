//===========================================================================
//                           UT_AST_BasedNumber.cpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_AST_BasedNumber.cpp
//!
//! Implements test fixture for testing
//!
//===========================================================================

#include "UT_AST_BasedNumber.hpp"
#include "AST_BasedNumber.hpp"
#include "AST_IntegerLiteral.hpp"
#include "AST.hpp"

#include "Mast_Core_Traits.hpp"
#include "Readers_Common_Enums_Traits.hpp"

#include <tuple>
#include <experimental/string_view>

using std::tuple;
using std::string;
using std::experimental::string_view;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;
using namespace Parsers;
using namespace mast;


//! Checks AST_BasedNumber constructor with valid values
//!
void UT_AST_BasedNumber::test_Constructor_valid ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto kind   = std::get<0>(data);
    auto digits = std::get<1>(data);

    AST builder;
    AST_BasedNumber* basedNumber = nullptr;

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (basedNumber = builder.Create_BasedNumber(kind, std::move(digits)));

    // ---------------- Verify
    //
    TS_ASSERT_NOT_NULLPTR (basedNumber);
    TS_ASSERT_FALSE       (basedNumber->IsSized());
    TS_ASSERT_NULLPTR     (basedNumber->SizeExpr());
    TS_ASSERT_THROWS      (basedNumber->Size(), std::runtime_error);
  };

  using data_t = tuple<Parsers::Kind, string>;
  auto data =
  {
    data_t{Kind::Number_Binary,  "'b0"},                   // 00
    data_t{Kind::Number_Binary,  "'b1"},                   // 01
    data_t{Kind::Number_Binary,  "'b001_1100"},            // 02
    data_t{Kind::Number_Binary,  "'b 001__100"},           // 03
    data_t{Kind::Number_Decimal, "'d123"},                 // 04
    data_t{Kind::Number_Decimal, "'d1234567890123456789"}, // 05
    data_t{Kind::Number_Decimal, "'d 123"},                // 06
    data_t{Kind::Number_Hexa,    "'h1"},                   // 07
    data_t{Kind::Number_Hexa,    "'h9"},                   // 08
    data_t{Kind::Number_Hexa,    "'hf"},                   // 09
    data_t{Kind::Number_Hexa,    "'hF"},                   // 10
    data_t{Kind::Number_Hexa,    "'h1234_567890"},         // 11
    data_t{Kind::Number_Hexa,    "'Habcdef_ABCDEF"},       // 12
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AST_BasedNumber constructor with valid values
//!
void UT_AST_BasedNumber::test_Constructor_not_valid ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto kind   = std::get<0>(data);
    auto digits = std::get<1>(data);

    AST builder;

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (builder.Create_BasedNumber(kind, std::move(digits)), std::invalid_argument);
  };

  using data_t = tuple<Parsers::Kind, string>;
  auto data =
  {
    data_t{Kind::IntegerLiteral, "'b0"},   // 00 - Bad kind
    data_t{Kind::Number_Binary,  ""},      // 01 - Empty base and digits
    data_t{Kind::Number_Binary,  "'b"},    // 02 - No digits
    data_t{Kind::Number_Binary,  "'F00"},  // 03 - Not valid base specification character
    data_t{Kind::Number_Binary,  "hCafe"}, // 04 - Missing quote before base specification character
    data_t{Kind::Number_Binary,  "10"},    // 05 - No base
    data_t{Kind::Number_Binary,  "1234"},  // 06 - No base
    data_t{Kind::Number_Binary,  "Bad"},   // 07 - No base
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks AST_BasedNumber::SizeExpr()
//!
void UT_AST_BasedNumber::test_SizeExpr ()
{
  // ---------------- Setup
  //
  AST builder;
  auto sizeExpr    = builder.Create_IntegerLiteral("24");
  auto basedNumber = builder.Create_BasedNumber(Kind::Number_Hexa, "'hCafe");

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (basedNumber->SizeExpr(sizeExpr));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE        (basedNumber->IsSized());
  TS_ASSERT_NOT_NULLPTR (basedNumber->SizeExpr());
  TS_ASSERT_EQUALS_PTR  (basedNumber->SizeExpr(), sizeExpr);
  TS_ASSERT_EQUALS      (basedNumber->Size(),     24u);
}


//! Checks AST_BasedNumber::AsText() when number is unsized
//!
void UT_AST_BasedNumber::test_AsText_unsized ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto kind           = std::get<0>(data);
    auto numberAsText   = std::get<1>(data);
    auto expectedAsText = numberAsText;

    AST    builder;
    auto   basedNumber = builder.Create_BasedNumber(kind, std::move(numberAsText));
    string asText;

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (asText = basedNumber->AsText());

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (basedNumber->AsText(), expectedAsText);
  };

  using data_t = tuple<Parsers::Kind, string>;
  auto data =
  {
    data_t{Kind::Number_Binary,  "'b001_1100"},            // 00
    data_t{Kind::Number_Binary,  "'b 001__100"},           // 01
    data_t{Kind::Number_Decimal, "'d1234567890123456789"}, // 02
    data_t{Kind::Number_Decimal, "'d 123"},                // 03
    data_t{Kind::Number_Hexa,    "'h1234_567890"},         // 04
    data_t{Kind::Number_Hexa,    "'Habcdef_ABCDEF"},       // 05
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AST_BasedNumber::AsText() when number is sized
//!
void UT_AST_BasedNumber::test_AsText_sized ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto kind           = std::get<0>(data);
    auto size           = std::get<1>(data);
    auto numberAsText   = std::get<2>(data);
    auto expectedAsText = std::get<3>(data);

    AST  builder;
    auto basedNumber = builder.Create_BasedNumber(kind, std::move(numberAsText));
    auto sizeExpr    = builder.Create_IntegerLiteral(std::move(size));

    basedNumber->SizeExpr(sizeExpr);

    string asText;

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (asText = basedNumber->AsText());

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (basedNumber->AsText(), expectedAsText);
  };

  using data_t = tuple<Parsers::Kind, string, string, string_view>;
  auto data =
  {
    data_t{Kind::Number_Binary,  "11u", "'b001_1100",            "11'b001_1100"},            // 00
    data_t{Kind::Number_Binary,  "12u", "'b 001__100",           "12'b 001__100"},           // 01
    data_t{Kind::Number_Decimal, "64u", "'d1234567890123456789", "64'd1234567890123456789"}, // 02
    data_t{Kind::Number_Decimal, "8u",  "'d 123",                "8'd 123"},                 // 03
    data_t{Kind::Number_Hexa,    "32u", "'h1234_567890",         "32'h1234_567890"},         // 04
    data_t{Kind::Number_Hexa,    "50u", "'Habcdef_ABCDEF",       "50'Habcdef_ABCDEF"},       // 05
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AST_BasedNumber::AsText() when number is sized and inverted
//!
void UT_AST_BasedNumber::test_AsText_sized_inverted ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto kind           = std::get<0>(data);
    auto size           = std::get<1>(data);
    auto numberAsText   = std::get<2>(data);
    auto expectedAsText = std::get<3>(data);

    AST  builder;
    auto basedNumber = builder.Create_BasedNumber(kind, std::move(numberAsText));
    auto sizeExpr    = builder.Create_IntegerLiteral(std::move(size));

    basedNumber->SizeExpr(sizeExpr);
    basedNumber->IsInverted(true);

    string asText;

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (asText = basedNumber->AsText());

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (basedNumber->AsText(), expectedAsText);
  };

  using data_t = tuple<Parsers::Kind, string, string, string_view>;
  auto data =
  {
    data_t{Kind::Number_Binary,  "11u", "'b001_1100",            "~11'b001_1100"},            // 00
    data_t{Kind::Number_Binary,  "12u", "'b 001__100",           "~12'b 001__100"},           // 01
    data_t{Kind::Number_Decimal, "64u", "'d1234567890123456789", "~64'd1234567890123456789"}, // 02
    data_t{Kind::Number_Decimal, "8u",  "'d 123",                "~8'd 123"},                 // 03
    data_t{Kind::Number_Hexa,    "32u", "'h1234_567890",         "~32'h1234_567890"},         // 04
    data_t{Kind::Number_Hexa,    "50u", "'Habcdef_ABCDEF",       "~50'Habcdef_ABCDEF"},       // 05
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks AST_BasedNumber::AsBinaryVector() when number is unsized
//!
void UT_AST_BasedNumber::test_AsBinaryVector_unsized ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto kind                 = std::get<0>(data);
    auto numberAsText         = std::get<1>(data);
    auto expectedBinaryVector = BinaryVector::CreateFromString(std::get<2>(data));

    AST    builder;
    auto   basedNumber = builder.Create_BasedNumber(kind, std::move(numberAsText));

    CxxTest::setAbortTestOnFail(true);

    BinaryVector asBinaryVector;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (asBinaryVector = basedNumber->AsBinaryVector());

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (asBinaryVector, expectedBinaryVector);
  };

  using data_t = tuple<Parsers::Kind, string, string_view>;
  auto data =
  {
    data_t{Kind::Number_Binary,  "'b001_1100",            "'b001_1100"},            // 00
    data_t{Kind::Number_Binary,  "'b 001__100",           "'b 001__100"},           // 01
    data_t{Kind::Number_Decimal, "'d1234567890123456789", "'d1234567890123456789"}, // 02
    data_t{Kind::Number_Decimal, "'d 123",                "'d 123"},                // 03
    data_t{Kind::Number_Hexa,    "'h1234_567890",         "'h1234_567890"},         // 04
    data_t{Kind::Number_Hexa,    "'Habcdef_ABCDEF",       "'Habcdef_ABCDEF"},       // 05
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AST_BasedNumber::AsBinaryVector() when number is sized
//!
void UT_AST_BasedNumber::test_AsBinaryVector_sized ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto kind                 = std::get<0>(data);
    auto size                 = std::get<1>(data);
    auto numberAsText         = std::get<2>(data);
    auto expectedBinaryVector = BinaryVector::CreateFromString(std::get<3>(data));

    AST  builder;
    auto basedNumber = builder.Create_BasedNumber(kind, std::move(numberAsText));
    auto sizeExpr    = builder.Create_IntegerLiteral(std::move(size));

    basedNumber->SizeExpr(sizeExpr);

    CxxTest::setAbortTestOnFail(true);

    BinaryVector asBinaryVector;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (asBinaryVector = basedNumber->AsBinaryVector());

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (asBinaryVector, expectedBinaryVector);
  };

  using data_t = tuple<Parsers::Kind, string, string, string_view>;
  auto data =
  {
    data_t{Kind::Number_Binary,  "11u", "'b001_1100",            "'b000_0001_1100"},       // 00
    data_t{Kind::Number_Binary,  "12u", "'b 101__100",           "'b000_000_101_100"},     // 01
    data_t{Kind::Number_Decimal, "32u", "'d1234_567890",         "'h499602D2"},            // 02
    data_t{Kind::Number_Decimal, "64u", "'d1234567890123456789", "'d1234567890123456789"}, // 03
    data_t{Kind::Number_Decimal, "8u",  "'d 234",                "'hEA"},                  // 04
    data_t{Kind::Number_Hexa,    "43u", "'h1234_567890",         "0b000/x1234_567890"},    // 05
    data_t{Kind::Number_Hexa,    "50u", "'Habcdef_ABCDEF",       "'b00:'Habcdef_ABCDEF"},  // 06
    data_t{Kind::Number_Hexa,    "45u", "'H1bcdef_ABCDEF",       "'b1:'Hbcdef_ABCDEF"},    // 07 ==> Truncation of leading zeroes is allowed
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AST_BasedNumber::AsBinaryVector() when giving a target size
//!
void UT_AST_BasedNumber::test_AsBinaryVector_target_size ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto kind                 = std::get<0>(data);
    auto size                 = std::get<1>(data);
    auto digits               = std::get<2>(data);
    auto targetSize           = std::get<3>(data);
    auto expectedBinaryVector = BinaryVector::CreateFromString(std::get<4>(data));

    AST  builder;
    auto basedNumber = builder.Create_BasedNumber(kind, std::move(digits));

    if (!size.empty())
    {
      auto sizeExpr = builder.Create_IntegerLiteral(std::move(size));

      basedNumber->SizeExpr(sizeExpr);
    }

    CxxTest::setAbortTestOnFail(true);

    BinaryVector asBinaryVector;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (asBinaryVector = basedNumber->AsBinaryVector(targetSize));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (asBinaryVector, expectedBinaryVector);
  };

  //                            Kind, size,   digits, target size, expected
  using data_t = tuple<Parsers::Kind, string, string, uint32_t, string_view>;
  auto data =
  {
    data_t{Kind::Number_Binary,  "11u", "'b001_1100",            8u,  "'b0001_1100"},           // 00
    data_t{Kind::Number_Binary,  "12u", "'b 101__100",           8u,  "'b00_101_100"},          // 01
    data_t{Kind::Number_Binary,  "7u",  "'b001_1100",            5u,  "'b1_1100"},              // 02
    data_t{Kind::Number_Decimal, "32u", "'d1234_567890",         31u, "'b100, 'h99602D2"},      // 03
    data_t{Kind::Number_Decimal, "64u", "'d1234567890123456789", 61u, "'b1,'h12210F47DE98115"}, // 04
    data_t{Kind::Number_Decimal, "8u",  "'d 123",                7u,  "'b111_1011"},            // 05
    data_t{Kind::Number_Hexa,    "43u", "'h1234_567890",         42u, "0b00/x1234_567890"},     // 06
    data_t{Kind::Number_Hexa,    "50u", "'Habcdef_ABCDEF",       49u, "'b0:'Habcdef_ABCDEF"},   // 07
    data_t{Kind::Number_Hexa,    "52u", "'H1bcdef_ABCDEF",       45u, "'b1:'Hbcdef_ABCDEF"},    // 08
    data_t{Kind::Number_Hexa,    "",    "'H1bcdef_ABCDEF",       45u, "'b1:'Hbcdef_ABCDEF"},    // 09
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks AST_BasedNumber::AsBinaryVector() when giving a target size and is inverted
//!
void UT_AST_BasedNumber::test_AsBinaryVector_inverted_target_size ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto kind                 = std::get<0>(data);
    auto digits               = std::get<1>(data);
    auto targetSize           = std::get<2>(data);
    auto expectedBinaryVector = BinaryVector::CreateFromString(std::get<3>(data));

    AST  builder;
    auto basedNumber = builder.Create_BasedNumber(kind, std::move(digits));

    basedNumber->IsInverted(true);

    CxxTest::setAbortTestOnFail(true);

    BinaryVector asBinaryVector;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (asBinaryVector = basedNumber->AsBinaryVector(targetSize));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (asBinaryVector, expectedBinaryVector);
  };

  //                            Kind, digits, target size, expected
  using data_t = tuple<Parsers::Kind, string, uint32_t, string_view>;
  auto data =
  {
    data_t{Kind::Number_Binary,  "'b001_1100",            8u,  "~'b0001_1100"},            // 00
    data_t{Kind::Number_Binary,  "'b001_1100",            5u,  "~'b1_1100"},               // 01
    data_t{Kind::Number_Decimal, "'d1234_567890",         31u, "~'b100, ~'h99602D2"},      // 02
    data_t{Kind::Number_Decimal, "'d1234567890123456789", 61u, "~'b1,~'h12210F47DE98115"}, // 03
    data_t{Kind::Number_Decimal, "'d 123",                7u,  "~'b111_1011"},             // 04
    data_t{Kind::Number_Hexa,    "'h1234_567890",         42u, "0b11, ~'h1234_567890"},    // 05
    data_t{Kind::Number_Hexa,    "'Habcdef_ABCDEF",       49u, "~'b0, ~'habcdef_ABCDEF"},  // 06
    data_t{Kind::Number_Hexa,    "'H1bcdef_ABCDEF",       45u, "~'b1, ~'h bcdef_ABCDEF"},  // 07
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AST_BasedNumber::AsBinaryVector() when giving a target size
//!
void UT_AST_BasedNumber::test_AsBinaryVector_target_size_bad_truncation ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto kind       = std::get<0>(data);
    auto size       = std::get<1>(data);
    auto digits     = std::get<2>(data);
    auto targetSize = std::get<3>(data);

    AST  builder;
    auto basedNumber = builder.Create_BasedNumber(kind, std::move(digits));

    if (!size.empty())
    {
      auto sizeExpr = builder.Create_IntegerLiteral(std::move(size));

      basedNumber->SizeExpr(sizeExpr);
    }

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (basedNumber->AsBinaryVector(targetSize), std::exception);
  };

  //                            Kind, size,   digits, target size
  using data_t = tuple<Parsers::Kind, string, string, uint32_t>;
  auto data =
  {
    data_t{Kind::Number_Binary,  "4u",  "'b001_1100",            0u},  // 00
    data_t{Kind::Number_Binary,  "6u",  "'b101_1100",            0u},  // 01
    data_t{Kind::Number_Binary,  "8u",  "'b 101__100",           5u},  // 02
    data_t{Kind::Number_Decimal, "32u", "'d1234_567890",         28u}, // 03
    data_t{Kind::Number_Decimal, "64u", "'d1234567890123456789", 60u}, // 04
    data_t{Kind::Number_Decimal, "8u",  "'d 123",                6u},  // 05
    data_t{Kind::Number_Hexa,    "",    "'H1bcdef_ABCDEF",       44u}, // 06
    data_t{Kind::Number_Hexa,    "",    "'Habcdef_ABCDEF",       40u}, // 07
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//===========================================================================
// End of UT_AST_BasedNumber.cpp
//===========================================================================
