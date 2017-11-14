//===========================================================================
//                           AST_Module.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Module.cpp
//!
//! Implements class AST_Module
//!
//===========================================================================

#include "AST_Module.hpp"
#include "AST_AccessLink.hpp"
#include "AST_Attribute.hpp"
#include "AST_ScanInterface.hpp"
#include "AST_ScanRegister.hpp"
#include "AST_Parameter.hpp"
#include "AST_Port.hpp"
#include "AST_Visitor.hpp"

#include <type_traits>

using namespace Parsers;


//! Visited part of the Visitor pattern
//!
void AST_Module::Accept (AST_Visitor& visitor)
{
  visitor.Visit_Module(this);
}


//! Dispatches children to specific member (for ease of use)
//!
void AST_Module::DispatchChildren ()
{
  for (auto& child : UndispatchedChildren())
  {
    if (child != nullptr)
    {
      switch (child->GetKind())
      {
        case Parsers::Kind::Attribute:      AST_ParentNode::AppendChild(child, m_attributes);      break;
        case Parsers::Kind::LocalParameter: AST_ParentNode::AppendChild(child, m_localParameters); break;
        case Parsers::Kind::Parameter:      AST_ParentNode::AppendChild(child, m_parameters);      break;
        case Parsers::Kind::ScanInterface:  AST_ParentNode::AppendChild(child, m_scanInterfaces);  break;
        case Parsers::Kind::ScanInPort:     AST_ParentNode::AppendChild(child, m_scanInPorts);     break;
        case Parsers::Kind::ScanOutPort:    AST_ParentNode::AppendChild(child, m_scanOutPorts);    break;
        case Parsers::Kind::ScanRegister:   AST_ParentNode::AppendChild(child, m_scanRegisters);   break;

        case Parsers::Kind::AccessLink :    SetChild(child, m_accessLink); break;

        default:  // Ignore all other for now
          break;
      }
    }
  }
}
//
//  End of: AST_Module::DispatchChildren
//---------------------------------------------------------------------------



//! Searches for a ScanRegister with specified identifier
//!
//! @param identifier   An identifier for ScanRegister to find
//!
AST_ScanRegister* AST_Module::FindScanRegister (const AST_Identifier* identifier)
{
  CHECK_PARAMETER_NOT_NULL(identifier, "Cannot find ScanRegister from nullptr identifier");

  AST_ScanRegister* foundScanRegister = nullptr;

  for (auto scanRegister : m_scanRegisters)
  {
    if (scanRegister->Identifier()->Name() == identifier->Name())
    {
      foundScanRegister = scanRegister;
      break;
    }
  }

  return foundScanRegister;
}
//
//  End of: AST_Module::FindScanRegister
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_Module.cpp
//===========================================================================
