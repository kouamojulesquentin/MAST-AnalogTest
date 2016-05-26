//===========================================================================
//                           BinaryVector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file BinaryVector.cpp
//!
//! Implements BinaryVector and SVFVector classes
//!
//===========================================================================

#include "BinaryVector.hpp"
#include "Utility.hpp"
#include <sstream>
#include <array>
#include <algorithm>

using std::array;
using std::ostringstream;
using std::string;
using std::experimental::string_view;
using namespace mast;

#define CHECK_FIXED_SIZE                      if (m_fixedSize)                            THROW_LOGIC_ERROR("BinaryVector size has been fixed")
#define CHECK_FIXED_SIZE_ASSIGNMENT(newSize)  if (m_fixedSize && (newSize != m_usedBits)) THROW_LOGIC_ERROR("BinaryVector size has been fixed")

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

  //! Defines a mask to keep most significant bits of a nibble
  //!
  constexpr uint8_t LEFT_BITS_MASK_4[] =
  {
    0b0000, // 0 bits
    0b1000, // 1 bits
    0b1100, // 2 bits
    0b1110, // 3 bits
    0b1111, // 4 bits
  };

} // End of unnamed namespace

//! Initializes with constant value for all bits
//!
BinaryVector::BinaryVector (uint32_t bitsCount, uint8_t fillPattern, bool fixSize)
  : m_data      ((bitsCount + 7) / 8, fillPattern)
  , m_usedBits  (bitsCount)
  , m_fixedSize (fixSize)
{
  auto lastByteBitsCount = bitsCount % 8;
  if (lastByteBitsCount != 0)
  {
    m_data.back() &= LEFT_BITS_MASK_8[lastByteBitsCount];
  }
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------



//! Copy constructor
//!
BinaryVector::BinaryVector (const mast::BinaryVector& rhs)
  : m_data     (rhs.m_data)
  , m_usedBits (rhs.m_usedBits)
{
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------


//! Move constructor
//!
BinaryVector::BinaryVector (mast::BinaryVector&& rhs) noexcept
  : m_data     (std::move(rhs.m_data))
  , m_usedBits (rhs.m_usedBits)
{
  rhs.m_usedBits = 0;
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------



//! Appends another scan vector
//!
BinaryVector& BinaryVector::Append (const BinaryVector& rhs)
{
  CHECK_FIXED_SIZE;

  uint32_t       bitsToAppend = rhs.BitsCount();
  const uint8_t* pRhsData     = rhs.Data();

  while (bitsToAppend >= 8)
  {
    bitsToAppend -= 8;
    Append(*pRhsData++);
  }

  if (bitsToAppend != 0)
  {
    auto lastValue = *pRhsData;
    Append(lastValue, bitsToAppend, BitsAlignment::Left);
  }

  return *this;
}
//
//  End of: BinaryVector::Append
//---------------------------------------------------------------------------



//! Appends from 8 bits
//!
//! @param value          The value to append to the vector
//! @param numberOfBits   Number of useful bits in the value
//! @param alignment      Tells whether bits are left (msb) or right (lsb) aligned
//!
BinaryVector& BinaryVector::Append (uint8_t value, uint8_t numberOfBits, BitsAlignment alignment)
{
  if (numberOfBits == 0)
  {
    THROW_INVALID_ARGUMENT("Number of bits to append must be != 0");
  }

  CHECK_FIXED_SIZE;

  if (numberOfBits > 8 * sizeof(uint8_t))
  {
    THROW_INVALID_ARGUMENT("Number of bits to append cannot exceed the number of bits of value.");
  }

  // ---------------- Align (pack) added bits to the MSB
  //                  This make sure that unused bits are set to zero (at least for test and debug purpose)
  //
  if (alignment == BitsAlignment::Right)
  {
    value <<= 8 - numberOfBits;
  }
  value &=  LEFT_BITS_MASK_8[numberOfBits];

  const uint8_t lastByteBits = m_usedBits % 8;
  const uint8_t freeBits     = (lastByteBits == 0) ? 8 : 8 - lastByteBits;

  if (freeBits == 8)
  {
    m_data.push_back(value);    // Value is already aligned on MSB
  }
  else if (freeBits >= numberOfBits)
  {
    auto lastByte     = m_data.back();
    auto shiftCount   = 8 - freeBits;
    auto shiftedValue = value    >> shiftCount;
    auto newByte      = lastByte |  shiftedValue;

    m_data.back()     = newByte;
  }
  else
  { // Added value must be split into 2 bytes

    // ---------------- First part (mixed with previous last value)
    //
    uint8_t lastByte       = m_data.back();
    uint8_t shiftedValue_1 = value    >> lastByteBits;
    uint8_t newByte_1      = lastByte |  shiftedValue_1;

    m_data.back()       = newByte_1;

    // ---------------- Second part
    //
    uint8_t newByte_2 = value << freeBits;  // Free bits have been used for first part of value

    m_data.push_back(newByte_2);
  }

  m_usedBits += numberOfBits;

  return *this;
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------


//! Appends from 16 bits
//!
BinaryVector& BinaryVector::Append (uint16_t value)
{
  CHECK_FIXED_SIZE;

  //! @todo [JFC]-[April/25/2016]: Use Boost.Endian to manage properly endianness
  //!
  m_data.push_back((value >>  8) & 0xff);
  m_data.push_back((value >>  0) & 0xff);

  m_usedBits += 16;

  return *this;
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------

//! Appends from 32 bits
//!
BinaryVector& BinaryVector::Append (uint32_t value)
{
  CHECK_FIXED_SIZE;
  //! @todo [JFC]-[April/25/2016]: Use Boost.Endian to manage properly endianness
  //!
  m_data.push_back((value >> 24) & 0xff);
  m_data.push_back((value >> 16) & 0xff);
  m_data.push_back((value >>  8) & 0xff);
  m_data.push_back((value >>  0) & 0xff);

  m_usedBits += 32;

  return *this;
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------


//! Appends from 64 bits
//!
BinaryVector& BinaryVector::Append (uint64_t value)
{
  CHECK_FIXED_SIZE;
  //! @todo [JFC]-[April/25/2016]: Use Boost.Endian to manage properly endianness
  //!
  m_data.push_back((value >> 56) & 0xff);
  m_data.push_back((value >> 48) & 0xff);
  m_data.push_back((value >> 40) & 0xff);
  m_data.push_back((value >> 32) & 0xff);
  m_data.push_back((value >> 24) & 0xff);
  m_data.push_back((value >> 16) & 0xff);
  m_data.push_back((value >>  8) & 0xff);
  m_data.push_back((value >>  0) & 0xff);

  m_usedBits += 64;

  return *this;
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------



//! Clears all content
//!
//! @note Post-condition is that bits and bytes count are zeros
//!
void BinaryVector::Clear ()
{
  m_data.clear();
  m_usedBits = 0;
}
//
//  End of: BinaryVector::Clear
//---------------------------------------------------------------------------



//! Gets content as formatted binary string
//!
//! @note An example of formatting is: 0001_1111:0011_0100:0101_010
//!
//! @param byteSeparator    Characters to insert every 8 bits
//! @param nibbleSeparator  Characters to insert every 4 bits
//! @param bytesPerLine     Number of bytes (sequence of 8 bits) to write per line.
//!                         When zero, all is on the "same line"
//! @param eolSeparator     Characters to insert just before new lines (when bytesPerLine != 0)
//!
string BinaryVector::DataAsBinaryString (string_view byteSeparator,
                                         string_view nibbleSeparator,
                                         uint32_t    bytesPerLine,
                                         string_view eolSeparator
                                        ) const
{
  static const std::array<string_view, 16> nibbles =
  {
    "0000",  // 00
    "0001",  // 01
    "0010",  // 02
    "0011",  // 03
    "0100",  // 04
    "0101",  // 05
    "0110",  // 06
    "0111",  // 07
    "1000",  // 08
    "1001",  // 09
    "1010",  // 10
    "1011",  // 11
    "1100",  // 12
    "1101",  // 13
    "1110",  // 14
    "1111",  // 15
  };

  ostringstream os;
  uint32_t      nibblesCount = 0;
  uint32_t      bytesCount   = 0;

  auto appendNibble = [&](string_view nibble)
  {
    if (nibblesCount == 1)
    {
      os << nibbleSeparator;
    }
    else if (nibblesCount == 2)
    {
      nibblesCount = 0;
      ++bytesCount;
      if (bytesCount == bytesPerLine)
      {
        os << eolSeparator << std::endl;
      }
      else
      {
        os << byteSeparator;
      }
    }

    ++nibblesCount;
    os << nibble;
  };

  auto appendBits = [&](uint8_t bitsOnLsb, uint32_t bitsCount)
  {
    if (bitsCount != 0)
    {
      auto nibble = nibbles[bitsOnLsb];       // Get string representation with padded zero on the right
      nibble = string_view(nibble.data(), bitsCount);
      appendNibble(nibble);
    }
  };

  auto bitsCount = m_usedBits;

  for (auto byte : m_data)
  {
    if (bitsCount >= 8)
    {
      appendNibble(nibbles[byte >> 4]);
      appendNibble(nibbles[byte &  0x0F]);
      bitsCount -= 8;
    }
    else if (bitsCount >= 4)
    {
      appendNibble(nibbles[byte >> 4]);
      bitsCount -= 4;
      appendBits(byte & 0x0F, bitsCount);
    }
    else
    {
      byte = (byte >> 4) & 0x0F; // Put bits on LSB
      appendBits(byte, bitsCount);
    }
  }

  return os.str();
}
//
//  End of: BinaryVector::DataAsBinaryString
//---------------------------------------------------------------------------


//! Gets content as formatted hexadecimal string
//!
//! @note An example of formatting is: FACE_DEAD:BEEF_0123:CAFE_4
//!
//! @param intSeparator    Characters to insert every 32 bits
//! @param shortSeparator  Characters to insert every 16 bits
//! @param bytesPerLine    Number of bytes (sequence of 8 bits) to write per line.
//!                        When zero, all is on the "same line"
//! @param eolSeparator    Characters to insert just before new lines character (when bytesPerLine != 0)
//!
string BinaryVector::DataAsHexString (string_view intSeparator,
                                      string_view shortSeparator,
                                      uint32_t    bytesPerLine,
                                      string_view eolSeparator
                                     ) const
{
  // ---------------- Associate 4 bits value with its hexadecimal representation
  //
  static const std::array<string_view, 16> nibbles =
  {
    "0",  // 00
    "1",  // 01
    "2",  // 02
    "3",  // 03
    "4",  // 04
    "5",  // 05
    "6",  // 06
    "7",  // 07
    "8",  // 08
    "9",  // 09
    "A",  // 10
    "B",  // 11
    "C",  // 12
    "D",  // 13
    "E",  // 14
    "F",  // 15
  };

  ostringstream os;
  uint32_t      nibblesCount = 0;
  uint32_t      bytesCount   = 0;

  auto appendNibble = [&](string_view nibble)
  {
    if ((nibblesCount != 0) && ((nibblesCount % 2) == 0))
    {
      ++bytesCount;
      if (bytesCount == bytesPerLine)
      {
        nibblesCount = 0;
        bytesCount   = 0;

        os << eolSeparator << std::endl;
      }
      else if (nibblesCount == 4)
      {
        os << shortSeparator;
      }
      else if (nibblesCount == 8)
      {
        nibblesCount = 0;
        os << intSeparator;
      }
    }

    ++nibblesCount;
    os << nibble;
  };


  auto bitsCount = m_usedBits;

  for (auto byte : m_data)
  {
    if (bitsCount >= 8)
    {
      appendNibble(nibbles[byte >> 4]);
      appendNibble(nibbles[byte &  0x0F]);
      bitsCount -= 8;
    }
    else if (bitsCount > 4)
    {
      appendNibble(nibbles[byte >> 4]);
      bitsCount -= 4;
      appendNibble(nibbles[byte & LEFT_BITS_MASK_4[bitsCount]]);
    }
    else
    {
      byte >>= 4;         // Put bits on least significant nibble
      appendNibble(nibbles[byte & LEFT_BITS_MASK_4[bitsCount]]);
    }
  }

  return os.str();
}
//
//  End of: BinaryVector::DataAsHexString
//---------------------------------------------------------------------------


//! Copy assignment
//!
//! @note Does not change the fixed size property
//!
BinaryVector& BinaryVector::operator= (const BinaryVector& rhs)
{
  CHECK_FIXED_SIZE_ASSIGNMENT(rhs.m_usedBits);

  if (this != &rhs)
  {
    m_data     = rhs.m_data;
    m_usedBits = rhs.m_usedBits;
  }
  return *this;
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------


//! Move assignment
//!
//! @note Does not change the fixed size property
//!
BinaryVector& BinaryVector::operator= (BinaryVector&& rhs)
{
  CHECK_FIXED_SIZE_ASSIGNMENT(rhs.m_usedBits);

  if (this != &rhs)
  {
    m_data     = std::move(rhs.m_data);
    m_usedBits = rhs.m_usedBits;
  }
  return *this;
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------



//! Returns true when *this is equal to another BinaryVector
//!
//! @note Fixed size property is not compare (only the value)
//!
bool BinaryVector::operator== (const BinaryVector& rhs) const
{
  if (m_usedBits != rhs.m_usedBits)
  {
    return false;
  }

  if (m_usedBits == 0)
  {
    return true;
  }


  auto bitsOnLastByte = m_usedBits % 8;
  auto areEqual       = true;

  if (bitsOnLastByte == 0)
  {
    areEqual = m_data == rhs.m_data;
  }
  else
  {
    if (m_data.size() > 1)
    {
      areEqual = std::equal(m_data.cbegin(), m_data.cend() - 1, rhs.m_data.cbegin(), rhs.m_data.cend() - 1);
    }

    if (areEqual)
    {
      auto left  = m_data.back()     & LEFT_BITS_MASK_8[bitsOnLastByte];
      auto right = rhs.m_data.back() & LEFT_BITS_MASK_8[bitsOnLastByte];

      areEqual = left == right;
    }
  }

  return areEqual;
}
//
//  End of: BinaryVector::operator==
//---------------------------------------------------------------------------



//! Returns another BinaryVector with every bits toggles
//!
BinaryVector BinaryVector::operator~ () const
{
  BinaryVector toggled(*this);

  toggled.ToggleBits();
  return toggled;
}
//
//  End of: BinaryVector::operator~
//---------------------------------------------------------------------------


//! Concatenate two scan vectors
//!
BinaryVector BinaryVector::operator+ (const BinaryVector& rhs) const
{
  BinaryVector result(*this);

  result.Append(rhs);

  return result;
}
//
//  End of: BinaryVector::operator~
//---------------------------------------------------------------------------



//! Sets from 8 bits
//!
void BinaryVector::Set (uint8_t value)
{
  Clear();
  Append(value);
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------



//! Creates a BinaryVector from text binary representation
//!
//! @note Firstly intended for test purposes, but can be used for anything else
//!
//! @param bits   Sequence of characters representing content of BinaryVector to create
//!               Characters in ",':_- \t" are ignored (can be used to ease display of string)
//!               An exception is thrown if there is any character different from
//!               set "01,':_- \t"
//!
//! @return A new BinaryVector initialized as defined by bits text
BinaryVector BinaryVector::CreateFromBinaryString (std::experimental::string_view bits, bool fixSize)
{
  BinaryVector result;

  uint8_t nextByte = 0;
  auto    bitCount = 0;

  for (const auto& nextChar : bits)
  {
    switch (nextChar)
    {
      case '0':
      case '1':
        nextByte <<= 1;
        if (nextChar == '1')
        {
          nextByte |= 1;
        }

        if (++bitCount == 8)
        {
          result.m_data.push_back(nextByte);
          result.m_usedBits += 8;

          nextByte = 0;
          bitCount = 0;
        }
        break;
      case '-':
      case '_':
      case ':':
      case ',':
      case '\'':
      case '\t':
      case ' ':
      case '\0':
        break;  // Ignored characters
      default:
        THROW_INVALID_ARGUMENT("CreateFromBinaryString only support characters in '01,\':_-\\x20\\t'");
        break;
    }
  }

  // ---------------- Append remaining bits (when not a multiple of 8 bits)
  //
  if (bitCount != 0)
  {
    nextByte <<= 8 - bitCount;
    result.m_data.push_back(nextByte);
    result.m_usedBits += bitCount;
  }

  result.m_fixedSize = fixSize;
  return result;
}
//
//  End of: BinaryVector::CreateFromBinaryString
//---------------------------------------------------------------------------


//! Creates a BinaryVector from text hexadecimal representation
//!
//! @note Firstly intended for test purposes, but can be used for anything else
//!
//! @param bits   Sequence of characters representing content of BinaryVector to create
//!               Characters in ",':_- \t" are ignored (can be used to ease display of string)
//!               An exception is thrown if there is any character different from
//!               set "0123456789abcdefABCDEF,':_- \t"
//!
//! @return A new BinaryVector initialized as defined by bits text
BinaryVector BinaryVector::CreateFromHexString (std::experimental::string_view bits, bool fixSize)
{
  BinaryVector result;

  uint8_t nextByte = 0;
  auto    bitCount = 0;

  for (const auto& nextChar : bits)
  {
    auto    hasValue = true;
    uint8_t value    = 0;

    switch (nextChar)
    {
      case '0': value = 0x0; break;
      case '1': value = 0x1; break;
      case '2': value = 0x2; break;
      case '3': value = 0x3; break;
      case '4': value = 0x4; break;
      case '5': value = 0x5; break;
      case '6': value = 0x6; break;
      case '7': value = 0x7; break;
      case '8': value = 0x8; break;
      case '9': value = 0x9; break;
      case 'A':
      case 'a': value = 0xa; break;
      case 'B':
      case 'b': value = 0xb; break;
      case 'C':
      case 'c': value = 0xc; break;
      case 'D':
      case 'd': value = 0xd; break;
      case 'E':
      case 'e': value = 0xe; break;
      case 'F':
      case 'f': value = 0xf; break;
      case '-':
      case '_':
      case ':':
      case ',':
      case '\'':
      case '\t':
      case ' ':
      case '\0':
        hasValue = false;
        break;  // Ignored characters
      default:
        THROW_INVALID_ARGUMENT("CreateFromBinaryString only support characters in '01,\':_-\\x20\\t'");
    }

    if (hasValue)
    {
      nextByte |= value;
      bitCount += 4;
      if (bitCount == 8)
      {
        result.m_data.push_back(nextByte);
        result.m_usedBits += 8;

        nextByte = 0;
        bitCount = 0;
      }
      else
      {
        nextByte <<= 4;
      }
    }
  }

  // ---------------- Append remaining bits (when not a multiple of 8 bits)
  //
  if (bitCount != 0)
  {
    result.m_data.push_back(nextByte);
    result.m_usedBits += bitCount;
  }

  result.m_fixedSize = fixSize;

  return result;
}
//
//  End of: BinaryVector::CreateFromBinaryString
//---------------------------------------------------------------------------



//! Sets from 16 bits
//!
void BinaryVector::Set (uint16_t value)
{
  CHECK_FIXED_SIZE_ASSIGNMENT(8u * sizeof(value));
  Clear();
  Append(value);
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------

//! Sets from 32 bits
//!
void BinaryVector::Set (uint32_t value)
{
  CHECK_FIXED_SIZE_ASSIGNMENT(8u * sizeof(value));
  Clear();
  Append(value);
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------


//! Sets from 64 bits
//!
void BinaryVector::Set (uint64_t value)
{
  CHECK_FIXED_SIZE_ASSIGNMENT(8u * sizeof(value));
  Clear();
  Append(value);
}
//
//  End of: BinaryVector::Set
//---------------------------------------------------------------------------


//! Returns a slice from BinaryVector
//!
//! @note   This call is not valid if it define a slice that exceed the actual
//!         bits count
//!
//! @param firstBitOffset Zero based offset of first bit of slice
//! @param bitsCount      Number of bits
//!
//! @return A new BinaryVector containing a copy of defined slice
BinaryVector BinaryVector::Slice (uint32_t firstBitOffset, uint32_t bitsCount) const
{
  BinaryVector result;

  if (bitsCount != 0)
  {
    // ---------------- Check parameters validity
    //
    if (firstBitOffset >= m_usedBits)
    {
      THROW_INVALID_ARGUMENT("Slice first bit must be within bits range");
    }
    if ((firstBitOffset + bitsCount) > m_usedBits)
    {
      THROW_INVALID_ARGUMENT("Bits count must be such that slice is within bits range");
    }

    auto byteOffset           = firstBitOffset / 8;
    auto bitOffsetInFirstByte = firstBitOffset % 8;
    auto bitsInFirstByte      = std::min(bitsCount, 8 - bitOffsetInFirstByte);

    // ---------------- Copies first bits of source
    //
    if (bitsInFirstByte != 0)
    {
      auto byte = m_data[byteOffset];

      byte <<= bitOffsetInFirstByte;

      result.Append(byte, bitsInFirstByte, BitsAlignment::Left);

      bitsCount -= bitsInFirstByte;
      ++byteOffset;
    }

    // ---------------- Copy full bytes from source
    //
    while (bitsCount >= 8)
    {
      bitsCount -= 8;
      result.Append(m_data[byteOffset++]);
    }

    // ---------------- Copy remaining bits from source
    //
    if (bitsCount != 0)
    {
      auto byte = m_data[byteOffset];
      result.Append(byte, bitsCount, BitsAlignment::Left);
      bitsCount = 0;
    }
  }

  return result;
}
//
//  End of: BinaryVector::Slice
//---------------------------------------------------------------------------



//! Toggles (flips) every bits of the vector
//!
//! @note Unused bits remains forced to zero
//!
//! @return Same vector with all the used bits toggled
BinaryVector& BinaryVector::ToggleBits()
{
  for (auto& byte : m_data)
  {
    byte = ~byte;
  }

  auto bitsOnLastByte = m_usedBits % 8;
  if (bitsOnLastByte != 0)
  {
    m_data.back() &= LEFT_BITS_MASK_8[bitsOnLastByte];
  }

  return *this;
}
//
//  End of: BinaryVector::ToggleBits
//---------------------------------------------------------------------------



//===========================================================================
// End of BinaryVector.cpp
//===========================================================================
