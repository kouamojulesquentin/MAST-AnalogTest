//===========================================================================
//                           RegistersAlias.cpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file RegistersAlias.cpp
//!
//! Implements class RegistersAlias
//!
//===========================================================================

#include "RegistersAlias.hpp"
#include "Utility.hpp"

using std::string;
using namespace mast;



//! Constructs a RegistersAlias
//!
//! @param baseName         Alias name (without any indices) - must be non empty
//! @param virtualRegister  Associated virtual register - must be not empty
//!
RegistersAlias::RegistersAlias (string&& baseName, VirtualRegister&& virtualRegister)
  : m_baseName        (std::move(baseName))
  , m_virtualRegister (std::move(virtualRegister))
{
  CHECK_PARAMETER_NOT_EMPTY(m_baseName,            "Register alias must have a valid (non empty) name");
  CHECK_PARAMETER_FALSE(m_virtualRegister.Empty(), "Register alias must have a valid (non empty) virtual register");
}
//
//  End of: RegistersAlias::RegistersAlias
//---------------------------------------------------------------------------




//===========================================================================
// End of RegistersAlias.cpp
//===========================================================================
