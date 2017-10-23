//===========================================================================
//                           UT_ModelBuildDriver.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_ModelBuildDriver.cpp
//!
//! Implements test fixture for testing ModelBuildDriver
//!
//===========================================================================


#include "UT_ModelBuildDriver.hpp"
#include "ModelBuildDriver.hpp"
#include "SystemModel.hpp"
#include "SystemModelNode.hpp"
#include "SystemModelFactory.hpp"
#include "AppFunctionNameAndNode.hpp"
#include "PrettyPrinter.hpp"
#include "Utility.hpp"
#include "SIT_Reader.hpp"
#include "ParserException.hpp"

#include "TestModelBuilder.hpp"
#include "TestUtilities.hpp"

#include <experimental/string_view>
#include <cxxtest/ValueTraits.h>

using std::string;
using std::experimental::string_view;
using std::shared_ptr;

using namespace std::string_literals;
using namespace mast;

namespace
{

//! Returns path of a file accessible by test runner
//!
//! @param fileName   File name used by test
//!
string GetTestFilePath (string_view fileName, bool checkExists = true)
{
  return test::GetTestFilePath({"Mast_Core"s, "UT_TestFiles"s, string(fileName)}, checkExists);
}
//
//  End of: GetTestFilePath
//---------------------------------------------------------------------------

} // End of unnamed namespace

//! Initializes test (called for each test)
void UT_ModelBuildDriver::setUp ()
{
  SystemModelNode::ResetNodeIdentifier();
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped
  CxxTest::setStringResultsOnNewLine(true);
}


//! Checks ModelBuildDriver default constructor()
//!
void UT_ModelBuildDriver::test_Constructor ()
{
  // ---------------- Exercise
  //
  ModelBuildDriver sut;

  // ---------------- Verify
  //
  TS_ASSERT_EMPTY (sut.PDLAlgorithmNameToNodeAssociation());
}



//! Checks ModelBuildDriver::CreateModelFromSitFile() when providing an empty path
//!
void UT_ModelBuildDriver::test_CreateModelFromSitFile_EmptyPath ()
{
  // ---------------- Setup
  //
  ModelBuildDriver sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.CreateModelFromSitFile(""), std::invalid_argument);
}


//! Checks ModelBuildDriver::CreateModelFromSitFile() when providing a file path that cannot be found
//!
void UT_ModelBuildDriver::test_CreateModelFromSitFile_NotFoundFile ()
{
  // ---------------- Setup
  //
  ModelBuildDriver sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.CreateModelFromSitFile("No_exist_file.sit"), std::runtime_error);
}


//! Checks ModelBuildDriver::CreateModelFromSitFile() when there is no INSTANCE statement
//!
void UT_ModelBuildDriver::test_CreateModelFromSitFile_NoInstance ()
{
  // ---------------- Setup
  //
  ModelBuildDriver        sut;
  auto                    sitFile = GetTestFilePath("UT_ModelBuildDriver_no_Instance.sit");
  shared_ptr<SystemModel> sm;

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sm = sut.CreateModelFromSitFile(sitFile));

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (sm);
  TS_ASSERT_EMPTY (sut.ErrorMessage());

  auto prettyPrint = PrettyPrinter::PrettyPrint(*(sm->Root()), PrettyPrinterOptions::DisplayValueAuto);
  auto expectedPrettyPrint = "[Access_I](0)  \"TAP\"\n"
                             " [Register](1)  \"TAP_IR\", length: 4, Hold value: true, bypass: 0b1111\n"
                             " [Linker](2)    \"TAP_DR_Mux\"\n"
                             "  :Selector:(1)  \"TAP_IR\"\n"
                             "  [Register](3)  \"TAP_BPY\", length: 1, bypass: 0b1\n"
                             "  [Register](4)  \"reg\", length: 12, bypass: 0xABC";

  TS_ASSERT_EQUALS (prettyPrint, expectedPrettyPrint);
}


