//===========================================================================
//                           AST_ScanRegister.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ScanRegister.cpp
//!
//! Implements class AST_ScanRegister
//!
//===========================================================================

#include "AST_ScanRegister.hpp"
#include "AST_Visitor.hpp"

using namespace Parsers;

//! Visited part of the Visitor pattern
//!
void AST_ScanRegister::Accept (AST_Visitor& visitor)
{
  visitor.Visit_ScanRegister(this);
}



//===========================================================================
// End of AST_ScanRegister.cpp
//===========================================================================
