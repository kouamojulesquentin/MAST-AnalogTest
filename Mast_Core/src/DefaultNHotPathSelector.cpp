//===========================================================================
//                           DefaultNHotPathSelector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DefaultNHotPathSelector.cpp
//!
//! Implements class DefaultNHotPathSelector
//!
//===========================================================================

#include "DefaultNHotPathSelector.hpp"
#include "DefaultOneHotPathSelector.hpp"
#include "Register.hpp"
#include "Utility.hpp"
#include "EnumsUtility.hpp"

#include <stdexcept>
#include <limits>
#include <sstream>
#include <experimental/string_view>

using std::ostringstream;
using std::shared_ptr;
using std::experimental::string_view;

using namespace std::experimental::literals::string_view_literals;

using namespace mast;

namespace
{
  auto REQUIRED_REGISTER_MESSAGE = "A valid associated register is mandatory to create DefaultTableBasedPathSelector"sv;
}

//! Initializes selector for fast selection/deselection of a path
//!
//! @param associatedRegister   Register that is used to drive the path multiplexer
//! @param pathsCount       Number of managed paths (including, optional, bypass register)
//! @param properties       Properties of the selector (bit order can be reverse or it can use negative logic)
//!
DefaultNHotPathSelector::DefaultNHotPathSelector(shared_ptr<Register> associatedRegister, uint32_t pathsCount, SelectorProperty properties)
  : DefaultTableBasedPathSelector (CHECK_PARAMETER_NOT_NULL(associatedRegister, REQUIRED_REGISTER_MESSAGE),
                                   pathsCount,
                                   CreateSelectTable   (CHECK_PARAMETER_NOT_NULL(associatedRegister, REQUIRED_REGISTER_MESSAGE)->BitsCount(), pathsCount, properties),
                                   CreateDeselectTable (CHECK_PARAMETER_NOT_NULL(associatedRegister, REQUIRED_REGISTER_MESSAGE)->BitsCount(), pathsCount, properties),
                                   properties
                                  )
  , m_invertedBits (IsSet(properties, SelectorProperty::InvertedBits))
{
}
//
//  End of: DefaultNHotPathSelector::DefaultNHotPathSelector
//---------------------------------------------------------------------------


//! Returns the associated register expected initial value for a path count and selector properties
//!
//! @param pathsCount   Number of managed paths (including, optional, bypass register)
//! @param properties   Properties of the selector (bit order can be reverse or it can use negative logic)
//!
BinaryVector DefaultNHotPathSelector::AssociatedRegisterInitialValue (uint32_t pathsCount, SelectorProperty properties)
{
  return DefaultOneHotPathSelector::AssociatedRegisterInitialValue(pathsCount, properties);
}
//
//  End of: DefaultNHotPathSelector::AssociatedRegisterInitialValue
//---------------------------------------------------------------------------



//! Checks that register length is enough to select all paths count
//!
void DefaultNHotPathSelector::CheckRegisterLength (uint32_t registerLength, uint32_t pathsCount)
{
  if (registerLength < pathsCount)
  {
    ostringstream os;
    os << "Register associated with DefaultNHotPathSelector has '" << registerLength;
    os << "' bits even though a minimum of '"                      << pathsCount << "' bits are necessary";
    THROW_LOGIC_ERROR(os.str());
  }
}
//
//  End of: DefaultNHotPathSelector::CheckRegisterLength
//---------------------------------------------------------------------------


//! Creates a table for selection of a path
//!
//! @note A slot in select LUT is reserved for no path selection
//!
//! @param pathsCount   Number of managed paths
//! @param isInverted   When true the bits for selecting a path are inverted (relative to the path identifier number)
//!
DefaultNHotPathSelector::TablesType DefaultNHotPathSelector::CreateSelectTable (uint32_t         registerLength,
                                                                                uint32_t         pathsCount,
                                                                                SelectorProperty properties)
{
  return std::move(DefaultOneHotPathSelector::CreateSelectTable(registerLength, pathsCount, properties));
}
//
//  End of: DefaultNHotPathSelector::CreateSelectTable
//---------------------------------------------------------------------------



