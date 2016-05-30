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
#include <sstream>

using std::ostringstream;
using std::shared_ptr;

using namespace mast;

//! Initializes selector for fast selection/deselection of a path
//!
//! @param associatedRegister   Register that is used to drive the path multiplexer
//! @param pathsCount       Number of managed paths (including, optional, bypass register)
//! @param isInverted       When true the bits for selecting a path are inverted (relative to the path identifier number)
//! @param canSelectNone    When true zero is reserved to select 'no path' otherwise 0 is used to select first path
//!                         (provided it is not inverted)
//!
DefaultBinaryPathSelector::DefaultBinaryPathSelector(shared_ptr<Register> associatedRegister, uint32_t pathsCount, bool isInverted, bool canSelectNone)
  : m_pathsCount    (pathsCount)
  , m_muxRegister   (CHECK_NOT_NULL_PARAMETER (associatedRegister, "associatedRegister must be a valid Register"))
  , m_select        (CreateSelectTable   (associatedRegister->BitsCount(), pathsCount, isInverted, canSelectNone))
  , m_deselect      (CreateDeselectTable (associatedRegister->BitsCount(), pathsCount, isInverted, canSelectNone))
  , m_canSelectNone (canSelectNone)
{
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


//! Checks that register length is enough to select all path count
//!
void DefaultBinaryPathSelector::CheckRegisterLength (uint32_t registerLength, uint32_t pathsCount, bool canSelectNone)
{
  //+ (JFC May/27/2016): consider using RegWidthForPathCount that throw when pathsCount is zero !!!
  auto maxSelectorValue = pathsCount;
  if (!canSelectNone && (pathsCount != 0))
  {
    maxSelectorValue = pathsCount - 1;
  }

  auto selectorMinWidth = Utility::MinimalBitsForValue(maxSelectorValue);

  if (registerLength < selectorMinWidth)
  {
    ostringstream os;
    os << "Register associated with DefaultBinaryPathSelector has '" << registerLength;
    os << "' bits even though a minimum of '"                        << selectorMinWidth << "' bits are necessary";
    THROW_LOGIC_ERROR(os.str());
  }
}
//
//  End of: DefaultBinaryPathSelector::CheckRegisterLength
//---------------------------------------------------------------------------


//! Creates a table for selection of a path
//!
//! @note A slot in select LUT is reserved for no path selection
//!
//! @param pathsCount        Number of managed paths
//! @param isInverted       When true the bits for selecting a path are inverted (relative to the path identifier number)
//! @param canSelectNone    When true zero is reserved to select 'no path' otherwise 0 is used to select first path
//!                         (provided it is not inverted)
//!
DefaultBinaryPathSelector::TablesType DefaultBinaryPathSelector::CreateSelectTable (uint32_t registerLength, uint32_t pathsCount, bool isInverted, bool canSelectNone)
{
  CheckRegisterLength(registerLength, pathsCount, canSelectNone);

  TablesType table;

  table.emplace_back(registerLength, 0, SizeProperty::FixedOnCopy); // Dummy entry for no selection and for path identifier starting from 1

  BinaryVector temp;
  for (uint32_t pathId = 1 ; pathId <= pathsCount ; ++pathId)
  {
    uint32_t selectValue = canSelectNone ? pathId : pathId - 1;

    temp.Clear();
    temp.Append(selectValue, registerLength, BitsAlignment::Right);

    table.emplace_back(temp, SizeProperty::FixedOnCopy);
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
DefaultBinaryPathSelector::TablesType DefaultBinaryPathSelector::CreateDeselectTable (uint32_t registerLength, uint32_t pathsCount, bool isInverted, bool canSelectNone)
{
  CheckRegisterLength(registerLength, pathsCount, canSelectNone);

  TablesType table(pathsCount + 1, BinaryVector(registerLength, 0, SizeProperty::FixedOnCopy));

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

  auto& lastToSut   = m_muxRegister->LastToSut();
  auto& selectValue = m_select[pathIdentifier];

  bool  isActive    = lastToSut == selectValue;

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
  m_muxRegister->SetPending();
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
  m_muxRegister->SetPending();
}
//
//  End of: DefaultBinaryPathSelector::Select
//---------------------------------------------------------------------------



//! Returns minimal bits count a register should have to drive a mux for a number of paths
//!
//! @param pathsCount      Number of managed paths (including, optional, bypass register)
//! @param canSelectNone  When true zero is reserved to select 'no path' otherwise 0 is used to select first path
//!                       (provided it is not inverted)
//!
uint32_t DefaultBinaryPathSelector::RegWidthForPathCount (uint32_t pathsCount, bool canSelectNone)
{
  if       (pathsCount == 0)
  {
    THROW_INVALID_ARGUMENT("A path selector with 0 path count is not valid");
  }
  else if ((pathsCount == 1) && !canSelectNone)
  {
    THROW_INVALID_ARGUMENT("A path selector with 1 path count is not valid when cannot select none");
  }

  uint32_t maxValue = canSelectNone ? pathsCount : pathsCount - 1;
  return Utility::MinimalBitsForValue(maxValue);
}
//
//  End of: DefaultBinaryPathSelector::RegWidthForPathCount
//---------------------------------------------------------------------------


//===========================================================================
// End of DefaultBinaryPathSelector.cpp
//===========================================================================
