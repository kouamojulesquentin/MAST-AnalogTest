//===========================================================================
//                           Mast_Core_Traits.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Mast_Core_Traits.hpp
//!
//! Declares CxxTest traits for class ErrorCode
//!
//===========================================================================

#ifndef MAST_CORE_TRAITS_H__D5832FFA_3D66_46E6_D6B0_DD0AFDDB868D__INCLUDED_
  #define MAST_CORE_TRAITS_H__D5832FFA_3D66_46E6_D6B0_DD0AFDDB868D__INCLUDED_

#include "PathSelector.hpp"
#include "SubModelPlaceHolder.hpp"

#include <cxxtest/ValueTraits.h>
#include <string>
#include <sstream>
#include <iomanip>


//! Enables CxxTest to display enums using their names (instead of their integer values)
//!
CXXTEST_ENUM_TRAITS(mast::SelectorProperty,
                    CXXTEST_ENUM_MEMBER(mast::SelectorProperty::None)
                    CXXTEST_ENUM_MEMBER(mast::SelectorProperty::CanSelectNone)
                    CXXTEST_ENUM_MEMBER(mast::SelectorProperty::InvertedBits)
                    CXXTEST_ENUM_MEMBER(mast::SelectorProperty::ReverseOrder)
                    CXXTEST_ENUM_MEMBER(mast::SelectorProperty::Brocade_Default)
                   );

//! Enables CxxTest to display enums using their names (instead of their integer values)
//!
CXXTEST_ENUM_TRAITS(mast::PlaceHolderKind,
                    CXXTEST_ENUM_MEMBER(mast::PlaceHolderKind::SIT)
                    CXXTEST_ENUM_MEMBER(mast::PlaceHolderKind::Factory)
                   );
namespace CxxTest
{
} // End of namespace CxxTest


#endif  // not defined MAST_CORE_TRAITS_H__D5832FFA_3D66_46E6_D6B0_DD0AFDDB868D__INCLUDED_
//===========================================================================
// End of Mast_Core_Traits.hpp
//===========================================================================
