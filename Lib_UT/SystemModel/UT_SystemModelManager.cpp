//===========================================================================
//                           UT_SystemModelManager.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SystemModelManager.cpp
//!
//! Implements test fixture for testing SystemModelManager
//!
//===========================================================================

#include "UT_SystemModelManager.hpp"
#include "SystemModelManager.hpp"
#include "SystemModelBuilder.hpp"
#include "Spy_AccessInterfaceProtocols.hpp"
#include "Spy_SVF_Protocol.hpp"
#include "GmlPrinterVisitor.hpp"
#include "BinaryVector_Traits.hpp"
#include "DefaultBinaryPathSelector.hpp"

#include <memory>

using namespace mast;
using namespace test;
using std::make_shared;
using std::make_tuple;
using std::string;
using std::vector;

namespace
{

//! Creates test case "1500", setting some register "next to sut" values
//!
std::shared_ptr<AccessInterface> Create_TestCase_1500 (SystemModel& sm, bool reportGml = false)
{
  SystemModelBuilder builder(sm);

  auto ai        = builder.Create_TestCase_1500("Tap", 4u);
  auto ir        = sm.RegisterWithId(1u);
  auto bpy       = sm.RegisterWithId(3u);
  auto regStatic = sm.RegisterWithId(5u);
  auto regDyn_0  = sm.RegisterWithId(14u);
  auto regDyn_1  = sm.RegisterWithId(15u);
  auto regDyn_2  = sm.RegisterWithId(16u);
  auto regDyn_3  = sm.RegisterWithId(17u);

  regStatic->SetToSut  (BinaryVector(STATIC_TDR_LEN,  0x05));
  regDyn_0->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x09));
  regDyn_1->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x0A));
  regDyn_2->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0xB0));
  regDyn_3->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0xC0));

  regStatic->SetBypass (BinaryVector(STATIC_TDR_LEN,  0x36));
  regDyn_0->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x40));
  regDyn_1->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x41));
  regDyn_2->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x42));
  regDyn_3->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x43));

  if (reportGml)
  {
    GmlPrinterVisitor gmlPrinter("Testcase_1500", GmlPrinterOptions::Std);
    ai->Accept(gmlPrinter);
    TS_ASSERT_EQUALS (gmlPrinter.Graph(), "");
  }

  return ai;
}
//
//  End of: Create_TestCase_1500
//---------------------------------------------------------------------------


//! Creates test case "MIB_Multichain_Pre"
//!
std::shared_ptr<AccessInterface> Create_TestCase_MIB_Multichain_Pre (SystemModel& sm, bool reportGml = false)
{
  SystemModelBuilder builder(sm);

  auto tap = builder.Create_TestCase_MIB_Multichain_Pre("TAP", 4u);

  auto regDyn_0  = sm.RegisterWithId(6u);
  auto regDyn_1  = sm.RegisterWithId(7u);
  auto regDyn_2  = sm.RegisterWithId(8u);
  auto regDyn_3  = sm.RegisterWithId(9u);

  regDyn_0->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x60));
  regDyn_1->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x61));
  regDyn_2->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x62));
  regDyn_3->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x63));

  regDyn_0->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x40));
  regDyn_1->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x41));
  regDyn_2->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x42));
  regDyn_3->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x43));

  if (reportGml)
  {
    GmlPrinterVisitor gmlPrinter("MIB_Multichain_Pre", GmlPrinterOptions::Std);
    tap->Accept(gmlPrinter);
    TS_ASSERT_EQUALS (gmlPrinter.Graph(), "");
  }

  return tap;
}
//
//  End of: Create_TestCase_MIB_Multichain_Pre
//---------------------------------------------------------------------------


