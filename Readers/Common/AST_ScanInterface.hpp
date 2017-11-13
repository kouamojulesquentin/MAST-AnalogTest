//===========================================================================
//                           AST_ScanInterface.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_ScanInterface.hpp
//!
//! Declares AST_ScanInterface class
//!
//===========================================================================

#ifndef AST_SCANINTERFACE_H__8EC960C9_FA74_4ACA_2AB6_D94E99695FF4__INCLUDED_
  #define AST_SCANINTERFACE_H__8EC960C9_FA74_4ACA_2AB6_D94E99695FF4__INCLUDED_

#include "AST_ParentNode.hpp"

#include <string>
#include <vector>
namespace Parsers
{
class AST_Attribute;
class AST_Port;

//! Represents a scan interface scan interface
//!
class AST_ScanInterface final : public AST_ParentNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_ScanInterface() = default;
  AST_ScanInterface()  = delete;

  //! Visited part of the Visitor pattern
  //!
  void Accept (AST_Visitor& visitor) override;

  //! Returns instance name (in enclosing scan interface)
  //!
  std::string Name() const override;

  //! Returns isntance identifier
  //!
  const AST_ScalarIdentifier* Identifier() const { return m_identifier; };

  //! Returns scan interface attributes
  //!
  const std::vector<AST_Attribute*>& Attributes() const { return m_attributes; }

  //! Returns scan interface ports
  //!
  const std::vector<AST_Port*>& Ports() const { return m_ports; }


  // ---------------- Private Methods
  //
  private:
  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_ScanInterface)(const Parsers::AST_ScalarIdentifier*&, std::vector<AST_Node*>&&);

  //! Initializes AST_ScanInterface with children
  //!
  //! @param identifier   Scan interface name
  //! @param children     Scan interface children nodes
  //!
  AST_ScanInterface(const AST_ScalarIdentifier* identifier, std::vector<AST_Node*>&& children)
    : AST_ParentNode (Kind::ScanInterface, std::move(children))
    , m_identifier   (identifier)
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
  const AST_ScalarIdentifier* m_identifier = nullptr; //!< Scan interface name
  std::vector<AST_Attribute*> m_attributes;           //!< Scan interface attributes
  std::vector<AST_Port*>      m_ports;                //!< Scan interface ports
};
//
//  End of AST_ScanInterface class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers



#endif  // not defined AST_SCANINTERFACE_H__8EC960C9_FA74_4ACA_2AB6_D94E99695FF4__INCLUDED_
//===========================================================================
// End of AST_ScanInterface.hpp
//===========================================================================
