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
#include <cxxtest/ValueTraits.h>

using std::shared_ptr;
using std::make_shared;
using namespace mast;


//! Checks ParentNode constructor
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_constructor ()
{
  // ---------------- Exercise
  //
  Chain sut("chain");

  // ---------------- Verify (ParentNode part)
  //
  TS_ASSERT_EQUALS  (sut.DirectChildrenCount(), 0);
  TS_ASSERT_NULLPTR (sut.FirstChild());
  TS_ASSERT_NULLPTR (sut.ChildAppender());
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

  // ---------------- Verify (ParentNode part)
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

  // ---------------- Verify (ParentNode part)
  //
  TS_ASSERT_EQUALS     (sut.DirectChildrenCount(), 2);
  TS_ASSERT_EQUALS_PTR (sut.FirstChild(),          node_1);
  TS_ASSERT_EQUALS_PTR (node_1->NextSibling(),     node_2);
}


//! Checks ParentNode::DisconnectDerivation() when first and only one
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectDerivation_1st_OutOf_1 ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("chain");
  auto node_1 = make_shared<Chain>("node 1");

  sut.AppendChild(node_1);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DisconnectDerivation(1u));

  // ---------------- Verify (ParentNode part)
  //
  TS_ASSERT_EQUALS  (sut.DirectChildrenCount(), 0);
  TS_ASSERT_NULLPTR (sut.FirstChild());
}


//! Checks ParentNode::DisconnectDerivation() when first out of 2
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectDerivation_1st_OutOf_2 ()
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
  TS_ASSERT_THROWS_NOTHING (sut.DisconnectDerivation(1u));

  // ---------------- Verify (ParentNode part)
  //
  TS_ASSERT_EQUALS     (sut.DirectChildrenCount(), 1);
  TS_ASSERT_EQUALS_PTR (sut.FirstChild(),          node_2);
}


//! Checks ParentNode::DisconnectDerivation() when 2nd out of 2
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectDerivation_2nd_OutOf_2 ()
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
  TS_ASSERT_THROWS_NOTHING (sut.DisconnectDerivation(2u));

  // ---------------- Verify (ParentNode part)
  //
  TS_ASSERT_EQUALS     (sut.DirectChildrenCount(), 1);
  TS_ASSERT_EQUALS_PTR (sut.FirstChild(),          node_1);
  TS_ASSERT_NULLPTR    (node_1->NextSibling());
}


//! Checks ParentNode::DisconnectDerivation() when 2nd out of 3
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectDerivation_2nd_OutOf_3 ()
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
  TS_ASSERT_THROWS_NOTHING (sut.DisconnectDerivation(2u));

  // ---------------- Verify (ParentNode part)
  //
  TS_ASSERT_EQUALS     (sut.DirectChildrenCount(), 2);
  TS_ASSERT_EQUALS_PTR (sut.FirstChild(),          node_1);
  TS_ASSERT_EQUALS_PTR (node_1->NextSibling(),     node_3);
}


//! Checks ParentNode::DisconnectDerivation() when 3rd out of 2
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectDerivation_3rd_OutOf_2 ()
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
  TS_ASSERT_THROWS (sut.DisconnectDerivation(3u), std::exception);
}


//! Checks ParentNode::DisconnectDerivation() when 0 out of 2
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectDerivation_0_OutOf_2 ()
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
  TS_ASSERT_THROWS (sut.DisconnectDerivation(0u), std::exception);
}


//! Checks ParentNode::DisconnectDerivation() when derivation 0 out of 0
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectDerivation_0_OutOf_0 ()
{
  // ---------------- Setup
  //
  auto sut = Chain("chain");

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.DisconnectDerivation(0u), std::exception);
}

//! Checks ParentNode::DisconnectDerivation() when derivation 1 out of 0
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectDerivation_1_OutOf_0 ()
{
  // ---------------- Setup
  //
  auto sut = Chain("chain");

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.DisconnectDerivation(1u), std::exception);
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

  // ---------------- Verify (ParentNode part)
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

  // ---------------- Verify (ParentNode part)
  //
  TS_ASSERT_EQUALS  (sut.DirectChildrenCount(), 0);
  TS_ASSERT_NULLPTR (sut.FirstChild());
  TS_ASSERT_NULLPTR (firstChild);
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

  // ---------------- Verify (ParentNode part)
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

//===========================================================================
// End of UT_ParentNode.cpp
//===========================================================================
