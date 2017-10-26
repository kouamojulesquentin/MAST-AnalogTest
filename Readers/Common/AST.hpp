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

#include <vector>
#include <memory>
#include <experimental/string_view>

namespace Parsers
{

class AST_Node;
class AST_Module;
class AST_ScanRegister;

//! Abstract Syntax Tree built when parsing some test network description
//!
class AST final
{
  // ---------------- Public Methods
  //
  public:
  ~AST();
  AST();

  AST_Module*       Create_Module       (std::experimental::string_view name, std::vector<AST_Node*>&& children);
  AST_ScanRegister* Create_ScanRegister (std::experimental::string_view name,
                                         std::experimental::string_view rangeLeft  = "",
                                         std::experimental::string_view rangeRight = "");

  //! Returns "top" module node
  //!
  AST_Module* TopModule();

  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private Methods
  //
  private:

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
