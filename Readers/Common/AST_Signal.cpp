//===========================================================================
//                           AST_Signal.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Signal.cpp
//!
//! Implements class AST_Signal
//!
//===========================================================================

#include "AST_Signal.hpp"
#include "AST_Number.hpp"
#include "AST_ScalarIdentifier.hpp"
#include "AST_VectorIdentifier.hpp"
#include <sstream>

using std::ostringstream;
using std::string;

using namespace Parsers;

//! Text representation of signal
//!
string AST_Signal::AsText () const
{
  ostringstream os;

  if (IsInverted())
  {
    os << "~";
  }

  if (IsNumber())
  {
    os << m_number->AsText();
  }
  else
  {
    os << AST_SimpleNode::AsText(m_path, "", ".");
    os << m_portName->AsText();
  }

  return os.str();
}
//
//  End of: AST_Signal::AsText
//---------------------------------------------------------------------------



//! Returns signal bits count
//!
uint32_t AST_Signal::BitsCount () const
{
  if (IsNumber())
  {
    return m_number->Size();
  }

  return m_portName->BitsCount();
}
//
//  End of: AST_Signal::BitsCount
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_Signal.cpp
//===========================================================================
