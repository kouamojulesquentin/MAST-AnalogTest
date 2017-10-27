//===========================================================================
//                           AST_NamedNode.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_NamedNode.hpp
//!
//! Declares AST_NamedNode class
//!
//===========================================================================

#ifndef AST_NAMEDNODE_H__A9E167DF_B162_4845_A187_CEAE38557F__INCLUDED_
  #define AST_NAMEDNODE_H__A9E167DF_B162_4845_A187_CEAE38557F__INCLUDED_

#include "AST_Node.hpp"
#include "AST_ScalarIdentifier.hpp"

#include <string>
#include <experimental/string_view>

namespace Parsers
{
//! Represents an AST node with a name
//!
class AST_NamedNode : public AST_Node
{
  // ---------------- Public Methods
  //
  public:
  ~AST_NamedNode() = default;
  AST_NamedNode()  = delete;

  //! Name of parsed entity
  //!
  virtual std::string Name() const = 0;

  // ---------------- Protected Methods
  //
  protected:

  AST_NamedNode(Kind kind)
    : AST_Node (kind)
  {
  }
};
//
//  End of AST_NamedNode class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_NAMEDNODE_H__A9E167DF_B162_4845_A187_CEAE38557F__INCLUDED_

//===========================================================================
// End of AST_NamedNode.hpp
//===========================================================================
