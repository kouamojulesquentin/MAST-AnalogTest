//===========================================================================
//                           C_API_Traits.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file C_API_Traits.hpp
//!
//! Declares CxxTest traits for class ErrorCode
//!
//===========================================================================


#ifndef C_API_TRAITS_H__778B891C_A392_4956_198B_7D87B3DFE740__INCLUDED_
  #define C_API_TRAITS_H__778B891C_A392_4956_198B_7D87B3DFE740__INCLUDED_

#include <cxxtest/ValueTraits.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "SystemModelAdapter_C.h"

//! Enables CxxTest to display enums using their names (instead of their integer values)
//!
CXXTEST_ENUM_TRAITS(ErrorCode_t,
                    CXXTEST_ENUM_MEMBER(Ok)
                    CXXTEST_ENUM_MEMBER(InvalidModel)
                    CXXTEST_ENUM_MEMBER(InvalidArgument)
                    CXXTEST_ENUM_MEMBER(LogicError)
                    CXXTEST_ENUM_MEMBER(OutOfRange)
                    CXXTEST_ENUM_MEMBER(RuntimeError)
                    CXXTEST_ENUM_MEMBER(RegexException)
                    CXXTEST_ENUM_MEMBER(StdException)
                    CXXTEST_ENUM_MEMBER(UndefinedFailure)
                    CXXTEST_ENUM_MEMBER(Failure)
                   );

namespace CxxTest
{
} // End of namespace CxxTest


#endif  // not defined C_API_TRAITS_H__778B891C_A392_4956_198B_7D87B3DFE740__INCLUDED_
//===========================================================================
// End of C_API_Traits.hpp
//===========================================================================
