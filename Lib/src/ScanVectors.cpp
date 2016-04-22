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

using namespace mast;

//! Copy constructor
//!
BinaryVector::BinaryVector (const BinaryVector& rhs)
  : m_data     (rhs.m_data)
  , m_usedBits (rhs.m_usedBits)
{
}
//
//  End of: ScanVectors::ScanVectors
//---------------------------------------------------------------------------


//! Move constructor
//!
BinaryVector::BinaryVector (BinaryVector&& rhs) noexcept
  : m_data     (rhs.m_data)
  , m_usedBits (std::move(rhs.m_usedBits))
{
}
//
//  End of: ScanVectors::ScanVectors
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
