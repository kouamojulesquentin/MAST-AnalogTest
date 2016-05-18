//===========================================================================
//                           UT_SystemModel.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SystemModel.cpp
//!
//! Implements test fixture for testing SystemModel
//!
//===========================================================================

#include "UT_SystemModel.hpp"
#include "SystemModel.hpp"
#include "SystemModelNode.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "BinaryVector_Traits.hpp"

using std::string;
using std::experimental::string_view;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;
using namespace mast;


//! Initializes test (called for each test)
void UT_SystemModel::setUp ()
{
//+  CxxTest::setStringResultsOnNewLine(true);
//+  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}


//! Checks SystemModel constructor
//!
void UT_SystemModel::test_Constructor ()
{
  // ---------------- Exercise
  //
  SystemModel sut;

  // ---------------- Verify
  //
  TS_ASSERT_NULLPTR (sut.GetRoot());
  TS_ASSERT_EQUALS  (sut.GetRegistersCount(), 0);
}


//! Checks SystemModel::GetNode() when empty
//!
void UT_SystemModel::test_GetNode_When_Empty ()
{
  // ---------------- Setup
  //
  SystemModel                     sut;
  SystemModelNode::NodeIdentifier id = 0;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.GetNode(id), std::exception);
}



//! Checks SystemModel::CreateAccessInterface()
//!
void UT_SystemModel::test_CreateAccessInterface ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  string_view name = "AI name";

  // ---------------- Exercise
  //
  auto node = sut.CreateAccessInterface(name, nullptr);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (node);
  TS_ASSERT_EQUALS      (node->GetName(), name);

  auto id = node->GetIdentifier();

  TS_ASSERT_EQUALS      (id, 0);
  TS_ASSERT_NOT_NULLPTR (sut.GetRoot());
  TS_ASSERT_NOT_NULLPTR (sut.GetNode(id));
  TS_ASSERT_EQUALS_PTR  (sut.GetRoot(), sut.GetNode(id));
}



//! Checks SystemModel::CreateRegister() without providing a parent node
//!
void UT_SystemModel::test_CreateRegister_Without_ParentNode ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  auto        tap    = sut.CreateAccessInterface("AI name", nullptr);
  string_view name   = "Reg name";
  auto        bypass = BinaryVector::CreateFromBinaryString("1010_1");

  // ---------------- Exercise
  //
  auto node = sut.CreateRegister(name, bypass);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (node);
  TS_ASSERT_EQUALS      (node->GetName(), name);

  auto id = node->GetIdentifier();

  TS_ASSERT_EQUALS_PTR (sut.GetNode(id), node);
}


//! Checks SystemModel::CreateChain() without providing a parent node
//!
void UT_SystemModel::test_CreateChain_Without_ParentNode ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  string_view name = "Given name";

  // ---------------- Exercise
  //
  auto node = sut.CreateChain(name);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (node);
  TS_ASSERT_EQUALS      (node->GetName(), name);

  auto id = node->GetIdentifier();

  TS_ASSERT_EQUALS_PTR (sut.GetNode(id), node);
}


//! Checks SystemModel::CreateChain() providing a parent node
//!
void UT_SystemModel::test_CreateChain_With_ParentNode ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  auto        ai   = sut.CreateAccessInterface("AI name", nullptr);
  string_view name = "Given name";

  // ---------------- Exercise
  //
  auto node = sut.CreateChain(name, ai);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (node);
  TS_ASSERT_EQUALS      (node->GetName(), name);

  auto id = node->GetIdentifier();

  TS_ASSERT_EQUALS_PTR (node, sut.GetNode(id));
  TS_ASSERT_EQUALS_PTR (node, ai->GetFirstChild());
}



//! Checks SystemModel::CreateLinker() without providing a parent node
//!
void UT_SystemModel::test_CreateLinker_Without_ParentNode ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  auto        ai           = sut.CreateAccessInterface ("ai name", nullptr);
  auto        muxNode      = sut.CreateRegister        ("Selector", BinaryVector::CreateFromBinaryString("01"), ai);
  auto        pathSelector = make_shared<DefaultBinaryPathSelector>(muxNode, 2);

  string_view name = "Given name";

  // ---------------- Exercise
  //
  auto linker = sut.CreateLinker(name, pathSelector);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (linker);
  TS_ASSERT_EQUALS      (linker->GetName(), name);

  auto id = linker->GetIdentifier();

  TS_ASSERT_EQUALS_PTR (sut.GetNode(id), linker);
}


