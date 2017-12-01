//===========================================================================
//                           AST.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST.cpp
//!
//! Implements class AST
//!
//===========================================================================

#include "AST.hpp"
#include "AST_AccessLink.hpp"
#include "AST_Attribute.hpp"
#include "AST_FileRef.hpp"
#include "AST_Instance.hpp"
#include "AST_ModuleIdentifier.hpp"
#include "AST_Module.hpp"
#include "AST_Namespace.hpp"
#include "AST_Parameter.hpp"
#include "AST_ParameterRef.hpp"
#include "AST_Port.hpp"
#include "AST_ScalarIdentifier.hpp"
#include "AST_ScanInterface.hpp"
#include "AST_ScanRegister.hpp"
#include "AST_ScanMux.hpp"
#include "AST_ScanMuxSelection.hpp"
#include "AST_Signal.hpp"
#include "AST_Source.hpp"
#include "AST_String.hpp"
#include "AST_Value.hpp"
#include "AST_VectorIdentifier.hpp"

#include "Utility.hpp"
#include <algorithm>

using std::vector;
using std::string;
using std::experimental::string_view;
using std::make_unique;

using namespace std::string_literals;

using namespace Parsers;

//! Releases constructed AST nodes
//!
AST::~AST ()
{
}


//! Initializes AST
//!
AST::AST ()
  : m_rootNamespace             (Create_Namespace_Impl(""s))
  , m_modulesNamespace          (m_rootNamespace)
  , m_instancesDefaultNamespace (m_rootNamespace)
{
}


//! Clones an instance
//!
AST_Instance* AST::Clone_Instance (const AST_Instance* instance)
{
  return Clone_Node(instance);
}
//
//  End of: AST::Clone_Instance
//---------------------------------------------------------------------------


//! Clones a module
//!
AST_Module* AST::Clone_Module (const AST_Module* module)
{
  auto node         = make_unique<AST_Module>(*module);
  auto clonedModule = node.get();

  auto newIdentifier = Create_UniquifiedIdentifier(module->Identifier());
  clonedModule->ChangeIdentifier(newIdentifier);

  m_modules.emplace_back(std::move(node));
  m_network.AddModule(m_modulesNamespace, clonedModule);
  return clonedModule;
}
//
//  End of: AST::Clone_Module
//---------------------------------------------------------------------------


//! Clones a Port
//!
AST_Port* AST::Clone_Port (const AST_Port* port)
{
  return Clone_Node(port);
}
//
//  End of: AST::Clone_Port
//---------------------------------------------------------------------------


//! Clones a scan multiplexer
//!
AST_ScanMux* AST::Clone_ScanMux (const AST_ScanMux* scanMux)
{
  return Clone_Node(scanMux);
}
//
//  End of: AST::Clone_ScanMux
//---------------------------------------------------------------------------


//! Clones a scan register
//!
AST_ScanRegister* AST::Clone_ScanRegister (const AST_ScanRegister* scanRegister)
{
  return Clone_Node(scanRegister);
}
//
//  End of: AST::Clone_ScanRegister
//---------------------------------------------------------------------------


//! Creates a Create_AccessLink node
//!
//! @param identifier   Access Link name
//! @param type         Type of Access Link
//! @param children     Access Link children nodes
//!
AST_AccessLink* AST::Create_AccessLink (const AST_ScalarIdentifier* identifier, AccessLinkType type, vector<AST_Node*>&& children)
{
  CHECK_PARAMETER_NOT_NULL(identifier, "Access link identifier must not be nullptr");

  return Create_Node<AST_AccessLink>(identifier, type, std::move(children));
}
//
//  End of: AST::Create_AccessLink
//---------------------------------------------------------------------------


//! Creates a Create_AccessLink node
//!
//! @param identifier   Access Link name
//! @param genericId    Generic access link identifier
//!
AST_AccessLink* AST::Create_AccessLink (const AST_ScalarIdentifier* identifier, const AST_ScalarIdentifier* genericId)
{
  CHECK_PARAMETER_NOT_NULL(identifier, "Access link identifier must not be nullptr");
  CHECK_PARAMETER_NOT_NULL(genericId,  "Access link generic identifier must not be nullptr");

  return Create_Node<AST_AccessLink>(identifier, genericId);
}
//
//  End of: AST::Create_AccessLink
//---------------------------------------------------------------------------


