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
#include <memory>
#include <string>
#include <set>

namespace Parsers
{
class AST_AccessLink;
class AST_Instance;
class AST_ScanMux;
class AST_ScanInterface;
class AST_ScanRegister;
class AST_Port;
class AST_Parameter;
class AST_Attribute;
class AST_Builder;

//! Represents a Module
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

  //! Returns module identifier
  //!
  const AST_ScalarIdentifier* Identifier() const { return m_identifier; };

  //! Changes identifier
  //!
  //! @note   This operation is intended to be used during unification process
  void  ChangeIdentifier (const AST_ScalarIdentifier* newIdentifier)
  {
    m_originalIdentifier = m_identifier;
    m_identifier         = newIdentifier;
  };


  //! Returns module name
  //!
  std::string Name() const override { return m_identifier->AsText(); }

  //! Returns (generic) module parameters
  //!
  const std::vector<AST_Parameter*>& Parameters() const { return m_parameters; }

  //! Returns module local parameters
  //!
  const std::vector<AST_Parameter*>& LocalParameters() const { return m_localParameters; }

  //! Returns module attributes
  //!
  const std::vector<AST_Attribute*>& Attributes() const { return m_attributes; }

  //! Returns, if exists attribute with specified name
  //!
  AST_Attribute* Attribute(std::experimental::string_view attributeName);

  //! Returns module ScanInterface(s)
  //!
  const std::vector<AST_ScanInterface*>& ScanInterfaces() const { return m_scanInterfaces; }


  //! Returns module ScanMuxes
  //!
  const std::vector<AST_ScanMux*>& ScanMuxes() const { return m_scanMuxes; }

  //! Returns module ScanRegister(s)
  //!
  const std::vector<AST_ScanRegister*>& ScanRegisters() const { return m_scanRegisters; }

  //! Returns module DataInPort(s)
  //!
  const std::vector<AST_Port*>& DataInPorts() const { return m_dataInPorts; }

  //! Returns module DataOutPort(s)
  //!
  const std::vector<AST_Port*>& DataOutPorts() const { return m_dataOutPorts; }

  //! Returns module ScanInPort(s)
  //!
  const std::vector<AST_Port*>& ScanInPorts() const { return m_scanInPorts; }

  //! Returns module ScanOutPort(s)
  //!
  const std::vector<AST_Port*>& ScanOutPorts() const { return m_scanOutPorts; }

  //! Returns module Instances
  //!
  const std::vector<AST_Instance*>& Instances() const { return m_instances; }

  //! Returns AccessLink or nullptr when none is defined
  //!
  AST_AccessLink* AccessLink() const { return m_accessLink; }

  //! Returns whether module defines an AccessLink
  //!
  bool HasAccessLink() const { return m_accessLink != nullptr; }

  //! Returns module ScanInterface with specified identifier
  //!
  AST_ScanInterface* FindScanInterface(const AST_Identifier* identifier);

  //! Returns module ScanInterface with specified identifier
  //!
  AST_ScanInterface* FindScanInterface(std::experimental::string_view interfaceName);

  //! Searches for a DataOutputPort with specified identifier
  //!
  AST_Port* FindDataOutPort (const AST_Identifier* identifier);

  //! Searches for a ScanMux with specified identifier
  //!
  AST_ScanMux* FindScanMux (const AST_Identifier* identifier);

  //! Searches for a ScanInputPort with specified identifier
  //!
  AST_Port* FindScanInPort (const AST_Identifier* identifier);

  //! Searches for a ScanOutputPort with specified identifier
  //!
  AST_Port* FindScanOutPort (const AST_Identifier* identifier);

  //! Searches for a ScanRegister with specified identifier
  //!
  AST_ScanRegister* FindScanRegister (const AST_Identifier* identifier);

  //! Searches for a Instance with specified identifier
  //!
  AST_Instance* FindInstance (const AST_Identifier* identifier);

  //! Uniquifies as the top module
  //!
  void UniquifyAsTop (AST_Builder& astBuilder);

