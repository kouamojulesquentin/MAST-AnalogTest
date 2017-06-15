//===========================================================================
//                           UT_YamlFile.cpp
//===========================================================================
//
//! @file UT_YamlFile.cpp
//!
//! Implements test fixture for testing YamlFile
//!
//===========================================================================

#include "UT_YamlFile.hpp"
#include "YamlFile.hpp"
#include "Utility.hpp"

#include <cxxtest/ValueTraits.h>
#include <tuple>
#include <vector>
#include <string>
#include <experimental/string_view>
#include <initializer_list>
#include <fstream>
#include <cstdio> // To delete files

using std::tuple;
using std::tie;
using std::vector;
using std::string;
using std::experimental::string_view;
using std::initializer_list;
using std::ofstream;

using namespace std::string_literals;
using namespace mast;

//! Initializes test (called for each test)
//!
void UT_YamlFile::setUp ()
{
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped
}


//! Checks YamlFile::Check() when given yaml is invalid
//!
void UT_YamlFile::test_Check_Invalid ()
{
  // ---------------- Setup
  //
  auto yaml = "Path1: Dibona\n"
              " - Path_A: Ecrin\n"
              " - Path_B: Meije"; // Path1 cannot be scalar and sequence

  auto ok           = true;
  auto errorMessage = ""s;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tie(ok, errorMessage) = YamlFile::Check(yaml));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (ok);
  auto expectedErrorMessage = "yaml-cpp: error at line 2, column 10: illegal map value";
  TS_ASSERT_EQUALS (errorMessage, expectedErrorMessage);
}


//! Checks YamlFile::Check() when given yaml is valid
//!
void UT_YamlFile::test_Check_Valid ()
{
  // ---------------- Setup
  //
  auto yaml = "Path1: \n"
              " - Path_A: Ecrin\n"
              " - Path_B: Meije";

  auto ok           = false;
  auto errorMessage = "??"s;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tie(ok, errorMessage) = YamlFile::Check(yaml));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (ok);
  TS_ASSERT_EMPTY (errorMessage);
}


//! Checks YamlFile::DefaultConstructor()
//!
void UT_YamlFile::test_DefaultConstructor ()
{
  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (YamlFile());
}


//! Checks YamlFile::Load() when given yaml is invalid
//!
void UT_YamlFile::test_Load_InvalidYaml ()
{
  // ---------------- Setup
  //
  YamlFile sut;
  auto     yaml = "Path1: Dibona\n"
                  " - Path_A: Ecrin\n"
                  " - Path_B: Meije"; // Path1 cannot be scarlar and sequence

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Load(yaml), std::exception);
}


//! Checks YamlFile::Load() when this is the first time (freshly constructed)
//!
void UT_YamlFile::test_Load_FirstTime ()
{
  // ---------------- Setup
  //
  YamlFile sut;
  auto     yaml = "Path1: Dibona\n"
                  "Path_A:\n"
                  "  Path_B: Meije\n"
                  "Path_W:\n"
                  "  Path_X:\n"
                  "    Path_Y: Ecrin\n"
                  "  Path_Z: Pelvoux";

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Load(yaml));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.IsDirty());
  auto asString = sut.AsString();
  TS_ASSERT_EQUALS (asString, yaml);
}


//! Checks YamlFile::Load() when this is the second time (freshly constructed)
//!
void UT_YamlFile::test_Load_SecondTime ()
{
  // ---------------- Setup
  //
  auto yaml_1 = "Key: Value\n";
  auto yaml_2 = "Path1: Dibona\n"
                "Path_A:\n"
                "  Path_B: Meije\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: Ecrin\n"
                "  Path_Z: Pelvoux";

  YamlFile sut;
  sut.Load(yaml_1);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Load(yaml_2));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.IsDirty());
  auto asString = sut.AsString();
  TS_ASSERT_EQUALS (asString, yaml_2);
}


//! Checks YamlFile::RemoveKey()
//!
void UT_YamlFile::test_RemoveKey ()
{

  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto yaml          = std::get<0>(data);
    auto keyPath       = std::get<1>(data);
    auto expectedDirty = std::get<2>(data);
    auto expectedYaml  = std::get<3>(data);

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.RemoveKey(keyPath));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.IsDirty(),  expectedDirty);
    TS_ASSERT_EQUALS (sut.AsString(), expectedYaml);
  };

  using data_t = tuple<string_view, string_view, bool, string_view>;
  auto data =
  {
    data_t{"Path1: Dibona",                         "Path1",             true,  "{}"},                             // 0
    data_t{"Path1: 1\nPath2: 2",                    "Path1",             true,  "Path2: 2"},                       // 1
    data_t{"Path1:\n  Path2: true\n  Path3: false", "Path1|Path3",       true,  "Path1:\n  Path2: true"},          // 2
    data_t{"Path1:\n  Path2:\n    Path3: 3",        "Path1|Path2|Path3", true,  "Path1:\n  Path2:\n    {}"},       // 3
    data_t{"Path1:\n  Path2:\n    Path3: 3",        "Path1|Path2",       true,  "Path1:\n  {}"},                   // 4
    data_t{"Path1:\n  Path2:\n    Path3: 3",        "Path1",             true,  "{}"},                             // 5
    data_t{"Path1:\n  Path2: 2\n  Path3: 3",        "Path1|Path4",       false, "Path1:\n  Path2: 2\n  Path3: 3"}, // 6
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsString() when providing an, invalid, empty path
//!
void UT_YamlFile::test_GetAsString_EmptyPath ()
{
  // ---------------- Setup
  //
  YamlFile sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.GetAsString("", "Foo", true), std::exception);
}


//! Checks YamlFile::GetAsString() when autoCreate is false
//!
void UT_YamlFile::test_GetAsString_AutoCreate_false ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path         = std::get<0>(data);
    auto defaultValue = std::get<1>(data);
    auto autoCreate   = false;

    YamlFile sut;

    // ---------------- Exercise
    //
    auto got = sut.GetAsString(path, defaultValue, autoCreate);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (got, defaultValue);

    // Check that default value has not been stored
    auto anotherDefaultValue = "a completly different default value";
    got = sut.GetAsString(path, anotherDefaultValue, false);
    TS_ASSERT_EQUALS (got, anotherDefaultValue);
  };

  using data_t = tuple<string_view, string>;
  auto data =
  {
    data_t{"Path1",             "Bar"},       // 0
    data_t{"Path1|Path2",       "Hello"},     // 1
    data_t{"Path1|Path2|Path3", "world!"},    // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsString() when default value is provided, path does not exist and autoCreate is false
//!
void UT_YamlFile::test_GetAsString_AutoCreate_false_NotExist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto path)
  {
    // ---------------- Setup
    //
    auto yaml = "Path1: Dibona\n"
                "Path_A:\n"
                "  Path_B: Meije\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: Ecrin\n"
                "  Path_Z: Pelvoux";

    YamlFile sut;
    sut.Load(yaml);

    auto defaultValue = "some default value";

    // ---------------- Exercise
    //
    auto got = sut.GetAsString(path, defaultValue, false);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.IsDirty());
    TS_ASSERT_EQUALS (got, defaultValue);
  };

  auto data =
  {
    "Path2",                  // 0
    "Path_A|Path_C",          // 1
    "Path_W|Path_Y|Path_Y",   // 2
    "Path_W|Path_z",          // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsString() when autoCreate is true
//!
void UT_YamlFile::test_GetAsString_AutoCreate_true ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path         = std::get<0>(data);
    auto defaultValue = std::get<1>(data);
    auto autoCreate   = true;

    YamlFile sut;

    // ---------------- Exercise
    //
    auto got = sut.GetAsString(path, defaultValue, autoCreate);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (got, defaultValue);

    // Check that previous default value has been stored
    auto anotherDefaultValue = "a completly different default value";
    got = sut.GetAsString(path, anotherDefaultValue, false);
    TS_ASSERT_EQUALS (got, defaultValue);

    //+ (begin JFC October/26/2016): for debug purpose
//+    TS_ASSERT_EMPTY (sut.AsString());
    //+ (end   JFC October/26/2016):
  };

  using data_t = tuple<string_view, string>;
  auto data =
  {
    data_t{"Path1",             "Bar"},       // 0
    data_t{"Path1|Path2",       "Hello"},     // 1
    data_t{"Path1|Path2|Path3", "world!"},    // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsString() when no default value is provided
//!
void UT_YamlFile::test_GetAsString_NoDefault ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path     = std::get<0>(data);
    auto expected = std::get<1>(data);

    auto yaml = "Path1: Dibona\n"
                "Path_A:\n"
                "  Path_B: Meije\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: Ecrin\n"
                "  Path_Z: Pelvoux";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise
    //
    auto got = sut.GetAsString(path);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.IsDirty());
    TS_ASSERT_EQUALS (got, expected);
  };

  using data_t = tuple<string_view, string_view>;
  auto data =
  {
    data_t{"Path1",                "Dibona"},   // 0
    data_t{"Path_A|Path_B",        "Meije"},    // 1
    data_t{"Path_W|Path_X|Path_Y", "Ecrin"},    // 2
    data_t{"Path_W|Path_Z",        "Pelvoux"},  // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsString() when no default value is provided and path does not exist
//!
void UT_YamlFile::test_GetAsString_NoDefault_NotExist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto path)
  {
    // ---------------- Setup
    //
    auto yaml = "Path1: Dibona\n"
                "Path_A:\n"
                "  Path_B: Meije\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: Ecrin\n"
                "  Path_Z: Pelvoux";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut.GetAsString(path), std::exception);
  };

  auto data =
  {
    "Path2",                  // 0
    "Path_A|Path_C",          // 1
    "Path_W|Path_Y|Path_Y",   // 2
    "Path_W|Path_z",          // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsInt() when providing an, invalid, empty path
//!
void UT_YamlFile::test_GetAsInt_EmptyPath ()
{
  // ---------------- Setup
  //
  YamlFile sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.GetAsInt("", 2, true), std::exception);
}


