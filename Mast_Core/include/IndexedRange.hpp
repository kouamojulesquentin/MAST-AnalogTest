//===========================================================================
//                           IndexedRange.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file IndexedRange.hpp
//!
//! Declares IndexedRange class
//!
//===========================================================================

#ifndef INDEXEDRANGE_H__755D90E6_D4DB_4D35_E6A3_DD366AE2ED17__INCLUDED_
  #define INDEXEDRANGE_H__755D90E6_D4DB_4D35_E6A3_DD366AE2ED17__INCLUDED_

#include "Utility.hpp"
#include <string>

#include <cstdint>

namespace mast
{
//! Defines a range of indexes
//!
//! @note This is intended to be use primarily in context of BinaryVector
//!
struct IndexedRange final
{
  uint32_t left  = 0; //!< 1st value
  uint32_t right = 0; //!< 2nd value

  ~IndexedRange() = default;

  IndexedRange() = default;

  //! Defines single bit range
  //!
  explicit IndexedRange(uint32_t index) : left(index), right(index) {  }

  //! Defines a multi-bits range
  //!
  //! @note It resolves to single bit range when given same value for both parameter)
  //!
  IndexedRange(uint32_t leftIndex, uint32_t rightIndex)
    : left  (leftIndex)
    , right (rightIndex)
  {}

  //! Copy constructs from other IndexedRange
  //!
  IndexedRange(const IndexedRange& other)
    : left  (other.left)
    , right (other.right)
  {
  }

  //! Assigns from another IndexedRange
  //!
  IndexedRange& operator=(const IndexedRange& rhs)
  {
    left  = rhs.left;
    right = rhs.right;
    return *this;
  }

  //! Returns true if compares equal to another IndexedRange
  //!
  bool operator==(const IndexedRange& rhs) const
  {
    return (left == rhs.left) && (right == rhs.right);
  }

  //! Returns true if compares not equal to another IndexedRange
  //!
  bool operator!=(const IndexedRange& rhs) const
  {
    return !(*this == rhs);
  }


  //! Returns a new IndexedRange with swapped indexes
  //!
  IndexedRange Reversed() const
  {
    return IndexedRange(right, left);
  }

  //! Returns true when left index is greater or same as right index
  //!
  bool HasStandardOrdering() const
  {
    return left >= right;
  }

  //! Returns number of bits for the range
  //!
  uint32_t Width() const
  {
    auto diff  = (left > right) ? left - right : right - left;
    auto width = 1u + diff;
    CHECK_VALUE_NEQ(width, 0, "Cannot give width for range or ["s.append(std::to_string(left)).append(std::to_string(right)).append("]"));

    return width;
  }
};
//
//  End of IndexedRange class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined INDEXEDRANGE_H__755D90E6_D4DB_4D35_E6A3_DD366AE2ED17__INCLUDED_

//===========================================================================
// End of IndexedRange.hpp
//===========================================================================
