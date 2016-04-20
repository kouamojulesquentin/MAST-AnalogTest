//===========================================================================
//                           Linker.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Linker.cpp
//!
//! Implements class Linker
//!
//===========================================================================

#include "Linker.hpp"
#include "SystemModelVisitor.hpp"

using namespace mast;

//! Visited part of the Visitor pattern
//!
void Linker::Accept (SystemModelVisitor& visitor)
{
  visitor.VisitLinker(*this);
}



//! Returns pointer on byte-stream for last sequence shifted from sut
//!
const uint8_t* Linker::GetLastSequence () const
{
  return nullptr;
}
//
//  End of: Linker::GetLastSequence
//---------------------------------------------------------------------------


//! Returns pointer on byte-stream for next sequence to shift into sut
//!
const uint8_t* Linker::GetNextSequence () const
{
  return nullptr;
}
//
//  End of: Linker::GetNextSequence
//---------------------------------------------------------------------------



//===========================================================================
// End of Linker.cpp
//===========================================================================