//! Creates an AST_Attribute node
//!
//! @param name   Attribute name
//!
AST_Attribute* AST::Create_Attribute (string&& name)
{
  return Create_Node<AST_Attribute>(std::move(name));
}
//
//  End of: AST::Create_Attribute
//---------------------------------------------------------------------------


//! Creates an AST_Attribute node
//!
//! @param name           Attribute name
//! @param numbersValue   Numbers that define parameter value
//!
AST_Attribute* AST::Create_Attribute (string&& name, std::string&& numbersValue)
{
  return Create_Node<AST_Attribute>(std::move(name), std::move(numbersValue));
}
//
//  End of: AST::Create_Attribute
//---------------------------------------------------------------------------


//! Creates an AST_Attribute node
//!
//! @param name                 Attribute name
//! @param stringsOrRefsValue   Strings and/or parameter ref that define parameter value
//!                             Should be AST_String and/or AST_ParameterRef
//!
AST_Attribute* AST::Create_Attribute (string&& name, std::vector<AST_SimpleNode*>&& stringsOrRefsValue)
{
  return Create_Node<AST_Attribute>(std::move(name), std::move(stringsOrRefsValue));
}
//
//  End of: AST::Create_Attribute
//---------------------------------------------------------------------------


//! Creates or returns existing AST_FileRef node
//!
//! @param kind   Kind of file
//! @param name   File name or path
//!
AST_FileRef* AST::Create_FileRef (Kind kind, string&& name)
{
  return Create_Node<AST_FileRef>(kind, std::move(name));
}
//
//  End of: AST::Create_FileRef
//---------------------------------------------------------------------------


//! Creates an AST_Instance node
//!
//! @param instanceIdentifier   Instance name
//! @param moduleIdentifier     Identifies module to instantiate
//!
AST_Instance* AST::Create_Instance (const AST_ScalarIdentifier* instanceIdentifier, const AST_ModuleIdentifier* moduleIdentifier)
{
  return Create_Node<AST_Instance>(instanceIdentifier, moduleIdentifier);
}
//
//  End of: AST::Create_Instance
//---------------------------------------------------------------------------


//! Creates an AST_Instance node
//!
//! @param instanceIdentifier   Instance name
//! @param moduleIdentifier     Identifies module to instantiate
//! @param children             Module children nodes
//!
AST_Instance* AST::Create_Instance (const AST_ScalarIdentifier* instanceIdentifier,
                                    const AST_ModuleIdentifier* moduleIdentifier,
                                    std::vector<AST_Node*>&&    children)
{
  return Create_Node<AST_Instance>(instanceIdentifier, moduleIdentifier, std::move(children));
}
//
//  End of: AST::Create_Instance
//---------------------------------------------------------------------------




//! Creates an AST_Parameter node for local parameter
//!
//! @param name           Local parameter name
//! @param numbersValue   Numbers that define parameter value
//!
AST_Parameter* AST::Create_LocalParameter (string&& name, string&& numbersValue)
{
  auto kind = Kind::LocalParameter;
  return Create_Node<AST_Parameter>(kind, std::move(name), std::move(numbersValue));
}
//
//  End of: AST::Create_LocalParameter
//---------------------------------------------------------------------------


//! Creates an AST_Parameter node for local parameter
//!
//! @param name               Parameter name
//! @param stringsOrRefsValue Strings and/or parameter ref that define parameter value
//!                           Should be AST_String and AST_ParameterRef
//!
AST_Parameter* AST::Create_LocalParameter (string&& name, vector<AST_SimpleNode*>&& stringsOrRefsValue)
{
  auto kind = Kind::LocalParameter;
  return Create_Node<AST_Parameter>(kind, std::move(name), std::move(stringsOrRefsValue));
}
//
//  End of: AST::Create_LocalParameter
//---------------------------------------------------------------------------


