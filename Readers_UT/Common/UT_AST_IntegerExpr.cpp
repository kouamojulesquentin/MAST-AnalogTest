//===========================================================================
//                           UT_AST_IntegerExpr.cpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_AST_IntegerExpr.cpp
//!
//! Implements test fixture for testing AST_IntegerExpr
//!
//===========================================================================

#include "UT_AST_IntegerExpr.hpp"
#include "AST_IntegerLiteral.hpp"
#include "AST_IntegerUnaryExpr.hpp"
#include "AST_IntegerBinaryExpr.hpp"
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



//! Checks AST_IntegerLiteral constructor with valid values
//!
void UT_AST_IntegerExpr::test_IntegerLiteral_Constructor_valid ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto integerText   = std::get<0>(data);
    auto expectedValue = std::get<1>(data);

    AST builder;
    AST_IntegerLiteral* node = nullptr;

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (node = builder.Create_IntegerLiteral(std::move(integerText)));

    // ---------------- Verify
    //
    TS_ASSERT_NOT_NULLPTR (node);
    TS_ASSERT_EQUALS      (node->Evaluate(), expectedValue);
  };

  using data_t = tuple<string, uint32_t>;
  auto data =
  {
    data_t{"0",            0u},          // 0
    data_t{"1",            1u},          // 1
    data_t{"1234567890",   1234567890u}, // 3
    data_t{"1234_567890",  1234567890u}, // 4
    data_t{"1234_5678_90", 1234567890u}, // 5
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AST_IntegerLiteral constructor with not valid values
//!
void UT_AST_IntegerExpr::test_IntegerLiteral_Constructor_not_valid ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto integerText)
  {
    // ---------------- Setup
    //
    AST builder;

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (builder.Create_IntegerLiteral(std::move(integerText)), std::invalid_argument);
  };

  auto data =
  {
    "_"s,           // 0 - Not valid first character
    "a"s,           // 1 - Not valid first character
    "One"s,         // 2 - Not a number
    "91234567890"s, // 3 - Too large value
    "-1234"s,       // 4 - Negative value are not allowed
    "12-34"s,       // 5 - Embedded minus sign
    " 1234"s,       // 6 - Embedded space(s)
    " 1234 "s,      // 7 - Embedded space(s)
    "12 34"s,       // 8 - Embedded space(s)
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}




//! Checks AST_IntegerBinaryExpr constructor with valid (not nullptr) sub-expressions
//!
void UT_AST_IntegerExpr::test_IntegerBinaryExpr_Constructor_valid ()
{
  // ---------------- Setup
  //
  AST  builder;
  auto leftExpr  = builder.Create_IntegerLiteral("12"s);
  auto rightExpr = builder.Create_IntegerLiteral("23"s);

  AST_IntegerBinaryExpr* node = nullptr;

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (node = builder.Create_IntegerBinaryExpr(Kind::Operator_Add, leftExpr, rightExpr));

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (node);
}


