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
#include "VirtualRegister.hpp"
#include "RegistersAlias.hpp"
#include "GmlPrinter.hpp"  // To get a visual representation of testcase
#include "TestModelBuilder.hpp"
#include <tuple>
#include <memory>
#include <cxxtest/ValueTraits.h>

using std::make_tuple;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;
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
  TS_ASSERT_NULLPTR (sut.RegistersAliases());
  TS_ASSERT_FALSE   (sut.IgnoreForNodePath());
  TS_ASSERT_FALSE   (sut.HasAliases());
}


//! Checks ParentNode::AppendChild() when it is the first child
//!
//! @note As this is an abstract base class, it uses a Chain to construct an instance
//!
void UT_ParentNode::test_AppendChild_1st ()
{
  // ---------------- Setup
  //
  Chain sut("chain");
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


//! Checks ParentNode::AppendChild() when it is the second added child
//!
//! @note As this is an abstract base class, it uses a Chain to construct an instance
//!
void UT_ParentNode::test_AppendChild_2nd ()
{
  // ---------------- Setup
  //
  Chain sut("chain");
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


//! Checks ParentNode::PrependChild() when it is the first child
//!
//! @note As this is an abstract base class, it uses a Chain to construct an instance
//!
void UT_ParentNode::test_PrependChild_1st ()
{
  // ---------------- Setup
  //
  Chain sut("chain");
  auto node_1 = make_shared<Chain>("node 1");

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.PrependChild(node_1));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS     (sut.DirectChildrenCount(), 1);
  TS_ASSERT_EQUALS_PTR (sut.FirstChild(),          node_1);
  TS_ASSERT_NULLPTR    (sut.ChildAppender());
}





//! Checks ParentNode::PrependChild() when it is the second added child
//!
//! @note As this is an abstract base class, it uses a Chain to construct an instance
//!
void UT_ParentNode::test_PrependChild_2nd ()
{
  // ---------------- Setup
  //
  Chain sut("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");

  sut.PrependChild(node_1);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.PrependChild(node_2));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS     (sut.DirectChildrenCount(), 2);
  TS_ASSERT_EQUALS_PTR (sut.FirstChild(),          node_2);
  TS_ASSERT_EQUALS_PTR (node_2->NextSibling(),     node_1);
}

//! Checks ParentNode::PrependChild() when added child has sibblings
//!
//! @note As this is an abstract base class, it uses a Chain to construct an instance
//!       Chain are also the simpler nodes to construct
//!
void UT_ParentNode::test_PrependChild_with_Sibbling ()
{
  // ---------------- Setup
  //
  Chain sut("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");
  auto node_3 = make_shared<Chain>("node 3");
  auto node_4 = make_shared<Chain>("node 4");
  auto node_5 = make_shared<Chain>("node 5");

  node_3->AppendSibling(node_4);
  node_3->AppendSibling(node_5);

  sut.PrependChild(node_1);
  sut.PrependChild(node_2);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.PrependChild(node_3));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS     (sut.DirectChildrenCount(), 5);
  TS_ASSERT_EQUALS_PTR (sut.FirstChild(),          node_3);
  TS_ASSERT_EQUALS_PTR (node_3->NextSibling(),     node_4);
  TS_ASSERT_EQUALS_PTR (node_4->NextSibling(),     node_5);
  TS_ASSERT_EQUALS_PTR (node_5->NextSibling(),     node_2);
  TS_ASSERT_EQUALS_PTR (node_2->NextSibling(),     node_1);
  TS_ASSERT_NULLPTR    (node_1->NextSibling());
  TS_ASSERT_NULLPTR    (sut.ChildAppender());
}

