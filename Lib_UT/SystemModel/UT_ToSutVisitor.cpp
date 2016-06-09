//===========================================================================
//                           UT_ToSutVisitor.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_ToSutVisitor.cpp
//!
//! Implements test fixture for testing
//!
//===========================================================================


#include "UT_ToSutVisitor.hpp"
#include "ToSutVisitor.hpp"
#include "ConfigureVisitor.hpp"
#include "FromSutUpdater.hpp"
#include "SystemModelBuilder.hpp"
#include "DefaultBinaryPathSelector.hpp"

#include <cxxtest/ValueTraits.h>
using namespace mast;
using namespace test;
using std::make_shared;
using std::make_tuple;

namespace
{
//! Creates SystemModel structure for tests using "AccessInterface" scenario
//!
SystemModel CreateSystemModel_AccessInterface (uint8_t regValue)
{
  SystemModel        sm;
  SystemModelBuilder builder(sm);
  auto tap    = builder.Create_TestCase_AccessInterface("TAP");
  auto reg    = sm.RegisterWithId(7u);

  TS_ASSERT_NOT_NULLPTR (reg);

  reg->SetToSut(BinaryVector(STATIC_TDR_LEN, regValue));  // Make the register pending

  ConfigureVisitor configurator;
  tap->Accept(configurator);

  return sm;
}
//
//  End of: CreateSystemModel_AccessInterface
//---------------------------------------------------------------------------
} // End of unnamed namespace

//! Initializes test (called for each test)
void UT_ToSutVisitor::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}


//! Checks ToSutVisitor::Constructor()
//!
void UT_ToSutVisitor::test_Constructor ()
{
  // ---------------- Exercise
  //
  ToSutVisitor sut;

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (sut.ActiveRegistersIdentifiers().empty());
  TS_ASSERT_TRUE (sut.ToSutVector().IsEmpty());
}


