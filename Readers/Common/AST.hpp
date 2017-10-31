//===========================================================================
//                           AST.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST.hpp
//!
//! Declares AST class
//!
//===========================================================================

#ifndef AST_H__C18638A1_3F91_4757_EBB6_C119CFFA82E__INCLUDED_
  #define AST_H__C18638A1_3F91_4757_EBB6_C119CFFA82E__INCLUDED_

#include "AST_Node.hpp"
#include <vector>
#include <memory>
#include <experimental/string_view>

namespace Parsers
{
class AST_Value;
class AST_Signal;
class AST_Source;
class AST_Module;
class AST_ScanRegister;
class AST_Identifier;
class AST_ScalarIdentifier;
class AST_VectorIdentifier;

//! Abstract Syntax Tree built when parsing some test network description
//!
class AST final
{
  // ---------------- Public Methods
  //
  public:
  ~AST();
  AST();

  AST_Signal*           Create_Signal           (std::experimental::string_view number);
  AST_Signal*           Create_Signal           (AST_Identifier* portName);
  AST_Signal*           Create_Signal           (std::vector<AST_ScalarIdentifier*>&& scope, AST_Identifier* portName);
  AST_Source*           Create_Source           (Kind kind, AST_Signal* signal);
  AST_Source*           Create_Source           (Kind kind, std::vector<AST_Signal*>&& signals);
  AST_Module*           Create_Module           (AST_ScalarIdentifier* identifier, std::vector<AST_Node*>&& children);
  AST_ScanRegister*     Create_ScanRegister     (AST_VectorIdentifier* identifier, std::vector<AST_Node*>&& children);
  AST_ScalarIdentifier* Create_ScalarIdentifier (std::experimental::string_view name);
  AST_VectorIdentifier* Create_VectorIdentifier (std::experimental::string_view name,
                                                 std::experimental::string_view leftIndex,
                                                 std::experimental::string_view rightIndex = "");
  AST_Value*            Create_Value            (Kind kind, std::experimental::string_view valueExpression);

  //! Returns "top" module node
  //!
  AST_Module* TopModule();

  // ---------------- Private Methods
  //
  template<typename T, typename... TArgs> T* Create_Node (TArgs&& ... args)
  {
    auto node    = std::make_unique<T>(std::forward<TArgs>(args) ...);
    auto pointer = node.get();

    m_nodes.emplace_back(std::move(node));
    return pointer;
  }

  // ---------------- Private Fields
  //
  private:
  std::vector<std::unique_ptr<AST_Module>>  m_modulesNodes;
  std::vector<std::unique_ptr<AST_Node>>    m_nodes;
};
//
//  End of AST class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_H__C18638A1_3F91_4757_EBB6_C119CFFA82E__INCLUDED_

//===========================================================================
// End of AST.hpp
//===========================================================================
