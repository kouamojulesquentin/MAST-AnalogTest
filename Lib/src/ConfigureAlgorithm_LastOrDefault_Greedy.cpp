//===========================================================================
//                           ConfigureAlgorithm_LastOrDefault_Greedy.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ConfigureAlgorithm_LastOrDefault_Greedy.cpp
//!
//! Implements class ConfigureAlgorithm_LastOrDefault_Greedy
//!
//===========================================================================

#include "ConfigureAlgorithm_LastOrDefault_Greedy.hpp"
#include "Linker.hpp"

using namespace mast;
using std::shared_ptr;



//! Does what is necessary for a pending linker child
//!
//! @param linker         The linker currently configured
//! @param pathIdentifier Pending child path identifier
//! @param child          Pending child
//!
void ConfigureAlgorithm_LastOrDefault_Greedy::ProcessPending (Linker& linker, uint32_t pathIdentifier, shared_ptr<SystemModelNode> /* child */)
{
  if (!linker.IsActive(pathIdentifier))
  {
    linker.Select(pathIdentifier);
  }
}
//
//  End of: ConfigureAlgorithm_LastOrDefault_Greedy::ProcessPending
//---------------------------------------------------------------------------



//! Selects path 0 when linker is not pending, otherwise if there is just one
//! pending, select path 0 if currently selected derivation is already active
//!
//! @note Must be called after linker pending count has been updated//! @note Post-condition: a selection choice must have been done (by this method
//!       or by one call of ProcessPending for current linker)
//!
//! @param linker The linker currently configured
//!
void ConfigureAlgorithm_LastOrDefault_Greedy::ResolvePendings (Linker& linker)
{
  if (!linker.IsPending())
  {
    linker.Select(0u);
  }
  else if (linker.PendingsCount() == 1)
  {
    auto pathCount = linker.SelectablePaths();

    for (uint32_t pathId = 1 ; pathId <= pathCount ; ++pathId)
    {
      if (linker.IsSelectedAndActive(pathId))
      {
        linker.Select(0u);
        break;
      }
    }
  }
}
//
//  End of: ConfigureAlgorithm_LastOrDefault_Greedy::ResolvePendings
//---------------------------------------------------------------------------

//===========================================================================
// End of ConfigureAlgorithm_LastOrDefault_Greedy.cpp
//===========================================================================