//! Checks ToSutVisitor::Accept with a MIB that use binary encoding and can select no path
//!
void UT_ToSutVisitor::test_Accept_Mib_Binary_CanSelectNone ()
{
  // ---------------- DDT Setup
  //
  SystemModel sm;

  auto root         = sm.CreateChain    ("Sut");
  auto regStatic    = sm.CreateRegister ("static",    BinaryVector(STATIC_TDR_LEN),  root);
  auto mib          = sm.CreateChain    ("MIB",       root);
  auto reg_Ctrl     = sm.CreateRegister ("MIB_Ctrl",  BinaryVector(3U, 0u, SizeProperty::Fixed), true, mib);
  auto pathSelector = make_shared<DefaultBinaryPathSelector>(reg_Ctrl, 4u, false, true);
  auto mux          = sm.CreateLinker   ("MIB_Mux",   pathSelector,                  mib);
  auto regDyn_0     = sm.CreateRegister ("dynamic_0", BinaryVector(DYNAMIC_TDR_LEN), mux);
  auto regDyn_1     = sm.CreateRegister ("dynamic_1", BinaryVector(DYNAMIC_TDR_LEN), mux);
  auto regDyn_2     = sm.CreateRegister ("dynamic_2", BinaryVector(DYNAMIC_TDR_LEN), mux);
  auto regDyn_3     = sm.CreateRegister ("dynamic_3", BinaryVector(DYNAMIC_TDR_LEN), mux);

  regStatic->SetBypass (BinaryVector(STATIC_TDR_LEN,  0x05));
  regStatic->SetToSut  (BinaryVector(STATIC_TDR_LEN,  0x08)); regStatic->SetPending();
  regDyn_0->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x01)); regDyn_0->SetPending();
  regDyn_1->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x02)); regDyn_1->SetPending();
  regDyn_2->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x03)); regDyn_2->SetPending();
  regDyn_3->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x04)); regDyn_3->SetPending();

  ToSutVisitor sut;
  sut.IgnorePendingState(true);

  auto checker = [&](const auto& data)
  {
    // ---------------- Setup
    //
    reg_Ctrl->SetToSut  (BinaryVector::CreateFromBinaryString(std::get<0>(data)));
    reg_Ctrl->UpdateLastToSut();
    sut.Reset();

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (root->Accept(sut));

    // ---------------- Verify
    //
    auto expected = BinaryVector::CreateFromString(std::get<1>(data));
    TS_ASSERT_EQUALS (sut.ToSutVector(), expected);
  };

  auto data =
  {
    make_tuple("000", "0x0808:/b000"),
    make_tuple("001", "0x0808:/b001:/x0101_0101"),
    make_tuple("010", "0x0808:/b010:/x0202_0202"),
    make_tuple("011", "0x0808:/b011:/x0303_0303"),
    make_tuple("100", "0x0808:/b100:/x0404_0404"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks ToSutVisitor::Accept with a MIB that use binary encoding and cannot select no path
//!
void UT_ToSutVisitor::test_Accept_Mib_Binary_CannotSelectNone ()
{
  // ---------------- DDT Setup
  //
  SystemModel sm;

  auto root         = sm.CreateChain    ("Sut");
  auto regStatic    = sm.CreateRegister ("static",    BinaryVector(STATIC_TDR_LEN),  root);
  auto mib          = sm.CreateChain    ("MIB",       root);
  auto reg_Ctrl     = sm.CreateRegister ("MIB_Ctrl",  BinaryVector(2U, 0u, SizeProperty::Fixed), true, mib);
  auto pathSelector = make_shared<DefaultBinaryPathSelector>(reg_Ctrl, 4u);
  auto mux          = sm.CreateLinker   ("MIB_Mux",   pathSelector,                  mib);
  auto regDyn_0     = sm.CreateRegister ("dynamic_0", BinaryVector(DYNAMIC_TDR_LEN), mux);
  auto regDyn_1     = sm.CreateRegister ("dynamic_1", BinaryVector(DYNAMIC_TDR_LEN), mux);
  auto regDyn_2     = sm.CreateRegister ("dynamic_2", BinaryVector(DYNAMIC_TDR_LEN), mux);
  auto regDyn_3     = sm.CreateRegister ("dynamic_3", BinaryVector(DYNAMIC_TDR_LEN), mux);

  regStatic->SetBypass (BinaryVector(STATIC_TDR_LEN,  0x05));
  regStatic->SetToSut  (BinaryVector(STATIC_TDR_LEN,  0x08));  regStatic->SetPending();
  regDyn_0->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x01));  regDyn_0->SetPending();
  regDyn_1->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x02));  regDyn_1->SetPending();
  regDyn_2->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x03));  regDyn_2->SetPending();
  regDyn_3->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x04));  regDyn_3->SetPending();

  ToSutVisitor sut;
  sut.IgnorePendingState(true);

  auto checker = [&](const auto& data)
  {
    // ---------------- Setup
    //
    reg_Ctrl->SetToSut  (BinaryVector::CreateFromBinaryString(std::get<0>(data)));
    reg_Ctrl->UpdateLastToSut();
    sut.Reset();

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (root->Accept(sut));

    // ---------------- Verify
    //
    auto expected = BinaryVector::CreateFromString(std::get<1>(data));
    TS_ASSERT_EQUALS (sut.ToSutVector(), expected);
  };

  auto data =
  {
    make_tuple("00", "0x0808:/b00:/x0101_0101"),
    make_tuple("01", "0x0808:/b01:/x0202_0202"),
    make_tuple("10", "0x0808:/b10:/x0303_0303"),
    make_tuple("11", "0x0808:/b11:/x0404_0404"),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}



//! Checks ToSutVisitor::Accept_Testcase_1500()
//!
void UT_ToSutVisitor::test_Accept_Testcase_1500_Nothing_Pending ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);
  auto tap = builder.Create_TestCase_1500("TAP", 3u);

  ConfigureVisitor configurator;
  tap->Accept(configurator);

  ToSutVisitor sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (sut.ActiveRegistersIdentifiers().empty());
  TS_ASSERT_TRUE (sut.ToSutVector().IsEmpty());
}

