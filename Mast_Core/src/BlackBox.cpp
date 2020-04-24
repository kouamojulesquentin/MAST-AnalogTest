//===========================================================================
//                           BlackBox.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file BlackBox.cpp
//!
//! Implements class BlackBox
//===========================================================================

#include "BlackBox.hpp"
#include "SystemModelVisitor.hpp"
#include "Utility.hpp"
#include <utility>

using std::experimental::string_view;

using namespace mast;


//! Initializes a new BlackBox (with a reset value)
//!
//! @param name             BlackBox name (to be identified using a path)
//! @param bypassSequence   Sequence of bits that define initial and bypass value
//! @param resetSequence    Sequence of bits that define the reset state
//! @param holdValue        When true, register value from iWrite command is maintained (by setting the bypass with the same value)
//! @param bitsOrdering     Bits ordering usage for this register
//!
//! @note All internal BinaryVector are initialized like the bypass sequence
//!
BlackBox::BlackBox (string_view        name,
                    mast::BinaryVector bypassSequence,
                    mast::BinaryVector resetSequence,
                    bool               holdValue,
                    mast::BitsOrdering bitsOrdering)
  : SystemModelNode   (name)
  , m_holdValue       (holdValue)
  , m_bitsOrdering    (bitsOrdering)
  , m_nextToSut       (bypassSequence,            SizeProperty::Fixed)
  , m_lastToSut       (bypassSequence,            SizeProperty::Fixed)
  , m_lastFromSut     (bypassSequence,            SizeProperty::Fixed)
  , m_lastReadFromSut (bypassSequence.BitsCount())
  , m_expectedFromSut (bypassSequence,            SizeProperty::Fixed)
  , m_bypass          (std::move(bypassSequence), SizeProperty::Fixed)
  , m_resetValue      (std::move(resetSequence))
  , m_identifiers     ({ Identifier() })
{
  if (!m_resetValue.IsEmpty())
  {
    CHECK_PARAMETER_EQ(m_resetValue.BitsCount(), m_bypass.BitsCount(), "Reset sequence must have same number of bits as bypass sequence");
    m_resetValue.FixSize(true);
  }
}
//
//  End of: BlackBox::BlackBox
//---------------------------------------------------------------------------



//! Initializes a new BlackBox
//!
//! @param name             BlackBox name (to be identified using a path)
//! @param bypassSequence   Sequence of bits that define initial and bypass value
//! @param holdValue        When true, register value from iWrite command is maintained (by setting the bypass with the same value)
//! @param bitsOrdering     Bits ordering usage for this register
//!
//! @note All registers are initialized like the bypass sequence
//!
BlackBox::BlackBox (string_view        name,
                    mast::BinaryVector bypassSequence,
                    bool               holdValue,
                    mast::BitsOrdering bitsOrdering)
  : BlackBox   (name, bypassSequence, BinaryVector(), holdValue, bitsOrdering)
{
}
//
//  End of: BlackBox::BlackBox
//---------------------------------------------------------------------------



//! Visited part of the Visitor pattern
//!
void BlackBox::Accept (SystemModelVisitor& visitor)
{
  visitor.VisitBlackBox(*this);
}



//! Returns true if register is pending for read or for write
//!
bool BlackBox::IsPending () const
{
  return m_pendingRead || SystemModelNode::IsPending();
}
//
//  End of: BlackBox::IsPending
//---------------------------------------------------------------------------


//! Returns XOR of the last value read from SUT and the expected value
//!
//! @note May contain x-values (for don't care)
//!
BinaryVector BlackBox::LastCompareResult () const
{
  auto result = m_lastReadFromSut ^ m_expectedFromSut;

  if (!m_dontCareMask.IsEmpty())
  {
    result &= m_dontCareMask;
  }
  return result;
}
//
//  End of: BlackBox::LastCompareResult
//---------------------------------------------------------------------------


//! Returns number of pending registers down the hierarchy
//!
uint32_t BlackBox::PendingCount () const
{
  return (m_pendingRead || SystemModelNode::IsPending()) ? 1u : 0;
}
//
//  End of: BlackBox::PendingCount
//---------------------------------------------------------------------------


//! When the reset sequence is not empty, resets internal state
//!
//! @warning Does nothing when reset sequence is empty
//!
void BlackBox::Reset ()
{
  ResetPending();

  m_mismatches        = 0;
  m_mustCheckExpected = false;
  m_dontCareMask.Clear();

  if (m_resetValue.IsEmpty())
  {
    return;
  }


//+  m_expectedFromSut = m_resetValue;
//+  m_lastFromSut     = m_resetValue;
//+  m_lastReadFromSut = m_resetValue;
//+  m_lastToSut       = m_resetValue;

  m_bypass    = m_resetValue;
  m_nextToSut = m_resetValue;
}
//
//  End of: BlackBox::Reset
//---------------------------------------------------------------------------



//! Resets the number of pending
//!
void BlackBox::ResetPending ()
{
  m_pendingRead = false;
  SystemModelNode::ResetPending();
}
//
//  End of: BlackBox::ResetPending
//---------------------------------------------------------------------------


//! Sets expected sequence and don't care mask (when updating from SUT)
//!
//! @param sequence     New expected value to read from SUT
//! @param dontCareMask Don't care mask (relative to expected value).
//!                     If it is empty, then all bits will be cared-of
//!
//! @warning dontCareMask must be either empty or have same size as BlackBox
//!
void BlackBox::SetExpectedFromSut (BinaryVector sequence, BinaryVector dontCareMask)
{
  if (dontCareMask.IsEmpty())
  {
    m_dontCareMask.Clear();
    SetExpectedFromSut(sequence);
    return;
  }

  CHECK_PARAMETER_EQ(dontCareMask.BitsCount(), sequence.BitsCount(), "Don't care mask must have same size as BlackBox");

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
void BlackBox::SetFromSut (BinaryVector sequence)
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
//  End of: BlackBox::SetFromSut
//---------------------------------------------------------------------------

//===========================================================================
// End of BlackBox.cpp
//===========================================================================