//! Creates test case "MIB_Multichain_Post"
//!
std::shared_ptr<AccessInterface> Create_TestCase_MIB_Multichain_Post (SystemModel& sm, bool reportGml = false)
{
  SystemModelBuilder builder(sm);

  auto tap = builder.Create_TestCase_MIB_Multichain_Post("TAP", 4u);

  auto regDyn_0  = sm.RegisterWithId(6u);
  auto regDyn_1  = sm.RegisterWithId(7u);
  auto regDyn_2  = sm.RegisterWithId(8u);
  auto regDyn_3  = sm.RegisterWithId(9u);

  regDyn_0->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x60));
  regDyn_1->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x61));
  regDyn_2->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x62));
  regDyn_3->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x63));

  regDyn_0->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x40));
  regDyn_1->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x41));
  regDyn_2->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x42));
  regDyn_3->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x43));

  if (reportGml)
  {
    GmlPrinterVisitor gmlPrinter("MIB_Multichain_Post", GmlPrinterOptions::Std);
    tap->Accept(gmlPrinter);
    TS_ASSERT_EQUALS (gmlPrinter.Graph(), "");
  }

  return tap;
}
//
//  End of: Create_TestCase_MIB_Multichain_Post
//---------------------------------------------------------------------------


} // End of unnamed namespace

//! Initializes test (called for each test)
void UT_SystemModelManager::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}


//! Checks SystemModelManager constructor when there are no node in system model
//!
void UT_SystemModelManager::test_Constructor_EmptySystemModel ()
{
  // ---------------- Setup
  //
  SystemModel sm;


  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (SystemModelManager sut(sm), std::exception);
}

//! Checks SystemModelManager constructor when there are no node in system model
//!
void UT_SystemModelManager::test_Constructor_SystemModel_Without_AI ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);

  auto root = builder.Create_Simple_MIB();

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (SystemModelManager sut(sm), std::exception);
}

//! Checks SystemModelManager constructor when root is an AccessInterface
//!
void UT_SystemModelManager::test_Constructor_Root_is_AI ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);

  auto root = builder.Create_TestCase_AccessInterface();


  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (SystemModelManager sut(sm));
}


//! Checks SystemModelManager constructor when root is a chain with a single AccessInterface
//!
void UT_SystemModelManager::test_Constructor_Root_is_Chain_1_AI ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto root = sm.CreateChain("root");
  SystemModelBuilder builder(sm);

  auto ai   = builder.Create_TestCase_AccessInterface();

  root->AppendChild(ai);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (SystemModelManager sut(sm));
}


//! Checks SystemModelManager constructor when root is a chain with 3 AccessInterfaces
//!
void UT_SystemModelManager::test_Constructor_Root_is_Chain_3_AI ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto root = sm.CreateChain("root");
  SystemModelBuilder builder(sm);

  auto ai_1 = builder.Create_TestCase_AccessInterface();
  auto ai_2 = builder.Create_TestCase_AccessInterface();
  auto ai_3 = builder.Create_TestCase_AccessInterface();

  root->AppendChild(ai_1);
  root->AppendChild(ai_2);
  root->AppendChild(ai_3);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (SystemModelManager sut(sm));
}


//! Checks SystemModelManager DoDataCycles when using AccessInterface testcase
//!
void UT_SystemModelManager::test_DoDataCycles_AccessInterface ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  SystemModelBuilder builder(sm);

  auto ai    = builder.Create_TestCase_AccessInterface();
  auto ir    = sm.RegisterWithId(1u);
  auto bpy   = sm.RegisterWithId(3u);
  auto reg_1 = sm.RegisterWithId(5u);
  auto reg_2 = sm.RegisterWithId(7u);

  reg_1->SetToSut   (BinaryVector(STATIC_TDR_LEN, 0x0A));
  reg_2->SetToSut   (BinaryVector(STATIC_TDR_LEN, 0xB0));
  reg_1->SetBypass  (BinaryVector(STATIC_TDR_LEN, 0x41));
  reg_2->SetBypass  (BinaryVector(STATIC_TDR_LEN, 0x42));

  auto spy = make_shared<Spy_AccessInterfaceProtocols>();
  ai->SetProtocol (spy);

