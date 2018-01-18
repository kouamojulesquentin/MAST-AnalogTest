//===========================================================================
//                           UT_SystemModelNode.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SystemModelNode.cpp
//!
//! Implements test fixture for testing SystemModelNode
//!
//===========================================================================

#include "UT_SystemModelNode.hpp"
#include "Chain.hpp"
#include "Conditioners.hpp"

#include <memory>
#include "Mast_Core_Traits.hpp"

using std::string;
using std::shared_ptr;
using std::make_shared;

using namespace mast;
using namespace std::string_literals;

namespace
{
  //! Instantiable Conditioners that do nothing at all
  //!
  class DummyConditioner : public Conditioners
  {
    virtual void DoPreConditioning  (std::shared_ptr<SystemModelNode> ) override {}
    virtual void DoPostConditioning (std::shared_ptr<SystemModelNode> ) override {}
  };
} // End of unnamed namespace

//! Checks SystemModelNode constructor
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_SystemModelNode::test_Constructor ()
{
  // ---------------- Setup
  //
  string name;
  name.append("A").append(" ").append("name");  // Make sure name is not constructed from a litteral (to avoid reference to it)

  // ---------------- Exercise
  //
  Chain sut(name);

  // ---------------- Verify (SystemModelNode part)
  //
  name = "This is no more initial name";  // To check that this is not a reference that is saved

  TS_ASSERT_EQUALS  (sut.Name(),     "A name");
  TS_ASSERT_EQUALS  (sut.Priority(), 0);
  TS_ASSERT_FALSE   (sut.IsPending());
  TS_ASSERT_FALSE   (sut.HasConditioner());
  TS_ASSERT_NULLPTR (sut.ApplicationData());
  TS_ASSERT_NULLPTR (sut.NextSibling());
}


//! Checks SystemModelNode::AppendSibling() first sibling
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_SystemModelNode::test_AppendSibling_1 ()
{
  // ---------------- Setup
  //
  auto sut       = Chain("A Name");
  auto otherNode = make_shared<Chain>("node");

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.AppendSibling(otherNode));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.NextSibling(), otherNode);
}

//! Checks SystemModelNode::AppendSibling() with a second node
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_SystemModelNode::test_AppendSibling_2 ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("A Name");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");

  sut.AppendSibling(node_1);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.AppendSibling(node_2));

  // ---------------- Verify
  //
  auto sibling_1 = sut.NextSibling();
  auto sibling_2 = sibling_1->NextSibling();

  TS_ASSERT_EQUALS (sibling_1, node_1);
  TS_ASSERT_EQUALS (sibling_2, node_2);
}


//! Checks SystemModelNode::SpliceSibling() first sibling
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_SystemModelNode::test_SpliceSibling_1st ()
{
  // ---------------- Setup
  //
  auto sut       = Chain("A Name");
  auto otherNode = make_shared<Chain>("node");

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.SpliceSibling(otherNode));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.NextSibling(), otherNode);
}

//! Checks SystemModelNode::SpliceSibling() when it has already a sibbling
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_SystemModelNode::test_SpliceSibling_2nd ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("A Name");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");

  sut.AppendSibling(node_1);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.SpliceSibling(node_2));

  // ---------------- Verify
  //
  auto sibling_1 = sut.NextSibling();
  TS_ASSERT_EQUALS (sibling_1, node_2);

  auto sibling_2 = sibling_1->NextSibling();
  TS_ASSERT_EQUALS (sibling_2, node_1);
}


//! Checks SystemModelNode::SpliceSibling() when sibling to insert has sibling(s)
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_SystemModelNode::test_SpliceSibling_NotSingleSibling ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("A Name");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");
  auto node_3 = make_shared<Chain>("node 3");

  node_2->AppendSibling(node_3);
  sut.AppendSibling(node_1);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS (sut.SpliceSibling(node_2), std::runtime_error);

  // ---------------- Verify
  //
  auto sibling_1 = sut.NextSibling();
  TS_ASSERT_EQUALS (sibling_1, node_1);

  auto sibling_2 = sibling_1->NextSibling();
  TS_ASSERT_EQUALS (sibling_2, nullptr);
}




//! Checks SystemModelNode::SetSibling()
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_SystemModelNode::test_SetSibling ()
{
  // ---------------- Setup
  //
  auto sut    = Chain("A Name");
  auto node_1 = make_shared<Chain>("node 1");
  auto node_2 = make_shared<Chain>("node 2");
  auto node_3 = make_shared<Chain>("node 3");

  sut.AppendSibling(node_1);
  sut.AppendSibling(node_2);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.SetNextSibling(node_3));

  // ---------------- Verify
  //
  auto sibling_1 = sut.NextSibling();
  auto sibling_2 = sibling_1->NextSibling();

  TS_ASSERT_EQUALS (sibling_1, node_3);
  TS_ASSERT_EQUALS (sibling_2, nullptr);
}


//! Checks SystemModelNode::SetPending()
//!
void UT_SystemModelNode::test_SetPending ()
{
  // ---------------- Setup
  //
  auto sut = Chain("A Name");

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.SetPending());

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (sut.IsPending());
}

//! Checks SystemModelNode::ResetPending()
//!
void UT_SystemModelNode::test_ResetPending ()
{
  // ---------------- Setup
  //
  auto sut = Chain("A Name");
  sut.SetPending();

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.ResetPending());

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (sut.IsPending());
}


//! Checks SystemModelNode::SetPriority()
//!
void UT_SystemModelNode::test_SetPriority ()
{
  // ---------------- Setup
  //
  auto sut = Chain("");

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.SetPriority(5));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS  (sut.Priority(), 5);
}


//! Checks SystemModelNode::SetName()
//!
void UT_SystemModelNode::test_SetName ()
{
  // ---------------- Setup
  //
  auto sut = Chain("");

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.SetName("Another name"));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.Name(), "Another name");
}

//! Checks SystemModelNode::SetApplicationData()
//!
void UT_SystemModelNode::test_SetApplicationData ()
{
  // ---------------- Setup
  //
  auto sut      = Chain("");
  auto someData = 0x4567;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.SetApplicationData(&someData));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.ApplicationData(), &someData);
}


//! Checks SystemModelNode::SetConditioners()
//!
void UT_SystemModelNode::test_SetConditioners ()
{
  // ---------------- Setup
  //
  auto sut         = Chain("");
  auto conditioner = make_shared<DummyConditioner>();

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.SetConditioners(conditioner));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.Conditioners(), conditioner);
  TS_ASSERT_TRUE   (sut.HasConditioner());
}


//! Checks SystemModelNode::ResetConditioners()
//!
void UT_SystemModelNode::test_ResetConditioners ()
{
  // ---------------- Setup
  //
  auto sut         = Chain("");
  auto conditioner = make_shared<DummyConditioner>();

  sut.SetConditioners(conditioner);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.ResetConditioners());

  // ---------------- Verify
  //
  TS_ASSERT_NULLPTR (sut.Conditioners());
  TS_ASSERT_FALSE   (sut.HasConditioner());
}

//===========================================================================
// End of UT_SystemModelNode.cpp
//===========================================================================
