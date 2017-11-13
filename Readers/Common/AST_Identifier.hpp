//===========================================================================
//                           AST_Identifier.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Identifier.hpp
//!
//! Declares AST_Identifier class
//!
//===========================================================================

#ifndef AST_IDENTIFIER_H__2904C7ED_BEBD_4BB4_9BBD_F627A1EF620F__INCLUDED_
  #define AST_IDENTIFIER_H__2904C7ED_BEBD_4BB4_9BBD_F627A1EF620F__INCLUDED_

#include "AST_SimpleNode.hpp"
#include <string>

namespace Parsers
{
//! Abstract identifier
//!
class AST_Identifier : public AST_SimpleNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_Identifier() = default;
  AST_Identifier()  = delete;

  // ---------------- Protected Methods
  //
  protected:
  AST_Identifier(Kind kind)
    : AST_SimpleNode (kind)
  {
  }
};
//
//  End of AST_Identifier class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers



#endif  // not defined AST_IDENTIFIER_H__2904C7ED_BEBD_4BB4_9BBD_F627A1EF620F__INCLUDED_
//===========================================================================
// End of AST_Identifier.hpp
//===========================================================================
