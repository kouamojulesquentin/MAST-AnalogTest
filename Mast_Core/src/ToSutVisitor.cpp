//===========================================================================
//                           ToSutVisitor.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ToSutVisitor.cpp
//!
//! Implements class ToSutVisitor
//!
//===========================================================================

#include "ToSutVisitor.hpp"
#include "SystemModelNodes.hpp"

using namespace mast;



//! Visits AccessInterface pending children
//!
void ToSutVisitor::VisitAccessInterface (AccessInterface& accessInterface)
{
  VisitChildren(accessInterface);
}
//
//  End of: ToSutVisitor::VisitAccessInterface
//---------------------------------------------------------------------------



//! Visits Chain pending children
//!
void ToSutVisitor::VisitChain (Chain& chain)
{
  VisitChildren(chain);
}
//
//  End of: ToSutVisitor::VisitChain
//---------------------------------------------------------------------------




//! Visits Linker pending children
//!
void ToSutVisitor::VisitLinker (Linker& linker)
{
  VisitActiveRegisters(linker);
}
//
//  End of: ToSutVisitor::VisitLinker
//---------------------------------------------------------------------------



//! Appends Register value to send to SUT while saving the fact that it was active
//!
//! @note Should be only used (indirectly called) for active (currently selected) Registers
//!       =================================================================================
//!
void ToSutVisitor::VisitRegister (Register& reg)
{
  if (m_ignorePendingState || reg.IsPendingForWrite())
  {
    m_toSutVector.Append(reg.NextToSut());
  }
  else
  {
    m_toSutVector.Append(reg.BypassSequence());
  }
  m_activeRegisters.emplace_back(reg.Identifier());
}
//
//  End of: ToSutVisitor::VisitRegister
//---------------------------------------------------------------------------



//===========================================================================
// End of ToSutVisitor.cpp
//===========================================================================