//! Checks ModelBuildDriver::CreateModelFromSitFile() when there is one INSTANCE statement
//!
void UT_ModelBuildDriver::test_CreateModelFromSitFile_OneInstance ()
{
  // ---------------- Setup
  //
  ModelBuildDriver        sut;
  auto                    sitFile = GetTestFilePath("UT_ModelBuildDriver_one_Instance.sit");
  shared_ptr<SystemModel> sm;

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sm = sut.CreateModelFromSitFile(sitFile));

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (sm);
  TS_ASSERT_EMPTY       (sut.ErrorMessage());

  auto prettyPrint         = PrettyPrinter::PrettyPrint(*(sm->Root()), PrettyPrinterOptions::DisplayValueAuto);
  auto expectedPrettyPrint = "[Access_I](0)  \"TAP\"\n"
                             " [Register](1)  \"TAP_IR\", length: 4, Hold value: true, bypass: 0b1111\n"
                             " [Linker](2)    \"TAP_DR_Mux\"\n"
                             "  :Selector:(1)  \"TAP_IR\"\n"
                             "  [Register](3)  \"TAP_BPY\", length: 1, bypass: 0b1\n"
                             "  [Register](4)  \"reg\", length: 12, bypass: 0xABC\n"
                             "  [Chain](5)     \"Inst_1\"\n"
                             "   [Chain](6)     \"FooChain\"\n"
                             "    [Register](7)  \"BarReg\", length: 8, bypass: 0xBD";

  TS_ASSERT_EQUALS (prettyPrint, expectedPrettyPrint);
}


//! Checks ModelBuildDriver::CreateModelFromSitFile() when there is one INSTANCE statement without ".sit" extension
//!
void UT_ModelBuildDriver::test_CreateModelFromSitFile_OneInstance_no_Ext ()
{
  // ---------------- Setup
  //
  ModelBuildDriver        sut;
  auto                    sitFile = GetTestFilePath("UT_ModelBuildDriver_one_Instance", false); // No ".sit" intended
  shared_ptr<SystemModel> sm;

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sm = sut.CreateModelFromSitFile(sitFile));

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (sm);
  TS_ASSERT_EMPTY       (sut.ErrorMessage());

  auto prettyPrint         = PrettyPrinter::PrettyPrint(*(sm->Root()), PrettyPrinterOptions::DisplayValueAuto);
  auto expectedPrettyPrint = "[Access_I](0)  \"TAP\"\n"
                             " [Register](1)  \"TAP_IR\", length: 4, Hold value: true, bypass: 0b1111\n"
                             " [Linker](2)    \"TAP_DR_Mux\"\n"
                             "  :Selector:(1)  \"TAP_IR\"\n"
                             "  [Register](3)  \"TAP_BPY\", length: 1, bypass: 0b1\n"
                             "  [Register](4)  \"reg\", length: 12, bypass: 0xABC\n"
                             "  [Chain](5)     \"Inst_1\"\n"
                             "   [Chain](6)     \"FooChain\"\n"
                             "    [Register](7)  \"BarReg\", length: 8, bypass: 0xBD";

  TS_ASSERT_EQUALS (prettyPrint, expectedPrettyPrint);
}


//! Checks ModelBuildDriver::CreateModelFromSitFile() when there is two INSTANCE statement
//!
void UT_ModelBuildDriver::test_CreateModelFromSitFile_TwoInstances ()
{
  // ---------------- Setup
  //
  ModelBuildDriver        sut;
  auto                    sitFile = GetTestFilePath("UT_ModelBuildDriver_two_Instances.sit");
  shared_ptr<SystemModel> sm;

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sm = sut.CreateModelFromSitFile(sitFile));

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (sm);
  TS_ASSERT_EMPTY       (sut.ErrorMessage());

  auto prettyPrint         = PrettyPrinter::PrettyPrint(*(sm->Root()), PrettyPrinterOptions::DisplayValueAuto);
  auto expectedPrettyPrint = "[Access_I](0)  \"TAP\"\n"
                             " [Register](1)  \"TAP_IR\", length: 4, Hold value: true, bypass: 0b1111\n"
                             " [Linker](2)    \"TAP_DR_Mux\"\n"
                             "  :Selector:(1)  \"TAP_IR\"\n"
                             "  [Register](3)  \"TAP_BPY\", length: 1, bypass: 0b1\n"
                             "  [Chain](4)     \"Inst_1\"\n"
                             "   [Chain](7)     \"FooChain\"\n"
                             "    [Register](8)  \"BarReg\", length: 8, bypass: 0xBD\n"
                             "  [Register](5)  \"reg\", length: 12, bypass: 0xABC\n"
                             "  [Chain](6)     \"Inst_2\"\n"
                             "   [Chain](9)     \"FooChain\"\n"
                             "    [Register](10) \"BarReg\", length: 8, bypass: 0xBD";

  TS_ASSERT_EQUALS (prettyPrint, expectedPrettyPrint);

  // Check with checker
  auto checkResult = sm->Check();
  TS_ASSERT_EMPTY (checkResult.InformativeReport());
}


