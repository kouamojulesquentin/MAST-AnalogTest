//===========================================================================
//                           ConfigureVisitor.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ConfigureVisitor.cpp
//!
//! Implements class ConfigureVisitor
//!
//===========================================================================

#include "ConfigureVisitor.hpp"
#include "SystemModelNodes.hpp"

using namespace mast;



//! Updates AccessInterface pending flag
//!
void ConfigureVisitor::VisitAccessInterface (AccessInterface& accessInterface)
{
  accessInterface.ResetPending();
  if (IsChildrenPending(accessInterface))
  {
    accessInterface.SetPending();
  }
}
//
//  End of: ConfigureVisitor::VisitAccessInterface
//---------------------------------------------------------------------------



//! Updates Chain pending flag
//!
void ConfigureVisitor::VisitChain (Chain& chain)
{
  chain.ResetPending();
  if (IsChildrenPending(chain))
  {
    chain.SetPending();
  }
}
//
//  End of: ConfigureVisitor::VisitChain
//---------------------------------------------------------------------------


//! Visits direct children of a parent node, returning true if at least one is pending
//!
bool ConfigureVisitor::IsChildrenPending (const ParentNode& parentNode)
{
  auto isPending = false;
  auto child = parentNode.FirstChild();
  while (child)
  {
    child->Accept(*this);
    isPending |= child->IsPending();

    child = child->NextSibling();
  }
  return isPending;
}
//
//  End of: ConfigureVisitor::IsChildrenPending
//---------------------------------------------------------------------------


//! Updates Linker pending flag while selecting a pending register
//!
//! @note Selected register is the last sibling that is pending
//! @note More elaborate register selection may be possible but actual needs may be
//!       defined to provide an appropriate solution
//!
void ConfigureVisitor::VisitLinker (Linker& linker)
{
  linker.ResetPending();

  uint32_t pathIdentifier = 1u;
  auto     isPending      = false;
  auto     child          = linker.FirstChild();

  while (child)
  {
    child->Accept(*this);
    if (child->IsPending())
    {
      isPending = true;

      linker.Select(pathIdentifier);
    }

    child = child->NextSibling();
    ++pathIdentifier;
  }

  if (isPending)
  {
    linker.SetPending();
  }
}
//
//  End of: ConfigureVisitor::VisitLinker
//---------------------------------------------------------------------------



//! Updates Register pending flag
//!
void ConfigureVisitor::VisitRegister (Register& reg)
{
  reg.ResetPending();
  auto isPending = reg.NextToSut() != reg.LastToSut();
  if (isPending)
  {
    reg.SetPending();
  }
}
//
//  End of: ConfigureVisitor::VisitRegister
//---------------------------------------------------------------------------



//===========================================================================
// End of ConfigureVisitor.cpp
//===========================================================================
