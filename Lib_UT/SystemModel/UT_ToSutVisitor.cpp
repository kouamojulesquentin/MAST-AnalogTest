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
#include "SystemModelBuilder.hpp"

#include <cxxtest/ValueTraits.h>
using namespace mast;
using namespace test;


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

//! Checks ToSutVisitor::Accept using Testcase_AccessInterface when a (single) register is pending
//!
void UT_ToSutVisitor::test_Accept_Testcase_AccessInterface_1_Pending_Register ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);
  auto tap    = builder.Create_TestCase_AccessInterface("TAP");
  auto reg    = sm.RegisterWithId(7u);

  TS_ASSERT_NOT_NULLPTR (reg);

  reg->SetToSut(BinaryVector(STATIC_TDR_LEN, 0x5A));  // Make the register pending

  ConfigureVisitor configurator;
  tap->Accept(configurator);

  ToSutVisitor sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut));

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  const auto& identifiers = sut.ActiveRegistersIdentifiers();
  const auto& sutVector   = sut.ToSutVector();

  size_t   expectedActiveRegistersCount = 1u;
  uint32_t expectedBitsCount            = test::DEFAULT_IR_LEN + test::DEFAULT_BPY_LEN + test::STATIC_TDR_LEN;

  TS_ASSERT_EQUALS (identifiers.size(),    expectedActiveRegistersCount);
  TS_ASSERT_EQUALS (sutVector.BitsCount(), expectedBitsCount);
}

//! Checks ToSutVisitor::Accept using Testcase_1500() when a (single) register is pending
//!
void UT_ToSutVisitor::test_Accept_Testcase_1500_Register_1_Pending ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);
  auto tap    = builder.Create_TestCase_1500("TAP", 3u);
  auto reg    = sm.RegisterWithId(15u);

  TS_ASSERT_NOT_NULLPTR (reg);

  reg->SetToSut(BinaryVector(DYNAMIC_TDR_LEN, 0x5A));  // Make the register pending

  ConfigureVisitor configurator;
  tap->Accept(configurator);

  ToSutVisitor sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut));

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  const auto& identifiers = sut.ActiveRegistersIdentifiers();
  const auto& sutVector   = sut.ToSutVector();

  size_t   expectedActiveRegistersCount = 1u;
  uint32_t expectedBitsCount            = test::DYNAMIC_TDR_LEN;

  TS_ASSERT_EQUALS (identifiers.size(),    expectedActiveRegistersCount);
  TS_ASSERT_EQUALS (sutVector.BitsCount(), expectedBitsCount);
}



//! Checks ToSutVisitor::Reset()
//!
void UT_ToSutVisitor::test_Reset ()
{
  // ---------------- Setup
  //
  ToSutVisitor sut;

  // ---------------- Exercise
  //
  sut.Reset();

  // ---------------- Verify
  //
}

//===========================================================================
// End of UT_ToSutVisitor.cpp
//===========================================================================
