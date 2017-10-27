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
//! @param pathsCount           Number of managed paths (including, optional, bypass register)
//! @param properties           Properties of the selector (bit order can be reverse or it can use negative logic)
//!
DefaultOneHotPathSelector::DefaultOneHotPathSelector(shared_ptr<Register> associatedRegister, uint32_t pathsCount, SelectorProperty properties)
  : DefaultTableBasedPathSelector (CHECK_PARAMETER_NOT_NULL(associatedRegister, REQUIRED_REGISTER_MESSAGE),
                                   pathsCount,
                                   CreateSelectTable   (CHECK_PARAMETER_NOT_NULL(associatedRegister, REQUIRED_REGISTER_MESSAGE)->BitsCount(), pathsCount, properties),
                                   CreateDeselectTable (CHECK_PARAMETER_NOT_NULL(associatedRegister, REQUIRED_REGISTER_MESSAGE)->BitsCount(), pathsCount, properties),
                                   properties
                                  )
{
}
//
//  End of: DefaultOneHotPathSelector::DefaultOneHotPathSelector
//---------------------------------------------------------------------------


//! Returns the associated register expected initial value for a path count and selector properties
//!
//! @param pathsCount   Number of managed paths (including, optional, bypass register)
//! @param properties   Properties of the selector (bit order can be reverse or it can use negative logic)
//!
BinaryVector DefaultOneHotPathSelector::AssociatedRegisterInitialValue (uint32_t pathsCount, SelectorProperty properties)
{
  auto canSelectNone = IsSet(properties, SelectorProperty::CanSelectNone);
  auto invertedBits  = IsSet(properties, SelectorProperty::InvertedBits);
  auto reverseOrder  = IsSet(properties, SelectorProperty::ReverseOrder);
  auto value         = invertedBits ? BinaryVector(pathsCount, 0xFF, SizeProperty::Fixed)
                                    : BinaryVector(pathsCount, 0x00, SizeProperty::Fixed);
  if (!canSelectNone)
  {
    uint32_t bitOffset = reverseOrder ? pathsCount - 1 : 0u;

    invertedBits ? value.ClearBit(bitOffset)
                 : value.SetBit(bitOffset);
  }
  return value;
}
//
//  End of: DefaultOneHotPathSelector::AssociatedRegisterInitialValue
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
//! @param registerLength Number of bits of associated register
//! @param pathsCount     Number of managed paths (including, optional, bypass register)
//! @param properties     Properties of the selector (bit order can be reverse or it can use negative logic)
//!
DefaultOneHotPathSelector::TablesType DefaultOneHotPathSelector::CreateSelectTable (uint32_t         registerLength,
                                                                                    uint32_t         pathsCount,
                                                                                    SelectorProperty properties)
{
  CheckRegisterLength(registerLength, pathsCount);

  TablesType table;

  table.emplace_back(registerLength, 0, SizeProperty::FixedOnCopy); // Dummy entry for no selection and for path identifier starting from 1

  bool     reverseOrder = IsSet(properties, SelectorProperty::ReverseOrder);
  uint32_t selectionBit = reverseOrder ? registerLength - 1u : 0;

  BinaryVector temp(registerLength);
  for (uint32_t pathId = 1u ; pathId <= pathsCount ; ++pathId)
  {
    temp.SetBit(selectionBit);
    table.emplace_back(temp, SizeProperty::FixedOnCopy);
    temp.ClearBit(selectionBit);

    selectionBit = reverseOrder ? selectionBit - 1u
                                : selectionBit + 1u;
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
//! @param registerLength Number of bits of associated register
//! @param pathsCount     Number of managed paths (including, optional, bypass register)
//! @param properties     Properties of the selector (bit order can be reverse or it can use negative logic)
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



//! Request activation of the specified path
//!
//! @See DefaultTableBasedPathSelector::Select
//!
void DefaultOneHotPathSelector::Select (uint32_t pathIdentifier)
{
  if (!CanSelectNone() && (pathIdentifier == 0))
  {
    THROW_INVALID_ARGUMENT("Try to select no path, even though selector is configured to at least select one");
  }

  DefaultTableBasedPathSelector::Select(pathIdentifier);
}
//
//  End of: DefaultOneHotPathSelector::Select
//---------------------------------------------------------------------------

//===========================================================================
// End of DefaultOneHotPathSelector.cpp
//===========================================================================
