//===========================================================================
//                           AST.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST.hpp
//!
//! Declares AST class
//!
//===========================================================================

#ifndef AST_H__C18638A1_3F91_4757_EBB6_C119CFFA82E__INCLUDED_
  #define AST_H__C18638A1_3F91_4757_EBB6_C119CFFA82E__INCLUDED_

#include "AST_Node.hpp"
#include "AST_Network.hpp"
#include <vector>
#include <memory>
#include <experimental/string_view>

namespace Parsers
{
class AST_Attribute;
class AST_Identifier;
class AST_Instance;
class AST_ModuleIdentifier;
class AST_Module;
class AST_Namespace;
class AST_Parameter;
class AST_ParameterRef;
class AST_Port;
class AST_ScalarIdentifier;
class AST_ScanInterface;
class AST_ScanMux;
class AST_ScanMuxSelection;
class AST_ScanRegister;
class AST_Signal;
class AST_SimpleNode;
class AST_Source;
class AST_String;
class AST_Value;
class AST_VectorIdentifier;

//! Abstract Syntax Tree built when parsing some test network description
//!
class AST final
{
  // ---------------- Public Methods
  //
  public:
  ~AST();
  AST();

  AST_Namespace*          Create_Namespace          (std::string&& name);
  AST_Attribute*          Create_Attribute          (std::string&& name);
  AST_Attribute*          Create_Attribute          (std::string&& name, std::string&& numbersValue);
  AST_Attribute*          Create_Attribute          (std::string&& name, std::vector<AST_SimpleNode*>&& stringsOrRefsValue);
  AST_Parameter*          Create_LocalParameter     (std::string&& name, std::string&& numbersValue);
  AST_Parameter*          Create_LocalParameter     (std::string&& name, std::vector<AST_SimpleNode*>&& stringsOrRefsValue);
  AST_Parameter*          Create_Parameter          (std::string&& name, std::string&& numbersValue);
  AST_Parameter*          Create_Parameter          (std::string&& name, std::vector<AST_SimpleNode*>&& stringsOrRefsValue);
  AST_ParameterRef*       Create_ParameterRef       (std::string&& name);
  AST_String*             Create_String             (std::string&& content);
  AST_Signal*             Create_Signal             (std::experimental::string_view number);
  AST_Signal*             Create_Signal             (AST_Identifier* portName);
  AST_Signal*             Create_Signal             (std::vector<AST_ScalarIdentifier*>&& scope, AST_Identifier* portName);
  AST_ScanMux*            Create_ScanMux            (AST_VectorIdentifier*                identifier,
                                                     std::vector<Parsers::AST_Signal*>&&  selectors,
                                                     std::vector<AST_ScanMuxSelection*>&& scanMuxSelection);
  AST_ScanMuxSelection*   Create_ScanMuxSelection   (std::vector<std::string>&& selectionValues, std::vector<Parsers::AST_Signal*>&& selectedSignals);
  AST_Source*             Create_Source             (Kind kind, AST_Signal* signal);
  AST_Source*             Create_Source             (Kind kind, std::vector<AST_Signal*>&& signals);
  AST_Port*               Create_Port               (Kind kind, AST_VectorIdentifier* identifier);
  AST_Port*               Create_Port               (Kind kind, AST_VectorIdentifier* identifier, std::vector<AST_Node*>&& children);
  AST_Instance*           Create_Instance           (const AST_ScalarIdentifier* instanceIdentifier,
                                                     const AST_ModuleIdentifier* moduleIdentifier);
  AST_Instance*           Create_Instance           (const AST_ScalarIdentifier* instanceIdentifier,
                                                     const AST_ModuleIdentifier* moduleIdentifier,
                                                     std::vector<AST_Node*>&&    children);
  AST_ModuleIdentifier*   Create_ModuleIdentifier   (const AST_Namespace* namespaceName, const AST_ScalarIdentifier* moduleName);
  AST_Module*             Create_Module             (const AST_ScalarIdentifier* identifier, std::vector<AST_Node*>&& children);
  AST_ScanInterface*      Create_ScanInterface      (const AST_ScalarIdentifier* identifier, std::vector<AST_Node*>&& children);
  AST_ScanRegister*       Create_ScanRegister       (AST_VectorIdentifier* identifier, std::vector<AST_Node*>&& children);
  AST_ScalarIdentifier*   Create_ScalarIdentifier   (std::experimental::string_view name);
  AST_VectorIdentifier*   Create_VectorIdentifier   (std::experimental::string_view name,
                                                     std::experimental::string_view leftIndex,
                                                     std::experimental::string_view rightIndex = "");
  AST_Value*              Create_Value              (Kind kind, std::experimental::string_view valueExpression);


  void SaveInstanceDefaultNamespace ()           { m_savedInstancesDefaultNamespace = m_instancesDefaultNamespace; }  //!< Saves instance default namespace (before parsing new module body)

  void SetRootNamespace ()                       { SetNamespace(m_rootNamespace); }                    //!< Forces following modules to be created in "root" namespace
  void SetModuleNamespace   (std::string&& name) { SetNamespace(Create_Namespace(std::move(name))); }  //!< Forces following modules to be created in specified namespace
  void SetInstanceNamespace ()                   { m_instancesDefaultNamespace = m_rootNamespace; }    //!< Forces following instances to refer to modules in "root" namespace
  void SetInstanceNamespace (std::string&& name) { m_instancesDefaultNamespace = Create_Namespace(std::move(name)); }  //!< Forces following instances to refer to modules in specified namespace

  AST_Network* Network() { return &m_network; } //!< Returns test network

  //! Returns "top" module node
  //!
//+  AST_Module* TopModule();

  //! Returns root namespace
  //!
  const AST_Namespace* RootNamespace() const { return m_rootNamespace; }

  //! Returns current default namespace for module instances
  //!
  const AST_Namespace* InstancesDefaultNamespace() const { return m_instancesDefaultNamespace; }

  // ---------------- Private Methods
  //
  AST_Namespace* Create_Namespace_Impl (std::string&& name);
  void           SetNamespace          (const AST_Namespace* newNamespace);

  template<typename T, typename... TArgs> T* Create_Node (TArgs&& ... args)
  {
    auto node    = std::make_unique<T>(std::forward<TArgs>(args) ...);
    auto pointer = node.get();

    m_nodes.emplace_back(std::move(node));
    return pointer;
  }

  // ---------------- Private Fields
  //
  private:
  AST_Network                                 m_network;                                  //!< Build test network
  std::vector<std::unique_ptr<AST_Module>>    m_modules;                                  //!< Managed module nodes
  std::vector<std::unique_ptr<AST_Node>>      m_nodes;                                    //!< Managed, not modules nor namespaces, nodes
  std::vector<std::unique_ptr<AST_Namespace>> m_namespaces;                               //!< Managed namespace nodes
  const AST_Namespace*                        m_rootNamespace                  = nullptr; //!< Root namespace
  const AST_Namespace*                        m_modulesNamespace               = nullptr; //!< Namespace used for following module definitions
  const AST_Namespace*                        m_savedInstancesDefaultNamespace = nullptr; //!< Saved default namespace for instance (to be restored at end of module parsing)
  const AST_Namespace*                        m_instancesDefaultNamespace      = nullptr; //!< Default namespace used for following module instantiations
};
//
//  End of AST class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers


#endif  // not defined AST_H__C18638A1_3F91_4757_EBB6_C119CFFA82E__INCLUDED_

//===========================================================================
// End of AST.hpp
//===========================================================================
