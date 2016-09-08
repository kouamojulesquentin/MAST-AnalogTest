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

using std::string;
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



//! Converts a plain SVF string stream to SVFVector
//!
SVFVector::SVFVector (std::experimental::string_view svfString, uint32_t bitsCount)
  : m_data     (CleanSvfString(svfString, bitsCount))
  , m_usedBits (bitsCount)
{
}
//
//  End of: SVFVector::SVFVector
//---------------------------------------------------------------------------


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

    auto binVectorLsbBits = bitsCount % 8;

    auto pBegin = binaryVector.DataLeftAligned();
    auto pEnd   = pBegin + binaryVector.BytesCount();
    auto pByte  = pBegin;

    if (binVectorLsbBits == 0) // Deal with fast case
    {
      do
      {
        os << std::setw(2) << static_cast<int>(*pByte);
        ++pByte;
      } while (pByte != pEnd);
    }
    else                    // Deal with case, each SVF byte is stranded on two bytes of BinaryVector
    {
      uint32_t remainingBitsCount = bitsCount;
      uint32_t binVectorMsbBits = 8 - binVectorLsbBits;
      uint32_t byte = 0;

      // ---------------- Process first bits (on first byte)
      //
      byte   = *pByte;
      byte >>= binVectorMsbBits;
      byte  &= RIGHT_BITS_MASK_8[binVectorLsbBits];

      os << std::setw(2) << static_cast<int>(byte);

      remainingBitsCount -= binVectorLsbBits;
      while ((remainingBitsCount != 0) && (pByte < pEnd))
      {
        uint8_t msb = *pByte++;   // msb for SVFVector is on lsb of "left" byte of binary BinaryVector
        uint8_t lsb = *pByte;     // lsb for SVFVector is on msb of "right" byte of binary BinaryVector

        // ---------------- Merge bytes
        //
        // e.g. : [xxxmmmmm][lllyyyyy] ==> [mmmmmlll]
        //
        msb <<= binVectorLsbBits;
        lsb >>= binVectorMsbBits;
        msb &= LEFT_BITS_MASK_8[binVectorMsbBits];
        lsb &= RIGHT_BITS_MASK_8[binVectorMsbBits];

        unsigned int byte = msb | lsb;
        os << std::setw(2) << static_cast<int>(byte);

        remainingBitsCount -= 8;
      }
    }

    m_usedBits = bitsCount;
    m_data     = os.str();
  }
}
//
//  End of: SVFVector::SVFVector
//---------------------------------------------------------------------------



//! Transforms an hexadecimal character into its integral value (from 0 to F)
//!
uint8_t SVFVector::CharToNibble (char aChar)
{
  switch (aChar)
  {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'A':
    case 'a': return 0xA;
    case 'B':
    case 'b': return 0xB;
    case 'C':
    case 'c': return 0xC;
    case 'D':
    case 'd': return 0xD;
    case 'E':
    case 'e': return 0xE;
    case 'F':
    case 'f': return 0xF;
    default: THROW_LOGIC_ERROR("Cannot convert non hexadecimal character");
      break;
  }
}
//
//  End of: SVFVector::CharToNibble
//---------------------------------------------------------------------------



//! Removes, from SVF string any new lines, spaces, and usual separator while forcing a multiple of
//! two characters (that ease other algorithms)
//!
//! @param svfString  A SVF compatible string embedding optional new lines, spaces and usual separator (-_:)
//! @param bitsCount
//!
string SVFVector::CleanSvfString (std::experimental::string_view svfString, uint32_t bitsCount)
{
  if (bitsCount > (svfString.length() * 4))
  {
    THROW_INVALID_ARGUMENT("bitsCount is too large relative to svfString");
  }

  auto expectedBytesCount = (bitsCount + 7) / 8;
  auto expectedCharCount  = expectedBytesCount * 2;

  auto   pChar = svfString.cbegin();
  string data;
  data.  reserve(expectedCharCount);

  while (pChar != svfString.cend())
  {
    auto nextChar = *pChar++;

    // ---------------- Validate nextChar
    //
    switch (nextChar)
    {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case 'A':
      case 'B':
      case 'C':
      case 'D':
      case 'E':
      case 'F':
        break;
      case 'a': nextChar = 'A'; break;
      case 'b': nextChar = 'B'; break;
      case 'c': nextChar = 'C'; break;
      case 'd': nextChar = 'D'; break;
      case 'e': nextChar = 'E'; break;
      case 'f': nextChar = 'F'; break;
      case '-':
      case '_':
      case ':':
      case ',':
      case ' ':
      case '\t':
      case '\'':
      case '\r':
      case '\n':
      case '\0':
        nextChar = '\0';
        break;  // Ignored characters
      default:
        THROW_INVALID_ARGUMENT("SVFVector only support characters in '01,\':_-\\x20\\t'");
    }

    if (nextChar != '\0')
    {
      data.push_back(nextChar);
    }
  }

  // ---------------- Make sure result has minimal characters and is a
  //                  multiple of two characters (that ease other algorithm)
  //
  if (data.length() == (expectedCharCount - 1))
  {
    auto bitsOnLastBytes = bitsCount % 8;
    if ((bitsOnLastBytes != 0) && (bitsOnLastBytes <= 4))
    {
      return string("0") + data;
    }

    THROW_INVALID_ARGUMENT("There is not enough hexadecimal character for specified bits count");
  }

  if (data.length() > expectedCharCount)
  {
    auto skipCount = data.length() - expectedCharCount;
    return data.substr(skipCount);
  }

  if (data.length() < expectedCharCount)
  {
    THROW_INVALID_ARGUMENT("There is not enough hexadecimal character for specified bits count");
  }



  return data;
}
//
//  End of: SVFVector::CleanSvfString
//---------------------------------------------------------------------------


//! Returns true when *this is equal to another SVFVector
//!
bool SVFVector::operator== (const SVFVector& rhs) const
{
  if (m_usedBits != rhs.m_usedBits)
  {
    return false;
  }

  if (m_usedBits == 0)
  {
    return true;
  }

  auto areEqual = m_data == rhs.m_data;
  return areEqual;
}
//
//  End of: SVFVector::operator==
//---------------------------------------------------------------------------


//! Creates a BinaryVector from content
//!
//! @note A pre-condition is that internal string is a multiple of two characters
//!
//! @note Supposing that the internal representation is 01234567890A and number of used bits is 44,
//!       then BinaryVector will be: A8967452301
BinaryVector SVFVector::ToBinaryVector () const
{
  BinaryVector result;

  auto endWithNibble = m_data.size() % 2; // SVF vector may not be always a multiple of 2 char

  if (endWithNibble)
  {
    THROW_LOGIC_ERROR("Expected SVFVector to end with a 2 digits");
  }


  uint8_t usedBitsOnLastByte = m_usedBits % 8;
  if (usedBitsOnLastByte == 0)
  {
    usedBitsOnLastByte = 8;
  }

  auto pChar = m_data.cbegin();
  while (pChar != m_data.cend())
  {
    auto msbChar = *pChar++;
    auto lsbChar = *pChar++;

    uint8_t msb = CharToNibble(msbChar);
    uint8_t lsb = CharToNibble(lsbChar);
    uint8_t byte = (msb << 4) + lsb;

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
