//===========================================================================
//                           UT_YamlLib.cpp
//===========================================================================
//
//! @file UT_YamlLib.cpp
//!
//! Implements test fixture for testing Yaml library
//!
//===========================================================================

#include "UT_YamlLib.hpp"
#include "yaml-cpp/yaml.h"
#include "Yaml_Traits.hpp"

#include <string>
#include <experimental/string_view>
#include <tuple>
#include <vector>
#include <map>
#include <initializer_list>

using std::vector;
using std::map;
using std::tuple;
using std::make_tuple;
using std::string;
using std::experimental::string_view;
using std::initializer_list;
using namespace std::string_literals;

using YAML::NodeType;

namespace
{
  template<typename T> void Check_Assignment(const T& data)
  {
    // ---------------- Setup
    //
    const auto yaml         = std::get<0>(data);
    const auto nodeToChange = std::get<1>(data);
    const auto newValue     = std::get<2>(data);
    const auto expected     = std::get<3>(data);
    auto       baseNode     = YAML::Load(yaml);

    // ---------------- Exercise
    //
    baseNode[nodeToChange] = newValue;

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);
    YAML::Emitter emitter;
    emitter.SetOutputCharset(YAML::EmitNonAscii);
//+    emitter.SetDoublePrecision(6u);
//+    emitter.SetFloatPrecision(3u);
    emitter << baseNode;
    TS_ASSERT_TRUE (emitter.good());

    auto got = emitter.str();
    TS_ASSERT_EQUALS (got, expected);
  }
} // End of unnamed namespace


//! Initializes test (called for each test)
//!
void UT_YamlLib::setUp ()
{
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped
}


//! Checks Yaml library YAML::Load()
//!
void UT_YamlLib::test_Load_String_Empty ()
{
  // ---------------- Exercise
  //
  YAML::Node baseNode = YAML::Load("");

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (baseNode.IsNull());
}


//! Checks Yaml library YAML::operator[] for a simple string scalar
//!
void UT_YamlLib::test_Subscript_Operator_Scalar_String ()
{
  // ---------------- Setup
  //
  auto data           = "Test: Hello";
  YAML::Node baseNode = YAML::Load(data);

  // ---------------- Exercise
  //
  auto testNode = baseNode["Test"];

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_EQUALS (testNode.Type(), NodeType::Scalar);
  TS_ASSERT_EQUALS (testNode.size(), 0u);
  TS_ASSERT_EQUALS (testNode.as<string>() , "Hello");
}


//! Checks Yaml library YAML::operator[] for key with int value
//!
void UT_YamlLib::test_Subscript_Operator_Scalar_Int ()
{
  // ---------------- Setup
  //
  auto data           = "Test: -123";
  YAML::Node baseNode = YAML::Load(data);

  // ---------------- Exercise
  //
  auto testNode = baseNode["Test"];

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_EQUALS (testNode.Type(), NodeType::Scalar);
  TS_ASSERT_EQUALS (testNode.size(), 0u);
  TS_ASSERT_EQUALS (testNode.as<int>(), -123);
}


//! Checks Yaml library YAML::operator[] for key without value
//!
void UT_YamlLib::test_Subscript_Operator_KeyWithoutValue ()
{
  // ---------------- Setup
  //
  auto data           = "Test:";
  YAML::Node baseNode = YAML::Load(data);

  // ---------------- Exercise
  //
  auto testNode = baseNode["Test"];

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_EQUALS (testNode.Type(), NodeType::Null);
  TS_ASSERT_TRUE   (testNode.IsNull());
}


//! Checks Yaml library YAML::operator[] for not exiting key
//!
void UT_YamlLib::test_Subscript_Operator_NotExistingKey ()
{
  // ---------------- Setup
  //
  auto data           = "Test: Hello";
  YAML::Node baseNode = YAML::Load(data);

  // ---------------- Exercise
  //
  auto testNode = baseNode["foo"];

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_EQUALS (testNode.Type(), NodeType::Undefined);
}



