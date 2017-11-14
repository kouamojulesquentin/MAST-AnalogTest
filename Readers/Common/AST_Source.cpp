//===========================================================================
//                           AST_Source.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Source.cpp
//!
//! Implements class AST_Source
//!
//===========================================================================

#include "AST_Source.hpp"
#include "AST_Signal.hpp"
#include "AST_Visitor.hpp"

#include <sstream>

using std::ostringstream;
using std::string;
using namespace Parsers;


//! Constructs from a bunch of ordered (concatenated) signals
//!
AST_Source::AST_Source (Kind kind, std::vector<AST_Signal*>&& signals)
  : AST_SimpleNode (kind)
  , m_signals      (std::move(signals))
{
  CHECK_PARAMETER_NOT_EMPTY(m_signals, "Source must have at least one signal");
}
//
//  End of: AST_Source::AST_Source
//---------------------------------------------------------------------------



//! Visited part of the Visitor pattern
//!
void AST_Source::Accept (AST_Visitor& visitor)
{
  visitor.Visit_Source(this);
}
//
//  End of: AST_Source::Accept
//---------------------------------------------------------------------------


//! Text representation of source
//!
string AST_Source::AsText () const
{
  auto asText =  AST_SimpleNode::AsText(m_signals, ", ");

  return asText;
}
//
//  End of: AST_Source::AsText
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_Source.cpp
//===========================================================================
