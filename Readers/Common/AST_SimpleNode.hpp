//===========================================================================
//                           AST_SimpleNode.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_SimpleNode.hpp
//!
//! Declares AST_SimpleNode class
//!
//===========================================================================

#ifndef AST_SIMPLENODE_H__72A2774B_F75F_4120_6CBF_EB31A65A751A__INCLUDED_
  #define AST_SIMPLENODE_H__72A2774B_F75F_4120_6CBF_EB31A65A751A__INCLUDED_

#include "AST_Node.hpp"

#include <string>
#include <experimental/string_view>

namespace Parsers
{
//! Represents a simple (not parent) AST node that can be represented as, one line, text
//!
//! @note AST_SimpleNode can nevertheless have AST_Node childrens but usually not AST_Parent ones
//!
class AST_SimpleNode : public AST_Node
{
  // ---------------- Public Methods
  //
  public:
  ~AST_SimpleNode() = default;
  AST_SimpleNode()  = delete;

  //! Text representation of attribute
  //!
  virtual std::string AsText() const = 0;

  // ---------------- Protected Methods
  //
  protected:

  AST_SimpleNode(Kind kind)
    : AST_Node (kind)
  {
  }
};
//
//  End of AST_SimpleNode class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers





#endif  // not defined AST_SIMPLENODE_H__72A2774B_F75F_4120_6CBF_EB31A65A751A__INCLUDED_
//===========================================================================
// End of AST_SimpleNode.hpp
//===========================================================================