//! Checks ParentNode::SetChildAppender() checking using ParentNode::Append
//!
//! @note As this is an abstract base class, it uses a Chain to construct an instance
//!
void UT_ParentNode::test_SetChildAppender_with_Append ()
{
  // ---------------- Setup
  //
  Chain sut("chain");
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


//! Checks ParentNode::SetChildAppender() checking using ParentNode::Prepend
//!
//! @note As this is an abstract base class, it uses a Chain to construct an instance
//!
void UT_ParentNode::test_SetChildAppender_with_Prepend ()
{
  // ---------------- Setup
  //
  Chain sut("chain");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");
  auto node_3 = make_shared<Chain>("node 3");
  auto node_4 = make_shared<Chain>("node 4");
  auto node_5 = make_shared<Chain>("node 5");

  sut.AppendChild(node_1);
  sut.AppendChild(node_2);
  sut.AppendChild(node_3);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.SetChildAppender(node_2));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS_PTR (sut.ChildAppender(), node_2);

  sut.PrependChild(node_4);
  sut.PrependChild(node_5);

  TS_ASSERT_EQUALS     (sut.DirectChildrenCount(),     3);
  TS_ASSERT_EQUALS_PTR (sut.FirstChild(),              node_1);
  TS_ASSERT_EQUALS_PTR (node_1->NextSibling(),         node_2);
  TS_ASSERT_EQUALS_PTR (node_2->NextSibling(),         node_3);
  TS_ASSERT_NULLPTR    (node_3->NextSibling());
  TS_ASSERT_EQUALS     (node_2->DirectChildrenCount(), 2);
  TS_ASSERT_EQUALS_PTR (node_2->FirstChild(),          node_5);
  TS_ASSERT_EQUALS_PTR (node_5->NextSibling(),         node_4);
  TS_ASSERT_NULLPTR    (node_4->NextSibling());
}


//! Checks ParentNode::DisconnectEndPoint() when first and only one
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_ParentNode::test_DisconnectEndPoint_1st_OutOf_1 ()
{
  // ---------------- Setup
  //
  Chain sut("chain");
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
  Chain sut("chain");
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
  Chain sut("chain");
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
  Chain sut("chain");
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
  Chain sut("chain");
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
  Chain sut("chain");
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
  Chain sut("chain");

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
  Chain sut("chain");

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
  Chain sut("chain");
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
  Chain sut("chain");
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
  Chain sut("chain");
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
  Chain sut("chain");
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
  Chain sut("chain");
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
  Chain sut("chain");
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
  Chain sut("chain");
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
  Chain sut("chain");
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
  Chain sut("chain");

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
  Chain sut("chain");
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
  Chain sut("chain");
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
  Chain sut("chain");
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
  Chain sut("chain");
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
  Chain sut("chain");
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
  Chain sut("chain");
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
  Chain sut("chain");
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
  Chain sut("chain");
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



//! Checks ParentNode::AddAlias() when it is 1st alias
//!
void UT_ParentNode::test_AddAlias_1st ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto topNode = CreateSystemModel(sm);   // See that function for details related to verification part

  CxxTest::setAbortTestOnFail(true);
  auto sut   = dynamic_pointer_cast<ParentNode> (topNode->FindNode("Chain_0"));;
  auto reg_1 = dynamic_pointer_cast<Register>   (topNode->FindNode("Chain_0.Reg_1"));
  auto reg_2 = dynamic_pointer_cast<Register>   (topNode->FindNode("Chain_0.Reg_4"));

  TS_ASSERT_NOT_NULLPTR (sut);
  TS_ASSERT_NOT_NULLPTR (reg_1);
  TS_ASSERT_NOT_NULLPTR (reg_2);

  // Create first alias
  VirtualRegister virtualRegister_1;
  {
    auto regSlice_1 = RegisterSlice{reg_1, IndexedRange{4, 1}};
    auto regSlice_2 = RegisterSlice{reg_2, IndexedRange{2, 1}};

    virtualRegister_1.Append(regSlice_1);
    virtualRegister_1.Append(regSlice_2);
  }
  RegistersAlias alias("Foo"s, std::move(virtualRegister_1));

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut->AddAlias(std::move(alias)));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE        (sut->HasAliases());
  TS_ASSERT_NOT_NULLPTR (sut->RegistersAliases());
  TS_ASSERT_EQUALS      (sut->RegistersAliases()->size(), 1u);
}


