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
#include <memory>
#include <cxxtest/ValueTraits.h>

using std::shared_ptr;
using std::make_shared;
using namespace mast;

//! Initializes test (called for each test)
void UT_SystemModelNode::setUp ()
{
}


//! Checks SystemModelNode constructor
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_SystemModelNode::test_Constructor ()
{
  // ---------------- Setup
  //
  auto name = "A name";

  // ---------------- Exercise
  //
  Chain sut(name);

  // ---------------- Verify (SystemModelNode part)
  //
  TS_ASSERT_EQUALS  (sut.Name(),     name);
  TS_ASSERT_EQUALS  (sut.Priority(), 0);
  TS_ASSERT_FALSE   (sut.IsPending());
  TS_ASSERT_FALSE   (sut.HasConditions());
  TS_ASSERT_NULLPTR (sut.ApplicationData());
  TS_ASSERT_NULLPTR (sut.NextSibling());
}


//! Checks SystemModelNode::AppendSibling()
//!
//! @note As this is an abstract base class, it uses a Chain to have an instance
//!
void UT_SystemModelNode::test_AppendSibling ()
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


//! @todo [JFC]-[May/26/2016]: Remove "No_test_yet_for_Guard" method when all tests are implemented
//!
void UT_SystemModelNode::test_No_test_yet_for_Guard ()
{
  TS_WARN ("Not all tests yet for: ConditionChecker");
  TS_ASSERT_TRUE (true); // Avoid the "no assertion" warning (there is already a message)
}




//===========================================================================
// End of UT_SystemModelNode.cpp
//===========================================================================
