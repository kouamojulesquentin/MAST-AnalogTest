//===========================================================================
//                           UT_SystemModelChecker.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SystemModelChecker.cpp
//!
//! Implements test fixture for testing SystemModelChecker
//!
//===========================================================================


#include "UT_SystemModelChecker.hpp"
#include "SystemModelChecker.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "SVF_SimulationProtocol.hpp"
#include "Spy_AccessInterfaceProtocols.hpp"
#include "TestModelBuilder.hpp"
#include "GmlPrinter.hpp"
#include "SystemModelCheckResult_Traits.hpp"


using std::string;
using std::make_shared;
using std::dynamic_pointer_cast;
using namespace mast;
using namespace test;


//! Initializes test (called for each test)
//!
void UT_SystemModelChecker::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}



//! Checks SystemModelChecker::Constructor() with an empty model
//!
void UT_SystemModelChecker::test_Constructor_From_EmptyModel ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (SystemModelChecker sut(sm));
}

//! Checks SystemModelChecker::Constructor() from a model with newly created tap
//!
void UT_SystemModelChecker::test_Constructor_From_ModelWithTap ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  builder.Create_JTAG_TAP("", 6u, 4u);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (SystemModelChecker sut(sm));
}


//! Checks SystemModelChecker::CheckIdentifiers() with empty model
//!
void UT_SystemModelChecker::test_CheckIdentifiers_With_EmptyModel ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  SystemModelChecker sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckIdentifiers());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_FALSE  (result.HasIssues());
  TS_ASSERT_EQUALS (result, SystemModelCheckResult::None);

  auto   report         = result.MakeReport();
  string expectedReport = "Errors   (0):\n"
                          "Warnings (0):\n"
                          "Infos    (0):\n";

  TS_ASSERT_EQUALS (report, expectedReport);
}


//! Checks SystemModelChecker::CheckIdentifiers() with newly created tap
//!
void UT_SystemModelChecker::test_CheckIdentifiers_With_ModelWithTap ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  builder.Create_JTAG_TAP("", 6u, 4u);

  SystemModelChecker sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckIdentifiers());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_FALSE  (result.HasIssues());
  TS_ASSERT_EQUALS (result, SystemModelCheckResult::None);
}


//! Checks SystemModelChecker::CheckIdentifiers() with unused identifier
//!
void UT_SystemModelChecker::test_CheckIdentifiers_With_UnusedIdentifier ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("", 6u, 4u);
  auto reg_1 = sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("1100_111"), tap);
  auto reg_2 = sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("111"),      tap);

  sm.RemoveNodeFromModel(reg_1);
  SystemModelChecker sut(sm);

  // ---------------- Exercise
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
                          "  - Identifier '4' is not associated with a node\n";

  TS_ASSERT_EQUALS (report, expectedReport);
}


//! Checks SystemModelChecker::CheckTree() with empty model
//!
void UT_SystemModelChecker::test_CheckTree_With_EmptyModel ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  SystemModelChecker sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckTree());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_TRUE   (result.HasIssues());

  auto   report         = result.MakeReport();
  string expectedReport = "Errors   (0):\n"
                          "Warnings (1):\n"
                          "  - SystemModel has no root node\n"
                          "Infos    (0):\n";

  TS_ASSERT_EQUALS (report, expectedReport);
}

//! Checks SystemModelChecker::CheckTree() with newly created tap
//!
void UT_SystemModelChecker::test_CheckTree_With_ModelWithTap ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap = builder.Create_JTAG_TAP("", 6u, 4u);
  sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("1100_1"),   tap);
  sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("1100_10"),  tap);
  sm.CreateRegister("Reg_3", BinaryVector::CreateFromBinaryString("1100_111"), tap);

  SystemModelChecker sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckTree());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_FALSE  (result.HasIssues());
  TS_ASSERT_EQUALS (result, SystemModelCheckResult::None);
}


