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

#include <cxxtest/ValueTraits.h>

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
}


//===========================================================================
// End of UT_SystemModelCheckerVisitor.cpp
//===========================================================================
