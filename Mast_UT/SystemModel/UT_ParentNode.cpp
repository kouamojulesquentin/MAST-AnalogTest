//===========================================================================
//                           UT_ParentNode.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_ParentNode.cpp
//!
//! Implements test fixture for testing ParentNode
//!
//===========================================================================

#include "UT_ParentNode.hpp"
#include "Chain.hpp"
#include "Register.hpp"
#include "SystemModel.hpp"
#include "GmlPrinter.hpp"  // To get a visual representation of testcase
#include "TestModelBuilder.hpp"
#include <tuple>
#include <cxxtest/ValueTraits.h>

using std::make_tuple;
using std::shared_ptr;
using std::make_shared;
using std::experimental::string_view;
using namespace mast;
using namespace test;

namespace
{
//! Creates a system model for testing node searching methods
//!
shared_ptr<ParentNode> CreateSystemModel (SystemModel& sm, bool reportGml = false)
{
  TestModelBuilder builder(sm);

  auto tap = builder.Create_UnitTestCase_6_Levels();

  if (reportGml)
  {
    TS_TRACE (GmlPrinter::Graph(tap, "UT_ParentNode"));
  }

  return tap;
}
//
//  End of: CreateSystemModel
//---------------------------------------------------------------------------
} // End of unnamed namespace


//! Initializes test (called for each test)
void UT_ParentNode::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}

//! Checks ParentNode constructor
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_constructor ()
{
  // ---------------- Exercise
  //
  Chain sut("chain");

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS  (sut.DirectChildrenCount(), 0);
  TS_ASSERT_NULLPTR (sut.FirstChild());
  TS_ASSERT_NULLPTR (sut.ChildAppender());
  TS_ASSERT_FALSE   (sut.IgnoreForNodePath());
}


//! Checks ParentNode::AppendChild() when it is the first child
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_AppendChild_1 ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.AppendChild(node_1));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS     (sut.DirectChildrenCount(), 1);
  TS_ASSERT_EQUALS_PTR (sut.FirstChild(),          node_1);
  TS_ASSERT_NULLPTR    (sut.ChildAppender());
}


//! Checks ParentNode::AppendChild() when it is the second child
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_AppendChild_2 ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");

  sut.AppendChild(node_1);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.AppendChild(node_2));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS     (sut.DirectChildrenCount(), 2);
  TS_ASSERT_EQUALS_PTR (sut.FirstChild(),          node_1);
  TS_ASSERT_EQUALS_PTR (node_1->NextSibling(),     node_2);
}

//! Checks ParentNode::SetChildAppender()
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_SetChildAppender ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");
  auto node_3 = make_shared<Chain>("node 3");
  auto node_4 = make_shared<Chain>("node 4");

  sut.AppendChild(node_1);
  sut.AppendChild(node_2);
  sut.AppendChild(node_3);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.SetChildAppender(node_2));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS_PTR (sut.ChildAppender(), node_2);

  sut.AppendChild(node_4);

  TS_ASSERT_EQUALS     (sut.DirectChildrenCount(),     3);
  TS_ASSERT_EQUALS     (node_2->DirectChildrenCount(), 1);
  TS_ASSERT_EQUALS_PTR (node_2->FirstChild(),          node_4);
  TS_ASSERT_EQUALS_PTR (node_1->NextSibling(),         node_2);
  TS_ASSERT_EQUALS_PTR (node_2->NextSibling(),         node_3);
  TS_ASSERT_NULLPTR    (node_3->NextSibling());
}

//! Checks ParentNode::DisconnectEndPoint() when first and only one
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectEndPoint_1st_OutOf_1 ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");

  sut.AppendChild(node_1);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DisconnectEndPoint(1u));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS  (sut.DirectChildrenCount(), 0);
  TS_ASSERT_NULLPTR (sut.FirstChild());
}


//! Checks ParentNode::DisconnectEndPoint() when first out of 2
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectEndPoint_1st_OutOf_2 ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");

  sut.AppendChild(node_1);
  sut.AppendChild(node_2);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DisconnectEndPoint(1u));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS     (sut.DirectChildrenCount(), 1);
  TS_ASSERT_EQUALS_PTR (sut.FirstChild(),          node_2);
}


