//===========================================================================
//                           DefaultConfigurationAlgorithm.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DefaultConfigurationAlgorithm.cpp
//!
//! Implements class DefaultConfigurationAlgorithm
//!
//===========================================================================

#include "DefaultConfigurationAlgorithm.hpp"
#include "Linker.hpp"

using namespace mast;
using std::shared_ptr;



//! Does what is necessary for a pending linker child
//!
//! @param linker         The linker currently configured
//! @param pathIdentifier Pending child path identifier
//! @param child          Pending child
//!
void DefaultConfigurationAlgorithm::ProcessPending (Linker& linker, uint32_t pathIdentifier, shared_ptr<SystemModelNode> /* child */)
{
  if (!linker.IsActive(pathIdentifier))
  {
    linker.Select(pathIdentifier);
  }
}
//
//  End of: DefaultConfigurationAlgorithm::ProcessPending
//---------------------------------------------------------------------------



//! Selects path 0 when linker is not pending, otherwise does nothing
//!
//! @note Must be called after linker pending state has been updated (this permit stateless algorithm) 
//! @note Post-condition: a selection choice must have done (by this method or by one call of ProcessPending for current linker)
//!
//! @param linker The linker currently configured
//!
void DefaultConfigurationAlgorithm::ResolvePendings (Linker& linker)
{
  if (!linker.IsPending())
  {
    linker.Select(0u);
  }
}
//
//  End of: DefaultConfigurationAlgorithm::ResolvePendings
//---------------------------------------------------------------------------

//===========================================================================
// End of DefaultConfigurationAlgorithm.cpp
//===========================================================================
