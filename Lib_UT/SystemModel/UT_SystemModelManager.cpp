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




//===========================================================================
// End of UT_SystemModelManager.cpp
//===========================================================================
