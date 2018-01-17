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
//! @param associatedRegister   Register that is used to drive the path multiplexer (must be not nullptr)
//! @param pathsCount           Number of managed paths (including, optional, bypass register)
//! @param properties           Properties of the selector (bit order can be reverse or it can use negative logic)
//!
DefaultBinaryPathSelector::DefaultBinaryPathSelector(shared_ptr<Register> associatedRegister, uint32_t pathsCount, SelectorProperty properties)
  : DefaultTableBasedPathSelector (CHECK_PARAMETER_NOT_NULL(associatedRegister, REQUIRED_REGISTER_MESSAGE),
                                   pathsCount,
                                   CreateSelectTable   (CHECK_PARAMETER_NOT_NULL(associatedRegister, REQUIRED_REGISTER_MESSAGE)->BitsCount(), pathsCount, properties),
                                   CreateDeselectTable (CHECK_PARAMETER_NOT_NULL(associatedRegister, REQUIRED_REGISTER_MESSAGE)->BitsCount(), pathsCount, properties),
                                   properties
                                  )
{
}
//
//  End of: DefaultBinaryPathSelector::DefaultBinaryPathSelector
//---------------------------------------------------------------------------


//! Initializes selector for fast selection/deselection of a path
//!
//! @param associatedRegisters  Register(s) ranges that is used to drive the path multiplexer
//! @param pathsCount           Number of managed paths (including, optional, bypass register)
//! @param properties           Properties of the selector (bit order can be reverse or it can use negative logic)
//!
DefaultBinaryPathSelector::DefaultBinaryPathSelector(const VirtualRegister& associatedRegisters, uint32_t pathsCount, SelectorProperty properties)
  : DefaultTableBasedPathSelector (associatedRegisters,
                                   pathsCount,
                                   CreateSelectTable   (associatedRegisters.BitsCount(), pathsCount, properties),
                                   CreateDeselectTable (associatedRegisters.BitsCount(), pathsCount, properties),
                                   properties
                                  )
{
}
//
//  End of: DefaultBinaryPathSelector::DefaultBinaryPathSelector
//---------------------------------------------------------------------------


//! Returns the associated register expected initial value for a path count and selector properties
//!
//! @param pathsCount   Number of managed paths (including, optional, bypass register)
//! @param properties   Properties of the selector (bit order can be reverse or it can use negative logic)
//!
BinaryVector DefaultBinaryPathSelector::AssociatedRegisterInitialValue (uint32_t pathsCount, SelectorProperty properties)
{
  auto canSelectNone = IsSet(properties, SelectorProperty::CanSelectNone);
  auto regSize       = DefaultBinaryPathSelector::RegWidthForPathCount(pathsCount, canSelectNone);
  auto value         = BinaryVector(regSize, 0, SizeProperty::Fixed);

  return value;
}
//
//  End of: DefaultBinaryPathSelector::AssociatedRegisterInitialValue
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
DefaultBinaryPathSelector::TablesType DefaultBinaryPathSelector::CreateSelectTable (uint32_t registerLength, uint32_t pathsCount, SelectorProperty properties)
{
  auto isInverted    = IsSet(properties, SelectorProperty::InvertedBits);
  auto canSelectNone = IsSet(properties, SelectorProperty::CanSelectNone);

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
DefaultBinaryPathSelector::TablesType DefaultBinaryPathSelector::CreateDeselectTable (uint32_t registerLength, uint32_t pathsCount, SelectorProperty properties)
{
  auto isInverted    = IsSet(properties, SelectorProperty::InvertedBits);
  auto canSelectNone = IsSet(properties, SelectorProperty::CanSelectNone);

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
