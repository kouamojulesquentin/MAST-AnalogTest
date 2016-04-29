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

//===========================================================================
// End of Register.cpp
//===========================================================================