//+  GmlPrinterVisitor gmlPrinter("Testcase_AccessInterface", GmlPrinterOptions::Std);
//+  ai->Accept(gmlPrinter);
//+  TS_ASSERT_EQUALS (gmlPrinter.Graph(), "");

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  auto gotSutVectors = spy->ToSutVectors();

  std::vector<mast::BinaryVector> expected
  {
    BinaryVector::CreateFromString("/x02"),   // 00 : IR
    BinaryVector::CreateFromString("/xB0B0"), // 01 : DR (reg_2)
    BinaryVector::CreateFromString("/x01"),   // 02 : IR
    BinaryVector::CreateFromString("/x0A0A"), // 03 : DR (reg_1)
    BinaryVector::CreateFromString("/xFF"),   // 04 : IR
  };

  TS_ASSERT_EQUALS (gotSutVectors, expected);
}


//! Checks SystemModelManager DoDataCycles when using "1500" testcase
//!
void UT_SystemModelManager::test_DoDataCycles_1500 ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  SystemModelBuilder builder(sm);

  auto ai = Create_TestCase_1500(sm, false);

  auto spy = make_shared<Spy_AccessInterfaceProtocols>();
  ai->SetProtocol (spy);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  auto gotSutVectors = spy->ToSutVectors();

  std::vector<mast::BinaryVector> expected
  {
    BinaryVector::CreateFromString("/x01"),                 // 00 : IR
    BinaryVector::CreateFromString("/x0505/b10"),           // 01 : DR
    BinaryVector::CreateFromString("/x3636/b0:100"),        // 02 : DR
    BinaryVector::CreateFromString("/x3636/b1/xC0C0_C0C0"), // 03 : DR
    BinaryVector::CreateFromString("/x3636/b0:011"),        // 04 : DR
    BinaryVector::CreateFromString("/x3636/b1/xB0B0_B0B0"), // 05 : DR
    BinaryVector::CreateFromString("/x3636/b0:010"),        // 06 : DR
    BinaryVector::CreateFromString("/x3636/b1/x0A0A_0A0A"), // 07 : DR
    BinaryVector::CreateFromString("/x3636/b0:001"),        // 08 : DR
    BinaryVector::CreateFromString("/x3636/b0/x0909_0909"), // 09 : DR
    BinaryVector::CreateFromString("/x3636/b1/x4040_4040"), // 10 : DR
    BinaryVector::CreateFromString("/x3636/b1:000"),        // 11 : DR
    BinaryVector::CreateFromString("/x3636/b0:000"),        // 15 : DR
    BinaryVector::CreateFromString("/xFF"),                 // 16 : IR
  };

  TS_ASSERT_EQUALS (gotSutVectors, expected);
}


//! Checks SystemModelManager DoDataCycles when using "1500" testcase and SVF protocol
//!
void UT_SystemModelManager::test_DoDataCycles_1500_SVF ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  SystemModelBuilder builder(sm);

  auto ai        = Create_TestCase_1500(sm);

  auto spy = make_shared<Spy_SVF_Protocol>();
  ai->SetProtocol (spy);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  auto gotSvfCommands = spy->SVFCommands();

  vector<string> expected
  {
     "SIR 8 TDI(01);\n",                   // 00
     "SDR 18 TDI(001416);\n",              // 01
     "SDR 20 TDI(036364);\n",              // 02
     "SDR 49 TDI(006C6DC0C0C0C0);\n",      // 03
     "SDR 20 TDI(036363);\n",              // 04
     "SDR 49 TDI(006C6DB0B0B0B0);\n",      // 05
     "SDR 20 TDI(036362);\n",              // 06
     "SDR 49 TDI(006C6D0A0A0A0A);\n",      // 07
     "SDR 20 TDI(036361);\n",              // 08
     "SDR 49 TDI(006C6C09090909);\n",      // 09
     "SDR 49 TDI(006C6D40404040);\n",      // 10
     "SDR 20 TDI(036368);\n",              // 11
     "SDR 20 TDI(036360);\n",              // 15
     "SIR 8 TDI(FF);\n",                   // 16
  };

  TS_ASSERT_EQUALS (gotSvfCommands, expected);
}


