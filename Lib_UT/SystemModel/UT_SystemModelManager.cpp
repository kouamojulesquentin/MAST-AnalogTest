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
#include "GmlPrinterVisitor.hpp"
#include "BinaryVector_Traits.hpp"

using namespace mast;
using namespace test;
using std::make_shared;
using std::make_tuple;


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

//+  GmlPrinterVisitor gmlPrinter("Testcase_AccessInterface", true, true, true);
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
    BinaryVector::CreateFromString("/b1"),    // 05 : DR (BPY)
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

  auto spy = make_shared<Spy_AccessInterfaceProtocols>();
  ai->SetProtocol (spy);

//+  GmlPrinterVisitor gmlPrinter("Testcase_1500", true, true, true);
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
    BinaryVector::CreateFromString("/x01"),                  // 00 : IR
    BinaryVector::CreateFromString("/x0505/b00:100"),        // 01 : DR
    BinaryVector::CreateFromString("/x01"),                  // 02 : IR
    BinaryVector::CreateFromString("/x3636/b01/xC0C0_C0C0"), // 03 : DR
    BinaryVector::CreateFromString("/x01"),                  // 04 : IR
    BinaryVector::CreateFromString("/x3636/b00:011"),        // 05 : DR
    BinaryVector::CreateFromString("/x01"),                  // 06 : IR
    BinaryVector::CreateFromString("/x3636/b01/xB0B0_B0B0"), // 07 : DR
    BinaryVector::CreateFromString("/x01"),                  // 08 : IR
    BinaryVector::CreateFromString("/x3636/b00:010"),        // 09 : DR
    BinaryVector::CreateFromString("/x01"),                  // 10 : IR
    BinaryVector::CreateFromString("/x3636/b01/x0A0A_0A0A"), // 11 : DR
    BinaryVector::CreateFromString("/x01"),                  // 12 : IR
    BinaryVector::CreateFromString("/x3636/b00:001"),        // 13 : DR
    BinaryVector::CreateFromString("/x01"),                  // 14 : IR
    BinaryVector::CreateFromString("/x3636/b00/x0909_0909"), // 15 : DR
    BinaryVector::CreateFromString("/x01"),                  // 16 : IR
    BinaryVector::CreateFromString("/x3636/b01/x4040_4040"), // 17 : DR
    BinaryVector::CreateFromString("/x01"),                  // 18 : IR
    BinaryVector::CreateFromString("/x3636/b01:000"),        // 19 : DR
    BinaryVector::CreateFromString("/xFF"),                  // 20 : IR
    BinaryVector::CreateFromString("/b1"),                   // 21 : DR
    BinaryVector::CreateFromString("/x01"),                  // 22 : IR
    BinaryVector::CreateFromString("/x3636/b00:000"),        // 23 : DR
    BinaryVector::CreateFromString("/xFF"),                  // 24 : IR
    BinaryVector::CreateFromString("/b1"),                   // 25 : DR
  };

  TS_ASSERT_EQUALS (gotSutVectors, expected);
}





//===========================================================================
// End of UT_SystemModelManager.cpp
//===========================================================================
