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
#include <vector>
#include <memory>
#include <fstream>

using std::vector;
using std::tuple;
using std::make_tuple;
using std::string;
using std::stringstream;
using std::istringstream;
using std::experimental::string_view;
using std::shared_ptr;
using std::dynamic_pointer_cast;
using std::make_shared;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;
using namespace mast;

using ICL::ICL_Reader;
using namespace Parsers;

namespace
{

//! Returns path of an ICL directory accessible by test runner
//!
//! @param dirName  Directory name
//!
string GetTestDirPath (string_view dirName)
{
  return test::GetTestDirPath({"Readers"s, "UT_TestFiles"s, "ICL"s, string(dirName)});
}

//! Returns path of an ICL file accessible by test runner
//!
//! @param fileName   File name (with extension)
//!
string GetTestFilePath (string_view fileName)
{
  return test::GetTestFilePath({"Readers"s, "UT_TestFiles"s, "ICL"s, string(fileName)});
}


//! Returns path of an ICL file accessible by test runner
//!
//! @param dirName    Directory name where file should stand
//! @param fileName   File name (with extension)
//!
string GetTestFilePath (string_view dirName, string_view fileName)
{
  return test::GetTestFilePath({"Readers"s, "UT_TestFiles"s, "ICL"s, string(dirName), string(fileName)});
}




//! Returns path of a file for expected AST pretty print
//!
//! @param fileName   File name (with extension)
//!
string GetExpectedAstPrintFilePath (string_view fileName)
{
  return GetTestFilePath("Expected_AstPrint"sv, fileName);
}



//! Gets content of expected AST pretty print from file
//!
//! @param fileName   File name (with extension) that contains expected AST pretty print
//!
string GetExpectedAstPrint (string_view fileName)
{
  auto expectedFilePath = GetExpectedAstPrintFilePath(fileName);
  return test::GetTextFileContent(expectedFilePath);
}


//! Gets content of expected AST pretty print from file
//!
//! @param fileName   File name (with extension) that contains expected AST pretty print
//!
string GetExpectedModelPrettyPrint (string_view fileName)
{
  auto expectedFilePath = GetTestFilePath("Expected_ModelPrint", fileName);
  return test::GetTextFileContent(expectedFilePath);
}


//! Provides access to protected methods (different steps for parsing ICL)
//!
class ICL_Reader_TSS : public ICL::ICL_Reader
{
  public:
  ICL_Reader_TSS(std::shared_ptr<mast::SystemModel> sm) : ICL_Reader(sm) {  }
  using ICL_Reader::AST;
  using ICL_Reader::UpdateAstFromIcl;
  using ICL_Reader::UpdateAstFromFiles;
  using ICL_Reader::UniquifyAST;
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
                                  "  DataInPort DI[7:0];\n"
                                  "  DataOutPort DO[7:0] { Source SR; }\n"
                                  "  ScanRegister SR[7:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    CaptureSource DI;\n"
                                  "    ResetValue 8'b00000000;\n"
                                  "  }\n"
                                  "}";

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
                                  "  DataInPort DI[7:0];\n"
                                  "  DataOutPort DO[7:0] { Source SR_1; }\n"
                                  "  ScanRegister SR_3[7:0]\n"
                                  "  {\n"
                                  "    ScanInSource SR_2[0];\n"
                                  "    CaptureSource DI;\n"
                                  "    ResetValue 8'b00000011;\n"
                                  "  }\n"
                                  "  ScanRegister SR_1[5:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    CaptureSource DI;\n"
                                  "    ResetValue 8'b000001;\n"
                                  "  }\n"
                                  "  ScanRegister SR_2[6:0]\n"
                                  "  {\n"
                                  "    ScanInSource SR_1[0];\n"
                                  "    CaptureSource DI;\n"
                                  "    ResetValue 8'b0000010;\n"
                                  "  }\n"
                                  "}"s;

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when parsing a module with parameter defined with value (not string)
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
                                  "  DataInPort DI[$MSB:0];\n"
                                  "  DataOutPort DO[$MSB:0] { Source SR; }\n"
                                  "  ScanRegister SR[$MSB:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    CaptureSource DI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}";

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
                                  "}";

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
                                  "}";

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
                                  "}";

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
                                  "}";

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
                                  "}";

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
                                  "}";

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
                                  "}";

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
                                  "}";

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
                                  "}";

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
                                  "}";


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
                                  "}"sv;


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
                                  "}"sv;


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
                        "}"s                                                              // 31
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
                                  "}"s;

