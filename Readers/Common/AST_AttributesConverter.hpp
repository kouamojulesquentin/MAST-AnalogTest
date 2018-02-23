//===========================================================================
//                           AST_AttributesConverter.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_AttributesConverter.hpp
//!
//! Declares AST_AttributesConverter class
//!
//===========================================================================

#ifndef AST_ATTRIBUTESCONVERTER_H__EB62C7DB_6E34_42DC_B49A_32ACF20D805E__INCLUDED_
  #define AST_ATTRIBUTESCONVERTER_H__EB62C7DB_6E34_42DC_B49A_32ACF20D805E__INCLUDED_

#include "AppFunctionNameAndNode.hpp"
#include <vector>

namespace mast
{
class ParentNode;
}

namespace Parsers
{
class AST_Module;
class AST_Instance;

//! Helper to convert AST attribules to some info for SystemModel
//!
class AST_AttributesConverter final
{
  // ---------------- Public Methods
  //
  public:
  ~AST_AttributesConverter() = default;
  AST_AttributesConverter()  = default;

  //! Creates associations between algorithm names and SystemModelNode
  //!
  static std::vector<mast::AppFunctionNameAndNode> CollectAlgorithms(AST_Instance*                     instance,
                                                                     AST_Module*                       module,
                                                                     std::shared_ptr<mast::ParentNode> parentNode);

  // ---------------- Private Methods
  //
  private:
  static std::vector<mast::AppFunctionNameAndNode> CollectLocalAlgorithms (AST_Instance*                     instance,
                                                                           AST_Module*                       module,
                                                                           std::shared_ptr<mast::ParentNode> parentNode);
};
//
//  End of AST_AttributesConverter class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers


#endif  // not defined AST_ATTRIBUTESCONVERTER_H__EB62C7DB_6E34_42DC_B49A_32ACF20D805E__INCLUDED_
//===========================================================================
// End of AST_AttributesConverter.hpp
//===========================================================================