//! Checks YamlFile::GetAsInt() when autoCreate is false
//!
void UT_YamlFile::test_GetAsInt_AutoCreate_false ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path         = std::get<0>(data);
    auto defaultValue = std::get<1>(data);
    auto autoCreate   = false;

    YamlFile sut;

    // ---------------- Exercise
    //
    auto got = sut.GetAsInt(path, defaultValue, autoCreate);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (got, defaultValue);

    // Check that default value has not been stored
    auto anotherDefaultValue = 0xBadCafe;
    got = sut.GetAsInt(path, anotherDefaultValue, false);
    TS_ASSERT_EQUALS (got, anotherDefaultValue);
  };

  using data_t = tuple<string_view, int>;
  auto data =
  {
    data_t{"Path1",             -1},   // 0
    data_t{"Path1|Path2",       -22},  // 1
    data_t{"Path1|Path2|Path3", 3333}, // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsInt() when autoCreate is true
//!
void UT_YamlFile::test_GetAsInt_AutoCreate_true ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path         = std::get<0>(data);
    auto defaultValue = std::get<1>(data);
    auto autoCreate   = true;

    YamlFile sut;

    // ---------------- Exercise
    //
    auto got = sut.GetAsInt(path, defaultValue, autoCreate);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (got, defaultValue);

    // Check that previous default value has been stored
    auto anotherDefaultValue = 0xFaceDead;
    got = sut.GetAsInt(path, anotherDefaultValue, false);
    TS_ASSERT_EQUALS (got, defaultValue);
  };

  using data_t = tuple<string_view, int>;
  auto data =
  {
    data_t{"Path1",             -1},   // 0
    data_t{"Path1|Path2",       -22},  // 1
    data_t{"Path1|Path2|Path3", 3333}, // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsInt() when no default value is provided
//!
void UT_YamlFile::test_GetAsInt_NoDefault ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path     = std::get<0>(data);
    auto expected = std::get<1>(data);

    auto yaml = "Path1: 0\n"
                "Path_A:\n"
                "  Path_B: 12\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: -456789\n"
                "  Path_Z: 0xBadCafe";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise
    //
    auto got = sut.GetAsInt(path);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.IsDirty());
    TS_ASSERT_EQUALS (got, expected);
  };

  using data_t = tuple<string_view, int>;
  auto data =
  {
    data_t{"Path1",                0},         // 0
    data_t{"Path_A|Path_B",        12},        // 1
    data_t{"Path_W|Path_X|Path_Y", -456789},   // 2
    data_t{"Path_W|Path_Z",        0xBadCafe}, // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsInt() when no default value is provided and path does not exist
//!
void UT_YamlFile::test_GetAsInt_NoDefault_NotExist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto path)
  {
    // ---------------- Setup
    //
    auto yaml = "Path1: 0\n"
                "Path_A:\n"
                "  Path_B: 12\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: -456789\n"
                "  Path_Z: 0xBadCafe";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut.GetAsInt(path), std::exception);
  };

  auto data =
  {
    "Path2",                // 0
    "Path_A|Path_C",        // 1
    "Path_W|Path_Y|Path_Y", // 2
    "Path_W|Path_z",        // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}




//! Checks YamlFile::GetAsBool() when providing an, invalid, empty path
//!
void UT_YamlFile::test_GetAsBool_EmptyPath ()
{
  // ---------------- Setup
  //
  YamlFile sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.GetAsBool("", true, true), std::exception);
}


