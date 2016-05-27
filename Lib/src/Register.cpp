//===========================================================================
//                           Register.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Register.cpp
//!
//! Implements class Register
//===========================================================================

#include "Register.hpp"
#include "SystemModelVisitor.hpp"
#include "Utility.hpp"
#include <utility>

using std::experimental::string_view;

using namespace mast;


//! Initializes a new Register
//!
//! @note All registers are initialized like the bypass sequence
Register::Register (string_view name, mast::BinaryVector bypassSequence)
  : SystemModelNode   (name)
  , m_nextToSut       (bypassSequence, SizeProperty::Fixed)
  , m_lastToSut       (bypassSequence, SizeProperty::Fixed)
  , m_lastFromSut     (bypassSequence, SizeProperty::Fixed)
  , m_expectedFromSut (bypassSequence, SizeProperty::Fixed)
  , m_bypass          (std::move(bypassSequence), SizeProperty::Fixed)
{
}
//
//  End of: Register::Register
//---------------------------------------------------------------------------



//! Visited part of the Visitor pattern
//!
void Register::Accept (SystemModelVisitor& visitor)
{
  visitor.VisitRegister(*this);
}


//! Sets last sequence of bits that have been shifted from SUT
//!
//! @param sequence   Sequence received from SUT during last scan
//!
void Register::SetFromSut (BinaryVector sequence)
{
  m_lastFromSut = std::move(sequence);

  if (m_mustCheckExpected && (m_lastFromSut != m_expectedFromSut))
  {
    ++m_mismatches;
  }
}
//
//  End of: Register::SetFromSut
//---------------------------------------------------------------------------

//===========================================================================
// End of Register.cpp
//===========================================================================
