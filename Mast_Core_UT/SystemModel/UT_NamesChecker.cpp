//===========================================================================
//                           UT_NamesChecker.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_NamesChecker.cpp
//!
//! Implements test fixture for testing NamesChecker
//!
//===========================================================================


#include "UT_NamesChecker.hpp"
#include "NamesChecker.hpp"

#include "TestModelBuilder.hpp"
#include "GmlPrinter.hpp"
#include "SystemModelCheckResult_Traits.hpp"

#include <cxxtest/ValueTraits.h>


using std::string;
using std::make_shared;
using std::dynamic_pointer_cast;

using namespace mast;
using namespace test;

using mast::NamesChecker;



//! Initializes test (called for each test)
void UT_NamesChecker::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}

//! Checks NamesChecker::Check() when two Register share the same name
//!
void UT_NamesChecker::test_Check_SameNames_Only_Regs ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("Tap", 8u, 4u);
  auto reg_1 = sm.CreateRegister("reg",   BinaryVector::CreateFromBinaryString("01"), tap);
  auto reg_2 = sm.CreateRegister("reg_2", BinaryVector::CreateFromBinaryString("10"), tap);
  auto reg_3 = sm.CreateRegister("reg",   BinaryVector::CreateFromBinaryString("11"), tap);

  // ---------------- Exercise
  //
  auto result = NamesChecker::Check(tap);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (result.infosCount,    0u);
  TS_ASSERT_EQUALS (result.warningsCount, 0u);
  TS_ASSERT_EQUALS (result.errorsCount,   1u);
//+  TS_ASSERT_EMPTY (result.MakeReport());
}


//! Checks NamesChecker::Check() when two ParentNode share the same name
//!
void UT_NamesChecker::test_Check_SameNames_Only_Parents ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("Tap", 8u, 4u);
  auto chain_1 = sm.CreateChain("chain",   tap);
  auto chain_2 = sm.CreateChain("chain_2", tap);
  auto chain_3 = sm.CreateChain("chain",   tap);

  // ---------------- Exercise
  //
  auto result = NamesChecker::Check(tap);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (result.infosCount,    0u);
  TS_ASSERT_EQUALS (result.warningsCount, 0u);
  TS_ASSERT_EQUALS (result.errorsCount,   1u);  // For same name
}


//! Checks NamesChecker::Check() when one ParentNode and a Register share the same name
//!
void UT_NamesChecker::test_Check_SameNames_Mix_Regs_and_Parents ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap     = builder.Create_JTAG_TAP("Tap", 8u, 4u);
  auto reg     = sm.CreateRegister("Foo", BinaryVector::CreateFromBinaryString("01"), tap);
  auto chain_1 = sm.CreateChain(   "Bar", tap);
  auto chain_2 = sm.CreateChain(   "Foo", tap);

  // ---------------- Exercise
  //
  auto result = NamesChecker::Check(tap);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (result.infosCount,    0u);
  TS_ASSERT_EQUALS (result.warningsCount, 0u);
  TS_ASSERT_EQUALS (result.errorsCount,   1u);
}


//! Checks NamesChecker::Check() when two ParentNode share the same name but are ignored
//!
void UT_NamesChecker::test_Check_SameNames_Only_Ignored ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("Tap", 8u, 4u);
  auto chain_1 = sm.CreateChain("chain",   tap); chain_1->IgnoreForNodePath(true);
  auto chain_2 = sm.CreateChain("chain_2", tap); chain_2->IgnoreForNodePath(true);
  auto chain_3 = sm.CreateChain("chain",   tap); chain_3->IgnoreForNodePath(true);

  // ---------------- Exercise
  //
  auto result = NamesChecker::Check(tap);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (result.infosCount,    1u);  // For same name but ignored
  TS_ASSERT_EQUALS (result.warningsCount, 0u);
  TS_ASSERT_EQUALS (result.errorsCount,   0u);
}


//! Checks NamesChecker::Check() when two ParentNode share the same name and 1st one is ignored
//!
void UT_NamesChecker::test_Check_SameNames_Some_Ignored_1 ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("Tap", 8u, 4u);
  auto chain_1 = sm.CreateChain("chain",   tap); chain_1->IgnoreForNodePath(true);
  auto chain_2 = sm.CreateChain("chain_2", tap);
  auto chain_3 = sm.CreateChain("chain",   tap);

  // ---------------- Exercise
  //
  auto result = NamesChecker::Check(tap);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (result.infosCount,    1u);  // For same name but ignored
  TS_ASSERT_EQUALS (result.warningsCount, 0u);
  TS_ASSERT_EQUALS (result.errorsCount,   0u);
}