//! Checks SystemModel::CreateLinker() providing a parent node
//!
void UT_SystemModel::test_CreateLinker_With_ParentNode ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  auto        ai           = sut.CreateAccessInterface ("ai name", nullptr);
  auto        muxNode      = sut.CreateRegister        ("Selector", BinaryVector::CreateFromBinaryString("01"), ai);
  auto        pathSelector = make_shared<DefaultBinaryPathSelector>(muxNode, 2);
  string_view name = "Given name";

  // ---------------- Exercise
  //
  auto linker = sut.CreateLinker(name, pathSelector, ai);


  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (linker);
  TS_ASSERT_EQUALS      (linker->GetName(), name);

  auto id            = linker->GetIdentifier();
  auto aiFirstChild = ai->GetFirstChild();
  auto muxSibling    = muxNode->GetNextSibling();

  TS_ASSERT_EQUALS_PTR (linker,  sut.GetNode(id));
  TS_ASSERT_EQUALS_PTR (muxNode, aiFirstChild);
  TS_ASSERT_EQUALS_PTR (linker,  muxSibling);
}


//! Checks SystemModel::CreateTap()
//!
void UT_SystemModel::test_CreateTap ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  string_view noName;
  uint32_t    irBitsCount   = 6u;
  uint32_t    muxPathsCount = 5u;

  // ---------------- Exercise
  //
  auto tapNode = sut.CreateTap(noName, irBitsCount, muxPathsCount);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (tapNode);
  TS_ASSERT_EQUALS      (tapNode->GetName(), DEFAULT_TAP_NAME);

  auto id = tapNode->GetIdentifier();

  TS_ASSERT_EQUALS      (id, 0);
  TS_ASSERT_NOT_NULLPTR (sut.GetRoot());
  TS_ASSERT_NOT_NULLPTR (sut.GetNode(id));
  TS_ASSERT_EQUALS_PTR  (sut.GetRoot(), sut.GetNode(id));

  // IR
  auto irNode = tapNode->GetFirstChild();
  TS_ASSERT_NOT_NULLPTR (irNode);
  TS_ASSERT_EQUALS      (irNode->GetName(), DEFAULT_TAP_IR_NAME);

  auto irAsRegister = dynamic_pointer_cast<Register>(irNode);
  TS_ASSERT_NOT_NULLPTR (irAsRegister);
  TS_ASSERT_EQUALS      (irAsRegister->GetBypassSequence(), BinaryVector::CreateFromBinaryString("1111_11"));

  // DR MUX
  auto muxNode = irNode->GetNextSibling();
  TS_ASSERT_NOT_NULLPTR (muxNode);
  auto muxAsLinker = dynamic_pointer_cast<Linker>(muxNode);
  TS_ASSERT_NOT_NULLPTR (muxAsLinker);
  TS_ASSERT_EQUALS      (muxAsLinker->GetName(), DEFAULT_TAP_MUX_NAME);

  // DR bypass
  auto bypassNode = muxAsLinker->GetFirstChild();
  TS_ASSERT_NOT_NULLPTR (bypassNode);
  auto bypassAsRegister = dynamic_pointer_cast<Register>(bypassNode);
  TS_ASSERT_NOT_NULLPTR (bypassAsRegister);
  TS_ASSERT_EQUALS (bypassAsRegister->GetName(), DEFAULT_TAP_MUX_BPY_NAME);

  // Check appending nodes to tap
  auto linkerSecondChild = bypassAsRegister->GetNextSibling();
  TS_ASSERT_NULLPTR (linkerSecondChild);

  auto newReg = sut.CreateRegister("New reg", BinaryVector::CreateFromBinaryString("1010"), tapNode);

  linkerSecondChild = bypassAsRegister->GetNextSibling();
  TS_ASSERT_NOT_NULLPTR (linkerSecondChild);
  TS_ASSERT_EQUALS      (linkerSecondChild->GetName(), "New reg");
}


//===========================================================================
// End of UT_SystemModel.cpp
//===========================================================================
