//===========================================================================
//                           AppFunctionNameAndNode.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AppFunctionNameAndNode.hpp
//!
//! Declares AppFunctionNameAndNode struct
//!
//===========================================================================

#ifndef APPFUNCTIONNAMEANDNODE_H__10C6AB21_6BEB_4D9B_3B8B_262CEF0589B1__INCLUDED_
  #define APPFUNCTIONNAMEANDNODE_H__10C6AB21_6BEB_4D9B_3B8B_262CEF0589B1__INCLUDED_

#include <string>
#include <memory>
#include <functional>

namespace mast
{
  class ParentNode;

  //! Associates a name for application function with a node in SystemModel
  //!
  struct AppFunctionNameAndNode final
  {
    std::string                 appName;  //!< PDL algorithm identifier
    std::shared_ptr<ParentNode> node;     //!< Associated node
    uint32_t                    line = 0; //!< file line number where the PDL declaration was

    //! Initializes instance with parameters
    //!
    AppFunctionNameAndNode(std::string p_appName, std::shared_ptr<ParentNode> p_node, int p_line)
      : appName (std::move(p_appName))
      , node    (p_node)
      , line    (p_line)
    {}

    //! Returns true if compares equal to another AppFunctionNameAndNode
    //!
    bool operator==(const AppFunctionNameAndNode& rhs) const
    {
      return   (appName == rhs.appName)
            && (node == rhs.node)
            && (line == rhs.line);
    }

    //! Returns true if compares not equal to another AppFunctionNameAndNode
    //!
    bool operator!=(const AppFunctionNameAndNode& rhs) const
    {
      return !(*this == rhs);
    }
  };
  //
  //  End of AppFunctionNameAndNode struct declaration
  //---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined APPFUNCTIONNAMEANDNODE_H__10C6AB21_6BEB_4D9B_3B8B_262CEF0589B1__INCLUDED_
//===========================================================================
// End of AppFunctionNameAndNode.hpp
//===========================================================================