//! Checks NamesChecker::Check() when two ParentNode share the same name and 2nd one is ignored
//!
void UT_NamesChecker::test_Check_SameNames_Some_Ignored_2 ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("Tap", 8u, 4u);
  auto chain_1 = sm.CreateChain("chain",   tap);
  auto chain_2 = sm.CreateChain("chain_2", tap);
  auto chain_3 = sm.CreateChain("chain",   tap); chain_3->IgnoreForNodePath(true);

  // ---------------- Exercise
  //
  auto result = NamesChecker::Check(tap);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (result.infosCount,    1u);  // For same name but ignored
  TS_ASSERT_EQUALS (result.warningsCount, 0u);
  TS_ASSERT_EQUALS (result.errorsCount,   0u);
}


//! Checks NamesChecker::Check() when a Register has same name as a following ParentNode that is ignored for node paths
//!
void UT_NamesChecker::test_Check_SameNames_Mix_Regs_and_Ignored_1 ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("Tap", 8u, 4u);
  auto reg     = sm.CreateRegister("Foo", BinaryVector::CreateFromBinaryString("01"), tap);
  auto chain_1 = sm.CreateChain("Bar", tap);
  auto chain_2 = sm.CreateChain("Foo", tap); chain_2->IgnoreForNodePath(true);

  // ---------------- Exercise
  //
  auto result = NamesChecker::Check(tap);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (result.infosCount,    1u);  // For same name but ignored
  TS_ASSERT_EQUALS (result.warningsCount, 0u);
  TS_ASSERT_EQUALS (result.errorsCount,   0u);
}


//! Checks NamesChecker::Check() when a Register has same name as a previous ParentNode that is ignored for node paths
//!
void UT_NamesChecker::test_Check_SameNames_Mix_Regs_and_Ignored_2 ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("Tap", 8u, 4u);
  auto chain_1 = sm.CreateChain("Foo", tap); chain_1->IgnoreForNodePath(true);
  auto chain_2 = sm.CreateChain("Bar", tap);
  auto reg     = sm.CreateRegister("Foo", BinaryVector::CreateFromBinaryString("01"), tap);

  // ---------------- Exercise
  //
  auto result = NamesChecker::Check(tap);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (result.infosCount,    1u);  // For same name but ignored
  TS_ASSERT_EQUALS (result.warningsCount, 0u);
  TS_ASSERT_EQUALS (result.errorsCount,   0u);
//+  TS_ASSERT_EMPTY (result.MakeReport());
}


//! Checks NamesChecker::Check() when a register has no valid name (cannot be identified by path)
//!
void UT_NamesChecker::test_Check_NoName_Reg ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("Tap", 8u, 4u);
  auto reg_1 = sm.CreateRegister("reg_1", BinaryVector::CreateFromBinaryString("01"), tap);
  auto reg_2 = sm.CreateRegister("reg_2", BinaryVector::CreateFromBinaryString("10"), tap);
  auto reg_3 = sm.CreateRegister("",      BinaryVector::CreateFromBinaryString("11"), tap);

  // ---------------- Exercise
  //
  auto result = NamesChecker::Check(tap);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (result.infosCount,    1u);  // This is just an info because this is not mandatory to be identified by path
  TS_ASSERT_EQUALS (result.warningsCount, 0u);
  TS_ASSERT_EQUALS (result.errorsCount,   0u);
}


//! Checks NamesChecker::Check() when a ParentNode has no valid name and not ignored for node paths (cannot be identified by path)
//!
void UT_NamesChecker::test_Check_NoName_Parent ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("Tap", 8u, 4u);
  auto reg_1 = sm.CreateRegister("Foo", BinaryVector::CreateFromBinaryString("01"), tap);
  auto reg_2 = sm.CreateRegister("Bar", BinaryVector::CreateFromBinaryString("01"), tap);
  auto chain = sm.CreateChain("",    tap);
  auto reg_3 = sm.CreateRegister("Bar", BinaryVector::CreateFromBinaryString("11"), chain);

  // ---------------- Exercise
  //
  auto result = NamesChecker::Check(tap);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (result.infosCount,    0u);
  TS_ASSERT_EQUALS (result.warningsCount, 1u);
  TS_ASSERT_EQUALS (result.errorsCount,   0u);
//+  TS_ASSERT_EMPTY (result.MakeReport());
}


//! Checks NamesChecker::Check() when a ParentNode has no valid name but ignored for node paths (cannot be identified by path)
//!
void UT_NamesChecker::test_Check_NoName_Ignored ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("Tap", 8u, 4u);
  auto reg_1 = sm.CreateRegister("Foo", BinaryVector::CreateFromBinaryString("01"), tap);
  auto reg_2 = sm.CreateRegister("Bar", BinaryVector::CreateFromBinaryString("01"), tap);
  auto chain = sm.CreateChain   ("",    tap);  chain->IgnoreForNodePath(true);
  auto reg_3 = sm.CreateRegister("Bit", BinaryVector::CreateFromBinaryString("11"), chain);

  // ---------------- Exercise
  //
  auto result = NamesChecker::Check(tap);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (result.infosCount,    0u);
  TS_ASSERT_EQUALS (result.warningsCount, 0u);
  TS_ASSERT_EQUALS (result.errorsCount,   0u);
//+  TS_ASSERT_EMPTY (result.MakeReport());
}


