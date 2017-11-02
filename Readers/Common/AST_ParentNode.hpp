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
#include <vector>

namespace Parsers
{
class AST_ScalarIdentifier;

//! Represents a named AST node that can have children
//!
//!
class AST_ParentNode : public AST_NamedNode
{
  // ---------------- Public Methods
  //
  public:
  virtual ~AST_ParentNode() = default;
  AST_ParentNode()          = delete;

  //! Returns handle on not yet processed children
  //!
  //! @note This is mainly intended to be "processed" by visitors
  //!
  std::vector<AST_Node*>& UndispatchedChildren() { return m_undispatchedChildren; }

  // ---------------- Protected Methods
  //
  protected:

  //! Initializes with no children
  //!
  //! @note This is for case when children are optional
  //!
  AST_ParentNode(Kind kind)
    : AST_NamedNode(kind)
  {
  }

  //! Initializes with children
  //!
  AST_ParentNode(Kind kind, std::vector<AST_Node*>&& children)
    : AST_NamedNode          (kind)
    , m_undispatchedChildren (std::move(children))
  {
  }

  //! Dispatches children to specific members
  //!
  virtual void DispatchChildren () = 0;

  // ---------------- Protected Fields
  //
  protected:
  std::vector<AST_Node*>  m_undispatchedChildren; //!< Children nodes got from parsing and not yet processed (some may be nullptr)
};
//
//  End of AST_ParentNode class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_PARENTNODE_H__9C01A73E_808_43D6_A6B3_243B7A5440B__INCLUDED_

//===========================================================================
// End of AST_ParentNode.hpp
//===========================================================================
