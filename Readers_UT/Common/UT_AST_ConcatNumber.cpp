//===========================================================================
//                           UT_AST_ConcatNumber.cpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_AST_ConcatNumber.cpp
//!
//! Implements test fixture for testing
//!
//===========================================================================

#include "UT_AST_ConcatNumber.hpp"
#include "AST_ConcatNumber.hpp"
#include "AST_BasedNumber.hpp"
#include "AST_IntegerLiteral.hpp"
#include "AST.hpp"

#include "Mast_Core_Traits.hpp"
#include "Readers_Common_Enums_Traits.hpp"

#include <tuple>
#include <experimental/string_view>
#include <initializer_list>

using std::vector;
using std::tuple;
using std::string;
using std::experimental::string_view;
using std::initializer_list;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;

using namespace Parsers;
using namespace mast;

namespace
{
using based_number_data_t = tuple<Kind, string, string>;
vector<AST_Number*> BuildBasedNumbers(AST_Builder& builder, vector<based_number_data_t>& items)
{
  vector<AST_Number*> numbers;

  for (const auto& data: items)
  {
    auto kind      = std::get<0>(data);
    auto bitsCount = std::get<1>(data);
    auto bits      = std::get<2>(data);

    auto basedNumber = builder.Create_BasedNumber(kind, std::move(bits));
    if (!bitsCount.empty())
    {
      auto sizeExpr = builder.Create_IntegerLiteral(std::move(bitsCount));
      basedNumber->SizeExpr(sizeExpr);
    }

    numbers.push_back(basedNumber);
  }
  return numbers;
}
} // End of unnamed namespace



