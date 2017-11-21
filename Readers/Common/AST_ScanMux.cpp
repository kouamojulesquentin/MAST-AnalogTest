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
#include "Utility.hpp"

using std::vector;
using namespace Parsers;


//! Initializes AST_ScanMux
//!
//! @param identifier           ScanMux identifier
//! @param selectors            Selection signals that are used to drive the ScanMux
//! @param scanMuxSelections    Selections definition i.e. which value(s) select which signal(s)
//!
AST_ScanMux::AST_ScanMux (AST_VectorIdentifier*           identifier,
                          vector<Parsers::AST_Signal*>&&  selectors,
                          vector<AST_ScanMuxSelection*>&& scanMuxSelections)
  : AST_ParentNode (Kind::ScanMux)
  , m_identifier   (identifier)
  , m_selectors    (std::move(selectors))
  , m_selections   (std::move(scanMuxSelections))
{
  CHECK_PARAMETER_NOT_NULL(identifier,         "ScanMux must have a valid identifier");
  CHECK_PARAMETER_NOT_EMPTY(m_selectors,       "ScanMux must have at least one selector");
  CHECK_PARAMETER_GTE(m_selections.size(), 2u, "ScanMux must have at least two defined selection");

  DispatchChildren();
}
//
//  End of: AST_ScanMux::AST_ScanMux
//---------------------------------------------------------------------------




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



//! Tells whether this ScanMux multiplexes more than one bit
//!
bool AST_ScanMux::IsBusMux () const
{
  return !m_identifier->IsSingleBit();
}
//
//  End of: AST_ScanMux::IsBusMux
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_ScanMux.cpp
//===========================================================================
