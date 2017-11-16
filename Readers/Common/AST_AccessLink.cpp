//===========================================================================
//                           AST_AccessLink.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_AccessLink.cpp
//!
//! Implements class AST_AccessLink
//!
//===========================================================================

#include "AST_AccessLink.hpp"
#include "AST_ScalarIdentifier.hpp"
#include "AST_FileRef.hpp"
#include "AST_Visitor.hpp"

using std::string;

using namespace Parsers;


//! Visited part of the Visitor pattern
//!
void AST_AccessLink::Accept (AST_Visitor& visitor)
{
  visitor.Visit_AccessLink(this);
}


//! Returns name/path of referred BSDL
//!
const string& AST_AccessLink::BSDLName () const
{
  return m_bsdlFile->Name();
}
//
//  End of: AST_AccessLink::BSDLName
//---------------------------------------------------------------------------



//! Dispatches children to specific member (for ease of use)
//!
void AST_AccessLink::DispatchChildren ()
{
  for (auto&  child: UndispatchedChildren())
  {
    if (child != nullptr)
    {
      switch (child->GetKind())
      {
        case Parsers::Kind::BSDLEntity : SetChild(child, m_bsdlFile); break;
        default:  // Ignore all other for now
          break;
      }
    }
  }
}
//
//  End of: AST_AccessLink::DispatchChildren
//---------------------------------------------------------------------------



//! Returns instance name (in enclosing module)
//!
string AST_AccessLink::Name () const
{
  return m_identifier->AsText();
}
//
//  End of: AST_AccessLink::Name
//---------------------------------------------------------------------------



//! Returns AccessLink type as text representation
//!
string AST_AccessLink::TypeAsText () const
{
  switch (m_type)
  {
    case AccessLinkType::Generic:         return m_genericId->AsText();
    case AccessLinkType::STD_1149_1_2001: return "STD_1149_1_2001";
    case AccessLinkType::STD_1149_1_2013: return "STD_1149_1_2013";
    case AccessLinkType::Undefined:       return "Undefined";
    default:
      CHECK_FAILED("Has invalid AccessLinkType");
  }
}
//
//  End of: AST_AccessLink::TypeAsText
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_AccessLink.cpp
//===========================================================================