  const auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when parsing a module with value expressions
//!
void UT_ICL_Reader::test_FromIcl_ValueExpr ()
{
  // ---------------- Setup
  //
  istringstream excerpt("Module SReg\n"                                                            // 01
                        "{\n"                                                                      // 02
                        "  Parameter Size          = 8;\n"                                         // 03
                        "  Parameter LSB           = 0;\n"                                         // 04
                        "  Parameter MSB           = $Size - 1;\n"                                 // 05
                        "  Parameter MSB_2         = $Size + 1;\n"                                 // 06
                        "  Parameter Val_1         = 3;\n"                                         // 07
                        "  Parameter Val_2         = 0 -$Val_1 + 4 * (9 - 5) + 7;\n"               // 08
                        "  Parameter Val_3         = $Val_1 + $Size * ($MSB_2 - $MSB) + $Val_2;\n" // 09
                        "  Parameter Val_4         = 1_3_4;\n"                                     // 10 - a valid decimal number (134)
                        "  Parameter Val_5         = 0235;\n"                                      // 11 - a valid decimal number
                        "  Parameter Val_6         = 'd 134;\n"                                    // 12 - a valid decimal number
                        "  Parameter Val_7         = 'h 2_3ff;\n"                                  // 13 - a valid hexadecimal number
                        "  Parameter Val_8         = 4'b0101;\n"                                   // 14 - a 4-bit binary number
                        "  Parameter Val_9         = 5'D 3;\n"                                     // 15 - a 5-bit decimal number
                        "  Parameter Val_10        = 3'b0_1x;\n"                                   // 16 - a 3-bit number with the lsb an unknown number
                        "  Parameter Val_11        = 7'hx;\n"                                      // 17 - a 7-bit unknown number (same as 7'bxxxxxxx)
                        "  Parameter Val_22        = $Size'h3;\n"                                  // 18 - Use of a parameter to size a number (which will be 2'b11)
                        "  Parameter Vector_Val_1  = 5'b10110, 4'hFade;\n"                         // 19
                        "  Parameter Vector_Val_2  = 5'b10110, ~0, 4'hFade;\n"                     // 20
                        "  Parameter Vector_Val_3  = ~4'b1000;\n"                                  // 21 - means 4'b0111
                        "  Parameter Vector_Val_4  = ~8'hAA;\n"                                    // 22 - means 8'h55
                        "  Parameter Vector_Val_5  = ~42;\n"                                       // 23 - means ~'d42 = ~'b101010 = ‘b010101
                        "  Parameter Vector_Val_6  = ~$ABC;\n"                                     // 24 - the value of ABC will be bitwise inverted before being used
                        "\n"                                                                       // 25
                        "  ScanInPort    SI;\n"                                                    // 26
                        "  ScanOutPort   SO { Source SR[0];}\n"                                    // 27
                        "  ScanRegister  SR_1[$MSB + 32 : 3/$Val_1]\n"                             // 28
                        "  {\n"                                                                    // 29
                        "    ScanInSource  SI;\n"                                                  // 30
                        "    ResetValue    'b0, $Vector_Val_1;\n"                                  // 31
                        "  }\n"                                                                    // 32
                        "\n"                                                                       // 33
                        "  ScanRegister  SR_2[$MSB / 2 : $LSB + 1]\n"                              // 34
                        "  {\n"                                                                    // 35
                        "    ScanInSource  SI;\n"                                                  // 36
                        "    ResetValue    8'h21, 3'b011;\n"                                       // 37
                        "  }\n"                                                                    // 38
                        "\n"                                                                       // 39
                        "  ScanMux compare_out SelectedBy check_mismatch[1:0]\n"                   // 40
                        "  {\n"                                                                    // 41
                        "    1'b0,1'b1 | 1'b1,1'b0 : different;\n"                                 // 42
                        "    1'b1,1'b1 | 1'b0,1'b0 : same;\n"                                      // 43
                        "  }\n"                                                                    // 44
                        "}\n"s);                                                                   // 45



  auto           sm = make_shared<SystemModel>();
  ICL_Reader_TSS sut(sm);

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.UpdateAstFromIcl(excerpt));

  // ---------------- Verify
  //

