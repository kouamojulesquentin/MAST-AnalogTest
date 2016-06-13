//===========================================================================
//                           UT_DataCycle.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_DataCycle.cpp
//!
//! Implements test fixture for testing DataCycle
//!
//===========================================================================

#include "UT_DataCycle.hpp"

#include "SystemModelBuilder.hpp"
#include "GmlPrinterVisitor.hpp"
#include "ConfigureVisitor.hpp"
#include "ToSutVisitor.hpp"
#include "FromSutUpdater.hpp"
#include "DefaultBinaryPathSelector.hpp"

#include "BinaryVector_Traits.hpp"

using namespace mast;
using namespace test;
using std::make_shared;
using std::make_tuple;

namespace
{

//! Creates SystemModel structure for tests using "AccessInterface" scenario
//!
SystemModel Create_SimpleMIB ()
{
  SystemModel        sm;
  SystemModelBuilder builder(sm);

  auto root      = builder.Create_Simple_MIB();
  auto mib_Ctrl  = sm.RegisterWithId(3u);
  auto regStatic = sm.RegisterWithId(1u);
  auto regDyn_0  = sm.RegisterWithId(5u);
  auto regDyn_1  = sm.RegisterWithId(6u);
  auto regDyn_2  = sm.RegisterWithId(7u);
  auto regDyn_3  = sm.RegisterWithId(8u);

  mib_Ctrl->SetToSut   (BinaryVector::CreateFromBinaryString("11"));
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

  mib_Ctrl->SetPending();
  regStatic->SetPending();
  regDyn_0->SetPending();
  regDyn_1->SetPending();
  regDyn_2->SetPending();
  regDyn_3->SetPending();

//+  GmlPrinterVisitor gmlPrinter("Simple MIB", true, true, true);
//+  root->Accept(gmlPrinter);
//+  TS_ASSERT_EQUALS (gmlPrinter.Graph(), "");

  return sm;
}
//
//  End of: Create_SimpleMIB
//---------------------------------------------------------------------------
} // End of unnamed namespace

//! Initializes test (called for each test)
void UT_DataCycle::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}



//! Checks a serie of data cycle over a simple MIB model
//! @note The data cycle consist of "Configure system model", "Data to SUT" and "Update from SUT"
void UT_DataCycle::test_SimpleDataCycle_Without_ConfAlgo ()
{
  // ---------------- DDT Setup
  //
  auto sm = Create_SimpleMIB();
  auto root = sm.Root();


  ConfigureVisitor configurator(nullptr); // Do not use DefaultConfigurationAlgorithm
  ToSutVisitor     toSutVisitor;
  FromSutUpdater   fromSutUpdater(sm);

  auto checker = [&](const auto& data)
  {
    // ---------------- Exercise
    //
    root->Accept(configurator);

    toSutVisitor.Reset();
    root->Accept(toSutVisitor);
    const auto& identifiers = toSutVisitor.ActiveRegistersIdentifiers();
    const auto& toSutVector = toSutVisitor.ToSutVector();

    TS_ASSERT_THROWS_NOTHING (fromSutUpdater.UpdateRegisters(identifiers, toSutVector));

    // ---------------- Verify
    //
    auto expected = BinaryVector::CreateFromString(data);
    TS_ASSERT_EQUALS (toSutVector, expected);
  };

  auto data =
  {
    "0x0505:/b11:/x0909_0909",    // 00 Can send data from path 1 as it is alreay active
    "0x3636:/b11:/xC0C0_C0C0",    // 01 Still select path 3 because it has not been served (could be optimized because it is already selected)
    "0x3636:/b10:/x4343_4343",    // 02 Send path 3 bypass because it is active but not pending
    "0x3636:/b10:/xB0B0_B0B0",    // 03 Can send data from path 2 as it is now active...
    "0x3636:/b01:/x4242_4242",    // 04 Send path 2 bypass because it is active but not pending
    "0x3636:/b01:/x0A0A_0A0A",    // 05 Can send data from path 1 as it is now active...
    "0x3636:/b00:/x4141_4141",    // 06 Send path 1 bypass because it is active but not pending
    "0x3636:/b00:/x4040_4040",    // 07 Send path 0 bypass because it is active but not pending
    "0x3636:/b00:/x4040_4040",    // 08 No more changes
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);

  // ---------------- Verify final state
  //
  TS_ASSERT_FALSE (root->IsPending());  // No more changes expected

  auto reg_0   = sm.RegisterWithId(5u);
  auto reg_1   = sm.RegisterWithId(6u);
  auto reg_2   = sm.RegisterWithId(7u);
  auto reg_3   = sm.RegisterWithId(8u);

  TS_ASSERT_EQUALS (reg_0->LastFromSut(), reg_0->BypassSequence());
  TS_ASSERT_EQUALS (reg_1->LastFromSut(), reg_1->BypassSequence());
  TS_ASSERT_EQUALS (reg_2->LastFromSut(), reg_2->BypassSequence());
  TS_ASSERT_EQUALS (reg_3->LastFromSut(), reg_3->BypassSequence());
}