//! Creates a AST_Module node
//!
//! @param identifier   Module name
//! @param children     Module children nodes
//!
AST_Module* AST::Create_Module (const AST_ScalarIdentifier* identifier, vector<AST_Node*>&& children)
{
  CHECK_PARAMETER_NOT_NULL(identifier, "identifier must not be nullptr");

  auto node   = make_unique<AST_Module>(identifier, std::move(children));
  auto module = node.get();

  // ---------------- Restore "UseNameSpace" before module
  //
  if (m_savedInstancesDefaultNamespace != nullptr)
  {
    m_instancesDefaultNamespace      = m_savedInstancesDefaultNamespace;
    m_savedInstancesDefaultNamespace = nullptr;
  }

  m_modules.emplace_back(std::move(node));
  m_network.AddModule(m_modulesNamespace, module);
  return module;
}
//
//  End of: AST::Create_Module
//---------------------------------------------------------------------------


//! Creates an AST_ModuleIdentifier node
//!
//! @param namespaceName  Namespace of module definition
//! @param moduleName     Module name in the namespace
//!
AST_ModuleIdentifier* AST::Create_ModuleIdentifier (const AST_Namespace* namespaceName, const AST_ScalarIdentifier* moduleName)
{
  return Create_Node<AST_ModuleIdentifier>(namespaceName, moduleName);
}
//
//  End of: AST::Create_ModuleIdentifier
//---------------------------------------------------------------------------


//! Creates or returns existing AST_Namespace node
//!
//! @param name   Namespace name
//!
AST_Namespace* AST::Create_Namespace (string&& name)
{
  auto predicate = [&name](const auto& node) { return node->Name() == name; };

  auto pos  = std::find_if(m_namespaces.cbegin(), m_namespaces.cend(), predicate);
  auto node = (pos == m_namespaces.cend()) ? Create_Namespace_Impl(std::move(name))
                                           : pos->get();

  return node;
}
//
//  End of: AST::Create_Namespace
//---------------------------------------------------------------------------


//! Creates an AST_Namespace node
//!
//! @param name   Namespace name
//!
AST_Namespace* AST::Create_Namespace_Impl (string&& name)
{
  auto node    = make_unique<AST_Namespace>(std::move(name));
  auto pointer = node.get();

  m_namespaces.emplace_back(std::move(node));

  return pointer;
}
//
//  End of: AST::Create_Namespace
//---------------------------------------------------------------------------


//! Creates an AST_Parameter node for local parameter
//!
//! @param name           Parameter name
//! @param numbersValue   Numbers that define parameter value
//!
AST_Parameter* AST::Create_Parameter (string&& name, string&& numbersValue)
{
  auto kind = Kind::Parameter;
  return Create_Node<AST_Parameter>(kind, std::move(name), std::move(numbersValue));
}
//
//  End of: AST::Create_Parameter
//---------------------------------------------------------------------------


//! Creates an AST_Parameter node for local parameter
//!
//! @param name               Parameter name
//! @param stringsOrRefsValue Strings and/or parameter ref that define parameter value
//!                           Should be AST_String and/or AST_ParameterRef
//!
AST_Parameter* AST::Create_Parameter (string&& name, vector<AST_SimpleNode*>&& stringsOrRefsValue)
{
  auto kind = Kind::Parameter;
  return Create_Node<AST_Parameter>(kind, std::move(name), std::move(stringsOrRefsValue));
}
//
//  End of: AST::Create_Parameter
//---------------------------------------------------------------------------


//! Creates an AST_ParameterRef node
//!
//! @param name   Refered parameter name
//!
AST_ParameterRef* AST::Create_ParameterRef (string&& name)
{
  return Create_Node<AST_ParameterRef>(std::move(name));
}
//
//  End of: AST::Create_ParameterRef
//---------------------------------------------------------------------------




//! Creates an AST_Port node
//!
//! @param kind         Kind of port
//! @param identifier   Port identifier
//!
//! @return Created AST_Port
//!
AST_Port* AST::Create_Port (Kind kind, AST_VectorIdentifier* identifier)
{
  return Create_Node<AST_Port>(kind, identifier);
}
//
//  End of: AST::Create_Port
//---------------------------------------------------------------------------