//! Checks SystemModelChecker::CheckTree() with a parent with no child
//!
void UT_SystemModelChecker::test_CheckTree_ParentWithoutChild ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("", 6u, 4u);
  auto chain = sm.CreateChain("Chain_1", tap);

  sm.CreateRegister("Reg_3", BinaryVector::CreateFromBinaryString("1100_1"),   tap);
  sm.CreateRegister("Reg_4", BinaryVector::CreateFromBinaryString("1100_10"),  tap);

  SystemModelChecker sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckTree());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_TRUE   (result.HasIssues());

  auto   report         = result.MakeReport();
  string expectedReport = "Errors   (0):\n"
                          "Warnings (1):\n"
                          "  - Chain 'Chain_1' (id: 4) has no child\n"
                          "Infos    (0):\n";

  TS_ASSERT_EQUALS (report, expectedReport);
}

//! Checks SystemModelChecker::CheckTree() with an node in tree not managed by SystemModel
//!
void UT_SystemModelChecker::test_CheckTree_UnmanagedNode ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("", 6u, 4u);
  auto chain = sm.CreateChain("Chain_1", tap);
  sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("1100_1"),   tap);
  sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("1100_10"),  tap);

  auto reg   = make_shared<Register>("Reg_1", BinaryVector::CreateFromBinaryString("1100_11"));
  chain->AppendChild(reg);

  SystemModelChecker sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckTree());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_TRUE   (result.HasIssues());

  auto   report         = result.MakeReport();
  string expectedReport = "Errors   (1):\n"
                          "  - Register 'Reg_1' (id: 7) is not managed by SystemModel\n"
                          "Warnings (0):\n"
                          "Infos    (0):\n";

  TS_ASSERT_EQUALS (report, expectedReport);
}


//! Checks SystemModelChecker::CheckTree() with a node that is not reachable from tree traversal
//!
void UT_SystemModelChecker::test_CheckTree_UnreachableNode ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("", 6u, 4u);
  auto chain = sm.CreateChain("Chain_1", tap);
  auto reg_1 = sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("1100_101"));  // Is not appended in the tree structure
  auto reg_2 = sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("1100_0010"), chain);
  sm.CreateRegister("Reg_3", BinaryVector::CreateFromBinaryString("1100_1"),   tap);
  sm.CreateRegister("Reg_4", BinaryVector::CreateFromBinaryString("1100_10"),  tap);

  SystemModelChecker sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckTree());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_TRUE   (result.HasIssues());

  auto   report         = result.MakeReport();
  string expectedReport = "Errors   (1):\n"
                          "  - Register 'Reg_1' (id: 5) has not been seen by checker tree traversal (may be unconnected)\n"
                          "Warnings (0):\n"
                          "Infos    (0):\n";

  TS_ASSERT_EQUALS (report, expectedReport);
}


//! Checks SystemModelChecker::CheckTree() with a node has been added twice (to two different parent)
//!
void UT_SystemModelChecker::test_CheckTree_NodeAppendedTwice ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("", 6u, 4u);
  auto chain = sm.CreateChain("Chain_1", tap);
  auto reg_1 = sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("1100_101"),  tap);
  auto reg_2 = sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("1100_0010"), chain);
  sm.CreateRegister("Reg_3", BinaryVector::CreateFromBinaryString("1100_1"),   tap);

  chain->AppendChild(reg_1);

  SystemModelChecker sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckTree());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_TRUE   (result.HasIssues());

  auto   report         = result.MakeReport();
  string expectedReport = "Errors   (1):\n"
                          "  - Register 'Reg_1' (id: 5), child of Linker 'TAP_DR_Mux' (id: 2), is also child of Chain 'Chain_1' (id: 4)\n"
                          "Warnings (0):\n"
                          "Infos    (0):\n";

  TS_ASSERT_EQUALS (report, expectedReport);
}

//! Checks SystemModelChecker::CheckTree() with a node has been added twice in a row to same parent
//!
void UT_SystemModelChecker::test_CheckTree_NodeAppendedTwice_SameParent_1 ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("", 6u, 4u);
  auto chain = sm.CreateChain("Chain_1", tap);
  auto reg_1 = sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("1100_101"),  chain);
  auto reg_2 = sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("1100_0010"), chain);
  sm.CreateRegister("Reg_3", BinaryVector::CreateFromBinaryString("110"),  tap);
  sm.CreateRegister("Reg_4", BinaryVector::CreateFromBinaryString("1100"), tap);

  chain->AppendChild(reg_2);

  SystemModelChecker sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckTree());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_TRUE   (result.HasIssues());

  auto   report         = result.MakeReport();
  string expectedReport = "Errors   (1):\n"
                          "  - Register 'Reg_2' (id: 6) has been appended twice in a row to Chain 'Chain_1' (id: 4)\n"
                          "Warnings (0):\n"
                          "Infos    (0):\n";

  TS_ASSERT_EQUALS (report, expectedReport);
}


