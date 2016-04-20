//===========================================================================
//                           Chain.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Chain.cpp
//!
//! Implements class Chain
//!
//===========================================================================

#include "Chain.hpp"
#include "SystemModelVisitor.hpp"

using namespace mast;

//! Visited part of the Visitor pattern
//!
void Chain::Accept (SystemModelVisitor& visitor)
{
  visitor.VisitChain(*this);
}

//===========================================================================
// End of Chain.cpp
//===========================================================================
