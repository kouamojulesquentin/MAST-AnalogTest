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
using std::istringstream;
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


//! Checks ICL_Reader::ParseExcerpt() when parsing a single ScanRegister
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_1_ScanRegister ()
{
  // ---------------- Setup
  //
  istringstream excerpt(excerpt_Module_SReg_8_bits);

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
                                  "  ScanOutPort SO\n"
                                  "  {\n"
                                  "    Source SR[0];\n"
                                  "  }\n"
                                  "  ScanRegister SR[7:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 8'b00000000;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(topModule);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when parsing a three ScanRegister
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_3_ScanRegister ()
{
  // ---------------- Setup
  //
  auto               sm  = make_shared<SystemModel>();
  std::istringstream excerpt("Module SReg {\n"
                             "ScanInPort    SI;\n"
                             "ScanOutPort   SO { Source  SR_3[0];}\n"
                             "ShiftEnPort   SE;\n"
                             "CaptureEnPort CE;\n"
                             "UpdateEnPort  UE;\n"
                             "SelectPort    SEL;\n"
                             "ResetPort     RST;\n"
                             "TCKPort       TCK;\n"
                             "DataInPort    DI[7:0];\n"
                             "DataOutPort   DO[7:0] {Source SR_1; }\n"
                             "ScanInterface scan_client { Port SI; Port SO; Port SEL; }\n"
                             "\n"
                             "ScanRegister SR_3[7:0] { ScanInSource SR_2[0];\n"
                             "                       CaptureSource DI;\n"
                             "                       ResetValue 8'b00000011; }\n"
                             "\n"
                             "ScanRegister SR_1[5:0] { ScanInSource SI;\n"
                             "                       CaptureSource DI;\n"
                             "                       ResetValue 8'b000001; }\n"
                             "\n"
                             "ScanRegister SR_2[6:0] { ScanInSource SR_1[0];\n"
                             "                       CaptureSource DI;\n"
                             "                       ResetValue 8'b0000010; }\n"
                             "}\n"s);

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
                                  "  ScanOutPort SO\n"
                                  "  {\n"
                                  "    Source SR_3[0];\n"
                                  "  }\n"
                                  "  ScanRegister SR_3[7:0]\n"
                                  "  {\n"
                                  "    ScanInSource SR_2[0];\n"
                                  "    ResetValue 8'b00000011;\n"
                                  "  }\n"
                                  "  ScanRegister SR_1[5:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 8'b000001;\n"
                                  "  }\n"
                                  "  ScanRegister SR_2[6:0]\n"
                                  "  {\n"
                                  "    ScanInSource SR_1[0];\n"
                                  "    ResetValue 8'b0000010;\n"
                                  "  }\n"
                                  "}\n"s;

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(topModule);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when parsing a with parameter defined with value (not string)
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_parameters_value ()
{
  // ---------------- Setup
  //
  istringstream excerpt(excerpt_Module_SReg_parameterized);

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
                                  "  Parameter MSB = 7;\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO\n"
                                  "  {\n"
                                  "    Source SR[0];\n"
                                  "  }\n"
                                  "  ScanRegister SR[$MSB:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(topModule);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when parsing a with parameter defined strings
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_parameters_strings ()
{
  // ---------------- Setup
  //
  istringstream excerpt("Module SReg\n"
                        "{\n"
                        "  Parameter      Param_1 = \"Hello\";\n"
                        "  Parameter      Param_2 = \"Hello\", \" World\";\n"
                        "  Parameter      Param_3 = \"Hello\", \" \\\"World\\\"\";\n"
                        "  LocalParameter Lp_1 = \"Hello\", \" World\";\n"
                        "  LocalParameter Lp_2 = \"Hello\", \" \\\\World\\\\\";\n"
                        "  ScanInPort     SI;\n"
                        "  ScanOutPort    SO { Source SR[0];}\n"
                        "  ScanRegister   SR[8:0]\n"
                        "  {\n"
                        "    ScanInSource  SI;\n"
                        "    ResetValue    'b0;\n"
                        "  }\n"
                        "}\n"s);

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
                                  "  Parameter Param_1 = \"Hello\";\n"
                                  "  Parameter Param_2 = \"Hello\", \" World\";\n"
                                  "  Parameter Param_3 = \"Hello\", \" \\\"World\\\"\";\n"
                                  "  LocalParameter Lp_1 = \"Hello\", \" World\";\n"
                                  "  LocalParameter Lp_2 = \"Hello\", \" \\\\World\\\\\";\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO\n"
                                  "  {\n"
                                  "    Source SR[0];\n"
                                  "  }\n"
                                  "  ScanRegister SR[8:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(topModule);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when parsing a with parameter defined with a parameter reference
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_parameters_param_ref ()
{
  // ---------------- Setup
  //
  istringstream excerpt("Module SReg\n"
                        "{\n"
                        "  Parameter      Param      = $Foo;\n"
                        "  LocalParameter LocalParam = $Bar;\n"
                        "  ScanInPort     SI;\n"
                        "  ScanOutPort    SO { Source SR[0];}\n"
                        "  ScanRegister   SR[8:0]\n"
                        "  {\n"
                        "    ScanInSource  SI;\n"
                        "    ResetValue    'b0;\n"
                        "  }\n"
                        "}\n"s);

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
                                  "  Parameter Param = $Foo;\n"
                                  "  LocalParameter LocalParam = $Bar;\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO\n"
                                  "  {\n"
                                  "    Source SR[0];\n"
                                  "  }\n"
                                  "  ScanRegister SR[8:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(topModule);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}





//! Checks ICL_Reader::ParseExcerpt() when parsing a with parameter defined with strings and parameter reference
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_parameters_string_and_param_ref ()
{
  // ---------------- Setup
  //
  istringstream excerpt("Module SReg\n"
                        "{\n"
                        "  Parameter      Param      = \"Hello\", \" world\",  $Foo;\n"
                        "  LocalParameter LocalParam = \"At the\", $Bar;\n"
                        "  ScanInPort     SI;\n"
                        "  ScanOutPort    SO { Source SR[0];}\n"
                        "  ScanRegister   SR[8:0]\n"
                        "  {\n"
                        "    ScanInSource  SI;\n"
                        "    ResetValue    'b0;\n"
                        "  }\n"
                        "}\n"s);

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
                                  "  Parameter Param = \"Hello\", \" world\", $Foo;\n"
                                  "  LocalParameter LocalParam = \"At the\", $Bar;\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO\n"
                                  "  {\n"
                                  "    Source SR[0];\n"
                                  "  }\n"
                                  "  ScanRegister SR[8:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(topModule);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when parsing a with parameter defined with parameter reference and strings (in that order)
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_parameters_param_ref_and_string ()
{
  // ---------------- Setup
  //
  istringstream excerpt("Module SReg\n"
                        "{\n"
                        "  Parameter      Param      = \"\", $Foo, \"Hello\", \" world\"  ;\n"     // @todo [JFC]-[November/06/2017]: Remove leading empty string workaround
                        "  LocalParameter LocalParam = \"\", $Bar, \"At the\";\n"                  // @todo [JFC]-[November/06/2017]: Remove leading empty string workaround
                        "  ScanInPort     SI;\n"
                        "  ScanOutPort    SO { Source SR[0];}\n"
                        "  ScanRegister   SR[8:0]\n"
                        "  {\n"
                        "    ScanInSource  SI;\n"
                        "    ResetValue    'b0;\n"
                        "  }\n"
                        "}\n"s);

                        //!
                        //!

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
                                  "  Parameter Param = $Foo, \"Hello\", \" world\";\n"
                                  "  LocalParameter LocalParam = $Bar, \"At the\";\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO\n"
                                  "  {\n"
                                  "    Source SR[0];\n"
                                  "  }\n"
                                  "  ScanRegister SR[8:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(topModule);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when parsing a with attribute defined with no value at all
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_attributes_no_value ()
{
  // ---------------- Setup
  //

  istringstream excerpt("Module SReg\n"
                        "{\n"
                        "  Attribute      Tested;\n"
                        "  ScanInPort     SI;\n"
                        "  ScanOutPort    SO { Source SR[0];}\n"
                        "  ScanRegister   SR[8:0]\n"
                        "  {\n"
                        "    ScanInSource  SI;\n"
                        "    ResetValue    'b0;\n"
                        "  }\n"
                        "}\n"s);

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
                                  "  Attribute Tested;\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO\n"
                                  "  {\n"
                                  "    Source SR[0];\n"
                                  "  }\n"
                                  "  ScanRegister SR[8:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(topModule);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when parsing a with attribute defined with value (not string)
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_attributes_value ()
{
  // ---------------- Setup
  //
  istringstream excerpt("Module SReg\n"
                        "{\n"
                        "  Attribute      Copyright = \"Pikus\";\n"
                        "  ScanInPort     SI;\n"
                        "  ScanOutPort    SO { Source SR[0];}\n"
                        "  ScanRegister   SR[8:0]\n"
                        "  {\n"
                        "    ScanInSource  SI;\n"
                        "    ResetValue    'b0;\n"
                        "  }\n"
                        "}\n"s);

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
                                  "  Attribute Copyright = \"Pikus\";\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO\n"
                                  "  {\n"
                                  "    Source SR[0];\n"
                                  "  }\n"
                                  "  ScanRegister SR[8:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(topModule);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when parsing a with attribute defined with strings
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_attributes_strings ()
{
  // ---------------- Setup
  //
  istringstream excerpt("Module SReg\n"
                        "{\n"
                        "  Attribute      Attrib_1 = \"Hello\";\n"
                        "  Attribute      Attrib_2 = \"Hello\", \" World\";\n"
                        "  Attribute      Attrib_3 = \"Hello\", \" \\\"World\\\"\";\n"
                        "  ScanInPort     SI;\n"
                        "  ScanOutPort    SO { Source SR[0];}\n"
                        "  ScanRegister   SR[8:0]\n"
                        "  {\n"
                        "    ScanInSource  SI;\n"
                        "    ResetValue    'b0;\n"
                        "  }\n"
                        "}\n"s);

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
                                  "  Attribute Attrib_1 = \"Hello\";\n"
                                  "  Attribute Attrib_2 = \"Hello\", \" World\";\n"
                                  "  Attribute Attrib_3 = \"Hello\", \" \\\"World\\\"\";\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO\n"
                                  "  {\n"
                                  "    Source SR[0];\n"
                                  "  }\n"
                                  "  ScanRegister SR[8:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(topModule);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}

//! Checks ICL_Reader::ParseExcerpt() when parsing a with attribute defined with a attribute reference
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_attributes_param_ref ()
{
  // ---------------- Setup
  //
  istringstream excerpt("Module SReg\n"
                        "{\n"
                        "  Attribute      Attrib      = $Foo;\n"
                        "  ScanInPort     SI;\n"
                        "  ScanOutPort    SO { Source SR[0];}\n"
                        "  ScanRegister   SR[8:0]\n"
                        "  {\n"
                        "    ScanInSource  SI;\n"
                        "    ResetValue    'b0;\n"
                        "  }\n"
                        "}\n"s);

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
                                  "  Attribute Attrib = $Foo;\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO\n"
                                  "  {\n"
                                  "    Source SR[0];\n"
                                  "  }\n"
                                  "  ScanRegister SR[8:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(topModule);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//===========================================================================
// End of UT_ICL_Reader.cpp
//===========================================================================