//! Checks SystemModelChecker::CheckTree() with a node has been added twice to same parent (but not in a row)
//!
void UT_SystemModelChecker::test_CheckTree_NodeAppendedTwice_SameParent_2 ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("", 6u, 4u);
  auto chain = sm.CreateChain("Chain_1", tap);
  auto reg_1 = sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("1100_101"),  chain);
  auto reg_2 = sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("1100_0010"), chain);
  sm.CreateRegister("Reg_3", BinaryVector::CreateFromBinaryString("110"),  tap);
  sm.CreateRegister("Reg_4", BinaryVector::CreateFromBinaryString("1100"), tap);

  chain->AppendChild(reg_1);

  SystemModelChecker sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckTree());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_TRUE   (result.HasIssues());

  auto   report         = result.MakeReport();
  string expectedReport = "Errors   (1):\n"
                          "  - Register 'Reg_1' (id: 5) has been appended twice to Chain 'Chain_1' (id: 4)\n"
                          "Warnings (0):\n"
                          "Infos    (0):\n";

  TS_ASSERT_EQUALS (report, expectedReport);
}


//! Checks SystemModelChecker::CheckTree() with a node has been added into itself (last child)
//!
void UT_SystemModelChecker::test_CheckTree_NodeAppended_ToSelf_Last ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("", 6u, 2u);
  auto chain = sm.CreateChain("Chain_1", tap);
  auto reg_1 = sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("1"),  chain);
  auto reg_2 = sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("10"), chain);
  chain->AppendChild(chain);

  SystemModelChecker sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckTree());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_TRUE   (result.HasIssues());

  auto   report         = result.MakeReport();
  string expectedReport = "Errors   (1):\n"
                          "  - Chain 'Chain_1' (id: 4) has been appended into itself\n"
                          "Warnings (0):\n"
                          "Infos    (0):\n";

  TS_ASSERT_EQUALS (report, expectedReport);
}


//! Checks SystemModelChecker::CheckTree() with a node has been added into itself (middle child)
//!
void UT_SystemModelChecker::test_CheckTree_NodeAppended_ToSelf_Middle ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("", 6u, 2u);
  auto chain = sm.CreateChain("Chain_1", tap);
  auto reg_1 = sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("1"),  chain);
  chain->AppendChild(chain);
  auto reg_2 = sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("10"), chain);

  SystemModelChecker sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckTree());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_TRUE   (result.HasIssues());

  auto   report         = result.MakeReport();
  string expectedReport = "Errors   (3):\n"
                          "  - Linker 'TAP_DR_Mux' (id: 2) has 3 children, even though it can only select 2 paths\n"
                          "  - Chain 'Chain_1' (id: 4) has been appended into itself\n"
                          "  - Register 'Reg_2' (id: 6), child of Linker 'TAP_DR_Mux' (id: 2), is also child of Chain 'Chain_1' (id: 4)\n"
                          "Warnings (0):\n"
                          "Infos    (0):\n";

  TS_ASSERT_EQUALS (report, expectedReport);
}


//! Checks SystemModelChecker::CheckTree() when linker has less children than can be selected
//!
void UT_SystemModelChecker::test_CheckTree_When_Linker_Less_Children ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap = builder.Create_JTAG_TAP("", 6u, 4u);
  sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("110"),  tap);
  sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("1100"), tap);

  SystemModelChecker sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckTree());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_TRUE   (result.HasIssues());

  auto   report         = result.MakeReport();
  string expectedReport = "Errors   (0):\n"
                          "Warnings (1):\n"
                          "  - Linker 'TAP_DR_Mux' (id: 2) has only 3 children, even though it can select 4 paths\n"
                          "Infos    (0):\n";

  TS_ASSERT_EQUALS (report, expectedReport);
}