//! Checks ModelBuildDriver::CreateModelFromSitFile() when the are hierarchical INSTANCE statements
//!
void UT_ModelBuildDriver::test_CreateModelFromSitFile_Hierarchical ()
{
  // ---------------- Setup
  //
  ModelBuildDriver        sut;
  auto                    sitFile = GetTestFilePath("UT_ModelBuildDriver_hierarchical.sit");
  shared_ptr<SystemModel> sm;

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sm = sut.CreateModelFromSitFile(sitFile));

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (sm);
  TS_ASSERT_EMPTY       (sut.ErrorMessage());

  // Check with pretty print
  auto prettyPrint         = PrettyPrinter::PrettyPrint(*(sm->Root()), PrettyPrinterOptions::DisplayValueAuto
                                                                     | PrettyPrinterOptions::ShowNodeIsIgnored);
  auto expectedPrettyPrint = "[Access_I](0)  \"TAP\"\n"
                             " [Register](1)  \"TAP_IR\", length: 6, Hold value: true, bypass: 0b1111_11\n"
                             " [Linker](2)    \"TAP_DR_Mux\"\n"
                             "  :Selector:(1)  \"TAP_IR\"\n"
                             "  [Register](3)  \"TAP_BPY\", length: 1, bypass: 0b1\n"
                             "  [Chain](4)     \"Inst_1\"\n"
                             "   [Chain](7)     \"FooChain\", ignore_in_path: true\n"
                             "    [Register](8)  \"CatReg\", length: 4, bypass: 0b1010\n"
                             "    [Register](9)  \"BatReg\", length: 5, bypass: 0b1101_1\n"
                             "    [Chain](10)    \"Sub_level_2\"\n"
                             "     [Chain](11)    \"FooChain\"\n"
                             "      [Register](12) \"BarReg\", length: 8, bypass: 0xBD\n"
                             "  [Register](5)  \"reg\", length: 12, bypass: 0xABC\n"
                             "  [Chain](6)     \"Inst_2\"\n"
                             "   [Chain](13)    \"FooChain\"\n"
                             "    [Register](14) \"BarReg\", length: 8, bypass: 0xBD";

  TS_ASSERT_EQUALS (prettyPrint, expectedPrettyPrint);

  // Check with checker
  auto checkResult = sm->Check();
  TS_ASSERT_EMPTY (checkResult.InformativeReport());
}


//! Checks ModelBuildDriver::CreateModelFromSitFile() when the are hierarchical INSTANCE statements
//!                                                   refering to file in sub-directory
//!
void UT_ModelBuildDriver::test_CreateModelFromSitFile_Hierarchical_SubDir ()
{
  // ---------------- Setup
  //
  ModelBuildDriver        sut;
  auto                    sitFile = GetTestFilePath("UT_ModelBuildDriver_hierarchical_2.sit");
  shared_ptr<SystemModel> sm;

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sm = sut.CreateModelFromSitFile(sitFile));

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (sm);
  TS_ASSERT_EMPTY       (sut.ErrorMessage());

  // Check with pretty print
  auto prettyPrint         = PrettyPrinter::PrettyPrint(*(sm->Root()), PrettyPrinterOptions::DisplayValueAuto
                                                                     | PrettyPrinterOptions::ShowNodeIsIgnored);
  auto expectedPrettyPrint = "[Access_I](0)  \"TAP\"\n"
                             " [Register](1)  \"TAP_IR\", length: 6, Hold value: true, bypass: 0b1111_11\n"
                             " [Linker](2)    \"TAP_DR_Mux\"\n"
                             "  :Selector:(1)  \"TAP_IR\"\n"
                             "  [Register](3)  \"TAP_BPY\", length: 1, bypass: 0b1\n"
                             "  [Chain](4)     \"Inst_1\"\n"
                             "   [Chain](6)     \"FooChain\", ignore_in_path: true\n"
                             "    [Register](7)  \"CatReg\", length: 4, bypass: 0b1010\n"
                             "    [Register](8)  \"BatReg\", length: 5, bypass: 0b1101_1\n"
                             "    [Chain](9)     \"Sub_level_2\"\n"
                             "     [Chain](10)    \"FooChain\"\n"
                             "      [Register](11) \"BarReg\", length: 8, bypass: 0xBD\n"
                             "  [Chain](5)     \"Inst_2\"\n"
                             "   [Chain](12)    \"Sub3Chain\", ignore_in_path: true\n"
                             "    [Register](13) \"Sub_3_Reg_1\", length: 8, bypass: 0x03\n"
                             "    [Register](14) \"Sub_3_Reg_4\", length: 6, bypass: 0b1001_01";

  TS_ASSERT_EQUALS (prettyPrint, expectedPrettyPrint);

  // Check with checker
  auto checkResult = sm->Check();
  TS_ASSERT_EMPTY (checkResult.InformativeReport());
}


