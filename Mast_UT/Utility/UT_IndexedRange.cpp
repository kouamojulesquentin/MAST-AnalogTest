//===========================================================================
//                           UT_IndexedRange.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_IndexedRange.cpp
//!
//! Implements test fixture for testing IndexedRange
//!
//===========================================================================

#include "UT_IndexedRange.hpp"
#include "IndexedRange.hpp"

#include "IndexedRange_Traits.hpp"

#include <tuple>

using std::tuple;
using std::make_tuple;
using mast::IndexedRange;



//! Checks IndexedRange default constructor
//!
void UT_IndexedRange::test_Constructor_Default ()
{
  // ---------------- Exercise
  //
  IndexedRange sut;

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.left,    0);
  TS_ASSERT_EQUALS (sut.right,   0);
  TS_ASSERT_EQUALS (sut.Width(), 1u);
  TS_ASSERT_TRUE   (sut.IsSingleBit());
  TS_ASSERT_FALSE  (sut.IncreasesTowardLeft());
  TS_ASSERT_FALSE  (sut.IncreasesTowardRight());
}



//! Checks IndexedRange constructor with single index
//!
void UT_IndexedRange::test_Constructor_SingleIndex ()
{
  auto checker = [](const auto& index)
  {
    // ---------------- Exercise
    //
    IndexedRange sut(index);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.left,    index);
    TS_ASSERT_EQUALS (sut.right,   index);
    TS_ASSERT_EQUALS (sut.Width(), 1u);
    TS_ASSERT_TRUE   (sut.IsSingleBit());
    TS_ASSERT_FALSE  (sut.IncreasesTowardLeft());
    TS_ASSERT_FALSE  (sut.IncreasesTowardRight());
  };

  auto data =
  {
    0u,          // 00
    1u,          // 01
    15u,         // 02
    UINT32_MAX , // 03
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks IndexedRange constructor with dual indexes
//!
void UT_IndexedRange::test_Constructor_DualIndex ()
{
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto leftIndex     = std::get<0>(data);
    auto rightIndex    = std::get<1>(data);
    auto expectedWidth = std::get<2>(data);

    // ---------------- Exercise
    //
    IndexedRange sut(leftIndex, rightIndex);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.left,                  leftIndex);
    TS_ASSERT_EQUALS (sut.right,                 rightIndex);
    TS_ASSERT_EQUALS (sut.Width(),               expectedWidth);
    TS_ASSERT_EQUALS (sut.IsSingleBit(),         leftIndex == rightIndex);
    TS_ASSERT_EQUALS (sut.IncreasesTowardLeft(),  leftIndex >  rightIndex);
    TS_ASSERT_EQUALS (sut.IncreasesTowardRight(), leftIndex <  rightIndex);
  };

  auto data =
  {        //  left,       right,      expected width
    make_tuple(0u,         0u,         1u),            // 00
    make_tuple(1u,         0u,         2u),            // 01
    make_tuple(0u,         1u,         2u),            // 02
    make_tuple(1u,         1u,         1u),            // 03
    make_tuple(2u,         1u,         2u),            // 04
    make_tuple(1u,         2u,         2u),            // 05
    make_tuple(UINT32_MAX, UINT32_MAX, 1u),            // 06
    make_tuple(1u,         UINT32_MAX, UINT32_MAX),    // 07
    make_tuple(2u,         UINT32_MAX, UINT32_MAX-1u), // 08
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks IndexedRange copy constructor
//!
void UT_IndexedRange::test_CopyConstructor ()
{
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto leftIndex     = std::get<0>(data);
    auto rightIndex    = std::get<1>(data);
    auto expectedWidth = std::get<2>(data);

    IndexedRange rhs(leftIndex, rightIndex);

    // ---------------- Exercise
    //
    IndexedRange sut(rhs);

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (sut.left,                  leftIndex);
    TS_ASSERT_EQUALS (sut.right,                 rightIndex);
    TS_ASSERT_EQUALS (sut.Width(),               expectedWidth);
    TS_ASSERT_EQUALS (sut.IsSingleBit(),         rhs.IsSingleBit());
    TS_ASSERT_EQUALS (sut.IncreasesTowardLeft(),  rhs.IncreasesTowardLeft());
    TS_ASSERT_EQUALS (sut.IncreasesTowardRight(), rhs.IncreasesTowardRight());
  };

  auto data =
  {        //  left,       right,      expected width
    make_tuple(0u,         0u,         1u),            // 00
    make_tuple(1u,         0u,         2u),            // 01
    make_tuple(0u,         1u,         2u),            // 02
    make_tuple(1u,         1u,         1u),            // 03
    make_tuple(20u,        15u,        6u),            // 04
    make_tuple(16u,        20u,        5u),            // 05
    make_tuple(UINT32_MAX, UINT32_MAX, 1u),            // 06
    make_tuple(1u,         UINT32_MAX, UINT32_MAX),    // 07
    make_tuple(2u,         UINT32_MAX, UINT32_MAX-1u), // 08
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks IndexedRange::Width when one index is 0 and the other is UINT32_MAX
//!
void UT_IndexedRange::test_Width_TooLarge ()
{
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto leftIndex  = std::get<0>(data);
    auto rightIndex = std::get<1>(data);

    IndexedRange sut(leftIndex, rightIndex);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut.Width(), std::runtime_error);
  };

  auto data =
  {
    make_tuple(UINT32_MAX, 0u),         // 0
    make_tuple(0u,         UINT32_MAX), // 1
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks IndexedRange::operator== when equal
//!
void UT_IndexedRange::test_Operator_EQ_when_Equal ()
{
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto leftIndex  = std::get<0>(data);
    auto rightIndex = std::get<1>(data);

    IndexedRange lhs(leftIndex, rightIndex);
    IndexedRange rhs(leftIndex, rightIndex);

    // ---------------- Exercise
    //
    auto areEqual = lhs == rhs;

    // ---------------- Verify
    //
    TS_ASSERT_TRUE (areEqual);
  };

  auto data =
  {
    make_tuple(0u,         0u),          // 00
    make_tuple(1u,         0u),          // 01
    make_tuple(0u,         1u),          // 02
    make_tuple(1u,         1u),          // 03
    make_tuple(20u,        15u),         // 04
    make_tuple(16u,        20u),         // 05
    make_tuple(UINT32_MAX, UINT32_MAX),  // 06
    make_tuple(1u,         UINT32_MAX),  // 07
    make_tuple(2u,         UINT32_MAX),  // 08
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks IndexedRange::operator== when not equal
//!
void UT_IndexedRange::test_Operator_EQ_when_Not_Equal ()
{
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto lhs = std::get<0>(data);
    auto rhs = std::get<1>(data);

    // ---------------- Exercise
    //
    auto areEqual = lhs == rhs;

    // ---------------- Verify
    //
    TS_ASSERT_FALSE (areEqual);
  };

  auto data =
  {
    make_tuple(IndexedRange(0u,  0u),  IndexedRange(0u,  1u)),  // 00
    make_tuple(IndexedRange(1u,  0u),  IndexedRange(0u,  1u)),  // 01
    make_tuple(IndexedRange(0u,  1u),  IndexedRange(1u,  0u)),  // 02
    make_tuple(IndexedRange(20u, 15u), IndexedRange(20u, 16u)), // 03
    make_tuple(IndexedRange(16u, 20u), IndexedRange(17u, 20u)), // 04
    make_tuple(IndexedRange(23,  17u), IndexedRange(5u,  11u)), // 05
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}




//! Checks IndexedRange::operator!= when equal
//!
void UT_IndexedRange::test_Operator_NEQ_when_Equal ()
{
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto leftIndex  = std::get<0>(data);
    auto rightIndex = std::get<1>(data);

    IndexedRange lhs(leftIndex, rightIndex);
    IndexedRange rhs(leftIndex, rightIndex);

    // ---------------- Exercise
    //
    auto areNotEqual = lhs != rhs;

    // ---------------- Verify
    //
    TS_ASSERT_FALSE (areNotEqual);
  };

  auto data =
  {
    make_tuple(0u,         0u),          // 00
    make_tuple(1u,         0u),          // 01
    make_tuple(0u,         1u),          // 02
    make_tuple(1u,         1u),          // 03
    make_tuple(20u,        15u),         // 04
    make_tuple(16u,        20u),         // 05
    make_tuple(UINT32_MAX, UINT32_MAX),  // 06
    make_tuple(1u,         UINT32_MAX),  // 07
    make_tuple(2u,         UINT32_MAX),  // 08
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks IndexedRange::operator!= when not equal
//!
void UT_IndexedRange::test_Operator_NEQ_when_Not_Equal ()
{
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto lhs = std::get<0>(data);
    auto rhs = std::get<1>(data);

    // ---------------- Exercise
    //
    auto areNotEqual = lhs != rhs;

    // ---------------- Verify
    //
    TS_ASSERT_TRUE (areNotEqual);
  };

  auto data =
  {
    make_tuple(IndexedRange(0u,  0u),  IndexedRange(0u,  1u)),  // 00
    make_tuple(IndexedRange(1u,  0u),  IndexedRange(0u,  1u)),  // 01
    make_tuple(IndexedRange(0u,  1u),  IndexedRange(1u,  0u)),  // 02
    make_tuple(IndexedRange(20u, 15u), IndexedRange(20u, 16u)), // 03
    make_tuple(IndexedRange(16u, 20u), IndexedRange(17u, 20u)), // 04
    make_tuple(IndexedRange(23,  17u), IndexedRange(5u,  11u)), // 05
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//===========================================================================
// End of UT_IndexedRange.cpp
//===========================================================================
