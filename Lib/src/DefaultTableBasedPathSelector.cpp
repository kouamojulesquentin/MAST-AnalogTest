//===========================================================================
//                           DefaultTableBasedPathSelector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DefaultTableBasedPathSelector.cpp
//!
//! Implements class DefaultTableBasedPathSelector
//!
//===========================================================================

#include "DefaultTableBasedPathSelector.hpp"
#include "Register.hpp"
#include "Utility.hpp"

#include <stdexcept>
#include <limits>
#include <sstream>

using std::ostringstream;
using std::shared_ptr;

using namespace mast;

//! Initializes selector for fast selection/deselection of a path
//!
//! @param associatedRegister   Register that is used to drive the path multiplexer
//! @param pathsCount           Number of managed paths (including, optional, bypass register)
//! @param selectTable          Table to use for selecting a path
//! @param deselectTable        Table to use for deselecting a path
//! @param isInverted           When true the bits for selecting a path are inverted (relative to the path identifier number)
//! @param canSelectNone        When true zero is reserved to select 'no path' otherwise 0 is used to select first path
//!                             (provided it is not inverted)
//!
DefaultTableBasedPathSelector::DefaultTableBasedPathSelector (shared_ptr<Register> associatedRegister,
                                                              uint32_t             pathsCount,
                                                              TablesType           selectTable,
                                                              TablesType           deselectTable,
                                                              SelectorProperty     properties)
  : PathSelector    (properties)
  , m_pathsCount    (pathsCount)
  , m_muxRegister   (CHECK_PARAMETER_NOT_NULL (associatedRegister, "associatedRegister must be a valid Register"))
  , m_selectTable   (FixTable(selectTable))
  , m_deselectTable (FixTable(deselectTable))
{
  if (m_selectTable.size() != m_deselectTable.size())
  {
    ostringstream os;
    os << "(de)Selection tables size differ, got " << m_selectTable.size();
    os << " for select and "                       << m_deselectTable.size() << " for deselect";
    THROW_INVALID_ARGUMENT(os.str());
  }

  if (m_deselectTable.size() <= m_pathsCount)
  {
    ostringstream os;
    os << "Selection and deselection table must have an entry for not used path identifier zero";
    THROW_INVALID_ARGUMENT(os.str());
  }
}
//
//  End of: DefaultTableBasedPathSelector::DefaultTableBasedPathSelector
//---------------------------------------------------------------------------


//! Forwards call to any embedded Register
//!
//! @note Visitor should keep track that it is visiting something within a PathSelector
//!
void DefaultTableBasedPathSelector::Accept (SystemModelVisitor& visitor)
{
  m_muxRegister->Accept(visitor);
}
//
//  End of: DefaultTableBasedPathSelector::Accept
//---------------------------------------------------------------------------



//! Returns the number of paths that are currently active
//!
uint32_t DefaultTableBasedPathSelector::ActiveCount () const
{
  uint32_t activeCount = 0u;

  for (uint32_t pathId = 1u ; pathId < m_selectTable.size() ; ++pathId)
  {
    if (IsActive(pathId))
    {
      ++activeCount;
    }
  }
  return activeCount;
}
//
//  End of: DefaultTableBasedPathSelector::ActiveCount
//---------------------------------------------------------------------------



//! Checks that path identifier is compatible with currently managed scan paths
//!
//! @param pathIdentifier
//!
void DefaultTableBasedPathSelector::CheckPathIdentifier (uint32_t pathIdentifier) const
{
  if (pathIdentifier >= m_selectTable.size())
  {
    ostringstream os; os << "pathIdentifier must be < " << m_selectTable.size() << ", got: " << pathIdentifier;
    THROW_OUT_OF_RANGE(os.str());
  }
}
//
//  End of: DefaultTableBasedPathSelector::CheckPathIdentifier
//---------------------------------------------------------------------------


//! Inverts all bits of a LUT
//!
DefaultTableBasedPathSelector::TablesType& DefaultTableBasedPathSelector::InvertTable (TablesType& table)
{
  for (auto& elem : table)
  {
    elem = ~elem;
  }
  return table;
}
//
//  End of: DefaultTableBasedPathSelector::InvertTable
//---------------------------------------------------------------------------


//! Makes table entry fix (cannot modify size)
//!
DefaultTableBasedPathSelector::TablesType& DefaultTableBasedPathSelector::FixTable (TablesType& table)
{
  for (auto& elem : table)
  {
    elem.FixSize(true);
  }
  return table;
}
//
//  End of: DefaultTableBasedPathSelector::FixTable
//---------------------------------------------------------------------------


//! Returns true when the specified path is already selected
//!
bool DefaultTableBasedPathSelector::IsActive (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  auto& lastToSut   = m_muxRegister->LastToSut();
  auto& selectValue = m_selectTable[pathIdentifier];

  bool  isActive    = lastToSut == selectValue;

  return isActive;
}
//
//  End of: DefaultTableBasedPathSelector::IsActive
//---------------------------------------------------------------------------


//! Returns true when the specified path is already selected
//!
bool DefaultTableBasedPathSelector::IsSelected (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  auto& nextToSut   = m_muxRegister->NextToSut();
  auto& selectValue = m_selectTable[pathIdentifier];

  bool  isSelected  = nextToSut == selectValue;

  return isSelected;
}
//
//  End of: DefaultTableBasedPathSelector::IsSelected
//---------------------------------------------------------------------------


//! Returns true when the specified path is selected and active
//!
bool DefaultTableBasedPathSelector::IsSelectedAndActive (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  auto& lastToSut   = m_muxRegister->LastToSut();
  auto& nextToSut   = m_muxRegister->NextToSut();
  auto& selectValue = m_selectTable[pathIdentifier];

  bool  isSelected  = nextToSut == selectValue;
  bool  isActive    = lastToSut == selectValue;

  return isSelected && isActive;
}
//
//  End of: DefaultTableBasedPathSelector::IsSelectedAndActive
//---------------------------------------------------------------------------

//! Requests deactivation of the specified path
//!
//! @note Also report that a selection is pending and this is now the default value for the mux register
void DefaultTableBasedPathSelector::Deselect (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);

  const auto& selectValue = m_deselectTable[pathIdentifier];
  if (m_muxRegister->NextToSut() != selectValue)
  {
    m_muxRegister->SetToSut(selectValue);
    m_muxRegister->SetPending();
  }
}
//
//  End of: DefaultTableBasedPathSelector::Deselect
//---------------------------------------------------------------------------



//! Requests activation of the specified path
//!
//! @note Also report that a selection is pending and this is now the default value for the mux register
void DefaultTableBasedPathSelector::Select (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);

  const auto& selectValue = m_selectTable[pathIdentifier];
  if (m_muxRegister->NextToSut() != selectValue)
  {
    m_muxRegister->SetToSut(selectValue);
    m_muxRegister->SetPending();
  }
}
//
//  End of: DefaultTableBasedPathSelector::Select
//---------------------------------------------------------------------------



//! Returns some value associated with specified path selection
//!
//! @note When cannot provide a value, an empty BinaryVector is returned
//!
//! @param pathIdentifier   Path identifier in range [1..nb_path]
//!
const BinaryVector& DefaultTableBasedPathSelector::SelectionValue (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  const auto& selectValue = m_selectTable[pathIdentifier];
  return selectValue;
}
//
//  End of: DefaultTableBasedPathSelector::SelectionValue
//---------------------------------------------------------------------------



//===========================================================================
// End of DefaultTableBasedPathSelector.cpp
//===========================================================================
