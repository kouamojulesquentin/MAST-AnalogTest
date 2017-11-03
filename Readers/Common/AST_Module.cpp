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
#include "AST_ScanRegister.hpp"
#include "AST_Parameter.hpp"
#include "AST_Port.hpp"
#include "AST_Visitor.hpp"

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
        case Parsers::Kind::LocalParameter :
          m_localParameters.push_back(static_cast<AST_Parameter*>(child));
          child = nullptr;
          break;
        case Parsers::Kind::Parameter :
          m_parameters.push_back(static_cast<AST_Parameter*>(child));
          child = nullptr;
          break;
        case Parsers::Kind::ScanRegister :
          m_scanRegisters.push_back(static_cast<AST_ScanRegister*>(child));
          child = nullptr;
          break;
        case Parsers::Kind::ScanInPort:
          m_scanInPort = static_cast<AST_Port*>(child);
          child = nullptr;
          break;
        case Parsers::Kind::ScanOutPort:
          m_scanOutPort = static_cast<AST_Port*>(child);
          child = nullptr;
          break;
        default:  // Ignore all other for now
          break;
      }
    }
  }
}
//
//  End of: AST_Module::DispatchChildren
//---------------------------------------------------------------------------



//===========================================================================
// End of AST_Module.cpp
//===========================================================================
