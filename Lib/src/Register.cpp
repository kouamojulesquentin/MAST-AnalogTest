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
Register::Register (string_view name, mast::BinaryVector bypassSequence, bool holdValue)
  : SystemModelNode   (name)
  , m_holdValue       (holdValue)
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



//! Returns true if register is pending for read or for write
//!
bool Register::IsPending () const
{
  return m_pendingRead || SystemModelNode::IsPending();
}
//
//  End of: Register::IsPending
//---------------------------------------------------------------------------


//! Returns number of pending registers down the hierarchy
//!
uint32_t Register::PendingCount () const
{
  return (m_pendingRead || SystemModelNode::IsPending()) ? 1u : 0;
}
//
//  End of: Register::PendingCount
//---------------------------------------------------------------------------



//! Resets the number of pending
//!
void Register::ResetPending ()
{
  m_pendingRead = false;
  SystemModelNode::ResetPending();
}
//
//  End of: Register::ResetPending
//---------------------------------------------------------------------------



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
