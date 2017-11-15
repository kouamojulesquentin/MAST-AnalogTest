//===========================================================================
//                           AST_ModuleIdentifier.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ModuleIdentifier.hpp
//!
//! Declares AST_ModuleIdentifier class
//!
//===========================================================================

#ifndef AST_MODULEIDENTIFIER_H__6E6F4C1F_8E8A_4321_3B9D_B4C6AA4DA37__INCLUDED_
  #define AST_MODULEIDENTIFIER_H__6E6F4C1F_8E8A_4321_3B9D_B4C6AA4DA37__INCLUDED_

#include "AST_Identifier.hpp"
#include <string>

namespace Parsers
{
class AST_Namespace;
class AST_ScalarIdentifier;

//! Represents a module identifier (used in instantiation)
//!
//! @note It is composed of a namespace reference and a scalar identifier
//!
class AST_ModuleIdentifier final : public AST_Identifier
{
  // ---------------- Public Methods
  //
  public:
  ~AST_ModuleIdentifier() = default;
  AST_ModuleIdentifier()  = delete;

  //! Text representation of module identifier
  //!
  std::string AsText() const override;

  //! Module name without namespace
  //!
  const std::string& Name() const override;

  //! Returns the namespace part
  //!
  const AST_Namespace* Namespace() const { return m_namespace; }

  //! Returns the namespace part
  //!
  const AST_ScalarIdentifier* ModuleIdentifier() const { return m_moduleIdentifier; }

  // ---------------- Private Methods
  //
  private:

  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_ModuleIdentifier)(const Parsers::AST_Namespace*&, const Parsers::AST_ScalarIdentifier*&);

  //! Initializes AST_ModuleIdentifier
  //!
  AST_ModuleIdentifier(const AST_Namespace* namespaceName, const AST_ScalarIdentifier* moduleName);

  // ---------------- Private Fields
  //
  private:
  const AST_Namespace*        m_namespace        = nullptr; //!< Namespace of module definition
  const AST_ScalarIdentifier* m_moduleIdentifier = nullptr; //!< Module name in the namespace
};
//
//  End of AST_ModuleIdentifier class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers


#endif  // not defined AST_MODULEIDENTIFIER_H__6E6F4C1F_8E8A_4321_3B9D_B4C6AA4DA37__INCLUDED_
//===========================================================================
// End of AST_ModuleIdentifier.hpp
//===========================================================================