  auto ast = sut.AST();
  TS_ASSERT_NOT_NULLPTR (ast);
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  Parameter Size = 8;\n"
                                  "  Parameter LSB = 0;\n"
                                  "  Parameter MSB = $Size - 1;\n"
                                  "  Parameter MSB_2 = $Size + 1;\n"
                                  "  Parameter Val_1 = 3;\n"
                                  "  Parameter Val_2 = 0 - $Val_1 + 4 * (9 - 5) + 7;\n"
                                  "  Parameter Val_3 = $Val_1 + $Size * ($MSB_2 - $MSB) + $Val_2;\n"
                                  "  Parameter Val_4 = 134;\n"  // Separator removed
                                  "  Parameter Val_5 = 235;\n"  // Leading (useless) leading zero removed
                                  "  Parameter Val_6 = 'd 134;\n"
                                  "  Parameter Val_7 = 'h 2_3ff;\n"
                                  "  Parameter Val_8 = 4'b0101;\n"
                                  "  Parameter Val_9 = 5'D 3;\n"  // Space removed
                                  "  Parameter Val_10 = 3'b0_1x;\n"
                                  "  Parameter Val_11 = 7'hx;\n"
                                  "  Parameter Val_22 = $Size'h3;\n"
                                  "  Parameter Vector_Val_1 = 5'b10110, 4'hFade;\n"
                                  "  Parameter Vector_Val_2 = 5'b10110, ~0, 4'hFade;\n"
                                  "  Parameter Vector_Val_3 = ~4'b1000;\n"
                                  "  Parameter Vector_Val_4 = ~8'hAA;\n"
                                  "  Parameter Vector_Val_5 = ~42;\n"
                                  "  Parameter Vector_Val_6 = ~$ABC;\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO { Source SR[0]; }\n"
                                  "  ScanMux compare_out SelectedBy check_mismatch[1:0]\n"
                                  "  {\n"
                                  "    1'b0, 1'b1 | 1'b1, 1'b0 : different;\n"
                                  "    1'b1, 1'b1 | 1'b0, 1'b0 : same;\n"
                                  "  }\n"
                                  "  ScanRegister SR_1[$MSB + 32:3 / $Val_1]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0, $Vector_Val_1;\n"
                                  "  }\n"
                                  "  ScanRegister SR_2[$MSB / 2:$LSB + 1]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 8'h21, 3'b011;\n"
                                  "  }\n"
                                  "}";


  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
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
                        "}"s                                                              // 33
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
                                  "}"s;

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
                        "}"s                                                              // 31
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
                                  "}"s;

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
                                  "    ijtag_en\n"
                                  "    {\n"
                                  "      ScanInterface { InstPath.MyScanInterface; }\n"
                                  "    }\n"
                                  "  }\n"
                                  "}";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when parsing aliases statement
//!
void UT_ICL_Reader::test_FromIcl_Alias ()
{
  // ---------------- Setup
  //
  istringstream excerpt(
                        "Module Top\n"                                                                 // 01
                        "{\n"                                                                          // 02
                        "  Parameter size = 8;\n"                                                      // 03
                        "\n"                                                                           // 04
                        "  Alias Simple[3:1]  = a, b, c;\n"                                            // 05 ==> a, b & c are supposed to be 1 bit each
                        "  Alias MyName1[2:0] = a[7:5];\n"                                             // 06
                        "  Alias MyName2[2:0] = u1.a[2], u2.u3.b[1:0];\n"                              // 07
                        "  Alias enable       = DI[$size-1];\n"                                        // 08
                        "  Alias RE           = TDR1[63] ;\n"                                          // 09
                        "  Alias CNTL[42:0]   = ADDR[7:0], DATA[31:0], CMD[2:0];\n"                    // 10
                        "  Alias Adr[1:0]     = a[1], C { RefEnum T1; } \n"                            // 11
                        "  Alias RE_2         = TDR1[63] { iApplyEndState 1'b0;}\n"                    // 12
                        "  Alias CNTL_2[42:0] = ADDR[7:0], DATA[31:0], CMD[2:0] { AccessTogether; }\n" // 13
                        "  Alias MyName3[2:0] = u1.a[2], u2.u3.b[1:0]\n"                               // 14
                        "  {\n"                                                                        // 15
                        "     AccessTogether;   \n"                                                    // 16
                        "     iApplyEndState    3'b110; \n"                                            // 17
                        "     RefEnum           T1; \n"                                                // 18
                        "     Attribute Cie   = \"Picus Logica\"; \n"                                  // 19
                        "   }\n"                                                                       // 20
                        "}\n"s);                                                                       // 21

  auto           sm = make_shared<SystemModel>();
  ICL_Reader_TSS sut(sm);

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.UpdateAstFromIcl(excerpt));

