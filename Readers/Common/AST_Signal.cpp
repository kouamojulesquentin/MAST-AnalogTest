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
#include "AST_Builder.hpp"
#include "AST_Number.hpp"
#include "AST_Parameter.hpp"
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


//! Returns true when there is a parameter reference in any part of signal
//!
bool AST_Signal::HasParameterRef () const
{
  if ((m_number != nullptr) && m_number->HasParameterRef())
  {
    return true;
  }

  if ((m_portName != nullptr) && m_portName->HasParameterRef())
  {
    return true;
  }

  return false;
}
//
//  End of: AST_Signal::HasParameterRef
//---------------------------------------------------------------------------


//! Replaces parameter references by its actual value
//!
//! @note This must be called after unification pass
//!
//! @param parameters   Parameter definitions to use to replace parameter reference(s)
//!
void AST_Signal::Resolve (const std::vector<AST_Parameter*>& parameters)
{
  if ((m_number != nullptr) && m_number->HasParameterRef())
  {
    m_number->Resolve(parameters);
  }

  if ((m_portName != nullptr) && m_portName->HasParameterRef())
  {
    m_portName->Resolve(parameters);
  }
}
//
//  End of: AST_Signal::Resolve
//---------------------------------------------------------------------------



//! Returns uniquified clone
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//!
//! @return New cloned and uniquified AST_Signal
AST_Signal* AST_Signal::UniquifiedClone (AST_Builder& astBuilder) const
{
  auto clone = astBuilder.Clone_Signal(this);

  // ---------------- Identifier
  //
  if (m_number != nullptr)
  {
    clone->m_number = m_number->UniquifiedClone(astBuilder);
  }

  // ---------------- iApplyEndState
  //
  if (m_portName != nullptr)
  {
    clone->m_portName = m_portName->UniquifiedClone(astBuilder);
  }

  return clone;
}
//
//  End of: AST_Signal::UniquifiedClone
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_Signal.cpp
//===========================================================================