//! Checks YamlFile::GetAsBool() when autoCreate is false
//!
void UT_YamlFile::test_GetAsBool_AutoCreate_false ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto path)
  {
    // ---------------- Setup
    //
    YamlFile sut;

    // ---------------- Exercise
    //
    auto got = sut.GetAsBool(path, true, false);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (got, true);

    // Check that default value has not been stored
    got = sut.GetAsBool(path, false, false);
    TS_ASSERT_EQUALS (got, false);
  };

  auto data =
  {
    "Path1",             // 0
    "Path1|Path2",       // 1
    "Path1|Path2|Path3", // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsBool() when autoCreate is true
//!
void UT_YamlFile::test_GetAsBool_AutoCreate_true ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto path)
  {
    // ---------------- Setup
    //
    YamlFile sut;

    // ---------------- Exercise
    //
    auto got = sut.GetAsBool(path, true, true);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (got, true);

    // Check that previous default value has been stored
    got = sut.GetAsBool(path, false, false);
    TS_ASSERT_EQUALS (got, true);
  };

  auto data =
  {
    "Path1",             // 0
    "Path1|Path2",       // 1
    "Path1|Path2|Path3", // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsBool() when no default value is provided
//!
void UT_YamlFile::test_GetAsBool_NoDefault ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path     = std::get<0>(data);
    auto expected = std::get<1>(data);

    auto yaml = "Path1: false\n"
                "Path_A:\n"
                "  Path_B: true\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: true\n"
                "  Path_Z: false";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise
    //
    auto got = sut.GetAsBool(path);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.IsDirty());
    TS_ASSERT_EQUALS (got, expected);
  };

  using data_t = tuple<string_view, int>;
  auto data =
  {
    data_t{"Path1",                false}, // 0
    data_t{"Path_A|Path_B",        true},  // 1
    data_t{"Path_W|Path_X|Path_Y", true},  // 2
    data_t{"Path_W|Path_Z",        false}, // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsBool() when no default value is provided and path does not exist
//!
void UT_YamlFile::test_GetAsBool_NoDefault_NotExist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto path)
  {
    // ---------------- Setup
    //
    auto yaml = "Path1: false\n"
                "Path_A:\n"
                "  Path_B: true\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: true\n"
                "  Path_Z: false";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut.GetAsBool(path), std::exception);
  };

  auto data =
  {
    "Path2",                // 0
    "Path_A|Path_C",        // 1
    "Path_W|Path_Y|Path_Y", // 2
    "Path_W|Path_z",        // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::TryGetAsString()
//!
void UT_YamlFile::test_TryGetAsString_Exist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path     = std::get<0>(data);
    auto expected = std::get<1>(data);

    auto yaml = "Path1: Dibona\n"
                "Path_A:\n"
                "  Path_B: Meije\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: Ecrin\n"
                "  Path_Z: Pelvoux\n"
                "Tag:\n"
                "  TaggingOptions:\n"
                "    IgnoreRegex:\n"
                "      Ext_cpp:\n"
                "        alias: data_t";

    YamlFile sut;
    sut.Load(yaml);

    bool   succeeded = false;
    string got;

    // ---------------- Exercise
    //
    tie(succeeded, got) = sut.TryGetAsString(path);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.IsDirty());
    TS_ASSERT_TRUE   (succeeded);
    TS_ASSERT_EQUALS (got, expected);
  };

  using data_t = tuple<string_view, string_view>;
  auto data =
  {
    data_t{"Path1",                                        "Dibona"},  // 0
    data_t{"Path_A|Path_B",                                "Meije"},   // 1
    data_t{"Path_W|Path_X|Path_Y",                         "Ecrin"},   // 2
    data_t{"Path_W|Path_Z",                                "Pelvoux"}, // 3
    data_t{"Tag|TaggingOptions|IgnoreRegex|Ext_cpp|alias", "data_t"},  // 4
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::TryGetAsString() when path does not exist
//!
void UT_YamlFile::test_TryGetAsString_NotExist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto path)
  {
    // ---------------- Setup
    //
    auto yaml = "Path1: Dibona\n"
                "Path_A:\n"
                "  Path_B: Meije\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: Ecrin\n"
                "  Path_Z: Pelvoux";

    YamlFile sut;
    sut.Load(yaml);

    bool   succeeded = false;
    string got;

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS_NOTHING (tie(succeeded, got) = sut.TryGetAsString(path));

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.IsDirty());
    TS_ASSERT_FALSE  (succeeded);
    TS_ASSERT_EMPTY (got);
  };

  auto data =
  {
    "Path2",                  // 0
    "Path_A|Path_C",          // 1
    "Path_W|Path_Y|Path_Y",   // 2
    "Path_W|Path_z",          // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks YamlFile::TryGetAsInt() when no default value is provided
//!
void UT_YamlFile::test_TryGetAsInt_NoDefault ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path     = std::get<0>(data);
    auto expected = std::get<1>(data);

    auto yaml = "Path1: 0\n"
                "Path_A:\n"
                "  Path_B: 12\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: -456789\n"
                "  Path_Z: 0xBadCafe";

    YamlFile sut;
    sut.Load(yaml);

    bool succeeded = false;
    int  got       = -1;

    // ---------------- Exercise
    //
    tie(succeeded, got) = sut.TryGetAsInt(path);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.IsDirty());
    TS_ASSERT_TRUE   (succeeded);
    TS_ASSERT_EQUALS (got, expected);
  };

  using data_t = tuple<string_view, int>;
  auto data =
  {
    data_t{"Path1",                0},         // 0
    data_t{"Path_A|Path_B",        12},        // 1
    data_t{"Path_W|Path_X|Path_Y", -456789},   // 2
    data_t{"Path_W|Path_Z",        0xBadCafe}, // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::TryGetAsInt() when no default value is provided and path does not exist
//!
void UT_YamlFile::test_TryGetAsInt_NoDefault_NotExist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto path)
  {
    // ---------------- Setup
    //
    auto yaml = "Path1: 0\n"
                "Path_A:\n"
                "  Path_B: 12\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: -456789\n"
                "  Path_Z: 0xBadCafe";

    YamlFile sut;
    sut.Load(yaml);

    bool succeeded = false;
    int  got       = -3;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (tie(succeeded, got) = sut.TryGetAsInt(path));

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.IsDirty());
    TS_ASSERT_FALSE  (succeeded);
    TS_ASSERT_EQUALS (got, 0);
  };

  auto data =
  {
    "Path2",                // 0
    "Path_A|Path_C",        // 1
    "Path_W|Path_Y|Path_Y", // 2
    "Path_W|Path_z",        // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::TryGetAsBool() when no default value is provided
//!
void UT_YamlFile::test_TryGetAsBool_NoDefault ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path     = std::get<0>(data);
    auto expected = std::get<1>(data);

    auto yaml = "Path1: false\n"
                "Path_A:\n"
                "  Path_B: true\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: true\n"
                "  Path_Z: false";

    YamlFile sut;
    sut.Load(yaml);

    bool succeeded = true;
    bool got       = true;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (tie(succeeded, got) = sut.TryGetAsBool(path));

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.IsDirty());
    TS_ASSERT_TRUE   (succeeded);
    TS_ASSERT_EQUALS (got, expected);
  };

  using data_t = tuple<string_view, int>;
  auto data =
  {
    data_t{"Path1",                false}, // 0
    data_t{"Path_A|Path_B",        true},  // 1
    data_t{"Path_W|Path_X|Path_Y", true},  // 2
    data_t{"Path_W|Path_Z",        false}, // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::TryGetAsBool() when no default value is provided and path does not exist
//!
void UT_YamlFile::test_TryGetAsBool_NoDefault_NotExist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto path)
  {
    // ---------------- Setup
    //
    auto yaml = "Path1: false\n"
                "Path_A:\n"
                "  Path_B: true\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: true\n"
                "  Path_Z: false";

    YamlFile sut;
    sut.Load(yaml);

    bool succeeded = true;
    bool got       = true;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (tie(succeeded, got) = sut.TryGetAsBool(path));

    // ---------------- Verify
    //
    TS_ASSERT_FALSE (sut.IsDirty());
    TS_ASSERT_FALSE (succeeded);
    TS_ASSERT_FALSE (got);
  };

  auto data =
  {
    "Path2",                // 0
    "Path_A|Path_C",        // 1
    "Path_W|Path_Y|Path_Y", // 2
    "Path_W|Path_z",        // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}




//! Checks YamlFile::GetAsString() when providing an, invalid, empty path
//!
void UT_YamlFile::test_GetAsString_Sequence_EmptyPath ()
{
  // ---------------- Setup
  //
  YamlFile sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.GetAsString("", 0, "Foo", true), std::exception);
}


