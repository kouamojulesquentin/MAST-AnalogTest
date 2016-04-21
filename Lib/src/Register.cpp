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

using std::string;
using namespace mast;


//! Initialize a new Register
//!
Register::Register (string name, uint32_t bitsCount, RegisterBits bypassSequence)
  : SystemModelNode        (std::move(name))
  , m_bitsCount            (bitsCount)
  , m_bytesCount           (Utility::BytesCountFromBitsCount(bitsCount))
  , m_sequenceToSend       (m_bytesCount, 0)
  , m_lastSentSequence     (m_bytesCount, 0)
  , m_lastReceivedSequence (m_bytesCount, 0)
  , m_expectedSequence     (m_bytesCount, 0)
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

//===========================================================================
// End of Register.cpp
//===========================================================================