//! Creates an AST_Port node
//!
//! @param kind         Kind of port
//! @param identifier   Port identifier
//! @param children     Port children nodes
//!
//! @return Created AST_Port
//!
AST_Port* AST::Create_Port (Kind kind, AST_VectorIdentifier* identifier, vector<AST_Node*>&& children)
{
  return Create_Node<AST_Port>(kind, identifier, std::move(children));
}
//
//  End of: AST::Create_Port
//---------------------------------------------------------------------------


//! Creates an AST_ScalarIdentifier node
//!
//! @param name Identifier
//!
AST_ScalarIdentifier* AST::Create_ScalarIdentifier (std::string&& name)
{
  return Create_Node<AST_ScalarIdentifier>(std::move(name));
}
//
//  End of: AST::Create_ScalarIdentifier
//---------------------------------------------------------------------------


//! Creates an Create_ScanMux node
//!
//! @param identifier           ScanMux identifier
//! @param selectors            Selection signals that are used to drive the ScanMux
//! @param scanMuxSelections    Selections definition i.e. which value(s) select which signal(s)
//!
AST_ScanMux* AST::Create_ScanMux (AST_VectorIdentifier*           identifier,
                                  vector<Parsers::AST_Signal*>&&  selectors,
                                  vector<AST_ScanMuxSelection*>&& scanMuxSelections)
{
  return Create_Node<AST_ScanMux>(identifier, std::move(selectors), std::move(scanMuxSelections));
}
//
//  End of: AST::Create_ScanMux
//---------------------------------------------------------------------------


//! Creates a Create_ScanInterface node
//!
//! @param identifier   Scan interface name
//! @param children     Scan interface children nodes
//!
AST_ScanInterface* AST::Create_ScanInterface (const AST_ScalarIdentifier* identifier, vector<AST_Node*>&& children)
{
  CHECK_PARAMETER_NOT_NULL(identifier, "Scan interface identifier must not be nullptr");

  return Create_Node<AST_ScanInterface>(identifier, std::move(children));
}
//
//  End of: AST::Create_ScanInterface
//---------------------------------------------------------------------------



//! Creates an Create_ScanMuxSelection node
//!
//! @param selectionValues  Selection values
//! @param selectedSignals  Selected signal when multiplex selector has one of selection values
//!
AST_ScanMuxSelection* AST::Create_ScanMuxSelection (vector<string>&& selectionValues, vector<Parsers::AST_Signal*>&& selectedSignals)
{
  return Create_Node<AST_ScanMuxSelection>(std::move(selectionValues), std::move(selectedSignals));
}
//
//  End of: AST::Create_ScanMuxSelection
//---------------------------------------------------------------------------



//! Creates an AST_ScanRegister node
//!
//! @param identifier   ScanRegister identifier
//! @param children     ScanRegister children nodes
//!
//! @return Created AST_ScanRegister
//!
AST_ScanRegister* AST::Create_ScanRegister (AST_VectorIdentifier* identifier, vector<AST_Node*>&& children)
{
  return Create_Node<AST_ScanRegister>(identifier, std::move(children));
}
//
//  End of: AST::Create_ScanRegister
//---------------------------------------------------------------------------

//! Creates an AST_Signal node
//!
//! @param number Signal value
//!
AST_Signal* AST::Create_Signal (string_view number)
{
  return Create_Node<AST_Signal>(number);
}
//
//  End of: AST::Create_Signal
//---------------------------------------------------------------------------


//! Creates an AST_Signal node
//!
//! @param portName   Port name
//!
AST_Signal* AST::Create_Signal (AST_Identifier* portName)
{
  return Create_Node<AST_Signal>(portName);
}
//
//  End of: AST::Create_Signal
//---------------------------------------------------------------------------


//! Creates an AST_Signal node
//!
//! @param scope      Port scope (dot separated instances names)
//! @param portName   Port name
//!
AST_Signal* AST::Create_Signal (vector<AST_ScalarIdentifier*>&& scope, AST_Identifier* portName)
{
  return Create_Node<AST_Signal>(std::move(scope), portName);
}
//
//  End of: AST::Create_Signal
//---------------------------------------------------------------------------


