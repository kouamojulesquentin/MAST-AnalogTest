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
#include "PrettyPrinter.hpp"
#include "AST.hpp"
#include "AST_Module.hpp"
#include "AST_PrettyPrinter.hpp"
#include "UT_Fragments.hpp"
#include "TestUtilities.hpp"
#include "Mast_Core_Traits.hpp"

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
class ICL_Reader_TSS : public ICL::ICL_Reader
{
  public:
  ICL_Reader_TSS(std::shared_ptr<mast::SystemModel> sm) : ICL_Reader(sm) {  }
  using ICL_Reader::AST;
};

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

auto excerpt_Module_SReg_parameterized = "Module SReg\n"
                                         "{\n"
                                         "  Parameter     MSB = 7;\n"
                                         "  ScanInPort    SI;\n"
                                         "  ScanOutPort   SO { Source SR[0];}\n"
                                         "  ShiftEnPort   SE;\n"
                                         "  CaptureEnPort CE;\n"
                                         "  UpdateEnPort  UE;\n"
                                         "  SelectPort    SEL;\n"
                                         "  ResetPort     RST;\n"
                                         "  TCKPort       TCK;\n"
                                         "  DataInPort    DI[$MSB:0];\n"
                                         "  DataOutPort   DO[$MSB:0]  {Source   SR; }\n"
                                         "  ScanInterface scan_client { Port SI; Port SO; Port SEL; }\n"
                                         "  ScanRegister  SR[$MSB:0]\n"
                                         "  {\n"
                                         "    ScanInSource  SI;\n"
                                         "    CaptureSource DI;\n"
                                         "    ResetValue    'b0;\n"
                                         "  }\n"
                                         "}\n"s;

auto excerpt_Module_SReg_8_bits = "Module SReg\n"
                                  "{\n"
                                  "  ScanInPort    SI;\n"
                                  "  ScanOutPort   SO { Source SR[0];}\n"
                                  "  ShiftEnPort   SE;\n"
                                  "  CaptureEnPort CE;\n"
                                  "  UpdateEnPort  UE;\n"
                                  "  SelectPort    SEL;\n"
                                  "  ResetPort     RST;\n"
                                  "  TCKPort       TCK;\n"
                                  "  DataInPort    DI[7:0];\n"
                                  "  DataOutPort   DO[7:0]     {Source   SR; }\n"
                                  "  ScanInterface scan_client { Port SI; Port SO; Port SEL; }\n"
                                  "  ScanRegister  SR[7:0]\n"
                                  "  {\n"
                                  "    ScanInSource  SI;\n"
                                  "    CaptureSource DI;\n"
                                  "    ResetValue    8'b00000000;\n"
                                  "  }\n"
                                  "}\n"s;



} // End of unnamed namespace



//! Initializes tests (called for each test)
//!
void UT_Fragments::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped
}


//! Checks ICL_Reader::ParseExcerpt() when parsing only a ScanRegister
//!
void UT_Fragments::test_ParseExcerpt_ScanRegister ()
{
  // ---------------- Setup
  //
  auto excerpt = excerpt_Module_SReg_8_bits;

  auto           sm = make_shared<SystemModel>();
  ICL_Reader_TSS sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.ParseExcerpt(excerpt));

  // ---------------- Verify
  //
  TS_ASSERT_EMPTY (sut.ErrorMessage());
  TS_ASSERT_EMPTY (sut.PlaceHolders());

  CxxTest::setAbortTestOnFail(true);

  // With AST
  auto ast = sut.AST();
  TS_ASSERT_NOT_NULLPTR (ast);
  auto topModule = ast->TopModule();
  TS_ASSERT_NOT_NULLPTR (topModule);

  auto expected_AST_PrettyPrint = "Module SReg\n"
                                  "{\n"
                                  "  ScanRegister SR[7:0]\n"
                                  "  {\n"
                                  "  }\n"
                                  "}\n";
  auto actual_ASTPring          = Parsers::AST_PrettyPrinter::PrettyPrint(topModule);
  TS_ASSERT_EQUALS (actual_ASTPring, expected_AST_PrettyPrint);

  // Model
  auto parsedModel = sut.ParsedSystemModel();
  TS_ASSERT_NOT_NULLPTR (parsedModel);

  // With PrettyPrinter
  auto expected_PrettyPrint = "[Register](0)  \"test_register\", length: 8, bypass: 0000:0000";
  auto actual_PrettyPrint   = PrettyPrinter::PrettyPrint(parsedModel, PrettyPrinterOptions::Parser_debug);

  TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

  // With Checker
  test::PrependWithTap(sm, parsedModel);   // This is to avoid warnings about missing AccessInterface
  auto checkResult = sm->Check();
  TS_ASSERT_EMPTY (checkResult.InformativeReport());
}


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
    TS_ASSERT_THROWS (sut.Parse(filePath), ParserException);
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
