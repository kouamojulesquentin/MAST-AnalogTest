//===========================================================================
//                           PropagatePendingVisitor.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PropagatePendingVisitor.cpp
//!
//! Implements class PropagatePendingVisitor
//!
//===========================================================================

#include "PropagatePendingVisitor.hpp"
#include "SystemModelNodes.hpp"

using namespace mast;


//! Updates AccessInterface pending flag
//!
void PropagatePendingVisitor::VisitAccessInterface (AccessInterface& accessInterface)
{
  auto pendings = ChildrenPendings(accessInterface);
  accessInterface.SetPendingsCount(pendings);
}
//
//  End of: PropagatePendingVisitor::VisitAccessInterface
//---------------------------------------------------------------------------



//! Updates Chain pending flag
//!
void PropagatePendingVisitor::VisitChain (Chain& chain)
{
  auto pendings = ChildrenPendings(chain);
  chain.SetPendingsCount(pendings);
}
//
//  End of: PropagatePendingVisitor::VisitChain
//---------------------------------------------------------------------------


//! Visits direct children of a parent node, returning true if at least one is pending
//!
uint32_t PropagatePendingVisitor::ChildrenPendings (const ParentNode& parentNode)
{
  uint32_t pendings = 0;
  auto     child    = parentNode.FirstChild();

  while (child)
  {
    child->Accept(*this);
    pendings += child->PendingsCount();

    child = child->NextSibling();
  }

  return pendings;
}
//
//  End of: PropagatePendingVisitor::ChildrenPendings
//---------------------------------------------------------------------------


//! Updates Linker pending flag
//!
void PropagatePendingVisitor::VisitLinker (Linker& linker)
{
  auto pendings = ChildrenPendings(linker);
  linker.SetPendingsCount(pendings);
}
//
//  End of: PropagatePendingVisitor::VisitLinker
//---------------------------------------------------------------------------



//! Updates Register pending flag
//!
void PropagatePendingVisitor::VisitRegister (Register& reg)
{
  auto isPending = reg.NextToSut() != reg.LastToSut();
  if (isPending)
  {
    reg.SetPending();
  }
}
//
//  End of: PropagatePendingVisitor::VisitRegister
//---------------------------------------------------------------------------



//===========================================================================
// End of PropagatePendingVisitor.cpp
//===========================================================================
