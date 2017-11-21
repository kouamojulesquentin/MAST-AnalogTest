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
#include "AST.hpp"
#include "AST_Module.hpp"
#include "AST_PrettyPrinter.hpp"
#include "AST_Checker.hpp"

#include "ParserException.hpp"
#include "SystemModel.hpp"
#include "SystemModelBuilder.hpp"
#include "LoopbackAccessInterfaceProtocol.hpp"
#include "PrettyPrinter.hpp"

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
using namespace Parsers;

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
  using ICL_Reader::GenerateSystemModelNodes;
};

//! Add a JTAG Tap to system mode, then append node to it
//!
void PrependWithTap (shared_ptr<SystemModel> sm, shared_ptr<SystemModelNode> node)
{
  auto protocol = make_shared<LoopbackAccessInterfaceProtocol> ();
  SystemModelBuilder builder(*sm);

  auto tap = builder.Create_JTAG_TAP("TAP", 8u, 2u, protocol);

  sm->ReplaceRoot(tap, false);
  tap->AppendChild(node);
}
//
//  End of: PrependWithTap
//---------------------------------------------------------------------------
} // End of unnamed namespace

//! Initializes tests (called for each test)
//!
void UT_ICL_Reader::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier(); // Needed to check with pretty print that include node identifiers
}



//! Checks ICL_Reader::ParseExcerpt() when parsing a single ScanRegister
//!
void UT_ICL_Reader::test_FromIcl_1_ScanRegister ()
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
void UT_ICL_Reader::test_FromIcl_3_ScanRegister ()
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
void UT_ICL_Reader::test_FromIcl_parameters_value ()
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
void UT_ICL_Reader::test_FromIcl_parameters_strings ()
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
void UT_ICL_Reader::test_FromIcl_parameters_param_ref ()
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
void UT_ICL_Reader::test_FromIcl_parameters_string_and_param_ref ()
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
void UT_ICL_Reader::test_FromIcl_parameters_param_ref_and_string ()
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
void UT_ICL_Reader::test_FromIcl_attributes_no_value ()
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
void UT_ICL_Reader::test_FromIcl_attributes_value ()
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
void UT_ICL_Reader::test_FromIcl_attributes_strings ()
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
void UT_ICL_Reader::test_FromIcl_attributes_param_ref ()
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
void UT_ICL_Reader::test_FromIcl_ScanInterface_InModuleDef ()
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
void UT_ICL_Reader::test_FromIcl_ScanMux_SIB ()
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
void UT_ICL_Reader::test_FromIcl_ScanMux_ValueList ()
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
void UT_ICL_Reader::test_FromIcl_Instance ()
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
void UT_ICL_Reader::test_FromIcl_Namespace_Def ()
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
void UT_ICL_Reader::test_FromIcl_UseNamespace_Def_OutsideModule ()
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
void UT_ICL_Reader::test_FromIcl_UseNamespace_Def_InsideModule ()
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
void UT_ICL_Reader::test_FromIcl_AccessLink_1149_2001 ()
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
                                  "    BSDLEntity chip2542;\n"
//+ Ignored !
//+                                  "    ijtag_en\n"
//+                                  "    {\n"
//+                                  "      ScanInterface { InstPath.MyScanInterface; }\n"
//+                                  "    }\n"
                                  "  }\n"
                                  "}\n";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::GenerateSystemModelNodes() when parsing a single empty top module
//!
void UT_ICL_Reader::test_Generate_EmptyModule ()
{
  // ---------------- Setup
  //
  istringstream excerpt("Module Empty_top\n"
                        "{\n"
                        "  ScanInPort  SI;\n"
                        "  ScanOutPort SO { Source SI;}\n"
                        "}\n"s);


  auto           sm = make_shared<SystemModel>();
  ICL_Reader_TSS sut(sm);

  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_THROWS_NOTHING (sut.UpdateAstFromIcl(excerpt));
  auto ast = sut.AST();
  TS_ASSERT_NOT_NULLPTR (ast);

  auto checkResult = AST_Checker::Check(ast->Network());

  TS_ASSERT_FALSE (checkResult.HasIssues());

  // ---------------- Exercise
  //
  auto topNode = sut.GenerateSystemModelNodes(ast);

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (topNode);
  TS_ASSERT_EQUALS      (topNode->Name(), "Empty_top");

  // With PrettyPrinter
  auto actual_PrettyPrint   = PrettyPrinter::PrettyPrint(topNode, PrettyPrinterOptions::Parser_debug);
  auto expected_PrettyPrint = "[Chain](0)     \"Empty_top\"";

  TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);
}