//! Checks ParentNode::AddAlias() when it is 2nd alias
//!
void UT_ParentNode::test_AddAlias_2nd ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto topNode = CreateSystemModel(sm);   // See that function for details related to verification part

  CxxTest::setAbortTestOnFail(true);
  auto sut   = dynamic_pointer_cast<ParentNode> (topNode->FindNode("Chain_0"));;
  auto reg_1 = dynamic_pointer_cast<Register>   (topNode->FindNode("Chain_0.Reg_1"));
  auto reg_2 = dynamic_pointer_cast<Register>   (topNode->FindNode("Chain_0.Reg_4"));

  TS_ASSERT_NOT_NULLPTR (sut);
  TS_ASSERT_NOT_NULLPTR (reg_1);
  TS_ASSERT_NOT_NULLPTR (reg_2);

  // Create 1st alias
  VirtualRegister virtualRegister_1;
  {
    virtualRegister_1.Append({reg_1, IndexedRange{4, 1}});
    virtualRegister_1.Append({reg_2, IndexedRange{2, 1}});
  }
  RegistersAlias alias_1("Foo"s, std::move(virtualRegister_1));

  // Create 2nd alias
  VirtualRegister virtualRegister_2;
  {
    virtualRegister_2.Append({reg_1, IndexedRange{4, 0}});
    virtualRegister_2.Append({reg_2, IndexedRange{1, 0}});
  }
  RegistersAlias alias_2("Bar", std::move(virtualRegister_2));

  TS_ASSERT_THROWS_NOTHING (sut->AddAlias(std::move(alias_1)));

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut->AddAlias(std::move(alias_2)));

  // ---------------- Verify
  //
  TS_ASSERT_TRUE        (sut->HasAliases());
  TS_ASSERT_NOT_NULLPTR (sut->RegistersAliases());
  TS_ASSERT_EQUALS      (sut->RegistersAliases()->size(), 2u);
}


