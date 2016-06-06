//===========================================================================
//                           UT_FromSutUpdater.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_FromSutUpdater.cpp
//!
//! Implements test fixture for testing FromSutUpdater
//!
//===========================================================================

#include "UT_FromSutUpdater.hpp"
#include "FromSutUpdater.hpp"
#include "SystemModel.hpp"
#include "BinaryVector_Traits.hpp"

#include <vector>

using std::vector;
using namespace mast;

//! Initializes test (called for each test)
void UT_FromSutUpdater::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}



//! Checks FromSutUpdater constructor()
//!
void UT_FromSutUpdater::test_Constructor ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (FromSutUpdater sut(sm));
}

//! Checks FromSutUpdater::UpdateRegisters() when there is nothing to update
//!
void UT_FromSutUpdater::test_UpdateRegisters_0 ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto tap      = sm.CreateTap("", 8u, 2u);
  auto reg      = sm.CreateRegister("Reg", BinaryVector::CreateFromBinaryString("1100"), tap);
  auto newValue = BinaryVector(0);

  vector<SystemModelNode::NodeIdentifier> ids;

  FromSutUpdater sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.UpdateRegisters(ids, newValue));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (reg->LastFromSut(), reg->BypassSequence());
}


//! Checks FromSutUpdater::UpdateRegisters() when there is a single register to update
//!
void UT_FromSutUpdater::test_UpdateRegisters_1 ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto tap      = sm.CreateTap("", 8u, 2u);
  auto reg      = sm.CreateRegister("Reg", BinaryVector::CreateFromBinaryString("1100"), tap);
  auto newValue = BinaryVector::CreateFromBinaryString("0011");

  vector<SystemModelNode::NodeIdentifier> ids({ reg->Identifier() });

  FromSutUpdater sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.UpdateRegisters(ids, newValue));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (reg->LastFromSut(), newValue);
  TS_ASSERT_EQUALS (reg->LastToSut(),   reg->NextToSut());
}





//! Checks FromSutUpdater::UpdateRegisters() when there are two registers to update
//!
void UT_FromSutUpdater::test_UpdateRegisters_2 ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto tap   = sm.CreateTap("", 8u, 2u);
  auto reg_1 = sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("0100_1"),  tap);
  auto reg_2 = sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("1000_10"), tap);

  auto newValue_1 = BinaryVector::CreateFromBinaryString("0101_1");
  auto newValue_2 = BinaryVector::CreateFromBinaryString("1001_11");
  auto fromVector = newValue_1 + newValue_2;

  vector<SystemModelNode::NodeIdentifier> ids(
  {
    reg_1->Identifier(),
    reg_2->Identifier(),
  });

  FromSutUpdater sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.UpdateRegisters(ids, fromVector));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (reg_1->LastFromSut(), newValue_1);
  TS_ASSERT_EQUALS (reg_2->LastFromSut(), newValue_2);
  TS_ASSERT_EQUALS (reg_1->LastToSut(),   reg_1->NextToSut());
  TS_ASSERT_EQUALS (reg_2->LastToSut(),   reg_2->NextToSut());
}


//! Checks FromSutUpdater::UpdateRegisters() when there are 3 registers to update
//!
void UT_FromSutUpdater::test_UpdateRegisters_3 ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto tap   = sm.CreateTap("", 8u, 2u);
  auto reg_1 = sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("0100_1"),  tap);
  auto reg_2 = sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("1000_10"), tap);
  auto reg_3 = sm.CreateRegister("Reg_3", BinaryVector::CreateFromBinaryString("1110_110"), tap);

  auto newValue_1 = BinaryVector::CreateFromBinaryString("0101_1");
  auto newValue_2 = BinaryVector::CreateFromBinaryString("1001_11");
  auto newValue_3 = BinaryVector::CreateFromBinaryString("1111_111");
  auto fromVector = newValue_1 + newValue_2 + newValue_3;

  vector<SystemModelNode::NodeIdentifier> ids(
  {
    reg_1->Identifier(),
    reg_2->Identifier(),
    reg_3->Identifier(),
  });

  FromSutUpdater sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.UpdateRegisters(ids, fromVector));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (reg_1->LastFromSut(), newValue_1);
  TS_ASSERT_EQUALS (reg_2->LastFromSut(), newValue_2);
  TS_ASSERT_EQUALS (reg_3->LastFromSut(), newValue_3);
}


//! Checks FromSutUpdater::UpdateRegisters() when vector from JTAG is too small
//!
void UT_FromSutUpdater::test_UpdateRegisters_Vector_TooSmall ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto tap   = sm.CreateTap("", 8u, 2u);
  auto reg_1 = sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("0100_1"),  tap);
  auto reg_2 = sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("1000_10"), tap);

  auto fromVector = BinaryVector::CreateFromBinaryString("0101_1:1001_1"); // Missing one bit !

  vector<SystemModelNode::NodeIdentifier> ids(
  {
    reg_1->Identifier(),
    reg_2->Identifier(),
  });

  FromSutUpdater sut(sm);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.UpdateRegisters(ids, fromVector), std::exception);
}


//! Checks FromSutUpdater::UpdateRegisters() when vector from JTAG is too large
//!
void UT_FromSutUpdater::test_UpdateRegisters_Vector_TooLarge ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto tap   = sm.CreateTap("", 8u, 2u);
  auto reg_1 = sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("0100_1"),  tap);
  auto reg_2 = sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("1000_10"), tap);

  auto fromVector = BinaryVector::CreateFromBinaryString("0101_1:1001_11:0"); // One too much bit !

  vector<SystemModelNode::NodeIdentifier> ids(
  {
    reg_1->Identifier(),
    reg_2->Identifier(),
  });

  FromSutUpdater sut(sm);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.UpdateRegisters(ids, fromVector), std::exception);
}


//! Checks FromSutUpdater::UpdateRegisters() when trying to update in a wrong order
//!
void UT_FromSutUpdater::test_UpdateRegisters_Vector_WrongOrder ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto tap   = sm.CreateTap("", 8u, 2u);
  auto reg_1 = sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("0100_1"),   tap);
  auto reg_2 = sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("1000_10"),  tap);
  auto reg_3 = sm.CreateRegister("Reg_3", BinaryVector::CreateFromBinaryString("1110_110"), tap);

  auto newValue_1 = BinaryVector::CreateFromBinaryString("0101_1");
  auto newValue_2 = BinaryVector::CreateFromBinaryString("1001_11");
  auto newValue_3 = BinaryVector::CreateFromBinaryString("1111_111");

  auto fromVector = newValue_1 + newValue_2 + newValue_3;

  vector<SystemModelNode::NodeIdentifier> ids(
  {
    reg_1->Identifier(),
    reg_3->Identifier(),    // Wrong order
    reg_2->Identifier(),
  });

  FromSutUpdater sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.UpdateRegisters(ids, fromVector));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS  (reg_1->LastFromSut(), newValue_1);
  TS_ASSERT_DIFFERS (reg_2->LastFromSut(), newValue_2);
  TS_ASSERT_DIFFERS (reg_3->LastFromSut(), newValue_3);
}




//===========================================================================
// End of UT_FromSutUpdater.cpp
//===========================================================================
