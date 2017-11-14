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
#include "AST_Signal.hpp"
#include "AST_Source.hpp"
#include "AST_Visitor.hpp"
#include "Utility.hpp"

using std::string;
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



//! Returns ScanRegister bits count
//!
size_t AST_ScanRegister::BitsCount () const
{
  const auto& left  = m_identifier->LeftIndex();
  const auto& right = m_identifier->RightIndex();

  if (right.empty() || (left == right))
  {
    return 1u;
  }

  auto leftAsInt  = std::stoul(left);
  auto rightAsInt = std::stoul(right);

  auto count = 1u;
  count += (rightAsInt > leftAsInt) ? rightAsInt - leftAsInt
                                    : leftAsInt  - rightAsInt;

  return count;
}
//
//  End of: AST_ScanRegister::BitsCount
//---------------------------------------------------------------------------



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
        case Parsers::Kind::ResetValue:       SetChild(child, m_resetValue);       break;
        case Parsers::Kind::DefaultLoadValue: SetChild(child, m_defaultLoadValue); break;
        case Parsers::Kind::ScanInSource:     SetChild(child, m_scanInSource);     break;

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
const string& AST_ScanRegister::RangeLeft () const
{
  return m_identifier->LeftIndex();
}
//
//  End of: AST_ScanRegister::RangeLeft
//---------------------------------------------------------------------------


//! Returns scan register right index or empty string when defined as a scalar identifier (single bit)
//!
const string& AST_ScanRegister::RangeRight () const
{
  return m_identifier->RightIndex();
}
//
//  End of: AST_ScanRegister::RangeRight
//---------------------------------------------------------------------------



//! ScanRegister source base name (without indices)
//!
const string& AST_ScanRegister::SourceBaseName () const
{
  const auto& signals = m_scanInSource->Signals();

  CHECK_VALUE_EQ(signals.size(), 1u, "Expecting ScanOutPort source to be drive by exactly one signal");
  const auto signal      = signals.front();
  const auto portScope   = signal->PortScope();
  const auto identifier  = signal->PortName();

  const auto& name = identifier->Name();
  return name;
}
//
//  End of: AST_ScanRegister::SourceBaseName
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_ScanRegister.cpp
//===========================================================================
