//===========================================================================
//                           Linker.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Linker.cpp
//!
//! Implements class Linker
//!
//===========================================================================

#include "Linker.hpp"
#include "SystemModelVisitor.hpp"
#include "PathSelector.hpp"

#include <cassert>

using namespace mast;


//! Initialized parent and ...
Linker::Linker (std::string name, PathSelector* pathSelector)
  : ParentNode     (name)
  , m_pathSelector (pathSelector)
{
  assert(pathSelector && "A valid pathSelector is mandatory");
}
//
//  End of: Linker::Linker
//---------------------------------------------------------------------------


//! Visited part of the Visitor pattern
//!
void Linker::Accept (SystemModelVisitor& visitor)
{
  visitor.VisitLinker(*this);
}

//! Returns true when the specified path is already selected
//!
bool Linker::IsActive (uint32_t pathIdentifier) const
{
  return m_pathSelector->IsActive(pathIdentifier);
}

//! Requests desactivation of the specified path
//!
void Linker::Deselect (uint32_t pathIdentifier)
{
  m_pathSelector->Deselect(pathIdentifier);
}

//! Requests activation of the specified path
//!
void Linker::Select   (uint32_t pathIdentifier)
{
  m_pathSelector->Select(pathIdentifier);
}



//! Returns pointer on byte-stream for last sequence shifted from sut
//!
const uint8_t* Linker::GetLastSequence () const
{
  return nullptr;
}
//
//  End of: Linker::GetLastSequence
//---------------------------------------------------------------------------


//! Returns pointer on byte-stream for next sequence to shift into sut
//!
const uint8_t* Linker::GetNextSequence () const
{
  return nullptr;
}
//
//  End of: Linker::GetNextSequence
//---------------------------------------------------------------------------



//===========================================================================
// End of Linker.cpp
//===========================================================================
