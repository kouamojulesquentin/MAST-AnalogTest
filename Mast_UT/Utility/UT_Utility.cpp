//===========================================================================
//                           UT_Utility.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_Utility.cpp
//!
//! Implements test fixture for testing Utility
//!
//===========================================================================

#include "UT_Utility.hpp"
#include "Utility.hpp"
#include <initializer_list>

#include <vector>
#include <tuple>
#include <experimental/string_view>
#include <cxxtest/ValueTraits.h>
#include <cxxtest/traits/STL11_Traits.h>

using std::tuple;
using std::make_tuple;
using std::experimental::string_view;
using std::initializer_list;
using std::vector;
using mast::Utility;


using namespace std::chrono_literals;


//! Initializes test (called for each test)
void UT_Utility::setUp ()
{
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped
}

//! Checks MinimalBitsForValue::MinimalBitsForValue()
//!
void UT_Utility::test_MinimalBitsForValue ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto input          = std::get<0>(data);
    auto expectedResult = std::get<1>(data);

    // ---------------- Exercise
    //
    auto bitsCount = Utility::MinimalBitsForValue(input);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (bitsCount, expectedResult);
  };

  auto data =
  {
    make_tuple(0U,          1U),  // 00
    make_tuple(1U,          1U),  // 01
    make_tuple(2U,          2U),  // 02
    make_tuple(4U,          3U),  // 03
    make_tuple(5U,          3U),  // 04
    make_tuple(7U,          3U),  // 05
    make_tuple(8U,          4U),  // 06
    make_tuple(9U,          4U),  // 07
    make_tuple(9U,          4U),  // 08
    make_tuple(16U,         5U),  // 09
    make_tuple(17U,         5U),  // 10
    make_tuple(31U,         5U),  // 11
    make_tuple(32U,         6U),  // 12
    make_tuple(63U,         6U),  // 13
    make_tuple(64U,         7U),  // 14
    make_tuple(127U,        7U),  // 15
    make_tuple(128U,        8U),  // 16
    make_tuple(255U,        8U),  // 17
    make_tuple(256U,        9U),  // 18
    make_tuple(1023U,       10U), // 19
    make_tuple(1024U,       11U), // 20
    make_tuple(65535U,      16U), // 21
    make_tuple(65536U,      17U), // 22
    make_tuple(16777215U,   24U), // 23
    make_tuple(16777216U,   25U), // 24
    make_tuple(416777216U,  29U), // 25
    make_tuple(2147483647U, 31U), // 26
    make_tuple(2147483648U, 32U), // 27
    make_tuple(2147483649U, 32U), // 28
    make_tuple(4294967295U, 32U), // 29
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Utility::StartsWith(StringView, StringView)
//!
void UT_Utility::test_StringView_Utility_StartsWith ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto sut       = std::get<0>(data);
    auto subString = std::get<1>(data);
    auto expected  = std::get<2>(data);

    // ---------------- Exercise
    //
    auto result = Utility::StartsWith(sut, subString);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (result, expected);
  };

  using data_t = tuple<string_view, string_view, bool>;
  auto  data =
  {
    data_t("",                         "",             false), // 00
    data_t("Hello::world",             "",             false), // 01
    data_t("Hello::world",             " ",            false), // 02
    data_t("Hello::world",             "h",            false), // 03
    data_t("Hello::world",             "d",            false), // 04
    data_t("Hello::world",             "w",            false), // 05
    data_t("nice and strange usage ",  "and",          false), // 06
    data_t("nice and strange usage ",  " nice",        false), // 07
    data_t(" nice and strange usage ", "nice",         false), // 08
    data_t("nice and strange usage",   "Nice",         false), // 09
    data_t("Fake_Plugin_1",            "lib",          false), // 10
    data_t("libFake_Plugin_1",         "lib",          true),  // 11
    data_t("Hello::world",             "H",            true),  // 12
    data_t("Hello::world",             "Hello::world", true),  // 13
    data_t("Hello::world",             "Hello",        true),  // 14
    data_t("nice and strange usage ",  "nice ",        true),  // 15
    data_t(" nice and strange usage",  " nice",        true),  // 16
    data_t("nice and strange usage",   "nice and",     true),  // 17

    data_t(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr), false), // 15
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks Utility::EndsWith(StringView, StringView)
//!
void UT_Utility::test_StringView_Utility_EndsWith ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto sut       = std::get<0>(data);
    auto subString = std::get<1>(data);
    auto expected  = std::get<2>(data);

    // ---------------- Exercise
    //
    auto result = Utility::EndsWith(sut, subString);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (result, expected);
  };

  using data_t = tuple<string_view, string_view, bool>;
  auto  data =
  {
    data_t("",                          "",              false), // 00
    data_t("Hello::world",              "",              false), // 01
    data_t("Hello::world",              " ",             false), // 02
    data_t("Hello::world",              "D",             false), // 03
    data_t("Hello::world",              "H",             false), // 04
    data_t("Hello::world",              "w",             false), // 05
    data_t("a nice and strange usage ", "and",           false), // 06
    data_t("a nice and strange usage ", "usage",         false), // 07
    data_t("a nice and strange usage",  "Usage",         false), // 08
    data_t("Hello::world",              "d",             true),  // 09
    data_t("Hello::world",              "Hello::world",  true),  // 10
    data_t("Hello::world",              "world",         true),  // 11
    data_t("a nice and strange usage ", "usage ",        true),  // 12
    data_t("a nice and strange usage",  " usage",        true),  // 13
    data_t("a nice and strange usage",  "strange usage", true),  // 14

    data_t(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr), false), // 15
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Utility::Split(StringView, StringView)
//!
void UT_Utility::test_StringView_Utility_Split ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto sut       = std::get<0>(data);
    auto separator = std::get<1>(data);
    auto expected  = std::get<2>(data);

    // ---------------- Exercise
    //
    auto result = Utility::Split(sut, separator);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (result, expected);
  };

  using data_t = tuple<string_view, string_view, vector<string_view>>;
  auto  data =
  {
    data_t("",                          "",   {}),                                                // 00
    data_t(" ",                         "",   {" "}),                                             // 01
    data_t("  ",                        " ",  {"",      ""}),                                     // 02
    data_t("\t",                        ",",  {"\t"}),                                            // 03
    data_t("\t\t",                      "\t", {"",      ""}),                                     // 04
    data_t("\t \t",                     "\t", {"",      " "}),                                    // 05
    data_t("a  ",                       ",",  {"a  "}),                                           // 06
    data_t(" a ",                       ",",  {" a "}),                                           // 07
    data_t("a,b",                       ",",  {"a",     "b"}),                                    // 08
    data_t("a,b, ",                     ",",  {"a",     "b",         " "}),                       // 09
    data_t("Hello::world",              "::", {"Hello", "world"}),                                // 10
    data_t("a nice and strange usage ", "a",  {"",      " nice ",    "nd str", "nge us", "ge "}), // 11
    data_t(" foo / bar / bore ",        "/",  {" foo ", " bar ",     " bore "}),                  // 12
    data_t("foo/bar/bore",              "/",  {"foo",   "bar",       "bore"}),                    // 13
    data_t("?  Hello  ?",               "?",  {"",      "  Hello  "}),                            // 14
    data_t(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr), {}),             // 15
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Utility::TrimLeft(StringView)
//!
void UT_Utility::test_StringView_Utility_TrimLeft ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto sut      = string_view(std::get<0>(data));
    auto expected = string_view(std::get<1>(data));

    // ---------------- Exercise
    //
    Utility::TrimLeft(sut);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, expected);
  };

  auto data =
  {
    make_tuple("",          ""),          // 00
    make_tuple(" ",         ""),          // 01
    make_tuple("  ",        ""),          // 02
    make_tuple("\t",        ""),          // 03
    make_tuple("\t\t",      ""),          // 04
    make_tuple("\t \t",     ""),          // 05
    make_tuple("a  ",       "a  "),       // 06
    make_tuple(" a ",       "a "),        // 07
    make_tuple("  a ",      "a "),        // 08
    make_tuple("\tab ",     "ab "),       // 09
    make_tuple("\t ab ",    "ab "),       // 10
    make_tuple(" \tab ",    "ab "),       // 11
    make_tuple(" \t ab ",   "ab "),       // 12
    make_tuple("?  Hello ", "?  Hello "), // 13

    make_tuple(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr)), // 14
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Utility::TrimRight(StringView)
//!
void UT_Utility::test_StringView_Utility_TrimRight ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto sut      = string_view(std::get<0>(data));
    auto expected = string_view(std::get<1>(data));

    // ---------------- Exercise
    //
    Utility::TrimRight(sut);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, expected);
  };

  auto data =
  {
    make_tuple("",          ""),         // 00
    make_tuple(" ",         ""),         // 01
    make_tuple("  ",        ""),         // 02
    make_tuple("\t",        ""),         // 03
    make_tuple("\t\t",      ""),         // 04
    make_tuple("\t \t",     ""),         // 05
    make_tuple("a  ",       "a"),        // 06
    make_tuple(" a ",       " a"),       // 07
    make_tuple("  a ",      "  a"),      // 08
    make_tuple("\tab ",     "\tab"),     // 09
    make_tuple("\t ab ",    "\t ab"),    // 10
    make_tuple(" \tab ",    " \tab"),    // 11
    make_tuple(" \t ab ",   " \t ab"),   // 12
    make_tuple("?  Hello ", "?  Hello"), // 13

    make_tuple(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr)), // 14
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Utility::TrimRight(StringView)
//!
void UT_Utility::test_StringView_Utility_TrimBoth ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto sut      = string_view(std::get<0>(data));
    auto expected = string_view(std::get<1>(data));

    // ---------------- Exercise
    //
    Utility::TrimBoth(sut);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut, expected);
  };

  auto data =
  {
    make_tuple("",         ""),      // 00
    make_tuple(" ",        ""),      // 01
    make_tuple("  ",       ""),      // 02
    make_tuple("\t",       ""),      // 03
    make_tuple("\t\t",     ""),      // 04
    make_tuple("\t \t",    ""),      // 05
    make_tuple("a  ",      "a"),     // 06
    make_tuple(" a ",      "a"),     // 07
    make_tuple("  a ",     "a"),     // 08
    make_tuple("\tab ",    "ab"),    // 09
    make_tuple("\t ab ",   "ab"),    // 10
    make_tuple(" \tab ",   "ab"),    // 11
    make_tuple(" \t ab ",  "ab"),    // 12
    make_tuple("  Hello ", "Hello"), // 13

    make_tuple(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr)), // 14
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks Utility::ExtractDirectoryPath()
//!
void UT_Utility::test_ExtractDirectoryPath ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto inputPath    = std::get<0>(data);
    auto expectedPath = std::get<1>(data);

    // ---------------- Exercise
    //
    auto extractedPath = Utility::ExtractDirectoryPath(inputPath);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (extractedPath, expectedPath);
  };

  auto data =
  {
    make_tuple("",                          "."),              // 00
    make_tuple(".",                         "."),              // 01
    make_tuple("Foo",                       "."),              // 02
    make_tuple("Foo.bar",                   "."),              // 03
    make_tuple("/Foo.bar",                  "/"),              // 04
    make_tuple("\\Foo.bar",                 "\\"),             // 05
    make_tuple("C:\\Foo.bar",               "C:\\"),           // 06
    make_tuple("C:\\Foo\\Bar\\Read me.txt", "C:\\Foo\\Bar\\"), // 07
    make_tuple("/usr/bin/Foo.bar",          "/usr/bin/"),      // 08
    make_tuple("C:\\Foo/Bar\\Read me.txt",  "C:\\Foo/Bar\\"),  // 09
    make_tuple("/Foo\\Bar/Read me.txt",     "/Foo\\Bar/"),     // 10
  };


  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Utility::ToMilliseconds() with bad strings
