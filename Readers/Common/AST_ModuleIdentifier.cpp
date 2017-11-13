//===========================================================================
//                           AST_ModuleIdentifier.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ModuleIdentifier.cpp
//!
//! Implements class AST_ModuleIdentifier
//!
//===========================================================================

#include "AST_ModuleIdentifier.hpp"
#include "AST_Namespace.hpp"
#include "AST_ScalarIdentifier.hpp"

using std::string;

using namespace Parsers;



//! Text representation of signal
//!
string AST_ModuleIdentifier::AsText () const
{
  auto asText = m_namespace->AsText();
  asText.append(m_moduleName->AsText());

  return asText;
}
//
//  End of: AST_ModuleIdentifier::AsText
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_ModuleIdentifier.cpp
//===========================================================================
