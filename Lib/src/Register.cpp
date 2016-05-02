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
  : SystemModelNode        (name)
  , m_sequenceToSend       (bypassSequence)
  , m_lastSentSequence     (bypassSequence)
  , m_lastReceivedSequence (bypassSequence)
  , m_expectedSequence     (bypassSequence)
  , m_bypassSequence       (std::move(bypassSequence))
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
//! @param lastReceivedSequence   Sequence received from SUT during last scan
//!
void Register::SetLastReceivedSequence (BinaryVector lastReceivedSequence)
{
  m_lastReceivedSequence = std::move(lastReceivedSequence);

  if (m_mustCheckExpected && (m_lastReceivedSequence != m_expectedSequence))
  {
    ++m_mismatches;
  }
}
//
//  End of: Register::SetLastReceivedSequence
//---------------------------------------------------------------------------

//! Sets the bits sequence to send during the next iApply cycle and set selection
//! pending flag when the value is different
//!
void Register::SetSequenceToSend (BinaryVector sequenceToSend)
{
  m_sequenceToSend = sequenceToSend;

  if (m_sequenceToSend != m_lastSentSequence)
  {
    m_pendingSelect = true;
  }
}
//
//  End of: Register::SetSequenceToSend
//---------------------------------------------------------------------------






//===========================================================================
// End of Register.cpp
//===========================================================================
