//===========================================================================
//                           DataCycleVisitor.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DataCycleVisitor.cpp
//!
//! Implements class DataCycleVisitor
//!
//===========================================================================

#include "DataCycleVisitor.hpp"
#include "SystemModelNodes.hpp"
#include "SystemModelManager_impl.hpp"

using namespace mast;

using std::shared_ptr;
using std::make_shared;


//! Updates AccessInterface pending flag
//!

void DataCycleVisitor::VisitAccessInterface (AccessInterface& accessInterface)
{
  VisitChildren(accessInterface);

  if (accessInterface.IsPending())
  {
    m_manager->DoHierarchicalDataCycle(&accessInterface, nullptr);
  }
}
//
//  End of: DataCycleVisitor::VisitAccessInterface
//---------------------------------------------------------------------------



//! Updates Chain pending flag
//!
void DataCycleVisitor::VisitChain (Chain& chain)
{
  VisitChildren(chain);
}
//
//  End of: DataCycleVisitor::VisitChain
//---------------------------------------------------------------------------


//! Ignores Register
//!
void DataCycleVisitor::VisitRegister (Register&)
{
}
//
//  End of: DataCycleVisitor::VisitRegister
//---------------------------------------------------------------------------


//! Updates Linker pending flag while selecting a pending register
//!
//! @note Selected register is the last sibling that is pending
//! @note More elaborate register selection may be possible but actual needs must be
//!       defined to provide an appropriate solution
//! @note When no child is pending, path 0 is selected
//!
void DataCycleVisitor::VisitLinker (Linker& linker)
{
  VisitChildren(linker);
}
//
//  End of: DataCycleVisitor::VisitLinker
//---------------------------------------------------------------------------





//===========================================================================
// End of DataCycleVisitor.cpp
//===========================================================================
