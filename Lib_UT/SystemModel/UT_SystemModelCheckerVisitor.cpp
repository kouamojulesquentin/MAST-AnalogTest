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
#include "DefaultBinaryPathSelector.hpp"
#include "TestModelBuilder.hpp"
#include "SystemModelCheckResult_Traits.hpp"


using std::string;
using std::make_shared;
using std::dynamic_pointer_cast;
using namespace mast;
using namespace test;


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
  TestModelBuilder builder(sm);

  builder.Create_JTAG_TAP("", 6u, 4u);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (SystemModelCheckerVisitor sut(sm));
}


//! Checks SystemModelCheckerVisitor::CheckIdentifiers() with empty model
//!
void UT_SystemModelCheckerVisitor::test_CheckIdentifiers_With_EmptyModel ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  SystemModelCheckerVisitor sut(sm);

  // ---------------- Exercise & Verify
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


//! Checks SystemModelCheckerVisitor::CheckIdentifiers() with newly created tap
//!
void UT_SystemModelCheckerVisitor::test_CheckIdentifiers_With_ModelWithTap ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  builder.Create_JTAG_TAP("", 6u, 4u);

  SystemModelCheckerVisitor sut(sm);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckIdentifiers());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_FALSE  (result.HasIssues());
  TS_ASSERT_EQUALS (result, SystemModelCheckResult::None);
}


//! Checks SystemModelCheckerVisitor::CheckIdentifiers() with unused identifier
//!
void UT_SystemModelCheckerVisitor::test_CheckIdentifiers_With_UnusedIdentifier ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("", 6u, 4u);
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
                          "  - Identifier '4' is not associated with a node\n";

  TS_ASSERT_EQUALS (report, expectedReport);
}


//! Checks SystemModelCheckerVisitor::CheckTree() with empty model
//!
void UT_SystemModelCheckerVisitor::test_CheckTree_With_EmptyModel ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  SystemModelCheckerVisitor sut(sm);

  // ---------------- Exercise & Verify
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

//! Checks SystemModelCheckerVisitor::CheckTree() with newly created tap
//!
void UT_SystemModelCheckerVisitor::test_CheckTree_With_ModelWithTap ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap = builder.Create_JTAG_TAP("", 6u, 4u);
  sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("1100_1"),   tap);
  sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("1100_10"),  tap);
  sm.CreateRegister("Reg_3", BinaryVector::CreateFromBinaryString("1100_111"), tap);

  SystemModelCheckerVisitor sut(sm);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckTree());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();
  TS_ASSERT_FALSE  (result.HasIssues());
  TS_ASSERT_EQUALS (result, SystemModelCheckResult::None);
}


//! Checks SystemModelCheckerVisitor::CheckTree() with a parent with no child
//!
void UT_SystemModelCheckerVisitor::test_CheckTree_ParentWithoutChild ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP("", 6u, 4u);
  auto chain = sm.CreateChain("Chain_1", tap);

  sm.CreateRegister("Reg_3", BinaryVector::CreateFromBinaryString("1100_1"),   tap);
  sm.CreateRegister("Reg_4", BinaryVector::CreateFromBinaryString("1100_10"),  tap);

  SystemModelCheckerVisitor sut(sm);

  // ---------------- Exercise & Verify
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

//! Checks SystemModelCheckerVisitor::CheckTree() with an node in tree not managed by SystemModel
//!
void UT_SystemModelCheckerVisitor::test_CheckTree_UnmanagedNode ()
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

  SystemModelCheckerVisitor sut(sm);

  // ---------------- Exercise & Verify
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


//! Checks SystemModelCheckerVisitor::CheckTree() with a node that is not reachable from tree traversal
//!
void UT_SystemModelCheckerVisitor::test_CheckTree_UnreachableNode ()
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

  SystemModelCheckerVisitor sut(sm);

  // ---------------- Exercise & Verify
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


