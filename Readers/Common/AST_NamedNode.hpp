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
#include <string>
#include <experimental/string_view>

using std::string;
using std::experimental::string_view;

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
  std::experimental::string_view Name() const { return m_name; }

  // ---------------- Protected Methods
  //
  protected:

  AST_NamedNode(std::experimental::string_view name)
    : m_name (name)
  {
  }

  // ---------------- Private Fields
  //
  private:
  std::string m_name; //!< Name of parsed entity
};
//
//  End of AST_NamedNode class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_NAMEDNODE_H__A9E167DF_B162_4845_A187_CEAE38557F__INCLUDED_

//===========================================================================
// End of AST_NamedNode.hpp
//===========================================================================
