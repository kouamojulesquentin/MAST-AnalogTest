//===========================================================================
//                           UT_SystemModelCheckerVisitor.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SystemModelCheckerVisitor.cpp
//!
//! Implements test fixture for testing SystemModelCheckerVisitor
//!
//===========================================================================


#include "UT_SystemModelCheckerVisitor.hpp"
#include "SystemModelCheckerVisitor.hpp"

#include "SystemModelCheckResult_Traits.hpp"

using std::string;
using namespace mast;


//! Initializes test (called for each test)
//!
void UT_SystemModelCheckerVisitor::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}



//! Checks SystemModelCheckerVisitor::Constructor() with an empty model
//!
void UT_SystemModelCheckerVisitor::test_Constructor_From_EmptyModel ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (SystemModelCheckerVisitor sut(sm));
}

//! Checks SystemModelCheckerVisitor::Constructor() from a model with newly created tap
//!
void UT_SystemModelCheckerVisitor::test_Constructor_From_ModelWithTap ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  sm.CreateTap("", 6u, 4u);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (SystemModelCheckerVisitor sut(sm));
}


//! Checks SystemModelCheckerVisitor::CheckIdentifiers() with newly created tap
//!
void UT_SystemModelCheckerVisitor::test_CheckIdentifiers_With_ModelWithTap ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  sm.CreateTap("", 6u, 4u);

  SystemModelCheckerVisitor sut(sm);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckIdentifiers());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_FALSE (result.HasIssues());

  auto   report         = result.MakeReport();
  string expectedReport = "Errors   (0):\n"
                          "Warnings (0):\n"
                          "Infos    (0):\n";

  TS_ASSERT_EQUALS (report, expectedReport);
  TS_ASSERT_EQUALS (result, SystemModelCheckResult::None);
}


//! Checks SystemModelCheckerVisitor::CheckIdentifiers() with unused identifier
//!
void UT_SystemModelCheckerVisitor::test_CheckIdentifiers_With_UnusedIdentifier ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto tap   = sm.CreateTap("", 6u, 4u);
  auto reg_1 = sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("1100_111"), tap);
  auto reg_2 = sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("111"),      tap);

  sm.RemoveNodeFromModel(reg_1);
  SystemModelCheckerVisitor sut(sm);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckIdentifiers());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_FALSE   (result.HasIssues());
  TS_ASSERT_DIFFERS (result, SystemModelCheckResult::None);

  auto   report         = result.MakeReport();
  string expectedReport = "Errors   (0):\n"
                          "Warnings (0):\n"
                          "Infos    (1):\n"
                          "  - Identifier '4' is not used\n";

  TS_ASSERT_EQUALS (report, expectedReport);
}


//===========================================================================
// End of UT_SystemModelCheckerVisitor.cpp
//===========================================================================