  // ---------------- Verify
  //
  auto ast = sut.AST();
  TS_ASSERT_NOT_NULLPTR (ast);
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint = "NameSpace;\n"
                                  "Module Top\n"
                                  "{\n"
                                  "  Parameter size = 8;\n"
                                  "  Alias Simple[3:1] = a, b, c;\n"
                                  "  Alias MyName1[2:0] = a[7:5];\n"
                                  "  Alias MyName2[2:0] = u1.a[2], u2.u3.b[1:0];\n"
                                  "  Alias enable = DI[$size - 1];\n"
                                  "  Alias RE = TDR1[63];\n"
                                  "  Alias CNTL[42:0] = ADDR[7:0], DATA[31:0], CMD[2:0];\n"
                                  "  Alias Adr[1:0] = a[1], C\n"
                                  "  {\n"
                                  "    RefEnum T1;\n"
                                  "  }\n"
                                  "  Alias RE_2 = TDR1[63]\n"
                                  "  {\n"
                                  "    iApplyEndState 1'b0;\n"
                                  "  }\n"
                                  "  Alias CNTL_2[42:0] = ADDR[7:0], DATA[31:0], CMD[2:0]\n"
                                  "  {\n"
                                  "    AccessTogether;\n"
                                  "  }\n"
                                  "  Alias MyName3[2:0] = u1.a[2], u2.u3.b[1:0]\n"
                                  "  {\n"
                                  "    Attribute Cie = \"Picus Logica\";\n"
                                  "    RefEnum T1;\n"
                                  "    AccessTogether;\n"
                                  "    iApplyEndState 3'b110;\n"
                                  "  }\n"
                                  "}";

  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks ICL_Reader::ParseExcerpt() when parsing aliases statement with errors
//!
void UT_ICL_Reader::test_FromIcl_Alias_errors ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& aliasWithError)
  {
    // ---------------- Setup
    //
    stringstream excerpt;
    excerpt << "Module Top\n{\n";
    excerpt << aliasWithError;
    excerpt << "}\n";

    auto           sm = make_shared<SystemModel>();
    ICL_Reader_TSS sut(sm);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut.UpdateAstFromIcl(excerpt), std::exception);
  };

  auto data =
  {
    "  Alias Adr[1:0] = a[1], C[4] \n"
    "  {\n"
    "    RefEnum T1;\n"
    "    RefEnum T2;\n"
    "  }\n",                      // 0 ==> Dual RefEnum is not valid
    "  Alias Adr[1:0] = a[1], C \n"
    "  {\n"
    "    AccessTogether;\n"
    "    AccessTogether;\n"
    "  }\n",                      // 1 ==> Dual AccessTogether is not valid
    "  Alias Adr[1:0] = a[1], C \n"
    "  {\n"
    "    iApplyEndState 3'b110;\n"
    "    iApplyEndState 3'b010;\n"
    "  }\n",                      // 2 ==> Dual iApplyEndState is not valid
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AST::Uniquify() with single instance
//!
void UT_ICL_Reader::test_Uniquify_1_Instance ()
{
  // ---------------- Setup
  //
  istringstream excerpt("Module AsTop\n"                                                     // 01
                        "{\n"                                                                // 02
                        "  Instance reg8  Of  SReg {\n"                                      // 03
                        "  InputPort SI = SI; InputPort DI = I1.DO; Parameter Size = 8;\n"   // 04
                        "  Attribute Copyright  =  \"Picus\";\n"                             // 05
                        "  }\n"                                                              // 06
                        "}\n"                                                                // 07
                        "\n"                                                                 // 08
                        "Module SReg\n"                                                      // 09
                        "{\n"                                                                // 10
                        "  ScanInPort    SI;\n"                                              // 11
                        "  ScanOutPort   SO          { Source  SR[0];}\n"                    // 12
                        "  ScanInterface scan_client { Port SI; Port SO; }\n"                // 13
                        "  ScanRegister SR[7:0]      { ScanInSource SI; ResetValue 'b0; }\n" // 14
                        "}"                                                                  // 15
                        ""s);

  auto           sm = make_shared<SystemModel>();
  ICL_Reader_TSS sut(sm);
  TS_ASSERT_THROWS_NOTHING (sut.UpdateAstFromIcl(excerpt));

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.UniquifyAST());

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  auto ast = sut.AST();
  TS_ASSERT_NOT_NULLPTR (ast);
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module AsTop\n"
                                  "{\n"
                                  "  Instance reg8 Of UniquifiedModules::SReg__uniquified__1\n"
                                  "  {\n"
                                  "    Attribute Copyright = \"Picus\";\n"
                                  "    Parameter Size = 8;\n"
                                  "    InputPort SI = SI;\n"
                                  "    InputPort DI = I1.DO;\n"
                                  "  }\n"
                                  "}\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  ScanInterface scan_client\n"
                                  "  {\n"
                                  "    Port SI;\n"
                                  "    Port SO;\n"
                                  "  }\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO { Source SR[0]; }\n"
                                  "  ScanRegister SR[7:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n"
                                  "NameSpace UniquifiedModules;\n"
                                  "Module SReg__uniquified__1\n"
                                  "{\n"
                                  "  ScanInterface scan_client\n"
                                  "  {\n"
                                  "    Port SI;\n"
                                  "    Port SO;\n"
                                  "  }\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO { Source SR[0]; }\n"
                                  "  ScanRegister SR[7:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}"
                                  ""sv;


  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks AST::Uniquify() with 2 instances of the same module
//!
void UT_ICL_Reader::test_Uniquify_2_Instances ()
{
  // ---------------- Setup
  //
  istringstream excerpt("Module AsTop\n"                                                     // 01
                        "{\n"                                                                // 02
                        "  ScanInPort    SI;\n"                                              // 03
                        "  ScanOutPort   SO { Source sreg_b.SO;}\n"                          // 04
                        "  Instance sreg_a  Of  SReg {\n"                                    // 05
                        "  InputPort SI = SI;\n"                                             // 06
                        "  }\n"                                                              // 07
                        "  Instance sreg_b Of  SReg {\n"                                     // 08
                        "  InputPort SI = sreg_a.SO;\n"                                      // 09
                        "  }\n"                                                              // 10
                        "}\n"                                                                // 11
                        "\n"                                                                 // 12
                        "Module SReg\n"                                                      // 13
                        "{\n"                                                                // 14
                        "  ScanInPort    SI;\n"                                              // 15
                        "  ScanOutPort   SO          { Source  SR[0];}\n"                    // 16
                        "  ScanInterface scan_client { Port SI; Port SO; }\n"                // 17
                        "  ScanRegister SR[7:0]      { ScanInSource SI; ResetValue 'b0; }\n" // 18
                        "}"                                                                  // 19
                        ""s);

  auto           sm = make_shared<SystemModel>();
  ICL_Reader_TSS sut(sm);
  TS_ASSERT_THROWS_NOTHING (sut.UpdateAstFromIcl(excerpt));

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.UniquifyAST());

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  auto ast = sut.AST();
  TS_ASSERT_NOT_NULLPTR (ast);
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint =
                                  "NameSpace;\n"
                                  "Module AsTop\n"
                                  "{\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO { Source sreg_b.SO; }\n"
                                  "  Instance sreg_a Of UniquifiedModules::SReg__uniquified__1\n"
                                  "  {\n"
                                  "    InputPort SI = SI;\n"
                                  "  }\n"
                                  "  Instance sreg_b Of UniquifiedModules::SReg__uniquified__2\n"
                                  "  {\n"
                                  "    InputPort SI = sreg_a.SO;\n"
                                  "  }\n"
                                  "}\n"
                                  "Module SReg\n"
                                  "{\n"
                                  "  ScanInterface scan_client\n"
                                  "  {\n"
                                  "    Port SI;\n"
                                  "    Port SO;\n"
                                  "  }\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO { Source SR[0]; }\n"
                                  "  ScanRegister SR[7:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n"
                                  "NameSpace UniquifiedModules;\n"
                                  "Module SReg__uniquified__1\n"
                                  "{\n"
                                  "  ScanInterface scan_client\n"
                                  "  {\n"
                                  "    Port SI;\n"
                                  "    Port SO;\n"
                                  "  }\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO { Source SR[0]; }\n"
                                  "  ScanRegister SR[7:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}\n"
                                  "Module SReg__uniquified__2\n"
                                  "{\n"
                                  "  ScanInterface scan_client\n"
                                  "  {\n"
                                  "    Port SI;\n"
                                  "    Port SO;\n"
                                  "  }\n"
                                  "  ScanInPort SI;\n"
                                  "  ScanOutPort SO { Source SR[0]; }\n"
                                  "  ScanRegister SR[7:0]\n"
                                  "  {\n"
                                  "    ScanInSource SI;\n"
                                  "    ResetValue 'b0;\n"
                                  "  }\n"
                                  "}"
                                  ""sv;


  auto actual_AST_String = Parsers::AST_PrettyPrinter::PrettyPrint(network);
  TS_ASSERT_EQUALS (actual_AST_String, expected_AST_PrettyPrint);
}


