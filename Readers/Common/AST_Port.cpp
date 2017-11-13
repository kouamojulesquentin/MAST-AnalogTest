//===========================================================================
//                           AST_Port.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Port.cpp
//!
//! Implements class AST_Port
//!
//===========================================================================

#include "AST_Port.hpp"
#include "AST_Attribute.hpp"
#include "AST_Source.hpp"
#include "AST_Visitor.hpp"

using namespace Parsers;



//! Visited part of the Visitor pattern
//!
void AST_Port::Accept (AST_Visitor& visitor)
{
  visitor.Visit_Port(this);
}
//
//  End of: AST_Port::Accept
//---------------------------------------------------------------------------


//! Dispatches children to specific member (for ease of use)
//!
void AST_Port::DispatchChildren ()
{
  for (auto& child : UndispatchedChildren())
  {
    if (child != nullptr)
    {
      switch (child->GetKind())
      {
        case Parsers::Kind::Attribute :
          m_attributes.push_back(static_cast<AST_Attribute*>(child));
          child = nullptr;
          break;
        case Parsers::Kind::Source :
        case Parsers::Kind::InputPortSource :
          m_source = static_cast<AST_Source*>(child);
          child = nullptr;
          break;
        default:  // Ignore all other for now
          break;
      }
    }
  }
}
//
//  End of: AST_Port::DispatchChildren
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_Port.cpp
//===========================================================================
