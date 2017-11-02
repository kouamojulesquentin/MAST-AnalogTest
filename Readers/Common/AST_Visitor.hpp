//===========================================================================
//                           AST_Visitor.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Visitor.hpp
//!
//! Declares interface AST_Visitor
//===========================================================================

#ifndef AST_VISITOR_H__9CC0A74_AFEB_40B5_DCBD_4AC52DA4840__INCLUDED_
  #define AST_VISITOR_H__9CC0A74_AFEB_40B5_DCBD_4AC52DA4840__INCLUDED_

namespace Parsers
{
class AST_Module;
class AST_Port;
class AST_ScanRegister;

//! AST visitors interface
//!
class AST_Visitor
{
  // ---------------- Public  Methods
  //
  public:
  virtual void Visit_Module       (AST_Module*       module) = 0;
  virtual void Visit_ScanRegister (AST_ScanRegister* scanRegister) = 0;
  virtual void Visit_Port         (AST_Port*         port) = 0;

  // ---------------- Protected Methods
  //
  protected:
  virtual ~AST_Visitor() = default;
  AST_Visitor() = default;
};
//
//  End of AST_Visitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined AST_VISITOR_H__9CC0A74_AFEB_40B5_DCBD_4AC52DA4840__INCLUDED_
//===========================================================================
// End of AST_Visitor.hpp
//===========================================================================