//! Checks AST::Uniquify() with multiple instances of the N modules
//!
void UT_ICL_Reader::test_Uniquify_N_Modules ()
{
  // ---------------- Setup
  //
  CxxTest::setAbortTestOnFail(true);

  auto           iclFile = GetTestFilePath("Bundle_N_Modules.icl");
  std::ifstream  ifs(iclFile);

  auto           sm = make_shared<SystemModel>();
  ICL_Reader_TSS sut(sm);
  TS_ASSERT_THROWS_NOTHING (sut.UpdateAstFromIcl(ifs));

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.UniquifyAST());

  // ---------------- Verify
  //

  auto ast = sut.AST();
  TS_ASSERT_NOT_NULLPTR (ast);
  auto network = ast->Network();
  TS_ASSERT_NOT_NULLPTR (network);

  auto expected_AST_PrettyPrint = GetExpectedAstPrint("test_Uniquify_N_Modules_PrettyPrint.icl");
  auto actual_AST_PrettyPrint   = Parsers::AST_PrettyPrinter::PrettyPrint(network);

  TS_ASSERT_EQUALS (actual_AST_PrettyPrint, expected_AST_PrettyPrint);
}


//! Checks AST::Uniquify() with examples from standard
//!
void UT_ICL_Reader::test_Uniquify_Examples ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto iclFileName      = std::get<0>(data);
    auto expectedFileName = std::get<1>(data);

    auto           iclFilePath = GetTestFilePath(iclFileName);
    std::ifstream  ifs(iclFilePath);
    auto           sm = make_shared<SystemModel>();
    ICL_Reader_TSS sut(sm);

    CxxTest::setAbortTestOnFail(true);
    TS_ASSERT_THROWS_NOTHING (sut.UpdateAstFromIcl(ifs));


    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.UniquifyAST());

    // ---------------- Verify
    //

    auto ast = sut.AST();
    TS_ASSERT_NOT_NULLPTR (ast);
    auto network = ast->Network();
    TS_ASSERT_NOT_NULLPTR (network);

    auto actual_AST_PrettyPrint   = Parsers::AST_PrettyPrinter::PrettyPrint(network);
    auto expected_AST_PrettyPrint = GetExpectedAstPrint(expectedFileName);

    TS_ASSERT_EQUALS (actual_AST_PrettyPrint, expected_AST_PrettyPrint);
  };

  auto data =
  {
    make_tuple("Bundle_Example_With_Parameters.icl", "Uniquified_Example_With_Parameters_PrettyPrint.icl"), // 0
    make_tuple("Bundle_SIB_mux_pre.icl",             "Uniquified_SIB_mux_pre_PrettyPrint.icl"),             // 1
    make_tuple("Bundle_Top_SReg.icl",                "Uniquified_Top_SReg_PrettyPrint.icl"),                // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks AST::Uniquify() when parsing examples from list of files defined in a file (mostly from IEEE 1687-2014 standard)
//!
void UT_ICL_Reader::test_Uniquify_Examples_ListFile ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto& listFileName     = std::get<0>(data);
    auto        expectedFileName = std::get<1>(data);

    const auto  listFilePath     = GetTestFilePath(listFileName);

    CxxTest::setAbortTestOnFail(true);

    auto           sm = make_shared<SystemModel>();
    ICL_Reader_TSS sut(sm);
    sut.FilesSearchPaths().emplace_back(GetTestDirPath(""));

    TS_ASSERT_THROWS_NOTHING (sut.UpdateAstFromFiles(listFilePath));


    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.UniquifyAST());

    // ---------------- Verify
    //
    auto ast = sut.AST();
    TS_ASSERT_NOT_NULLPTR (ast);
    auto network = ast->Network();
    TS_ASSERT_NOT_NULLPTR (network);

    auto actual_AST_PrettyPrint   = Parsers::AST_PrettyPrinter::PrettyPrint(network);
    auto expected_AST_PrettyPrint = GetExpectedAstPrint(expectedFileName);

    TS_ASSERT_EQUALS (actual_AST_PrettyPrint, expected_AST_PrettyPrint);
  };

  auto data =
  {
    make_tuple("List_SReg.txt",             "Uniquified_SReg_PrettyPrint.icl"),             // 0
    make_tuple("List_WrappedInstr.txt",     "Uniquified_WrappedInstr_PrettyPrint.icl"),     // 1
    make_tuple("List_Multiple_SIB_3WI.txt", "Uniquified_Multiple_SIB_3WI_PrettyPrint.icl"), // 2
    make_tuple("List_mux_inline3.txt",      "Uniquified_mux_inline3_PrettyPrint.icl"),      // 3
    make_tuple("List_mux_remote3.txt",      "Uniquified_mux_remote3_PrettyPrint.icl"),      // 4
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
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

  TS_ASSERT_THROWS_NOTHING (sut.UniquifyAST());

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

    TS_ASSERT_THROWS_NOTHING (sut.UniquifyAST());

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

  TS_ASSERT_THROWS_NOTHING (sut.UniquifyAST());

  shared_ptr<SystemModelNode> topNode;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (topNode = sut.GenerateSystemModelNodes(ast));

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
  auto           iclFile = GetTestFilePath("Bundle_N_Modules.icl");
  std::ifstream  ifs(iclFile);

  auto           sm = make_shared<SystemModel>();
  ICL_Reader_TSS sut(sm);

  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_THROWS_NOTHING (sut.UpdateAstFromIcl(ifs));
  auto ast = sut.AST();
  TS_ASSERT_NOT_NULLPTR (ast);

  auto checkResult = AST_Checker::Check(ast->Network());

  TS_ASSERT_FALSE (checkResult.HasIssues());
  TS_ASSERT_THROWS_NOTHING (sut.UniquifyAST());

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

  // PDL algorithm associations
  auto topAsParentNode = dynamic_pointer_cast<ParentNode>(topNode);

  const auto& associations = sut.PDLAlgorithmNameToNodeAssociation();
  TS_ASSERT_NOT_EMPTY (associations);

  auto findNode = [topAsParentNode](string_view path)
  {
    return dynamic_pointer_cast<ParentNode>(topAsParentNode->FindNode(path));
  };

  TS_ASSERT_CONTAINS (associations, AppFunctionNameAndNode("Topless"s, topAsParentNode,         0u)); // Line in ICL is not yet supported!
  TS_ASSERT_CONTAINS (associations, AppFunctionNameAndNode("Bat"s,     findNode("inst_1"),      0u));
  TS_ASSERT_CONTAINS (associations, AppFunctionNameAndNode("Bool"s,    findNode("inst_1"),      0u));
  TS_ASSERT_CONTAINS (associations, AppFunctionNameAndNode("Boot"s,    findNode("inst_1"),      0u));
  TS_ASSERT_CONTAINS (associations, AppFunctionNameAndNode("Mast"s,    findNode("inst_2"),      0u));
  TS_ASSERT_CONTAINS (associations, AppFunctionNameAndNode("Picus"s,   findNode("inst_2"),      0u));
  TS_ASSERT_CONTAINS (associations, AppFunctionNameAndNode("Logica"s,  findNode("inst_2"),      0u));
  TS_ASSERT_CONTAINS (associations, AppFunctionNameAndNode("Bat"s,     findNode("inst_2.inst"), 0u));
  TS_ASSERT_CONTAINS (associations, AppFunctionNameAndNode("Boot"s,    findNode("inst_2.inst"), 0u));

  TS_ASSERT_EQUALS   (associations.size(), 9u);
}