//! Checks SystemModelChecker::CheckTree() when linker has more children than can be selected
//!
void UT_SystemModelChecker::test_CheckTree_When_Linker_More_Children ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap = builder.Create_JTAG_TAP("", 6u, 4u);
  sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("1"),   tap);
  sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("10"),  tap);
  sm.CreateRegister("Reg_3", BinaryVector::CreateFromBinaryString("111"), tap);
  sm.CreateRegister("Reg_4", BinaryVector::CreateFromBinaryString("100"), tap);

  SystemModelChecker sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckTree());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_TRUE   (result.HasIssues());

  auto   report         = result.MakeReport();
  string expectedReport = "Errors   (1):\n"
                          "  - Linker 'TAP_DR_Mux' (id: 2) has 5 children, even though it can only select 4 paths\n"
                          "Warnings (0):\n"
                          "Infos    (0):\n";

  TS_ASSERT_EQUALS (report, expectedReport);
}


//! Checks SystemModelChecker::CheckTree() when linker has a selector that can select no path at all
//!
void UT_SystemModelChecker::test_CheckTree_When_MaxPath_Zero ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap          = builder.Create_JTAG_TAP("", 6u, 3u);
  auto reg_1        = sm.CreateRegister("reg_1", BinaryVector::CreateFromBinaryString("10"),  tap);
  auto pathSelector = make_shared<DefaultBinaryPathSelector>(reg_1, 0u);
  auto linker       = sm.CreateLinker("linker", pathSelector, tap);

  SystemModelChecker sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckTree());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_TRUE   (result.HasIssues());

  auto   report         = result.MakeReport();
  string expectedReport = "Errors   (0):\n"
                          "Warnings (2):\n"
                          "  - Linker 'linker' (id: 5) has a selector that can select no path at all\n"
                          "  - Linker 'linker' (id: 5) has no child\n"
                          "Infos    (0):\n";

  TS_ASSERT_EQUALS (report, expectedReport);
}


//! Checks SystemModelChecker::CheckTree() when there is no AccessInterface
//!
void UT_SystemModelChecker::test_Check_When_NoAccessInterface ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  SystemModelBuilder builder(sm);

  auto wrapper = builder.Create_1500_Wrapper("1500", 2u);
  auto reg_1   = sm.CreateRegister("reg_1", BinaryVector::CreateFromBinaryString("01"),  wrapper);
  auto reg_2   = sm.CreateRegister("reg_2", BinaryVector::CreateFromBinaryString("10"),  wrapper);

  SystemModelChecker sut(sm);

  // ---------------- Exercise
  //
  auto result = SystemModelChecker::Check(sm);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE      (result.HasErrors());
  TS_ASSERT_FALSE     (result.HasWarnings());
  TS_ASSERT_EQUALS    (result.infosCount,  0u);
  TS_ASSERT_EQUALS    (result.errorsCount, 2u); // 2 for No AccessInterface + for child not an AccessInterface
  TS_ASSERT_NOT_EMPTY (result.errors);
  TS_ASSERT_NOT_EMPTY (result.MakeReport());
}


//! Checks SystemModelChecker::CheckTree() when root is and AccessInterface
//!
void UT_SystemModelChecker::test_Check_When_RootIsAccessInterface ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("1500", 8u, 3u);
  auto reg_1 = sm.CreateRegister("reg_1", BinaryVector::CreateFromBinaryString("01"), tap);
  auto reg_2 = sm.CreateRegister("reg_2", BinaryVector::CreateFromBinaryString("10"), tap);

  // ---------------- Exercise
  //
  auto result = SystemModelChecker::Check(sm);

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (result.HasErrors());
  TS_ASSERT_FALSE (result.HasWarnings());
}


//! Checks SystemModelChecker::CheckTree() when root is a Chain with single AccessInterface
//!
void UT_SystemModelChecker::test_Check_When_RootIsChainWithAccessInterface ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto root  = sm.CreateChain("root");
  auto tap   = builder.Create_JTAG_TAP("1500", 8u, 3u);
  auto reg_1 = sm.CreateRegister("reg_1", BinaryVector::CreateFromBinaryString("01"), tap);
  auto reg_2 = sm.CreateRegister("reg_2", BinaryVector::CreateFromBinaryString("10"), tap);

  root->AppendChild(tap);

  // ---------------- Exercise
  //
  auto result = SystemModelChecker::Check(sm);

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (result.HasErrors());
  TS_ASSERT_FALSE (result.HasWarnings());
}