  //! Uniquifies module using parameters overrides
  //!
  AST_Module* Uniquify (AST_Builder& astBuilder, const std::vector<AST_Parameter*>& parameters);

  //! Uniquifies module instances
  //!
  void UniquifyInstances (AST_Builder& astBuilder);

  //! Marks an ScanOutPort
  //!
  //! @note Marking is caller responsibility for any kind of usage
  void  MarkScanOutPort (const AST_Port* port) { m_markedScanOutPorts.insert(port); };

  //! Tells whether an ScanOutPort is marked
  //!
  //! @note Marking is caller responsibility for any kind of usage
  bool IsScanOutPortMarked (const AST_Port* port) { return m_markedScanOutPorts.find(port) != m_markedScanOutPorts.cend(); };

  //! Clears system that try to detect circular modules dependencies
  //!
  static void ResetCircularDependencyTracker() { sm_clonedModules.clear(); }

  // ---------------- Private Methods
  //
  private:
  friend class AST;   // This is AST that manages construction/destruction of AST nodes (it uses make_unit<T>() to create nodes)
  MAKE_UNIQUE_AS_FRIEND(AST_Module)(const Parsers::AST_ScalarIdentifier*&, std::vector<AST_Node*>&&);
  MAKE_UNIQUE_AS_FRIEND(AST_Module)(const Parsers::AST_Module&); // For clones

  AST_Module(const AST_ScalarIdentifier* identifier, std::vector<AST_Node*>&& children)
    : AST_ParentNode (Kind::Module, std::move(children))
    , m_identifier   (identifier)
  {
    DispatchChildren();
    CleanupChildren();
  }

  AST_Module(const AST_Module&) = default;

  //! Dispatches children to specific members
  //!
  void DispatchChildren () override;

  void JoinParameters ();

  void Resolve ();

  void Uniquify_impl              (AST_Builder& astBuilder, const std::vector<AST_Parameter*>& parameters);
  void UniquifyOrUpdateParameters (AST_Builder& astBuilder, const std::vector<AST_Parameter*>& parameters);

  // ---------------- Private Fields
  //
  private:
  const AST_ScalarIdentifier*     m_identifier         = nullptr; //!< Module name (change during unification)
  const AST_ScalarIdentifier*     m_originalIdentifier = nullptr; //!< Module name prior to unification
  AST_AccessLink*                 m_accessLink         = nullptr; //!< AccessLink: for top module (only one per network)
  AST_Module*                     m_parentModule       = nullptr; //!< When uniquified, tells hierarchical, direct, parent module this one is instanciated (using an Instance)
  std::vector<AST_Parameter*>     m_parameters;                   //!< Generic module parameters
  std::vector<AST_Parameter*>     m_localParameters;              //!< Module local parameters
  std::vector<AST_ScanInterface*> m_scanInterfaces;               //!< Scan interfaces
  std::vector<AST_Port*>          m_dataInPorts;                  //!< Data input port(s)
  std::vector<AST_Port*>          m_dataOutPorts;                 //!< Data output port(s)
  std::vector<AST_Port*>          m_scanInPorts;                  //!< Scan input port(s)
  std::vector<AST_Port*>          m_scanOutPorts;                 //!< Scan output port(s)
  std::set<const AST_Port*>       m_markedScanOutPorts;           //!< Set of "marked" ScanOutPort
  std::vector<AST_Attribute*>     m_attributes;                   //!< Module attributes
  std::vector<AST_ScanRegister*>  m_scanRegisters;                //!< Scan registers in module
  std::vector<AST_ScanMux*>       m_scanMuxes;                    //!< Scan muxes in module
  std::vector<AST_Instance*>      m_instances;                    //!< Instances in module

  static std::vector<const AST_Module*> sm_clonedModules; //!< To detect circular dependencies in module
};
//
//  End of AST_Module class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_MODULE_H__A9A400A_9537_4176_349E_822BAF7E4956__INCLUDED_

//===========================================================================
// End of AST_Module.hpp
//===========================================================================
