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


//! Checks Register:: constructor
//!
//! @note SystemModelNode parts have been test elsewhere
void UT_Register::test_Constructor ()
{
  // ---------------- Setup
  //
  auto name           = "A name";
  auto bypassSequence = BinaryVector::CreateFromBinaryString("1111_1111:0");

  // ---------------- Exercise
  //
  Register sut(name, bypassSequence);

  // ---------------- Verify
  //
  TS_ASSERT_FALSE  (sut.MustCheckExpected());
  TS_ASSERT_EQUALS (sut.BitsCount(),       9U);
  TS_ASSERT_EQUALS (sut.BypassSequence(),  bypassSequence);
  TS_ASSERT_EQUALS (sut.NextToSut(),       bypassSequence);
  TS_ASSERT_EQUALS (sut.LastToSut(),       bypassSequence);
  TS_ASSERT_EQUALS (sut.ExpectedFromSut(), bypassSequence);
  TS_ASSERT_EQUALS (sut.LastFromSut(),     bypassSequence);
}


//! Checks Register::SetBypass() with a proper value
//!
void UT_Register::test_SetBypass ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:0");

  Register sut("Reg", initial);

  // ---------------- Exercise
  //
  sut.SetBypass(newValue);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.BypassSequence(), newValue);
}

//! Checks Register::SetBypass() with a too small value
//!
void UT_Register::test_SetBypass_TooSmall ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111");

  Register sut("Reg", initial);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.SetBypass(newValue), std::exception);
}


//! Checks Register::SetBypass() with a too large value
//!
void UT_Register::test_SetBypass_TooLarge ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:01");

  Register sut("Reg", initial);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.SetBypass(newValue), std::exception);
}

//! Checks Register::SetToSut() with a proper value
//!
void UT_Register::test_SetToSut ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:0");

  Register sut("Reg", initial);

  // ---------------- Exercise
  //
  sut.SetToSut(newValue);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.NextToSut(), newValue);
  TS_ASSERT_EQUALS (sut.LastToSut(), initial);
}

//! Checks Register::SetToSut() with a value of different size
//!
void UT_Register::test_SetToSut_DifferentSize ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111");

  Register sut("Reg", initial);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.SetToSut(newValue), std::exception);
}

//! Checks Register::SetExpectedFromSut() with a proper value
//!
void UT_Register::test_SetExpectedFromSut ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:0");

  Register sut("Reg", initial);

  // ---------------- Exercise
  //
  sut.SetExpectedFromSut(newValue);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.ExpectedFromSut(), newValue);
}

//! Checks Register::SetExpectedFromSut() with a value of different size
//!
void UT_Register::test_SetExpectedFromSut_DifferentSize ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111");

  Register sut("Reg", initial);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.SetExpectedFromSut(newValue), std::exception);
}

//! Checks Register::SetFromSut() with a value different from expected one and check disabled
//!
void UT_Register::test_SetFromSut_DifferingFromExpected_WithoutCheck ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:0");
  const auto expected = BinaryVector::CreateFromBinaryString("1110_0111:1");

  Register sut("Reg", initial);
  sut.SetExpectedFromSut(expected);

  // ---------------- Exercise
  //
  sut.SetFromSut(newValue);

  // ---------------- Verify
  //
  TS_ASSERT_FALSE  (sut.MustCheckExpected());
  TS_ASSERT_EQUALS (sut.LastFromSut(), newValue);
  TS_ASSERT_EQUALS (sut.Mismatches(), 0);
}

//! Checks Register::SetFromSut() with a value different from expected one and check enabled
//!
void UT_Register::test_SetFromSut_DifferingFromExpected_WithCheck ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:0");
  const auto expected = BinaryVector::CreateFromBinaryString("1110_0111:1");

  Register sut("Reg", initial);
  sut.SetExpectedFromSut(expected);
  sut.SetCheckExpected(true);

  // ---------------- Exercise
  //
  sut.SetFromSut(newValue);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.MustCheckExpected());
  TS_ASSERT_EQUALS (sut.LastFromSut(), newValue);
  TS_ASSERT_EQUALS (sut.Mismatches(), 1);
}


//! Checks Register::SetFromSut() with a value of different size
//!
void UT_Register::test_SetFromSut_DifferentSize ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111");

  Register sut("Reg", initial);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.SetFromSut(newValue), std::exception);
}



//! Checks Register::UpdateLastToSut()
//!
void UT_Register::test_UpdateLastToSut ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:1");

  Register sut("Reg", initial);
  sut.SetToSut(newValue);

  // ---------------- Exercise
  //
  sut.UpdateLastToSut();

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.LastToSut(), newValue);
}

//! Checks Register::ResetMismatches()
//!
void UT_Register::test_ResetMismatches ()
{
  // ---------------- Setup
  //
  const auto initial  = BinaryVector::CreateFromBinaryString("1111_1111:0");
  const auto newValue = BinaryVector::CreateFromBinaryString("1110_0111:0");
  const auto expected = BinaryVector::CreateFromBinaryString("1110_0111:1");

  Register sut("Reg", initial);
  sut.SetExpectedFromSut(expected);
  sut.SetCheckExpected(true);
  sut.SetFromSut(newValue);

  // ---------------- Exercise
  //
  sut.ResetMismatches();

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (sut.MustCheckExpected());
  TS_ASSERT_EQUALS (sut.Mismatches(), 0);
}

//===========================================================================
// End of UT_Register.cpp
//===========================================================================
