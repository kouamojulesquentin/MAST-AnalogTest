//===========================================================================
//                           AST_Module.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Module.hpp
//!
//! Declares AST_Module class
//!
//===========================================================================

#ifndef AST_MODULE_H__A9A400A_9537_4176_349E_822BAF7E4956__INCLUDED_
  #define AST_MODULE_H__A9A400A_9537_4176_349E_822BAF7E4956__INCLUDED_

#include "AST_ParentNode.hpp"
#include <memory>
#include <experimental/string_view>

namespace Parsers
{
//! Represents a parsed test network, instiable, module
//!
class AST_Module final : public AST_ParentNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_Module() = default;
  AST_Module()  = delete;

  //! Visited part of the Visitor pattern
  //!
  void Accept (AST_Visitor& visitor) override;

  // ---------------- Private Methods
  //
  private:
  friend class AST;                                                   // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_Module)(std::experimental::string_view&, std::vector<AST_Node*>&&); // AST currently uses make_unit<T>() to create nodes

  AST_Module(std::experimental::string_view name, std::vector<AST_Node*>&& children)
    : AST_ParentNode (Kind::Module, name, std::move(children))
  {
  }

  // ---------------- Private Fields
  //
  private:
};
//
//  End of AST_Module class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_MODULE_H__A9A400A_9537_4176_349E_822BAF7E4956__INCLUDED_

//===========================================================================
// End of AST_Module.hpp
//===========================================================================