//! Checks YamlFile::GetAsString() when autoCreate is false
//!
void UT_YamlFile::test_GetAsString_Sequence_AutoCreate_false ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path         = std::get<0>(data);
    auto defaultValue = std::get<1>(data);
    auto autoCreate   = false;

    YamlFile sut;

    // ---------------- Exercise
    //
    auto got = sut.GetAsString(path, 1, defaultValue, autoCreate);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (got, defaultValue);

    // Check that default value has not been stored
    auto anotherDefaultValue = "a completly different default value";
    got = sut.GetAsString(path, anotherDefaultValue, false);
    TS_ASSERT_EQUALS (got, anotherDefaultValue);
  };

  using data_t = tuple<string_view, string>;
  auto data =
  {
    data_t{"Path1",             "Bar"},       // 0
    data_t{"Path1|Path2",       "Hello"},     // 1
    data_t{"Path1|Path2|Path3", "world!"},    // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsString() when default value is provided, path does not exist and autoCreate is false
//!
void UT_YamlFile::test_GetAsString_Sequence_AutoCreate_false_NotExist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto path)
  {
    // ---------------- Setup
    //
    auto yaml = "Path1: \n"
                " - Dibona\n"
                " - Olan\n"
                "Path_A:\n"
                "   Path_B: Meije\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: Ecrin\n"
                "  Path_Z: Pelvoux";

    YamlFile sut;
    sut.Load(yaml);

    auto defaultValue = "some default value";

    // ---------------- Exercise
    //
    auto got = sut.GetAsString(path, 0, defaultValue, false);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.IsDirty());
    TS_ASSERT_EQUALS (got, defaultValue);
  };

  auto data =
  {
    "Path2",                  // 0
    "Path_A|Path_C",          // 1
    "Path_W|Path_Y|Path_Y",   // 2
    "Path_W|Path_z",          // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsString() when autoCreate is true
//!
void UT_YamlFile::test_GetAsString_Sequence_AutoCreate_true ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path         = std::get<0>(data);
    auto index        = std::get<1>(data);
    auto defaultValue = std::get<2>(data);
    auto autoCreate   = true;

    YamlFile sut;

    // ---------------- Exercise
    //
    auto got = sut.GetAsString(path, index, defaultValue, autoCreate);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (got, defaultValue);

    // Check that previous default value has been stored
    auto anotherDefaultValue = "a completly different default value";
    got = sut.GetAsString(path, index, anotherDefaultValue, false);
    TS_ASSERT_EQUALS (got, defaultValue);
  };

  using data_t = tuple<string_view, int,  string>;
  auto data =
  {
    data_t{"Path1",             0, "Bar"},                                     // 0
    data_t{"Path1|Path2",       0, "Hello"},                                   // 1
    data_t{"Path1|Path3|Path4", 0, "world!"},                                  // 2
    data_t{"Path1|Path3|Path4", 1, "Sequences must be"},                       // 3 ==> Tranformed to map
    data_t{"Path1|Path3|Path4", 2, "accessed in order!"},                      // 4 ==> Tranformed to map
    data_t{"Path1|Path3|Path4", 3, "Otherwise they are transformed to maps!"}, // 5 ==> Tranformed to map
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsString() for sequence when autoCreate is true and requested in index order
//!
void UT_YamlFile::test_GetAsString_Sequence_AutoCreate_true_Ordered ()
{
  // ---------------- Setup
  //
  auto path          = "Path1|Path3|Path4";
  auto index         = 0;
  auto defaultValues = {"In order test", "Sequences must be", "accessed in order!", "Otherwise they would be transformed to maps!" };
  auto autoCreate    = true;
  auto gots          = vector<string>()  ;
  YamlFile sut;

  // ---------------- Exercise
  //
  for (const auto& defaultValue : defaultValues)
  {
    auto got = sut.GetAsString(path, index++, defaultValue, autoCreate);
    gots.emplace_back(std::move(got));
  }

  // ---------------- Verify
  //
  index = 0;
  for (const auto& defaultValue : defaultValues)
  {
    auto& got = gots[index++];
    TS_ASSERT_EQUALS (got, defaultValue);
  }

  // Check that previous default values have been stored
  index = 0;
  for (const auto& defaultValue : defaultValues)
  {
    auto got = sut.GetAsString(path, index++, "A completly different default value", false);
    TS_ASSERT_EQUALS (got, defaultValue);
  }

  auto expectedYaml = "Path1:\n"
                      "  Path3:\n"
                      "    Path4:\n"
                      "      - In order test\n"
                      "      - Sequences must be\n"
                      "      - accessed in order!\n"
                      "      - Otherwise they would be transformed to maps!"s;

  TS_ASSERT_EQUALS (sut.AsString(), expectedYaml);
}


//! Checks YamlFile::GetAsString() when no default value is provided
//!
void UT_YamlFile::test_GetAsString_Sequence_NoDefault ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path     = std::get<0>(data);
    auto index    = std::get<1>(data);
    auto expected = std::get<2>(data);

    auto yaml = "Path1: \n"
                " - Dibona\n"
                " - Meije\n"
                " - Ecrin\n"
                "Path_A:\n"
                "  Path_B: \n"
                "    - Mont Blanc\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: \n"
                "      - Dru\n"
                "      - Droite\n"
                "  Path_Z: \n"
                "    - Pelvoux";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise
    //
    auto got = sut.GetAsString(path, index);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.IsDirty());
    TS_ASSERT_EQUALS (got, expected);
  };

  using data_t = tuple<string_view, int, string_view>;
  auto data =
  {
    data_t{"Path1",                0, "Dibona"},     // 0
    data_t{"Path_A|Path_B",        0, "Mont Blanc"}, // 1
    data_t{"Path_W|Path_X|Path_Y", 1, "Droite"},     // 2
    data_t{"Path_W|Path_Z",        0, "Pelvoux"},    // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsString() when no default value is provided and path does not exist
//!
void UT_YamlFile::test_GetAsString_Sequence_NoDefault_NotExist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto path  = std::get<0>(data);
    auto index = std::get<1>(data);
    auto yaml  = "Path1: \n"
                 "  - Dibona\n"
                 "  - Meije\n"
                 "  - Ecrin\n"
                 "Path_A:\n"
                 "  Path_B: \n"
                 "    - Mont Blanc\n"
                 "Path_W:\n"
                 "  Path_X:\n"
                 "    Path_Y: \n"
                 "      - Dru\n"
                 "      - Droite\n"
                 "  Path_Z: \n"
                 "    - Pelvoux";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut.GetAsString(path, index), std::exception);
  };

  using data_t = tuple<string_view, int>;
  auto data =
  {
    data_t{"Path1",                3}, // 0
    data_t{"Path2",                0}, // 1
    data_t{"Path_A|Path_C",        0}, // 2
    data_t{"Path_A|Path_B",        1}, // 3
    data_t{"Path_W|Path_Y|Path_Y", 0}, // 4
    data_t{"Path_W|Path_z",        0}, // 5
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsInt() when providing an, invalid, empty path
//!
void UT_YamlFile::test_GetAsInt_Sequence_EmptyPath ()
{
  // ---------------- Setup
  //
  YamlFile sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.GetAsInt("", 0, 12, true), std::exception);
}


//! Checks YamlFile::GetAsInt() at index when autoCreate is false
//!
void UT_YamlFile::test_GetAsInt_Sequence_AutoCreate_false ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path         = std::get<0>(data);
    auto defaultValue = std::get<1>(data);
    auto autoCreate   = false;

    YamlFile sut;

    // ---------------- Exercise
    //
    auto got = sut.GetAsInt(path, 0, defaultValue, autoCreate);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (got, defaultValue);

    // Check that default value has not been stored
    auto anotherDefaultValue = 98635454;
    got = sut.GetAsInt(path, anotherDefaultValue, false);
    TS_ASSERT_EQUALS (got, anotherDefaultValue);
  };

  using data_t = tuple<string_view, int>;
  auto data =
  {
    data_t{"Path1",             10},  // 0
    data_t{"Path1|Path2",       111}, // 1
    data_t{"Path1|Path2|Path3", -33}, // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsInt() when default value is provided, path does not exist and autoCreate is false
//!
void UT_YamlFile::test_GetAsInt_Sequence_AutoCreate_false_NotExist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto path)
  {
    // ---------------- Setup
    //
    auto yaml = "Path1: \n"
                " - 12 \n"
                " - 24\n"
                "Path_A:\n"
                "   Path_B: 36\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: -48\n"
                "  Path_Z: 50";

    YamlFile sut;
    sut.Load(yaml);

    auto defaultValue = 0xBad;

    // ---------------- Exercise
    //
    auto got = sut.GetAsInt(path, 0, defaultValue, false);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.IsDirty());
    TS_ASSERT_EQUALS (got, defaultValue);
  };

  auto data =
  {
    "Path2",                  // 0
    "Path_A|Path_C",          // 1
    "Path_W|Path_Y|Path_Y",   // 2
    "Path_W|Path_z",          // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsInt() when autoCreate is true
//!
void UT_YamlFile::test_GetAsInt_Sequence_AutoCreate_true ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path         = std::get<0>(data);
    auto index        = std::get<1>(data);
    auto defaultValue = std::get<2>(data);
    auto autoCreate   = true;

    YamlFile sut;

    // ---------------- Exercise
    //
    auto got = sut.GetAsInt(path, index, defaultValue, autoCreate);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (got, defaultValue);

    // Check that previous default value has been stored
    auto anotherDefaultValue = 0xBaf1e;
    got = sut.GetAsInt(path, index, anotherDefaultValue, false);
    TS_ASSERT_EQUALS (got, defaultValue);
  };

  using data_t = tuple<string_view, int,  int>;
  auto data =
  {
    data_t{"Path1",             0, 12}, // 0
    data_t{"Path1|Path2",       0, 13}, // 1
    data_t{"Path1|Path3|Path4", 0, 14}, // 2
    data_t{"Path1|Path3|Path4", 1, 15}, // 3 ==> Tranformed to map
    data_t{"Path1|Path3|Path4", 2, 16}, // 4 ==> Tranformed to map
    data_t{"Path1|Path3|Path4", 3, 17}, // 5 ==> Tranformed to map
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsInt() for sequence when autoCreate is true and requested in index order
//!
void UT_YamlFile::test_GetAsInt_Sequence_AutoCreate_true_Ordered ()
{
  // ---------------- Setup
  //
  auto path          = "Path1|Path3|Path4";
  auto index         = 0;
  auto defaultValues = {-3, 256, +1, 29};
  auto autoCreate    = true;
  auto gots          = vector<int>();
  YamlFile sut;

  // ---------------- Exercise
  //
  for (const auto& defaultValue : defaultValues)
  {
    auto got = sut.GetAsInt(path, index++, defaultValue, autoCreate);
    gots.push_back(got);
  }

  // ---------------- Verify
  //
  index = 0;
  for (const auto& defaultValue : defaultValues)
  {
    auto got = gots[index++];
    TS_ASSERT_EQUALS (got, defaultValue);
  }

  // Check that previous default values have been stored
  index = 0;
  for (const auto& defaultValue : defaultValues)
  {
    auto got = sut.GetAsInt(path, index++, 0xDead, false);
    TS_ASSERT_EQUALS (got, defaultValue);
  }

  auto expectedYaml = "Path1:\n"
                      "  Path3:\n"
                      "    Path4:\n"
                      "      - -3\n"
                      "      - 256\n"
                      "      - 1\n"
                      "      - 29"s;

  TS_ASSERT_EQUALS (sut.AsString(), expectedYaml);
}


//! Checks YamlFile::GetAsInt() when no default value is provided
//!
void UT_YamlFile::test_GetAsInt_Sequence_NoDefault ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path     = std::get<0>(data);
    auto index    = std::get<1>(data);
    auto expected = std::get<2>(data);

    auto yaml = "Path1: \n"
                " - 9\n"
                " - 8\n"
                " - 7\n"
                "Path_A:\n"
                "  Path_B: \n"
                "    - 62\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: \n"
                "      - 543\n"
                "      - 210\n"
                "  Path_Z: \n"
                "    - -9865";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise
    //
    auto got = sut.GetAsInt(path, index);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.IsDirty());
    TS_ASSERT_EQUALS (got, expected);
  };

  using data_t = tuple<string_view, int, int>;
  auto data =
  {
    data_t{"Path1",                0, 9},     // 0
    data_t{"Path_A|Path_B",        0, 62},    // 1
    data_t{"Path_W|Path_X|Path_Y", 1, 210},   // 2
    data_t{"Path_W|Path_Z",        0, -9865}, // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsInt() when no default value is provided and path does not exist
//!
void UT_YamlFile::test_GetAsInt_Sequence_NoDefault_NotExist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto path  = std::get<0>(data);
    auto index = std::get<1>(data);
    auto yaml  = "Path1: \n"
                 " - 9\n"
                 " - 8\n"
                 " - 7\n"
                 "Path_A:\n"
                 "  Path_B: \n"
                 "    - 62\n"
                 "Path_W:\n"
                 "  Path_X:\n"
                 "    Path_Y: \n"
                 "      - 543\n"
                 "      - 210\n"
                 "  Path_Z: \n"
                 "    - -9865";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut.GetAsInt(path, index), std::exception);
  };

  using data_t = tuple<string_view, int>;
  auto data =
  {
    data_t{"Path1",                3}, // 0
    data_t{"Path2",                0}, // 1
    data_t{"Path_A|Path_C",        0}, // 2
    data_t{"Path_A|Path_B",        1}, // 3
    data_t{"Path_W|Path_Y|Path_Y", 0}, // 4
    data_t{"Path_W|Path_z",        0}, // 5
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks YamlFile::GetAsBool() when providing an, invalid, empty path
//!
void UT_YamlFile::test_GetAsBool_Sequence_EmptyPath ()
{
  // ---------------- Setup
  //
  YamlFile sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.GetAsBool("", 0, false, true), std::exception);
}