//! Creates an AST_Source node
//!
//! @param kind     Kind of source
//! @param signal   Source signal
//!
//! @return Created AST_Source
//!
AST_Source* AST::Create_Source (Kind kind, AST_Signal* signal)
{
  return Create_Node<AST_Source>(kind, signal);
}
//
//  End of: AST::Create_Source
//---------------------------------------------------------------------------


//! Creates an AST_Source node
//!
//! @param kind     Kind of source
//! @param signals  Source signals
//!
//! @return Created AST_Source
//!
AST_Source* AST::Create_Source (Kind kind, vector<AST_Signal*>&& signals)
{
  return Create_Node<AST_Source>(kind, std::move(signals));
}
//
//  End of: AST::Create_Source
//---------------------------------------------------------------------------


//! Creates an AST_String node
//!
//! @param content  String content
//!
AST_String* AST::Create_String (string&& content)
{
  return Create_Node<AST_String>(std::move(content));
}
//
//  End of: AST::Create_String
//---------------------------------------------------------------------------


//! Creates an identifier for a uniquified entity
//!
AST_ScalarIdentifier* AST::Create_UniquifiedIdentifier (const AST_ScalarIdentifier* identifier)
{
  auto newName = identifier->Name();

  newName.append("__uniquified__").append(std::to_string(++m_uniqueIdCounter));

  return Create_ScalarIdentifier(std::move(newName));
}
//
//  End of: AST::Create_UniquifiedIdentifier
//---------------------------------------------------------------------------


//! Creates an module identifier for a uniquified module
//!
//! @param module Uniquified module
//!
AST_ModuleIdentifier* AST::Create_UniquifiedModuleIdentifier (const AST_Module* module)
{
  auto moduleIdentifier = Create_ModuleIdentifier(m_uniquifiedNamespace, module->Identifier());
  return moduleIdentifier;
}
//
//  End of: AST::Create_UniquifiedModuleIdentifier
//---------------------------------------------------------------------------




//! Creates an AST_Identifier node
//!
//! @param name         Identifier
//! @param leftIndex    Left index
//! @param rightIndex   Right index (can be empty for single bit identifier)
//!
//! @return Created AST_VectorIdentifier
//!
AST_VectorIdentifier* AST::Create_VectorIdentifier (std::string&& name,
                                                    std::string&& leftIndex,
                                                    std::string&& rightIndex)
{
  return Create_Node<AST_VectorIdentifier>(std::move(name), std::move(leftIndex), std::move(rightIndex));
}
//
//  End of: AST::Create_VectorIdentifier
//---------------------------------------------------------------------------






//! Creates an AST_Value node
//!
//! @param kind             Kind of value
//! @param valueExpression  Value expression
//!
//! @return Created AST_Value
//!
AST_Value* AST::Create_Value (Kind kind, string_view valueExpression)
{
  return Create_Node<AST_Value>(kind, valueExpression);
}
//
//  End of: AST::Create_Value
//---------------------------------------------------------------------------



//! Changes namespace in which following modules are created
//!
//! @param newNamespace   New namespace
//!
void AST::SetNamespace (const AST_Namespace* newNamespace)
{
  m_modulesNamespace          = newNamespace;
  m_instancesDefaultNamespace = newNamespace;
}
//
//  End of: AST::SetNamespace
//---------------------------------------------------------------------------



//! Uniquifies network (starting from top module)
//!
//! @note Unification consist to have a single object representing each module instance (using their specific parameters)
//!
void AST::Uniquify ()
{
  m_uniquifiedNamespace = Create_Namespace("UniquifiedModules");
  SetNamespace(m_uniquifiedNamespace);

  auto topModule = m_network.TopModule();

  CHECK_VALUE_NOT_NULL(topModule, "Network has no \"top\" node");

  topModule->UniquifyInstances(*this);
}
//
//  End of: AST::Uniquify
//---------------------------------------------------------------------------




//===========================================================================
// End of AST.cpp
//===========================================================================
