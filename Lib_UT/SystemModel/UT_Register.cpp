//===========================================================================
//                           UT_Register.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_Register.cpp
//!
//! Implements test fixture for testing Register
//!
//===========================================================================


#include "UT_Register.hpp"
#include "Register.hpp"
#include "BinaryVector_Traits.hpp"

using namespace mast;


//! Checks Register::Constructor() `...`
//!
void UT_Register::test_Constructor ()
{
  // ---------------- Setup
  //
  auto name           = "My register name";
  const auto bypassSequence = BinaryVector::CreateFromBinaryString("1111_1111:0");

  // ---------------- Exercise
  //
  Register sut(name, bypassSequence);

  // ---------------- Verify SystemModelNode part
  //
  TS_ASSERT_EQUALS  (sut.GetName(),     name);
  TS_ASSERT_EQUALS  (sut.GetPriority(), 0);
  TS_ASSERT_FALSE   (sut.IsPending());
  TS_ASSERT_FALSE   (sut.HasConditions());
  TS_ASSERT_NULLPTR (sut.GetApplicationData());
  TS_ASSERT_NULLPTR (sut.GetNextSibling());

  // ---------------- Verify Register part
  //
  TS_ASSERT_FALSE  (sut.MustCheckExpected());
  TS_ASSERT_EQUALS (sut.GetBypassSequence(),       bypassSequence);
  TS_ASSERT_EQUALS (sut.GetSequenceToSend(),       bypassSequence);
  TS_ASSERT_EQUALS (sut.GetLastSendSequence(),     bypassSequence);
  TS_ASSERT_EQUALS (sut.GetExpectedSequence(),     bypassSequence);
  TS_ASSERT_EQUALS (sut.GetLastReceivedSequence(), bypassSequence);
}

//===========================================================================
// End of UT_Register.cpp
//===========================================================================
