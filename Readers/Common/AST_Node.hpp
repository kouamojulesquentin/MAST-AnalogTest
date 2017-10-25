//===========================================================================
//                           AST_Node.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Node.hpp
//!
//! Declares AST_Node class
//!
//===========================================================================


#ifndef AST_NODE_H__E1535643_2BE2_4F0C_92B8_43667E41845B__INCLUDED_
  #define AST_NODE_H__E1535643_2BE2_4F0C_92B8_43667E41845B__INCLUDED_

namespace Parsers
{
class AST_Visitor;

//! Base, abstract, class for AST specialized nodes
//!
class AST_Node
{
  // ---------------- Public Methods
  //
  public:
  virtual ~AST_Node() = default;

  virtual void Accept (AST_Visitor& visitor) = 0; //!< Visited part of the Visitor pattern

//+  virtual std::experimental::string_view TypeName() const = 0; //!< Returns readable type name


  // ---------------- Protected Methods
  //
  protected:

  AST_Node() = default;

  // ---------------- Private Methods
  //
  private:

  // ---------------- Private Fields
  //
  private:
};
//
//  End of AST_Node class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers

#define MAKE_UNIQUE_AS_FRIEND(x) friend std::unique_ptr<x> std::make_unique<x>



#endif  // not defined AST_NODE_H__E1535643_2BE2_4F0C_92B8_43667E41845B__INCLUDED_

//===========================================================================
// End of AST_Node.hpp
//===========================================================================
