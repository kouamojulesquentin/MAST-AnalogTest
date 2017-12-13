//===========================================================================
//                           UT_BSDL_Reader.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_BSDL_Reader.cpp
//!
//! Implements test fixture for testing BSDL_Reader
//!
//===========================================================================

#include "UT_BSDL_Reader.hpp"
#include "BSDL_Reader.hpp"
#include "Utility.hpp"
#include "TestUtilities.hpp"

#include <cxxtest/ValueTraits.h>
#include <experimental/string_view>
#include <vector>

using Parsers::BSDL_Reader;
using std::vector;
using std::experimental::string_view;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;


//! Checks BSDL_Reader default constructor
//!
void UT_BSDL_Reader::test_Constructor_Default ()
{
  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING
  (
    BSDL_Reader sut;

    TS_ASSERT_EQUALS (sut.IrBitsCount(), 0u);
    TS_ASSERT_EMPTY  (sut.SelectTable());
  );
}


//! Checks BSDL_Reader::Parse()
//!
void UT_BSDL_Reader::test_Parse ()
{
  // ---------------- Setup
  //
  auto bsdlExcerpt = "Entity Foo is\n"
                     "attribute INSTRUCTION_LENGTH of Foo: entity is 5\n"
                     "attribute INSTRUCTION_OPCODE of Foo: entity is \"EXTEST(00000),\"&\n"
                     "\"SAMPLE(00001),\"&\n"
                     "\"INTEST(00010),\"&\n"
                     "\"ijtag_en(00011),\"&\n"
                     "\"IDCODE(00100),\"&\n"
                     "--\"USRSC(00101),\"&\n"
                     "\"HIGHZ(01110),\"&\n"
                     "\"CLAMP(01111),\"&\n"
                     "\"PROBE(10000),\"&\n"
                     "\"BYPASS(11111)\"\n"sv;


  vector<string_view> instructionNames { "ijtag_en"sv };

  BSDL_Reader sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Parse(bsdlExcerpt, instructionNames));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.IrBitsCount(), 5u);

  vector<string_view> expectedSelectTable { "11111"sv, "00011"sv };

  TS_ASSERT_EQUALS (sut.SelectTable(), expectedSelectTable);
}


//! Checks BSDL_Reader::Parse()
//!
void UT_BSDL_Reader::test_Parse_LargeFile ()
{
  // ---------------- Setup
  //
  auto bsdFilePath = test::GetTestFilePath({"Readers"s, "UT_TestFiles"s, "ICL"s, "Example.bsd"s});
  auto bsdContent  = mast::Utility::ReadTextFile(bsdFilePath);

  vector<string_view> instructionNames { "BYPASS"sv, "EXTEST"sv, "IDCODE"sv, "SAMPLE"sv };

  BSDL_Reader sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.Parse(bsdContent, instructionNames));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.IrBitsCount(), 3u);

  vector<string_view> expectedSelectTable { "111"sv, "000"sv, "010"sv, "001"sv };

  TS_ASSERT_EQUALS (sut.SelectTable(), expectedSelectTable);
}


//===========================================================================
// End of UT_BSDL_Reader.cpp
//===========================================================================