//! Checks ModelBuildDriver::CreateModelFromSitFile() when there is an error in a SIT file
//!
void UT_ModelBuildDriver::test_CreateModelFromSitFile_ParsingError ()
{
  // ---------------- Setup
  //
  ModelBuildDriver        sut;
  auto                    sitFile = GetTestFilePath("UT_ModelBuildDriver_with_BadSit.sit");
  shared_ptr<SystemModel> sm;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS (sm = sut.CreateModelFromSitFile(sitFile), ParserException);

  // ---------------- Verify
  //
  TS_ASSERT_NULLPTR  (sm);
  TS_ASSERT_CONTAINS (sut.ErrorMessage(), "Line 5:12-13: syntax error");
}


//! Checks ModelBuildDriver::CreateModelFromSitFile() when there is one "factory" INSTANCE statement
//!
void UT_ModelBuildDriver::test_CreateModelFromSitFile_OneFactoryInstance ()
{
  // ---------------- Setup
  //
  ModelBuildDriver        sut;
  auto                    sitFile = GetTestFilePath("UT_ModelBuildDriver_one_FactoryInstance.sit");
  shared_ptr<SystemModel> sm;

  SystemModelFactory::Instance().RegisterCreator("Make_MIB"s, [](SystemModel& sm, const string& /* parameters */)
  {
    test::TestModelBuilder builder(sm);

    auto node = builder.Create_Default_MIB("testMIB", 3u);
    sm.CreateRegister("FooReg", BinaryVector::CreateFromString("0xDead"), false, node);
    sm.CreateRegister("BarReg", BinaryVector::CreateFromString("0xFace"), false, node);
    sm.CreateRegister("TarReg", BinaryVector::CreateFromString("0xBeef"), false, node);
    return node;
  });

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sm = sut.CreateModelFromSitFile(sitFile));

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (sm);
  TS_ASSERT_EMPTY       (sut.ErrorMessage());

  auto prettyPrint         = PrettyPrinter::PrettyPrint(*(sm->Root()), PrettyPrinterOptions::DisplayValueAuto);
  auto expectedPrettyPrint = "[Access_I](0)  \"TAP\"\n"
                             " [Register](1)  \"TAP_IR\", length: 4, Hold value: true, bypass: 0b1111\n"
                             " [Linker](2)    \"TAP_DR_Mux\"\n"
                             "  :Selector:(1)  \"TAP_IR\"\n"
                             "  [Register](3)  \"TAP_BPY\", length: 1, bypass: 0b1\n"
                             "  [Register](4)  \"reg\", length: 12, bypass: 0xABC\n"
                             "  [Chain](5)     \"Inst_1\"\n"
                             "   [Chain](7)     \"testMIB\"\n"
                             "    [Register](6)  \"testMIB_ctrl\", length: 2, Hold value: true, bypass: 0b00\n"
                             "    [Linker](8)    \"testMIB_mux\"\n"
                             "     :Selector:(6)  \"testMIB_ctrl\"\n"
                             "     [Register](9)  \"FooReg\", length: 16, bypass: 0xDEAD\n"
                             "     [Register](10) \"BarReg\", length: 16, bypass: 0xFACE\n"
                             "     [Register](11) \"TarReg\", length: 16, bypass: 0xBEEF";

  TS_ASSERT_EQUALS (prettyPrint, expectedPrettyPrint);
}


