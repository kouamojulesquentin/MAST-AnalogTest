//===========================================================================
//                           Mast_Core_Enums_Traits.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Mast_Core_Enums_Traits.hpp
//!
//! Declares cxxtest traits for Mast_Core enums. It enables CxxTest to display
//! enums using their names (instead of their integer values)
//!
//===========================================================================


#ifndef MAST_CORE_ENUMS_TRAITS_H__3CD2543D_9C95_4053_B3A6_F5E77FB23A3A__INCLUDED_
  #define MAST_CORE_ENUMS_TRAITS_H__3CD2543D_9C95_4053_B3A6_F5E77FB23A3A__INCLUDED_

#include "PathSelector.hpp"
#include "SubModelPlaceHolder.hpp"
#include "BitsOrdering.hpp"

#include <cxxtest/ValueTraits.h>

CXXTEST_ENUM_TRAITS(mast::SelectorProperty,
                    CXXTEST_ENUM_MEMBER(mast::SelectorProperty::None)
                    CXXTEST_ENUM_MEMBER(mast::SelectorProperty::CanSelectNone)
                    CXXTEST_ENUM_MEMBER(mast::SelectorProperty::InvertedBits)
                    CXXTEST_ENUM_MEMBER(mast::SelectorProperty::ReverseOrder)
                    CXXTEST_ENUM_MEMBER(mast::SelectorProperty::Brocade_Default)
                   );

CXXTEST_ENUM_TRAITS(mast::PlaceHolderKind,
                    CXXTEST_ENUM_MEMBER(mast::PlaceHolderKind::SIT)
                    CXXTEST_ENUM_MEMBER(mast::PlaceHolderKind::Factory)
                   );


CXXTEST_ENUM_TRAITS(mast::BitsOrdering,
                   CXXTEST_ENUM_MEMBER(mast::BitsOrdering::Undefined)
                   CXXTEST_ENUM_MEMBER(mast::BitsOrdering::Downto)
                   CXXTEST_ENUM_MEMBER(mast::BitsOrdering::Upto)
                  );


#endif  // not defined MAST_CORE_ENUMS_TRAITS_H__3CD2543D_9C95_4053_B3A6_F5E77FB23A3A__INCLUDED_
//===========================================================================
// End of Mast_Core_Enums_Traits.hpp
//===========================================================================