//! Checks ParentNode::FindRegister() when it is 2nd alias
//!
void UT_ParentNode::test_FindRegister ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto registerName      = std::get<0>(data);
    auto expectedBitsCount = std::get<1>(data);

    SystemModel sm;
    auto topNode = CreateSystemModel(sm);   // See that function for details related to verification part

    CxxTest::setAbortTestOnFail(true);

    auto sut       = dynamic_pointer_cast<ParentNode> (topNode->FindNode("Chain_0"));;
    auto chain_1   = dynamic_pointer_cast<ParentNode> (topNode->FindNode("Chain_0.Chain"));;
    auto chain_2   = dynamic_pointer_cast<ParentNode> (topNode->FindNode("Chain_0.Chain_2"));;
    auto reg_1     = dynamic_pointer_cast<Register>   (topNode->FindNode("Chain_0.Reg_1"));
    auto reg_2     = dynamic_pointer_cast<Register>   (topNode->FindNode("Chain_0.Chain.Reg_1"));
    auto reg_3     = dynamic_pointer_cast<Register>   (topNode->FindNode("Chain_0.Chain.Reg_2"));
    auto reg_4     = dynamic_pointer_cast<Register>   (topNode->FindNode("Chain_0.Reg_4"));
    auto reg_lvl_3 = dynamic_pointer_cast<Register>   (topNode->FindNode("Chain_0.Chain_2.Reg_1"));

    TS_ASSERT_NOT_NULLPTR (sut);
    TS_ASSERT_NOT_NULLPTR (chain_1);
    TS_ASSERT_NOT_NULLPTR (chain_2);
    TS_ASSERT_NOT_NULLPTR (reg_1);
    TS_ASSERT_NOT_NULLPTR (reg_2);
    TS_ASSERT_NOT_NULLPTR (reg_3);
    TS_ASSERT_NOT_NULLPTR (reg_4);
    TS_ASSERT_NOT_NULLPTR (reg_lvl_3);

    // Create 1st alias
    VirtualRegister virtualRegister_1;
    virtualRegister_1.Append({reg_1, IndexedRange{4, 1}});
    virtualRegister_1.Append({reg_4, IndexedRange{2, 1}});
    sut->AddAlias({"Foo"s, std::move(virtualRegister_1)});

    // Create 2nd alias
    VirtualRegister virtualRegister_2;
    virtualRegister_2.Append({reg_1, IndexedRange{4, 0}});
    virtualRegister_2.Append({reg_4, IndexedRange{1, 0}});
    sut->AddAlias({"Bar", std::move(virtualRegister_2)});

    // Create 3rd alias
    VirtualRegister virtualRegister_3({reg_lvl_3, IndexedRange{3, 1}});
    sut->AddAlias({"Boat", std::move(virtualRegister_3)});

    // Create 4th alias
    VirtualRegister virtualRegister_4(reg_lvl_3);
    sut->AddAlias({"Bounty", std::move(virtualRegister_4)});

    // Create 5th alias
    VirtualRegister virtualRegister_5(reg_lvl_3);
    virtualRegister_5.Append({reg_2, IndexedRange{3, 0}});
    virtualRegister_5.Append({reg_3, IndexedRange{3, 0}});
    chain_1->AddAlias({"Picus", std::move(virtualRegister_5)});


    // ---------------- Exercise
    //
    auto foundRegister = sut->FindRegister(registerName);

    // ---------------- Verify
    //
    TS_ASSERT_NOT_NULLPTR (foundRegister);
    TS_ASSERT_EQUALS      (foundRegister->BitsCount(), expectedBitsCount);
  };

  auto data =
  {
    make_tuple("Reg_1",         5),  //  0 ==> Plain name
    make_tuple("Reg_4",         4),  //  1 ==> Plain name
    make_tuple("Foo",           6),  //  2 ==> Alias
    make_tuple("Bar",           7),  //  3 ==> Alias
    make_tuple(".Reg_4",        4),  //  4 ==> Single dot is ignored (plain name)
    make_tuple(".Foo",          6),  //  5 ==> Single dot is ignored (alias)
    make_tuple("Chain_2.Reg_1", 6),  //  6 ==> Plain name
    make_tuple("Chain.Reg_1",   5),  //  7 ==> Plain name
    make_tuple("Chain.Reg_5",   7),  //  8 ==> Plain name
    make_tuple("Chain.Reg_5",   7),  //  9 ==> Plain name
    make_tuple("Boat",          3),  // 10 ==> Alias
    make_tuple("Bounty",        6),  // 11 ==> Alias
    make_tuple("Chain.Picus",   14), // 12 ==> Alias at 2nd level
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks ParentNode::FindRegister() for an alias found by traversing "transparent" parent nodes
//!
void UT_ParentNode::test_FindRegister_deep_alias ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto regsBitsCount = 32u;

  TestModelBuilder builder(sm);
  auto sut = builder.Create_TestCase_MIB_Multichain_Pre("TAP", 4u, regsBitsCount);  // Sut is tap node (first node in the hierarchy)

  auto mux      = sm.LinkerWithId(2u);   TS_ASSERT_NOT_NULLPTR (mux);
  auto regDyn_0 = sm.RegisterWithId(6u); TS_ASSERT_NOT_NULLPTR (regDyn_0);
  auto regDyn_1 = sm.RegisterWithId(7u); TS_ASSERT_NOT_NULLPTR (regDyn_1);
  auto regDyn_2 = sm.RegisterWithId(8u); TS_ASSERT_NOT_NULLPTR (regDyn_2);
  auto regDyn_3 = sm.RegisterWithId(9u); TS_ASSERT_NOT_NULLPTR (regDyn_3);

  regDyn_0->SetToSut(BinaryVector(regsBitsCount, 0x60));
  regDyn_1->SetToSut(BinaryVector(regsBitsCount, 0x61));
  regDyn_2->SetToSut(BinaryVector(regsBitsCount, 0x62));
  regDyn_3->SetToSut(BinaryVector(regsBitsCount, 0x63));

  VirtualRegister virtualRegister;
  virtualRegister.Append({regDyn_1, IndexedRange{31, 24}});
  virtualRegister.Append({regDyn_2, IndexedRange{23, 16}});
  virtualRegister.Append({regDyn_3, IndexedRange{15, 8}});
  mux->AddAlias({"Foo"s, std::move(virtualRegister)});    // ==> 24 bits

  // ---------------- Exercise
  //
  auto foundRegister = sut->FindRegister("Foo");

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (foundRegister);
  TS_ASSERT_EQUALS      (foundRegister->BitsCount(), 24u);
}


//===========================================================================
// End of UT_ParentNode.cpp
//===========================================================================