//! Checks SystemModelChecker::CheckTree() when root is a Chain with several AccessInterface
//!
void UT_SystemModelChecker::test_Check_When_RootIsChainWithAccessInterfaces ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto root  = sm.CreateChain("root");
  auto tap_1 = builder.Create_JTAG_TAP("1500_1", 8u, 2u);
  auto tap_2 = builder.Create_JTAG_TAP("1500_2", 8u, 2u);
  auto tap_3 = builder.Create_JTAG_TAP("1500_3", 8u, 2u);
  auto reg_1 = sm.CreateRegister("reg_1", BinaryVector::CreateFromBinaryString("01"), tap_1);
  auto reg_2 = sm.CreateRegister("reg_2", BinaryVector::CreateFromBinaryString("10"), tap_2);
  auto reg_3 = sm.CreateRegister("reg_2", BinaryVector::CreateFromBinaryString("11"), tap_3);

  root->AppendChild(tap_1);
  root->AppendChild(tap_2);
  root->AppendChild(tap_3);

  // ---------------- Exercise
  //
  auto result = SystemModelChecker::Check(sm);

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (result.HasErrors());
  TS_ASSERT_FALSE (result.HasWarnings());
}


//! Checks SystemModelChecker::CheckTree() when root is a Chain without AccessInterface
//!
void UT_SystemModelChecker::test_Check_When_RootIsChainWithoutAccessInterface ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto root  = sm.CreateChain("root");
  auto reg_1 = sm.CreateRegister("reg_1", BinaryVector::CreateFromBinaryString("01"), root);
  auto reg_2 = sm.CreateRegister("reg_2", BinaryVector::CreateFromBinaryString("10"), root);

  // ---------------- Exercise
  //
  auto result = SystemModelChecker::Check(sm);

  // ---------------- Verify
  //
  TS_ASSERT_FALSE  (result.HasWarnings());
  TS_ASSERT_EQUALS (result.errorsCount, 3u); // 1 for no AccessInterface + 2 for regs that are not AccessInterface
}


//! Checks SystemModelChecker::CheckTree() when root is a Chain with AccessInterface and a register
//!
void UT_SystemModelChecker::test_Check_When_RootIsChainWithMixKindChildren ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto root  = sm.CreateChain("root");
  auto tap_1 = builder.Create_JTAG_TAP("1500_1", 8u, 2u);
  auto tap_2 = builder.Create_JTAG_TAP("1500_2", 8u, 2u);
  auto reg_1 = sm.CreateRegister("reg_1", BinaryVector::CreateFromBinaryString("01"), tap_1);
  auto reg_2 = sm.CreateRegister("reg_2", BinaryVector::CreateFromBinaryString("10"), tap_2);
  auto reg_3 = sm.CreateRegister("reg_3", BinaryVector::CreateFromBinaryString("11"), root);

  root->AppendChild(tap_1);
  root->AppendChild(tap_2);

  // ---------------- Exercise
  //
  auto result = SystemModelChecker::Check(sm);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (result.HasErrors());
  TS_ASSERT_FALSE  (result.HasWarnings());
  TS_ASSERT_EQUALS (result.errorsCount, 1u); // 1 for child not an AccessInterface
}


//! Checks SystemModelChecker::CheckTree() when root is an AccessInterface and there is anoter one bellow
//!
//+void UT_SystemModelChecker::test_Check_When_RootIsAccessInterface_and_AnotherBellow ()
//+{
//+  // ---------------- Setup
//+  //
//+  SystemModel sm;
//+  TestModelBuilder builder(sm);

//+  auto tap_1 = builder.Create_JTAG_TAP("1500_1", 8u, 4u);
//+  auto reg_1 = sm.CreateRegister("reg_1", BinaryVector::CreateFromBinaryString("01"), tap_1);
//+  auto reg_2 = sm.CreateRegister("reg_2", BinaryVector::CreateFromBinaryString("10"), tap_1);
//+  auto tap_2 = builder.Create_JTAG_TAP("1500_2", 8u, 3u);
//+  auto reg_3 = sm.CreateRegister("reg_3", BinaryVector::CreateFromBinaryString("011"), tap_2);
//+  auto reg_4 = sm.CreateRegister("reg_4", BinaryVector::CreateFromBinaryString("100"), tap_2);

//+  tap_1->AppendChild(tap_2);


//+  // ---------------- Exercise
//+  //
//+  auto result = SystemModelChecker::Check(sm);