//! Checks SystemModelManager DoDataCycles when using "MIB_Multichain_Pre" testcase
//!
void UT_SystemModelManager::test_DoDataCycles_MIB_Multichain_Pre ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  auto ai = Create_TestCase_MIB_Multichain_Pre(sm, false);

  auto spy = make_shared<Spy_AccessInterfaceProtocols>();
  ai->SetProtocol (spy);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  auto gotSutVectors = spy->ToSutVectors();

  std::vector<mast::BinaryVector> expected
  {
    BinaryVector::CreateFromString("0x02"),        // 00 : IR
    BinaryVector::CreateFromString("0x6060_6060"), // 01 : DR
    BinaryVector::CreateFromString("0x01"),        // 02 : IR
    BinaryVector::CreateFromString("0b11"),        // 03 : DR
    BinaryVector::CreateFromString("0x02"),        // 04 : IR
    BinaryVector::CreateFromString("0x6363_6363"), // 05 : DR
    BinaryVector::CreateFromString("0x01"),        // 06 : IR
    BinaryVector::CreateFromString("0b10"),        // 07 : DR
    BinaryVector::CreateFromString("0x02"),        // 08 : IR
    BinaryVector::CreateFromString("0x6262_6262"), // 09 : DR
    BinaryVector::CreateFromString("0x01"),        // 10 : IR
    BinaryVector::CreateFromString("0b01"),        // 11 : DR
    BinaryVector::CreateFromString("0x02"),        // 12 : IR
    BinaryVector::CreateFromString("0x6161_6161"), // 13 : DR
    BinaryVector::CreateFromString("0xFF"),        // 14 : IR
    BinaryVector::CreateFromString("0b1"),         // 15 : DR
    BinaryVector::CreateFromString("0x01"),        // 16 : IR
    BinaryVector::CreateFromString("0b00"),        // 17 : DR
    BinaryVector::CreateFromString("0xFF"),        // 18 : IR
  };

  TS_ASSERT_EQUALS (gotSutVectors, expected);
}



//! Checks SystemModelManager DoDataCycles when using "MIB_Multichain_Post" testcase
//!
void UT_SystemModelManager::test_DoDataCycles_MIB_Multichain_Post ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  auto ai = Create_TestCase_MIB_Multichain_Post(sm, false);

  auto spy = make_shared<Spy_AccessInterfaceProtocols>();
  ai->SetProtocol (spy);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  auto gotSutVectors = spy->ToSutVectors();

  std::vector<mast::BinaryVector> expected
  {
    BinaryVector::CreateFromString("0x02"),        // 00 : IR
    BinaryVector::CreateFromString("0b11"),        // 01 : DR
    BinaryVector::CreateFromString("0x01"),        // 02 : IR
    BinaryVector::CreateFromString("0x6363_6363"), // 03 : DR
    BinaryVector::CreateFromString("0x02"),        // 04 : IR
    BinaryVector::CreateFromString("0b10"),        // 05 : DR
    BinaryVector::CreateFromString("0x01"),        // 06 : IR
    BinaryVector::CreateFromString("0x6262_6262"), // 07 : DR
    BinaryVector::CreateFromString("0x02"),        // 08 : IR
    BinaryVector::CreateFromString("0b01"),        // 09 : DR
    BinaryVector::CreateFromString("0x01"),        // 10 : IR
    BinaryVector::CreateFromString("0x6161_6161"), // 11 : DR
    BinaryVector::CreateFromString("0x02"),        // 12 : IR
    BinaryVector::CreateFromString("0b00"),        // 13 : DR
    BinaryVector::CreateFromString("0x01"),        // 14 : IR
    BinaryVector::CreateFromString("0x6060_6060"), // 15 : DR
    BinaryVector::CreateFromString("0xFF"),        // 16 : IR
  };

  TS_ASSERT_EQUALS (gotSutVectors, expected);
}

//===========================================================================
// End of UT_SystemModelManager.cpp
//===========================================================================
