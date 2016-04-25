//===========================================================================
//                           UT_BinaryVector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_BinaryVector.cpp
//!
//! Implements test fixture for testing BinaryVector
//!
//===========================================================================

#include "UT_BinaryVector.hpp"
#include "ScanVectors.hpp"
#include <cxxtest/ValueTraits.h>

using mast::BinaryVector;



//! Checks BinaryVector defaut constructor
//!
void UT_BinaryVector::test_Constructor_Default ()
{
  // ---------------- Exercise
  //
  BinaryVector sut;

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS  (sut.BitCount(),   0);
  TS_ASSERT_EQUALS  (sut.BytesCount(), 0);
  TS_ASSERT_NULLPTR (sut.Data());
}


//! Checks BinaryVector copyt constructor when the source BinaryVector is empty
//!
void UT_BinaryVector::test_Constructor_Copy_When_SrcIsEmpty ()
{
  // ---------------- Setup
  //
  const BinaryVector srce;

  // ---------------- Exercise
  //
  BinaryVector sut(srce);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS  (sut.BitCount(),   0);
  TS_ASSERT_EQUALS  (sut.BytesCount(), 0);
  TS_ASSERT_NULLPTR (sut.Data());
}

//! Checks BinaryVector move constructor when the source BinaryVector is empty
//!
void UT_BinaryVector::test_Constructor_Move_When_SrcIsEmpty ()
{
  // ---------------- Setup
  //
  BinaryVector srce;

  // ---------------- Exercise
  //
  BinaryVector sut(std::move(srce));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS  (sut.BitCount(),   0);
  TS_ASSERT_EQUALS  (sut.BytesCount(), 0);
  TS_ASSERT_NULLPTR (sut.Data());
}


//! Checks Append when sut is empty and adding 8 bits from uint8_t
//!
void UT_BinaryVector::test_Append_8_bits_When_Empty ()
{
  // ---------------- Setup
  //
  const uint8_t value = 0xA5;
  BinaryVector  sut;

  // ---------------- Exercise
  //
  sut.Append(value);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS      (sut.BitCount(),   8);
  TS_ASSERT_EQUALS      (sut.BytesCount(), 1);
  TS_ASSERT_NOT_NULLPTR (sut.Data());
  TS_ASSERT_EQUALS      (sut.Data()[0],    value);
}


//! @todo [JFC]-[April/25/2016]: Remove "No_test_yet_for_Guard" method when all tests are implemented
//!
void UT_BinaryVector::test_No_test_yet_for_Guard ()
{
  TS_WARN ("No test yet for: append with different width and bits count");
}




//===========================================================================
// End of UT_BinaryVector.cpp
//===========================================================================
