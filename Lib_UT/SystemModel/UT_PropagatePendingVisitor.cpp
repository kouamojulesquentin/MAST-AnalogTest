//===========================================================================
//                           UT_PropagatePendingVisitor.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_PropagatePendingVisitor.cpp
//!
//! Implements test fixture for testing PropagatePendingVisitor
//!
//===========================================================================


#include "UT_PropagatePendingVisitor.hpp"
#include "PropagatePendingVisitor.hpp"
#include "SystemModelBuilder.hpp"
#include <cxxtest/ValueTraits.h>

using namespace mast;
using namespace test;


//! Initializes test (called for each test)
void UT_PropagatePendingVisitor::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}


//! Checks PropagatePendingVisitor::Constructor() `...`
//!
void UT_PropagatePendingVisitor::test_Constructor ()
{
  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (PropagatePendingVisitor sut);
}


//! Checks PropagatePendingVisitor::Visitxxx() `...`
//!
void UT_PropagatePendingVisitor::test_Visit_xxx ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);

  auto root = builder.Create_Simple_MIB();

  auto mib_Ctrl  = sm.RegisterWithId(3u);
  auto linker    = sm.LinkerWithId(4u);
  auto regStatic = sm.RegisterWithId(1u);
  auto regDyn_0  = sm.RegisterWithId(5u);
  auto regDyn_1  = sm.RegisterWithId(6u);
  auto regDyn_2  = sm.RegisterWithId(7u);
  auto regDyn_3  = sm.RegisterWithId(8u);

  mib_Ctrl->SetToSut   (BinaryVector::CreateFromBinaryString("10"));
  regStatic->SetToSut  (BinaryVector(STATIC_TDR_LEN,  0x05));
  regDyn_0->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x09));
  regDyn_1->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x0A));
  regDyn_2->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0xB0));
  regDyn_3->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0xC0));

  TS_ASSERT_FALSE (root->IsPending());
  TS_ASSERT_FALSE (mib_Ctrl->IsPending());
  TS_ASSERT_FALSE (linker->IsPending());
  TS_ASSERT_FALSE (regDyn_0->IsPending());

  PropagatePendingVisitor sut;

  // ---------------- Exercise
  //
  root->Accept(sut);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (root->IsPending());
  TS_ASSERT_TRUE (mib_Ctrl->IsPending());
  TS_ASSERT_TRUE (linker->IsPending());
  TS_ASSERT_TRUE (regDyn_0->IsPending());
}


//===========================================================================
// End of UT_PropagatePendingVisitor.cpp
//===========================================================================