//! Checks ParentNode::DisconnectEndPoint() when 2nd out of 2
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectEndPoint_2nd_OutOf_2 ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");

  sut.AppendChild(node_1);
  sut.AppendChild(node_2);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DisconnectEndPoint(2u));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS     (sut.DirectChildrenCount(), 1);
  TS_ASSERT_EQUALS_PTR (sut.FirstChild(),          node_1);
  TS_ASSERT_NULLPTR    (node_1->NextSibling());
}


//! Checks ParentNode::DisconnectEndPoint() when 2nd out of 3
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectEndPoint_2nd_OutOf_3 ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");
  auto node_3 = make_shared<Chain>("node 3");

  sut.AppendChild(node_1);
  sut.AppendChild(node_2);
  sut.AppendChild(node_3);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DisconnectEndPoint(2u));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS     (sut.DirectChildrenCount(), 2);
  TS_ASSERT_EQUALS_PTR (sut.FirstChild(),          node_1);
  TS_ASSERT_EQUALS_PTR (node_1->NextSibling(),     node_3);
}


//! Checks ParentNode::DisconnectEndPoint() when 3rd out of 2
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectEndPoint_3rd_OutOf_2 ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");

  sut.AppendChild(node_1);
  sut.AppendChild(node_2);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.DisconnectEndPoint(3u), std::exception);
}


//! Checks ParentNode::DisconnectEndPoint() when 0 out of 2
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectEndPoint_0_OutOf_2 ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");

  sut.AppendChild(node_1);
  sut.AppendChild(node_2);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.DisconnectEndPoint(0u), std::exception);
}


//! Checks ParentNode::DisconnectEndPoint() when endpoint 0 out of 0
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectEndPoint_0_OutOf_0 ()
{
  // ---------------- Setup
  //
  auto sut = Chain("chain");

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.DisconnectEndPoint(0u), std::exception);
}

//! Checks ParentNode::DisconnectEndPoint() when endpoint 1 out of 0
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectEndPoint_1_OutOf_0 ()
{
  // ---------------- Setup
  //
  auto sut = Chain("chain");

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.DisconnectEndPoint(1u), std::exception);
}

//! Checks ParentNode::DisconnectChild() when first and only one
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectChild_1st_OutOf_1 ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");

  sut.AppendChild(node_1);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DisconnectChild(node_1));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS  (sut.DirectChildrenCount(), 0);
  TS_ASSERT_NULLPTR (sut.FirstChild());
}


//! Checks ParentNode::DisconnectChild() when first out of 2
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectChild_1st_OutOf_2 ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");

  sut.AppendChild(node_1);
  sut.AppendChild(node_2);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DisconnectChild(node_1));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS     (sut.DirectChildrenCount(), 1);
  TS_ASSERT_EQUALS_PTR (sut.FirstChild(),          node_2);
}


//! Checks ParentNode::DisconnectChild() when 2nd out of 2
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectChild_2nd_OutOf_2 ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");

  sut.AppendChild(node_1);
  sut.AppendChild(node_2);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DisconnectChild(node_2));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS     (sut.DirectChildrenCount(), 1);
  TS_ASSERT_EQUALS_PTR (sut.FirstChild(),          node_1);
  TS_ASSERT_NULLPTR    (node_1->NextSibling());
}


//! Checks ParentNode::DisconnectChild() when 2nd out of 3
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectChild_2nd_OutOf_3 ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");
  auto node_3 = make_shared<Chain>("node 3");

  sut.AppendChild(node_1);
  sut.AppendChild(node_2);
  sut.AppendChild(node_3);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DisconnectChild(node_2));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS     (sut.DirectChildrenCount(), 2);
  TS_ASSERT_EQUALS_PTR (sut.FirstChild(),          node_1);
  TS_ASSERT_EQUALS_PTR (node_1->NextSibling(),     node_3);
}


//! Checks ParentNode::DisconnectChild() when node is not a child
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectChild_NotAChild ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");
  auto node_3 = make_shared<Chain>("node 3");

  sut.AppendChild(node_1);
  sut.AppendChild(node_2);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.DisconnectChild(node_3), std::exception);
}



//! Checks ParentNode::DisconnectChild() when there is no child nodes
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectChild_NoChild ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.DisconnectChild(node_1), std::exception);
}

//! Checks ParentNode::DisconnectChild() when node is nullptr
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectChild_Nullptr ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");

  sut.AppendChild(node_1);
  sut.AppendChild(node_2);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.DisconnectChild(nullptr), std::exception);
}

