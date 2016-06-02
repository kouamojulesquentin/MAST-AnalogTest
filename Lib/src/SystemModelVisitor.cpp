//===========================================================================
//                           SystemModelVisitor.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelVisitor.cpp
//!
//! Implements class SystemModelVisitor
//!
//===========================================================================

#include "ParentNode.hpp"
#include "Linker.hpp"
#include "SystemModelVisitor.hpp"

using namespace mast;

//! Visits direct active children of a parent node
//!
//! @note Does nothing special on the nodes
//!
void SystemModelVisitor::VisitActiveRegisters (const Linker& linker)
{
  uint32_t pathSelector = 1u;

  auto child = linker.FirstChild();
  while (child)
  {
    if (linker.IsActive(pathSelector))
    {
      child->Accept(*this);
    }
    child = child->NextSibling();
    ++pathSelector;
  }
}
//
//  End of: SystemModelVisitor::VisitActiveRegisters
//---------------------------------------------------------------------------

//! Visits direct pending children of a parent node
//!
//! @note Does nothing special on the nodes
//!
void SystemModelVisitor::VisitPendingChildren (const ParentNode& parentNode)
{
  auto child = parentNode.FirstChild();
  while (child)
  {
    if (child->IsPending())
    {
      child->Accept(*this);
    }

    child = child->NextSibling();
  }
}
//
//  End of: SystemModelVisitor::VisitPendingChildren
//---------------------------------------------------------------------------


//! Visits direct children of a parent node
//!
//! @note Does nothing special on the nodes
//!
void SystemModelVisitor::VisitChildren (const ParentNode& parentNode)
{
  auto child = parentNode.FirstChild();
  while (child)
  {
    child->Accept(*this);

    child = child->NextSibling();
  }
}
//
//  End of: SystemModelVisitor::VisitChildren
//---------------------------------------------------------------------------


//===========================================================================
// End of SystemModelVisitor.cpp
//===========================================================================
