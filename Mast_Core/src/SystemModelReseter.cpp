//===========================================================================
//                           SystemModelReseter.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelReseter.cpp
//!
//! Implements class SystemModelReseter
//!
//===========================================================================

#include "SystemModelReseter.hpp"
#include "AccessInterfaceProtocol.hpp"
#include "SystemModelNodes.hpp"

using namespace mast;



//! Resets Linker and all its children nodes then issue a reset sequence at the AccessInterface level
//!
void SystemModelReseter::VisitAccessInterface (AccessInterface& accessInterface)
{
  VisitParent(accessInterface);
  accessInterface.Protocol()->DoReset(m_doSynchronousReset);
}

//! Resets Chain and all its children nodes
//!
void SystemModelReseter::VisitChain (Chain& chain)
{
  VisitParent(chain);
}

//! Resets Linker and all its children nodes
//!
void SystemModelReseter::VisitLinker (Linker& linker)
{
  VisitParent(linker);
}



//! Resets parent node and all its children
//!
void SystemModelReseter::VisitParent (ParentNode& parent)
{
  parent.ResetPending();
  VisitChildren(parent);
}
//
//  End of: SystemModelReseter::VisitParent
//---------------------------------------------------------------------------


//! Collects mismatches of a Register
//!
void SystemModelReseter::VisitRegister (Register& reg)
{
  reg.Reset();
}



//===========================================================================
// End of SystemModelReseter.cpp
//===========================================================================