//! Checks SystemModelCheckerVisitor::CheckTree() with a node has been added twice (to two different parent)
//!
void UT_SystemModelCheckerVisitor::test_CheckTree_NodeAppendedTwice ()
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

  SystemModelCheckerVisitor sut(sm);

  // ---------------- Exercise & Verify
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

//! Checks SystemModelCheckerVisitor::CheckTree() with a node has been added twice in a row to same parent
//!
void UT_SystemModelCheckerVisitor::test_CheckTree_NodeAppendedTwice_SameParent_1 ()
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

  SystemModelCheckerVisitor sut(sm);

  // ---------------- Exercise & Verify
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


//! Checks SystemModelCheckerVisitor::CheckTree() with a node has been added twice to same parent (but not in a row)
//!
void UT_SystemModelCheckerVisitor::test_CheckTree_NodeAppendedTwice_SameParent_2 ()
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

  SystemModelCheckerVisitor sut(sm);

  // ---------------- Exercise & Verify
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


//! Checks SystemModelCheckerVisitor::CheckTree() with a node has been added into itself (last child)
//!
void UT_SystemModelCheckerVisitor::test_CheckTree_NodeAppended_ToSelf_Last ()
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

  SystemModelCheckerVisitor sut(sm);

  // ---------------- Exercise & Verify
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


//! Checks SystemModelCheckerVisitor::CheckTree() with a node has been added into itself (middle child)
//!
void UT_SystemModelCheckerVisitor::test_CheckTree_NodeAppended_ToSelf_Middle ()
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

  SystemModelCheckerVisitor sut(sm);

  // ---------------- Exercise & Verify
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


//! Checks SystemModelCheckerVisitor::CheckTree() when linker has less children than can be selected
//!
void UT_SystemModelCheckerVisitor::test_CheckTree_When_Linker_Less_Children ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap = builder.Create_JTAG_TAP("", 6u, 4u);
  sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("110"),  tap);
  sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("1100"), tap);

  SystemModelCheckerVisitor sut(sm);

  // ---------------- Exercise & Verify
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


//! Checks SystemModelCheckerVisitor::CheckTree() when linker has more children than can be selected
//!
void UT_SystemModelCheckerVisitor::test_CheckTree_When_Linker_More_Children ()
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

  SystemModelCheckerVisitor sut(sm);

  // ---------------- Exercise & Verify
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


//! Checks SystemModelCheckerVisitor::CheckTree() when linker has a selector that can select no path at all
//!
void UT_SystemModelCheckerVisitor::test_CheckTree_When_MaxPath_Zero ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap          = builder.Create_JTAG_TAP("", 6u, 3u);
  auto reg_1        = sm.CreateRegister("reg_1", BinaryVector::CreateFromBinaryString("10"),  tap);
  auto pathSelector = make_shared<DefaultBinaryPathSelector>(reg_1, 0u);
  auto linker       = sm.CreateLinker("linker", pathSelector, tap);

  SystemModelCheckerVisitor sut(sm);

  // ---------------- Exercise & Verify
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

//! Checks SystemModelCheckerVisitor::CheckTree() when an access interface has no associated AccessInterfaceProtocol
//!
void UT_SystemModelCheckerVisitor::test_CheckTree_NoProtocol ()
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

  SystemModelCheckerVisitor sut(sm);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.CheckTree());

  // ---------------- Verify
  //
  auto result = sut.MakeCheckResult();

  TS_ASSERT_TRUE    (result.HasWarnings());
  TS_ASSERT_FALSE   (result.HasErrors());
  TS_ASSERT_EQUALS  (result.infosCount, 0u);
  TS_ASSERT_EQUALS  (result.warningsCount, 1u);
  TS_ASSERT_DIFFERS (result.warnings,      "");
  TS_ASSERT_DIFFERS (result.MakeReport(),  "");
}

//===========================================================================
// End of UT_SystemModelCheckerVisitor.cpp
//===========================================================================
