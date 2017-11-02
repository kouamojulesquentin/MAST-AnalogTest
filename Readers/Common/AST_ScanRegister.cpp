//===========================================================================
//                           AST_ScanRegister.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ScanRegister.cpp
//!
//! Implements class AST_ScanRegister
//!
//===========================================================================

#include "AST_ScanRegister.hpp"
#include "AST_VectorIdentifier.hpp"
#include "AST_Value.hpp"
#include "AST_Source.hpp"
#include "AST_Visitor.hpp"
#include "Utility.hpp"

using std::experimental::string_view;
using namespace std::experimental::literals::string_view_literals;
using namespace std::string_literals;
using namespace Parsers;

//! Visited part of the Visitor pattern
//!
void AST_ScanRegister::Accept (AST_Visitor& visitor)
{
  visitor.Visit_ScanRegister(this);
}


//! Dispatches children to specific member (for ease of use)
//!
void AST_ScanRegister::DispatchChildren ()
{
  for (auto&  child:  UndispatchedChildren())
  {
    if (child != nullptr)
    {
      switch (child->GetKind())
      {
        case Parsers::Kind::ResetValue :
          m_resetValue = static_cast<AST_Value*>(child);
          child = nullptr;
          break;
        case Parsers::Kind::DefaultLoadValue:
          break;
        case Parsers::Kind::ScanInSource:
          m_scanInSource = static_cast<AST_Source*>(child);
          child = nullptr;
          break;
        default:  // Ignore all other for now
          break;
      }
    }
  }

  CHECK_VALUE_NOT_NULL(m_scanInSource, "The source of the scan input data for a ScanRegister is required");
}
//
//  End of: AST_ScanRegister::DispatchChildren
//---------------------------------------------------------------------------


//! Returns scan register left index or empty string when defined as a scalar identifier (single bit)
//!
string_view AST_ScanRegister::RangeLeft () const
{
  return m_identifier->LeftIndex();
}
//
//  End of: AST_ScanRegister::RangeLeft
//---------------------------------------------------------------------------


//! Returns scan register right index or empty string when defined as a scalar identifier (single bit)
//!
string_view AST_ScanRegister::RangeRight () const
{
  return m_identifier->RightIndex();
}
//
//  End of: AST_ScanRegister::RangeRight
//---------------------------------------------------------------------------



//===========================================================================
// End of AST_ScanRegister.cpp
//===========================================================================
