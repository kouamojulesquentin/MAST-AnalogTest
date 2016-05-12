//===========================================================================
//                           SVFVector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SVFVector.cpp
//!
//! Implements class SVFVector
//!
//===========================================================================

#include "SVFVector.hpp"
#include "Utility.hpp"

#include <sstream>
#include <algorithm>
#include <iomanip>

using std::ostringstream;

using namespace mast;

namespace
{
  //! Defines a mask to keep most significant bits of a uint8_t
  //!
  constexpr uint8_t LEFT_BITS_MASK_8[] =
  {
    0b00000000, // 0 bits
    0b10000000, // 1 bits
    0b11000000, // 2 bits
    0b11100000, // 3 bits
    0b11110000, // 4 bits
    0b11111000, // 5 bits
    0b11111100, // 6 bits
    0b11111110, // 7 bits
    0b11111111, // 8 bits
  };

  //! Defines a mask to keep less significant bits of a uint8_t
  //!
  constexpr uint8_t RIGHT_BITS_MASK_8[] =
  {
    0b00000000, // 0 bits
    0b00000001, // 1 bits
    0b00000011, // 2 bits
    0b00000111, // 3 bits
    0b00001111, // 4 bits
    0b00011111, // 5 bits
    0b00111111, // 6 bits
    0b01111111, // 7 bits
    0b11111111, // 8 bits
  };


} // End of unnamed namespace



//! Constructs from BinaryVector
//!
//! @note Take into account that all bits in a BinaryVector are left aligned
//! @note Takes packet of 8 bits from BinaryVector end to form hexadimal string representation;
//!       then move backward in BinaryVector and forward in SVFVector string
//!
SVFVector::SVFVector (const BinaryVector& binaryVector)
{
  auto bitsCount = binaryVector.BitsCount();

  if (bitsCount != 0)
  {
    ostringstream os;
    os << std::uppercase << std::hex << std::setw(2) << std::right << std::setfill('0');

    auto bitsOnLastBytes = bitsCount % 8;

    auto pBegin = binaryVector.Data();
    auto pEnd   = pBegin + binaryVector.BytesCount();
    auto pByte  = pEnd;

    if (bitsOnLastBytes == 0) // Deal with fast case
    {
      do
      {
        --pByte;

        os << std::setw(2) << static_cast<int>(*pByte);
      } while (pByte != pBegin);
    }
    else                    // Deal with case, each SVF byte is stranded on two bytes of BinaryVector
    {
      uint32_t bitsOnPreviousBytes = 8 - bitsOnLastBytes;
      uint32_t byte = 0;

      --pByte;  // Points on last byte
      while (pByte != pBegin)
      {
        uint8_t lsb = *pByte--;  // lsb for SVFVector is on msb of BinaryVector
        uint8_t msb = *pByte;    // msb for SVFVector is on lsb of BinaryVector

        // ---------------- Merge bytes
        //
        // e.g. : [xxxmmmmm][lllyyyyy] ==> [mmmmmlll]
        //
        lsb >>= bitsOnPreviousBytes;
        msb <<= bitsOnLastBytes;
        lsb &= RIGHT_BITS_MASK_8[bitsOnLastBytes];
        msb &= LEFT_BITS_MASK_8[bitsOnPreviousBytes];

        unsigned int byte = msb | lsb;
        os << std::setw(2) << static_cast<int>(byte);
      }

      // ---------------- Process last bits (on first byte)
      //
      byte   = *pByte;
      byte >>= bitsOnPreviousBytes;
      byte  &= RIGHT_BITS_MASK_8[bitsOnLastBytes];

      os << std::setw(2) << static_cast<int>(byte);
    }

    m_usedBits = bitsCount;
    m_data     = os.str();
  }

}
//
//  End of: SVFVector::SVFVector
//---------------------------------------------------------------------------


//! Create a BinaryVector from content
//!
//! @note Takes packet of 8 bits from BinaryVector end to form hexadimal string representation;
//!       then move backward in BinaryVector and forward in SVFVector string
//!
BinaryVector SVFVector::ToBinaryVector () const
{
  BinaryVector result;

  auto    endWithNibble      = m_data.size() % 2; // SVF vector may not be alway a multiple of 2 char

  if (endWithNibble)
  {
    THROW_LOGIC_ERROR("Expected SVFVector to end with a 2 digits");
  }

  auto makeByteFromChars = [](auto msbChar, auto lsbChar) -> uint8_t
  {
    //+ (JFC May/12/2016): To real code
    return 0xA5;
  };

  auto pChar = m_data.crbegin();
//+  if (usedBitsOnLastByte != 0)
//+  {
//+    auto lsbChar = *pChar++;
//+    auto msbChar = *pChar++;

//+    uint8_t byte = makeByteFromChars(msbChar, lsbChar);
//+    result.Append(byte, usedBitsOnLastByte, BitsAlignment::Right);
//+    usedBitsOnLastByte = 8;
//+  }

  uint8_t usedBitsOnLastByte = m_usedBits % 8;
  if (usedBitsOnLastByte == 0)
  {
    usedBitsOnLastByte = 8;
  }

  while (pChar != m_data.crend())
  {
    auto lsbChar = *pChar++;
    auto msbChar = *pChar++;

    uint8_t byte = makeByteFromChars(msbChar, lsbChar);
    result.Append(byte, usedBitsOnLastByte, BitsAlignment::Right);
    usedBitsOnLastByte = 8;
  }
  return result;
}
//
//  End of: SVFVector::ToBinaryVector
//---------------------------------------------------------------------------



//===========================================================================
// End of SVFVector.cpp
//===========================================================================
