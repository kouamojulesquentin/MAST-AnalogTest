//===========================================================================
//                           AST_Module.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Module.hpp
//!
//! Declares AST_Module class
//!
//===========================================================================

#ifndef AST_MODULE_H__A9A400A_9537_4176_349E_822BAF7E4956__INCLUDED_
  #define AST_MODULE_H__A9A400A_9537_4176_349E_822BAF7E4956__INCLUDED_

#include "AST_ParentNode.hpp"
#include "AST_ScalarIdentifier.hpp"
#include <memory>
#include <experimental/string_view>
#include <string>

namespace Parsers
{
class AST_ScanRegister;
class AST_Port;
class AST_Parameter;

//! Represents a parsed test network, instiable, module
//!
class AST_Module final : public AST_ParentNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_Module() = default;
  AST_Module()  = delete;

  //! Visited part of the Visitor pattern
  //!
  void Accept (AST_Visitor& visitor) override;

  //! Returns module name
  //!
  std::string Name() const override { return m_identifier->AsText(); }

  //! Returns (generic) module parameters
  //!
  const std::vector<AST_Parameter*>& Parameters() const { return m_parameters; }

  //! Returns module local parameters
  //!
  const std::vector<AST_Parameter*>& LocalParameters() const { return m_localParameters; }


  //! Returns scan registers defined in module
  //!
  const std::vector<AST_ScanRegister*>& ScanRegisters() const { return m_scanRegisters; }

  //! Returns module ScanInPort
  //!
  AST_Port* ScanInPort() const { return m_scanInPort; }

  //! Returns module ScanOutPort
  //!
  AST_Port* ScanOutPort() const { return m_scanOutPort; }


  // ---------------- Private Methods
  //
  private:
  friend class AST;                                                                             // This is AST that manages construction/destruction of AST nodes
  MAKE_UNIQUE_AS_FRIEND(AST_Module)(Parsers::AST_ScalarIdentifier*&, std::vector<AST_Node*>&&); // AST currently uses make_unit<T>() to create nodes

  AST_Module(AST_ScalarIdentifier* identifier, std::vector<AST_Node*>&& children)
    : AST_ParentNode (Kind::Module, std::move(children))
    , m_identifier   (identifier)
  {
    DispatchChildren();
  }

  //! Dispatches children to specific members
  //!
  void DispatchChildren () override;

  // ---------------- Private Fields
  //
  private:
  const AST_ScalarIdentifier*    m_identifier  = nullptr; //!< Module name
  AST_Port*                      m_scanInPort  = nullptr; //!< Scan input port
  AST_Port*                      m_scanOutPort = nullptr; //!< Scan output port
  std::vector<AST_Parameter*>    m_parameters;            //!< Generic module  parameters
  std::vector<AST_Parameter*>    m_localParameters;       //!< Module local parameters
  std::vector<AST_ScanRegister*> m_scanRegisters;         //!< Scan registers defined in module
};
//
//  End of AST_Module class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_MODULE_H__A9A400A_9537_4176_349E_822BAF7E4956__INCLUDED_

//===========================================================================
// End of AST_Module.hpp
//===========================================================================
