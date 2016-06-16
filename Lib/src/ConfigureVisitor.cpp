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
  if (ConfigureChildren(accessInterface))
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
  if (ConfigureChildren(chain))
  {
    chain.SetPending();
  }
}
//
//  End of: ConfigureVisitor::VisitChain
//---------------------------------------------------------------------------


//! Visits direct children of a parent node, returning true if at least one is pending
//!
//! @note For pending state, there are two pass on children because configuring a child
//!       may change pending state of a previous sibling
//!
//! @return true when at least one child is pending
//!
bool ConfigureVisitor::ConfigureChildren (const ParentNode& parentNode)
{
  auto isPending = false;
  auto child     = parentNode.FirstChild();

  while (child)
  {
    child->Accept(*this);
    isPending |= child->IsPending();

    child = child->NextSibling();
  }

  if (!isPending)
  {
    // ---------------- Check again pending state
    //                 (a child may be changed while a sibling is configured)
    //
    child = parentNode.FirstChild();
    while (child)
    {
      isPending = child->IsPending();
      if (isPending)
      {
        break;
      }

      child = child->NextSibling();
    }
  }

  return isPending;
}
//
//  End of: ConfigureVisitor::ConfigureChildren
//---------------------------------------------------------------------------


//! Updates Linker pending flag while selecting a pending register
//!
//! @note Selected register is the last sibling that is pending
//! @note More elaborate register selection may be possible but actual needs must be
//!       defined to provide an appropriate solution
//! @note When no child is pending, path 0 is selected
//!
void ConfigureVisitor::VisitLinker (Linker& linker)
{
  linker.ResetPending();
  if (m_configurationAlgorithm)
  {
    m_configurationAlgorithm->StartLinkerProcessing(linker);
  }

  uint32_t pathIdentifier = 1u;
  auto     isPending      = false;
  auto     child          = linker.FirstChild();

  while (child)
  {
    child->Accept(*this);
    if (child->IsPending())
    {
      isPending = true;

      if (m_configurationAlgorithm)
      {
        m_configurationAlgorithm->ProcessPending(linker, pathIdentifier, child);
      }
      else
      {
        linker.Select(pathIdentifier);
      }
    }

    child = child->NextSibling();
    ++pathIdentifier;
  }

  if (isPending)
  {
    linker.SetPending();
  }

  if (m_configurationAlgorithm)
  {
    m_configurationAlgorithm->ResolvePendings(linker);
  }
  else if (!linker.IsPending())
  {
    linker.Select(0u);  // Select reserved "path"
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
