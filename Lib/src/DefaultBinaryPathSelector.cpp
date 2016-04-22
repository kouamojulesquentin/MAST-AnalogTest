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
#include <stdexcept>
#include <limits>

using namespace mast;

//! Initializes selector for fast selection/deselection of a path
//!
//! @param pathCount    Number of managed paths (it must be compatible with the type defined by TablesElementType)
//! @param isInverted   When true the bits for selecting a path are inverted (relative to the path identifier number)
//! @param noIdle       When true zero is used to select first path, otherwise 1 is used instead (provided it is not inverted)
//!
DefaultBinaryPathSelector::DefaultBinaryPathSelector (SystemModelNode* associatedNode, uint32_t pathCount, bool isInverted, bool noIdle)
  : m_select      (CreateSelectTable   (pathCount, isInverted, noIdle))
  , m_deselect    (CreateDeselectTable (pathCount, isInverted))
  , m_muxRegister (dynamic_cast<Register*>(associatedNode))
{
  if (!m_muxRegister)
  {
    throw std::invalid_argument("associatedNode must be a Register");
  }
}
//
//  End of: DefaultBinaryPathSelector::DefaultBinaryPathSelector
//---------------------------------------------------------------------------
//


//! Checks that path identifier is compatible with currently managed scan paths
//!
//! @param pathIdentifier
//!
void DefaultBinaryPathSelector::CheckPathIdentifier (uint32_t pathIdentifier) const
{
  if (pathIdentifier == 0)
  {
    throw std::out_of_range("pathIdentifier must be >= 1");
  }

  if (pathIdentifier >= m_select.size())
  {
    throw std::out_of_range("pathIdentifier is too large");
  }
}
//
//  End of: DefaultBinaryPathSelector::CheckPathIdentifier
//---------------------------------------------------------------------------


//! Creates a table for selection of a path
//!
//! @note A slot in select LUT is reserved for no path selection
//!
//! @param pathCount
//! @param isInverted
//! @param noIdle
//!
DefaultBinaryPathSelector::TablesType DefaultBinaryPathSelector::CreateSelectTable (uint32_t pathCount, bool isInverted, bool noIdle)
{
  TablesType table(pathCount + 1);

  for (uint32_t pathId = 1 ; pathId <= pathCount ; ++pathId)
  {
    table[pathId] = noIdle ? pathId - 1 : pathId;
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



  return false;
}
//
//  End of: DefaultBinaryPathSelector::IsActive
//---------------------------------------------------------------------------



//! Request deactivation of the specified path
//!
void DefaultBinaryPathSelector::Deselect (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);
}
//
//  End of: DefaultBinaryPathSelector::Deselect
//---------------------------------------------------------------------------



//! Request activation of the specified path
//!
void DefaultBinaryPathSelector::Select (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);
}
//
//  End of: DefaultBinaryPathSelector::Select
//---------------------------------------------------------------------------

//===========================================================================
// End of DefaultBinaryPathSelector.cpp
//===========================================================================
