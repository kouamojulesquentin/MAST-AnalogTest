//===========================================================================
//                           Utility.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Utility.cpp
//!
//! Implements Utility class methods
//!
//===========================================================================

#include "Utility.hpp"

#include <string>
#include <sstream>
#include <array>

using std::string;

using namespace mast;

//! Formats message for (std) exception
//!
//! @note It can also be used for non std::exception derived class
//! @note Format example:
//!   std::invalid_argument: Number of append bits cannot exceed number of bits of value.
//!   In File:     C:\Users\Jean-Francois\Documents\Tima\NewMast\Lib\src\BinaryVector.cpp
//!   In Function:
//!   At Line:     87'
//!
string Utility::MakeExceptionMessage(const char*                    file,
                                     const char*                    function,
                                     uint32_t                       line,
                                     std::experimental::string_view issueKind,
                                     std::experimental::string_view userMessage)
{
  std::ostringstream os;

  os << issueKind << ": " << userMessage << "." << std::endl;
  os << "In file:     "  << file << std::endl;
  if (function)
  {
    os << "In function: "  << function << std::endl;
  }
  os << "At line:     "  << line << std::endl;

  return os.str();
}


//! Returns the minimal number of bits to encode a binary value
//!
//! @note This is not optimized (is faster for small value than for large one)
//!
//! @param value  A value for which the minimal number of bits is required
//!
uint32_t Utility::MinimalBitsForValue (uint32_t value)
{
  constexpr std::array<uint32_t, 16> bitsForVal =
  {
    1,  // 00
    1,  // 01
    2,  // 02
    2,  // 03
    3,  // 04
    3,  // 05
    3,  // 06
    3,  // 07
    4,  // 08
    4,  // 09
    4,  // 10
    4,  // 11
    4,  // 12
    4,  // 13
    4,  // 14
    4,  // 15
  };

  uint32_t bitsCount = 0;

  while (value > 15)
  {
    bitsCount +=  4;
    value     >>= 4;
  }

  bitsCount += bitsForVal[value];

  return bitsCount;
}
//
//  End of: Utility::MinimalBitsForValue
//---------------------------------------------------------------------------



//===========================================================================
// End of Utility.cpp
//===========================================================================
