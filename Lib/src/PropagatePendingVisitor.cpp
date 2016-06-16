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
  if (HasChildPending(accessInterface))
  {
    accessInterface.SetPending();
  }
}
//
//  End of: PropagatePendingVisitor::VisitAccessInterface
//---------------------------------------------------------------------------



//! Updates Chain pending flag
//!
void PropagatePendingVisitor::VisitChain (Chain& chain)
{
  if (HasChildPending(chain))
  {
    chain.SetPending();
  }
}
//
//  End of: PropagatePendingVisitor::VisitChain
//---------------------------------------------------------------------------


//! Visits direct children of a parent node, returning true if at least one is pending
//!
bool PropagatePendingVisitor::HasChildPending (const ParentNode& parentNode)
{
  auto isPending = false;
  auto child     = parentNode.FirstChild();

  while (child)
  {
    child->Accept(*this);
    isPending |= child->IsPending();

    child = child->NextSibling();
  }
  return isPending;
}
//
//  End of: PropagatePendingVisitor::HasChildPending
//---------------------------------------------------------------------------


//! Updates Linker pending flag
//!
void PropagatePendingVisitor::VisitLinker (Linker& linker)
{
  if (HasChildPending(linker))
  {
    linker.SetPending();
  }
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
