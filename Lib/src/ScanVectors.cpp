//===========================================================================
//                           ScanVectors.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ScanVectors.cpp
//!
//! Implements BinaryVector and SVFVector classes
//!
//===========================================================================


#include "ScanVectors.hpp"
#include "Utility.hpp"

using namespace mast;

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
  : m_data     (rhs.m_data)
  , m_usedBits (std::move(rhs.m_usedBits))
{
}
//
//  End of: BinaryVector::BinaryVector
//---------------------------------------------------------------------------


//! Copy assignment
//!
BinaryVector& BinaryVector::operator= (const BinaryVector& rhs)
{
  if (this != &rhs)
  {
    m_data     = rhs.m_data;
    m_usedBits = std::move(rhs.m_usedBits);
  }
  return *this;
}
//
//  End of: ScanVectors::ScanVectors
//---------------------------------------------------------------------------


//! Move assignment
//!
BinaryVector& BinaryVector::operator= (BinaryVector&& rhs) noexcept
{
  if (this != &rhs)
  {
    m_data     = rhs.m_data;
    m_usedBits = std::move(rhs.m_usedBits);
  }
  return *this;
}
//
//  End of: ScanVectors::ScanVectors
//---------------------------------------------------------------------------


//! Appends from 8 bits
//!
BinaryVector& BinaryVector::Append (uint8_t value, uint8_t numberOfBits)
{
  if (numberOfBits == 0)
  {
    THROW_INVALID_ARGUMENT("Number of bits to append must be != 0");
  }

  if (numberOfBits > 8 * sizeof(uint8_t))
  {
    THROW_INVALID_ARGUMENT("Number of append bits cannot exceed number of bits of value.");
  }

  uint8_t freeBits = m_usedBits % 8;

  m_data.push_back(value);
  m_usedBits += numberOfBits;

  return *this;
}
//
//  End of: ScanVectors::ScanVectors
//---------------------------------------------------------------------------


//! Appends from 16 bits
//!
BinaryVector& BinaryVector::Append (uint16_t value)
{
  //! @todo [JFC]-[April/25/2016]: Use Boost.Endian to manage properly endianness
  //!
  m_data.push_back((value >>  8) & 0xff);
  m_data.push_back((value >>  0) & 0xff);

  m_usedBits += 16;

  return *this;
}
//
//  End of: ScanVectors::ScanVectors
//---------------------------------------------------------------------------

//! Appends from 32 bits
//!
BinaryVector& BinaryVector::Append (uint32_t value)
{
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
//  End of: ScanVectors::ScanVectors
//---------------------------------------------------------------------------


//! Appends from 64 bits
//!
BinaryVector& BinaryVector::Append (uint64_t value)
{
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
//  End of: ScanVectors::ScanVectors
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


//! Sets from 8 bits
//!
void BinaryVector::Set (uint8_t value)
{
  Clear();
  Append(value);
}
//
//  End of: ScanVectors::ScanVectors
//---------------------------------------------------------------------------


//! Sets from 16 bits
//!
void BinaryVector::Set (uint16_t value)
{
  Clear();
  Append(value);
}
//
//  End of: ScanVectors::ScanVectors
//---------------------------------------------------------------------------

//! Sets from 32 bits
//!
void BinaryVector::Set (uint32_t value)
{
  Clear();
  Append(value);
}
//
//  End of: ScanVectors::ScanVectors
//---------------------------------------------------------------------------


//! Sets from 64 bits
//!
void BinaryVector::Set (uint64_t value)
{
  Clear();
  Append(value);
}
//
//  End of: ScanVectors::ScanVectors
//---------------------------------------------------------------------------


//! Toggles (flips) every bits of the vector
//!
//! @return Same vector with all the bits toggled
BinaryVector& BinaryVector::operator~()
{
  for (auto& byte : m_data)
  {
    byte = ~byte;
  }
  return *this;
}
//
//  End of: BinaryVectoroperator~
//---------------------------------------------------------------------------



//===========================================================================
// End of ScanVectors.cpp
//===========================================================================
