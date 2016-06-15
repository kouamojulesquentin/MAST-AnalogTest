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
using std::initializer_list;
using std::ostringstream;
using std::string;
using std::experimental::string_view;

using namespace std::string_literals;
using namespace mast;

#define CHECK_SAME_SIZE(other)                if (other.m_usedBits != m_usedBits)         THROW_LOGIC_ERROR("BinaryVectors must have same size")
#define CHECK_FIXED_SIZE                      if (FixedSize())                            THROW_LOGIC_ERROR("BinaryVector size has been fixed")
#define CHECK_FIXED_SIZE_ASSIGNMENT(newSize)  if (FixedSize() && (newSize != m_usedBits)) THROW_LOGIC_ERROR("BinaryVector size has been fixed")
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
//! @param value          The value to append to the vector
//! @param numberOfBits   Number of useful bits in the value
//! @param alignment      Tells whether bits are left (msb) or right (lsb) aligned
//!
BinaryVector& BinaryVector::Append (uint16_t value, uint8_t numberOfBits, BitsAlignment alignment)
{
  CHECK_FIXED_SIZE;
  CHECK_AT_LEAST_1_BIT(numberOfBits);

  //! @todo [JFC]-[April/25/2016]: Use Boost.Endian to manage properly endianness
  //!

  uint8_t byte_1 = (value >>  8) & 0xff;
  uint8_t byte_2 = (value >>  0) & 0xff;

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

  //! @todo [JFC]-[April/25/2016]: Use Boost.Endian to manage properly endianness
  //!
  uint8_t byte_1 = (value >> 24) & 0xff;
  uint8_t byte_2 = (value >> 16) & 0xff;
  uint8_t byte_3 = (value >>  8) & 0xff;
  uint8_t byte_4 = (value >>  0) & 0xff;

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

  //! @todo [JFC]-[April/25/2016]: Use Boost.Endian to manage properly endianness
  //!

  uint8_t byte_1 = (value >> 56) & 0xff;
  uint8_t byte_2 = (value >> 48) & 0xff;
  uint8_t byte_3 = (value >> 40) & 0xff;
  uint8_t byte_4 = (value >> 32) & 0xff;
  uint8_t byte_5 = (value >> 24) & 0xff;
  uint8_t byte_6 = (value >> 16) & 0xff;
  uint8_t byte_7 = (value >>  8) & 0xff;
  uint8_t byte_8 = (value >>  0) & 0xff;

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



//! Gets content as formatted binary string
//!
//! @note An example of formatting is: 0001_1111:0011_0100:0101_010
//!
//! @param octoSeparator    Characters to insert every 8 bits
//! @param quadSeparator  Characters to insert every 4 bits
//! @param bytesPerLine     Number of bytes (sequence of 8 bits) to write per line.
//!                         When zero, all is on the "same line"
//! @param eolSeparator     Characters to insert just before new lines (when bytesPerLine != 0)
//!
string BinaryVector::DataAsBinaryString (string_view quadSeparator,
                                         string_view octoSeparator,
                                         uint32_t    bytesPerLine,
                                         string_view eolSeparator
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
//! @param octoSeparator    Characters to insert every 32 bits
//! @param quadSeparator  Characters to insert every 16 bits
//! @param bytesPerLine    Number of bytes (sequence of 8 bits) to write per line.
//!                        When zero, all is on the "same line"
//! @param eolSeparator    Characters to insert just before new lines character (when bytesPerLine != 0)
//!
string BinaryVector::DataAsHexString (string_view quadSeparator,
                                      string_view octoSeparator,
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
    return "0b"s + DataAsBinaryString(quadSeparator, octaSeparator, bytesPerLine, eolSeparator);
  }

  auto smartString       = "0x"s + DataAsHexString(quadSeparator, octaSeparator, bytesPerLine, eolSeparator);
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



//! Copy assignment
//!
//! @note Does not change the fixed size property
//!
BinaryVector& BinaryVector::operator= (const BinaryVector& rhs)
{
  CHECK_FIXED_SIZE_ASSIGNMENT(rhs.m_usedBits);

  if (this != &rhs)
  {
    m_data         = rhs.m_data;
    m_usedBits     = rhs.m_usedBits;
    m_sizeProperty = rhs.m_sizeProperty == SizeProperty::FixedOnCopy ? SizeProperty::FixedOnCopy : m_sizeProperty;
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
    m_data         = std::move(rhs.m_data);
    m_usedBits     = rhs.m_usedBits;
    m_sizeProperty = rhs.m_sizeProperty == SizeProperty::FixedOnCopy ? SizeProperty::FixedOnCopy : m_sizeProperty;
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
//!               Characters in ",':_- \t/\" are ignored (can be used to ease display of string)
//!               An exception is thrown if there is any character different from
//!               set "01,':_- \t"
//!               '0b' is ignored at start of string. An exception is thrown everywhere else
//!               '/b', '/B', '\b', '\B' constructions are ignored anywhere
//!
//! @return A new BinaryVector initialized as defined by bits text
BinaryVector BinaryVector::CreateFromBinaryString (std::experimental::string_view bits, SizeProperty sizeProperty)
{
  BinaryVector result;

  uint8_t nextByte = 0;
  auto    bitCount = 0;

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
      case ',':
      case '\'':
      case '\t':
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
        THROW_INVALID_ARGUMENT("CreateFromBinaryString only support characters in '01,\':_-\\x20\\t'");
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
//!               set "0123456789abcdefABCDEF,':_- \t/\"
//!               '0x' is ignored at start of string. An exception is thrown everywhere else
//!               '/x', '/X', '\x', '\X' constructions are ignored anywhere
//!
//! @return A new BinaryVector initialized as defined by bits text
//!
BinaryVector BinaryVector::CreateFromHexString (string_view bits, SizeProperty sizeProperty)
{
  BinaryVector result;

  uint8_t nextByte = 0;
  auto    bitCount = 0;

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
      case ',':
      case '\'':
      case '\t':
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
        THROW_INVALID_ARGUMENT("CreateFromBinaryString only support characters in '01,\':_-\\x20\\t/\\'");
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

  auto format = StringFormat::Undefined;
  bool firstCharOk = (bits[0] == '0') || (bits[0] == '/') || (bits[0] == '\\');

  if      (firstCharOk && ((bits[1] == 'x') || (bits[1] == 'X')))
  {
    format = StringFormat::Hexadecimal;
  }
  else if (firstCharOk && ((bits[1] == 'b') || (bits[1] == 'B')))
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

  bits.remove_prefix(2);  // Remove one of: '0x', '/x', '\x', '0b', '/b' and '\b'
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

  MaskLastByte();

  return *this;
}
//
//  End of: BinaryVector::ToggleBits
//---------------------------------------------------------------------------



//===========================================================================
// End of BinaryVector.cpp
//===========================================================================