//! Checks a serie of data cycle over a simple MIB model
//! @note The data cycle consist of "Configure system model", "Data to SUT" and "Update from SUT"
void UT_DataCycle::test_SimpleDataCycle_With_DefaultConfAlgo ()
{
  // ---------------- DDT Setup
  //
  auto sm = Create_SimpleMIB();
  auto root = sm.Root();


  ConfigureVisitor configurator; // DO USE DefaultConfigurationAlgorithm
  ToSutVisitor     toSutVisitor;
  FromSutUpdater   fromSutUpdater(sm);

  auto checker = [&](const auto& data)
  {
    // ---------------- Exercise
    //
    root->Accept(configurator);

    toSutVisitor.Reset();
    root->Accept(toSutVisitor);
    const auto& identifiers = toSutVisitor.ActiveRegistersIdentifiers();
    const auto& toSutVector = toSutVisitor.ToSutVector();

    TS_ASSERT_THROWS_NOTHING (fromSutUpdater.UpdateRegisters(identifiers, toSutVector));

    // ---------------- Verify
    //
    auto expected = BinaryVector::CreateFromString(data);
    TS_ASSERT_EQUALS (toSutVector, expected);
  };

  auto data =
  {
    "0x0505:/b11:/x0909_0909",    // 00 Can send data from path 1 as it is alreay active
    "0x3636:/b10:/xC0C0_C0C0",    // 01 Can send data from path 4 as it is now active...
    "0x3636:/b01:/xB0B0_B0B0",    // 03 Can send data from path 3 as it is now active...
    "0x3636:/b01:/x0A0A_0A0A",    // 05 Can send data from path 2 as it is now active...
    "0x3636:/b00:/x4141_4141",    // 06 Send path 1 bypass because it is active but not pending
    "0x3636:/b00:/x4040_4040",    // 07 Send path 0 bypass because it is active but not pending
    "0x3636:/b00:/x4040_4040",    // 08 No more changes
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);

  // ---------------- Verify final state
  //
  TS_ASSERT_FALSE (root->IsPending());  // No more changes expected

  auto reg_0   = sm.RegisterWithId(5u);
  auto reg_1   = sm.RegisterWithId(6u);
  auto reg_2   = sm.RegisterWithId(7u);
  auto reg_3   = sm.RegisterWithId(8u);

  TS_ASSERT_EQUALS  (reg_0->LastFromSut(), reg_0->BypassSequence());
  TS_ASSERT_EQUALS  (reg_1->LastFromSut(), reg_1->BypassSequence());
  TS_ASSERT_DIFFERS (reg_2->LastFromSut(), reg_2->BypassSequence());
  TS_ASSERT_DIFFERS (reg_3->LastFromSut(), reg_3->BypassSequence());
}

//===========================================================================
// End of UT_DataCycle.cpp
//===========================================================================