//! Checks ParentNode::DisconnectAllChildren()
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectAllChildren ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");
  auto node_3 = make_shared<Chain>("node 3");

  sut.AppendChild(node_1);
  sut.AppendChild(node_2);
  sut.AppendChild(node_3);

  // ---------------- Exercise
  //
  auto firstChild = sut.DisconnectAllChildren();

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS     (sut.DirectChildrenCount(), 0);
  TS_ASSERT_NULLPTR    (sut.FirstChild());
  TS_ASSERT_EQUALS_PTR (firstChild, node_1);
}


//! Checks ParentNode::DisconnectAllChildren() when there were none
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectAllChildren_When_None ()
{
  // ---------------- Setup
  //
  auto sut = Chain("chain");

  // ---------------- Exercise
  //
  auto firstChild = sut.DisconnectAllChildren();

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS  (sut.DirectChildrenCount(), 0);
  TS_ASSERT_NULLPTR (sut.FirstChild());
  TS_ASSERT_NULLPTR (firstChild);
}




//! Checks ParentNode::HasDirectChild() when first and only one
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_HasDirectChild_1st_OutOf_1 ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");

  sut.AppendChild(node_1);

  // ---------------- Exercise
  //
  bool hasDirectChild = sut.HasDirectChild(node_1);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (hasDirectChild);
}


//! Checks ParentNode::HasDirectChild() when first out of 2
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_HasDirectChild_1st_OutOf_2 ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");

  sut.AppendChild(node_1);
  sut.AppendChild(node_2);

  // ---------------- Exercise
  //
  bool hasDirectChild = sut.HasDirectChild(node_1);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (hasDirectChild);
}


//! Checks ParentNode::HasDirectChild() when 2nd out of 2
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_HasDirectChild_2nd_OutOf_2 ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");

  sut.AppendChild(node_1);
  sut.AppendChild(node_2);

  // ---------------- Exercise
  //
  bool hasDirectChild = sut.HasDirectChild(node_2);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (hasDirectChild);
}


//! Checks ParentNode::HasDirectChild() when 2nd out of 3
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_HasDirectChild_2nd_OutOf_3 ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");
  auto node_3 = make_shared<Chain>("node 3");

  sut.AppendChild(node_1);
  sut.AppendChild(node_2);
  sut.AppendChild(node_3);

  // ---------------- Exercise
  //
  bool hasDirectChild = sut.HasDirectChild(node_2);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (hasDirectChild);
}


//! Checks ParentNode::HasDirectChild() when node is not a child
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_HasDirectChild_NotAChild ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");
  auto node_3 = make_shared<Chain>("node 3");

  sut.AppendChild(node_1);
  sut.AppendChild(node_2);

  // ---------------- Exercise
  //
  bool hasDirectChild = sut.HasDirectChild(node_3);

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (hasDirectChild);
}

//! Checks ParentNode::HasDirectChild() when there is no child nodes
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_HasDirectChild_NoChild ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");

  // ---------------- Exercise
  //
  bool hasDirectChild = sut.HasDirectChild(node_1);

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (hasDirectChild);
}

//! Checks ParentNode::HasDirectChild() when node is nullptr
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_HasDirectChild_Nullptr ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");

  sut.AppendChild(node_1);
  sut.AppendChild(node_2);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.HasDirectChild(nullptr), std::exception);
}


//! Checks ParentNode::FindParentOfNode() when ParentNode is not manage by a shared_ptr
//!
void UT_ParentNode::test_FindParentOfNode_NotAShared_ptr ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");

  sut.AppendChild(node_1);
  node_1->AppendChild(node_2);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.FindParentOfNode(node_2), std::exception);
}

//! Checks ParentNode::FindParentOfNode() when node is a direct child
//!
void UT_ParentNode::test_FindParentOfNode_DirectChild ()
{
  // ---------------- Setup
  //
  auto sut    = make_shared<Chain>("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");
  auto node_3 = make_shared<Chain>("node 3");
  auto node_4 = make_shared<Chain>("node 4");

  sut->AppendChild(node_1);
  sut->AppendChild(node_2);
  sut->AppendChild(node_3);
  node_2->AppendChild(node_4);

  // ---------------- Exercise
  //
  auto parent = sut->FindParentOfNode(node_2);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS_PTR (parent, sut);
}


