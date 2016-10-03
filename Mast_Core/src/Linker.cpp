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
#include "Register.hpp"
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
  CHECK_PARAMETER_NOT_NULL(pathSelector, "A valid pathSelector is mandatory to construct a Linker");

  IgnoreForNodePath(true);    // This is the default for linkers
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


//! Returns true when the specified path is currently selected and active
//!
//! @param pathIdentifier   Path identifier in range [1..nb_path]
//!
bool Linker::IsSelectedAndActive (uint32_t pathIdentifier) const
{
  return m_pathSelector->IsSelectedAndActive(pathIdentifier);
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
//! @internal
//! @note         When pathIdentifier = 0 (unselect all), if the associated Register is currently pending,
//!               the request is ignored
//! @endinternal
//!
//! @param pathIdentifier   Path identifier in range [1..nb_path]
//!
void Linker::Select   (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);

  if (pathIdentifier == 0)
  {
    auto muxRegister = m_pathSelector->AssociatedRegister();
    if (muxRegister->IsPending())
    {
      return;
    }
  }

  m_pathSelector->Select(pathIdentifier);
}

//===========================================================================
// End of Linker.cpp
//===========================================================================