//! Checks Yaml library YAML::Load() for a simple sequence
//!
void UT_YamlLib::test_Load_String_1_Sequence ()
{
  // ---------------- Setup
  //
  auto data = "List: [Player, Monster, Gem]";

  // ---------------- Exercise
  //
  YAML::Node baseNode = YAML::Load(data);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_FALSE  (baseNode.IsNull());
  TS_ASSERT_EQUALS (baseNode.Type(), NodeType::Map);
  TS_ASSERT_EQUALS (baseNode.size(), 1u);

  auto listNode = baseNode["List"];

  TS_ASSERT_EQUALS (listNode.Type(), NodeType::Sequence);
  TS_ASSERT_EQUALS (listNode.size(), 3u);
  TS_ASSERT_EQUALS (listNode[0].as<string>() , "Player");
  TS_ASSERT_EQUALS (listNode[1].as<string>(),  "Monster");
  TS_ASSERT_EQUALS (listNode[2].as<string>(),  "Gem");
}


//! Checks Yaml library YAML::Load() with more than one "root" node
//!
void UT_YamlLib::test_Load_String_N_Sequence ()
{
  // ---------------- Setup
  //
  auto data = "List: [Player, Monster, Gem]\n"
              "Number: 123";

  // ---------------- Exercise
  //
  YAML::Node baseNode = YAML::Load(data);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_FALSE  (baseNode.IsNull());
  TS_ASSERT_EQUALS (baseNode.Type(), NodeType::Map);
  TS_ASSERT_EQUALS (baseNode.size(), 2u);

  auto listNode = baseNode["List"];

  TS_ASSERT_EQUALS (listNode.Type(), NodeType::Sequence);
  TS_ASSERT_EQUALS (listNode.size(), 3u);
  TS_ASSERT_EQUALS (listNode[0].as<string>() , "Player");
  TS_ASSERT_EQUALS (listNode[1].as<string>(),  "Monster");
  TS_ASSERT_EQUALS (listNode[2].as<string>(),  "Gem");

  auto numberNode = baseNode["Number"];

  TS_ASSERT_EQUALS (numberNode.Type(), NodeType::Scalar);
  TS_ASSERT_EQUALS (numberNode.size(), 0u);
  TS_ASSERT_EQUALS (numberNode.as<int>(), 123);
}


