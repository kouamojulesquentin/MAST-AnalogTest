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
using std::vector;
using std::initializer_list;
using std::ostringstream;
using std::string;
using std::to_string;
using std::experimental::string_view;

using namespace std::string_literals;
using namespace mast;

#define CHECK_SAME_SIZE(other)                if (other.m_usedBits != m_usedBits)         THROW_LOGIC_ERROR("BinaryVectors must have same size")
#define CHECK_NOT_EMPTY                       if (IsEmpty())                              THROW_LOGIC_ERROR("BinaryVector must not be empty")
#define CHECK_FIXED_SIZE                      if (FixedSize())                            THROW_LOGIC_ERROR("BinaryVector size has been fixed to " + to_string(m_usedBits) + " bit(s)")
#define CHECK_FIXED_SIZE_ASSIGNMENT(newSize)  if (FixedSize() && (newSize != m_usedBits)) THROW_LOGIC_ERROR("BinaryVector size has been fixed to " + to_string(m_usedBits) + " bit(s); Cannot assign " + to_string(newSize) + " bit(s)")
#define CHECK_AT_LEAST_1_BIT(numBits)         if (numBits == 0)                           THROW_INVALID_ARGUMENT("Number of bits to append must be != 0")

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

  //! Defines a mask to least most significant bits of a uint8_t
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

  constexpr std::array<uint8_t, 8> BIT_MASK_8 =
  {
    0b10000000,  // 00
    0b01000000,  // 01
    0b00100000,  // 02
    0b00010000,  // 03
    0b00001000,  // 04
    0b00000100,  // 05
    0b00000010,  // 06
    0b00000001,  // 07
  };

  static const std::array<string_view, 16> BINARY_NIBBLES =
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
} // End of unnamed namespace

