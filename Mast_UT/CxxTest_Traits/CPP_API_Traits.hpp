//===========================================================================
//                           CPP_API_Traits.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file CPP_API_Traits.hpp
//!
//! Declares CxxTest traits for class ErrorCode
//!
//===========================================================================


#ifndef CPP_API_TRAITS_H__C14B8301_816F_46EA_4884_EA7EB740A836__INCLUDED_
  #define CPP_API_TRAITS_H__C14B8301_816F_46EA_4884_EA7EB740A836__INCLUDED_

#include <cxxtest/ValueTraits.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "SystemModelAdapter_CPP.hpp"


//! Enables CxxTest to display enums using their names (instead of their integer values)
//!
CXXTEST_ENUM_TRAITS(mast::StringType,
                    CXXTEST_ENUM_MEMBER(mast::StringType::Binary)
                    CXXTEST_ENUM_MEMBER(mast::StringType::Hex)
                    CXXTEST_ENUM_MEMBER(mast::StringType::Decimal)
                   );

namespace CxxTest
{
} // End of namespace CxxTest


#endif  // not defined CPP_API_TRAITS_H__C14B8301_816F_46EA_4884_EA7EB740A836__INCLUDED_
//===========================================================================
// End of CPP_API_Traits.hpp
//===========================================================================
