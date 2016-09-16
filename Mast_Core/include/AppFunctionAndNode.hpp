//===========================================================================
//                           AppFunctionAndNode.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AppFunctionAndNode.hpp
//!
//! Declares AppFunctionAndNode struct
//!
//===========================================================================

#ifndef APPFUNCTIONANDNODE_H__582B27ED_5C9D_4973_4BA1_9589CD255535__INCLUDED_
  #define APPFUNCTIONANDNODE_H__582B27ED_5C9D_4973_4BA1_9589CD255535__INCLUDED_

#include <string>
#include <memory>
#include <functional>

namespace mast
{
  class ParentNode;

  //! Associates application function (an algorithm) with an associated node in SystemModel (and an optional application name that may appear in log)
  //!
  struct AppFunctionAndNode final
  {
    using Application_t = std::function<void()>;

    Application_t               appFunction;
    std::shared_ptr<ParentNode> node;
    std::string                 appName;

    AppFunctionAndNode(Application_t p_appFunction, std::shared_ptr<ParentNode> p_node, std::string p_appName = "")
      : appFunction (p_appFunction)
      , node        (p_node)
      , appName     (std::move(p_appName))
    {}
  };
  //
  //  End of AppFunctionAndNode struct declaration
  //---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined APPFUNCTIONANDNODE_H__582B27ED_5C9D_4973_4BA1_9589CD255535__INCLUDED_
//===========================================================================
// End of AppFunctionAndNode.hpp
//===========================================================================
