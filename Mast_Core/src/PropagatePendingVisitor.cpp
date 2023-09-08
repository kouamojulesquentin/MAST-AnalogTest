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
  accessInterface.SetPendingCount(pendingCount.first);
  accessInterface.SetiRunLoop(0); //iRunLoops are treated at the Interface level
}
//
//  End of: PropagatePendingVisitor::VisitAccessInterface
//---------------------------------------------------------------------------

//! Updates AccessInterfaceTranslator pending flag
//!
void PropagatePendingVisitor::VisitAccessInterfaceTranslator (AccessInterfaceTranslator& accessInterfaceTranslator)
{
  auto pendingCount = ChildrenPendings(accessInterfaceTranslator);
  accessInterfaceTranslator.SetPendingCount(pendingCount.first);
  accessInterfaceTranslator.SetiRunLoop(0); //iRunLoops are treated at the Interface level
}
//
//  End of: PropagatePendingVisitor::VisitAccessInterface
//---------------------------------------------------------------------------


//! Updates Chain pending flag
//!
void PropagatePendingVisitor::VisitChain (Chain& chain)
{
  auto pendingCount = ChildrenPendings(chain);
  chain.SetPendingCount(pendingCount.first);
  chain.SetiRunLoop(pendingCount.second);
}
//
//  End of: PropagatePendingVisitor::VisitChain
//---------------------------------------------------------------------------

//! Updates Streamer pending flag
//!
void PropagatePendingVisitor::VisitStreamer (Streamer& streamer)
{
  auto pendingCount = ChildrenPendings(streamer);
  streamer.SetPendingCount(pendingCount.first);
  streamer.SetiRunLoop(pendingCount.second);
}
//
//  End of: PropagatePendingVisitor::VisitChain
//---------------------------------------------------------------------------


//! Visits direct children of a parent node, returning true if at least one is pending
//!
std::pair<uint32_t,uint32_t> PropagatePendingVisitor::ChildrenPendings (const ParentNode& parentNode)
{
  uint32_t pendingCount = 0;
  uint32_t iRunLoopCount = 0;
  auto     child    = parentNode.FirstChild();

  while (child)
  {
    child->Accept(*this);
    pendingCount += child->PendingCount();
    if (child->iRunLoopCount() > iRunLoopCount)
    iRunLoopCount = std::max(iRunLoopCount,child->iRunLoopCount());

    child = child->NextSibling();
  }

 iRunLoopCount = std::max(iRunLoopCount,parentNode.iRunLoopCount());

  return std::make_pair(pendingCount,iRunLoopCount);
}
//
//  End of: PropagatePendingVisitor::ChildrenPendings
//---------------------------------------------------------------------------


//! Updates Linker pending flag
//!
void PropagatePendingVisitor::VisitLinker (Linker& linker)
{
  auto pendingCount = ChildrenPendings(linker);
  linker.SetPendingCount(pendingCount.first);
  linker.SetiRunLoop(pendingCount.second);
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