//! Checks NamesChecker::Check() when one Register downward the hierarchy has same name as another Register
//!
void UT_NamesChecker::test_Check_SamePath_Downward_2_Regs ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap     = builder.Create_JTAG_TAP("Tap", 8u, 4u);

  auto reg_1   = sm.CreateRegister ("Foo",  BinaryVector::CreateFromBinaryString("01"), tap);
  auto chain_1 = sm.CreateChain    ("Bar",  tap);  chain_1->IgnoreForNodePath(true);
  auto chain_2 = sm.CreateChain    ("Food", tap);
  auto reg_2   = sm.CreateRegister ("Foo",  BinaryVector::CreateFromBinaryString("10"), chain_1);

  // ---------------- Exercise
  //
  auto result = NamesChecker::Check(tap);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (result.infosCount,    0u);
  TS_ASSERT_EQUALS (result.warningsCount, 0u);
  TS_ASSERT_EQUALS (result.errorsCount,   1u);
//+  TS_ASSERT_EMPTY (result.MakeReport());
}


//! Checks NamesChecker::Check() when 2 Registers downward the hierarchy has same name as another Register
//!
void UT_NamesChecker::test_Check_SamePath_Downward_3_Regs ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap     = builder.Create_JTAG_TAP("Tap", 8u, 4u);

  auto reg_1   = sm.CreateRegister ("Foo",  BinaryVector::CreateFromBinaryString("01"), tap);
  auto chain_1 = sm.CreateChain    ("Bar",  tap);      chain_1->IgnoreForNodePath(true);
  auto chain_2 = sm.CreateChain    ("Bat",  chain_1);  chain_2->IgnoreForNodePath(true);
  auto chain_3 = sm.CreateChain    ("Bit",  tap);
  auto reg_2   = sm.CreateRegister ("Foo",  BinaryVector::CreateFromBinaryString("10"), chain_1);
  auto reg_3   = sm.CreateRegister ("Foo",  BinaryVector::CreateFromBinaryString("11"), chain_2);

  // ---------------- Exercise
  //
  auto result = NamesChecker::Check(tap);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (result.infosCount,    0u);
  TS_ASSERT_EQUALS (result.warningsCount, 0u);
  TS_ASSERT_EQUALS (result.errorsCount,   2u);
//+  TS_ASSERT_EMPTY (result.MakeReport());
}


//! Checks NamesChecker::Check() when one Register on a side hierarchy has same name as another Register
//!
void UT_NamesChecker::test_Check_SamePath_Sideward_2_Regs ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap     = builder.Create_JTAG_TAP("Tap", 8u, 4u);

  auto chain_1 = sm.CreateChain    ("Bar",  tap); chain_1->IgnoreForNodePath(true);
  auto chain_2 = sm.CreateChain    ("Food", tap); chain_2->IgnoreForNodePath(true);
  auto reg_1   = sm.CreateRegister ("Foo",  BinaryVector::CreateFromBinaryString("01"), chain_1);
  auto reg_2   = sm.CreateRegister ("Foo",  BinaryVector::CreateFromBinaryString("10"), chain_2);

  // ---------------- Exercise
  //
  auto result = NamesChecker::Check(tap);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (result.infosCount,    0u);
  TS_ASSERT_EQUALS (result.warningsCount, 0u);
  TS_ASSERT_EQUALS (result.errorsCount,   1u);
}


//! Checks NamesChecker::Check() when one Register on a side hierarchy has same name as another Register
//!
void UT_NamesChecker::test_Check_SamePath_Sideward_3_Regs ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap     = builder.Create_JTAG_TAP("Tap", 8u, 4u);

  auto chain_1 = sm.CreateChain    ("Bar",      tap); chain_1->IgnoreForNodePath(true);
  auto chain_2 = sm.CreateChain    ("Mood",     tap);
  auto chain_3 = sm.CreateChain    ("VAT",      tap); chain_3->IgnoreForNodePath(true);
  auto chain_4 = sm.CreateChain    ("Mood",     chain_1);
  auto reg_1   = sm.CreateRegister ("Foo",      BinaryVector::CreateFromBinaryString("001"), chain_1);
  auto reg_2   = sm.CreateRegister ("Foo",      BinaryVector::CreateFromBinaryString("010"), chain_2);
  auto reg_3   = sm.CreateRegister ("Mood.Foo", BinaryVector::CreateFromBinaryString("011"), chain_3);
  auto reg_4   = sm.CreateRegister ("Foo",      BinaryVector::CreateFromBinaryString("100"), chain_4);

  // ---------------- Exercise
  //
  auto result = NamesChecker::Check(tap);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (result.infosCount,    0u);
  TS_ASSERT_EQUALS (result.warningsCount, 0u);
  TS_ASSERT_EQUALS (result.errorsCount,   2u);
//+  TS_ASSERT_EMPTY (result.MakeReport());
}


//===========================================================================
// End of UT_NamesChecker.cpp
//===========================================================================
