//===========================================================================
//                           AST_Instance.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Instance.cpp
//!
//! Implements class AST_Instance
//!
//===========================================================================

#include "AST_Identifier.hpp"
#include "AST_Instance.hpp"
#include "AST_Attribute.hpp"
#include "AST_ModuleIdentifier.hpp"
#include "AST_Parameter.hpp"
#include "AST_Port.hpp"
#include "AST_ScalarIdentifier.hpp"
#include "AST_Visitor.hpp"

using std::string;

using namespace Parsers;


//! Visited part of the Visitor pattern
//!
void AST_Instance::Accept (AST_Visitor& visitor)
{
  visitor.Visit_Instance(this);
}


//! Dispatches children to specific member (for ease of use)
//!
void AST_Instance::DispatchChildren ()
{
  for (auto& child : UndispatchedChildren())
  {
    if (child != nullptr)
    {
      switch (child->GetKind())
      {
        case Parsers::Kind::Attribute: AST_ParentNode::AppendChild(child, m_attributes); break;
        case Parsers::Kind::Parameter: AST_ParentNode::AppendChild(child, m_parameters); break;
        case Parsers::Kind::InputPort: AST_ParentNode::AppendChild(child, m_inputPorts); break;

        default:  // Ignore all other for now
          break;
      }
    }
  }
}
//
//  End of: AST_Instance::DispatchChildren
//---------------------------------------------------------------------------



//! Searches for a InputPort with specified identifier
//!
//! @param identifier   An identifier for InputPort to find
//!
const AST_Port* AST_Instance::FindInputPort (const AST_Identifier* identifier) const
{
  CHECK_PARAMETER_NOT_NULL(identifier, "Cannot find InputPort from nullptr identifier");

  AST_Port* foundInputPort = nullptr;

  for (auto inputPort : m_inputPorts)
  {
    if (inputPort->Identifier()->Name() == identifier->Name())
    {
      foundInputPort = inputPort;
      break;
    }
  }

  return foundInputPort;
}
//
//  End of: AST_Instance::FindInputPort
//---------------------------------------------------------------------------


//! Returns instance name (in enclosing module)
//!
string AST_Instance::Name () const
{
  return m_instanceIdentifier->AsText();
}
//
//  End of: AST_Instance::Name
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_Instance.cpp
//===========================================================================
