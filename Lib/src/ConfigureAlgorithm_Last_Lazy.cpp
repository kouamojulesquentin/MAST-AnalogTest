//===========================================================================
//                           ConfigureAlgorithm_Last_Lazy.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ConfigureAlgorithm_Last_Lazy.cpp
//!
//! Implements class ConfigureAlgorithm_Last_Lazy
//!
//===========================================================================

#include "ConfigureAlgorithm_Last_Lazy.hpp"
#include "Linker.hpp"

using namespace mast;
using std::shared_ptr;

//! Selects specified child if pending (and not active yet)
//!
//! @param linker         The linker currently configured
//! @param pathIdentifier Pending child path identifier
//! @param child          Pending child
//!
void ConfigureAlgorithm_Last_Lazy::ProcessPending (Linker& linker, uint32_t pathIdentifier, shared_ptr<SystemModelNode> /* child */)
{
  if (!linker.IsActive(pathIdentifier))
  {
    linker.Select(pathIdentifier);
  }
}
//
//  End of: ConfigureAlgorithm_Last_Lazy::ProcessPending
//---------------------------------------------------------------------------


//===========================================================================
// End of ConfigureAlgorithm_Last_Lazy.cpp
//===========================================================================
