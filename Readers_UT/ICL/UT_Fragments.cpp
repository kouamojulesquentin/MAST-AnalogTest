//===========================================================================
//                           UT_Fragments.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_Fragments.cpp
//!
//! Implements test fixture for testing ICL_Reader
//!
//===========================================================================


#include "ICL_Reader.hpp"
#include "ParserException.hpp"
#include "SystemModel.hpp"
#include "UT_Fragments.hpp"
#include "TestUtilities.hpp"

#include <cxxtest/ValueTraits.h>
#include <experimental/string_view>
#include <memory>

using std::tuple;
using std::make_tuple;
using std::string;
using std::stringstream;
using std::experimental::string_view;
using std::shared_ptr;
using std::make_shared;

using namespace std::string_literals;
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
  return test::GetTestFilePath({"Readers"s, "UT_TestFiles"s, "ICL"s, string(dirName), string(fileName)});
}
//
//  End of: GetTestFilePath
//---------------------------------------------------------------------------


} // End of unnamed namespace


// Checks ICL_Reader::Parse() with fragments files got with Bison parser
//
void UT_Fragments::test_Parse_Parser_Fragments ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto fileName)
  {
    // ---------------- Setup
    //
    auto filePath = GetTestFilePath("Parser_Examples", fileName);
    auto sm       = make_shared<SystemModel>();

    ICL::ICL_Reader sut(sm);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS_NOTHING (sut.Parse(filePath));
  };

  auto files =
  {
    "Parser_Fragment.icl_source.module_items.icl", // 00
    "Parser_Fragment.icl_source.namespace.icl",    // 01
    "Parser_Fragment.icl_source.ports.icl",        // 02
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, files);
}


// Checks ICL_Reader::Parse() with fragments files got with Bison parser that have, intentionally, syntax errors
//
void UT_Fragments::test_Parse_Parser_Fragments_with_SyntaxErrors ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](auto fileName)
  {
    // ---------------- Setup
    //
    auto filePath = GetTestFilePath("Parser_Examples", fileName);
    auto sm       = make_shared<SystemModel>();

    ICL::ICL_Reader sut(sm);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut.Parse(filePath), ICL::ParserException);
  };

  auto files =
  {
    "Parser_Fragment.icl_source.comment.icl", // 00
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST (checker, files);
}

//===========================================================================
// End of UT_Fragments.cpp
//===========================================================================