//! Checks YamlFile::GetAsBool() at index when autoCreate is false
//!
void UT_YamlFile::test_GetAsBool_Sequence_AutoCreate_false ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto path)
  {
    // ---------------- Setup
    //
    YamlFile sut;

    // ---------------- Exercise
    //
    auto got = sut.GetAsBool(path, 0, true, false);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (got, true);

    // Check that default value has not been stored
    got = sut.GetAsBool(path, 0, false, false);
    TS_ASSERT_EQUALS (got, false);
  };

  auto data =
  {
    "Path1",             // 0
    "Path1|Path2",       // 1
    "Path1|Path2|Path3", // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsBool() when default value is provided, path does not exist and autoCreate is false
//!
void UT_YamlFile::test_GetAsBool_Sequence_AutoCreate_false_NotExist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto path)
  {
    // ---------------- Setup
    //
    auto yaml = "Path1: \n"
                " - false \n"
                " - true\n"
                "Path_A:\n"
                "   Path_B: true\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: false\n"
                "  Path_Z: true";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise
    //
    auto got = sut.GetAsBool(path, 0, false, false);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.IsDirty());
    TS_ASSERT_EQUALS (got, false);
  };

  auto data =
  {
    "Path2",                // 0
    "Path_A|Path_C",        // 1
    "Path_W|Path_Y|Path_Y", // 2
    "Path_W|Path_z",        // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsBool() when autoCreate is true
//!
void UT_YamlFile::test_GetAsBool_Sequence_AutoCreate_true ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path         = std::get<0>(data);
    auto index        = std::get<1>(data);
    auto defaultValue = std::get<2>(data);

    YamlFile sut;

    // ---------------- Exercise
    //
    auto got = sut.GetAsBool(path, index, defaultValue, true);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (got, defaultValue);

    // Check that previous default value has been stored
    got = sut.GetAsBool(path, index, !defaultValue, false);
    TS_ASSERT_EQUALS (got, defaultValue);
  };

  using data_t = tuple<string_view, int,  bool>;
  auto data =
  {
    data_t{"Path1",             0, false}, // 0
    data_t{"Path1|Path2",       0, true},  // 1
    data_t{"Path1|Path3|Path4", 0, false}, // 2
    data_t{"Path1|Path3|Path4", 1, true},  // 3 ==> Tranformed to map
    data_t{"Path1|Path3|Path4", 2, false}, // 4 ==> Tranformed to map
    data_t{"Path1|Path3|Path4", 3, true},  // 5 ==> Tranformed to map
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsBool() for sequence when autoCreate is true and requested in index order
//!
void UT_YamlFile::test_GetAsBool_Sequence_AutoCreate_true_Ordered ()
{
  // ---------------- Setup
  //
  auto path          = "Path1|Path3|Path4";
  auto index         = 0;
  auto defaultValues = {true, false, true, false};
  auto autoCreate    = true;
  auto gots          = vector<bool>();
  YamlFile sut;

  // ---------------- Exercise
  //
  for (const auto& defaultValue : defaultValues)
  {
    auto got = sut.GetAsBool(path, index++, defaultValue, autoCreate);
    gots.push_back(got);
  }

  // ---------------- Verify
  //
  index = 0;
  for (const auto& defaultValue : defaultValues)
  {
    bool got = gots[index++];
    TS_ASSERT_EQUALS (got, defaultValue);
  }

  // Check that previous default values have been stored
  index = 0;
  for (const auto& defaultValue : defaultValues)
  {
    auto got = sut.GetAsBool(path, index++, !defaultValue, false);
    TS_ASSERT_EQUALS (got, defaultValue);
  }

  auto expectedYaml = "Path1:\n"
                      "  Path3:\n"
                      "    Path4:\n"
                      "      - true\n"
                      "      - false\n"
                      "      - true\n"
                      "      - false"s;

  TS_ASSERT_EQUALS (sut.AsString(), expectedYaml);
}


//! Checks YamlFile::GetAsBool() when no default value is provided
//!
void UT_YamlFile::test_GetAsBool_Sequence_NoDefault ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path     = std::get<0>(data);
    auto index    = std::get<1>(data);
    auto expected = std::get<2>(data);

    auto yaml = "Path1: \n"
                " - true\n"
                " - false\n"
                " - true\n"
                "Path_A:\n"
                "  Path_B: \n"
                "    - false\n"
                "Path_W:\n"
                "  Path_X:\n"
                "    Path_Y: \n"
                "      - false\n"
                "      - true\n"
                "  Path_Z: \n"
                "    - false";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise
    //
    auto got = sut.GetAsBool(path, index);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE  (sut.IsDirty());
    TS_ASSERT_EQUALS (got, expected);
  };

  using data_t = tuple<string_view, int, bool>;
  auto data =
  {
    data_t{"Path1",                0, true},  // 0
    data_t{"Path_A|Path_B",        0, false}, // 1
    data_t{"Path_W|Path_X|Path_Y", 1, true},  // 2
    data_t{"Path_W|Path_Z",        0, false}, // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsBool() when no default value is provided and path does not exist
//!
void UT_YamlFile::test_GetAsBool_Sequence_NoDefault_NotExist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](auto data)
  {
    // ---------------- Setup
    //
    auto path  = std::get<0>(data);
    auto index = std::get<1>(data);
    auto yaml  = "Path1: \n"
                 " - true\n"
                 " - false\n"
                 " - true\n"
                 "Path_A:\n"
                 "  Path_B: \n"
                 "    - false\n"
                 "Path_W:\n"
                 "  Path_X:\n"
                 "    Path_Y: \n"
                 "      - false\n"
                 "      - true\n"
                 "  Path_Z: \n"
                 "    - false";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut.GetAsBool(path, index), std::exception);
  };

  using data_t = tuple<string_view, int>;
  auto data =
  {
    data_t{"Path1",                3}, // 0
    data_t{"Path2",                0}, // 1
    data_t{"Path_A|Path_C",        0}, // 2
    data_t{"Path_A|Path_B",        1}, // 3
    data_t{"Path_W|Path_Y|Path_Y", 0}, // 4
    data_t{"Path_W|Path_z",        0}, // 5
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::TryGetAsStringVector() when providing an, invalid, empty path
//!
void UT_YamlFile::test_TryGetAsStringVector_EmptyPath ()
{
  // ---------------- Setup
  //
  YamlFile sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.TryGetAsStringVector(""), std::exception);
}


