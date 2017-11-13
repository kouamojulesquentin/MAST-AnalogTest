//===========================================================================
//                           AST_ScanMux.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ScanMux.cpp
//!
//! Implements class AST_ScanMux
//!
//===========================================================================

#include "AST_ScanMux.hpp"
#include "AST_Visitor.hpp"

using namespace Parsers;



//! Visited part of the Visitor pattern
//!
void AST_ScanMux::Accept (AST_Visitor& visitor)
{
  visitor.Visit_ScanMux(this);
}
//
//  End of: AST_ScanMux::Accept
//---------------------------------------------------------------------------


//! Does nothing
//!
void AST_ScanMux::DispatchChildren ()
{
}
//
//  End of: AST_ScanMux::DispatchChildren
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_ScanMux.cpp
//===========================================================================