//! Checks ICL_Reader::GenerateSystemModelNodes() when parsing a single ScanRegister in a single (top module)
//!
void UT_ICL_Reader::test_Generate_1_ScanRegister ()
{
  // ---------------- Setup
  //
  istringstream excerpt("Module SReg\n"
                        "{\n"
                        "  ScanInPort    SI;\n"
                        "  ScanOutPort   SO { Source SR[0];}\n"
                        "  ScanRegister  SR[7:0]\n"
                        "  {\n"
                        "    ScanInSource  SI;\n"
                        "    ResetValue    8'b00000000;\n"
                        "  }\n"
                        "}\n"s);


  auto           sm = make_shared<SystemModel>();
  ICL_Reader_TSS sut(sm);

  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_THROWS_NOTHING (sut.UpdateAstFromIcl(excerpt));
  auto ast = sut.AST();
  TS_ASSERT_NOT_NULLPTR (ast);

  auto checkResult = AST_Checker::Check(ast->Network());

  TS_ASSERT_FALSE (checkResult.HasIssues());

  // ---------------- Exercise
  //
  auto topNode = sut.GenerateSystemModelNodes(ast);

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (topNode);
  TS_ASSERT_EQUALS      (topNode->Name(), "SReg");

  // With PrettyPrinter
  auto actual_PrettyPrint   = PrettyPrinter::PrettyPrint(topNode, PrettyPrinterOptions::Parser_debug);
  auto expected_PrettyPrint = "[Chain](0)     \"SReg\"\n"
                              " [Register](1)  \"SR\", length: 8, bypass: 0000_0000";

  TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

  // With Checker
  PrependWithTap(sm, topNode);   // This is to avoid warnings about missing AccessInterface
  auto modelCheckResult = sm->Check();
  TS_ASSERT_EMPTY (modelCheckResult.InformativeReport());
}