//! Checks ICL_Reader::GenerateSystemModelNodes() when parsing a SIB with the mux before the scan register bit
//!
//! @note Use SIB_mux_pre module that should be connected to something.
//!       This is why there is a warning about Linker with no child
//!
void UT_ICL_Reader::test_Generate_SIB_mux_pre ()
{
  // ---------------- Setup
  //
  auto           iclFile = GetTestFilePath("Bundle_SIB_mux_pre.icl");
  std::ifstream  ifs(iclFile);
  auto           sm = make_shared<SystemModel>();
  ICL_Reader_TSS sut(sm);

  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_THROWS_NOTHING (sut.UpdateAstFromIcl(ifs));
  auto ast = sut.AST();
  TS_ASSERT_NOT_NULLPTR (ast);

  auto checkResult = AST_Checker::Check(ast->Network());

  TS_ASSERT_FALSE          (checkResult.HasIssues());
  TS_ASSERT_THROWS_NOTHING (sut.UniquifyAST());

  // ---------------- Exercise
  //
  auto topNode = sut.GenerateSystemModelNodes(ast);

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (topNode);
  TS_ASSERT_EQUALS      (topNode->Name(), "SIB_mux_pre");

  // With PrettyPrinter
  auto actual_PrettyPrint   = PrettyPrinter::PrettyPrint(topNode,   PrettyPrinterOptions::Parser_debug
                                                                  | PrettyPrinterOptions::ShowSelectorTables);
  auto expected_PrettyPrint = "[Chain](0)     \"SIB_mux_pre\"\n"
                              " [Linker](2)    \"SIBmux\"\n"
                              "  :Selector:(1)  \"SR\", kind: Table_Based, can_select_none: true, inverted_bits: false, reversed_order: false\n"
                              "  Selection Table:\n"
                              "    [0] 0b0\n"
                              "    [1] 0b1\n"
                              "  Deselection Table:\n"
                              "    [0] 0b0\n"
                              "    [1] 0b0\n"
                              " [Register](1)  \"SR\", length: 1, Hold value: true, bypass: 0";

  TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

  // With Checker
  PrependWithTap(sm, topNode);   // This is to avoid warnings about missing AccessInterface
  auto modelCheckResult = sm->Check();
  TS_ASSERT_CONTAINS (modelCheckResult.InformativeReport(), "Linker 'SIBmux' (id: 2) has no child");
}