//! Creates a table for deselection of a path
//!
//! @note A slot in select LUT is reserved for any paths deselection
//!
DefaultNHotPathSelector::TablesType DefaultNHotPathSelector::CreateDeselectTable (uint32_t         registerLength,
                                                                                  uint32_t         pathsCount,
                                                                                  SelectorProperty properties)
{
  auto table = CreateSelectTable(registerLength, pathsCount, properties);

  InvertTable(table); // Deselection table must always have bit reversed relative to selection table

  return std::move(table);
}
//
//  End of: DefaultNHotPathSelector::CreateDeselectTable
//---------------------------------------------------------------------------


//! Requests deactivation of the specified path
//!
//! @note Also report that a selection is pending and this is now the default value for the mux register
void DefaultNHotPathSelector::Deselect (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);

  const auto& deselectMask = m_deselectTable[pathIdentifier];
  auto        muxRegisters = AssociatedRegisters();
  auto        toSut        = muxRegisters->NextToSut();

  if (m_invertedBits) // Active low?
  {
    toSut |= deselectMask;
  }
  else
  {
    toSut &= deselectMask;
  }

  if (!CanSelectNone() && (toSut == m_selectTable[0]))
      toSut = m_selectTable[1]; //As selector cannot be closed, select first path

  if (!CanSelectNone() && (toSut == m_selectTable[0]))
  {
    //Proactive check, should never come here
    THROW_LOGIC_ERROR("Try to select no path, even though selector is configured to at least select one");
  }

  if (muxRegisters->NextToSut() != toSut)
  {
    muxRegisters->SetToSut(toSut);
    muxRegisters->SetPending();
  }
}
//
//  End of: DefaultNHotPathSelector::Deselect
//---------------------------------------------------------------------------



//! Returns true when the specified path is already selected
//!
bool DefaultNHotPathSelector::IsActive (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  auto        muxRegisters = AssociatedRegisters();
  const auto& lastToSut    = muxRegisters->LastToSut();
  const auto& selectMask   = m_selectTable[pathIdentifier];

  bool  isActive = m_invertedBits ? (lastToSut | selectMask) == selectMask
                                  : (lastToSut & selectMask) == selectMask;

  return isActive;
}
//
//  End of: DefaultNHotPathSelector::IsActive
//---------------------------------------------------------------------------


//! Returns true when the specified path is already selected
//!
bool DefaultNHotPathSelector::IsSelected (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  auto        muxRegisters = AssociatedRegisters();
  auto        nextToSut    = muxRegisters->NextToSut();
  const auto& selectMask   = m_selectTable[pathIdentifier];

  if (m_invertedBits)
  {
    nextToSut |= selectMask;
  }
  else
  {
    nextToSut &= selectMask;
  }

  bool  isSelected = nextToSut == selectMask;

  return isSelected;
}
//
//  End of: DefaultNHotPathSelector::IsSelected
//---------------------------------------------------------------------------


//! Returns true when the specified path is selected and active
//!
bool DefaultNHotPathSelector::IsSelectedAndActive (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  auto& selectMask   = m_selectTable[pathIdentifier];
  auto  muxRegisters = AssociatedRegisters();
  auto  lastToSut    = muxRegisters->LastToSut();
  auto  nextToSut    = muxRegisters->NextToSut();

  if (m_invertedBits)
  {
    nextToSut |= selectMask;
    lastToSut |= selectMask;
  }
  else
  {
    nextToSut &= selectMask;
    lastToSut &= selectMask;
  }

  bool  isSelected = nextToSut == selectMask;
  bool  isActive   = lastToSut == selectMask;

  return isSelected && isActive;
}
//
//  End of: DefaultNHotPathSelector::IsSelectedAndActive
//---------------------------------------------------------------------------




//! Requests activation of the specified path
//!
//! @note Also report that a selection is pending and this is now the default value for the mux register
void DefaultNHotPathSelector::Select (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);

  const auto& selectMask   = m_selectTable[pathIdentifier];
  auto        muxRegisters = AssociatedRegisters();
  auto        toSut        = muxRegisters->NextToSut();

  if (pathIdentifier == 0)
  {
    toSut = selectMask;
  }
  else if (m_invertedBits) // Active low?
  {
    toSut &= selectMask;
  }
  else
  {
    toSut |= selectMask;
  }

  if (muxRegisters->NextToSut() != toSut)
  {
    muxRegisters->SetToSut(toSut);
    muxRegisters->SetPending();
  }
}
//
//  End of: DefaultNHotPathSelector::Select
//---------------------------------------------------------------------------


//===========================================================================
// End of DefaultNHotPathSelector.cpp
//===========================================================================