//! Checks ParentNode::FindParentOfNode() when node is a child of a direct child
//!
void UT_ParentNode::test_FindParentOfNode_Level_2_Child ()
{
  // ---------------- Setup
  //
  auto sut    = make_shared<Chain>("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");
  auto node_3 = make_shared<Chain>("node 3");
  auto node_4 = make_shared<Chain>("node 4");

  sut->AppendChild(node_1);
  sut->AppendChild(node_2);
  sut->AppendChild(node_3);
  node_2->AppendChild(node_4);

  // ---------------- Exercise
  //
  auto parent = sut->FindParentOfNode(node_4);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS_PTR (parent, node_2);
}


//! Checks ParentNode::FindParentOfNode() when node is a child of child of child
//!
void UT_ParentNode::test_FindParentOfNode_Level_3_Child ()
{
  // ---------------- Setup
  //
  auto sut    = make_shared<Chain>    ("chain");
  auto node_1 = make_shared<Register> ("reg 1_1", BinaryVector());
  auto node_2 = make_shared<Chain>    ("chain 1_2");
  auto node_3 = make_shared<Chain>    ("chain 1_3");
  auto node_4 = make_shared<Register> ("reg 2_1", BinaryVector());
  auto node_5 = make_shared<Chain>    ("chain 2_2");

  sut->AppendChild(node_1);
  sut->AppendChild(node_2);
  sut->AppendChild(node_3);
  node_3->AppendChild(node_5);
  node_5->AppendChild(node_4);

  // ---------------- Exercise
  //
  auto parent = sut->FindParentOfNode(node_4);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS_PTR (parent, node_5);
}


//! Checks ParentNode::FindParentOfNode() when node is a child of child of child of child
//!
void UT_ParentNode::test_FindParentOfNode_Level_4_Child ()
{
  // ---------------- Setup
  //
  auto sut    = make_shared<Chain>    ("chain");
  auto node_1 = make_shared<Register> ("reg 1_1", BinaryVector());
  auto node_2 = make_shared<Chain>    ("chain 1_2");
  auto node_3 = make_shared<Chain>    ("chain 1_3");
  auto node_4 = make_shared<Register> ("reg 2_1", BinaryVector());
  auto node_5 = make_shared<Chain>    ("chain 2_2");
  auto node_6 = make_shared<Register> ("reg 3_1", BinaryVector());
  auto node_7 = make_shared<Register> ("reg 3_2", BinaryVector());
  auto node_8 = make_shared<Chain>    ("chain 3_3");
  auto node_9 = make_shared<Register> ("reg 4_1", BinaryVector());

  sut->AppendChild(node_1);
  sut->AppendChild(node_2);
  sut->AppendChild(node_3);
  node_3->AppendChild(node_4);
  node_3->AppendChild(node_5);
  node_5->AppendChild(node_6);
  node_5->AppendChild(node_7);
  node_5->AppendChild(node_8);
  node_8->AppendChild(node_9);

  // ---------------- Exercise
  //
  auto parent = sut->FindParentOfNode(node_9);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS_PTR (parent, node_8);
}

//! Checks ParentNode::FindParentOfNode() when node is not a (grand)child
//!
void UT_ParentNode::test_FindParentOfNode_NotAChild ()
{
  // ---------------- Setup
  //
  auto sut    = make_shared<Chain>("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");
  auto node_3 = make_shared<Chain>("node 3");
  auto node_4 = make_shared<Chain>("node 4");

  sut->AppendChild(node_1);
  sut->AppendChild(node_2);
  node_2->AppendChild(node_3);

  // ---------------- Exercise
  //
  auto parent = sut->FindParentOfNode(node_4);

  // ---------------- Verify
  //
  TS_ASSERT_NULLPTR (parent);
}

//! Checks ParentNode::FindParentOfNode() when there is no child nodes
//!
void UT_ParentNode::test_FindParentOfNode_NoChild ()
{
  // ---------------- Setup
  //
  auto sut    = make_shared<Chain>("chain");
  auto node_1 = make_shared<Chain>("node 1");

  // ---------------- Exercise
  //
  auto parent = sut->FindParentOfNode(node_1);

  // ---------------- Verify
  //
  TS_ASSERT_NULLPTR (parent);
}

