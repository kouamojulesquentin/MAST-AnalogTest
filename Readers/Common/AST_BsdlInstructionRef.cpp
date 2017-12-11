//===========================================================================
//                           AST_BsdlInstructionRef.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_BsdlInstructionRef.cpp
//!
//! Implements class AST_BsdlInstructionRef
//!
//===========================================================================

#include "AST_BsdlInstructionRef.hpp"
#include "AST_ScanInterfaceRef.hpp"

using namespace Parsers;


//! Dispatches children to specific member (for ease of use)
//!
void AST_BsdlInstructionRef::DispatchChildren ()
{
  for (auto& child : UndispatchedChildren())
  {
    if (child != nullptr)
    {
      switch (child->GetKind())
      {
        case Parsers::Kind::ScanInterfaceRef:
          AST_ParentNode::AppendChild(child, m_scanInterfacesRef);
          break;
        default:  // Ignore all other for now
          break;
      }
    }
  }
}
//
//  End of: AST_BsdlInstructionRef::DispatchChildren
//---------------------------------------------------------------------------



//===========================================================================
// End of AST_BsdlInstructionRef.cpp
//===========================================================================
