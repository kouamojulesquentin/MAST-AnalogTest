//===========================================================================
//                           AST_FileRef.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_FileRef.hpp
//!
//! Declares AST_FileRef class
//!
//===========================================================================

#ifndef AST_FILEREF_H__A9CA32E2_5391_4885_BE9D_5C79EB592C__INCLUDED_
  #define AST_FILEREF_H__A9CA32E2_5391_4885_BE9D_5C79EB592C__INCLUDED_

#include "AST_SimpleNode.hpp"
#include <string>

namespace Parsers
{
//! Represents a reference to some file
//!
class AST_FileRef final : public AST_SimpleNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_FileRef() = default;
  AST_FileRef()  = delete;

  //! File name or path
  //!
  const std::string& Name() const { return m_name; }

  //! Text representation of parameter
  //!
  std::string AsText() const override { return m_name; }

  // ---------------- Private Methods
  //
  private:
  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_FileRef)(Parsers::Kind&, std::string&&);

  //! Initializes AST_FileRef with a number
  //!
  //! @param name  Refered parameter name
  //!
  AST_FileRef(Kind fileKind, std::string&& name)
    : AST_SimpleNode (fileKind)
    , m_name         (std::move(name))
  {
  }

  // ---------------- Private Fields
  //
  private:
  const std::string m_name; //!< Referred file name/path
};
//
//  End of AST_FileRef class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers



#endif  // not defined AST_FILEREF_H__A9CA32E2_5391_4885_BE9D_5C79EB592C__INCLUDED_
//===========================================================================
// End of AST_FileRef.hpp
//===========================================================================
