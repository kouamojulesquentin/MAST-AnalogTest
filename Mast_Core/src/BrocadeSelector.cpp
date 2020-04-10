//===========================================================================
//                           BrocadeSelector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file BrocadeSelector.cpp
//!
//! Implements class BrocadeSelector
//!
//===========================================================================

#include "BrocadeSelector.hpp"
#include "DefaultNHotPathSelector.hpp"
#include "Register.hpp"
#include "Utility.hpp"
#include "g3log/g3log.hpp"

#include <stdexcept>
#include <limits>
#include <sstream>

using std::ostringstream;
using std::shared_ptr;

using namespace mast;

const uint32_t BrocadeSelector::REGISTER_BITS_COUNT = 8u; //!< Brocade control register width
const uint32_t BrocadeSelector::MAX_TAPS_COUNT      = 5u; //!< Max number of TAPs managed by Brocade circuit


//! Initializes selector for selection/deselection of a path
//!
//! @param associatedRegister   Register that is used to drive the path IRs and DRs multiplexers
//! @param properties           Properties of the selector (bit order can be reverse)
//!
BrocadeSelector::BrocadeSelector(shared_ptr<Register> associatedRegister, uint32_t slaveTapsCount, SelectorProperty properties)
  : DefaultTableBasedPathSelector (associatedRegister,
                                   slaveTapsCount,
                                   CreateSelectTable   (slaveTapsCount, properties),
                                   CreateDeselectTable (slaveTapsCount, properties),
                                   properties
                                  )
{
  CHECK_PARAMETER_NOT_NULL(AssociatedRegisters(), "Brocade selector cannot be constructed without valid associated Register(s)");

  CHECK_PARAMETER_EQ(AssociatedRegisters()->BitsCount(), REGISTER_BITS_COUNT,
                     "Register to control Brocade circuit is expected to have exactly " + std::to_string(REGISTER_BITS_COUNT) + " bits");
}
//
//  End of: BrocadeSelector::BrocadeSelector
//---------------------------------------------------------------------------



//! Creates a table for selection of a path
//!
//! @note A slot in select LUT is reserved for no path selection
//!
//! @param pathsCount  Number of effective paths to handle (to size the table)
//! @param properties  Specifies whether bits are reversed. By default first TAP is controled by
//!                    associated Register least significant bit
//!
BrocadeSelector::TablesType BrocadeSelector::CreateSelectTable (uint32_t pathsCount, SelectorProperty properties)
{
  CHECK_PARAMETER_RANGE(pathsCount, 1u, MAX_TAPS_COUNT,
                        "Brocade can only support from 1 to " + std::to_string(MAX_TAPS_COUNT) + " TAP(s)");

  return std::move(DefaultNHotPathSelector::CreateSelectTable(REGISTER_BITS_COUNT, pathsCount, properties));
}
//
//  End of: BrocadeSelector::CreateSelectTable
//---------------------------------------------------------------------------



//! Creates a table for deselection of a path
//!
//! @note A slot in select LUT is reserved for any paths deselection
//!
//! @param pathsCount  Number of effective paths to handle (to size the table)
//! @param properties  Specifies whether bits are reversed. By default first TAP is controled by
//!                    associated Register least significant bit
//!
BrocadeSelector::TablesType BrocadeSelector::CreateDeselectTable (uint32_t pathsCount, SelectorProperty properties)
{
  CHECK_PARAMETER_RANGE(pathsCount, 1u, MAX_TAPS_COUNT,
                        "Brocade can only support from 1 to " + std::to_string(MAX_TAPS_COUNT) + " TAP(s)");

  auto table = DefaultNHotPathSelector::CreateSelectTable(REGISTER_BITS_COUNT, pathsCount, properties);

  InvertTable(table); // Deselection table must always have bit reversed relative to selection table

  return std::move(table);
}
//
//  End of: BrocadeSelector::CreateDeselectTable
//---------------------------------------------------------------------------


//! Requests deactivation of the specified path
//!
//! @note Also report that a selection is pending and this is now the default value for the mux register
void BrocadeSelector::Deselect (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);

  const auto& deselectMask = m_deselectTable[pathIdentifier];
  auto        muxRegisters = AssociatedRegisters();
  auto        toSut        = muxRegisters->NextToSut();

  toSut &= deselectMask;

   LOG(DEBUG) << "Setting Pending : from "<< " to " <<muxRegisters->NextToSut().DataAsHexString();
  if (muxRegisters->NextToSut() != toSut)
  {
    muxRegisters->SetToSut(toSut);
    muxRegisters->SetPending();
  }
}
//
//  End of: BrocadeSelector::Deselect
//---------------------------------------------------------------------------



//! Returns true when the specified path is already selected
//!
bool BrocadeSelector::IsActive (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  auto        muxRegisters = AssociatedRegisters();
  const auto& lastToSut    = muxRegisters->LastToSut();
  const auto& selectMask   = m_selectTable[pathIdentifier];

  bool  isActive = (lastToSut & selectMask) == selectMask;

  return isActive;
}
//
//  End of: BrocadeSelector::IsActive
//---------------------------------------------------------------------------


//! Returns true when the specified path is already selected
//!
bool BrocadeSelector::IsSelected (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  auto        muxRegisters = AssociatedRegisters();
  const auto& nextToSut    = muxRegisters->NextToSut();
  const auto& selectMask   = m_selectTable[pathIdentifier];

  bool  isSelected = (nextToSut & selectMask) == selectMask;

  return isSelected;
}
//
//  End of: BrocadeSelector::IsSelected
//---------------------------------------------------------------------------


//! Returns true when the specified path is selected and active
//!
bool BrocadeSelector::IsSelectedAndActive (uint32_t pathIdentifier) const
{
  CheckPathIdentifier(pathIdentifier);

  auto& selectMask   = m_selectTable[pathIdentifier];
  auto  muxRegisters = AssociatedRegisters();
  auto  nextToSut    = muxRegisters->NextToSut();
  auto  lastToSut    = muxRegisters->LastToSut();

  nextToSut &= selectMask;
  lastToSut &= selectMask;

  bool  isSelected = nextToSut == selectMask;
  bool  isActive   = lastToSut == selectMask;

  return isSelected && isActive;
}
//
//  End of: BrocadeSelector::IsSelectedAndActive
//---------------------------------------------------------------------------




//! Requests activation of the specified path
//!
//! @note Also report that a selection is pending and this is now the default value for the mux register
void BrocadeSelector::Select (uint32_t pathIdentifier)
{
  CheckPathIdentifier(pathIdentifier);

  const auto& selectMask   = m_selectTable[pathIdentifier];
  auto        muxRegisters = AssociatedRegisters();
  auto        toSut        = muxRegisters->NextToSut();

  if (pathIdentifier == 0)
  {
    toSut = selectMask;
  }
  else
  {
    toSut |= selectMask;
  }

   LOG(DEBUG) << "Setting Pending : from "<< " to " <<muxRegisters->NextToSut().DataAsHexString();
  if (muxRegisters->NextToSut() != toSut)
  {
    muxRegisters->SetToSut(toSut);
    if (toSut!= muxRegisters->LastToSut())
      muxRegisters->SetPending();
  }
}
//
//  End of: BrocadeSelector::Select
//---------------------------------------------------------------------------


//===========================================================================
// End of BrocadeSelector.cpp
//===========================================================================
