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
#include "AccessInterfaceTranslator.hpp"

using namespace mast;



//! Stops recursing down the model hierarchy
//!
//! @note ToSutVisitor must stop at AccessInterface boundary in order to manage hierarchical processing of the model
//!       By CONSEQUENCE it MUST NOT initially be started on an AccessInterface BUT successively on its endpoints (children)
//!
void ToSutVisitor::VisitAccessInterface (AccessInterface& /* accessInterface */)
{
}
//
//  End of: ToSutVisitor::VisitAccessInterface
//---------------------------------------------------------------------------


//! Visits AccessInterfaceTranslator pending children
//!
void ToSutVisitor::VisitAccessInterfaceTranslator (AccessInterfaceTranslator& accessInterfaceTranslator)
{
  VisitChildren(accessInterfaceTranslator);
}
//
//  End of: ToSutVisitor::VisitChain
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
//! @note __Should be only used (indirectly called) for active (currently selected) Registers__
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

//! BBox is treated like it was a register with a size fixed for this cycle
//! Appends BlackBox value to send to SUT while saving the fact that it was active
//!
//! @note __Should be only used (indirectly called) for active (currently selected) BlackBox__
//!
void ToSutVisitor::VisitBlackBox (BlackBox& bbox)
{
  if (m_ignorePendingState || bbox.IsPendingForWrite())
  {
    m_toSutVector.Append(bbox.NextToSut());
  }
  else
  {
    m_toSutVector.Append(bbox.BypassSequence());
  }
  m_activeRegisters.emplace_back(bbox.Identifier());
}
//
//  End of: ToSutVisitor::VisitRegister
//---------------------------------------------------------------------------


//===========================================================================
// End of ToSutVisitor.cpp
//===========================================================================