//! Checks ParentNode::FindParentOfNode() when node is nullptr
//!
void UT_ParentNode::test_FindParentOfNode_Nullptr ()
{
  // ---------------- Setup
  //
  auto sut    = make_shared<Chain>("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");
  auto node_3 = make_shared<Chain>("node 3");

  sut->AppendChild(node_1);
  sut->AppendChild(node_2);
  node_2->AppendChild(node_3);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut->FindParentOfNode(nullptr), std::exception);
}


//! Checks NodePathFindNoder::FindNode() giving valid path
//!
void UT_ParentNode::test_FindNode ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](const auto& data)
  {
    // ---------------- Setup
    //
    SystemModel sm;
    auto sut = CreateSystemModel(sm);

    auto path       = std::get<0>(data);
    auto expectedId = std::get<1>(data);

    // ---------------- Exercise
    //
    auto node = sut->FindNode(path);

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);

    TS_ASSERT_NOT_NULLPTR (node);
    TS_ASSERT_EQUALS      (node->Identifier(), expectedId);
  };

  auto data =
  {
    make_tuple(".",                         0u),  // 00
    make_tuple("Tap_IR",                    1u),  // 01
    make_tuple("Tap_BPY",                   3u),  // 02
    make_tuple("Chain_0",                   4u),  // 03
    make_tuple("Chain_0.Reg_1",             5u),  // 04
    make_tuple("Chain_0.Chain",             7u),  // 05
    make_tuple("Chain_0.Reg_4",             8u),  // 06
    make_tuple("Chain_0.Chain.Reg_1",       9u),  // 07
    make_tuple("Chain_0.Chain.Reg_2",       10u), // 08
    make_tuple("Chain_0.Chain.Chain",       11u), // 09
    make_tuple("Chain_0.Chain_2",           12u), // 10
    make_tuple("Chain_0.Reg_2",             13u), // 11
    make_tuple("Chain_0.Reg_3",             14u), // 12
    make_tuple("Chain_0.Chain.Chain.Reg_1", 15u), // 13
    make_tuple("Chain_0.Chain.Chain.Reg_2", 16u), // 14
    make_tuple("Chain_0.Chain_2.Reg_1",     17u), // 15
    make_tuple("Chain_0.Chain.Reg_5",       18u), // 16
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}

//! Checks ParentNode::FindNode() from a chain in middle of the hierarchy
//!
void UT_ParentNode::test_FindNode_FromMiddle ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](const auto& data)
  {
    // ---------------- Setup
    //
    SystemModel sm;
    auto tap = CreateSystemModel(sm);
    auto sut = sm.ChainWithId(7);

    auto path       = std::get<0>(data);
    auto expectedId = std::get<1>(data);

    // ---------------- Exercise
    //
    auto node = sut->FindNode(path);

    // ---------------- Verify
    //
    CxxTest::setAbortTestOnFail(true);

    TS_ASSERT_NOT_NULLPTR (node);
    TS_ASSERT_EQUALS      (node->Identifier(), expectedId);
  };

  auto data =
  {
    make_tuple(".",           7u),  // 00
    make_tuple("Reg_1",       9u),  // 01
    make_tuple("Reg_2",       10u), // 02
    make_tuple("Chain",       11u), // 03
    make_tuple("Chain.Reg_1", 15u), // 04
    make_tuple("Chain.Reg_2", 16u), // 05
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}

//! Checks NodePathFindNoder::FindNode() giving not valid path
//!
void UT_ParentNode::test_FindNode_NotValidPaths ()
{
  // ---------------- DDT Setup
  //
  auto checker = [&](string_view path)
  {
    // ---------------- Setup
    //
    SystemModel sm;
    auto sut = CreateSystemModel(sm);

    // ---------------- Exercise
    //
    auto node = sut->FindNode(path);

    // ---------------- Verify
    //
    TS_ASSERT_NULLPTR (node);
  };

  auto data =
  {
    "..",                  // 00
    "Tap_iR",              // 01
    "Chain",               // 02
    "Reg_1",               // 03
    "Chain_2.Chain",       // 04
    "Chain.Reg_3",         // 05
    "Chain.Reg_1",         // 06
    "Tap_DR_Mux.Chain_0",  // 07
    "Tap_DR_Mux.tap_BPY",  // 08
    "Chain_0/Chain.Chain", // 09
    "Chain_0..Reg_1",      // 10
    "Chain_0.Reg_5",       // 11
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}





//===========================================================================
// End of UT_ParentNode.cpp
//===========================================================================
