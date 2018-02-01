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
#include "AST_ConcatNumber.hpp"
#include "Utility.hpp"

#include <sstream>

using std::ostringstream;
using std::string;
using std::vector;

using namespace Parsers;



//! Initializes AST_ScanMuxSelection with selection values and selected signals
//!
//! @param selectionValues  Selection values
//! @param selectedSignals  Selected signal when multiplex selector has one of selection values
//!
AST_ScanMuxSelection::AST_ScanMuxSelection (vector<AST_ConcatNumber*>&& selectionValues, vector<Parsers::AST_Signal*>&& selectedSignals)
  : AST_SimpleNode    (Kind::ScanMuxSelection)
  , m_selectionValues (selectionValues)
  , m_selectedSignals (selectedSignals)
{
  CHECK_PARAMETER_NOT_EMPTY(m_selectionValues, "ScanMux Selection must have a value");
  CHECK_PARAMETER_NOT_EMPTY(m_selectedSignals, "ScanMux Selection must select at least one signal");
}
//
//  End of: AST_ScanMuxSelection::AST_ScanMuxSelection
//---------------------------------------------------------------------------




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