//! Checks ModelBuildDriver::CreateModelFromSitFile() when there is "factory" and "SIT" INSTANCE statements
//!
void UT_ModelBuildDriver::test_CreateModelFromSitFile_MixKindsInstances ()
{
  // ---------------- Setup
  //
  ModelBuildDriver        sut;
  auto                    sitFile = GetTestFilePath("UT_ModelBuildDriver_MixKindsInstances.sit");
  shared_ptr<SystemModel> sm;

  auto& factory = SystemModelFactory::Instance();
  factory.Clear();
  TS_ASSERT_EQUALS (factory.RegisteredCreatorsCount(), 0);
  factory.RegisterCreator("Make_MIB"s, [](SystemModel& sm, const string& /* parameters */)
  {
    test::TestModelBuilder builder(sm);

    auto node = builder.Create_Default_MIB("testMIB", 3u);
    sm.CreateRegister("FooReg", BinaryVector::CreateFromString("0xDead"), false, node);
    sm.CreateRegister("BarReg", BinaryVector::CreateFromString("0xFace"), false, node);
    sm.CreateRegister("TarReg", BinaryVector::CreateFromString("0xBeef"), false, node);
    return node;
  });
  factory.RegisterCreator("Make_SIB"s, [](SystemModel& sm, const string& /* parameters */)
  {
    test::TestModelBuilder builder(sm);

    auto node = builder.Create_Default_SIB("testSIB");
    sm.CreateRegister("FarReg", BinaryVector::CreateFromString("0x345"), false, node);
    return node;
  });


  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sm = sut.CreateModelFromSitFile(sitFile));

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (sm);
  TS_ASSERT_EMPTY       (sut.ErrorMessage());

  auto prettyPrint         = PrettyPrinter::PrettyPrint(*(sm->Root()), PrettyPrinterOptions::DisplayValueAuto);
  auto expectedPrettyPrint = "[Access_I](0)  \"TAP\"\n"
                             " [Register](1)  \"TAP_IR\", length: 4, Hold value: true, bypass: 0b1111\n"
                             " [Linker](2)    \"TAP_DR_Mux\"\n"
                             "  :Selector:(1)  \"TAP_IR\"\n"
                             "  [Register](3)  \"TAP_BPY\", length: 1, bypass: 0b1\n"
                             "  [Chain](4)     \"Inst_1\"\n"
                             "   [Chain](7)     \"Sub4Chain\"\n"
                             "    [Register](8)  \"Sub_4_Reg_1\", length: 8, bypass: 0x03\n"
                             "    [Register](9)  \"Sub_4_Reg_2\", length: 6, bypass: 0b1001_01\n"
                             "    [Chain](10)    \"Level_4_SIB\"\n"
                             "     [Chain](12)    \"testSIB\"\n"
                             "      [Register](11) \"testSIB_ctrl\", length: 1, Hold value: true, bypass: 0b0\n"
                             "      [Linker](13)   \"testSIB_mux\"\n"
                             "       :Selector:(11) \"testSIB_ctrl\"\n"
                             "       [Register](14) \"FarReg\", length: 12, bypass: 0x345\n"
                             "  [Register](5)  \"reg\", length: 12, bypass: 0xABC\n"
                             "  [Chain](6)     \"Inst_2\"\n"
                             "   [Chain](16)    \"testMIB\"\n"
                             "    [Register](15) \"testMIB_ctrl\", length: 2, Hold value: true, bypass: 0b00\n"
                             "    [Linker](17)   \"testMIB_mux\"\n"
                             "     :Selector:(15) \"testMIB_ctrl\"\n"
                             "     [Register](18) \"FooReg\", length: 16, bypass: 0xDEAD\n"
                             "     [Register](19) \"BarReg\", length: 16, bypass: 0xFACE\n"
                             "     [Register](20) \"TarReg\", length: 16, bypass: 0xBEEF";

  TS_ASSERT_EQUALS (prettyPrint, expectedPrettyPrint);
}

//! Checks ModelBuildDriver::CreateModelFromSitFile() when there is one "factory" INSTANCE statement
//!
void UT_ModelBuildDriver::test_CreateModelFromSitFile_MissingFactory ()
{
  // ---------------- Setup
  //
  ModelBuildDriver        sut;
  auto                    sitFile = GetTestFilePath("UT_ModelBuildDriver_one_FactoryInstance.sit");
  shared_ptr<SystemModel> sm;

  SystemModelFactory::Instance().Clear();

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sm = sut.CreateModelFromSitFile(sitFile), std::exception);
}

//===========================================================================
// End of UT_ModelBuildDriver.cpp
//===========================================================================
