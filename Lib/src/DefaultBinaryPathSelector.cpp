//===========================================================================
//                           DefaultBinaryPathSelector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DefaultBinaryPathSelector.cpp
//!
//! Implements class DefaultBinaryPathSelector
//!
//===========================================================================

#include "DefaultBinaryPathSelector.hpp"
#include "Register.hpp"
#include "Utility.hpp"
#include <stdexcept>
#include <limits>

using std::shared_ptr;

using namespace mast;

//! Initializes selector for fast selection/deselection of a path
//!
//! @param associatedNode   Register that is used to drive the path multiplexer
//! @param pathCount        Number of managed paths
//! @param isInverted       When true the bits for selecting a path are inverted (relative to the path identifier number)
//! @param canSelectNone    When true zero is reserved to select 'no path' otherwise 0 is used to select first path
//!                         (provided it is not inverted)
//!
DefaultBinaryPathSelector::DefaultBinaryPathSelector(shared_ptr<SystemModelNode> associatedNode, uint32_t pathCount, bool isInverted, bool canSelectNone)
  : m_select        (CreateSelectTable   (pathCount, isInverted, canSelectNone))
  , m_deselect      (CreateDeselectTable (pathCount, isInverted))
  , m_muxRegister   (std::dynamic_pointer_cast<Register>(associatedNode))
  , m_canSelectNone (canSelectNone)
{
  if (!m_muxRegister)
  {
    THROW_INVALID_ARGUMENT("associatedNode must be a Register");
  }
}
//
//  End of: DefaultBinaryPathSelector::DefaultBinaryPathSelector
//---------------------------------------------------------------------------


//! Forwards call to any embedded Register
//!
//! @note Visitor should keep track that it is visiting something within a PathSelector
//!
void DefaultBinaryPathSelector::Accept (SystemModelVisitor& visitor)
{
  m_muxRegister->Accept(visitor);
}
//
//  End of: DefaultBinaryPathSelector::Accept
//---------------------------------------------------------------------------



//! Returns the number of paths that are currently active
//!
uint32_t DefaultBinaryPathSelector::ActiveCount () const
{
  uint32_t activeCount = 0u;

  for (uint32_t pathId = 1u ; pathId < m_select.size() ; ++pathId)
  {
    if (IsActive(pathId))
    {
      ++activeCount;
    }
  }
  return activeCount;
}
//
//  End of: DefaultBinaryPathSelector::ActiveCount
//---------------------------------------------------------------------------



//! Checks that path identifier is compatible with currently managed scan paths
//!
//! @param pathIdentifier
//!
void DefaultBinaryPathSelector::CheckPathIdentifier (uint32_t pathIdentifier) const
{
  if (pathIdentifier == 0u)
  {
    THROW_OUT_OF_RANGE("pathIdentifier must be >= 1");
  }

  if (pathIdentifier >= m_select.size())
  {
    THROW_OUT_OF_RANGE("pathIdentifier is too large");
  }
}
//
//  End of: DefaultBinaryPathSelector::CheckPathIdentifier
//---------------------------------------------------------------------------


//! Creates a table for selection of a path
//!
//! @note A slot in select LUT is reserved for no path selection
//!
//! @param pathCount        Number of managed paths
//! @param isInverted       When true the bits for selecting a path are inverted (relative to the path identifier number)
//! @param canSelectNone    When true zero is reserved to select 'no path' otherwise 0 is used to select first path
//!                         (provided it is not inverted)
//!
DefaultBinaryPathSelector::TablesType DefaultBinaryPathSelector::CreateSelectTable (uint32_t pathCount, bool isInverted, bool canSelectNone)
{
  TablesType table(pathCount + 1);

  for (uint32_t pathId = 1 ; pathId <= pathCount ; ++pathId)
  {
    table[pathId].Append(canSelectNone ? pathId : pathId - 1);
  }

  if (isInverted)
  {
    InvertTable(table);
  }

  return std::move(table);
}
//
//  End of: DefaultBinaryPathSelector::CreateSelectTable
//---------------------------------------------------------------------------



//! Creates a table for deselection of a path
//!
//! @note A slot in select LUT is reserved for any paths deselection
DefaultBinaryPathSelector::TablesType DefaultBinaryPathSelector::CreateDeselectTable (uint32_t pathCount, bool isInverted)
{
  TablesType table(pathCount + 1);

  if (isInverted)
  {
    InvertTable(table);
  }

  return std::move(table);
}
//
//  End of: DefaultBinaryPathSelector::CreateDeselectTable
//---------------------------------------------------------------------------


//! Inverts all bits of a LUT
//!
void DefaultBinaryPathSelector::InvertTable (TablesType& table)
{
  for (auto& elem : table)
  {
    elem = ~elem;
  }
}
//
//  End of: DefaultBinaryPathSelector::InvertTable
//---------------------------------------------------------------------------



//! Returns true when the specified path is already selected
//!
bool DefaultBinaryPathSelector::IsActive (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  bool isActive = m_muxRegister->GetLastToSut() == m_select[pathIdentifier];

  return isActive;
}
//
//  End of: DefaultBinaryPathSelector::IsActive
//---------------------------------------------------------------------------



//! Requests deactivation of the specified path
//!
//! @note Also report that a selection is pending and this is now the default value for the mux register
void DefaultBinaryPathSelector::Deselect (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);

  m_muxRegister->SetToSut(m_deselect[pathIdentifier]);
  m_muxRegister->SetBypass(m_deselect[pathIdentifier]);
  m_muxRegister->ForcePending();
}
//
//  End of: DefaultBinaryPathSelector::Deselect
//---------------------------------------------------------------------------



//! Requests activation of the specified path
//!
//! @note Also report that a selection is pending and this is now the default value for the mux register
void DefaultBinaryPathSelector::Select (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);

  m_muxRegister->SetToSut(m_select[pathIdentifier]);
  m_muxRegister->SetBypass(m_select[pathIdentifier]);
  m_muxRegister->ForcePending();
}
//
//  End of: DefaultBinaryPathSelector::Select
//---------------------------------------------------------------------------

//===========================================================================
// End of DefaultBinaryPathSelector.cpp
//===========================================================================
