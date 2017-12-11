//===========================================================================
//                           AST_BsdlInstructionRef.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_BsdlInstructionRef.hpp
//!
//! Declares AST_BsdlInstructionRef class
//!
//===========================================================================

#ifndef AST_BSDLINSTRUCTIONREF_H__9827C9F4_A3CC_4E3E_FC86_13037EBADED3__INCLUDED_
  #define AST_BSDLINSTRUCTIONREF_H__9827C9F4_A3CC_4E3E_FC86_13037EBADED3__INCLUDED_

#include "AST_ParentNode.hpp"

#include <string>
#include <vector>

namespace Parsers
{
class AST_ScanInterfaceRef;

//! Represents a reference to a BSDL instruction
//!
class AST_BsdlInstructionRef final : public AST_ParentNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_BsdlInstructionRef() = default;
  AST_BsdlInstructionRef()  = delete;

  //! Returns BSDL instruction name
  //!
  std::string Name() const override { return m_instructionName; };

  //! Returns selected scan interfaces
  //!
  const std::vector<AST_ScanInterfaceRef*>& ScanInterfacesRef() const { return m_scanInterfacesRef; }

  // ---------------- Private Methods
  //
  private:
  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_BsdlInstructionRef)(std::string&&, std::vector<Parsers::AST_Node*>&&);

  //! Initializes AST_BsdlInstructionRef with children
  //!
  //! @param instructionName  Referred BSDL instruction name
  //! @param children         BSDL instruction reference children nodes (scan interface or active signals references)
  //!
  AST_BsdlInstructionRef(std::string&& instructionName, std::vector<AST_Node*>&& children)
    : AST_ParentNode    (Kind::BsdlInstructionRef, std::move(children))
    , m_instructionName (std::move(instructionName))
  {
    DispatchChildren();
    CleanupChildren();
  }


  //! Dispatches children to specific members
  //!
  void DispatchChildren () override;

  // ---------------- Private Fields
  //
  private:
  const std::string                  m_instructionName;   //!< Referred BSDL instruction name
  std::vector<AST_ScanInterfaceRef*> m_scanInterfacesRef; //!< Scan interfaces selected by referred instruction name
};
//
//  End of AST_BsdlInstructionRef class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers



#endif  // not defined AST_BSDLINSTRUCTIONREF_H__9827C9F4_A3CC_4E3E_FC86_13037EBADED3__INCLUDED_
//===========================================================================
// End of AST_BsdlInstructionRef.hpp
//===========================================================================
