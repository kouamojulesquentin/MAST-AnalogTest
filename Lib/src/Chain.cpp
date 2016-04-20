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


//! Returns pointer on byte-stream for last sequence shifted from sut
//!
const uint8_t* Chain::GetLastSequence () const
{
  return nullptr;
}
//
//  End of: Chain::GetLastSequence
//---------------------------------------------------------------------------


//! Returns pointer on byte-stream for next sequence to shift into sut
//!
const uint8_t* Chain::GetNextSequence () const
{
  return nullptr;
}
//
//  End of: Chain::GetNextSequence
//---------------------------------------------------------------------------




//===========================================================================
// End of Chain.cpp
//===========================================================================
