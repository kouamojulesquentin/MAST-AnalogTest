//===========================================================================
//                           ResetiRunLoopVisitor.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ResetiRunLoopVisitor.cpp
//!
//! Implements class ResetiRunLoopVisitor
//!
//===========================================================================

#include "ResetiRunLoopVisitor.hpp"
#include "SystemModelNodes.hpp"
#include "AccessInterfaceTranslator.hpp"

using namespace mast;



//! Stops recursing down the model hierarchy
//!
//! @note ResetiRunLoopVisitor must stop at AccessInterface boundary in order to manage hierarchical processing of the model
//!       By CONSEQUENCE it MUST NOT initially be started on an AccessInterface BUT successively on its channels (children)
//!
void ResetiRunLoopVisitor::VisitAccessInterface (AccessInterface&  accessInterface )
{
 accessInterface.SetiRunLoop(0); 
}
//
//  End of: ResetiRunLoopVisitor::VisitAccessInterface
//---------------------------------------------------------------------------


//! Visits AccessInterfaceTranslator pending children
//!
void ResetiRunLoopVisitor::VisitAccessInterfaceTranslator (AccessInterfaceTranslator& accessInterfaceTranslator)
{
  VisitChildren(accessInterfaceTranslator);
  accessInterfaceTranslator.SetiRunLoop(0); 
}
//
//  End of: ResetiRunLoopVisitor::VisitAccessInterfaceTranslator
//---------------------------------------------------------------------------


//! Visits Chain pending children
//!
void ResetiRunLoopVisitor::VisitChain (Chain& chain)
{
  VisitChildren(chain);
  chain.SetiRunLoop(0); 
}
//
//  End of: ResetiRunLoopVisitor::VisitChain
//---------------------------------------------------------------------------

//! Visits Streamer pending children
//!
void ResetiRunLoopVisitor::VisitStreamer (Streamer& streamer)
{
  
  VisitChildren(streamer);
  streamer.SetiRunLoop(0); 
}
//
//  End of: ResetiRunLoopVisitor::VisitStreamer
//---------------------------------------------------------------------------



//! Visits Linker pending children
//!
void ResetiRunLoopVisitor::VisitLinker (Linker& linker)
{
  VisitActiveRegisters(linker);
  linker.SetiRunLoop(0); 
}
//
//  End of: ResetiRunLoopVisitor::VisitLinker
//---------------------------------------------------------------------------



//! Resets iRunLoop to 0
//!
//! @note __Should be only used (indirectly called) for active (currently selected) Registers__
//!
void ResetiRunLoopVisitor::VisitRegister (Register& reg)
{
  reg.SetiRunLoop(0); 
}
//
//  End of: ResetiRunLoopVisitor::VisitRegister
//---------------------------------------------------------------------------


//===========================================================================
// End of ResetiRunLoopVisitor.cpp
//===========================================================================
