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
#include "AST_Attribute.hpp"
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
//+  auto setChild = [](AST_Node*& child, auto& dest)
//+  {
//+    using t1 = decltype(dest);
//+    using dest_t = typename std::remove_reference<t1>::type;

//+    dest = static_cast<dest_t>(child);
//+    child = nullptr;
//+  };

  auto appendChild = [](AST_Node*& child, auto& dest)
  {
    using t1 = decltype(dest);
    using t2 = typename std::remove_reference<t1>::type;
    using dest_t = typename t2::value_type;

    dest.push_back(static_cast<dest_t>(child));
    child = nullptr;
  };

  for (auto& child : UndispatchedChildren())
  {
    if (child != nullptr)
    {
      switch (child->GetKind())
      {
        case Parsers::Kind::Attribute:
          appendChild(child, m_attributes);
          break;
        case Parsers::Kind::LocalParameter :
          appendChild(child, m_localParameters);
          break;
        case Parsers::Kind::Parameter :
          appendChild(child, m_parameters);
          break;
        case Parsers::Kind::ScanRegister :
          appendChild(child, m_scanRegisters);
          break;
        case Parsers::Kind::ScanInPort:
          appendChild(child, m_scanInPorts);
          break;
        case Parsers::Kind::ScanOutPort:
          appendChild(child, m_scanOutPorts);
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
