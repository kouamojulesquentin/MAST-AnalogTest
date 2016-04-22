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

#include <stdexcept>

using namespace mast;


//! Initialized parent and ...
Linker::Linker (std::string name, PathSelector* pathSelector)
  : ParentNode     (name)
  , m_pathSelector (pathSelector)
{
  if (!pathSelector)
  {
    throw std::invalid_argument("A valid pathSelector is mandatory");
  }
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


//! Checks that path identifier is compatible with currently managed scan paths
//!
//! @param pathIdentifier   Path identifier in range [1..nb_path]
//!
void Linker::CheckPathIdentifier (uint32_t pathIdentifier) const
{
  if (pathIdentifier > m_pathsCount)
  {
    throw std::out_of_range("pathIdentifier is too large");
  }
}
//
//  End of: Linker::CheckPathIdentifier
//---------------------------------------------------------------------------



//! Returns true when the specified path is already selected
//!
//! @param pathIdentifier   Path identifier in range [1..nb_path]
bool Linker::IsActive (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);
  return m_pathSelector->IsActive(pathIdentifier);
}

//! Requests deactivation of the specified path
//!
//! @param pathIdentifier   Path identifier in range [1..nb_path]
void Linker::Deselect (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);
  m_pathSelector->Deselect(pathIdentifier);
}

//! Requests activation of the specified path
//!
//! @param pathIdentifier   Path identifier in range [1..nb_path]
void Linker::Select   (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);
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