//! Checks ToSutVisitor::Accept using Testcase_1500() when a nothing is pending but ToSutVisitor
//! set to ignore pending state
//!
void UT_ToSutVisitor::test_Accept_Testcase_1500_BypassMode_IgnorePending ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);
  auto tap = builder.Create_TestCase_1500("TAP", 3u);

  ToSutVisitor sut;
  sut.IgnorePendingState(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut));

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  const auto& identifiers = sut.ActiveRegistersIdentifiers();
  const auto& sutVector   = sut.ToSutVector();

  size_t   expectedActiveRegistersCount = 2u;
  uint32_t expectedBitsCount            = test::DEFAULT_IR_LEN + test::DEFAULT_BPY_LEN;         // Tap is still in bypass mode
  auto     expectedVector               = BinaryVector::CreateFromBinaryString("1111_1111:1");

  TS_ASSERT_EQUALS (identifiers.size(),    expectedActiveRegistersCount);
  TS_ASSERT_EQUALS (sutVector.BitsCount(), expectedBitsCount);
  TS_ASSERT_EQUALS (sutVector,             expectedVector);
}


//! Checks ToSutVisitor::Accept using Testcase_AccessInterface when a (single) register is pending
//!
//! Step 1: Request to select the branch where the register stands
//!
void UT_ToSutVisitor::test_Accept_Testcase_AccessInterface_1_Pending_Step_1 ()
{
  // ---------------- Setup
  //
  auto sm  = CreateSystemModel_AccessInterface(0x5A);
  auto tap = sm.Root();

  ToSutVisitor sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut));

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  const auto& identifiers = sut.ActiveRegistersIdentifiers();
  const auto& sutVector   = sut.ToSutVector();

  size_t   expectedActiveRegistersCount = 2u;
  uint32_t expectedBitsCount            = test::DEFAULT_IR_LEN + test::DEFAULT_BPY_LEN;
  auto     expectedVector               = BinaryVector::CreateFromBinaryString("0000_0010:1");  // Request to select 3rd path

  TS_ASSERT_EQUALS (identifiers.size(),    expectedActiveRegistersCount);
  TS_ASSERT_EQUALS (sutVector.BitsCount(), expectedBitsCount);
  TS_ASSERT_EQUALS (sutVector,             expectedVector);

  TS_ASSERT_TRUE   (tap->IsPending());
}


//! Checks ToSutVisitor::Accept using Testcase_AccessInterface when a (single) register is pending
//!
//! Step 2: The branch where the register stands should be active and the register value should be send to JTAG
//!
void UT_ToSutVisitor::test_Accept_Testcase_AccessInterface_1_Pending_Step_2 ()
{
  // ---------------- Setup
  //
  auto sm      = CreateSystemModel_AccessInterface(0x5A);
  auto tap     = sm.Root();
  auto updater = FromSutUpdater(sm);

  ToSutVisitor sut;
  // Step 1:
  tap->Accept(sut);
  updater.UpdateRegisters(sut.ActiveRegistersIdentifiers(), sut.ToSutVector());

  // Step 2:
  sut.Reset();
  ConfigureVisitor configurator;
  tap->Accept(configurator);

  // ---------------- Exercise (step 2)
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut));

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  const auto& identifiers = sut.ActiveRegistersIdentifiers();
  const auto& sutVector   = sut.ToSutVector();

  size_t   expectedActiveRegistersCount = 2u;
  uint32_t expectedBitsCount            = test::DEFAULT_IR_LEN + test::STATIC_TDR_LEN;
  auto     expectedVector               = BinaryVector::CreateFromHexString("02:5A5A");

  TS_ASSERT_EQUALS (identifiers.size(),    expectedActiveRegistersCount);
  TS_ASSERT_EQUALS (sutVector.BitsCount(), expectedBitsCount);
  TS_ASSERT_EQUALS (sutVector,             expectedVector);

  // Check that register value has been "served"
  updater.UpdateRegisters(identifiers, sutVector);
  tap->Accept(configurator);
  TS_ASSERT_FALSE  (tap->IsPending());
}


//! Checks ToSutVisitor::Reset()
//!
void UT_ToSutVisitor::test_Reset ()
{
  // ---------------- Setup
  //
  auto sm  = CreateSystemModel_AccessInterface(0x5A);
  auto tap = sm.Root();

  ToSutVisitor sut;
  tap->Accept(sut);

  // ---------------- Exercise
  //
  sut.Reset();

  // ---------------- Verify
  //
  const auto& identifiers = sut.ActiveRegistersIdentifiers();
  const auto& sutVector   = sut.ToSutVector();

  TS_ASSERT_TRUE (identifiers.empty());
  TS_ASSERT_TRUE (sutVector.IsEmpty());
}

//===========================================================================
// End of UT_ToSutVisitor.cpp
//===========================================================================
