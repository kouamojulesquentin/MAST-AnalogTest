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
  TS_ASSERT_NULLPTR (sut.Root());
  TS_ASSERT_EQUALS  (sut.RegistersCount(), 0);
}


//! Checks SystemModel::NodeWithId() when empty
//!
void UT_SystemModel::test_NodeWithId_When_Empty ()
{
  // ---------------- Setup
  //
  SystemModel                     sut;
  SystemModelNode::NodeIdentifier id = 0;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.NodeWithId(id), std::exception);
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
  TS_ASSERT_EQUALS      (node->Name(), name);

  auto id = node->Identifier();

  TS_ASSERT_EQUALS      (id, 0);
  TS_ASSERT_NOT_NULLPTR (sut.Root());
  TS_ASSERT_NOT_NULLPTR (sut.NodeWithId(id));
  TS_ASSERT_EQUALS_PTR  (sut.Root(), sut.NodeWithId(id));
}



//! Checks SystemModel::CreateRegister() without providing a parent node
//!
void UT_SystemModel::test_CreateRegister_Without_ParentNode ()
{
  // ---------------- Setup
  //
  SystemModel sut;
  string_view name   = "Reg name";
  auto        bypass = BinaryVector::CreateFromBinaryString("1010_1");

  sut.CreateAccessInterface("", nullptr); // Necessary to have a root node

  // ---------------- Exercise
  //
  auto node = sut.CreateRegister(name, bypass);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (node);
  TS_ASSERT_EQUALS      (node->Name(), name);

  auto id = node->Identifier();

  TS_ASSERT_EQUALS_PTR (sut.NodeWithId(id), node);
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
  TS_ASSERT_EQUALS      (node->Name(), name);

  auto id = node->Identifier();

  TS_ASSERT_EQUALS_PTR (sut.NodeWithId(id), node);
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
  TS_ASSERT_EQUALS      (node->Name(), name);

  auto id = node->Identifier();

  TS_ASSERT_EQUALS_PTR (node, sut.NodeWithId(id));
  TS_ASSERT_EQUALS_PTR (node, ai->FirstChild());
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
  TS_ASSERT_EQUALS      (linker->Name(), name);

  auto id = linker->Identifier();

  TS_ASSERT_EQUALS_PTR (sut.NodeWithId(id), linker);
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
  TS_ASSERT_EQUALS      (linker->Name(), name);

  auto id            = linker->Identifier();
  auto aiFirstChild = ai->FirstChild();
  auto muxSibling    = muxNode->NextSibling();

  TS_ASSERT_EQUALS_PTR (linker,  sut.NodeWithId(id));
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
  TS_ASSERT_EQUALS      (tapNode->Name(), DEFAULT_TAP_NAME);

  auto id = tapNode->Identifier();

  TS_ASSERT_EQUALS      (id, 0);
  TS_ASSERT_NOT_NULLPTR (sut.Root());
  TS_ASSERT_NOT_NULLPTR (sut.NodeWithId(id));
  TS_ASSERT_EQUALS_PTR  (sut.Root(), sut.NodeWithId(id));

  // IR
  auto irNode = tapNode->FirstChild();
  TS_ASSERT_NOT_NULLPTR (irNode);
  TS_ASSERT_EQUALS      (irNode->Name(), DEFAULT_TAP_IR_NAME);

  auto irAsRegister = dynamic_pointer_cast<Register>(irNode);
  TS_ASSERT_NOT_NULLPTR (irAsRegister);
  TS_ASSERT_EQUALS      (irAsRegister->BypassSequence(), BinaryVector::CreateFromBinaryString("1111_11"));

  // DR MUX
  auto muxNode = irNode->NextSibling();
  TS_ASSERT_NOT_NULLPTR (muxNode);
  auto muxAsLinker = dynamic_pointer_cast<Linker>(muxNode);
  TS_ASSERT_NOT_NULLPTR (muxAsLinker);
  TS_ASSERT_EQUALS      (muxAsLinker->Name(), DEFAULT_TAP_MUX_NAME);

  // DR bypass
  auto bypassNode = muxAsLinker->FirstChild();
  TS_ASSERT_NOT_NULLPTR (bypassNode);
  auto bypassAsRegister = dynamic_pointer_cast<Register>(bypassNode);
  TS_ASSERT_NOT_NULLPTR (bypassAsRegister);
  TS_ASSERT_EQUALS (bypassAsRegister->Name(), DEFAULT_TAP_MUX_BPY_NAME);

  // Check appending nodes to tap
  auto linkerSecondChild = bypassAsRegister->NextSibling();
  TS_ASSERT_NULLPTR (linkerSecondChild);

  auto newReg = sut.CreateRegister("New reg", BinaryVector::CreateFromBinaryString("1010"), tapNode);

  linkerSecondChild = bypassAsRegister->NextSibling();
  TS_ASSERT_NOT_NULLPTR (linkerSecondChild);
  TS_ASSERT_EQUALS      (linkerSecondChild->Name(), "New reg");
}


//===========================================================================
// End of UT_SystemModel.cpp
//===========================================================================
