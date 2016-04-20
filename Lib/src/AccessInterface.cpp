//===========================================================================
//                           AccessInterface.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterface.cpp
//!
//! Implements class AccessInterface
//!
//===========================================================================

#include "AccessInterface.hpp"
#include "SystemModelVisitor.hpp"

using namespace mast;

//! Visited part of the Visitor pattern
//!
void AccessInterface::Accept (SystemModelVisitor& visitor)
{
  visitor.VisitAccessInterface(*this);
}


//===========================================================================
// End of AccessInterface.cpp
//===========================================================================
