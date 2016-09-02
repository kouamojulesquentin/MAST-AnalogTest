//===========================================================================
//                           ConfigureAlgorithm_LastOrDefault.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ConfigureAlgorithm_LastOrDefault.cpp
//!
//! Implements class ConfigureAlgorithm_LastOrDefault
//!
//===========================================================================

#include "ConfigureAlgorithm_LastOrDefault.hpp"
#include "Linker.hpp"

using namespace mast;
using std::shared_ptr;



//! Does what is necessary for a pending linker child
//!
//! @param linker         The linker currently configured
//! @param pathIdentifier Pending child path identifier
//! @param child          Pending child
//!
void ConfigureAlgorithm_LastOrDefault::ProcessPending (Linker& linker, uint32_t pathIdentifier, shared_ptr<SystemModelNode> /* child */)
{
  if (!linker.IsActive(pathIdentifier))
  {
    linker.Select(pathIdentifier);
  }
}
//
//  End of: ConfigureAlgorithm_LastOrDefault::ProcessPending
//---------------------------------------------------------------------------



//! Selects path 0 when linker is not pending, otherwise does nothing
//!
//! @note Must be called after linker pending state has been updated (this permit stateless algorithm)
//! @note Post-condition: a selection choice must have done (by this method or by one call of ProcessPending for current linker)
//!
//! @param linker The linker currently configured
//!
void ConfigureAlgorithm_LastOrDefault::ResolvePendings (Linker& linker)
{
  if (!linker.IsPending())
  {
    linker.Select(0u);
  }
}
//
//  End of: ConfigureAlgorithm_LastOrDefault::ResolvePendings
//---------------------------------------------------------------------------

//===========================================================================
// End of ConfigureAlgorithm_LastOrDefault.cpp
//===========================================================================
