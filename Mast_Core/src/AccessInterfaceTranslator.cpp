//===========================================================================
//                           AccessInterfaceTranslator.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterfaceTranslator.cpp
//!
//! Implements class AccessInterfaceTranslator
//!
//===========================================================================

#include "AccessInterfaceTranslator.hpp"
#include "SystemModelVisitor.hpp"

using namespace mast;

//! Visited part of the Visitor pattern
//!
void AccessInterfaceTranslator::Accept (SystemModelVisitor& visitor)
{
  visitor.VisitAccessInterfaceTranslator(*this);
}


//===========================================================================
// End of AccessInterface.cpp
//===========================================================================
