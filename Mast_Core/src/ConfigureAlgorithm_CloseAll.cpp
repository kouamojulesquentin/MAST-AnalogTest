//===========================================================================
//                           ConfigureAlgorithm_CloseAll.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ConfigureAlgorithm_CloseAll.cpp
//!
//! Implements class ConfigureAlgorithm_CloseAll
//!
//===========================================================================

#include "ConfigureAlgorithm_CloseAll.hpp"
#include "Linker.hpp"

using namespace mast;
using std::shared_ptr;

//! Selects specified child if pending, while closing the path that was active before
//!
//! @param linker         The linker currently configured
//! @param pathIdentifier Pending child path identifier
//! @param child          Pending child
//!
void ConfigureAlgorithm_CloseAll::ProcessPending (Linker& linker, uint32_t pathIdentifier, shared_ptr<SystemModelNode> /* child */)
{
  if (linker.IsActive(pathIdentifier))
   {
    //If it is active, it will be serviced during the current DataCycle
    if (!linker.KeepOpen())
      linker.Deselect(pathIdentifier);
   }
 else
  {
    linker.Select(pathIdentifier);
  }
}
//
//  End of: ConfigureAlgorithm_CloseAll::ProcessPending
//---------------------------------------------------------------------------


//===========================================================================
// End of ConfigureAlgorithm_CloseAll.cpp
//===========================================================================
