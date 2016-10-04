//===========================================================================
//                           PathSelector.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PathSelector.cpp
//!
//! Implements class PathSelector
//!
//===========================================================================

#include "PathSelector.hpp"
#include "Utility.hpp"

#include <string>
#include <sstream>

using std::string;
using std::ostringstream;
using namespace mast;


//! Returns debug information about selector
//!
string PathSelector::DebugSelectorInfo (bool /* onlyProperties */) const
{
  ostringstream os;

  os << "Kind: "            << KindName() << std::endl;
  os << "Can_select_none: " << std::boolalpha << IsSet(m_properties, SelectorProperty::CanSelectNone) << std::endl;
  os << "Reversed_order:  " << std::boolalpha << IsSet(m_properties, SelectorProperty::ReverseOrder)  << std::endl;
  os << "Inverted_bits:   " << std::boolalpha << IsSet(m_properties, SelectorProperty::InvertedBits);

  return os.str();
}
//
//  End of: PathSelector::DebugSelectorInfo
//---------------------------------------------------------------------------


//===========================================================================
// End of PathSelector.cpp
//===========================================================================