//! Checks ICL_Reader::GenerateSystemModelNodes() when parsing N ScanRegisters in a single (top module)
//!
void UT_ICL_Reader::test_Generate_N_ScanRegisters ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto icl                 = std::get<0>(data);
    auto expectedPrettyPrint = std::get<1>(data);

    std::istringstream excerpt{string(icl.cbegin(), icl.cend())};

    auto           sm = make_shared<SystemModel>();
    ICL_Reader_TSS sut(sm);

    CxxTest::setAbortTestOnFail(true);
    TS_ASSERT_THROWS_NOTHING (sut.UpdateAstFromIcl(excerpt));
    auto ast = sut.AST();
    TS_ASSERT_NOT_NULLPTR (ast);

    auto checkResult = AST_Checker::Check(ast->Network());

    TS_ASSERT_FALSE (checkResult.HasIssues());

    // ---------------- Exercise
    //
    auto topNode = sut.GenerateSystemModelNodes(ast);

    // ---------------- Verify
    //
    TS_ASSERT_NOT_NULLPTR (topNode);

    // With PrettyPrinter
    auto actualPrettyPrint   = PrettyPrinter::PrettyPrint(topNode, PrettyPrinterOptions::Parser_debug);

    TS_ASSERT_EQUALS (actualPrettyPrint, expectedPrettyPrint);

    // With Checker
    PrependWithTap(sm, topNode);   // This is to avoid warnings about missing AccessInterface
    auto modelCheckResult = sm->Check();
    TS_ASSERT_EMPTY (modelCheckResult.InformativeReport());
  };

  using data_t = tuple<string_view, string_view>;
  auto data =
  {
    data_t  // 00
    {
      // ICL
      "Module Test_3x_Reg {\n"
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
      "                       ResetValue 6'b000001; }\n"
      "\n"
      "ScanRegister SR_2[6:0] { ScanInSource SR_1[0];\n"
      "                       CaptureSource DI;\n"
      "                       ResetValue 7'b0000010; }\n"
      "}\n"sv,
      // Pretty Print
      "[Chain](0)     \"Test_3x_Reg\"\n"
      " [Register](3)  \"SR_1\", length: 6, bypass: 0000_01\n"
      " [Register](2)  \"SR_2\", length: 7, bypass: 0000_010\n"
      " [Register](1)  \"SR_3\", length: 8, bypass: 0000_0011"sv
    },

    data_t  // 01
    {
      // ICL
      "Module Test_3x_Reg {\n"
      "ScanInPort    SI;\n"
      "ScanOutPort   SO { Source  SR_1[0];}\n"
      "\n"
      "ScanRegister SR_3[7:0] { ScanInSource SR_2[0];\n"
      "                       ResetValue 8'b00000011; }\n"
      "\n"
      "ScanRegister SR_1[5:0] { ScanInSource SR_3[0];\n"
      "                       ResetValue 6'b000001; }\n"
      "\n"
      "ScanRegister SR_2[6:0] { ScanInSource SI[0];\n"
      "                       CaptureSource DI;\n"
      "                       ResetValue 7'b0000010; }\n"
      "}\n"sv,
      // Pretty Print
      "[Chain](0)     \"Test_3x_Reg\"\n"
      " [Register](3)  \"SR_2\", length: 7, bypass: 0000_010\n"
      " [Register](2)  \"SR_3\", length: 8, bypass: 0000_0011\n"
      " [Register](1)  \"SR_1\", length: 6, bypass: 0000_01"sv
    },

    data_t  // 02
    {
      // ICL
      "Module Test_3x_Reg {\n"
      "ScanInPort    SI;\n"
      "ScanOutPort   SO { Source  SR_2[6];}\n"
      "\n"
      "ScanRegister SR_1[5:0] { ScanInSource SI;\n"
      "                       ResetValue 6'b000001; }\n"
      "\n"
      "ScanRegister SR_2[0:6] { ScanInSource SR_1[0];\n"
      "                       CaptureSource DI;\n"
      "                       ResetValue 7'b0000010; }\n"
      "}\n"sv,
      // Pretty Print
      "[Chain](0)     \"Test_3x_Reg\"\n"
      " [Register](2)  \"SR_1\", length: 6, bypass: 0000_01\n"
      " [Register](1)  \"SR_2\", length: 7, bypass: 0000_010"sv
    },

  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks ICL_Reader::GenerateSystemModelNodes() when parsing 2 modules with only ScanRegisters and no parameters
//!
void UT_ICL_Reader::test_Generate_2_Modules ()
{
  // ---------------- Setup
  //
  istringstream excerpt(
                        "Module top\n"                                  // 01
                        "{\n"                                           // 02
                        "  ScanInPort    SI;\n"                         // 03
                        "  ScanOutPort   SO { Source SR_2[0];}\n"       // 04
                        "  ScanRegister  SR_2[7:0]\n"                   // 05
                        "  {\n"                                         // 06
                        "    ScanInSource  inst_1.bso;\n"               // 07
                        "    ResetValue    8'b0001_0010;\n"             // 08
                        "  }\n"                                         // 09
                        "  ScanRegister  SR_1[4:0]\n"                   // 10
                        "  {\n"                                         // 11
                        "    ScanInSource  SI;\n"                       // 12
                        "    ResetValue    5'b1_0001;\n"                // 13
                        "  }\n"                                         // 14
                        "  Instance inst_1 Of bottom\n"                 // 15
                        "  {\n"                                         // 16
                        "    InputPort bsi = SR_1[0];\n"                // 17
                        "  }\n"                                         // 18
                        "}\n"                                           // 19
                        "\n"                                            // 20
                        "Module bottom\n"                               // 21
                        "{\n"                                           // 22
                        "  ScanInPort    bsi;\n"                        // 23
                        "  ScanOutPort   bso { Source reg_2[0];}\n"     // 24
                        "  ScanRegister  reg_1[7:0]\n"                  // 25
                        "  {\n"                                         // 26
                        "    ScanInSource  bsi;\n"                      // 27
                        "    ResetValue    8'b0010_0001;\n"             // 28
                        "  }\n"                                         // 29
                        "  ScanRegister  reg_2[6:0]\n"                  // 30
                        "  {\n"                                         // 31
                        "    ScanInSource  reg_1[0];\n"                 // 32
                        "    ResetValue    7'b010_0010;\n"              // 33
                        "  }\n"                                         // 34
                        "}\n"s);                                        // 35


  auto           sm = make_shared<SystemModel>();
  ICL_Reader_TSS sut(sm);

  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_THROWS_NOTHING (sut.UpdateAstFromIcl(excerpt));
  auto ast = sut.AST();
  TS_ASSERT_NOT_NULLPTR (ast);

  auto checkResult = AST_Checker::Check(ast->Network());

  TS_ASSERT_FALSE (checkResult.HasIssues());

  // ---------------- Exercise
  //
  auto topNode = sut.GenerateSystemModelNodes(ast);

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (topNode);
  TS_ASSERT_EQUALS      (topNode->Name(), "top");

  // With PrettyPrinter
  auto actual_PrettyPrint = PrettyPrinter::PrettyPrint(topNode, PrettyPrinterOptions::Parser_debug);
  auto expected_PrettyPrint = "[Chain](0)     \"top\"\n"
                              " [Register](5)  \"SR_1\", length: 5, bypass: 1000_1\n"
                              " [Chain](2)     \"inst_1\"\n"
                              "  [Register](4)  \"reg_1\", length: 8, bypass: 0010_0001\n"
                              "  [Register](3)  \"reg_2\", length: 7, bypass: 0100_010\n"
                              " [Register](1)  \"SR_2\", length: 8, bypass: 0001_0010"
                              ;

  TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

  // With Checker
  PrependWithTap(sm, topNode);   // This is to avoid warnings about missing AccessInterface
  auto modelCheckResult = sm->Check();
  TS_ASSERT_EMPTY (modelCheckResult.InformativeReport());
}



//! Checks ICL_Reader::GenerateSystemModelNodes() when parsing N modules with only ScanRegisters and no parameters
//!
void UT_ICL_Reader::test_Generate_N_Modules ()
{
  // ---------------- Setup
  //
  istringstream excerpt(
                        "Module top\n"                                  // 01
                        "{\n"                                           // 02
                        "  ScanInPort    SI;\n"                         // 03
                        "  ScanOutPort   SO { Source SR_2[0];}\n"       // 04
                        "  ScanRegister  SR_2[7:0]\n"                   // 05
                        "  {\n"                                         // 06
                        "    ScanInSource  inst_1.bso;\n"               // 07
                        "    ResetValue    8'b0001_0010;\n"             // 08
                        "  }\n"                                         // 09
                        "  ScanRegister  SR_1[4:0]\n"                   // 10
                        "  {\n"                                         // 11
                        "    ScanInSource  inst_2.midout;\n"            // 12
                        "    ResetValue    5'b1_0001;\n"                // 13
                        "  }\n"                                         // 14
                        "  Instance inst_1 Of bottom\n"                 // 15
                        "  {\n"                                         // 16
                        "    InputPort bsi = SR_1[0];\n"                // 17
                        "  }\n"                                         // 18
                        "  Instance inst_2 Of middle\n"                 // 19
                        "  {\n"                                         // 20
                        "    InputPort midin = SI;\n"                   // 21
                        "  }\n"                                         // 22
                        "}\n"                                           // 23
                        "\n"                                            // 24
                        "Module bottom\n"                               // 25
                        "{\n"                                           // 26
                        "  ScanInPort    bsi;\n"                        // 27
                        "  ScanOutPort   bso { Source reg_2[0];}\n"     // 28
                        "  ScanRegister  reg_1[7:0]\n"                  // 29
                        "  {\n"                                         // 30
                        "    ScanInSource  bsi;\n"                      // 31
                        "    ResetValue    8'b0010_0001;\n"             // 32
                        "  }\n"                                         // 33
                        "  ScanRegister  reg_2[6:0]\n"                  // 34
                        "  {\n"                                         // 35
                        "    ScanInSource  reg_1[0];\n"                 // 36
                        "    ResetValue    7'b010_0010;\n"              // 37
                        "  }\n"                                         // 38
                        "}\n"                                           // 39
                        "Module middle\n"                               // 40
                        "{\n"                                           // 41
                        "  ScanInPort    midin;\n"                      // 42
                        "  ScanOutPort   midout { Source mreg_2[0];}\n" // 43
                        "  Instance inst Of bottom\n"                   // 44
                        "  {\n"                                         // 45
                        "    InputPort bsi = mreg_1[0];\n"              // 46
                        "  }\n"                                         // 47
                        "  ScanRegister  mreg_1[7:0]\n"                 // 48
                        "  {\n"                                         // 49
                        "    ScanInSource  midin;\n"                    // 50
                        "    ResetValue    8'b0010_0001;\n"             // 51
                        "  }\n"                                         // 52
                        "  ScanRegister  mreg_2[6:0]\n"                 // 53
                        "  {\n"                                         // 54
                        "    ScanInSource  inst.bso;\n"                 // 55
                        "    ResetValue    7'b010_0010;\n"              // 56
                        "  }\n"                                         // 57
                        "}\n"s);                                        // 58


  auto           sm = make_shared<SystemModel>();
  ICL_Reader_TSS sut(sm);

  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_THROWS_NOTHING (sut.UpdateAstFromIcl(excerpt));
  auto ast = sut.AST();
  TS_ASSERT_NOT_NULLPTR (ast);

  auto checkResult = AST_Checker::Check(ast->Network());

  TS_ASSERT_FALSE (checkResult.HasIssues());

  // ---------------- Exercise
  //
  auto topNode = sut.GenerateSystemModelNodes(ast);

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (topNode);
  TS_ASSERT_EQUALS      (topNode->Name(), "top");

  // With PrettyPrinter
  auto actual_PrettyPrint = PrettyPrinter::PrettyPrint(topNode, PrettyPrinterOptions::Parser_debug);
  auto expected_PrettyPrint = "[Chain](0)     \"top\"\n"
                              " [Chain](6)     \"inst_2\"\n"
                              "  [Register](11) \"mreg_1\", length: 8, bypass: 0010_0001\n"
                              "  [Chain](8)     \"inst\"\n"
                              "   [Register](10) \"reg_1\", length: 8, bypass: 0010_0001\n"
                              "   [Register](9)  \"reg_2\", length: 7, bypass: 0100_010\n"
                              "  [Register](7)  \"mreg_2\", length: 7, bypass: 0100_010\n"
                              " [Register](5)  \"SR_1\", length: 5, bypass: 1000_1\n"
                              " [Chain](2)     \"inst_1\"\n"
                              "  [Register](4)  \"reg_1\", length: 8, bypass: 0010_0001\n"
                              "  [Register](3)  \"reg_2\", length: 7, bypass: 0100_010\n"
                              " [Register](1)  \"SR_2\", length: 8, bypass: 0001_0010"
                              ;

  TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

  // With Checker
  PrependWithTap(sm, topNode);   // This is to avoid warnings about missing AccessInterface
  auto modelCheckResult = sm->Check();
  TS_ASSERT_EMPTY (modelCheckResult.InformativeReport());
}


//! Checks ICL_Reader::GenerateSystemModelNodes() when parsing a SIB with the mux before the scan register bit
//!
//+void UT_ICL_Reader::test_Generate_SIB_mux_pre ()
//+{
//+  // ---------------- Setup
//+  //
//+  istringstream excerpt(
//+                        "Module SIB_mux_pre {\n"
//+                        "  ScanInPort     SI;\n"
//+                        "  CaptureEnPort  CE;\n"
//+                        "  ShiftEnPort    SE;\n"
//+                        "  UpdateEnPort   UE;\n"
//+                        "  SelectPort     SEL;\n"
//+                        "  ResetPort      RST;\n"
//+                        "  TCKPort        TCK;\n"
//+                        "  ScanOutPort    SO { Source SR; }\n"
//+                        "  ScanInterface client {\n"
//+                        "    Port SI; Port CE; Port SE; Port UE;\n"
//+                        "    Port SEL; Port RST; Port TCK; Port SO;\n"
//+                        "  }\n"
//+                        "  ScanInPort       fromSO;\n"
//+                        "  ToCaptureEnPort  toCE;\n"
//+                        "  ToShiftEnPort    toSE;\n"
//+                        "  ToUpdateEnPort   toUE;\n"
//+                        "  ToSelectPort     toSEL;\n"
//+                        "  ToResetPort      toRST;\n"
//+                        "  ToTCKPort        toTCK;\n"
//+                        "  ScanOutPort      toSI { Source SI; }\n"
//+                        "  ScanInterface host {\n"
//+                        "    Port fromSO; Port toCE; Port toSE; Port toUE;\n"
//+                        "    Port toSEL; Port toRST; Port toTCK; Port toSI;\n"
//+                        "  }\n"
//+                        "  ScanRegister SR {\n"
//+                        "    ScanInSource SIBmux; CaptureSource SR; ResetValue 1'b0;\n"
//+                        "  }\n"
//+                        "  ScanMux SIBmux SelectedBy SR {\n"
//+                        "    1'b0 : SI;\n"
//+                        "    1'b1 : fromSO;\n"
//+                        "  }\n"
//+                        "}\n"s);


//+  auto           sm = make_shared<SystemModel>();
//+  ICL_Reader_TSS sut(sm);

//+  CxxTest::setAbortTestOnFail(true);
//+  TS_ASSERT_THROWS_NOTHING (sut.UpdateAstFromIcl(excerpt));
//+  auto ast = sut.AST();
//+  TS_ASSERT_NOT_NULLPTR (ast);

//+  auto checkResult = AST_Checker::Check(ast->Network());

//+  TS_ASSERT_FALSE (checkResult.HasIssues());

//+  // ---------------- Exercise
//+  //
//+  auto topNode = sut.GenerateSystemModelNodes(ast);

//+  // ---------------- Verify
//+  //
//+  TS_ASSERT_NOT_NULLPTR (topNode);
//+  TS_ASSERT_EQUALS      (topNode->Name(), "SIB_mux_pre");

//+  // With PrettyPrinter
//+  auto actual_PrettyPrint   = PrettyPrinter::PrettyPrint(topNode, PrettyPrinterOptions::Parser_debug);
//+  auto expected_PrettyPrint = "[Chain](0)     \"SIB_mux_pre\"\n"
//+                              " [Linker](2)    \"SIBmux\"\n"
//+                              " :Selector:(1)  \"SR\", kind: Table_Based, can_select_none: 1, inverted_bits: 0, reversed_order: 0\n"
//+                              " [Register](1)  \"SR\", length: 1, bypass: 0";

//+  TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

//+  // With Checker
//+  PrependWithTap(sm, topNode);   // This is to avoid warnings about missing AccessInterface
//+  auto modelCheckResult = sm->Check();
//+  TS_ASSERT_EMPTY (modelCheckResult.InformativeReport());
//+}



//! Checks ICL_Reader::GenerateSystemModelNodes() when parsing a module with a single SIB and 3 wrapped instrument instances
//! @note ICL has been cleaned up to ease understanding (some useless stuffs have been removed)
//+void UT_ICL_Reader::test_Generate_Single_SIB_3WI ()
//+{
//+  // ---------------- Setup
//+  //
//+  istringstream excerpt(
//+                        "Module Single_SIB_3WI {\n"                                          // 01
//+                        "  ScanInPort SI;\n"                                                 // 02
//+                        "  ScanOutPort SO { Source SIB1.SO; }\n"                             // 03
//+                        "\n"                                                                 // 04
//+                        "  Instance SIB1 Of SIB_mux_pre { InputPort SI = SI;\n"              // 05
//+                        "                                 InputPort fromSO = WI3.SO; }\n"    // 06
//+                        "  Instance WI1 Of WrappedInstr { InputPort SI = SIB1.toSI; }\n"     // 07
//+                        "  Instance WI2 Of WrappedInstr { InputPort SI = WI1.SO; }\n"        // 08
//+                        "  Instance WI3 Of WrappedInstr { InputPort SI = WI2.SO; }\n"        // 09
//+                        "}\n"                                                                // 10
//+                        "\n"                                                                 // 11
//+                        "Module WrappedInstr {\n"                                            // 12
//+                        "  ScanInPort    SI;\n"                                              // 13
//+                        "  ScanOutPort   SO { Source reg8.SO;}\n"                            // 14
//+                        "  ScanInterface scan_client { Port SI; Port SO; }\n"                // 15
//+                        "\n"                                                                 // 16
//+                        "  Instance I1   Of Instrument { InputPort DI = reg8.DO; }\n"        // 17
//+                        "  Instance reg8 Of SReg       { InputPort SI = SI;      }\n"        // 18
//+                        "}\n"                                                                // 19
//+                        "\n"                                                                 // 20
//+                        "Module SReg\n"                                                      // 21
//+                        "{\n"                                                                // 22
//+                        "  ScanInPort    SI;\n"                                              // 23
//+                        "  ScanOutPort   SO          { Source  SR[0];}\n"                    // 24
//+                        "  ScanInterface scan_client { Port SI; Port SO; }\n"                // 25
//+                        "  ScanRegister SR[7:0]      { ScanInSource SI; ResetValue 'b0; }\n" // 26
//+                        "}\n"                                                                // 27
//+                        "\n"                                                                 // 28
//+                        "Module Instrument\n"                                                // 29
//+                        "{\n"                                                                // 30
//+                        "  DataInPort DI[7:0];\n"                                            // 31
//+                        "  DataOutPort DO[7:0];\n"                                           // 32
//+                        "}\n"                                                                // 33
//+                        "\n"                                                                 // 34
//+                        "Module SIB_mux_pre {\n"                                             // 35
//+                        "  ScanInPort     SI;\n"                                             // 36
//+                        "  ScanOutPort    SO { Source SR; }\n"                               // 37
//+                        "  ScanInterface client {\n"                                         // 38
//+                        "    Port SI;\n"                                                     // 39
//+                        "    Port SO;\n"                                                     // 40
//+                        "  }\n"                                                              // 41
//+                        "  ScanInPort       fromSO;\n"                                       // 42
//+                        "  ScanOutPort      toSI { Source SI; }\n"                           // 43
//+                        "  ScanInterface host {\n"                                           // 44
//+                        "    Port fromSO; \n"                                                // 45
//+                        "    Port toSI;\n"                                                   // 46
//+                        "  }\n"                                                              // 47
//+                        "  ScanRegister SR {\n"                                              // 48
//+                        "    ScanInSource SIBmux; CaptureSource SR; ResetValue 1'b0;\n"      // 49
//+                        "  }\n"                                                              // 50
//+                        "  ScanMux SIBmux SelectedBy SR {\n"                                 // 51
//+                        "    1'b0 : SI;\n"                                                   // 52
//+                        "    1'b1 : fromSO;\n"                                               // 53
//+                        "  }\n"                                                              // 54
//+                        "}\n"s);                                                             // 55


//+  auto           sm = make_shared<SystemModel>();
//+  ICL_Reader_TSS sut(sm);

//+  CxxTest::setAbortTestOnFail(true);
//+  TS_ASSERT_THROWS_NOTHING (sut.UpdateAstFromIcl(excerpt));
//+  auto ast = sut.AST();
//+  TS_ASSERT_NOT_NULLPTR (ast);

//+  auto checkResult = AST_Checker::Check(ast->Network());

//+  TS_ASSERT_EMPTY (checkResult.IssuesReport());

//+  // ---------------- Exercise
//+  //
//+  auto topNode = sut.GenerateSystemModelNodes(ast);

//+  // ---------------- Verify
//+  //
//+  TS_ASSERT_NOT_NULLPTR (topNode);
//+  TS_ASSERT_EQUALS      (topNode->Name(), "Single_SIB_3WI");

//+  // With PrettyPrinter
//+  auto actual_PrettyPrint   = PrettyPrinter::PrettyPrint(topNode, PrettyPrinterOptions::Parser_debug_no_id);
//+  auto expected_PrettyPrint = "[Chain]     \"Single_SIB_3WI\"\n"
//+                              " [Chain]     \"SIB1\"\n"
//+                              "  [Linker]    \"SIBmux\"\n"
//+                              "  :Selector:  \"SR\", kind: Table_Based, can_select_none: 1, inverted_bits: 0, reversed_order: 0\n"
//+                              "   [Chain]     \"generated_1\", ignore_in_path: true\n"
//+                              "    [Chain]     \"WI1\"\n"
//+                              "     [Chain]     \"reg8\"\n"
//+                              "      [Register]  \"SR\", length: 1, bypass: 0\n"
//+                              "    [Chain]     \"WI2\"\n"
//+                              "     [Chain]     \"reg8\"\n"
//+                              "      [Register]  \"SR\", length: 1, bypass: 0\n"
//+                              "    [Chain]     \"WI3\"\n"
//+                              "     [Chain]     \"reg8\"\n"
//+                              "      [Register]  \"SR\", length: 1, bypass: 0\n"
//+                              "  [Register]  \"SR\", length: 1, bypass: 0";

//+  TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

//+  // With Checker
//+  PrependWithTap(sm, topNode);   // This is to avoid warnings about missing AccessInterface
//+  auto modelCheckResult = sm->Check();
//+  TS_ASSERT_EMPTY (modelCheckResult.InformativeReport());
//+}





//===========================================================================
// End of UT_ICL_Reader.cpp
//===========================================================================
