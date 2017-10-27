//===========================================================================
//                           UT_EnumsUtility.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_EnumsUtility.cpp
//!
//! Implements test fixture for testing EnumsUtility helpers
//!
//===========================================================================

#include "EnumsUtility.hpp"
#include "UT_EnumsUtility.hpp"

#include <cxxtest/ValueTraits.h>

#include <tuple>
#include <experimental/string_view>

using std::experimental::string_view;
using std::tuple;
using std::make_tuple;

using namespace mast;
namespace NS_EnumsUtility
{
  namespace Inner
  {
    enum class Foo
    {
      First,
      Bar,
      Bier,
      Last,
    };
  }
}

ENUM_TRAITS(NS_EnumsUtility::Inner::Foo,
            ENUM_MEMBER(NS_EnumsUtility::Inner::Foo::First)
            ENUM_MEMBER(NS_EnumsUtility::Inner::Foo::Bar)
            ENUM_MEMBER(NS_EnumsUtility::Inner::Foo::Bier)
            ENUM_MEMBER(NS_EnumsUtility::Inner::Foo::Last)
           );

//! Checks EnumsUtility::ExtractedScope()
//!
void UT_EnumsUtility::test_ExtractedScope ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto scopedName    = std::get<0>(data);
    auto expectedScope = std::get<1>(data);

    // ---------------- Exercise
    //
    auto scope = EnumsUtility::ExtractedScope(scopedName);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (scope, expectedScope);
  };

  auto data =
  {
    make_tuple("",                 ""),             // 0
    make_tuple("No_Scope",         ""),             // 1
    make_tuple("::",               "::"),           // 2
    make_tuple("::Name",           "::"),           // 3
    make_tuple("s::n",             "s::"),          // 4
    make_tuple("Scope::",          "Scope::"),      // 5
    make_tuple("Scope::Name",      "Scope::"),      // 6
    make_tuple("Scope::Sub::Name", "Scope::Sub::"), // 7
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks EnumsUtility::RemovedScope()
//!
void UT_EnumsUtility::test_RemovedScope ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto scopedName    = std::get<0>(data);
    auto expectedScope = std::get<1>(data);

    // ---------------- Exercise
    //
    auto scope = EnumsUtility::RemovedScope(scopedName);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (scope, expectedScope);
  };

  auto data =
  {
    make_tuple("",                 ""),     // 0
    make_tuple("Name",             "Name"), // 1
    make_tuple("::",               ""),     // 2
    make_tuple("::Name",           "Name"), // 3
    make_tuple("Scope::",          ""),     // 4
    make_tuple("Scope::Name",      "Name"), // 5
    make_tuple("Scope::Sub::Name", "Name"), // 6
    make_tuple("Scope::Sub::n",    "n"),    // 7
    make_tuple("s::n",             "n"),    // 8
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks NameString() with different value of valid and not valid enum members
//!
void UT_EnumsUtility::test_Enum_NameString ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto enumValue    = std::get<0>(data);
    auto expectedName = std::get<1>(data);

    // ---------------- Exercise
    //
    auto name = NameString(enumValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (name, expectedName);
  };

  auto data =
  {
    make_tuple(NS_EnumsUtility::Inner::Foo::First,           "First"),              // 0
    make_tuple(NS_EnumsUtility::Inner::Foo::Bar,             "Bar"),                // 1
    make_tuple(NS_EnumsUtility::Inner::Foo::Bier,            "Bier"),               // 2
    make_tuple(NS_EnumsUtility::Inner::Foo::Last,            "Last"),               // 3
    make_tuple(static_cast<NS_EnumsUtility::Inner::Foo>(12), "Unknown_enum_value"), // 4
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks ScopeString() with different value of valid and not valid enum members
//!
void UT_EnumsUtility::test_Enum_ScopeString ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto enumValue    = std::get<0>(data);
    auto expectedName = std::get<1>(data);

    // ---------------- Exercise
    //
    auto name = ScopeString(enumValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (name, expectedName);
  };

  auto data =
  {
    make_tuple(NS_EnumsUtility::Inner::Foo::First,           "NS_EnumsUtility::Inner::Foo::"), // 0
    make_tuple(NS_EnumsUtility::Inner::Foo::Bar,             "NS_EnumsUtility::Inner::Foo::"), // 1
    make_tuple(NS_EnumsUtility::Inner::Foo::Bier,            "NS_EnumsUtility::Inner::Foo::"), // 2
    make_tuple(NS_EnumsUtility::Inner::Foo::Last,            "NS_EnumsUtility::Inner::Foo::"), // 3
    make_tuple(static_cast<NS_EnumsUtility::Inner::Foo>(12), "NS_EnumsUtility::Inner::Foo::"), // 4
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks ScopedNameString() with different value of valid and not valid enum members
//!
void UT_EnumsUtility::test_Enum_ScopedNameString ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto enumValue    = std::get<0>(data);
    auto expectedName = std::get<1>(data);

    // ---------------- Exercise
    //
    auto name = ScopedNameString(enumValue);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (name, expectedName);
  };

  auto data =
  {
    make_tuple(NS_EnumsUtility::Inner::Foo::First,           "NS_EnumsUtility::Inner::Foo::First"),              // 0
    make_tuple(NS_EnumsUtility::Inner::Foo::Bar,             "NS_EnumsUtility::Inner::Foo::Bar"),                // 1
    make_tuple(NS_EnumsUtility::Inner::Foo::Bier,            "NS_EnumsUtility::Inner::Foo::Bier"),               // 2
    make_tuple(NS_EnumsUtility::Inner::Foo::Last,            "NS_EnumsUtility::Inner::Foo::Last"),               // 3
    make_tuple(static_cast<NS_EnumsUtility::Inner::Foo>(12), "NS_EnumsUtility::Inner::Foo::Unknown_enum_value"), // 4
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//===========================================================================
// End of UT_EnumsUtility.cpp
//===========================================================================
