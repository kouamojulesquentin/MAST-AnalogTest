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
    std::string                 appName;
    std::shared_ptr<ParentNode> node;

    AppFunctionNameAndNode(std::string p_appName, std::shared_ptr<ParentNode> p_node)
      : appName (std::move(p_appName))
      , node    (p_node)
    {}
  };
  //
  //  End of AppFunctionNameAndNode struct declaration
  //---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined APPFUNCTIONNAMEANDNODE_H__10C6AB21_6BEB_4D9B_3B8B_262CEF0589B1__INCLUDED_
//===========================================================================
// End of AppFunctionNameAndNode.hpp
//===========================================================================
