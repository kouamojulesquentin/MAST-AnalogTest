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
  , m_nextToSut       (bypassSequence,            SizeProperty::Fixed)
  , m_lastToSut       (bypassSequence,            SizeProperty::Fixed)
  , m_lastFromSut     (bypassSequence,            SizeProperty::Fixed)
  , m_lastReadFromSut (bypassSequence.BitsCount())
  , m_expectedFromSut (bypassSequence,            SizeProperty::Fixed)
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


//! Returns XOR of the last value read from SUT and the expected value
//!
//! @note May contain x-values (for don't care)
//!
BinaryVector Register::LastCompareResult () const
{
  auto result = m_lastReadFromSut ^ m_expectedFromSut;

  if (!m_dontCareMask.IsEmpty())
  {
    result &= m_dontCareMask;
  }
  return result;
}
//
//  End of: Register::LastCompareResult
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


//! Sets expected sequence and don't care mask (when updating from SUT)
//!
//! @param sequence     New expected value to read from SUT
//! @param dontCareMask Don't care mask (relative to expected value).
//!                     If it is empty, then all bits will be cared-of
//!
//! @warning dontCareMask must be either empty or have same size as Register
//!
void Register::SetExpectedFromSut (BinaryVector sequence, BinaryVector dontCareMask)
{
  if (dontCareMask.IsEmpty())
  {
    m_dontCareMask.Clear();
    SetExpectedFromSut(sequence);
    return;
  }

  CHECK_PARAMETER_EQ(dontCareMask.BitsCount(), sequence.BitsCount(), "Don't care mask must have same size as Register");

  if (m_dontCareMask.IsEmpty())
  {
    m_dontCareMask = BinaryVector(m_expectedFromSut.BitsCount(), 0, SizeProperty::Fixed);
  }
  m_expectedFromSut = std::move(sequence);
  m_dontCareMask    = std::move(dontCareMask);
}


//! Sets last sequence of bits that have been shifted from SUT
//!
//! @param sequence   Sequence received from SUT during last scan
//!
void Register::SetFromSut (BinaryVector sequence)
{
  if (m_pendingRead)
  {
    m_lastReadFromSut = sequence;
  }

  if (m_mustCheckExpected && !sequence.CompareEqualTo(m_expectedFromSut, m_dontCareMask))
  {
    ++m_mismatches;
  }
  m_lastFromSut = std::move(sequence);
}
//
//  End of: Register::SetFromSut
//---------------------------------------------------------------------------

//===========================================================================
// End of Register.cpp
//===========================================================================
