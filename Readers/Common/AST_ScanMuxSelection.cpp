//===========================================================================
//                           AST_ScanMuxSelection.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ScanMuxSelection.cpp
//!
//! Implements class AST_ScanMuxSelection
//!
//===========================================================================

#include "AST_ScanMuxSelection.hpp"
#include "AST_Signal.hpp"

#include <sstream>

using std::ostringstream;
using std::string;

using namespace Parsers;



//! Text representation of value
//!
string AST_ScanMuxSelection::AsText () const
{
  ostringstream os;

  os << AST_SimpleNode::AsText(m_selectionValues, " | ");
  os << " : ";
  os << AST_SimpleNode::AsText(m_selectedSignals, ", ");

  return os.str();
}
//
//  End of: AST_ScanMuxSelection::AsText
//---------------------------------------------------------------------------





//===========================================================================
// End of AST_ScanMuxSelection.cpp
//===========================================================================
