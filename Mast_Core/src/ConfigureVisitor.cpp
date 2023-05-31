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
  auto pendingCount = ConfigureChildren(accessInterface);
  accessInterface.SetPendingCount(pendingCount);
}
//
//  End of: ConfigureVisitor::VisitAccessInterface
//---------------------------------------------------------------------------

//! Updates AccessInterfaceTranslator pending flag
//!
void ConfigureVisitor::VisitAccessInterfaceTranslator (AccessInterfaceTranslator& accessInterfaceTranslator)
{
  auto pendingCount = ConfigureChildren(accessInterfaceTranslator);
  accessInterfaceTranslator.SetPendingCount(pendingCount);
}
//
//  End of: ConfigureVisitor::VisitAccessInterfaceTranslator
//---------------------------------------------------------------------------


//! Updates Chain pending flag
//!
void ConfigureVisitor::VisitChain (Chain& chain)
{
  auto pendingCount = ConfigureChildren(chain);
  chain.SetPendingCount(pendingCount);
}
//
//  End of: ConfigureVisitor::VisitChain
//---------------------------------------------------------------------------

//! Updates Streamer pending flag
//!
void ConfigureVisitor::VisitStreamer (Streamer& streamer)
{
  auto pendingCount = ConfigureChildren(streamer);
  streamer.SetPendingCount(pendingCount);
}
//
//  End of: ConfigureVisitor::VisitStreamer
//---------------------------------------------------------------------------


//! Visits direct children of a parent node, returning true if at least one is pending
//!
//! @note For pending state, there are two pass on children because configuring a child
//!       may change pending state of a previous sibling
//!
//! @return true when at least one child is pending
//!
uint32_t ConfigureVisitor::ConfigureChildren (const ParentNode& parentNode)
{
  auto child = parentNode.FirstChild();
  while (child)
  {
    child->Accept(*this);
    child = child->NextSibling();
  }

  // ---------------- Tally pendings after configuration of all children
  //                 (a child may be changed while a sibling is configured)
  //
  uint32_t pendingCount = 0;
  child = parentNode.FirstChild();
  while (child)
  {
    pendingCount += child->PendingCount();
    child         = child->NextSibling();
  }

  return pendingCount;
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
  auto     child          = linker.FirstChild();

  while (child)
  {
    child->Accept(*this);
    if (child->IsPending())
    {
      linker.IncrementPendings(child->PendingCount());

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

  // ---------------- Count pendings again (because child may changes while configuring latter sibling)
  //
  linker.ResetPending();
  child = linker.FirstChild();
  while (child)
  {
    linker.IncrementPendings(child->PendingCount());

    child = child->NextSibling();
  }

  // ---------------- Take last decision about what to select
  //
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
  reg.SetPendingForWrite(reg.NextToSut() != reg.LastToSut());
}
//
//  End of: ConfigureVisitor::VisitRegister
//---------------------------------------------------------------------------



//===========================================================================
// End of ConfigureVisitor.cpp
//===========================================================================
