//===========================================================================
//                           AST_ScanRegister.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ScanRegister.hpp
//!
//! Declares AST_ScanRegister class
//!
//===========================================================================

#ifndef AST_SCANREGISTER_H__A8AADEC0_468C_4F9B_7B8F_A81761036A43__INCLUDED_
  #define AST_SCANREGISTER_H__A8AADEC0_468C_4F9B_7B8F_A81761036A43__INCLUDED_

#include "AST_ParentNode.hpp"
#include "AST_Identifier.hpp"

#include <memory>
using std::experimental::string_view;

namespace Parsers
{
//! Represents a parsed test network, instiable, module
//!
class AST_ScanRegister final : public AST_ParentNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_ScanRegister() = default;
  AST_ScanRegister()  = delete;

  //! Visited part of the Visitor pattern
  //!
  void Accept (AST_Visitor& visitor) override;

  //! Module name
  //!
  std::string Name() const override { return m_identifier->AsText(); }


  std::experimental::string_view RangeLeft()  const { return m_identifier->LeftIndex();  }
  std::experimental::string_view RangeRight() const { return m_identifier->RightIndex(); }


  // ---------------- Private Methods
  //
  private:

  friend class AST;                                                   // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_ScanRegister)(Parsers::AST_Identifier*&); // AST currently uses make_unit<T>() to create nodes

  AST_ScanRegister(Parsers::AST_Identifier* identifier)
    : AST_ParentNode (Kind::ScanRegister)
    , m_identifier   (identifier)
  {
  }

  // ---------------- Private Fields
  //
  private:
  const AST_Identifier* m_identifier = nullptr; //!< Module name
};
//
//  End of AST_ScanRegister class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers

#endif  // not defined AST_SCANREGISTER_H__A8AADEC0_468C_4F9B_7B8F_A81761036A43__INCLUDED_

//===========================================================================
// End of AST_ScanRegister.hpp
//===========================================================================
