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
#include "Utility.hpp"

using std::string;

using namespace Parsers;


//! Constructor...
//!
//! @param namespaceName  Namespace of module definition
//! @param moduleName     Module name in the namespace
//!
AST_ModuleIdentifier::AST_ModuleIdentifier (const AST_Namespace* namespaceName, const AST_ScalarIdentifier* moduleName)
  : AST_Identifier (Kind::ModuleIdentifier, ""s)
  , m_namespace    (CHECK_PARAMETER_NOT_NULL(namespaceName, "Module identifier must have a valid namespace "))
  , m_moduleName   (CHECK_PARAMETER_NOT_NULL(moduleName,    "Module identifier must have a valid module name"))
{
}
//
//  End of: AST_ModuleIdentifier::AST_ModuleIdentifier
//---------------------------------------------------------------------------




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



//! Module name without namespace
//!
const string& AST_ModuleIdentifier::Name () const
{
  return m_moduleName->Name();
}
//
//  End of: AST_ModuleIdentifier::Name
//---------------------------------------------------------------------------



//===========================================================================
// End of AST_ModuleIdentifier.cpp
//===========================================================================
