//===========================================================================
//                           AST_Namespace.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Namespace.hpp
//!
//! Declares AST_Namespace class
//!
//===========================================================================

#ifndef AST_NAMESPACE_H__84F60A34_C2B4_4425_7387_F7CD5F8063A6__INCLUDED_
  #define AST_NAMESPACE_H__84F60A34_C2B4_4425_7387_F7CD5F8063A6__INCLUDED_


#include "AST_SimpleNode.hpp"
#include <string>

namespace Parsers
{
//! Represents a namespace (either definition or reference)
//!
class AST_Namespace final : public AST_SimpleNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_Namespace() = default;

  //! Returns namespace name
  //!
  const std::string& Name() const { return m_name; }


  //! Text representation of value
  //!
  std::string AsText() const override;

  //! Returns whether it refers to "root" namespace or not
  //!
  bool IsRoot() const { return m_name.empty(); }


  // ---------------- Private Methods
  //
  private:
  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_Namespace)();
  MAKE_UNIQUE_AS_FRIEND(AST_Namespace)(std::string&&);

  //! Initializes the namespace as "root"
  //!
  AST_Namespace()
    : AST_SimpleNode (Kind::NameSpace)
  {
  }

  //! Initializes the namespace with name
  //!
  AST_Namespace(std::string&& name)
    : AST_SimpleNode (Kind::NameSpace)
    , m_name         (name)
  {
  }

  // ---------------- Private Fields
  //
  private:
  std::string m_name; //!< Namespace name
};
//
//  End of AST_Namespace class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers



#endif  // not defined AST_NAMESPACE_H__84F60A34_C2B4_4425_7387_F7CD5F8063A6__INCLUDED_
//===========================================================================
// End of AST_Namespace.hpp
//===========================================================================
