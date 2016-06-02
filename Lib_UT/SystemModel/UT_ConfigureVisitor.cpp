//===========================================================================
//                           UT_ConfigureVisitor.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_ConfigureVisitor.cpp
//!
//! Implements test fixture for testing ConfigureVisitor
//!
//===========================================================================


#include "UT_ConfigureVisitor.hpp"
#include "ConfigureVisitor.hpp"
#include "SystemModelBuilder.hpp"

#include <cxxtest/ValueTraits.h>

using namespace mast;
using namespace test;


//! Initializes test (called for each test)
void UT_ConfigureVisitor::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}


//! Checks ConfigureVisitor::Constructor()
//!
void UT_ConfigureVisitor::test_Constructor ()
{
  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (ConfigureVisitor sut);
}



//! Checks ConfigureVisitor::Accept_Testcase_1500()
//!
void UT_ConfigureVisitor::test_Accept_Testcase_1500_Nothing_Pending ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);
  auto tap = builder.Create_TestCase_1500("TAP", 3u);

  ConfigureVisitor sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (tap->IsPending());
}


//! Checks ConfigureVisitor::Accept_Testcase_1500() when a (single) register is pending
//!
void UT_ConfigureVisitor::test_Accept_Testcase_1500_Register_1_Pending ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);

  auto tap    = builder.Create_TestCase_1500("TAP", 3u);
  auto reg    = sm.RegisterWithId(15u);
  auto linker = sm.LinkerWithId(12u);

  TS_ASSERT_NOT_NULLPTR (reg);
  TS_ASSERT_NOT_NULLPTR (linker);
  TS_ASSERT_FALSE       (linker->IsSelected(3u));

  reg->SetToSut(BinaryVector(DYNAMIC_TDR_LEN, 0x5A));  // Make the register pending
  ConfigureVisitor sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE  (tap->IsPending());
  TS_ASSERT_FALSE (linker->IsSelected(1u));
  TS_ASSERT_FALSE (linker->IsSelected(2u));
  TS_ASSERT_TRUE  (linker->IsSelected(3u));
  TS_ASSERT_FALSE (linker->IsSelected(4u));

  TS_ASSERT_TRUE  (linker->IsActive(1u));
  TS_ASSERT_FALSE (linker->IsActive(2u));
  TS_ASSERT_FALSE (linker->IsActive(3u));
  TS_ASSERT_FALSE (linker->IsActive(4u));
}


//! Checks ConfigureVisitor::Accept_Testcase_1500() when a (single) register is pending
//!
void UT_ConfigureVisitor::test_Accept_Testcase_1500_Register_2_Pending ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);

  auto tap    = builder.Create_TestCase_1500("TAP", 3u);
  auto reg_1  = sm.RegisterWithId(14u);
  auto reg_2  = sm.RegisterWithId(16u);
  auto linker = sm.LinkerWithId(12u);

  reg_1->SetToSut(BinaryVector(DYNAMIC_TDR_LEN, 0xA1));  // Make the register pending
  reg_2->SetToSut(BinaryVector(DYNAMIC_TDR_LEN, 0x52));  // Make the register pending
  ConfigureVisitor sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE  (tap->IsPending());
  TS_ASSERT_FALSE (linker->IsSelected(1u));
  TS_ASSERT_FALSE (linker->IsSelected(2u));
  TS_ASSERT_FALSE (linker->IsSelected(3u));
  TS_ASSERT_TRUE  (linker->IsSelected(4u));

  TS_ASSERT_TRUE  (linker->IsActive(1u));
  TS_ASSERT_FALSE (linker->IsActive(2u));
  TS_ASSERT_FALSE (linker->IsActive(3u));
  TS_ASSERT_FALSE (linker->IsActive(4u));
}


//===========================================================================
// End of UT_ConfigureVisitor.cpp
//===========================================================================
