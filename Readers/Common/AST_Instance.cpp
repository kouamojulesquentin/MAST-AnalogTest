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

#include "AST_Instance.hpp"
#include "AST_Attribute.hpp"
#include "AST_ModuleIdentifier.hpp"
#include "AST_Parameter.hpp"
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
        case Parsers::Kind::Attribute:
          AST_ParentNode::AppendChild(child, m_attributes);
          break;
        case Parsers::Kind::Parameter :
          AST_ParentNode::AppendChild(child, m_parameters);
          break;
        default:  // Ignore all other for now
          break;
      }
    }
  }
}
//
//  End of: AST_Instance::DispatchChildren
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