//! Checks Yaml library YAML::Load() with some "complex" string value
//!
void UT_YamlLib::test_Load_String_Complex_StringItem ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto yaml     = "Text: "s + std::get<0>(data);
    auto expected = string(std::get<1>(data));

    // ---------------- Exercise
    //
    YAML::Node baseNode = YAML::Load(yaml);

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);

    auto textNode = baseNode["Text"];
    auto value    = textNode.as<string>();

    TS_ASSERT_EQUALS (value, expected);
  };

  auto data =
  {
    make_tuple("<LFN><PROJECT_PATH>ProjectFiles.txt",                       "<LFN><PROJECT_PATH>ProjectFiles.txt"),                    // 0
    make_tuple(R"('/SearchRegex=.*\\(\w+)\.(\w+)/ReplaceRegex=UT_\1.\2')",  R"(/SearchRegex=.*\\(\w+)\.(\w+)/ReplaceRegex=UT_\1.\2)"), // 1
    make_tuple(R"(/SearchRegex=(w++).(w++)/ReplaceRegex=UT_\1.\2)",         R"(/SearchRegex=(w++).(w++)/ReplaceRegex=UT_\1.\2)"),      // 2
    make_tuple("'*~~#UTF-8#\\\\.*;*\\\\CVS\\\\*.*'",                        R"(*~~#UTF-8#\\.*;*\\CVS\\*.*)"),                          // 3
    make_tuple("\"*~~#UTF-8#\\\\\\\\.*;*\\\\\\\\CVS\\\\\\\\*.*\"",          R"(*~~#UTF-8#\\.*;*\\CVS\\*.*)"),                          // 4
    make_tuple(">\n  Mark set a major league\n  home run record in 1998.",  "Mark set a major league home run record in 1998."),       // 5
    make_tuple("|\n   65 Home Runs\n   0.278 Batting Average",              "65 Home Runs\n0.278 Batting Average"),                    // 6
    make_tuple("\n  This unquoted scalar\n   spans many lines.",            "This unquoted scalar spans many lines."),                 // 7
    make_tuple("\"So does this\n  quoted scalar.\n\"",                      "So does this quoted scalar. "),                           // 8
    make_tuple("|\n   Alpha\n   Bravo\n      Charly",                       "Alpha\nBravo\n   Charly"),                                // 9
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Yaml library YAML::Load() in case there is a syntax error
//!
void UT_YamlLib::test_Load_String_SyntaxError ()
{
  // ---------------- Setup
  //
  auto data = "List: [Player, Monster], Gem";

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (YAML::Load(data), YAML::ParserException);
}

//! Checks Yaml library YAML::Emitter with an empty string "source"
//!
void UT_YamlLib::test_Emitter_Empty ()
{
  // ---------------- Setup
  //
  YAML::Node    baseNode = YAML::Load("");
  YAML::Emitter emitter;

  // ---------------- Exercise
  //
  emitter << baseNode;

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (emitter.good());

  auto got = emitter.str();
  TS_ASSERT_EMPTY (got);
}


//! Checks Yaml library YAML::Emitter with some string items
//!
void UT_YamlLib::test_Emitter_String ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto yaml     = "Item: "s + std::get<0>(data);
    auto expected = "Item: "s + std::get<1>(data);
    auto baseNode = YAML::Load(yaml);

    YAML::Emitter emitter;
    emitter.SetOutputCharset(YAML::EmitNonAscii);
    emitter.SetOutputCharset(YAML::DoubleQuoted);
//+    emitter.SetStringFormat(YAML::Literal);
//+    emitter.SetStringFormat(YAML::SingleQuoted);
//+    emitter.SetOutputCharset(YAML::EscapeNonAscii);
//+    emitter.SetIndent(4);
//+    emitter.SetMapFormat(YAML::Flow);

    // ---------------- Exercise
    //
    emitter << baseNode;

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);
    TS_ASSERT_TRUE (emitter.good());

    auto got = emitter.str();
    TS_ASSERT_EQUALS (got, expected);

    // Round-trip check (cannot compare baseNode & rountripNode)
    auto rountripNode = YAML::Load(got);
    YAML::Emitter roundTripEmitter;
    roundTripEmitter << rountripNode;
    auto roundTripGot = roundTripEmitter.str();
    TS_ASSERT_EQUALS (roundTripGot, expected);
  };

  auto data =
  {
    make_tuple("<LFN><PROJECT_PATH>ProjectFiles.txt",                       "<LFN><PROJECT_PATH>ProjectFiles.txt"),                         // 0
    make_tuple("\n  This unquoted scalar\n   spans many lines.",            "This unquoted scalar spans many lines."),                      // 1
    make_tuple(R"(/SearchRegex=(w++).(w++)/ReplaceRegex=UT_\1.\2)",         R"(/SearchRegex=(w++).(w++)/ReplaceRegex=UT_\1.\2)"),           // 2
    make_tuple(R"('/SearchRegex=.*\\(\w+)\.(\w+)/ReplaceRegex=UT_\1.\2')",  R"(/SearchRegex=.*\\(\w+)\.(\w+)/ReplaceRegex=UT_\1.\2)"), // 3
    make_tuple("'*~~#UTF-8#\\\\.*;*\\\\CVS\\\\*.*'",                        "\"*~~#UTF-8#\\\\\\\\.*;*\\\\\\\\CVS\\\\\\\\*.*\""),       // 4
    make_tuple("\"*~~#UTF-8#\\\\\\\\.*;*\\\\\\\\CVS\\\\\\\\*.*\"",          "\"*~~#UTF-8#\\\\\\\\.*;*\\\\\\\\CVS\\\\\\\\*.*\""),       // 5
    make_tuple(">\n  Mark set a major league\n  home run record in 1998.",  "Mark set a major league home run record in 1998."),       // 6
    make_tuple("|\n   65 Home Runs\n   0.278 Batting Average",              "\"65 Home Runs\\n0.278 Batting Average\""),               // 7
    make_tuple("\"So does this\n  quoted scalar.\n\"",                      "\"So does this quoted scalar. \""),                       // 8
    make_tuple("|\n   Alpha\n   Bravo\n      Charly",                       "\"Alpha\\nBravo\\n   Charly\""),                          // 9
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}





