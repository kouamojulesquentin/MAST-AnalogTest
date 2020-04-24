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
  auto pendingCount = ChildrenPendings(accessInterface);
  accessInterface.SetPendingCount(pendingCount);
}
//
//  End of: PropagatePendingVisitor::VisitAccessInterface
//---------------------------------------------------------------------------

//! Updates AccessInterfaceTranslator pending flag
//!
void PropagatePendingVisitor::VisitAccessInterfaceTranslator (AccessInterfaceTranslator& accessInterfaceTranslator)
{
  auto pendingCount = ChildrenPendings(accessInterfaceTranslator);
  accessInterfaceTranslator.SetPendingCount(pendingCount);
}
//
//  End of: PropagatePendingVisitor::VisitAccessInterface
//---------------------------------------------------------------------------


//! Updates Chain pending flag
//!
void PropagatePendingVisitor::VisitChain (Chain& chain)
{
  auto pendingCount = ChildrenPendings(chain);
  chain.SetPendingCount(pendingCount);
}
//
//  End of: PropagatePendingVisitor::VisitChain
//---------------------------------------------------------------------------


//! Visits direct children of a parent node, returning true if at least one is pending
//!
uint32_t PropagatePendingVisitor::ChildrenPendings (const ParentNode& parentNode)
{
  uint32_t pendingCount = 0;
  auto     child    = parentNode.FirstChild();

  while (child)
  {
    child->Accept(*this);
    pendingCount += child->PendingCount();

    child = child->NextSibling();
  }

  return pendingCount;
}
//
//  End of: PropagatePendingVisitor::ChildrenPendings
//---------------------------------------------------------------------------


//! Updates Linker pending flag
//!
void PropagatePendingVisitor::VisitLinker (Linker& linker)
{
  auto pendingCount = ChildrenPendings(linker);
  linker.SetPendingCount(pendingCount);
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
//! Updates BlackBox pending flag
//!
void PropagatePendingVisitor::VisitBlackBox (BlackBox& bbox)
{
  auto isPending = bbox.NextToSut() != bbox.LastToSut();
  if (isPending)
  {
    bbox.SetPending();
  }
}
//
//  End of: PropagatePendingVisitor::VisitBlackBox
//---------------------------------------------------------------------------



//===========================================================================
// End of PropagatePendingVisitor.cpp
//===========================================================================
