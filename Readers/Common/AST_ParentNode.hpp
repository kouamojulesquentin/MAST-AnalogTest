//===========================================================================
//                           AST_ParentNode.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ParentNode.hpp
//!
//! Declares AST_ParentNode class
//!
//===========================================================================


#ifndef AST_PARENTNODE_H__9C01A73E_808_43D6_A6B3_243B7A5440B__INCLUDED_
  #define AST_PARENTNODE_H__9C01A73E_808_43D6_A6B3_243B7A5440B__INCLUDED_

#include "AST_NamedNode.hpp"

namespace Parsers
{
//! Represents a named AST node that can have children
//!
//!
//!
class AST_ParentNode : public AST_NamedNode
{
  // ---------------- Public Methods
  //
  public:
  virtual ~AST_ParentNode() = default;
  AST_ParentNode()          = delete;

  // ---------------- Protected Methods
  //
  protected:
  AST_ParentNode(std::experimental::string_view name)
    : AST_NamedNode(name)
  {
  }

  // ---------------- Private Methods
  //
  private:

  // ---------------- Private Fields
  //
  private:
};
//
//  End of AST_ParentNode class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_PARENTNODE_H__9C01A73E_808_43D6_A6B3_243B7A5440B__INCLUDED_

//===========================================================================
// End of AST_ParentNode.hpp
//===========================================================================