//! Checks Yaml library YAML::Emitter with some integer items
//!
void UT_YamlLib::test_Emitter_Int ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto yaml     = "Item: "s + std::get<0>(data);
    auto expected = "Item: "s + std::get<1>(data);
    auto baseNode = YAML::Load(yaml);

    YAML::Emitter emitter;

    // ---------------- Exercise
    //
    emitter << baseNode;

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);
    TS_ASSERT_TRUE (emitter.good());

    auto got = emitter.c_str();
    TS_ASSERT_EQUALS (got, expected);
  };

  auto data =
  {
    make_tuple("123",  "123"),  // 0
    make_tuple("+456", "+456"), // 1
    make_tuple("-789", "-789"), // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Yaml library YAML::Emitter with some double items
//!
void UT_YamlLib::test_Emitter_Double ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto yaml     = "Item: "s + std::get<0>(data);
    auto expected = "Item: "s + std::get<1>(data);
    auto baseNode = YAML::Load(yaml);

    YAML::Emitter emitter;

    // ---------------- Exercise
    //
    emitter << baseNode;

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);
    TS_ASSERT_TRUE (emitter.good());

    auto got = emitter.c_str();
    TS_ASSERT_EQUALS (got, expected);
  };

  auto data =
  {
    make_tuple("123.45",    "123.45"),    // 0
    make_tuple("+456.7801", "+456.7801"), // 1
    make_tuple("-789.002",  "-789.002"),  // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}

//! Checks Yaml library YAML::Emitter with some sequence items
//!
void UT_YamlLib::test_Emitter_Sequence ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto yaml     = "Item:"s + std::get<0>(data);
    auto expected = "Item:"s + std::get<1>(data);
    auto baseNode = YAML::Load(yaml);

    YAML::Emitter emitter;
    emitter.SetOutputCharset(YAML::EmitNonAscii);
    emitter.SetIndent(4);

    // ---------------- Exercise
    //
    emitter << baseNode;

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);
    TS_ASSERT_TRUE (emitter.good());

    auto got = emitter.str();
    TS_ASSERT_EQUALS (got, expected);

    // Round-trip check (cannot compare baseNode & rountripNode)
    auto rountripNode = YAML::Load(got);
    YAML::Emitter roundTripEmitter;
    roundTripEmitter.SetIndent(4);
    roundTripEmitter << rountripNode;
    auto roundTripGot = roundTripEmitter.str();
    TS_ASSERT_EQUALS (roundTripGot, expected);
  };

  auto data =
  {
    make_tuple(" [Alpha,   Bravo, Charly]",                       " [Alpha, Bravo, Charly]"),                                           // 0
    make_tuple("\n  - One\n  - Two\n  - Three",                   "\n    -   One\n    -   Two\n    -   Three"),                         // 1
    make_tuple("\n- Hello World\n- How is it\n- Going this days", "\n    -   Hello World\n    -   How is it\n    -   Going this days"), // 2
    make_tuple(" [Apple, Peer, Banana] # And a comment",          " [Apple, Peer, Banana]"),                                            // 3 - Comments are discarded while parsing
    make_tuple(" [Apple, Peer, Banana]\n# And a comment",         " [Apple, Peer, Banana]"),                                            // 4 - Comments are discarded while parsing
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Yaml library YAML::Emitter with some map items
//!
void UT_YamlLib::test_Emitter_Map ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto yaml     = std::get<0>(data);
    auto expected = std::get<1>(data);
    auto baseNode = YAML::Load(yaml);

    YAML::Emitter emitter;
    emitter.SetOutputCharset(YAML::EmitNonAscii);

    // ---------------- Exercise
    //
    emitter << baseNode;

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);
    TS_ASSERT_TRUE (emitter.good());

    auto got = emitter.str();
    TS_ASSERT_EQUALS (got, expected);

    // Round-trip check (cannot compare baseNode & rountripNode)
    auto rountripNode = YAML::Load(got);
    YAML::Emitter roundTripEmitter;
    roundTripEmitter << rountripNode;
    auto roundTripGot = roundTripEmitter.str();
    TS_ASSERT_EQUALS (roundTripGot, expected);
  };

  auto data =
  {
    make_tuple("One: 1\nTwo: 2\nThree: 3",                                                        "One: 1\nTwo: 2\nThree: 3"),                                                      // 0
    make_tuple("Number: {One: 1, Two: 2, Three: 3}",                                              "Number: {One: 1, Two: 2, Three: 3}"),                                            // 1
    make_tuple("Set Numbers: [1, 2, 3, 4.5]\nSet Fruits: [Apple, Peer, Orange]",                  "Set Numbers: [1, 2, 3, 4.5]\nSet Fruits: [Apple, Peer, Orange]"),                // 2
    make_tuple("Sets:\n   - Set Numbers: [1, 2, 3, 4.5]\n   - Set Fruits: [Apple, Peer, Orange]", "Sets:\n  - Set Numbers: [1, 2, 3, 4.5]\n  - Set Fruits: [Apple, Peer, Orange]"), // 3
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Yaml library YAML::Emitter in context of anchor
//!
void UT_YamlLib::test_Emitter_Anchor ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto yaml     = std::get<0>(data);
    auto expected = std::get<1>(data);
    auto baseNode = YAML::Load(yaml);

    YAML::Emitter emitter;
    emitter.SetOutputCharset(YAML::EmitNonAscii);

    // ---------------- Exercise
    //
    emitter << baseNode;

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);
    TS_ASSERT_TRUE (emitter.good());

    auto got = emitter.str();
    TS_ASSERT_EQUALS (got, expected);

    // Round-trip check (cannot compare baseNode & rountripNode)
    auto rountripNode = YAML::Load(got);
    YAML::Emitter roundTripEmitter;
    roundTripEmitter << rountripNode;
    auto roundTripGot = roundTripEmitter.str();
    TS_ASSERT_EQUALS (roundTripGot, expected);
  };

  auto data =
  {
    // 0 (for now - October/20/2016 - the library discard anchor names)
    make_tuple("Number: &Num    {One: 1, Two: 2, Three: 3}\n"
               "Fruits: &Fruits [Apple, Peer, Orange]\n"
               "Data:\n"
               "  - m1: *Num\n"
               "  - m2: *Fruits\n\n",
               "Number: &1 {One: 1, Two: 2, Three: 3}\n"
               "Fruits: &2 [Apple, Peer, Orange]\n"
               "Data:\n"
               "  - m1: *1\n"
               "  - m2: *2"), // Trailing \n, at EOF, are discarded too
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks Yaml library YAML::Node value change (text)
//!
void UT_YamlLib::test_Assignment_String ()
{
  // ---------------- DDT Setup
  //
  using data_t = tuple<string, string, string, string>;
  auto data =
  {
    // 0
    make_tuple("Number: Foo\n", // Original
               "Number",        // Item (node to change)
               "Bar",           // New value
               "Number: Bar"),  // Expected result

    // 1
    make_tuple("Number: Foo\nOther Item: Hello World", // Original
               "Other Item",                           // Item (node to change)
               "Hi there!",                            // New value
               "Number: Foo\nOther Item: Hi there!"),  // Expected result
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(Check_Assignment<data_t>, data);
}


//! Checks Yaml library YAML::Node value change (int)
//!
void UT_YamlLib::test_Assignment_Int ()
{
  // ---------------- DDT Setup
  //
  using data_t = tuple<string, string, int, string>;
  auto data =
  {
    // 0
    make_tuple("Number: 123\n", // Original
               "Number",        // Item (node to change)
               +789,            // New value
               "Number: 789"),  // Expected result

    // 1
    make_tuple("Number: 123\n", // Original
               "Number",        // Item (node to change)
               -456,            // New value
               "Number: -456"), // Expected result
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(Check_Assignment<data_t>, data);
}


//! Checks Yaml library YAML::Node value change (double)
//!
void UT_YamlLib::test_Assignment_Double ()
{
  // ---------------- DDT Setup
  //
  using data_t = tuple<string, string, double, string>;
  auto data =
  {
    // 0
    make_tuple("Number: 123\n", // Original
               "Number",        // Item (node to change)
               +789.2401,       // New value
               "Number: 789.2401"), // Expected result

    // 1
    make_tuple("Number: 1.23\n",    // Original
               "Number",            // Item (node to change)
               -45.6009,            // New value
               "Number: -45.6009"), // Expected result
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(Check_Assignment<data_t>, data);
}


//! Checks Yaml library YAML::Node value change (float)
//!
void UT_YamlLib::test_Assignment_Float ()
{
  // ---------------- DDT Setup
  //
  using data_t = tuple<string, string, float, string>;
  auto data =
  {
    // 0
    make_tuple("Number: 123\n",     // Original
               "Number",            // Item (node to change)
               +789.2401f,          // New value
               "Number: 789.2401"), // Expected result

    // 1
    make_tuple("Number: 1.23\n",    // Original
               "Number",            // Item (node to change)
               -45.6009f,           // New value
               "Number: -45.6009"), // Expected result
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(Check_Assignment<data_t>, data);
}


//! Checks Yaml library YAML::Node value change (sequence with vector)
//!
void UT_YamlLib::test_Assignment_Sequence_Vector ()
{
  // ---------------- DDT Setup
  //
  using data_t = tuple<string_view, string, vector<string>, string>;
  auto data =
  {
    // 0
    data_t("Items: Foo",                           // Original
           "Items",                                // Item (node to change)
           {"One", "Two", "Three"},                // New value
           "Items:\n  - One\n  - Two\n  - Three"), // Expected result
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(Check_Assignment<data_t>, data);
}


//! Checks Yaml library YAML::Node value change (sequence with map)
//!
void UT_YamlLib::test_Assignment_Sequence_Map ()
{
  // ----------------  Setup
  //
  auto baseNode = YAML::Load("Items: Foo");

  // ---------------- Exercise
  //
  baseNode["Items"] = map<string, int>{{"One", 1}, {"Two", 2}, {"Three", 3}};

  // ---------------- Verify
  //
  auto itemsNode = baseNode["Items"];

  TS_ASSERT_EQUALS (itemsNode["One"].as<int>(),   1);
  TS_ASSERT_EQUALS (itemsNode["Two"].as<int>(),   2);
  TS_ASSERT_EQUALS (itemsNode["Three"].as<int>(), 3);
}

//! Checks Yaml library YAML::Node value change (sequence with initializer_list)
//!
void UT_YamlLib::test_Assignment_Sequence_I_List ()
{
  // ----------------  Setup
  //
  auto baseNode = YAML::Load("Items: Foo");

  // ---------------- Exercise
  //
  baseNode["Items"] = {"One", "Two", "Three"};

  // ---------------- Verify
  //
  YAML::Emitter emitter;
  emitter << baseNode;

  auto got      = emitter.str();
  auto expected = "Items:\n  - One\n  - Two\n  - Three";
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks Yaml library YAML::Node value change (sequence with initializer_list of vectors)
//!
void UT_YamlLib::test_Assignment_Sequence_I_List_of_Vector ()
{
  // ----------------  Setup
  //
  auto baseNode = YAML::Load("Items: []");

  // ---------------- Exercise
  //
  baseNode["Items"] = {vector<string>{"One", "Two", "Three"}, vector<string>{"Apple", "Peer"}, vector<string>{"45", "-12", "0"}};

  // ---------------- Verify
  //
  YAML::Emitter emitter;
  emitter << baseNode;

  auto got      = emitter.str();
  auto expected = "Items:\n"
                  "  -\n"
                  "    - One\n"
                  "    - Two\n"
                  "    - Three\n"
                  "  -\n"
                  "    - Apple\n"
                  "    - Peer\n"
                  "  -\n"
                  "    - 45\n"
                  "    - -12\n"
                  "    - 0";

  TS_ASSERT_EQUALS (got, expected);
}


//! Checks Yaml library YAML::Node value change (sequence with initializer_list of vectors)
//!
void UT_YamlLib::test_PushBack_Vector ()
{
  // ----------------  Setup
  //
  auto baseNode = YAML::Load("Items: []");    // Make base node a sequence

  // ---------------- Exercise
  //
  baseNode["Items"].push_back(vector<string>{"One", "Two", "Three"});
  baseNode["Items"].push_back(vector<string>{"Apple", "Peer"});
  baseNode["Items"].push_back(vector<int>{45, -12, 0});

  // ---------------- Verify
  //
  YAML::Emitter emitter;
  emitter << baseNode;

  auto got      = emitter.str();
  auto expected = "Items: [[One, Two, Three], [Apple, Peer], [45, -12, 0]]";

  TS_ASSERT_EQUALS (got, expected);
}

//! Checks Yaml library YAML::Node value change (sequence with map)
//!
void UT_YamlLib::test_PushBack_Map ()
{
  // ----------------  Setup
  //
  auto baseNode = YAML::Load("Items: [Apple, Peer]");
  auto value    = map<string, int>{{"One", 1}, {"Two", 2}, {"Three", 3}};

  // ---------------- Exercise
  //
  baseNode["Items"].push_back(value);

  // ---------------- Verify
  //
  auto itemsNode = baseNode["Items"];

  TS_ASSERT_EQUALS (itemsNode[0].as<string>(), "Apple");
  TS_ASSERT_EQUALS (itemsNode[1].as<string>(), "Peer");

  auto mapNode = itemsNode[2];  // Skip Apple and Peer

  TS_ASSERT_EQUALS (mapNode["One"].as<int>(),   1);
  TS_ASSERT_EQUALS (mapNode["Two"].as<int>(),   2);
  TS_ASSERT_EQUALS (mapNode["Three"].as<int>(), 3);
}

//! Checks Yaml library YAML::Node value change (sequence with initializer_list of vectors)
//!
void UT_YamlLib::test_PushBack_I_List ()
{
  // ----------------  Setup
  //
  auto baseNode = YAML::Load("Items: []");    // Make base node a sequence

  // ---------------- Exercise
  //
  baseNode["Items"].push_back({"One", "Two", "Three"});
  baseNode["Items"].push_back({"Apple", "Peer"});
  baseNode["Items"].push_back({45, -12, 0});

  // ---------------- Verify
  //
  YAML::Emitter emitter;
  emitter << baseNode;

  auto got      = emitter.str();
  auto expected = "Items: [[One, Two, Three], [Apple, Peer], [45, -12, 0]]";

  TS_ASSERT_EQUALS (got, expected);
}

//! Checks Yaml library to build up map type "path nodes"
//!
void UT_YamlLib::test_PathBuilding_Map_1st_Nodes ()
{
  // ---------------- Setup
  //
  YAML::Node baseNode(NodeType::Map);

  // ---------------- Exercise
  //
  baseNode["Foo"] = YAML::Node(NodeType::Map);

  // ---------------- Verify
  //
  auto fooNode    = baseNode["Foo"];
  TS_ASSERT_EQUALS (fooNode.Type(), NodeType::Map);

  YAML::Emitter emitter;
  emitter << baseNode;

  auto got      = emitter.str();
  auto expected = "Foo:\n"
                  "  {}";

  TS_ASSERT_EQUALS (got, expected);
}


//! Checks Yaml library to build up map type "path nodes"
//!
void UT_YamlLib::test_PathBuilding_Map_2nd_Nodes ()
{
  // ---------------- Setup
  //
  YAML::Node baseNode(NodeType::Map);
  baseNode["Foo"] = YAML::Node(NodeType::Map);
  auto fooNode    = baseNode["Foo"];

  // ---------------- Exercise
  //
  fooNode["Bar"] = YAML::Node(NodeType::Map);

  // ---------------- Verify
  //
  auto barNode   = fooNode["Bar"];
  TS_ASSERT_EQUALS (barNode.Type(), NodeType::Map);

  YAML::Emitter emitter;
  emitter << baseNode;

  auto got      = emitter.str();
  auto expected = "Foo:\n"
                  "  Bar:\n"
                  "    {}";

  TS_ASSERT_EQUALS (got, expected);
}


//! Checks Yaml library to build up map type "path nodes"
//!
void UT_YamlLib::test_PathBuilding_Map_3rd_Nodes ()
{
  // ---------------- Setup
  //
  YAML::Node baseNode(NodeType::Map);
  baseNode["Foo"] = YAML::Node(NodeType::Map);
  auto fooNode    = baseNode["Foo"];

  fooNode["Bar"] = YAML::Node(NodeType::Map);
  auto barNode   = fooNode["Bar"];

  // ---------------- Exercise
  //
  barNode["Third"] = YAML::Node(NodeType::Map);
  auto thirdNode   = barNode["Third"];

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (thirdNode.Type(), NodeType::Map);

  YAML::Emitter emitter;
  emitter << baseNode;

  auto got      = emitter.str();
  auto expected = "Foo:\n"
                  "  Bar:\n"
                  "    Third:\n"
                  "      {}";

  TS_ASSERT_EQUALS (got, expected);
}



//! Checks Node copy
//!
void UT_YamlLib::test_NodeCopy ()
{
  // ---------------- Setup
  //
  auto yaml = "Tag:\n"
              "  Lang_C:\n"
              "    key_1: Hello\n"
              "    Key_2: World"s;

  YAML::Node baseNode = YAML::Load(yaml);
  YAML::Node node     = baseNode["Tag"]["Lang_C"]["key_1"];
  YAML::Node nodeCopy;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (nodeCopy = node);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (nodeCopy,              node);
  TS_ASSERT_EQUALS (node.as<string>(),     "Hello");
  TS_ASSERT_EQUALS (nodeCopy.as<string>(), "Hello");

  nodeCopy = "Dibona";
  TS_ASSERT_EQUALS (nodeCopy,              node);
  TS_ASSERT_EQUALS (node.as<string>(),     "Dibona");
  TS_ASSERT_EQUALS (nodeCopy.as<string>(), "Dibona");
}


//===========================================================================
// End of UT_YamlLib.cpp
//===========================================================================