//! Checks YamlFile::TryGetAsStringVector() when providing not existing empty path
//!
void UT_YamlFile::test_TryGetAsStringVector_NotExist ()
{
  // ---------------- Setup
  //
  auto yaml = "Direction: North\n"
              "Color: [Red, Green, Blue, Yellow]\n"
              "Numbers:\n"
              "  - One\n"
              "  - Two\n"
              "  - Three";

  YamlFile sut;
  sut.Load(yaml);

  bool           succeeded = false;
  vector<string> got;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tie(succeeded, got) = sut.TryGetAsStringVector("Hubert"));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.IsDirty());
  TS_ASSERT_FALSE (succeeded);
  TS_ASSERT_TRUE  (got.empty());
}


//! Checks YamlFile::TryGetAsStringVector() when providing the sequence is empty
//!
void UT_YamlFile::test_TryGetAsStringVector_Empty ()
{
  // ---------------- Setup
  //
  auto yaml = "Direction: North\n"
              "Color: [Red, Green, Blue, Yellow]\n"
              "Numbers:\n"
              "  - One\n"
              "  - Two\n"
              "  - Three\n"
              "Tree:";

  YamlFile sut;
  sut.Load(yaml);

  bool           succeeded = false;
  vector<string> got;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tie(succeeded, got) = sut.TryGetAsStringVector("Tree"));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.IsDirty());
  TS_ASSERT_FALSE (succeeded);
  TS_ASSERT_TRUE  (got.empty());
}


//! Checks YamlFile::TryGetAsStringVector() when providing the sequence is not empty
//!
void UT_YamlFile::test_TryGetAsStringVector_Success ()
{
  // ---------------- Setup
  //
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto  path     = std::get<0>(data);
    const auto& expected = std::get<1>(data);

    auto yaml = "Direction: [North]\n"
                "Color: [Red, Green, Blue, Yellow]\n"
                "Numbers:\n"
                "  - One\n"
                "  - Two\n"
                "  - Three\n"
                "Tree: []";

    YamlFile sut;
    sut.Load(yaml);

    bool           succeeded = false;
    vector<string> got;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (tie(succeeded, got) = sut.TryGetAsStringVector(path));

    // ---------------- Verify
    //
    TS_ASSERT_TRUE   (succeeded);
    TS_ASSERT_EQUALS (got, expected);
  };

  using data_t = tuple<string_view, vector<string>>;
  auto data =
  {
    data_t("Tree",      {}),
    data_t("Direction", {"North"}),
    data_t("Color",     {"Red", "Green", "Blue", "Yellow"}),
    data_t("Numbers",   {"One", "Two",   "Three"}),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::TryGetAsIntVector() when providing an, invalid, empty path
//!
void UT_YamlFile::test_TryGetAsIntVector_EmptyPath ()
{
  // ---------------- Setup
  //
  YamlFile sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.TryGetAsIntVector(""), std::exception);
}


//! Checks YamlFile::TryGetAsIntVector() when providing not existing empty path
//!
void UT_YamlFile::test_TryGetAsIntVector_NotExist ()
{
  // ---------------- Setup
  //
  auto yaml = "Odds:   [1, 3, 5, 7]\n"
              "Primes: [1, 3, 5, 7, 11, 13, 17]\n";

  YamlFile sut;
  sut.Load(yaml);

  bool        succeeded = false;
  vector<int> got;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tie(succeeded, got) = sut.TryGetAsIntVector("Even"));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.IsDirty());
  TS_ASSERT_FALSE (succeeded);
  TS_ASSERT_TRUE  (got.empty());
}


//! Checks YamlFile::TryGetAsIntVector() when providing the sequence is empty
//!
void UT_YamlFile::test_TryGetAsIntVector_Empty ()
{
  // ---------------- Setup
  //
  auto yaml = "Odds:   [1, 3, 5, 7]\n"
              "Primes: [1, 3, 5, 7, 11, 13, 17]\n"
              "Empty:";

  YamlFile sut;
  sut.Load(yaml);

  bool        succeeded = false;
  vector<int> got;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tie(succeeded, got) = sut.TryGetAsIntVector("Empty"));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.IsDirty());
  TS_ASSERT_FALSE (succeeded);
  TS_ASSERT_TRUE  (got.empty());
}


//! Checks YamlFile::TryGetAsIntVector() when providing the sequence is not empty
//!
void UT_YamlFile::test_TryGetAsIntVector_Success ()
{
  // ---------------- Setup
  //
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto  path     = std::get<0>(data);
    const auto& expected = std::get<1>(data);

    auto yaml = "Odds:   [1, 3, 5, 7]\n"
                "Primes: [1, 3, 5, 7, 11, 13, 17]\n"
                "Empty:";

    YamlFile sut;
    sut.Load(yaml);

    bool        succeeded = false;
    vector<int> got;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (tie(succeeded, got) = sut.TryGetAsIntVector(path));

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (got, expected);
  };

  using data_t = tuple<string_view, vector<int>>;
  auto data =
  {
    data_t("Empty",  {}),
    data_t("Odds",   {1, 3, 5, 7}),
    data_t("Primes", {1, 3, 5, 7, 11, 13, 17}),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}




//! Checks YamlFile::Set() for string when providing an invalid empty path
//!
void UT_YamlFile::test_Set_String_EmptyPath ()
{
  // ---------------- Setup
  //
  YamlFile sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Set("", "Foo"s), std::exception);
}


//! Checks YamlFile::GetAsString() when the path does not exist yet
//!
void UT_YamlFile::test_Set_String_NotAlreadyExist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path     = std::get<0>(data);
    auto newValue = std::get<1>(data);

    YamlFile sut;

    // ---------------- Exercise
    //
    auto created = sut.Set(path, newValue);

    // ---------------- Verify
    //
    TS_ASSERT_TRUE   (created);
    TS_ASSERT_TRUE   (sut.IsDirty());

    auto readValue = sut.GetAsString(path);
    TS_ASSERT_EQUALS (readValue, newValue);
  };

  using data_t = tuple<string_view, string>;
  auto data =
  {
    data_t{"Path1",             "Bar"},    // 0
    data_t{"Path1|Path2",       "Hello"},  // 1
    data_t{"Path1|Path2|Path3", "world!"}, // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsString() when the path already exists with a different value
//!
void UT_YamlFile::test_Set_String_AlreadyExist_Different ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path     = std::get<0>(data);
    auto newValue = std::get<1>(data);
    auto yaml     = "Path1: Dibona\n"
                    "Path_A:\n"
                    "  Path_B: Meije\n"
                    "Path_W:\n"
                    "  Path_X:\n"
                    "    Path_Y: Ecrin\n"
                    "  Path_Z: Pelvoux";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise
    //
    auto created = sut.Set(path, newValue);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE (created);
    TS_ASSERT_TRUE  (sut.IsDirty());

    auto readValue = sut.GetAsString(path);
    TS_ASSERT_EQUALS (readValue, newValue);
  };

  using data_t = tuple<string_view, string>;
  auto data =
  {
    data_t{"Path1",                "Bar"},    // 0
    data_t{"Path_A|Path_B",        "Hello"},  // 1
    data_t{"Path_W|Path_X|Path_Y", "world!"}, // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsString() when the path already exists with a same value
//!
void UT_YamlFile::test_Set_String_AlreadyExist_Same ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path     = std::get<0>(data);
    auto newValue = std::get<1>(data);
    auto yaml     = "Path1: Dibona\n"
                    "Path_A:\n"
                    "  Path_B: Meije\n"
                    "Path_W:\n"
                    "  Path_X:\n"
                    "    Path_Y: Ecrin\n"
                    "  Path_Z: Pelvoux";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise
    //
    auto created = sut.Set(path, newValue);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE (created);
    TS_ASSERT_FALSE (sut.IsDirty());

    auto readValue = sut.GetAsString(path);
    TS_ASSERT_EQUALS (readValue, newValue);
    TS_ASSERT_EQUALS (sut.AsString(), yaml);
  };

  using data_t = tuple<string_view, string>;
  auto data =
  {
    data_t{"Path1",                "Dibona"}, // 0
    data_t{"Path_A|Path_B",        "Meije"},  // 1
    data_t{"Path_W|Path_X|Path_Y", "Ecrin"},  // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::Set() for string when providing an invalid empty path
//!
void UT_YamlFile::test_Set_Int_EmptyPath ()
{
  // ---------------- Setup
  //
  YamlFile sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Set("", 12), std::exception);
}


