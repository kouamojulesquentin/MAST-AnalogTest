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
#include "Utility.hpp"

using namespace Parsers;
using namespace mast;

using std::make_unique;

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
AST_Module* AST::Create_Module (string_view name)
{
  auto node    = make_unique<AST_Module>(name);
  auto pointer = node.get();

  m_modulesNodes.emplace_back(std::move(node));
  return pointer;
}
//
//  End of: AST::Create_Module
//---------------------------------------------------------------------------



//! Creates an AST_ScanRegister node
//!
//! @param name ScanRegister name
//!
AST_ScanRegister* AST::Create_ScanRegister (string_view name)
{
  auto node    = make_unique<AST_ScanRegister>(name);
  auto pointer = node.get();

  m_nodes.emplace_back(std::move(node));
  return pointer;
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