//+  // ---------------- Verify
//+  //
//+  TS_ASSERT_FALSE (result.HasWarnings());
//+  TS_ASSERT_EMPTY (result.MakeReport());
//+}


//! Checks SystemModelChecker::CheckTree() when root is and AccessInterface but has 1 too much endpoint
//!
void UT_SystemModelChecker::test_Check_When_AccessInterface_has_MoreEndPoints ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  SystemModelBuilder builder(sm);

  auto protocol = make_shared<SVF_SimulationProtocol>("ToSut.svf", "FromSut.svf");
  auto tap      = builder.Create_JTAG_TAP("Tap", 8u, 3u, protocol);
  auto reg_1    = sm.CreateRegister("reg_1", BinaryVector::CreateFromBinaryString("01"), tap);
  auto reg_2    = sm.CreateRegister("reg_2", BinaryVector::CreateFromBinaryString("10"), tap);
  tap->SetChildAppender(nullptr);
  auto reg_3    = sm.CreateRegister("reg_3", BinaryVector::CreateFromBinaryString("11"), tap);

  // ---------------- Exercise
  //
  auto result = SystemModelChecker::Check(sm);

  // ---------------- Verify
  //
  TS_ASSERT_FALSE  (result.HasWarnings());
  TS_ASSERT_EQUALS (result.errorsCount,  1u);
}


//! Checks SystemModelChecker::CheckTree() when root is and AccessInterface but has fewer
//! endpoints than supported by its endpoint
//!
void UT_SystemModelChecker::test_Check_When_AccessInterface_has_LessEndPoints ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  SystemModelBuilder builder(sm);

  auto protocol = make_shared<Spy_AccessInterfaceProtocols>();
  auto tap      = builder.Create_JTAG_TAP("Tap", 8u, 3u, protocol);
  auto reg_1    = sm.CreateRegister("reg_1", BinaryVector::CreateFromBinaryString("01"), tap);
  auto reg_2    = sm.CreateRegister("reg_2", BinaryVector::CreateFromBinaryString("10"), tap);

  //+ (begin JFC August/26/2016): for debug purpose
//+   TS_ASSERT_EQUALS (GmlPrinter::Graph(sm.Root()), "", GmlPrinterOptions::All);
  //+ (end   JFC August/26/2016):

  // ---------------- Exercise
  //
  auto result = SystemModelChecker::Check(sm);

  // ---------------- Verify
  //
  TS_ASSERT_FALSE  (result.HasWarnings());
  TS_ASSERT_FALSE  (result.HasErrors());
  TS_ASSERT_EQUALS (result.infosCount,  1u);
//+  TS_ASSERT_EMPTY (result.MakeReport());
}




//! Checks SystemModelChecker::Check() when one ParentNode and a Register share the same name
//!
//! @note Other name issues are tested by UT_NamesChecker
void UT_SystemModelChecker::test_Check_SameNames ()
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
  auto result = SystemModelChecker::Check(sm);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (result.infosCount,    0u);
  TS_ASSERT_EQUALS (result.warningsCount, 2u);  // For no child in chains
  TS_ASSERT_EQUALS (result.errorsCount,   1u);  // For same name
}




//! Checks SystemModelChecker::CheckTree() when an access interface has no associated AccessInterfaceProtocol
//!
void UT_SystemModelChecker::test_Check_NoProtocol ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap = builder.Create_JTAG_TAP("", 6u, 2u);
  auto reg = sm.CreateRegister("reg", BinaryVector::CreateFromBinaryString("10"),  tap);
  auto ai  = dynamic_pointer_cast<AccessInterface>(tap);
  TS_ASSERT_NOT_NULLPTR (ai);
  ai->SetProtocol(nullptr);        // This remove any protocol associated with the AccessInterface

  // ---------------- Exercise
  //
  auto result = SystemModelChecker::Check(sm);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE      (result.HasWarnings());
  TS_ASSERT_FALSE     (result.HasErrors());
  TS_ASSERT_EQUALS    (result.infosCount,    0u);
  TS_ASSERT_EQUALS    (result.warningsCount, 1u);
  TS_ASSERT_NOT_EMPTY (result.warnings);
  TS_ASSERT_NOT_EMPTY (result.MakeReport());
}

//===========================================================================
// End of UT_SystemModelChecker.cpp
//===========================================================================
