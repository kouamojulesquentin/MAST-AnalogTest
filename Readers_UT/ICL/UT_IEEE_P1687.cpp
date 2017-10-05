//===========================================================================
//                           UT_IEEE_P1687.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_IEEE_P1687.cpp
//!
//! Implements test fixture for testing ICL_Reader
//!
//===========================================================================


#include "ICL_Reader.hpp"
#include "PrettyPrinter.hpp"
#include "SystemModelBuilder.hpp"
#include "Utility.hpp"
#include "LoopbackAccessInterfaceProtocol.hpp"
#include "g3log/g3log.hpp"
#include "UT_IEEE_P1687.hpp"
#include "TestUtilities.hpp"

#include "Mast_Core_Traits.hpp"

#include <cxxtest/ValueTraits.h>
#include <experimental/string_view>
#include <iostream>
#include <tuple>
#include <vector>
#include <memory>

using std::tuple;
using std::make_tuple;
using std::string;
using std::stringstream;
using std::experimental::string_view;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;

using namespace std::chrono_literals;
using namespace std::experimental::literals::string_view_literals;
using namespace mast;

namespace
{

//! Returns path of an ICL file accessible by test runner
//!
//! @param dirName    Directory name where file should stand
//! @param fileName   ICL file name (with .icl extension)
//!
string GetTestFilePath (string_view dirName, string_view fileName)
{
  return test::GetTestFilePath({"Readers_UT"s, "UT_TestFiles"s, "ICL"s, string(dirName), string(fileName)});
}
//
//  End of: GetTestFilePath
//---------------------------------------------------------------------------


} // End of unnamed namespace

//! Initializes tests (called for each test)
//!
void UT_IEEE_P1687::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier(); // Needed to check with pretty print that include node identifiers
}

//! Cleanups test (called for each test)
void UT_IEEE_P1687::tearDown ()
{
}


// Checks ICL_Reader::Parse() with all IEEE P1687 example (from Annex E)
//
void UT_IEEE_P1687::test_Parse_Annex_E_Examples ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto fileName)
  {
    // ---------------- Setup
    //
    auto filePath = GetTestFilePath("IEEE_P1687_Examples", fileName);
    auto sm       = make_shared<SystemModel>();

    ICL::ICL_Reader sut(sm);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS_NOTHING (sut.Parse(filePath));
  };

  auto files =
  {
    "IEEE_P1687_Example_1.icl",  // 00
    "IEEE_P1687_Example_2.icl",  // 01
    "IEEE_P1687_Example_3.icl",  // 02
    "IEEE_P1687_Example_4.icl",  // 03
    "IEEE_P1687_Example_5.icl",  // 04
    "IEEE_P1687_Example_6.icl",  // 05
    "IEEE_P1687_Example_7.icl",  // 06
    "IEEE_P1687_Example_8.icl",  // 07
    "IEEE_P1687_Example_9.icl",  // 08
    "IEEE_P1687_Example_10.icl", // 09
    "IEEE_P1687_Example_11.icl", // 10
    "IEEE_P1687_Example_12.icl", // 11
    "IEEE_P1687_Example_13.icl", // 12
    "IEEE_P1687_Example_14.icl", // 13
    "IEEE_P1687_Example_15.icl", // 14
    "IEEE_P1687_Example_16.icl", // 15
    "IEEE_P1687_Example_17.icl", // 16
    "IEEE_P1687_Example_18.icl", // 17
    "IEEE_P1687_Example_19.icl", // 18
    "IEEE_P1687_Example_10.icl", // 19
    "IEEE_P1687_Example_21.icl", // 20
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, files);
}


// Checks ICL_Reader::Parse() with all IEEE P1687 design guidances (from Annex F)
//
void UT_IEEE_P1687::test_Parse_Annex_F_Design_Guidance ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto fileName)
  {
    // ---------------- Setup
    //
    auto filePath = GetTestFilePath("IEEE_P1687_Design_Guidances", fileName);
    auto sm       = make_shared<SystemModel>();

    ICL::ICL_Reader sut(sm);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS_NOTHING (sut.Parse(filePath));
  };

  auto files =
  {
    "IEEE_P1687_Design_Guidance_1.icl",  // 00
    "IEEE_P1687_Design_Guidance_2.icl",  // 01
    "IEEE_P1687_Design_Guidance_3.icl",  // 02
    "IEEE_P1687_Design_Guidance_4.icl",  // 03
    "IEEE_P1687_Design_Guidance_5.icl",  // 04
    "IEEE_P1687_Design_Guidance_6.icl",  // 05
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, files);
}



//===========================================================================
// End of UT_IEEE_P1687.cpp
//===========================================================================