//!
void UT_Utility::test_ToMilliseconds_BadStr ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& strDuration)
  {
    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (Utility::ToMilliseconds(strDuration), std::invalid_argument);
  };

  auto data =
  {
    "",            // 01
    "2",           // 02
    "ms",          // 03
    "s",           // 04
    "5  m",        // 05
    "6  sm",       // 06
    "7  mm",       // 07
    "8 ss",        // 08
    "9 us",        // 09
    "10 mn",       // 10
    "11 Stuff ms", // 11
    "12 Stuffms",  // 12
    "Foo 12 ms",   // 13
    "-10 ms",      // 14
    "Bar ms",      // 15
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Utility::ToMilliseconds() with supported strings
//!
void UT_Utility::test_ToMilliseconds_SupportedStr ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto strDuration      = std::get<0>(data);
    auto expectedDuration = std::get<1>(data);

    // ---------------- Exercise
    //
    auto gotDuration = Utility::ToMilliseconds(strDuration);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (gotDuration, expectedDuration);
  };

  auto data =
  {
    make_tuple("0 ms",    0ms),      // 00
    make_tuple("1 ms",    1ms),      // 01
    make_tuple("+2 ms",   2ms),      // 02
    make_tuple("  3ms",   3ms),      // 03
    make_tuple("4 ms ",   4ms),      // 04
    make_tuple(" 5 ms ",  5ms),      // 05
    make_tuple(" 66 ms ", 66ms),     // 06
    make_tuple(" 777 s ", 777000ms), // 07
    make_tuple(" 0 s ",   0ms),      // 08
    make_tuple("9s",      9000ms),   // 09
    make_tuple("10ms",    10ms),     // 10
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//===========================================================================
// End of UT_Utility.cpp
//===========================================================================