//! Checks AST_ConcatNumber constructor
//!
void UT_AST_ConcatNumber::test_Constructor ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto items        = std::get<0>(data);
    auto isFullySized = std::get<1>(data);
    AST builder;

    auto numbers = BuildBasedNumbers(builder, items);

    CxxTest::setAbortTestOnFail(true);
    AST_ConcatNumber* concatNumbers = nullptr;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (concatNumbers = builder.Create_ConcatNumber(std::move(numbers)));

    // ---------------- Verify
    //
    TS_ASSERT_NOT_NULLPTR (concatNumbers);
    TS_ASSERT_EQUALS      (concatNumbers->IsFullySized(), isFullySized);

  };

  using data_t = tuple<vector<based_number_data_t>, bool>;  // Numbers data and whether concat number if fully sized or not
  auto data =
  {
    // 0
   /* data_t
    {
      {
      },
      false
    },*/

    // 1
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
      },
      true
    },

    // 2
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
        based_number_data_t{ Kind::Number_Hexa,    "20", "'hFade" },
        based_number_data_t{ Kind::Number_Decimal, "7",  "'d123"  },
      },
      true
    },

    // 3
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
        based_number_data_t{ Kind::Number_Hexa,    "24", "'hCafe" },
        based_number_data_t{ Kind::Number_Hexa,    "",   "'hBad" },
        based_number_data_t{ Kind::Number_Decimal, "7",  "'d123"  },
      },
      false
    },
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AST_ConcatNumber::AsText()
//!
void UT_AST_ConcatNumber::test_AsText ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto items          = std::get<0>(data);
    auto expectedAsText = std::get<1>(data);

    AST builder;

    auto numbers = BuildBasedNumbers(builder, items);
    AST_ConcatNumber* concatNumbers = builder.Create_ConcatNumber(std::move(numbers));

    CxxTest::setAbortTestOnFail(true);

    string asText;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (asText = concatNumbers->AsText());

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (asText, expectedAsText);
  };

  using data_t = tuple<vector<based_number_data_t>, string_view>;
  auto data =
  {
    // 0
/*    data_t
    {
      {
      },
      ""
    },
*/
    // 1
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
      },
      "6'b0110"
    },

    // 2
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
        based_number_data_t{ Kind::Number_Hexa,    "24", "'hCafe" },
        based_number_data_t{ Kind::Number_Hexa,    "20", "'hFade" },
        based_number_data_t{ Kind::Number_Decimal, "7",  "'d123"  },
      },
      "6'b0110, 24'hCafe, 20'hFade, 7'd123"
    },

    // 3
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
        based_number_data_t{ Kind::Number_Hexa,    "24", "'hCafe" },
        based_number_data_t{ Kind::Number_Hexa,    "",   "'hBad" },
        based_number_data_t{ Kind::Number_Decimal, "7",  "'d123"  },
      },
      "6'b0110, 24'hCafe, 'hBad, 7'd123"
    },
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AST_ConcatNumber::AsBinaryVector()
//!
void UT_AST_ConcatNumber::test_AsBinaryVector ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto items                  = std::get<0>(data);
    auto targetSize             = std::get<1>(data);
    auto expectedAsBinaryVector = BinaryVector::CreateFromString(std::get<2>(data));

    AST builder;

    auto numbers = BuildBasedNumbers(builder, items);
    AST_ConcatNumber* concatNumbers = builder.Create_ConcatNumber(std::move(numbers));

    CxxTest::setAbortTestOnFail(true);

    BinaryVector asBinaryVector;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (asBinaryVector = concatNumbers->AsBinaryVector(targetSize));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (asBinaryVector, expectedAsBinaryVector);
  };

  using data_t = tuple<vector<based_number_data_t>, uint32_t, string_view>;  // Numbers data, target size and expected BinaryVector
  auto data =
  {
    // 0 ==> No target size, single number
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
      },
      0u,
      "6'b0110"
    },

    // 1 ==> No target size, several numbers
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
        based_number_data_t{ Kind::Number_Hexa,    "24", "'hCafe" },
        based_number_data_t{ Kind::Number_Hexa,    "20", "'hFade" },
        based_number_data_t{ Kind::Number_Decimal, "7",  "'d123"  },
      },
      0u,
      "6'b0110, 'h00Cafe, 20'h0Fade, 7'd123"
    },

    // 2 ==> Target size == total numbers sizes
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
        based_number_data_t{ Kind::Number_Hexa,    "24", "'hCafe" },
        based_number_data_t{ Kind::Number_Hexa,    "20", "'hFade" },
        based_number_data_t{ Kind::Number_Decimal, "7",  "'d123"  },
      },
      57u,
      "'b00:0110, 'h00:Cafe, 'h0:Fade, 7'd123"
    },

    // 3 ==> Target size > total numbers sizes
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
        based_number_data_t{ Kind::Number_Hexa,    "24", "'hCafe" },
        based_number_data_t{ Kind::Number_Hexa,    "20", "'hFade" },
        based_number_data_t{ Kind::Number_Decimal, "7",  "'d123"  },
      },
      60u,
      "'b00_0000:110, 'h00Cafe, 'h0:Fade, 7'd123"
    },

    // 4 ==> Target size < total numbers sizes
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
        based_number_data_t{ Kind::Number_Hexa,    "24", "'hCafe" },
        based_number_data_t{ Kind::Number_Hexa,    "20", "'hFade" },
        based_number_data_t{ Kind::Number_Decimal, "7",  "'d123"  },
      },
      56u,
      "'b0:0110, 'h00Cafe, 'h0:Fade, 7'd123"
    },

    // 5 ==> Single unsized number (at start)
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Hexa,    "",   "'hBad" },
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
        based_number_data_t{ Kind::Number_Hexa,    "24", "'hCafe" },
        based_number_data_t{ Kind::Number_Decimal, "7",  "'d123"  },
      },
      50u,
      "'b0, 'hBad, 'b00_0110, 'h00Cafe, 7'd123"
    },

    // 6 ==> Single unsized number (in middle)
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
        based_number_data_t{ Kind::Number_Hexa,    "24", "'hCafe" },
        based_number_data_t{ Kind::Number_Hexa,    "",   "'hBad" },
        based_number_data_t{ Kind::Number_Decimal, "7",  "'d123"  },
      },
      50u,
      "6'b0110, 'h00Cafe, 'b0, 'hBad, 7'd123"
    },

    // 7 ==> Single unsized number (at the end)
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
        based_number_data_t{ Kind::Number_Hexa,    "24", "'hCafe" },
        based_number_data_t{ Kind::Number_Decimal, "7",  "'d123"  },
        based_number_data_t{ Kind::Number_Hexa,    "",   "'hBad" },
      },
      50u,
      "6'b0110, 'h00Cafe, 7'd123, 'b0, 'hBad, "
    },
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AST_ConcatNumber::AsBinaryVector() when it is not a valid operation
//!
void UT_AST_ConcatNumber::test_AsBinaryVector_not_valid ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto items      = std::get<0>(data);
    auto targetSize = std::get<1>(data);

    AST builder;

    auto numbers = BuildBasedNumbers(builder, items);
    AST_ConcatNumber* concatNumbers = builder.Create_ConcatNumber(std::move(numbers));

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (concatNumbers->AsBinaryVector(targetSize), std::exception);
  };

  using data_t = tuple<vector<based_number_data_t>, uint32_t>;  // Numbers data and target size
  auto data =
  {
    // 0 ==> No numbers at all
 /*   data_t
    {
      {
      },
      0u,
    },
*/
    // 1 ==> Two unsized
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
        based_number_data_t{ Kind::Number_Hexa,    "", "'hCafe" },
        based_number_data_t{ Kind::Number_Hexa,    "", "'hFade" },
        based_number_data_t{ Kind::Number_Decimal, "7",  "'d123"  },
      },
      64u,
    },

    // 2 ==> One unsized but no target size
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
        based_number_data_t{ Kind::Number_Hexa,    "24", "'hCafe" },
        based_number_data_t{ Kind::Number_Hexa,    "20", "'hFade" },
        based_number_data_t{ Kind::Number_Decimal, "",   "'d123"  },
      },
      0u,
    },

    // 3 ==> Truncation of non zero bits
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
        based_number_data_t{ Kind::Number_Hexa,    "24", "'hCafe" },
        based_number_data_t{ Kind::Number_Hexa,    "20", "'hFade" },
        based_number_data_t{ Kind::Number_Decimal, "7",  "'d123"  },
      },
      53u,
    },

    // 4 ==> One unsized but target bits count too small
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
        based_number_data_t{ Kind::Number_Hexa,    "24", "'hCafe" },
        based_number_data_t{ Kind::Number_Hexa,    "",   "'hBad" },
        based_number_data_t{ Kind::Number_Decimal, "7",  "'d123"  },
      },
      37u,
    },

    // 5 ==> One unsized but target bits count too small
    data_t
    {
      {
        based_number_data_t{ Kind::Number_Binary,  "6",  "'b0110" },
        based_number_data_t{ Kind::Number_Hexa,    "24", "'hCafe" },
        based_number_data_t{ Kind::Number_Hexa,    "",   "'hBad" },
        based_number_data_t{ Kind::Number_Decimal, "7",  "'d123"  },
      },
      36u,
    },

  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}




//===========================================================================
// End of UT_AST_ConcatNumber.cpp
//===========================================================================