//! Checks YamlFile::GetAsInt() when the path does not exist yet
//!
void UT_YamlFile::test_Set_Int_NotAlreadyExist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path     = std::get<0>(data);
    auto newValue = std::get<1>(data);

    YamlFile sut;

    // ---------------- Exercise
    //
    auto created = sut.Set(path, newValue);

    // ---------------- Verify
    //
    TS_ASSERT_TRUE (created);
    TS_ASSERT_TRUE (sut.IsDirty());

    auto readValue = sut.GetAsInt(path);
    TS_ASSERT_EQUALS (readValue, newValue);
  };

  using data_t = tuple<string_view, int>;
  auto data =
  {
    data_t{"Path1",             -698},  // 0
    data_t{"Path1|Path2",       +2158}, // 1
    data_t{"Path1|Path2|Path3", 0xFad}, // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsInt() when the path already exists with a different value
//!
void UT_YamlFile::test_Set_Int_AlreadyExist_Different ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path     = std::get<0>(data);
    auto newValue = std::get<1>(data);
    auto yaml     = "Path1: 68\n"
                    "Path_A:\n"
                    "  Path_B: -268\n"
                    "Path_W:\n"
                    "  Path_X:\n"
                    "    Path_Y: 0x9\n"
                    "  Path_Z: 0xFeed";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise
    //
    auto created = sut.Set(path, newValue);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE (created);
    TS_ASSERT_TRUE  (sut.IsDirty());

    auto readValue = sut.GetAsInt(path);
    TS_ASSERT_EQUALS (readValue, newValue);
  };

  using data_t = tuple<string_view, int>;
  auto data =
  {
    data_t{"Path1",                -57},      // 0
    data_t{"Path_A|Path_B",        0xBead},   // 1
    data_t{"Path_W|Path_X|Path_Y", 07654321}, // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsInt() when the path already exists with a same value
//!
void UT_YamlFile::test_Set_Int_AlreadyExist_Same ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path     = std::get<0>(data);
    auto newValue = std::get<1>(data);
    auto yaml     = "Path1: 68\n"
                    "Path_A:\n"
                    "  Path_B: -268\n"
                    "Path_W:\n"
                    "  Path_X:\n"
                    "    Path_Y: 0x9\n"
                    "  Path_Z: 0xFeed";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise
    //
    auto created = sut.Set(path, newValue);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE (created);
    TS_ASSERT_FALSE (sut.IsDirty());

    auto readValue = sut.GetAsInt(path);
    TS_ASSERT_EQUALS (readValue, newValue);
    TS_ASSERT_EQUALS (sut.AsString(), yaml);
  };

  using data_t = tuple<string_view, int>;
  auto data =
  {
    data_t{"Path1",                68},   // 0
    data_t{"Path_A|Path_B",        -268}, // 1
    data_t{"Path_W|Path_X|Path_Y", +9},   // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks YamlFile::Set() for string when providing an invalid empty path
//!
void UT_YamlFile::test_Set_Bool_EmptyPath ()
{
  // ---------------- Setup
  //
  YamlFile sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Set("", false), std::exception);
}


//! Checks YamlFile::GetAsBool() when the path does not exist yet
//!
void UT_YamlFile::test_Set_Bool_NotAlreadyExist ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path     = std::get<0>(data);
    auto newValue = std::get<1>(data);

    YamlFile sut;

    // ---------------- Exercise
    //
    auto created = sut.Set(path, newValue);

    // ---------------- Verify
    //
    TS_ASSERT_TRUE (created);
    TS_ASSERT_TRUE (sut.IsDirty());

    auto readValue = sut.GetAsBool(path);
    TS_ASSERT_EQUALS (readValue, newValue);
  };

  using data_t = tuple<string_view, bool>;
  auto data =
  {
    data_t{"Path1",             true},  // 0
    data_t{"Path1|Path2",       false}, // 1
    data_t{"Path1|Path2|Path3", true},  // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsBool() when the path already exists with a different value
//!
void UT_YamlFile::test_Set_Bool_AlreadyExist_Different ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path     = std::get<0>(data);
    auto newValue = std::get<1>(data);
    auto yaml     = "Path1: false\n"
                    "Path_A:\n"
                    "  Path_B: true\n"
                    "Path_W:\n"
                    "  Path_X:\n"
                    "    Path_Y: false\n"
                    "  Path_Z: true";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise
    //
    auto created = sut.Set(path, newValue);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE (created);
    TS_ASSERT_TRUE  (sut.IsDirty());

    auto readValue = sut.GetAsBool(path);
    TS_ASSERT_EQUALS (readValue, newValue);
  };

  using data_t = tuple<string_view, bool>;
  auto data =
  {
    data_t{"Path1",                true},  // 0
    data_t{"Path_A|Path_B",        false}, // 1
    data_t{"Path_W|Path_X|Path_Y", true},  // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks YamlFile::GetAsBool() when the path already exists with a same value
//!
void UT_YamlFile::test_Set_Bool_AlreadyExist_Same ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto path     = std::get<0>(data);
    auto newValue = std::get<1>(data);
    auto yaml     = "Path1: false\n"
                    "Path_A:\n"
                    "  Path_B: true\n"
                    "Path_W:\n"
                    "  Path_X:\n"
                    "    Path_Y: false\n"
                    "  Path_Z: true";

    YamlFile sut;
    sut.Load(yaml);

    // ---------------- Exercise
    //
    auto created = sut.Set(path, newValue);

    // ---------------- Verify
    //
    TS_ASSERT_FALSE (created);
    TS_ASSERT_FALSE (sut.IsDirty());

    auto readValue = sut.GetAsBool(path);
    TS_ASSERT_EQUALS (readValue, newValue);
    TS_ASSERT_EQUALS (sut.AsString(), yaml);
  };

  using data_t = tuple<string_view, bool>;
  auto data =
  {
    data_t{"Path1",                false}, // 0
    data_t{"Path_A|Path_B",        true},  // 1
    data_t{"Path_W|Path_Z",        true},  // 2
    data_t{"Path_W|Path_X|Path_Y", false}, // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}




//! Checks YamlFile::Load() when sut is modified but unsaved and no save requested
//!
void UT_YamlFile::test_Load_Dirty_NoSave ()
{
  // ---------------- Setup
  //
  auto yaml = "Summit: Dibona";

  YamlFile sut;
  sut.Set("Message", "Hello world!");

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Load(yaml, false));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.IsDirty());
  auto asString = sut.AsString();
  TS_ASSERT_EQUALS (asString, yaml);
}



//! Checks YamlFile::Load() when sut is modified but unsaved and prior saving requested
//!
void UT_YamlFile::test_Load_Dirty_SaveButNoAssociatedFile ()
{
  // ---------------- Setup
  //
  YamlFile sut;
  sut.Set("Message", "Hello world!");

  auto yaml = "Summit: Dibona";

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Load(yaml, true), std::exception);  // Cannot save as there is no associated file
}

//! Checks YamlFile::Save() when giving empty file path
//!
void UT_YamlFile::test_Save_EmptyFilePath ()
{
  // ---------------- Setup
  //
  auto     yaml = "Path1: Dibona";

  YamlFile sut;
  TS_ASSERT_THROWS_NOTHING (sut.Load(yaml));

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Save(""), std::exception);
}


//! Checks YamlFile::Save() when there is no associated file
//!
void UT_YamlFile::test_Save_NoAssociatedFile ()
{
  // ---------------- Setup
  //
  auto     yaml = "Path1: Dibona";

  YamlFile sut;
  TS_ASSERT_THROWS_NOTHING (sut.Load(yaml));

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Save(), std::exception);
}


//! Checks YamlFile::Save() when giving a valid (trivial) file path
//!
void UT_YamlFile::test_Save_ValidPath ()
{
  // ---------------- Setup
  //
  auto yaml = "Path1: Dibona\n"
              "Path_A:\n"
              "  Path_B: Meije\n"
              "Path_W:\n"
              "  Path_X:\n"
              "    Path_Y: Ecrin\n"
              "  Path_Z: Pelvoux";

  YamlFile sut;
  TS_ASSERT_THROWS_NOTHING (sut.Load(yaml));

  auto path = "Yaml-test_Save_ExistingPath.yml";
  TS_ASSERT_THROWS_NOTHING (std::remove(path));

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Save(path));

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_FILE_EXISTS  (path);
  TS_ASSERT_FILE_CONTENT (path, yaml);
  TS_ASSERT_THROWS_NOTHING (std::remove(path));
}


