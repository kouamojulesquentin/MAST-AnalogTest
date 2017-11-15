//===========================================================================
//                           AST_Instance.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Instance.hpp
//!
//! Declares AST_Instance class
//!
//===========================================================================

#ifndef AST_INSTANCE_H__359986B0_3CF_4D4A_3D90_72AA6A46C10F__INCLUDED_
  #define AST_INSTANCE_H__359986B0_3CF_4D4A_3D90_72AA6A46C10F__INCLUDED_

#include "AST_ParentNode.hpp"

#include <string>
#include <vector>
namespace Parsers
{
class AST_Identifier;
class AST_Port;
class AST_Parameter;
class AST_Attribute;
class AST_ModuleIdentifier;

//! Represents a parsed test network, instiable, module
//!
class AST_Instance final : public AST_ParentNode
{
  // ---------------- Public Methods
  //
  public:
  ~AST_Instance() = default;
  AST_Instance()  = delete;

  //! Visited part of the Visitor pattern
  //!
  void Accept (AST_Visitor& visitor) override;

  //! Returns instance name (in enclosing module)
  //!
  std::string Name() const override;

  //! Returns isntance identifier
  //!
  const AST_ScalarIdentifier* InstanceIdentifier() const { return m_instanceIdentifier; };

  //! Returns module identifier
  //!
  const AST_ModuleIdentifier* ModuleIdentifier() const { return m_moduleIdentifier; };

  //! Returns instance input port(s)
  //!
  const std::vector<AST_Port*>& InputPorts() const { return m_inputPorts; }


  //! Searches for a InputPort with specified identifier
  //!
  const AST_Port* FindInputPort (const AST_Identifier* identifier) const;

  //! Returns (generic) module parameters for the instance
  //!
  const std::vector<AST_Parameter*>& Parameters() const { return m_parameters; }

  //! Returns instance attributes
  //!
  const std::vector<AST_Attribute*>& Attributes() const { return m_attributes; }


  // ---------------- Private Methods
  //
  private:
  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_Instance)(const Parsers::AST_ScalarIdentifier*&,
                                      const Parsers::AST_ModuleIdentifier*&);

  MAKE_UNIQUE_AS_FRIEND(AST_Instance)(const Parsers::AST_ScalarIdentifier*&,
                                      const Parsers::AST_ModuleIdentifier*&,
                                      std::vector<AST_Node*>&&);

  //! Initializes AST_Instance without children
  //!
  //! @param instanceIdentifier   Instance name
  //! @param moduleIdentifier     Identifies module to instantiate
  //!
  AST_Instance(const AST_ScalarIdentifier* instanceIdentifier,
               const AST_ModuleIdentifier* moduleIdentifier)
    : AST_ParentNode       (Kind::Instance)
    , m_instanceIdentifier (instanceIdentifier)
    , m_moduleIdentifier   (moduleIdentifier)
  {
    DispatchChildren();
    CleanupChildren();
  }

  //! Initializes AST_Instance with children
  //!
  //! @param instanceIdentifier   Instance name
  //! @param moduleIdentifier     Identifies module to instantiate
  //! @param children             Module children nodes
  //!
  AST_Instance(const AST_ScalarIdentifier* instanceIdentifier,
               const AST_ModuleIdentifier* moduleIdentifier,
               std::vector<AST_Node*>&&    children)
    : AST_ParentNode       (Kind::Instance, std::move(children))
    , m_instanceIdentifier (instanceIdentifier)
    , m_moduleIdentifier   (moduleIdentifier)
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
  const AST_ScalarIdentifier* m_instanceIdentifier = nullptr; //!< Instance name
  const AST_ModuleIdentifier* m_moduleIdentifier   = nullptr; //!< Identifies module to instantiate
  std::vector<AST_Parameter*> m_parameters;                   //!< Parameters for module instantiation
  std::vector<AST_Attribute*> m_attributes;                   //!< Instance attributes
  std::vector<AST_Port*>      m_inputPorts;                   //!< Instance input ports (connections in enclosing module)
};
//
//  End of AST_Instance class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers


#endif  // not defined AST_INSTANCE_H__359986B0_3CF_4D4A_3D90_72AA6A46C10F__INCLUDED_
//===========================================================================
// End of AST_Instance.hpp
//===========================================================================
