//===========================================================================
//                           Streamer.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Streamer.cpp
//!
//! Implements class Streamer
//!
//! A Streamer is a component that is put on a scan chain and modifies the 
//! stream of data coming through it, therefore having an effect on all
//! downstream components
//! A Streamer makes bit-by-bit transformations on incoming and outcoming data
//! which can be seen as a MASK applied on the whole Chain
//!
//===========================================================================

#include "Streamer.hpp"
#include "SystemModelVisitor.hpp"

using namespace mast;

//! Visited part of the Visitor pattern
//!
void Streamer::Accept (SystemModelVisitor& visitor)
{
  visitor.VisitStreamer(*this);
}

//===========================================================================
// End of Streamer.cpp
//===========================================================================