//! Checks AST_IntegerBinaryExpr constructor with not valid sub-expressions or operator kind
//!
void UT_AST_IntegerExpr::test_IntegerBinaryExpr_Constructor_not_valid ()
{
  // ---------------- DDT Setup
  //
  AST  builder;

  auto checker = [&builder](const auto& data)
  {
    // ---------------- Setup
    //
    auto operatorKind = std::get<0>(data);
    auto leftExpr     = std::get<1>(data);
    auto rightExpr    = std::get<2>(data);

    AST_IntegerBinaryExpr* node = nullptr;

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (node = builder.Create_IntegerBinaryExpr(operatorKind, leftExpr, rightExpr), std::invalid_argument);
  };

  using data_t = tuple<Kind, AST_IntegerExpr*, AST_IntegerExpr*>;
  auto data =
  {
    data_t{Kind::Operator_Add,   builder.Create_IntegerLiteral("12"s), nullptr},                              // 0 - Bad right expression
    data_t{Kind::Operator_Add,   nullptr,                              builder.Create_IntegerLiteral("12"s)}, // 1 - Bad left expression
    data_t{Kind::IntegerLiteral, builder.Create_IntegerLiteral("36"),  builder.Create_IntegerLiteral("12"s)}, // 2 - Bad Kind (not an operator)
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AST_IntegerUnaryExpr constructor with a valid (not nullptr) sub-expression
//!
void UT_AST_IntegerExpr::test_IntegerUnaryExpr_Constructor_valid ()
{
  // ---------------- Setup
  //
  AST  builder;
  auto expression = builder.Create_IntegerLiteral("12"s);

  AST_IntegerUnaryExpr* node = nullptr;

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (node = builder.Create_IntegerUnaryExpr(Kind::ParenthesizedExpr, expression));

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (node);
}


//! Checks AST_IntegerUnaryExpr constructor with a not valid (nullptr) sub-expression
//!
void UT_AST_IntegerExpr::test_IntegerUnaryExpr_Constructor_not_valid ()
{
  // ---------------- DDT Setup
  //
  AST  builder;

  auto checker = [&builder](const auto& data)
  {
    // ---------------- Setup
    //
    auto operatorKind = std::get<0>(data);
    auto expression   = std::get<1>(data);

    AST_IntegerUnaryExpr* node = nullptr;

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (node = builder.Create_IntegerUnaryExpr(operatorKind, expression), std::invalid_argument);
  };

  using data_t = tuple<Kind, AST_IntegerExpr*>;
  auto data =
  {
    data_t{Kind::ParenthesizedExpr, nullptr},                             // 0 - Bad  expression
    data_t{Kind::Operator_Add,      builder.Create_IntegerLiteral("36")}, // 1 - Bad Kind (not an unary expression)
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AST_IntegerBinaryExpr::Evaluate() when result is valid
//!
void UT_AST_IntegerExpr::test_IntegerBinaryExpr_Evaluate_valid ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto operatorKind   = std::get<0>(data);
    auto leftExprText   = std::get<1>(data);
    auto rightExprText  = std::get<2>(data);
    auto expectedResult = std::get<3>(data);

    AST  builder;
    auto leftExpr   = builder.Create_IntegerLiteral(std::move(leftExprText));
    auto rightExpr  = builder.Create_IntegerLiteral(std::move(rightExprText));
    auto binaryExpr = builder.Create_IntegerBinaryExpr(operatorKind, leftExpr, rightExpr);

    CxxTest::setAbortTestOnFail(true);

    uint32_t result = 0;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (result = binaryExpr->Evaluate());

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (result, expectedResult);
  };

  using data_t = tuple<Kind, string, string, uint32_t>;
  auto data =
  {
    data_t{Kind::Operator_Add,       "0",          "0",    0u},          // 00
    data_t{Kind::Operator_Add,       "0",          "1",    1u},          // 01
    data_t{Kind::Operator_Add,       "1",          "0",    1u},          // 02
    data_t{Kind::Operator_Add,       "1",          "1",    2u},          // 03
    data_t{Kind::Operator_Add,       "1234_0000",  "5678", 12345678u},   // 04
    data_t{Kind::Operator_Add,       "4294967294", "1",    4294967295u}, // 05
    data_t{Kind::Operator_Add,       "4294967295", "0",    4294967295u}, // 06
    data_t{Kind::Operator_Substract, "1",          "0",    1u},          // 07
    data_t{Kind::Operator_Substract, "1",          "1",    0u},          // 08
    data_t{Kind::Operator_Substract, "2",          "1",    1u},          // 09
    data_t{Kind::Operator_Substract, "23",         "10",   13u},         // 10
    data_t{Kind::Operator_Substract, "1234_5678",  "5678", 12340000},    // 11
    data_t{Kind::Operator_Multiply,  "1",          "0",    0u},          // 12
    data_t{Kind::Operator_Multiply,  "0",          "1",    0u},          // 13
    data_t{Kind::Operator_Multiply,  "1",          "1",    1u},          // 14
    data_t{Kind::Operator_Multiply,  "2",          "1",    2u},          // 15
    data_t{Kind::Operator_Multiply,  "23",         "10",   230u},        // 16
    data_t{Kind::Operator_Multiply,  "1234",       "5678", 7006652},     // 17
    data_t{Kind::Operator_Divide,    "0",          "1",    0u},          // 18
    data_t{Kind::Operator_Divide,    "1",          "1",    1u},          // 19
    data_t{Kind::Operator_Divide,    "2",          "1",    2u},          // 20
    data_t{Kind::Operator_Divide,    "1",          "2",    0u},          // 21
    data_t{Kind::Operator_Divide,    "143",        "13",   11u},         // 22
    data_t{Kind::Operator_Divide,    "230",        "10",   23u},         // 23
    data_t{Kind::Operator_Divide,    "7_006_652",  "5678", 1234u},       // 24
    data_t{Kind::Operator_Modulo,    "0",          "1",    0u},          // 25
    data_t{Kind::Operator_Modulo,    "1",          "1",    0u},          // 26
    data_t{Kind::Operator_Modulo,    "2",          "1",    0u},          // 27
    data_t{Kind::Operator_Modulo,    "1",          "2",    1u},          // 28
    data_t{Kind::Operator_Modulo,    "143",        "13",   0u},          // 29
    data_t{Kind::Operator_Modulo,    "144",        "13",   1u},          // 30
    data_t{Kind::Operator_Modulo,    "142",        "13",   12u},         // 31
    data_t{Kind::Operator_Modulo,    "237",        "10",   7u},          // 32
    data_t{Kind::Operator_Modulo,    "7_006_752",  "5678", 100u},        // 33
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AST_IntegerBinaryExpr::Evaluate() when operation or result is not valid
//!
void UT_AST_IntegerExpr::test_IntegerBinaryExpr_Evaluate_not_valid ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto operatorKind  = std::get<0>(data);
    auto leftExprText  = std::get<1>(data);
    auto rightExprText = std::get<2>(data);

    AST  builder;
    auto leftExpr   = builder.Create_IntegerLiteral(std::move(leftExprText));
    auto rightExpr  = builder.Create_IntegerLiteral(std::move(rightExprText));
    auto binaryExpr = builder.Create_IntegerBinaryExpr(operatorKind, leftExpr, rightExpr);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (binaryExpr->Evaluate(), std::exception);
  };

  using data_t = tuple<Kind, string, string>;
  auto data =
  {
    data_t{Kind::Operator_Add,       "1",          "4294967295"}, // 00 - Overflow
    data_t{Kind::Operator_Add,       "4294967295", "1"},          // 01 - Overflow
    data_t{Kind::Operator_Add,       "4294967295", "4294967295"}, // 02 - Overflow
    data_t{Kind::Operator_Substract, "0",          "1"},          // 03 - Underflow
    data_t{Kind::Operator_Substract, "1",          "2"},          // 04 - Underflow
    data_t{Kind::Operator_Multiply,  "98765",      "543210"},     // 05 - Overflow
    data_t{Kind::Operator_Multiply,  "543210",     "98765"},      // 06 - Overflow
    data_t{Kind::Operator_Divide,    "0",          "0"},          // 07 - Division by 0
    data_t{Kind::Operator_Divide,    "1",          "0"},          // 08 - Division by 0
    data_t{Kind::Operator_Divide,    "143",        "0"},          // 09 - Division by 0
    data_t{Kind::Operator_Modulo,    "0",          "0"},          // 10 - Division by 0
    data_t{Kind::Operator_Modulo,    "1",          "0"},          // 11 - Division by 0
    data_t{Kind::Operator_Modulo,    "143",        "0"},          // 12 - Division by 0
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AST_IntegerLiteral::AsText()
//!
void UT_AST_IntegerExpr::test_IntegerLiteral_AsText ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto integerText    = std::get<0>(data);
    auto expectedResult = std::get<1>(data);

    AST builder;
    auto integerLiteral = builder.Create_IntegerLiteral(std::move(integerText));

    CxxTest::setAbortTestOnFail(true);
    string result;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (result = integerLiteral->AsText());

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (result, expectedResult);
  };

  using data_t = tuple<string, string_view>;
  auto data =
  {
    data_t{"0",            "0"},          // 0
    data_t{"9",            "9"},          // 1
    data_t{"1234567890",   "1234567890"}, // 2
    data_t{"1234_567890",  "1234567890"}, // 3
    data_t{"1234_5678_90", "1234567890"}, // 4
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AST_IntegerBinaryExpr::AsText()
//!
void UT_AST_IntegerExpr::test_IntegerBinaryExpr_AsText ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto operatorKind   = std::get<0>(data);
    auto leftExprText   = std::get<1>(data);
    auto rightExprText  = std::get<2>(data);
    auto expectedResult = std::get<3>(data);

    AST  builder;
    auto leftExpr   = builder.Create_IntegerLiteral(std::move(leftExprText));
    auto rightExpr  = builder.Create_IntegerLiteral(std::move(rightExprText));
    auto binaryExpr = builder.Create_IntegerBinaryExpr(operatorKind, leftExpr, rightExpr);

    CxxTest::setAbortTestOnFail(true);

    string result;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (result = binaryExpr->AsText());

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (result, expectedResult);
  };

  using data_t = tuple<Kind, string, string, string_view>;
  auto data =
  {
    data_t{Kind::Operator_Add,       "0",          "0",    "0 + 0"},           // 00
    data_t{Kind::Operator_Add,       "1234_0000",  "5678", "12340000 + 5678"}, // 01
    data_t{Kind::Operator_Add,       "4294967295", "0",    "4294967295 + 0"},  // 02
    data_t{Kind::Operator_Substract, "1",          "0",    "1 - 0"},           // 03
    data_t{Kind::Operator_Substract, "23",         "10",   "23 - 10"},         // 04
    data_t{Kind::Operator_Substract, "1234_5678",  "5678", "12345678 - 5678"}, // 05
    data_t{Kind::Operator_Multiply,  "1234",       "5678", "1234 * 5678"},     // 06
    data_t{Kind::Operator_Divide,    "143",        "13",   "143 / 13"},        // 07
    data_t{Kind::Operator_Divide,    "7_006_652",  "5678", "7006652 / 5678"},  // 08
    data_t{Kind::Operator_Modulo,    "144",        "13",   "144 % 13"},        // 09
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks AST_IntegerUnaryExpr::AsText()
//!
void UT_AST_IntegerExpr::test_IntegerUnaryExpr_AsText ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto operatorKind   = std::get<0>(data);
    auto leftExprText   = std::get<1>(data);
    auto rightExprText  = std::get<2>(data);
    auto expectedResult = std::get<3>(data);

    AST  builder;
    auto leftExpr   = builder.Create_IntegerLiteral(std::move(leftExprText));
    auto rightExpr  = builder.Create_IntegerLiteral(std::move(rightExprText));
    auto binaryExpr = builder.Create_IntegerBinaryExpr(operatorKind, leftExpr, rightExpr);
    auto unaryExpr  = builder.Create_IntegerUnaryExpr(Kind::ParenthesizedExpr, binaryExpr);

    CxxTest::setAbortTestOnFail(true);

    string result;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (result = unaryExpr->AsText());

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (result, expectedResult);
  };

  using data_t = tuple<Kind, string, string, string_view>;
  auto data =
  {
    data_t{Kind::Operator_Add,       "0",          "0",    "(0 + 0)"},           // 00
    data_t{Kind::Operator_Add,       "1234_0000",  "5678", "(12340000 + 5678)"}, // 01
    data_t{Kind::Operator_Add,       "4294967295", "0",    "(4294967295 + 0)"},  // 02
    data_t{Kind::Operator_Substract, "1",          "0",    "(1 - 0)"},           // 03
    data_t{Kind::Operator_Substract, "23",         "10",   "(23 - 10)"},         // 04
    data_t{Kind::Operator_Substract, "1234_5678",  "5678", "(12345678 - 5678)"}, // 05
    data_t{Kind::Operator_Multiply,  "1234",       "5678", "(1234 * 5678)"},     // 06
    data_t{Kind::Operator_Divide,    "143",        "13",   "(143 / 13)"},        // 07
    data_t{Kind::Operator_Divide,    "7_006_652",  "5678", "(7006652 / 5678)"},  // 08
    data_t{Kind::Operator_Modulo,    "144",        "13",   "(144 % 13)"},        // 09
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}





//===========================================================================
// End of UT_AST_IntegerExpr.cpp
//===========================================================================
