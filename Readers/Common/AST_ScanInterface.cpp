//===========================================================================
//                           AST_ScanInterface.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ScanInterface.cpp
//!
//! Implements class AST_ScanInterface
//!
//===========================================================================

#include "AST_ScanInterface.hpp"
#include "AST_Attribute.hpp"
#include "AST_Port.hpp"
#include "AST_ScalarIdentifier.hpp"
#include "AST_Visitor.hpp"

using std::string;

using namespace Parsers;


//! Visited part of the Visitor pattern
//!
void AST_ScanInterface::Accept (AST_Visitor& visitor)
{
  visitor.Visit_ScanInterface(this);
}


//! Dispatches children to specific member (for ease of use)
//!
void AST_ScanInterface::DispatchChildren ()
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
        case Parsers::Kind::Port :
          AST_ParentNode::AppendChild(child, m_ports);
          break;
        default:  // Ignore all other for now
          break;
      }
    }
  }
}
//
//  End of: AST_ScanInterface::DispatchChildren
//---------------------------------------------------------------------------



//! Returns instance name (in enclosing module)
//!
string AST_ScanInterface::Name () const
{
  return m_identifier->AsText();
}
//
//  End of: AST_ScanInterface::Name
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_ScanInterface.cpp
//===========================================================================
