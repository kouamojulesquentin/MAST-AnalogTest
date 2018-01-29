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
#include "Utility.hpp"

#include <vector>
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
  virtual ~AST_SimpleNode() = default;
  AST_SimpleNode()  = delete;

  //! Text representation of attribute
  //!
  virtual std::string AsText() const = 0;

  //! Builds up text representation for a collection of strings
  //!
  //! @param strings    Collection of strings
  //! @param separator  Optional text to place between 2 strings (can be empty)
  //! @param suffix     Optional text to place after each strings (usually empty)
  //!
  //! @return Built up string
  static std::string AsText(const std::vector<std::string>& strings,
                            std::experimental::string_view  separator,
                            std::experimental::string_view  suffix = "")
  {
    std::string result;

    auto first = true;
    for (const auto& curString : strings)
    {
      if (!first && !separator.empty())
      {
        result.append(separator.cbegin(), separator.cend());
      }
      else
      {
        first = false;
      }

      result.append(curString);

      if (!suffix.empty())
      {
        result.append(suffix.cbegin(), suffix.cend());
      }
    }
    return result;
  }

  //! Builds up text representation for a collection of AST_SimpleNodes
  //!
  //! @param nodes      Collection of nodes
  //! @param separator  Optional text to place between 2 nodes (can be empty)
  //! @param suffix     Optional text to place after each node (usually empty)
  //!
  //! @return Built up string
  template<typename Container>
  static std::string AsText(const Container&               nodes,
                            std::experimental::string_view separator,
                            std::experimental::string_view suffix = "")
  {
    std::string result;

    auto first = true;
    for (const auto& node : nodes)
    {
      CHECK_VALUE_NOT_NULL(node, "Expected to have only not nullptr nodes");
      if (!first && !separator.empty())
      {
        result.append(separator.cbegin(), separator.cend());
      }
      else
      {
        first = false;
      }

      result.append(node->AsText());

      if (!suffix.empty())
      {
        result.append(suffix.cbegin(), suffix.cend());
      }
    }
    return result;
  }

  // ---------------- Protected Methods
  //
  protected:

  //! Initializes AST_SimpleNode with specified AST node kind
  //!
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
