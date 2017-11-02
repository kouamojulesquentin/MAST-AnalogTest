//===========================================================================
//                           UT_ICL_Reader.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_ICL_Reader.cpp
//!
//! Implements test fixture for testing ICL_Reader
//!
//===========================================================================

#include "UT_ICL_Reader.hpp"

#include "ICL_Reader.hpp"
#include "ParserException.hpp"
#include "SystemModel.hpp"
#include "PrettyPrinter.hpp"
#include "AST.hpp"
#include "AST_Module.hpp"
#include "AST_PrettyPrinter.hpp"
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

using ICL::ICL_Reader;

namespace
{
//! Provides access to protected methods (different steps for parsing ICL)
//!
class ICL_Reader_TSS : public ICL::ICL_Reader
{
  public:
  ICL_Reader_TSS(std::shared_ptr<mast::SystemModel> sm) : ICL_Reader(sm) {  }
  using ICL_Reader::AST;
  using ICL_Reader::UpdateAstFromIcl;
};


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
void UT_ICL_Reader::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped
}


//! Checks ICL_Reader::ParseExcerpt() when parsing only a ScanRegister
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_ScanRegister ()
{
  // ---------------- Setup
  //
  std::istringstream excerpt(excerpt_Module_SReg_8_bits);

  auto           sm = make_shared<SystemModel>();
  ICL_Reader_TSS sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.UpdateAstFromIcl(excerpt));

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  auto ast = sut.AST();
  TS_ASSERT_NOT_NULLPTR (ast);
  auto topModule = ast->TopModule();
  TS_ASSERT_NOT_NULLPTR (topModule);

  auto expected_AST_PrettyPrint = "Module SReg\n"
                                  "{\n"
                                  "  ScanInPort SI;\n"
//+                                  "  ScanOutPort SO { Source SR[0];}\n"
                                  "  ScanRegister SR[7:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 8'b00000000;\n"
                                  "  }\n"
                                  "}\n";
  auto actual_AST_String          = Parsers::AST_PrettyPrinter::PrettyPrint(topModule);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}

//===========================================================================
// End of UT_ICL_Reader.cpp
//===========================================================================
