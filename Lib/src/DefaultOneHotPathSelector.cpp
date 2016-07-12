//===========================================================================
//                           DefaultOneHotPathSelector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DefaultOneHotPathSelector.cpp
//!
//! Implements class DefaultOneHotPathSelector
//!
//===========================================================================

#include "DefaultOneHotPathSelector.hpp"
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
DefaultOneHotPathSelector::DefaultOneHotPathSelector(shared_ptr<Register> associatedRegister, uint32_t pathsCount, SelectorProperty properties)
  : DefaultTableBasedPathSelector (associatedRegister,
                                   pathsCount,
                                   CreateSelectTable   (associatedRegister->BitsCount(), pathsCount, properties),
                                   CreateDeselectTable (associatedRegister->BitsCount(), pathsCount, properties),
                                   true
                                  )
{
}
//
//  End of: DefaultOneHotPathSelector::DefaultOneHotPathSelector
//---------------------------------------------------------------------------



//! Checks that register length is enough to select all paths count
//!
void DefaultOneHotPathSelector::CheckRegisterLength (uint32_t registerLength, uint32_t pathsCount)
{
  if (registerLength < pathsCount)
  {
    ostringstream os;
    os << "Register associated with DefaultOneHotPathSelector has '" << registerLength;
    os << "' bits even though a minimum of '"                        << pathsCount << "' bits are necessary";
    THROW_LOGIC_ERROR(os.str());
  }
}
//
//  End of: DefaultOneHotPathSelector::CheckRegisterLength
//---------------------------------------------------------------------------


//! Creates a table for selection of a path
//!
//! @note A slot in select LUT is reserved for no path selection
//!
//! @param pathsCount   Number of managed paths
//! @param isInverted   When true the bits for selecting a path are inverted (relative to the path identifier number)
//!
DefaultOneHotPathSelector::TablesType DefaultOneHotPathSelector::CreateSelectTable (uint32_t         registerLength,
                                                                                    uint32_t         pathsCount,
                                                                                    SelectorProperty properties)
{
  CheckRegisterLength(registerLength, pathsCount);

  TablesType table;

  table.emplace_back(registerLength, 0, SizeProperty::FixedOnCopy); // Dummy entry for no selection and for path identifier starting from 1

  bool     reverseOrder = IsSet(properties, SelectorProperty::ReverseOrder);
  uint32_t selectionBit = reverseOrder ? 0 : registerLength - 1u;

  BinaryVector temp(registerLength);
  for (uint32_t pathId = 1u ; pathId <= pathsCount ; ++pathId)
  {
    temp.SetBit(selectionBit);
    table.emplace_back(temp, SizeProperty::FixedOnCopy);
    temp.ClearBit(selectionBit);

    selectionBit = reverseOrder ? selectionBit + 1u
                                : selectionBit - 1u;
  }

  if (IsSet(properties, SelectorProperty::InvertedBits))
  {
    InvertTable(table);
  }

  return std::move(table);
}
//
//  End of: DefaultOneHotPathSelector::CreateSelectTable
//---------------------------------------------------------------------------



//! Creates a table for deselection of a path
//!
//! @note A slot in select LUT is reserved for any paths deselection
//!
DefaultOneHotPathSelector::TablesType DefaultOneHotPathSelector::CreateDeselectTable (uint32_t         registerLength,
                                                                                      uint32_t         pathsCount,
                                                                                      SelectorProperty properties)
{
  CheckRegisterLength(registerLength, pathsCount);

  TablesType table(pathsCount + 1, BinaryVector(registerLength, 0, SizeProperty::FixedOnCopy));

  if (IsSet(properties, SelectorProperty::InvertedBits))
  {
    InvertTable(table);
  }

  return std::move(table);
}
//
//  End of: DefaultOneHotPathSelector::CreateDeselectTable
//---------------------------------------------------------------------------


//===========================================================================
// End of DefaultOneHotPathSelector.cpp
//===========================================================================