//! Checks YamlFile::Save() when giving a invalid file path (that cannot be opened)
//!
void UT_YamlFile::test_Save_InvalidPath ()
{
  // ---------------- Setup
  //
  YamlFile sut;
  TS_ASSERT_THROWS_NOTHING (sut.Load("Path1: Dibona"));

  auto path = "w:/Yaml/__Not valid__/test_Save_InvalidPath.yml";

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Save(path), std::exception);
}


//! Checks YamlFile::CheckFile() when given yaml file path is invalid
//!
void UT_YamlFile::test_CheckFile_InvalidPath ()
{
  // ---------------- Setup
  //
  auto path = "w:/Yaml/__Not valid__/test_CheckFile_InvalidPath.yml";

  auto ok           = true;
  auto errorMessage = ""s;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tie(ok, errorMessage) = YamlFile::CheckFile(path));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (ok);
  auto expectedErrorMessage = "Cannot open YAML file: "s + path;
  TS_ASSERT_EQUALS (errorMessage, expectedErrorMessage);
}


//! Checks YamlFile::CheckFile() when given yaml file content is invalid
//!
void UT_YamlFile::test_CheckFile_InvalidYaml ()
{
  // ---------------- Setup
  //
  CxxTest::setAbortTestOnFail(true);

  auto path   = "Yaml-test_Check_InvalidYaml.yml";
  AT_SCOPE_EXIT([path]{ TS_ASSERT_THROWS_NOTHING(std::remove(path)); });

  // Prepare file
  {
    ofstream ofs(path, std::ios::trunc);
    TS_ASSERT_TRUE (ofs.is_open());

    auto yaml = "Path1: Dibona\n"
                " - Path_A: Ecrin\n"
                " - Path_B: Meije"; // Path1 cannot be scalar and sequence
    ofs << yaml;
  }

  auto ok           = true;
  auto errorMessage = ""s;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tie(ok, errorMessage) = YamlFile::CheckFile(path));

  // ---------------- Verify
  //

  TS_ASSERT_FALSE (ok);
  auto expectedErrorMessage = "yaml-cpp: error at line 2, column 10: illegal map value";
  TS_ASSERT_EQUALS (errorMessage, expectedErrorMessage);
}


//! Checks YamlFile::CheckFile() when given yaml file content is invalid
//!
void UT_YamlFile::test_CheckFile_Valid ()
{
  // ---------------- Setup
  //
  CxxTest::setAbortTestOnFail(true);

  auto path   = "Yaml-test_CheckFile_Valid.yml";
  AT_SCOPE_EXIT([path]{ TS_ASSERT_THROWS_NOTHING(std::remove(path)); });

  // Prepare file
  {
    ofstream ofs(path, std::ios::trunc);
    TS_ASSERT_TRUE (ofs.is_open());

    auto yaml = "Path1: \n"
                " - Path_A: Ecrin\n"
                " - Path_B: Meije"; // Path1 cannot be scalar and sequence
    ofs << yaml;
  }

  auto ok           = true;
  auto errorMessage = ""s;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tie(ok, errorMessage) = YamlFile::CheckFile(path));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (ok);
  TS_ASSERT_EMPTY (errorMessage);
}


//! Loads YamlFile::LoadFile() when given yaml file path is invalid
//!
void UT_YamlFile::test_LoadFile_InvalidPath ()
{
  // ---------------- Setup
  //
  auto path = "w:/Yaml/__Not valid__/test_LoadFile_InvalidPath.yml";

  YamlFile sut;

  auto expectedMessage   = string_view("std::runtime_error: Cannot open YAML file: w:/Yaml/__Not valid__/test_LoadFile_InvalidPath.yml");
  auto startWithExpected = [expectedMessage](string_view message)
  {
    return message.substr(0, expectedMessage.length()) == expectedMessage;
  };

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_PREDICATE (sut.LoadFile(path), const std::exception& excp, startWithExpected, excp.what());
}


//! Loads YamlFile::LoadFile() when given yaml file content is invalid
//!
void UT_YamlFile::test_LoadFile_InvalidYaml ()
{
  // ---------------- Setup
  //
  CxxTest::setAbortTestOnFail(true);

  auto path   = "Yaml-test_Load_InvalidYaml.yml";
  AT_SCOPE_EXIT([path]{ TS_ASSERT_THROWS_NOTHING(std::remove(path)); });

  // Prepare file
  {
    ofstream ofs(path, std::ios::trunc);
    TS_ASSERT_TRUE (ofs.is_open());

    auto yaml = "Path1: Dibona\n"
                " - Path_A: Ecrin\n"
                " - Path_B: Meije"; // Path1 cannot be scalar and sequence
    ofs << yaml;
  }

  YamlFile sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_EQUALS (sut.LoadFile(path), const std::exception& excp, excp.what(), "yaml-cpp: error at line 2, column 10: illegal map value");
}



void UT_YamlFile::test_LoadFile_Valid ()
{
  // ---------------- Setup
  //
  CxxTest::setAbortTestOnFail(true);

  auto path   = "Yaml-test_LoadFile_Valid.yml";
  AT_SCOPE_EXIT([path]{ TS_ASSERT_THROWS_NOTHING(std::remove(path)); });

  // Prepare file
  {
    ofstream ofs(path, std::ios::trunc);
    TS_ASSERT_TRUE (ofs.is_open());

    auto yaml = "Path1: \n"
                " - Path_A: Ecrin\n"
                " - Path_B: Meije"; // Path1 cannot be scalar and sequence
    ofs << yaml;
  }

  YamlFile sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.LoadFile(path));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.IsDirty());
}


//! Loads YamlFile::TryLoadFile() when given yaml file path is invalid
//!
void UT_YamlFile::test_TryLoadFile_InvalidPath ()
{
  // ---------------- Setup
  //
  auto path = "w:/Yaml/__Not valid__/test_TryLoadFile_InvalidPath.yml";

  YamlFile sut;
  bool     succeeded = true;
  string   message;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tie(succeeded, message) = sut.TryLoadFile(path));

  // ---------------- Verify
  //
  auto expectedMessage = "Cannot open YAML file: w:/Yaml/__Not valid__/test_TryLoadFile_InvalidPath.yml";
  TS_ASSERT_FALSE  (succeeded);
  TS_ASSERT_EQUALS (message, expectedMessage);
}


//! Loads YamlFile::TryLoadFile() when given yaml file content is invalid
//!
void UT_YamlFile::test_TryLoadFile_InvalidYaml ()
{
  // ---------------- Setup
  //
  CxxTest::setAbortTestOnFail(true);

  auto path   = "Yaml-test_Load_InvalidYaml.yml";
  AT_SCOPE_EXIT([path]{ TS_ASSERT_THROWS_NOTHING(std::remove(path)); });

  // Prepare file
  {
    ofstream ofs(path, std::ios::trunc);
    TS_ASSERT_TRUE (ofs.is_open());

    auto yaml = "Path1: Dibona\n"
                " - Path_A: Ecrin\n"
                " - Path_B: Meije"; // Path1 cannot be scalar and sequence
    ofs << yaml;
  }

  YamlFile sut;
  bool     succeeded = true;
  string   message;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tie(succeeded, message) = sut.TryLoadFile(path));

  // ---------------- Verify
  //
  auto expectedMessage = "yaml-cpp: error at line 2, column 10: illegal map value";
  TS_ASSERT_FALSE  (succeeded);
  TS_ASSERT_EQUALS (message, expectedMessage);
}



void UT_YamlFile::test_TryLoadFile_Valid ()
{
  // ---------------- Setup
  //
  CxxTest::setAbortTestOnFail(true);

  auto path = "Yaml-test_TryLoadFile_Valid.yml";
  AT_SCOPE_EXIT([path]{ TS_ASSERT_THROWS_NOTHING(std::remove(path)); });

  // Prepare file
  {
    ofstream ofs(path, std::ios::trunc);
    TS_ASSERT_TRUE (ofs.is_open());

    auto yaml = "Path1: \n"
                " - Path_A: Ecrin\n"
                " - Path_B: Meije"; // Path1 cannot be scalar and sequence
    ofs << yaml;
  }

  YamlFile sut;
  bool     succeeded = false;
  string   message   = "?";

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tie(succeeded, message) = sut.TryLoadFile(path));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE  (sut.IsDirty());
  TS_ASSERT_TRUE   (succeeded);
  TS_ASSERT_EMPTY (message);
}



//===========================================================================
// End of UT_YamlFile.cpp
//===========================================================================
