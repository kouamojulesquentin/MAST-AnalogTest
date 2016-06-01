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
#include "BinaryVector.hpp"
#include "Utility.hpp"

#include <stdexcept>
using std::shared_ptr;
using std::experimental::string_view;

using namespace mast;


//! Initializes parent and path selector
//!
//! @param name           Name associated with the Linker node
//! @param pathSelector   PathSelector associated with the Linker
//!
Linker::Linker (string_view name, shared_ptr<PathSelector> pathSelector)
  : ParentNode     (name)
  , m_pathSelector (pathSelector)
{
  if (!pathSelector)
  {
    THROW_INVALID_ARGUMENT("A valid pathSelector is mandatory");
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
  if (pathIdentifier > m_pathSelector->SelectablePaths())
  {
    THROW_OUT_OF_RANGE("pathIdentifier is too large");
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

//! Returns true when the specified path is currently pending to be selected
//!
//! @note path may be selected but not active yet
//!
//! @param pathIdentifier   Path identifier in range [1..nb_path]
bool Linker::IsSelected (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);
  return m_pathSelector->IsSelected(pathIdentifier);
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
//+BinaryVector Linker::GetLastSequence () const
//+{
//+  return BinaryVector();
//+}
//
//  End of: Linker::GetLastSequence
//---------------------------------------------------------------------------


//! Returns pointer on byte-stream for next sequence to shift into sut
//!
//+BinaryVector Linker::GetNextSequence () const
//+{
//+  return BinaryVector();
//+}
//
//  End of: Linker::GetNextSequence
//---------------------------------------------------------------------------



//===========================================================================
// End of Linker.cpp
//===========================================================================
