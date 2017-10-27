//===========================================================================
//                           AST.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST.cpp
//!
//! Implements class AST
//!
//===========================================================================

#include "AST.hpp"
#include "AST_Module.hpp"
#include "AST_ScanRegister.hpp"
#include "AST_ScalarIdentifier.hpp"
#include "AST_Identifier.hpp"
#include "Utility.hpp"

using std::vector;
using std::make_unique;

using namespace Parsers;
using namespace mast;


//! Releases constructed AST nodes
//!
AST::~AST ()
{
}
//
//  End of: AST::~AST
//---------------------------------------------------------------------------


//! Initializes AST
//!
AST::AST ()
{
}
//
//  End of: AST::AST
//---------------------------------------------------------------------------




//! Creates a AST_Module node
//!
//! @param name Module name
//!
AST_Module* AST::Create_Module (AST_ScalarIdentifier* identifier, vector<AST_Node*>&& children)
{
  CHECK_PARAMETER_NOT_NULL(identifier, "identifier must not be nullptr");

  auto node    = make_unique<AST_Module>(identifier, std::move(children));
  auto pointer = node.get();

  m_modulesNodes.emplace_back(std::move(node));
  return pointer;
}
//
//  End of: AST::Create_Module
//---------------------------------------------------------------------------


//! Creates an AST_ScalarIdentifier node
//!
//! @param name Identifier
//!
AST_ScalarIdentifier* AST::Create_ScalarIdentifier (string_view name)
{
  return Create_Node<AST_ScalarIdentifier>(name);
}
//
//  End of: AST::Create_ScalarIdentifier
//---------------------------------------------------------------------------


//! Creates an AST_Identifier node
//!
//! @param name         Identifier
//! @param leftIndex    Left index
//! @param rightIndex   Right index (can be empty for single bit identifier)
//!
//! @return Created AST_Identifier
//!
AST_Identifier* AST::Create_Identifier (string_view name,
                                        string_view leftIndex,
                                        string_view rightIndex)
{
  return Create_Node<AST_Identifier>(name, leftIndex, rightIndex);
}
//
//  End of: AST::Create_ScanRegister
//---------------------------------------------------------------------------


//! Creates an AST_ScanRegister node
//!
//! @param identifier   ScanRegister identifier
//!
//! @return Created AST_ScanRegister
//!
AST_ScanRegister* AST::Create_ScanRegister (AST_Identifier* identifier)
{
  return Create_Node<AST_ScanRegister>(identifier);
}
//
//  End of: AST::Create_ScanRegister
//---------------------------------------------------------------------------



//! Returns "top" parsed module
//!
AST_Module* AST::TopModule ()
{
  CHECK_VALUE_NOT_EMPTY(m_modulesNodes, "AST has no module yet");

  return m_modulesNodes.front().get();
}
//
//  End of: AST::TopModule
//---------------------------------------------------------------------------




//===========================================================================
// End of AST.cpp
//===========================================================================
