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
  istringstream excerpt("Module SReg\n"
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
                        "  ScanRegister  SR[7:0]\n"
                        "  {\n"
                        "    ScanInSource  SI;\n"
                        "    CaptureSource DI;\n"
                        "    ResetValue    8'b00000000;\n"
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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO { Source SR[0]; }\n"
                                  "  ScanRegister SR[7:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 8'b00000000;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO { Source SR_3[0]; }\n"
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

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when parsing a with parameter defined with value (not string)
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_parameters_value ()
{
  // ---------------- Setup
  //
  istringstream excerpt("Module SReg\n"
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
                        "  ScanRegister  SR[$MSB:0]\n"
                        "  {\n"
                        "    ScanInSource  SI;\n"
                        "    CaptureSource DI;\n"
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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Parameter MSB = 7;\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO { Source SR[0]; }\n"
                                  "  ScanRegister SR[$MSB:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Parameter Param_1 = \"Hello\";\n"
                                  "  Parameter Param_2 = \"Hello\", \" World\";\n"
                                  "  Parameter Param_3 = \"Hello\", \" \\\"World\\\"\";\n"
                                  "  LocalParameter Lp_1 = \"Hello\", \" World\";\n"
                                  "  LocalParameter Lp_2 = \"Hello\", \" \\\\World\\\\\";\n"
                                  "  ScanRegister SR[8:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Parameter Param = $Foo;\n"
                                  "  LocalParameter LocalParam = $Bar;\n"
                                  "  ScanRegister SR[8:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Parameter Param = \"Hello\", \" world\", $Foo;\n"
                                  "  LocalParameter LocalParam = \"At the\", $Bar;\n"
                                  "  ScanRegister SR[8:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Parameter Param = $Foo, \"Hello\", \" world\";\n"
                                  "  LocalParameter LocalParam = $Bar, \"At the\";\n"
                                  "  ScanRegister SR[8:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Attribute Tested;\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO { Source SR[0]; }\n"
                                  "  ScanRegister SR[8:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
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
                        "  Attribute      Copyright = \"Picus\";\n"
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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Attribute Copyright = \"Picus\";\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO { Source SR[0]; }\n"
                                  "  ScanRegister SR[8:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Attribute Attrib_1 = \"Hello\";\n"
                                  "  Attribute Attrib_2 = \"Hello\", \" World\";\n"
                                  "  Attribute Attrib_3 = \"Hello\", \" \\\"World\\\"\";\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO { Source SR[0]; }\n"
                                  "  ScanRegister SR[8:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Attribute Attrib = $Foo;\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO { Source SR[0]; }\n"
                                  "  ScanRegister SR[8:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when parsing a ScanInterface statement (in a Module statement)
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_ScanInterface_InModuleDef ()
{
  // ---------------- Setup
  //
  istringstream excerpt("Module SReg\n"
                        "{\n"
                        "  ScanInterface scan_client { \n"
                        "  Attribute Foo = 15; "
                        "  Port SI; Port SO; Port SEL; }\n"
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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint = "NameSpace;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  ScanInterface scan_client\n"
                                  "  {\n"
                                  "    Attribute Foo = 15;\n"
                                  "    Port SI;\n"
                                  "    Port SO;\n"
                                  "    Port SEL;\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when parsing a SIB Scan mux
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_ScanMux_SIB ()
{
  // ---------------- Setup
  //
  istringstream excerpt("Module SIB_mux_pre\n"
                        "{\n"
                        "  ScanInPort   SI;\n"
                        "  ScanOutPort  SO   { Source SR; }\n"
                        "  ScanInPort   fromSO;\n"
                        "  ScanOutPort  toSI { Source SI; }\n"
                        "  ScanRegister SR\n"
                        "  {\n"
                        "    ScanInSource SIBmux;\n"
                        "    ResetValue   1'b0;\n"
                        "  }\n"
                        "\n"
                        "  ScanMux SIBmux SelectedBy SR\n"
                        "  {\n"
                        "    1'b0 : SI;\n"
                        "    1'b1 : fromSO;\n"
                        "  }\n"
                        "}"s);

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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module SIB_mux_pre\n"
                                  "{\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanInPort fromSO;\n"
                                  "  ScanOutPort SO { Source SR; }\n"
                                  "  ScanOutPort toSI { Source SI; }\n"
                                  "  ScanMux SIBmux SelectedBy SR\n"
                                  "  {\n"
                                  "    1'b0 : SI;\n"
                                  "    1'b1 : fromSO;\n"
                                  "  }\n"
                                  "  ScanRegister SR\n"
                                  "  {\n"
                                  "    ScanInSource SIBmux;\n"
                                  "    ResetValue 1'b0;\n"
                                  "  }\n"
                                  "}\n";


  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when parsing a Scan muxes defined with values list and vector identifiers
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_ScanMux_ValueList ()
{
  // ---------------- Setup
  //
  istringstream excerpt("Module mux\n"                                                        // 01
                        "{\n"                                                                 // 02
                        "  // implements SIB_out = (SIBREG) ? aux[0] : base[0]\n"             // 03
                        "  ScanMux SIB_out SelectedBy SIBREG {\n"                             // 04
                        "     1'b0 : base[0];\n"                                              // 05
                        "     1'b1 : aux[0];\n"                                               // 06
                        "  }\n"                                                               // 07
                        "  // implements compare_out = (check_mismatch) ? different : same\n" // 08
                        "  ScanMux compare_out SelectedBy check_mismatch[1:0] {\n"            // 09
                        "     1'b0,1'b1|1'b1,1'b0 :     sr.different;\n"                      // 10
                        "     1'b1, 1'b1 | 1'b0, 1'b0 : sr.same;\n"                           // 11
                        "  }\n"                                                               // 12
                        "}"s);                                                                // 13

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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module mux\n"
                                  "{\n"
                                  "  ScanMux SIB_out SelectedBy SIBREG\n"
                                  "  {\n"
                                  "    1'b0 : base[0];\n"
                                  "    1'b1 : aux[0];\n"
                                  "  }\n"
                                  "  ScanMux compare_out SelectedBy check_mismatch[1:0]\n"
                                  "  {\n"
                                  "    1'b0, 1'b1 | 1'b1, 1'b0 : sr.different;\n"
                                  "    1'b1, 1'b1 | 1'b0, 1'b0 : sr.same;\n"
                                  "  }\n"
                                  "}\n"sv;


  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when parsing a module instance
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_Instance ()
{
  // ---------------- Setup
  //
  istringstream excerpt("Module WrappedInstr\n"                                            // 01
                        "{\n"                                                              // 02
                        "  Instance I1 Of Instrument { InputPort DI = reg8.DO; }\n"        // 03
                        "  Instance reg8  Of  SReg {\n"                                    // 04
                        "  InputPort SI = SI; InputPort DI = I1.DO; Parameter Size = 8;\n" // 05
                        "  Attribute Copyright  =  \"Picus\";\n"                           // 06
                        "  }\n"                                                            // 07
                        "}"s);                                                             // 08

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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module WrappedInstr\n"
                                  "{\n"
                                  "  Instance I1 Of ::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg8.DO;\n"
                                  "  }\n"
                                  "  Instance reg8 Of ::SReg\n"
                                  "  {\n"
                                  "    Attribute Copyright = \"Picus\";\n"
                                  "    Parameter Size = 8;\n"
                                  "    InputPort SI = SI;\n"
                                  "    InputPort DI = I1.DO;\n"
                                  "  }\n"
                                  "}\n"sv;


  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when setting non "root" namespace
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_Namespace_Def ()
{
  // ---------------- Setup
  //
  istringstream excerpt(
                        "Module SReg\n"                                                     // 01
                        "{\n"                                                               // 02
                        "  Instance Inst_1 Of Instrument { InputPort DI = reg8; }\n"        // 03
                        "  Instance Inst_2 Of Picus::Instrument { InputPort DI = reg9; }\n" // 04
                        "}\n"                                                               // 05
                        "NameSpace Foo;\n"                                                  // 06
                        "Module WrappedInstr\n"                                             // 07
                        "{\n"                                                               // 08
                        "  Instance I1 Of Instrument { InputPort DI = reg8.DO; }\n"         // 09
                        "  Instance reg8  Of  ::SReg {\n"                                   // 10
                        "  InputPort SI = SI; \n"                                           // 11
                        "  }\n"                                                             // 12
                        "}"                                                                 // 13
                        "\n"                                                                // 14
                        "NameSpace Bar;\n"                                                  // 15
                        "Module SReg\n"                                                     // 16
                        "{\n"                                                               // 17
                        "  Instance I1 Of Instrument { InputPort DI = reg8.DO; }\n"         // 18
                        "}\n"                                                               // 19
                        "NameSpace Picus;\n"                                                // 20
                        "Module Reg\n"                                                      // 21
                        "{\n"                                                               // 22
                        "  Instance I1 Of Instrument { InputPort DI = reg8; }\n"            // 23
                        "  Instance I2 Of Foo::Instrument { InputPort DI = reg9; }\n"       // 24
                        "  Instance I3 Of Bar::Instrument { InputPort DI = reg9; }\n"       // 25
                        "}\n"                                                               // 26
                        "Module SReg\n"                                                     // 27
                        "{\n"                                                               // 28
                        "  Instance A Of Instrument { InputPort DI = reg8; }\n"             // 29
                        "  Instance B Of ::Instrument { InputPort DI = reg9; }\n"           // 30
                        "}\n"s                                                              // 31
                        );

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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Instance Inst_1 Of ::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg8;\n"
                                  "  }\n"
                                  "  Instance Inst_2 Of Picus::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg9;\n"
                                  "  }\n"
                                  "}\n"
                                  "NameSpace Bar;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Instance I1 Of Bar::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg8.DO;\n"
                                  "  }\n"
                                  "}\n"
                                  "NameSpace Foo;\n"
                                  "Module WrappedInstr\n"
                                  "{\n"
                                  "  Instance I1 Of Foo::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg8.DO;\n"
                                  "  }\n"
                                  "  Instance reg8 Of ::SReg\n"
                                  "  {\n"
                                  "    InputPort SI = SI;\n"
                                  "  }\n"
                                  "}\n"
                                  "NameSpace Picus;\n"
                                  "Module Reg\n"
                                  "{\n"
                                  "  Instance I1 Of Picus::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg8;\n"
                                  "  }\n"
                                  "  Instance I2 Of Foo::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg9;\n"
                                  "  }\n"
                                  "  Instance I3 Of Bar::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg9;\n"
                                  "  }\n"
                                  "}\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Instance A Of Picus::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg8;\n"
                                  "  }\n"
                                  "  Instance B Of ::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg9;\n"
                                  "  }\n"
                                  "}\n"s;

  const auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when explicitly declaring using some namespace (outside of Module definition)
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_UseNamespace_Def_OutsideModule ()
{
  // ---------------- Setup
  //
  istringstream excerpt(
                        "Module SReg\n"                                                     // 01
                        "{\n"                                                               // 02
                        "  Instance Inst_1 Of Instrument { InputPort DI = reg8; }\n"        // 03
                        "  Instance Inst_2 Of Picus::Instrument { InputPort DI = reg9; }\n" // 04
                        "}\n"                                                               // 05
                        "NameSpace Foo;\n"                                                  // 06
                        "UseNameSpace;\n"                                                   // 07
                        "Module WrappedInstr\n"                                             // 08
                        "{\n"                                                               // 09
                        "  Instance I1 Of Instrument { InputPort DI = reg8; }\n"            // 10
                        "  Instance I2 Of Bar::Instrument { InputPort DI = reg9; }\n"       // 11
                        "  Instance I3 Of Picus::Instrument { InputPort DI = reg10; }\n"    // 12
                        "}"                                                                 // 13
                        "\n"                                                                // 14
                        "NameSpace Bar;\n"                                                  // 15
                        "UseNameSpace Foo;\n"                                               // 16
                        "Module SReg\n"                                                     // 17
                        "{\n"                                                               // 18
                        "  Instance I1 Of Instrument { InputPort DI = reg8.DO; }\n"         // 19
                        "}\n"                                                               // 20
                        "NameSpace Picus;\n"                                                // 21
                        "Module Reg\n"                                                      // 22
                        "{\n"                                                               // 23
                        "  Instance I1 Of Instrument { InputPort DI = reg8; }\n"            // 24
                        "  Instance I2 Of Foo::Instrument { InputPort DI = reg9; }\n"       // 25
                        "  Instance I3 Of Bar::Instrument { InputPort DI = reg9; }\n"       // 26
                        "}\n"                                                               // 27
                        "UseNameSpace Bar;\n"                                               // 28
                        "Module SReg\n"                                                     // 29
                        "{\n"                                                               // 30
                        "  Instance A Of Instrument { InputPort DI = reg8; }\n"             // 31
                        "  Instance B Of ::Instrument { InputPort DI = reg9; }\n"           // 32
                        "}\n"s                                                              // 33
                        );

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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Instance Inst_1 Of ::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg8;\n"
                                  "  }\n"
                                  "  Instance Inst_2 Of Picus::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg9;\n"
                                  "  }\n"
                                  "}\n"
                                  "NameSpace Bar;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Instance I1 Of Foo::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg8.DO;\n"
                                  "  }\n"
                                  "}\n"
                                  "NameSpace Foo;\n"
                                  "Module WrappedInstr\n"
                                  "{\n"
                                  "  Instance I1 Of ::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg8;\n"
                                  "  }\n"
                                  "  Instance I2 Of Bar::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg9;\n"
                                  "  }\n"
                                  "  Instance I3 Of Picus::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg10;\n"
                                  "  }\n"
                                  "}\n"
                                  "NameSpace Picus;\n"
                                  "Module Reg\n"
                                  "{\n"
                                  "  Instance I1 Of Picus::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg8;\n"
                                  "  }\n"
                                  "  Instance I2 Of Foo::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg9;\n"
                                  "  }\n"
                                  "  Instance I3 Of Bar::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg9;\n"
                                  "  }\n"
                                  "}\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Instance A Of Bar::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg8;\n"
                                  "  }\n"
                                  "  Instance B Of ::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg9;\n"
                                  "  }\n"
                                  "}\n"s;

  const auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when explicitly declaring using some namespace (inside of Module definition)
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_UseNamespace_Def_InsideModule ()
{
  // ---------------- Setup
  //
  istringstream excerpt(
                        "Module SReg\n"                                                     // 01
                        "{\n"                                                               // 02
                        "  UseNameSpace Bar;\n"                                             // 03
                        "  Instance Inst_1 Of Instrument { InputPort DI = reg8; }\n"        // 04
                        "  Instance Inst_2 Of Picus::Instrument { InputPort DI = reg9; }\n" // 05
                        "}\n"                                                               // 06
                        "NameSpace Foo;\n"                                                  // 07
                        "Module WrappedInstr\n"                                             // 08
                        "{\n"                                                               // 09
                        "  Instance I1 Of Instrument { InputPort DI = reg8; }\n"            // 10
                        "  UseNameSpace;\n"                                                 // 11
                        "  Instance I2 Of Instrument { InputPort DI = reg9; }\n"            // 12
                        "  Instance I3 Of Instrument { InputPort DI = reg10; }\n"           // 13
                        "}"                                                                 // 14
                        "\n"                                                                // 15
                        "Module SReg\n"                                                     // 16
                        "{\n"                                                               // 17
                        "  Instance I1 Of Instrument { InputPort DI = reg8.DO; }\n"         // 18
                        "}\n"                                                               // 19
                        "NameSpace Picus;\n"                                                  // 07
                        "Module Reg\n"                                                      // 20
                        "{\n"                                                               // 21
                        "  Instance I1 Of Instrument { InputPort DI = reg8; }\n"            // 22
                        "  UseNameSpace Bar;\n"                                             // 23
                        "  Instance I2 Of Foo::Instrument { InputPort DI = reg9; }\n"       // 24
                        "  Instance I3 Of Instrument { InputPort DI = reg9; }\n"            // 25
                        "}\n"                                                               // 26
                        "Module SReg\n"                                                     // 27
                        "{\n"                                                               // 28
                        "  Instance A Of Instrument { InputPort DI = reg8; }\n"             // 29
                        "  Instance B Of ::Instrument { InputPort DI = reg9; }\n"           // 30
                        "}\n"s                                                              // 31
                        );

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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Instance Inst_1 Of Bar::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg8;\n"
                                  "  }\n"
                                  "  Instance Inst_2 Of Picus::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg9;\n"
                                  "  }\n"
                                  "}\n"
                                  "NameSpace Foo;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Instance I1 Of Foo::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg8.DO;\n"
                                  "  }\n"
                                  "}\n"
                                  "Module WrappedInstr\n"
                                  "{\n"
                                  "  Instance I1 Of Foo::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg8;\n"
                                  "  }\n"
                                  "  Instance I2 Of ::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg9;\n"
                                  "  }\n"
                                  "  Instance I3 Of ::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg10;\n"
                                  "  }\n"
                                  "}\n"
                                  "NameSpace Picus;\n"
                                  "Module Reg\n"
                                  "{\n"
                                  "  Instance I1 Of Picus::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg8;\n"
                                  "  }\n"
                                  "  Instance I2 Of Foo::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg9;\n"
                                  "  }\n"
                                  "  Instance I3 Of Bar::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg9;\n"
                                  "  }\n"
                                  "}\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Instance A Of Picus::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg8;\n"
                                  "  }\n"
                                  "  Instance B Of ::Instrument\n"
                                  "  {\n"
                                  "    InputPort DI = reg9;\n"
                                  "  }\n"
                                  "}\n"s;

  const auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when parsing a AccessLink statement
//!
void UT_ICL_Reader::test_UpdateAstFromIcl_AccessLink_1149_2001 ()
{
  // ---------------- Setup
  //
  istringstream excerpt("Module Top\n"
                        "{\n"
                        "  AccessLink dot1 Of STD_1149_1_2001 {\n"
                        "    BSDLEntity chip2542;\n"
                        "    ijtag_en { // Name of instruction\n"
                        "      ScanInterface { InstPath.MyScanInterface; } // defines the selected ScanInterface\n"
                        "    }\n"
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
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint = "NameSpace;\n"
                                  "Module Top\n"
                                  "{\n"
                                  "  AccessLink dot1 Of STD_1149_1_2001\n"
                                  "  {\n"
//+ Ignored !                                 "    BSDLEntity chip2542;\n"
//+                                  "    ijtag_en\n"
//+                                  "    {\n"
//+                                  "      ScanInterface { InstPath.MyScanInterface; }\n"
//+                                  "    }\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}

//===========================================================================
// End of UT_ICL_Reader.cpp
//===========================================================================
