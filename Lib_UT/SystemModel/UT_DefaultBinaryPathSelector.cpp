//===========================================================================
//                           UT_DefaultBinaryPathSelector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_DefaultBinaryPathSelector.cpp
//!
//! Implements test fixture for testing DefaultBinaryPathSelector
//!
//===========================================================================


#include "UT_DefaultBinaryPathSelector.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "BinaryVector_Traits.hpp"
#include "Register.hpp"

#include <memory>
using std::make_shared;

using namespace mast;


//! Checks DefaultBinaryPathSelector constructor
//!
void UT_DefaultBinaryPathSelector::test_Constructor ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("1111_1111:0");
  auto associatedNode = make_shared<Register>("My register name", bypassSequence);

  // ---------------- Exercise
  //

  // ---------------- Verify
  //
}

//! @todo [JFC]-[April/29/2016]: Remove "No_test_yet_for_Guard" method when all tests are implemented
//!
//+void UT_DefaultBinaryPathSelector::test_No_test_yet_for_Guard ()
//+{
//+  TS_WARN ("No test yet for: `method_name`");
//+}




//===========================================================================
// End of UT_DefaultBinaryPathSelector.cpp
//===========================================================================
