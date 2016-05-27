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
#include <cxxtest/ValueTraits.h>

using std::shared_ptr;
using std::make_shared;
using namespace mast;

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
  auto name = "A name";

  // ---------------- Exercise
  //
  Chain sut(name);

  // ---------------- Verify (SystemModelNode part)
  //
  TS_ASSERT_EQUALS  (sut.Name(),     name);
  TS_ASSERT_EQUALS  (sut.Priority(), 0);
  TS_ASSERT_FALSE   (sut.IsPending());
  TS_ASSERT_FALSE   (sut.HasConditioner());
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
