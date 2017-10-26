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


  const std::string& RangeLeft()  const { return m_rangeLeft;  }
  const std::string& RangeRight() const { return m_rangeRight; }


  // ---------------- Private Methods
  //
  private:

  friend class AST;                                                         // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_ScanRegister)(std::experimental::string_view&,
                                          std::experimental::string_view&,
                                          std::experimental::string_view&); // AST currently uses make_unit<T>() to create nodes

  AST_ScanRegister(std::experimental::string_view name,
                   std::experimental::string_view rangeLeft  = "",
                   std::experimental::string_view rangeRight = "")
    : AST_ParentNode (name)
    , m_rangeLeft    (rangeLeft)
    , m_rangeRight   (rangeRight)
  {
  }

  // ---------------- Private Fields
  //
  private:
  std::string m_rangeLeft;
  std::string m_rangeRight;
};
//
//  End of AST_ScanRegister class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers

#endif  // not defined AST_SCANREGISTER_H__A8AADEC0_468C_4F9B_7B8F_A81761036A43__INCLUDED_

//===========================================================================
// End of AST_ScanRegister.hpp
//===========================================================================