//! Checks ICL_Reader::GenerateSystemModelNodes() when parsing examples (mostly from IEEE 1687-2014 standard)
//!
//! @note ICL has been cleaned up to ease understanding (some useless stuffs may have been removed)
void UT_ICL_Reader::test_Generate_Examples ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto iclFileName      = std::get<0>(data);
    auto expectedFileName = std::get<1>(data);

    auto           iclFilePath = GetTestFilePath(iclFileName);
    std::ifstream  ifs(iclFilePath);
    auto           sm = make_shared<SystemModel>();
    ICL_Reader_TSS sut(sm);

    sut.FilesSearchPaths().emplace_back(GetTestDirPath(""));

    CxxTest::setAbortTestOnFail(true);
    TS_ASSERT_THROWS_NOTHING (sut.UpdateAstFromIcl(ifs));
    auto ast = sut.AST();
    TS_ASSERT_NOT_NULLPTR (ast);

    auto checkResult = AST_Checker::Check(ast->Network());

    TS_ASSERT_EMPTY          (checkResult.IssuesReport());
    TS_ASSERT_THROWS_NOTHING (sut.UniquifyAST());

    // ---------------- Exercise
    //
    auto topNode = sut.GenerateSystemModelNodes(ast);

    // ---------------- Verify
    //
    // With PrettyPrinter
    auto actual_PrettyPrint   = PrettyPrinter::PrettyPrint(topNode,   PrettyPrinterOptions::Parser_debug_no_id
                                                                    | PrettyPrinterOptions::ShowSelectorTables);
    auto expected_PrettyPrint = GetExpectedModelPrettyPrint(expectedFileName);

    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, topNode);   // This is to avoid warnings about missing AccessInterface
    auto modelCheckResult = sm->Check();
    TS_ASSERT_EMPTY (modelCheckResult.errors);
  };

  auto data =
  {
    make_tuple("Bundle_SIB_mux_post.icl",       "test_Generate_SIB_mux_post_PrettyPrint.txt"),       // 00
    make_tuple("Bundle_BAD_Nested_SIB_3WI.icl", "test_Generate_BAD_Nested_SIB_3WI_PrettyPrint.txt"), // 01
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks ICL_Reader::Parse() when parsing examples bundled in single file (mostly from IEEE 1687-2014 standard)
//!
//! @note ICL files have been cleaned up to ease understanding (some useless stuffs may have been removed)
void UT_ICL_Reader::test_Parse_Examples_Bundles ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto iclFileName      = std::get<0>(data);
    auto expectedFileName = std::get<1>(data);

    auto       iclFilePath = GetTestFilePath(iclFileName);
    auto       sm          = make_shared<SystemModel>();
    ICL_Reader sut(sm);

    sut.FilesSearchPaths().emplace_back(GetTestDirPath(""));

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.Parse(iclFilePath));

    // ---------------- Verify
    //
    auto topNode = std::dynamic_pointer_cast<ParentNode>(sut.ParsedSystemModel());

    // With PrettyPrinter
    auto actual_PrettyPrint   = PrettyPrinter::PrettyPrint(topNode,   PrettyPrinterOptions::Parser_debug_no_id
                                                                    | PrettyPrinterOptions::ShowSelectorTables);
    auto expected_PrettyPrint = GetExpectedModelPrettyPrint(expectedFileName);

    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, topNode);   // This is to avoid warnings about missing AccessInterface
    auto modelCheckResult = sm->Check();
    TS_ASSERT_EMPTY (modelCheckResult.errors);
  };

  auto data =
  {
    make_tuple("Bundle_Top_SReg.icl",     "test_Generate_Top_SReg_PrettyPrint.txt"),     // 00
    make_tuple("Bundle_SIB_mux_post.icl", "test_Generate_SIB_mux_post_PrettyPrint.txt"), // 01
    make_tuple("Bundle_Daisy_3WI.icl",    "test_Generate_Daisy_3WI_PrettyPrint.txt"),    // 02
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks ICL_Reader::Parse() when parsing examples from list of files (mostly from IEEE 1687-2014 standard)
//!
//! @note ICL files have been cleaned up to ease understanding (some useless stuffs may have been removed)
void UT_ICL_Reader::test_CreateModelFromFiles ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto& iclFilesNames    = std::get<0>(data);
    auto        expectedFileName = std::get<1>(data);

    CxxTest::setAbortTestOnFail(true);

    vector<string> iclFilesPaths;
    for (auto iclFileName : iclFilesNames)
    {
      iclFilesPaths.push_back(GetTestFilePath(iclFileName));
    }

    auto sm = make_shared<SystemModel>();
    ICL_Reader sut(sm);

    sut.FilesSearchPaths().emplace_back(GetTestDirPath(""));

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.CreateModelFromFiles(iclFilesPaths));

    // ---------------- Verify
    //
    auto topNode = std::dynamic_pointer_cast<ParentNode>(sut.ParsedSystemModel());

    // With PrettyPrinter
    auto actual_PrettyPrint   = PrettyPrinter::PrettyPrint(topNode,   PrettyPrinterOptions::Parser_debug_no_id
                                                                    | PrettyPrinterOptions::ShowSelectorTables);
    auto expected_PrettyPrint = GetExpectedModelPrettyPrint(expectedFileName);

    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, topNode);   // This is to avoid warnings about missing AccessInterface
    auto modelCheckResult = sm->Check();
    TS_ASSERT_EMPTY (modelCheckResult.errors);
  };

  using data_t = tuple<vector<string_view>, string_view>;
  auto data =
  {
    // 0
    data_t({
            "Tap_and_SReg.icl",
            "SReg.icl",
           },
           "test_Generate_Top_SReg_PrettyPrint.txt"),

    // 1 - Same as test 0 but with files order reversed
    data_t({
            "SReg.icl",
            "Tap_and_SReg.icl",
           },
           "test_Generate_Top_SReg_PrettyPrint.txt"),

    // 2
    data_t({
             "Daisy_3WI.icl",
             "WrappedInstr.icl",
             "Instrument.icl",
             "SReg.icl",
            },
            "test_Generate_Daisy_3WI_PrettyPrint.txt"),

    // 3 - Same as test 2 but with different files order (except to as there is no AccessLink)
    data_t({
             "Daisy_3WI.icl",
             "SReg.icl",
             "Instrument.icl",
             "WrappedInstr.icl",
            },
            "test_Generate_Daisy_3WI_PrettyPrint.txt"),

    // 4 - Same as test 3 but with different files order and a JTAG TAP
    data_t({
             "SReg.icl",
             "Daisy_3WI.icl",
             "Instrument.icl",
             "Tap_and_Daisy_3WI.icl",
             "WrappedInstr.icl",
            },
            "Tap_and_Daisy_3WI_PrettyPrint.txt"),

    // 5
    data_t({
             "Top_Single_SIB_3WI.icl",
             "WrappedInstr.icl",
             "Instrument.icl",
             "SIB_mux_pre.icl",
             "SReg.icl",
           },
           "test_Generate_Single_SIB_3WI_PrettyPrint.txt"),

    // 6
    data_t({
             "Top_Multiple_SIB_3WI.icl",
             "WrappedInstr.icl",
             "Instrument.icl",
             "SIB_mux_pre.icl",
             "SReg.icl",
           },
           "test_Generate_Multiple_SIB_3WI_PrettyPrint.txt"),

    // 7
    data_t({
             "Top_Nested_SIB_3WI.icl",
             "WrappedInstr.icl",
             "Instrument.icl",
             "SIB_mux_pre.icl",
             "SReg.icl",
           },
           "test_Generate_Nested_SIB_3WI_PrettyPrint.txt"),

    // 8
    data_t({
             "Top_BAD_Nested_SIB_3WI.icl",
             "WrappedInstr.icl",
             "Instrument.icl",
             "SIB_mux_post.icl",
             "SIB_mux_pre.icl", // This one is not used but should have no impact on resulting SystemModel !
             "SReg.icl",
           },
           "test_Generate_BAD_Nested_SIB_3WI_PrettyPrint.txt"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks ICL_Reader::Parse() when parsing examples from list of files defined in a file (mostly from IEEE 1687-2014 standard)
//!
void UT_ICL_Reader::test_CreateModelFromFiles_ListFile ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    const auto& listFileName     = std::get<0>(data);
    auto        expectedFileName = std::get<1>(data);

    CxxTest::setAbortTestOnFail(true);

    const auto listFilePath = GetTestFilePath(listFileName);

    auto sm = make_shared<SystemModel>();
    ICL_Reader sut(sm);

    sut.FilesSearchPaths().emplace_back(GetTestDirPath(""));

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.CreateModelFromFiles(listFilePath));

    // ---------------- Verify
    //
    auto topNode = std::dynamic_pointer_cast<ParentNode>(sut.ParsedSystemModel());

    // With PrettyPrinter
    auto actual_PrettyPrint   = PrettyPrinter::PrettyPrint(topNode,   PrettyPrinterOptions::Parser_debug_no_id
                                                                    | PrettyPrinterOptions::ShowSelectorTables);
    auto expected_PrettyPrint = GetExpectedModelPrettyPrint(expectedFileName);

    TS_ASSERT_EQUALS (actual_PrettyPrint, expected_PrettyPrint);

    // With Checker
    PrependWithTap(sm, topNode);   // This is to avoid warnings about missing AccessInterface
    auto modelCheckResult = sm->Check();
    TS_ASSERT_EMPTY (modelCheckResult.errors);
  };

  auto data =
  {
    make_tuple("List_mux_remote3.txt",      "test_Generate_mux_remote3_PrettyPrint.txt"),      // 3
    make_tuple("List_WrappedInstr.txt",     "test_Generate_WrappedInstr_PrettyPrint.txt"),     // 0
    make_tuple("List_Multiple_SIB_3WI.txt", "test_Generate_Multiple_SIB_3WI_PrettyPrint.txt"), // 1
    make_tuple("List_mux_inline3.txt",      "test_Generate_mux_inline3_PrettyPrint.txt"),      // 2
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}




//===========================================================================
// End of UT_ICL_Reader.cpp
//===========================================================================