//! Initializes with constant value for all bits
//!
BinaryVector::BinaryVector (uint32_t bitsCount, uint8_t fillPattern, SizeProperty sizeProperty)
  : m_data         ((bitsCount + 7) / 8, fillPattern)
  , m_usedBits     (bitsCount)
  , m_sizeProperty (sizeProperty)
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
  : m_data         (rhs.m_data)
  , m_usedBits     (rhs.m_usedBits)
  , m_sizeProperty (rhs.m_sizeProperty == SizeProperty::FixedOnCopy ? SizeProperty::FixedOnCopy : SizeProperty::NotFixed)
{
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------


//! Move constructor
//!
BinaryVector::BinaryVector (mast::BinaryVector&& rhs) noexcept
  : m_data         (std::move(rhs.m_data))
  , m_usedBits     (rhs.m_usedBits)
  , m_sizeProperty (rhs.m_sizeProperty == SizeProperty::FixedOnCopy ? SizeProperty::FixedOnCopy : SizeProperty::NotFixed)
{
  rhs.m_usedBits     = 0;
  rhs.m_sizeProperty = SizeProperty::NotFixed;
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------


//! Constructs from raw data
//!
//! @note Raw data must be formatted as:
//!         - MSB comes first (data[0])
//!         - For last byte, bits are left aligned (on most significant bits)
//!
//! @note If data does not contained exactly the number of bytes for specified bits count,
//!       the content is either truncated or extended with zeros
//!
//! @param data           Raw data (formated as expected)
//! @param bitsCount      Number of effectively used bits
//! @param sizeProperty   Size property
//!
BinaryVector::BinaryVector (const std::vector<uint8_t>& data, uint32_t bitsCount, SizeProperty sizeProperty)
  : m_data         (data)
  , m_usedBits     (bitsCount)
  , m_sizeProperty (sizeProperty)
{
  auto expectedSize = (bitsCount + 7u) / 8u;
  m_data.resize(expectedSize);
  MaskLastByte();
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------


//! Constructs from "moveable" raw data
//!
//! @note Raw data must be formatted as:
//!         - MSB comes first (data[0])
//!         - For last byte, bits are left aligned (on most significant bits)
//!
//! @note If data does not contained exactly the number of bytes for specified bits count,
//!       the content is either truncated or extended with zeros
//!
//! @param data           Raw data (formated as expected)
//! @param bitsCount      Number of effectively used bits
//! @param sizeProperty   Size property
//!
BinaryVector::BinaryVector (std::vector<uint8_t>&& data, uint32_t bitsCount, SizeProperty sizeProperty)
  : m_data         (std::move(data))
  , m_usedBits     (bitsCount)
  , m_sizeProperty (sizeProperty)
{
  auto expectedSize = (bitsCount + 7u) / 8u;
  m_data.resize(expectedSize);
  MaskLastByte();
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------


//! Appends another scan vector
//!
BinaryVector& BinaryVector::Append (const BinaryVector& rhs)
{
  CHECK_FIXED_SIZE;

  uint32_t       bitsToAppend = rhs.m_usedBits;
  const uint8_t* pRhsData     = rhs.m_data.data();

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
//! @param value          The value to append to the vector
//! @param numberOfBits   Number of useful bits in the value
//! @param alignment      Tells whether bits are left (msb) or right (lsb) aligned
//!
BinaryVector& BinaryVector::Append (uint16_t value, uint8_t numberOfBits, BitsAlignment alignment)
{
  CHECK_FIXED_SIZE;
  CHECK_AT_LEAST_1_BIT(numberOfBits);

  auto asBytes = reinterpret_cast<uint8_t*>(&value);
  #if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t byte_1 = asBytes[1];
    uint8_t byte_2 = asBytes[0];
  #else
    uint8_t byte_1 = asBytes[0];
    uint8_t byte_2 = asBytes[1];
  #endif

  auto chunksList = {byte_1, byte_2};
  return AppendChunks(numberOfBits, alignment, chunksList);
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------



//! Appends from 32 bits
//!
//! @param value          The value to append to the vector
//! @param numberOfBits   Number of useful bits in the value
//! @param alignment      Tells whether bits are left (msb) or right (lsb) aligned
//!
BinaryVector& BinaryVector::Append (uint32_t value, uint8_t numberOfBits, BitsAlignment alignment)
{
  CHECK_FIXED_SIZE;
  CHECK_AT_LEAST_1_BIT(numberOfBits);

  auto asBytes = reinterpret_cast<uint8_t*>(&value);
  #if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t byte_1 = asBytes[3];
    uint8_t byte_2 = asBytes[2];
    uint8_t byte_3 = asBytes[1];
    uint8_t byte_4 = asBytes[0];
  #else
    uint8_t byte_1 = asBytes[0];
    uint8_t byte_2 = asBytes[1];
    uint8_t byte_3 = asBytes[2];
    uint8_t byte_4 = asBytes[3];
  #endif

  auto chunksList = {byte_1, byte_2, byte_3, byte_4};
  return AppendChunks(numberOfBits, alignment, chunksList);
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------


//! Appends from 64 bits
//!
//! @param value          The value to append to the vector
//! @param numberOfBits   Number of useful bits in the value
//! @param alignment      Tells whether bits are left (msb) or right (lsb) aligned
//!
BinaryVector& BinaryVector::Append (uint64_t value, uint8_t numberOfBits, BitsAlignment alignment)
{
  CHECK_FIXED_SIZE;
  CHECK_AT_LEAST_1_BIT(numberOfBits);

  auto asBytes = reinterpret_cast<uint8_t*>(&value);
  #if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t byte_1 = asBytes[7];
    uint8_t byte_2 = asBytes[6];
    uint8_t byte_3 = asBytes[5];
    uint8_t byte_4 = asBytes[4];
    uint8_t byte_5 = asBytes[3];
    uint8_t byte_6 = asBytes[2];
    uint8_t byte_7 = asBytes[1];
    uint8_t byte_8 = asBytes[0];
  #else
    uint8_t byte_1 = asBytes[0];
    uint8_t byte_2 = asBytes[1];
    uint8_t byte_3 = asBytes[2];
    uint8_t byte_4 = asBytes[3];
    uint8_t byte_5 = asBytes[4];
    uint8_t byte_6 = asBytes[5];
    uint8_t byte_7 = asBytes[6];
    uint8_t byte_8 = asBytes[7];
  #endif

  auto chunksList = {byte_1, byte_2, byte_3, byte_4, byte_5, byte_6, byte_7, byte_8};

  return AppendChunks(numberOfBits, alignment, chunksList);
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------


//! Appends from a list of uint8_t ordered from msb to lsb
//!
//! @param numberOfBits   Number of bits to use from chunksList
//! @param alignment      Tells whether bits are left (msb) or right (lsb) aligned
//! @param chunksList     A set of uint8_t from msb to lsb
//!
BinaryVector& BinaryVector::AppendChunks (uint8_t numberOfBits, BitsAlignment alignment, initializer_list<uint8_t> chunksList)
{
  CHECK_FIXED_SIZE;
  CHECK_AT_LEAST_1_BIT(numberOfBits);

  uint8_t maxBits   = chunksList.size() * 8u;
  if (numberOfBits > maxBits)
  {
    THROW_INVALID_ARGUMENT("Numbers of bits cannot exceed bits count of chunks list");
  }

  uint8_t threshold = maxBits - 8u;
  auto    chunks    = chunksList.begin();
  uint8_t chunkId   = 0;

  while (threshold >= numberOfBits)
  {
    threshold -= 8;
    if (alignment == BitsAlignment::Right)
    {
      ++chunkId;
    }
  }

  while (numberOfBits > threshold)
  {
    uint8_t bits              = chunks[chunkId];
    uint8_t maxBits           = std::min(numberOfBits, static_cast<uint8_t>(8u));
    uint8_t chunkNumberOfBits = (alignment == BitsAlignment::Left) ? maxBits
                                                                   : numberOfBits - threshold;

    Append(bits, chunkNumberOfBits, alignment);
    numberOfBits -= chunkNumberOfBits;

    if (threshold == 0)
    {
      break;
    }
    threshold -= 8u;
    ++chunkId;
  }
  return *this;
}
//
//  End of: BinaryVector::AppendChunks
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


//! Clears specified bit (to zero)
//!
//! @param bitOffset Zero based bit offset (from left)
//!
void BinaryVector::ClearBit (uint32_t bitOffset)
{
  CHECK_PARAMETER_LT(bitOffset, m_usedBits, "Out of range bit id: "s + to_string(bitOffset));

  auto byteOffset      = bitOffset / 8;
  auto bitOffsetInByte = bitOffset % 8;

  m_data[byteOffset] &= ~BIT_MASK_8[bitOffsetInByte];
}
//
//  End of: BinaryVector::ClearBit
//---------------------------------------------------------------------------



//! Gets content as formatted binary string
//!
//! @note An example of formatting is: 0001_1111:0011_0100:0101_010
//!
//! @param octoSeparator  Characters to insert every 8 bits
//! @param quadSeparator  Characters to insert every 4 bits
//! @param bytesPerLine   Number of bytes (sequence of 8 bits) to write per line.
//!                       When zero, all is on the "same line"
//! @param eolSeparator   Characters to insert just before new lines (when bytesPerLine != 0)
//! @param prefixWith0b   When true, "0b" will be prepended to the resulting string
//!
string BinaryVector::DataAsBinaryString (string_view quadSeparator,
                                         string_view octoSeparator,
                                         uint32_t    bytesPerLine,
                                         string_view eolSeparator,
                                         bool        prefixWith0b
                                        ) const
{

  ostringstream os;
  uint32_t      nibblesCount = 0;
  uint32_t      bytesCount   = 0;

  auto appendNibble = [&](string_view nibble)
  {
    if (nibblesCount == 1)
    {
      os << quadSeparator;
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
        os << octoSeparator;
      }
    }

    ++nibblesCount;
    os << nibble;
  };

  auto appendBits = [&](uint8_t bitsOnLsb, uint32_t bitsCount)
  {
    if (bitsCount != 0)
    {
      auto nibble = BINARY_NIBBLES[bitsOnLsb];       // Get string representation with padded zero on the right
      nibble = string_view(nibble.data(), bitsCount);
      appendNibble(nibble);
    }
  };

  if (prefixWith0b)
  {
    os << "0b";
  }

  auto bitsCount = m_usedBits;

  for (auto byte : m_data)
  {
    if (bitsCount >= 8)
    {
      appendNibble(BINARY_NIBBLES[byte >> 4]);
      appendNibble(BINARY_NIBBLES[byte &  0x0F]);
      bitsCount -= 8;
    }
    else if (bitsCount >= 4)
    {
      appendNibble(BINARY_NIBBLES[byte >> 4]);
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


//! Gets content as formatted hexadecimal string (as saved internally - bits are appended from left to right)
//!
//! @note An example of formatting is: FACE_DEAD:BEEF_0123:CAFE_4 (where last '4' may mean '0b0100' or '0b010' or '0b01')
//! @note For precise display of last bits, please use DataAsMixString (or DataAsBinaryString)
//!
//! @param octoSeparator   Characters to insert every 32 bits
//! @param quadSeparator   Characters to insert every 16 bits
//! @param bytesPerLine    Number of bytes (sequence of 8 bits) to write per line.
//!                        When zero, all is on the "same line"
//! @param eolSeparator    Characters to insert just before new lines character (when bytesPerLine != 0)
//! @param prefixWith0x    When true, "0x" will be prepended to the resulting string
//!
string BinaryVector::DataAsHexString (string_view quadSeparator,
                                      string_view octoSeparator,
                                      uint32_t    bytesPerLine,
                                      string_view eolSeparator,
                                      bool        prefixWith0x
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
        os << quadSeparator;
      }
      else if (nibblesCount == 8)
      {
        nibblesCount = 0;
        os << octoSeparator;
      }
    }

    ++nibblesCount;
    os << nibble;
  };

  if (prefixWith0x)
  {
    os << "0x";
  }


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


//! Gets content as formatted hexadecimal or/and binary string
//!
//!
//! @note An example of formatting is: 0xFACE_DEAD:BEEF_0123:CAFE_/b01
//!
//! @param hexStyleThreshold  The number of bits that makes the result starting as hex string (preference is to be >= 8)
//! @param octaSeparator      Characters to insert every 8 digits
//! @param quadSeparator      Characters to insert every 4 digits
//! @param bytesPerLine       Number of bytes (sequence of 8 bits) to write per line.
//!                           When zero, all is on the "same line"
//! @param eolSeparator       Characters to insert just before new lines character (when bytesPerLine != 0)
//!
string BinaryVector::DataAsMixString (uint32_t    hexStyleThreshold,
                                      string_view quadSeparator,
                                      string_view octaSeparator,
                                      uint32_t    bytesPerLine,
                                      string_view eolSeparator) const
{
  if (m_usedBits == 0)
  {
    return "";
  }

  if (m_usedBits < hexStyleThreshold)
  {
    return DataAsBinaryString(quadSeparator, octaSeparator, bytesPerLine, eolSeparator, true);
  }

  auto smartString       = DataAsHexString(quadSeparator, octaSeparator, bytesPerLine, eolSeparator, true);
  auto lastByteBitsCount = m_usedBits % 8;
  auto lastQuadBitsCount = m_usedBits % 4;

  if (lastQuadBitsCount != 0)
  {
    // ---------------- Replace last digit with its binary equivalent
    //
    auto shiftCount = (lastByteBitsCount < 4) ? 8u - lastQuadBitsCount    // For cases last bits are on msb
                                              : 4u - lastQuadBitsCount;   // For cases last bits are on lsb
    auto byte       = (m_data.back() >> shiftCount) & 0x0F;
    auto lastBits   = BINARY_NIBBLES[byte].substr(4u - lastQuadBitsCount, lastQuadBitsCount);
    smartString     = smartString.substr(0, smartString.length() - 1) + "/b"s + string(lastBits);
  }
  return smartString;
}
//
//  End of: BinaryVector::DataAsMixString
//---------------------------------------------------------------------------


//! Returns data right aligned in a new buffer
//!
//! @note Internal representation is left aligned.
//!       For example for a 23 bits BinaryVector with value 0x654321 (0b110_0101_0100_0011_0010_0001)
//!       Will be in memory (with increased address)
//!       Left aligned:   [CA][86][42]  (0b11001010:10000110:01000010)
//!       Right aligned:  [65][43][21]  (0b01100101:01000011:00100001)
//!
//! @note To be cache friendly data are processed in increased address order
vector<uint8_t> BinaryVector::DataRightAligned () const
{
  vector<uint8_t> rightAligned;

  auto binVectorLsbBits = m_usedBits % 8; // This is the number of meaningful bits on last byte in BinaryVector representation (left aligned)

  if (binVectorLsbBits == 0) // Deal with fast case
  {
    rightAligned = m_data;
  }
  else     // Deal with case, right aligned bytes are stranded on two bytes of left aligned version
  {
    rightAligned.reserve(m_data.size());

    auto pBegin = m_data.cbegin();
    auto pEnd   = m_data.cend();
    auto pByte  = pBegin;

    uint8_t  bufferShiftCount   = 8u - binVectorLsbBits;  // This is the number of meaningful bits on first byte in right aligned result
    uint32_t remainingBitsCount = m_usedBits;
    uint8_t  byte = 0;

    // ---------------- Process first bits (on first byte)
    //
    byte   = *pByte;
    byte >>= bufferShiftCount;
    byte  &= RIGHT_BITS_MASK_8[binVectorLsbBits];

    rightAligned.push_back(byte);

    // ---------------- Process other bytes
    //
    remainingBitsCount -= binVectorLsbBits;
    while ((remainingBitsCount != 0) && (pByte < pEnd))
    {
      uint8_t msb = *pByte++;   // msb for result is on lsb of "left" byte of binary BinaryVector
      uint8_t lsb = *pByte;     // lsb for result is on msb of "right" byte of binary BinaryVector

      // ---------------- Merge bytes
      //
      // e.g. : [xxxmmmmm][lllyyyyy] ==> [mmmmmlll]
      //           msb        lsb           byte
      //
      msb <<= binVectorLsbBits;
      lsb >>= bufferShiftCount;
      msb &= LEFT_BITS_MASK_8[bufferShiftCount];
      lsb &= RIGHT_BITS_MASK_8[binVectorLsbBits];

      uint8_t byte = msb | lsb;
      rightAligned.push_back(byte);

      remainingBitsCount -= 8;
    }
  }

  return rightAligned;
}
//
//  End of: BinaryVector::DataRightAligned
//---------------------------------------------------------------------------



//! Copy assignment
//!
//! @note Does not change the fixed size property
//!
BinaryVector& BinaryVector::operator= (const BinaryVector& rhs)
{
  CHECK_FIXED_SIZE_ASSIGNMENT(rhs.m_usedBits);

  m_data         = rhs.m_data;
  m_usedBits     = rhs.m_usedBits;
  m_sizeProperty = rhs.m_sizeProperty == SizeProperty::FixedOnCopy ? SizeProperty::FixedOnCopy : m_sizeProperty;

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
  if (this != &rhs)
  {
    CHECK_FIXED_SIZE_ASSIGNMENT(rhs.m_usedBits);
    m_data         = std::move(rhs.m_data);
    m_usedBits     = rhs.m_usedBits;
    m_sizeProperty = rhs.m_sizeProperty == SizeProperty::FixedOnCopy ? SizeProperty::FixedOnCopy : m_sizeProperty;

    rhs.m_usedBits     = 0;
    rhs.m_sizeProperty = SizeProperty::NotFixed;
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


//! Bitwise and with another vector
//!
BinaryVector& BinaryVector::operator&= (const BinaryVector& rhs)
{
  CHECK_SAME_SIZE(rhs);

  auto left  = m_data.begin();
  auto right = rhs.m_data.cbegin();

  while (left != m_data.end())
  {
    *left++ &= *right++;
  }

  MaskLastByte();

  return *this;
}
//
//  End of: BinaryVector::operator&=
//---------------------------------------------------------------------------


//! Bitwise or with another vector
//!
BinaryVector& BinaryVector::operator|= (const BinaryVector& rhs)
{
  CHECK_SAME_SIZE(rhs);

  auto left  = m_data.begin();
  auto right = rhs.m_data.cbegin();

  while (left != m_data.end())
  {
    *left++ |= *right++;
  }

  MaskLastByte();

  return *this;
}
//
//  End of: BinaryVector::operator&=
//---------------------------------------------------------------------------


//! Bitwise xor with another vector
//!
BinaryVector& BinaryVector::operator^= (const BinaryVector& rhs)
{
  CHECK_SAME_SIZE(rhs);

  auto left  = m_data.begin();
  auto right = rhs.m_data.cbegin();

  while (left != m_data.end())
  {
    *left++ ^= *right++;
  }

  MaskLastByte();

  return *this;
}
//
//  End of: BinaryVector::operator&=
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






//! Creates a BinaryVector from text binary representation
//!
//! @note Firstly intended for test purposes, but can be used for anything else
//!
//! @param bits   Sequence of characters representing content of BinaryVector to create
//!               Characters in ",':_- \t/\|" are ignored (can be used to ease display of string)
//!               An exception is thrown if there is any character different from
//!               set "01,':_- \t\n"
//!               '0b' is ignored at start of string. An exception is thrown everywhere else
//!               '/b', '/B', '\b', '\B' constructions are ignored anywhere
//!
//! @return A new BinaryVector initialized as defined by bits text
BinaryVector BinaryVector::CreateFromBinaryString (std::experimental::string_view bits, SizeProperty sizeProperty)
{
  CHECK_PARAMETER_NOT_NULL(bits.data(), "BinaryVector cannot be created from nullptr");
  BinaryVector result;

  uint8_t nextByte = 0;
  auto    bitCount = 0;

  // ---------------- Skip leading blank chars
  //
  auto bitId  = size_t(0);
  while (   (bitId < bits.length())
         && (   (bits[bitId] == '\n')
             || (bits[bitId] == '\t')
             || (bits[bitId] == ' ')
            )
        )
  {
    ++bitId;
  }
  bits.remove_prefix(bitId);

  // ---------------- Tolerate strings beginning with "0b"
  //
  if (bits.length() >= 2
      && ((bits[1] == 'b') || (bits[1] == 'B'))
      &&  (bits[0] == '0')
     )
  {
    bits.remove_prefix(2);
  }

  string_view::value_type previousChar = '\0'; // To detect construction like '\b' and '/b' that are tolerated
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
      case '|':
      case ',':
      case '\'':
      case '\t':
      case '\n':
      case ' ':
      case '\0':
      case '/':
      case '\\':
        break;  // Ignored characters
      case 'b':
      case 'B':
        if ((previousChar != '\\') && (previousChar != '/'))
        {
          THROW_INVALID_ARGUMENT("CreateFromBinaryString support only constructions: '\\b' and '/b'");
        }
        break;
      default:
        THROW_INVALID_ARGUMENT("CreateFromBinaryString only support characters in '01,\':|_-\\x20\\t\\n'");
        break;
    }
    previousChar = nextChar;
  }

  // ---------------- Append remaining bits (when not a multiple of 8 bits)
  //
  if (bitCount != 0)
  {
    nextByte <<= 8 - bitCount;
    result.m_data.push_back(nextByte);
    result.m_usedBits += bitCount;
  }

  result.m_sizeProperty = sizeProperty;
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
//!               Characters in ",':_- \t/\" are ignored (can be used to ease display of string)
//!               An exception is thrown if there is any character different from
//!               set "0123456789abcdefABCDEF,':_- \t\n/\"
//!               '0x' is ignored at start of string. An exception is thrown everywhere else
//!               '/x', '/X', '\x', '\X' constructions are ignored anywhere
//!
//! @return A new BinaryVector initialized as defined by bits text
//!
BinaryVector BinaryVector::CreateFromHexString (string_view bits, SizeProperty sizeProperty)
{
  CHECK_PARAMETER_NOT_NULL(bits.data(), "BinaryVector cannot be created from nullptr");
  BinaryVector result;

  uint8_t nextByte = 0;
  auto    bitCount = 0;

  // ---------------- Skip leading blank chars
  //
  auto bitId  = size_t(0);
  while (   (bits[bitId] == '\n')
         || (bits[bitId] == '\t')
         || (bits[bitId] == ' ')
        )
  {
    ++bitId;
  }
  bits.remove_prefix(bitId);

  // ---------------- Tolerate strings beginning with "0x"
  //
  if (bits.length() >= 2
      && ((bits[1] == 'x') || (bits[1] == 'X'))
      &&  (bits[0] == '0')
     )
  {
    bits.remove_prefix(2);
  }

  string_view::value_type previousChar = '\0'; // To detect construction like '\x' and '/x' that are tolerated
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
      case '|':
      case ',':
      case '\'':
      case '\t':
      case '\n':
      case ' ':
      case '\0':
      case '/':
      case '\\':
        hasValue = false;
        break;  // Ignored characters
      case 'x':
      case 'X':
        if ((previousChar == '\\') || (previousChar == '/'))
        {
          hasValue = false;
        }
        else
        {
          THROW_INVALID_ARGUMENT("CreateFromBinaryString support only constructions: '\\x' and '/x'");
        }
        break;
      default:
        THROW_INVALID_ARGUMENT("CreateFromBinaryString only support characters in '01,\':|_-\\x20\\t\\n/\\'");
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

    previousChar = nextChar;
  }

  // ---------------- Append remaining bits (when not a multiple of 8 bits)
  //
  if (bitCount != 0)
  {
    result.m_data.push_back(nextByte);
    result.m_usedBits += bitCount;
  }

  result.m_sizeProperty = sizeProperty;

  return result;
}
//
//  End of: BinaryVector::CreateFromBinaryString
//---------------------------------------------------------------------------


//! Creates a BinaryVector from mix of hexadecimal and binary representation
//!
//! @note Firstly intended for test purposes, but can be used for anything else
//!
//! @param bits   Sequence of characters representing content of BinaryVector to create
//!               Characters in ",':_- \t/\" are ignored (can be used to ease display of string)
//!               An exception is thrown if there is any character different from
//!               set "0123456789abcdefABCDEF,':_- \t/\"
//!               '0x' is ignored at start of string. An exception is thrown everywhere else
//!               '/x', '/X', '\x', '\X' constructions are interpreted as: What follow is hexadecimal
//!               '/b', '/B', '\b', '\B' constructions are interpreted as: What follow is binary
//!
//! @return A new BinaryVector initialized as defined by bits text
//!
BinaryVector BinaryVector::CreateFromString (string_view bits, SizeProperty sizeProperty)
{
  CHECK_PARAMETER_NOT_NULL(bits.data(), "BinaryVector cannot be created from nullptr");

  //! Defines how a string is formatted to represent BinaryVector content
  //!
  enum class StringFormat
  {
    Undefined,   //!< Format is not defined
    Binary,      //!< Format is recognized as binary
    Hexadecimal, //!< Format is recognized hexadecimal
  };

  // ---------------- Skip leading blank spaces
  //
  Utility::TrimLeft(bits);

  // ---------------- Tolerate strings beginning with "0x"
  //
  if (bits.length() == 0)
  {
    return BinaryVector();
  }

  if (bits.length() == 1)
  {
    THROW_INVALID_ARGUMENT("Cannot interpret one, non space, character");
  }

  // ---------------- Skip leading blank chars
  //
  auto bitId  = size_t(0);
  while (   (bitId < bits.length())
         && (   (bits[bitId] == '\n')
             || (bits[bitId] == '\t')
             || (bits[bitId] == ' ')
            )
        )
  {
    ++bitId;
  }

  auto format = StringFormat::Undefined;
  auto firstCharOk = (bits[bitId] == '0') || (bits[bitId] == '/') || (bits[bitId] == '\\');
  ++bitId;
  if      (firstCharOk && ((bits[bitId] == 'x') || (bits[bitId] == 'X')))
  {
    format = StringFormat::Hexadecimal;
  }
  else if (firstCharOk && ((bits[bitId] == 'b') || (bits[bitId] == 'B')))
  {
    format = StringFormat::Binary;
  }

  // ---------------- Defines how to get next, largest, chunk of current format
  //
  auto getNextChunk = [&bits, &format]()
  {
    // ---------------- Find format change
    //
    auto foundFormatChange = false;
    auto offset            = string_view::size_type(0u);

    do
    {
      offset = bits.find_first_of("/\\", offset);
      if (   (offset == string_view::npos)  // Not found
          || (offset == bits.length())      // Found at end of string ==> can be ignored
         )
      {
        break;
      }

      auto nextChar     = bits[offset + 1];
      foundFormatChange = (format == StringFormat::Binary) ? (nextChar == 'x') || (nextChar == 'X')
                                                           : (nextChar == 'b') || (nextChar == 'B');
      if (!foundFormatChange)
      {
        ++offset;
      }
    } while (!foundFormatChange);


    auto chunk = bits;

    if (!foundFormatChange)
    {
      bits.remove_prefix(bits.length());  // Clear remaining (this was the last chunk)
    }
    else
    {
      chunk = bits.substr(0, offset);
      bits.remove_prefix(offset + 2u);
    }

    return chunk;
  };

  if (format == StringFormat::Undefined)
  {
    THROW_INVALID_ARGUMENT("Cannot tell if bits start as binary or hexadecimal");
  }

  BinaryVector result;
  BinaryVector chunkVector;

  bits.remove_prefix(bitId + 1);  // Remove leading blank chars and one of: '0x', '/x', '\x', '0b', '/b' and '\b'
  while (!bits.empty())
  {
    auto bitsChunk = getNextChunk();

    if (format == StringFormat::Binary)
    {
      chunkVector = BinaryVector::CreateFromBinaryString(bitsChunk);
      format      = StringFormat::Hexadecimal;
    }
    else
    {
      chunkVector = BinaryVector::CreateFromHexString(bitsChunk);
      format      = StringFormat::Binary;
    }
    result.Append(chunkVector);
  }

  result.m_sizeProperty = sizeProperty;
  return result;
}
//
//  End of: BinaryVector::CreateFromBinaryString
//---------------------------------------------------------------------------


//! Reads 8 bits value from BinaryVector
//!
//! @note This is an invalid operation when the BinaryVector is empty
//! @note If the BinaryVector has less than 8 bits, then a padding will be appled on value most significants bits
//! @note If the BinaryVector has more than 8 bits, then the value will take only the least significants bits
//!
//! @param value  Variable to update with current value
//!
void BinaryVector::Get (uint8_t&  value) const
{
  CHECK_NOT_EMPTY;

  uint8_t  lastByteBits   = LastByteBitsCount();
  uint32_t lastByteOffset = m_data.size() - 1u;

  value = (m_usedBits > 0) ? MergeToByte(lastByteOffset, lastByteBits) : 0;
}
//
//  End of: BinaryVector::Get
//---------------------------------------------------------------------------




//! Reads 16 bits value from BinaryVector
//!
//! @note This is an invalid operation when the BinaryVector is empty
//! @note If the BinaryVector has less than 16 bits, then a padding will be appled on value most significants bits
//! @note If the BinaryVector has more than 16 bits, then the value will take only the least significants bits
//!
//! @param value  Variable to update with current value
//!
void BinaryVector::Get (uint16_t&  value) const
{
  CHECK_NOT_EMPTY;

  uint8_t  lastByteBits   = LastByteBitsCount();
  uint32_t lastByteOffset = m_data.size() - 1u;

  uint8_t value_0 =  (m_usedBits > 0)  ? MergeToByte(lastByteOffset,      lastByteBits) : 0;  // LSB
  uint8_t value_1 =  (m_usedBits > 8)  ? MergeToByte(lastByteOffset - 1u, lastByteBits) : 0;  // MSB

  auto asBytes = reinterpret_cast<uint8_t*>(&value);
  #if __BYTE_ORDER == __LITTLE_ENDIAN
    asBytes[0] = value_0;
    asBytes[1] = value_1;
  #else
    asBytes[0] = value_1;
    asBytes[1] = value_0;
  #endif
}
//
//  End of: BinaryVector::Get
//---------------------------------------------------------------------------


//! Reads 32 bits value from BinaryVector
//!
//! @note This is an invalid operation when the BinaryVector is empty
//! @note If the BinaryVector has less than 32 bits, then a padding will be appled on value most significants bits
//! @note If the BinaryVector has more than 32 bits, then the value will take only the least significants bits
//!
//! @param value  Variable to update with current value
//!
void BinaryVector::Get (uint32_t&  value) const
{
  CHECK_NOT_EMPTY;

  uint8_t  lastByteBits   = LastByteBitsCount();
  uint32_t lastByteOffset = m_data.size() - 1u;

  uint8_t value_0 =  (m_usedBits > 0)  ? MergeToByte(lastByteOffset,      lastByteBits) : 0;  // LSB
  uint8_t value_1 =  (m_usedBits > 8)  ? MergeToByte(lastByteOffset - 1u, lastByteBits) : 0;
  uint8_t value_2 =  (m_usedBits > 16) ? MergeToByte(lastByteOffset - 2u, lastByteBits) : 0;
  uint8_t value_3 =  (m_usedBits > 24) ? MergeToByte(lastByteOffset - 3u, lastByteBits) : 0;  // MSB

  // ---------------- Combine all bytes dealing with endianness
  //
  auto asBytes = reinterpret_cast<uint8_t*>(&value);
  #if __BYTE_ORDER == __LITTLE_ENDIAN
    asBytes[0] = value_0;
    asBytes[1] = value_1;
    asBytes[2] = value_2;
    asBytes[3] = value_3;
  #else
    asBytes[0] = value_3;
    asBytes[1] = value_2;
    asBytes[2] = value_1;
    asBytes[3] = value_0;
  #endif
}
//
//  End of: BinaryVector::Get
//---------------------------------------------------------------------------


//! Reads 64 bits value from BinaryVector
//!
//! @note This is an invalid operation when the BinaryVector is empty
//! @note If the BinaryVector has less than 64 bits, then a padding will be appled on value most significants bits
//! @note If the BinaryVector has more than 64 bits, then the value will take only the least significants bits
//!
//! @param value  Variable to update with current value
//!
void BinaryVector::Get (uint64_t&  value) const
{
  CHECK_NOT_EMPTY;

  uint8_t  lastByteBits   = LastByteBitsCount();
  uint32_t lastByteOffset = m_data.size() - 1u;

  uint8_t value_0 =  (m_usedBits > 0)  ? MergeToByte(lastByteOffset,      lastByteBits) : 0;  // LSB
  uint8_t value_1 =  (m_usedBits > 8)  ? MergeToByte(lastByteOffset - 1u, lastByteBits) : 0;
  uint8_t value_2 =  (m_usedBits > 16) ? MergeToByte(lastByteOffset - 2u, lastByteBits) : 0;
  uint8_t value_3 =  (m_usedBits > 24) ? MergeToByte(lastByteOffset - 3u, lastByteBits) : 0;
  uint8_t value_4 =  (m_usedBits > 32) ? MergeToByte(lastByteOffset - 4u, lastByteBits) : 0;
  uint8_t value_5 =  (m_usedBits > 40) ? MergeToByte(lastByteOffset - 5u, lastByteBits) : 0;
  uint8_t value_6 =  (m_usedBits > 48) ? MergeToByte(lastByteOffset - 6u, lastByteBits) : 0;
  uint8_t value_7 =  (m_usedBits > 56) ? MergeToByte(lastByteOffset - 7u, lastByteBits) : 0; // MSB

  // ---------------- Combine all bytes dealing with endianness
  //
  auto asBytes = reinterpret_cast<uint8_t*>(&value);
  #if __BYTE_ORDER == __LITTLE_ENDIAN
    asBytes[0] = value_0;
    asBytes[1] = value_1;
    asBytes[2] = value_2;
    asBytes[3] = value_3;
    asBytes[4] = value_4;
    asBytes[5] = value_5;
    asBytes[6] = value_6;
    asBytes[7] = value_7;
  #else
    asBytes[0] = value_7;
    asBytes[1] = value_6;
    asBytes[2] = value_5;
    asBytes[3] = value_4;
    asBytes[4] = value_3;
    asBytes[5] = value_2;
    asBytes[6] = value_1;
    asBytes[7] = value_0;
  #endif
}
//
//  End of: BinaryVector::Get
//---------------------------------------------------------------------------


//! Reads 8 signed bits value from BinaryVector
//!
//! @note This is an invalid operation when the BinaryVector is empty
//! @note If the BinaryVector has less than 8 bits, then a padding will be appled on value most significants bits (with sign extension)
//! @note If the BinaryVector has more than 8 bits, then the value will take only the least significants bits (truncation occurs)
//!
//! @param value  Variable to update with current value
//!
void BinaryVector::Get (int8_t&  value) const
{
  CHECK_NOT_EMPTY;

  uint8_t  lastByteBits   = LastByteBitsCount();
  uint32_t lastByteOffset = m_data.size() - 1u;

  value = MergeToByte(lastByteOffset, lastByteBits, true);
}
//
//  End of: BinaryVector::Get
//---------------------------------------------------------------------------


//! Reads 16 signed bits value from BinaryVector
//!
//! @note This is an invalid operation when the BinaryVector is empty
//! @note If the BinaryVector has less than 8 bits, then a padding will be appled on value most significants bits (with sign extension)
//! @note If the BinaryVector has more than 8 bits, then the value will take only the least significants bits (truncation occurs)
//!
//! @param value  Variable to update with current value
//!
void BinaryVector::Get (int16_t&  value) const
{
  CHECK_NOT_EMPTY;

  uint8_t  lastByteBits   = LastByteBitsCount();
  uint32_t lastByteOffset = m_data.size() - 1u;
  uint8_t  fillBits       = IsNegative() ? 0xFF : 0x00;

  uint8_t value_0 =  (m_usedBits > 0)  ? MergeToByte(lastByteOffset,      lastByteBits, true) : fillBits;  // LSB
  uint8_t value_1 =  (m_usedBits > 8)  ? MergeToByte(lastByteOffset - 1u, lastByteBits, true) : fillBits;  // MSB

  auto asBytes = reinterpret_cast<uint8_t*>(&value);
  #if __BYTE_ORDER == __LITTLE_ENDIAN
    asBytes[0] = value_0;
    asBytes[1] = value_1;
  #else
    asBytes[0] = value_1;
    asBytes[1] = value_0;
  #endif
}
//
//  End of: BinaryVector::Get
//---------------------------------------------------------------------------


//! Reads 32 signed bits value from BinaryVector
//!
//! @note This is an invalid operation when the BinaryVector is empty
//! @note If the BinaryVector has less than 8 bits, then a padding will be appled on value most significants bits (with sign extension)
//! @note If the BinaryVector has more than 8 bits, then the value will take only the least significants bits (truncation occurs)
//!
//! @param value  Variable to update with current value
//!
void BinaryVector::Get (int32_t&  value) const
{
  CHECK_NOT_EMPTY;

  uint8_t  lastByteBits   = LastByteBitsCount();
  uint32_t lastByteOffset = m_data.size() - 1u;
  uint8_t  fillBits       = IsNegative() ? 0xFF : 0x00;

  uint8_t value_0 = (m_usedBits > 0)  ? MergeToByte(lastByteOffset,       lastByteBits, true) : fillBits; // LSB
  uint8_t value_1 = (m_usedBits > 8)  ? MergeToByte(lastByteOffset  - 1u, lastByteBits, true) : fillBits;
  uint8_t value_2 = (m_usedBits > 16) ? MergeToByte(lastByteOffset  - 2u, lastByteBits, true) : fillBits;
  uint8_t value_3 = (m_usedBits > 24) ? MergeToByte(lastByteOffset  - 3u, lastByteBits, true) : fillBits; // MSB

  // ---------------- Combine all bytes dealing with endianness
  //
  auto asBytes = reinterpret_cast<uint8_t*>(&value);
  #if __BYTE_ORDER == __LITTLE_ENDIAN
    asBytes[0] = value_0;
    asBytes[1] = value_1;
    asBytes[2] = value_2;
    asBytes[3] = value_3;
  #else
    asBytes[0] = value_3;
    asBytes[1] = value_2;
    asBytes[2] = value_1;
    asBytes[3] = value_0;
  #endif
}
//
//  End of: BinaryVector::Get
//---------------------------------------------------------------------------


//! Reads 64 signed bits value from BinaryVector
//!
//! @note This is an invalid operation when the BinaryVector is empty
//! @note If the BinaryVector has less than 8 bits, then a padding will be appled on value most significants bits (with sign extension)
//! @note If the BinaryVector has more than 8 bits, then the value will take only the least significants bits (truncation occurs)
//!
//! @param value  Variable to update with current value
//!
void BinaryVector::Get (int64_t&  value) const
{
  CHECK_NOT_EMPTY;

  uint8_t  lastByteBits   = LastByteBitsCount();
  uint64_t lastByteOffset = m_data.size() - 1u;
  uint8_t  fillBits       = IsNegative() ? 0xFF : 0x00;

  uint8_t value_0 = (m_usedBits > 0)  ? MergeToByte(lastByteOffset,      lastByteBits, true) : fillBits; // LSB
  uint8_t value_1 = (m_usedBits > 8)  ? MergeToByte(lastByteOffset - 1u, lastByteBits, true) : fillBits;
  uint8_t value_2 = (m_usedBits > 16) ? MergeToByte(lastByteOffset - 2u, lastByteBits, true) : fillBits;
  uint8_t value_3 = (m_usedBits > 24) ? MergeToByte(lastByteOffset - 3u, lastByteBits, true) : fillBits; // MSB
  uint8_t value_4 = (m_usedBits > 32) ? MergeToByte(lastByteOffset - 4u, lastByteBits, true) : fillBits; // LSB
  uint8_t value_5 = (m_usedBits > 40) ? MergeToByte(lastByteOffset - 5u, lastByteBits, true) : fillBits;
  uint8_t value_6 = (m_usedBits > 48) ? MergeToByte(lastByteOffset - 6u, lastByteBits, true) : fillBits;
  uint8_t value_7 = (m_usedBits > 56) ? MergeToByte(lastByteOffset - 7u, lastByteBits, true) : fillBits; // MSB

  // ---------------- Combine all bytes dealing with endianness
  //
  auto asBytes = reinterpret_cast<uint8_t*>(&value);
  #if __BYTE_ORDER == __LITTLE_ENDIAN
    asBytes[0] = value_0;
    asBytes[1] = value_1;
    asBytes[2] = value_2;
    asBytes[3] = value_3;
    asBytes[4] = value_4;
    asBytes[5] = value_5;
    asBytes[6] = value_6;
    asBytes[7] = value_7;
  #else
    asBytes[0] = value_7;
    asBytes[1] = value_6;
    asBytes[2] = value_5;
    asBytes[3] = value_4;
    asBytes[4] = value_3;
    asBytes[5] = value_2;
    asBytes[6] = value_1;
    asBytes[7] = value_0;
  #endif
}
//
//  End of: BinaryVector::Get
//---------------------------------------------------------------------------

//! Masks last byte to be sure that unused bits are always set to zero
//!
void BinaryVector::MaskLastByte ()
{
  auto bitsOnLastByte = m_usedBits % 8;
  if (bitsOnLastByte != 0)
  {
    m_data.back() &= LEFT_BITS_MASK_8[bitsOnLastByte];
  }
}
//
//  End of: BinaryVector::MaskLastByte
//---------------------------------------------------------------------------


//! Merges bits from two bytes viewed as signed
//!
//! @param lsbOffset      Rightmost byte to merge
//! @param lsbBitsCount   Number of bits to take form rightmost byte
//! @param asSigned       When true underlying value is considered signed
//!
uint8_t BinaryVector::MergeToByte (uint32_t lsbOffset, uint8_t lsbBitsCount, bool asSigned) const
{
  const uint8_t secondToLastByteBits = 8u - lsbBitsCount;

  uint8_t value_L = m_data[lsbOffset] >> secondToLastByteBits;

  if (lsbOffset == 0)
  {
    if (asSigned && IsNegative())
    {
      value_L |= LEFT_BITS_MASK_8[secondToLastByteBits];
    }
    return value_L;
  }

  auto value_H = m_data[lsbOffset - 1u] << lsbBitsCount;
  auto value   = value_H | value_L;

  return value;
}
//
//  End of: BinaryVector::MergeToByte
//---------------------------------------------------------------------------


//! Appends 0 or 1 bit N times
//!
//! @param bitIsOne   When true '1' otherwise '0' are appended to the vector
//! @param count      Number of '0' or '1' to append
//!
BinaryVector& BinaryVector::AppendBits (bool bitIsOne, uint32_t count)
{
  uint8_t value = bitIsOne ? 0xFF : 0;
  auto    bytesCount = count / 8u;

  if (bytesCount != 0)
  {
    m_data.insert(m_data.cend(), bytesCount, value);

    auto addedBits = 8u * bytesCount;
    m_usedBits    += addedBits;
    count         -= addedBits;
  }

  if (count != 0)
  {
    Append(value, count);
  }

  return *this;
}
//
//  End of: BinaryVector::AppendBits
//---------------------------------------------------------------------------

//! Sets from N signed bits
//!
template<typename T>
void BinaryVector::SetSigned (T value)
{
  static_assert(std::is_signed<T>::value, "Must only be called for signed value as it includes sign bit extension");
  auto constexpr valueBitsCount   = uint32_t(8u * sizeof(value));
  auto           initialBitsCount = m_usedBits;

  Clear();

  if (!FixedSize() || (initialBitsCount == 0))
  {
    Append(static_cast<std::make_unsigned_t<T>>(value));
  }
  else
  {
    m_sizeProperty = SizeProperty::NotFixed;

    auto bitsCountToAppend = initialBitsCount;

    if (initialBitsCount > valueBitsCount)
    {
      auto msbCount = initialBitsCount - valueBitsCount;

      AppendBits(value < 0, msbCount);
      bitsCountToAppend = valueBitsCount;
    }

    Append(static_cast<std::make_unsigned_t<T>>(value), bitsCountToAppend);
    m_sizeProperty = SizeProperty::Fixed;
  }
}


//! Sets from N unsigned bits
//!
template<typename T>
void BinaryVector::SetUnsigned (T value)
{
  static_assert(std::is_unsigned<T>::value, "Must only be called for unsigned value as it DOES NOT includes sign bit extension");

  auto constexpr valueBitsCount   = uint32_t(8u * sizeof(value));
  auto           initialBitsCount = m_usedBits;

  Clear();

  if (!FixedSize() || (initialBitsCount == 0))
  {
    Append(value);
  }
  else
  {
    m_sizeProperty = SizeProperty::NotFixed;

    auto bitsCountToAppend = initialBitsCount;

    if (initialBitsCount > valueBitsCount)
    {
      auto msbCount = initialBitsCount - valueBitsCount;

      AppendBits(false, msbCount);
      bitsCountToAppend = valueBitsCount;
    }

    Append(value, bitsCountToAppend);
    m_sizeProperty = SizeProperty::Fixed;
  }
}


//! Sets from 8 bits
//!
void BinaryVector::Set (int8_t value)
{
  SetSigned(value);
}
//
//  End of: BinaryVector::Set
//---------------------------------------------------------------------------


//! Sets from 16 bits
//!
void BinaryVector::Set (int16_t value)
{
  SetSigned(value);
}
//
//  End of: BinaryVector::Set
//---------------------------------------------------------------------------


//! Sets from 32 bits
//!
void BinaryVector::Set (int32_t value)
{
  SetSigned(value);
}
//
//  End of: BinaryVector::Set
//---------------------------------------------------------------------------


//! Sets from 64 bits
//!
void BinaryVector::Set (int64_t value)
{
  SetSigned(value);
}
//
//  End of: BinaryVector::Set
//---------------------------------------------------------------------------


//! Sets from 8 bits
//!
void BinaryVector::Set (uint8_t value)
{
  SetUnsigned(value);
}
//
//  End of: BinaryVector::Set
//---------------------------------------------------------------------------


//! Sets from 16 bits
//!
void BinaryVector::Set (uint16_t value)
{
  SetUnsigned(value);
}
//
//  End of: BinaryVector::Set
//---------------------------------------------------------------------------

//! Sets from 32 bits
//!
void BinaryVector::Set (uint32_t value)
{
  SetUnsigned(value);
}
//
//  End of: BinaryVector::Set
//---------------------------------------------------------------------------


//! Sets from 64 bits
//!
void BinaryVector::Set (uint64_t value)
{
  SetUnsigned(value);
}
//
//  End of: BinaryVector::Set
//---------------------------------------------------------------------------




//! Sets specified bit (to one)
//!
//! @param bitOffset Zero based bit offset (from left)
//!
void BinaryVector::SetBit (uint32_t bitOffset)
{
  CHECK_PARAMETER_LT(bitOffset, m_usedBits, "Out of range bit id: "s + to_string(bitOffset));

  auto byteOffset      = bitOffset / 8;
  auto bitOffsetInByte = bitOffset % 8;

  m_data[byteOffset] |= BIT_MASK_8[bitOffsetInByte];
}
//
//  End of: BinaryVector::SetBit
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
    CHECK_PARAMETER_LT  (firstBitOffset,               m_usedBits, "Slice first bit must be within bits range");
    CHECK_PARAMETER_LTE ((firstBitOffset + bitsCount), m_usedBits, "Bits count must be such that slice is within bits range");

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


//! Toggle specified bit
//!
//! @param bitOffset Zero based bit offset (from left)
//!
void BinaryVector::ToggleBit (uint32_t bitOffset)
{
  CHECK_PARAMETER_LT(bitOffset, m_usedBits, "Out of range bit id: "s + to_string(bitOffset));

  auto byteOffset      = bitOffset / 8;
  auto bitOffsetInByte = bitOffset % 8;

  m_data[byteOffset] ^= BIT_MASK_8[bitOffsetInByte];
}
//
//  End of: BinaryVector::ToggleBit
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

  MaskLastByte();

  return *this;
}
//
//  End of: BinaryVector::ToggleBits
//---------------------------------------------------------------------------



//===========================================================================
// End of BinaryVector.cpp
//===========================================================================
