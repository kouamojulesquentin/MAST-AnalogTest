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
#include "AST_Alias.hpp"
#include "AST_Attribute.hpp"
#include "AST_BasedNumber.hpp"
#include "AST_BsdlInstructionRef.hpp"
#include "AST_ConcatNumber.hpp"
#include "AST_EnumRef.hpp"
#include "AST_FileRef.hpp"
#include "AST_IntegerBinaryExpr.hpp"
#include "AST_IntegerUnaryExpr.hpp"
#include "AST_IntegerLiteral.hpp"
#include "AST_IntegerExprRef.hpp"
#include "AST_Instance.hpp"
#include "AST_ModuleIdentifier.hpp"
#include "AST_Module.hpp"
#include "AST_Namespace.hpp"
#include "AST_Parameter.hpp"
#include "AST_ParameterRef.hpp"
#include "AST_PlaceHolder.hpp"
#include "AST_Port.hpp"
#include "AST_ScalarIdentifier.hpp"
#include "AST_ScanInterface.hpp"
#include "AST_ScanInterfaceRef.hpp"
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
using std::tuple;
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


//! Clones an integer expression reference
//!
AST_IntegerExprRef* AST::Clone_IntegerExprRef (const AST_IntegerExprRef* integerExprRef)
{
  return Clone_Node(integerExprRef);
}
//
//  End of: AST::Clone_IntegerExprRef
//---------------------------------------------------------------------------


//! Clones an integer binary expression
//!
AST_IntegerBinaryExpr* AST::Clone_IntegerBinaryExpr (const AST_IntegerBinaryExpr* integerBinaryExpr)
{
  return Clone_Node(integerBinaryExpr);
}
//
//  End of: AST::Clone_IntegerBinaryExpr
//---------------------------------------------------------------------------


//! Clones a integer literal
//!
AST_IntegerLiteral* AST::Clone_IntegerLiteral (const AST_IntegerLiteral* integerLiteral)
{
  return Clone_Node(integerLiteral);
}
//
//  End of: AST::Clone_IntegerLiteral
//---------------------------------------------------------------------------


//! Clones a integer literal
//!
AST_IntegerUnaryExpr* AST::Clone_IntegerUnaryExpr (const AST_IntegerUnaryExpr* integerUnaryExpr)
{
  return Clone_Node(integerUnaryExpr);
}
//
//  End of: AST::Clone_IntegerUnaryExpr
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


//! Clones a Parameter
//!
AST_Parameter* AST::Clone_Parameter (const AST_Parameter* parameter)
{
  return Clone_Node(parameter);
}
//
//  End of: AST::Clone_Parameter
//---------------------------------------------------------------------------


//! Clones an alias
//!
AST_Alias* AST::Clone_Alias (const AST_Alias* alias)
{
  return Clone_Node(alias);
}
//
//  End of: AST::Clone_Port
//---------------------------------------------------------------------------


//! Clones an attribute
//!
AST_Attribute* AST::Clone_Attribute (const AST_Attribute* attribute)
{
  return Clone_Node(attribute);
}
//
//  End of: AST::Clone_Port
//---------------------------------------------------------------------------

//! Clones a "based" number
//!
AST_BasedNumber* AST::Clone_BasedNumber (const AST_BasedNumber* basedNumber)
{
  return Clone_Node(basedNumber);
}
//
//  End of: AST::Clone_BasedNumber
//---------------------------------------------------------------------------


//! Clones a concatenated number
//!
AST_ConcatNumber* AST::Clone_ConcatNumber (const AST_ConcatNumber* concatNumber)
{
  return Clone_Node(concatNumber);
}
//
//  End of: AST::Clone_ConcatNumber
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


//! Clones a signal
//!
AST_Signal* AST::Clone_Signal (const AST_Signal* signal)
{
  return Clone_Node(signal);
}
//
//  End of: AST::Clone_Signal
//---------------------------------------------------------------------------


//! Clones a vector identifier
//!
AST_VectorIdentifier* AST::Clone_VectorIdentifier (const AST_VectorIdentifier* identifier)
{
  return Clone_Node(identifier);
}
//
//  End of: AST::Clone_VectorIdentifier
//---------------------------------------------------------------------------


//! Creates a Create_Alias node
//!
//! @param aliasIdentifier    Alias identifier
//! @param signals            Aliased signal(s)
//! @param optionalChildren   Optional parts
//!
AST_Alias* AST::Create_Alias (AST_VectorIdentifier* aliasIdentifier,
                              vector<AST_Signal*>&& signals,
                              vector<AST_Node*>&&   optionalChildren)
{
  return Create_Node<AST_Alias>(aliasIdentifier, std::move(signals), std::move(optionalChildren));
}
//
//  End of: AST::Create_Alias
//---------------------------------------------------------------------------


//! Creates a Create_AccessLink node
//!
//! @param identifier           Access Link name
//! @param type                 Type of Access Link
//! @param bsdlFile             Identifies BSDL file
//! @param bsdlInstructionRef   Reference to BSDL instructions
//!
AST_AccessLink* AST::Create_AccessLink (const AST_ScalarIdentifier*       identifier,
                                        AccessLinkType                    type,
                                        AST_FileRef*                      bsdlFile,
                                        vector<AST_BsdlInstructionRef*>&& bsdlInstructionRef)
{
  CHECK_PARAMETER_NOT_NULL(identifier, "Access link identifier must not be nullptr");

  return Create_Node<AST_AccessLink>(identifier, type, bsdlFile, std::move(bsdlInstructionRef));
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
//! @param concatNumber   Concatenated numbers
//!
AST_Attribute* AST::Create_Attribute (string&& name, AST_ConcatNumber* concatNumber)
{
  return Create_Node<AST_Attribute>(std::move(name), concatNumber);
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


//! Creates an AST_BsdlInstructionRef node
//!
//! @param instructionName  Referred BSDL instruction name
//! @param children         BSDL instruction reference children nodes (scan interface or active signals references)
//!
AST_BsdlInstructionRef* AST::Create_BsdlInstructionRef (std::string&& instructionName, std::vector<AST_Node*>&& children)
{
  return Create_Node<AST_BsdlInstructionRef>(std::move(instructionName), std::move(children));
}
//
//  End of: AST::Create_BsdlInstructionRef
//---------------------------------------------------------------------------


//! Creates an AST_EnumRef node
//!
//! @param name   Refereed enum name
//!
AST_EnumRef* AST::Create_EnumRef (string&& name)
{
  return Create_Node<AST_EnumRef>(std::move(name));
}
//
//  End of: AST::Create_FileRef
//---------------------------------------------------------------------------



//! Creates an AST_FileRef node
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
//! @param concatNumber   Numbers that define parameter value
//!
AST_Parameter* AST::Create_LocalParameter (string&& name, AST_ConcatNumber* concatNumber)
{
  auto kind = Kind::LocalParameter;
  return Create_Node<AST_Parameter>(kind, std::move(name), concatNumber);
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



//! Creates an AST_IntegerBinaryExpr node
//!
AST_IntegerBinaryExpr* AST::Create_IntegerBinaryExpr (Kind kind, AST_IntegerExpr* leftOperand, AST_IntegerExpr* rightOperand)
{
  return Create_Node<AST_IntegerBinaryExpr>(kind, leftOperand, rightOperand);
}
//
//  End of: AST::Create_IntegerBinaryExpr
//---------------------------------------------------------------------------


//! Creates an AST_BasedNumber node
//!
AST_BasedNumber* AST::Create_BasedNumber (Kind kind, std::string&& baseAndDigits)
{
  return Create_Node<AST_BasedNumber>(kind, std::move(baseAndDigits));
}
//
//  End of: AST::Create_BasedNumber
//---------------------------------------------------------------------------


//! Creates an AST_ConcatNumber node empty
//!
AST_ConcatNumber* AST::Create_ConcatNumber ()
{
  return Create_Node<AST_ConcatNumber>();
}
//
//  End of: AST::Create_ConcatNumber
//---------------------------------------------------------------------------


//! Creates an AST_ConcatNumber node
//!
//! @param numbers  Concatenated numbers
//!
AST_ConcatNumber* AST::Create_ConcatNumber (vector<AST_Number*>&& numbers)
{
  return Create_Node<AST_ConcatNumber>(std::move(numbers));
}
//
//  End of: AST::Create_ConcatNumber
//---------------------------------------------------------------------------


//! Creates an AST_IntegerUnaryExpr node
//!
AST_IntegerUnaryExpr* AST::Create_IntegerUnaryExpr (Kind kind, AST_IntegerExpr* operand)
{
  return Create_Node<AST_IntegerUnaryExpr>(kind, operand);
}
//
//  End of: AST::Create_IntegerUnaryExpr
//---------------------------------------------------------------------------


//! Creates an AST_IntegerExprRef node
//!
//! @param parameterRef   Parameter reference to an integer expression
//!
AST_IntegerExprRef* AST::Create_IntegerExprRef (AST_ParameterRef* parameterRef)
{
  return Create_Node<AST_IntegerExprRef>(parameterRef);
}
//
//  End of: AST::Create_IntegerExprRef
//---------------------------------------------------------------------------



//! Creates an AST_IntegerLiteral node
//!
//! @param integerText  Text representing the, unsigned, integer value
//!
AST_IntegerLiteral* AST::Create_IntegerLiteral (std::string&& integerText)
{
  return Create_Node<AST_IntegerLiteral>(std::move(integerText));
}
//
//  End of: AST::Create_IntegerLiteral
//---------------------------------------------------------------------------


//! Creates an AST_IntegerLiteral node
//!
//! @param value  Literal value
//!
AST_IntegerLiteral* AST::Create_IntegerLiteral (uint32_t value)
{
  return Create_Node<AST_IntegerLiteral>(value);
}
//
//  End of: AST::Create_IntegerLiteral
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
//! @param concatNumber   Numbers that define parameter value
//!
AST_Parameter* AST::Create_Parameter (string&& name, AST_ConcatNumber* concatNumber)
{
  auto kind = Kind::Parameter;
  return Create_Node<AST_Parameter>(kind, std::move(name), concatNumber);
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


//! Creates a place holder node for some target kind
//!
//! @param targetKind   Kind of target wrapped node should be assigned
//! @param wrappedNode  Node to assign to the defined target
//!
//! @return Created AST_PlaceHolder
//!
#define CREATE_PLACEHOLDER(TYPE)                                                    \
AST_PlaceHolder<TYPE>* AST::Create_PlaceHolder (Kind targetKind, TYPE* wrappedNode) \
{                                                                                   \
  return Create_Node<AST_PlaceHolder<TYPE>>(targetKind, wrappedNode);               \
}

CREATE_PLACEHOLDER(AST_Value<bool>)
CREATE_PLACEHOLDER(AST_ConcatNumber)
//
//  End of: AST::AST_PlaceHolder
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


//! Creates a Create_ScanInterfaceRef node
//!
//! @param scanInterfaceNames Identifies at least one scan interface (in top module or a referenced instance)
//!
AST_ScanInterfaceRef* AST::Create_ScanInterfaceRef (vector<tuple<AST_ScalarIdentifier*, string>>&& scanInterfaceNames)
{
  CHECK_PARAMETER_NOT_EMPTY(scanInterfaceNames, "Scan interface reference must identify at least one scan interface");

  return Create_Node<AST_ScanInterfaceRef>(std::move(scanInterfaceNames));
}
//
//  End of: AST::Create_ScanInterfaceRef
//---------------------------------------------------------------------------



//! Creates an Create_ScanMuxSelection node
//!
//! @param selectionValues  Selection values
//! @param selectedSignals  Selected signal when multiplex selector has one of selection values
//!
AST_ScanMuxSelection* AST::Create_ScanMuxSelection (vector<AST_ConcatNumber*>&& selectionValues, vector<Parsers::AST_Signal*>&& selectedSignals)
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
AST_Signal* AST::Create_Signal (AST_Number* number)
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
AST_Signal* AST::Create_Signal (AST_VectorIdentifier* portName)
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
AST_Signal* AST::Create_Signal (vector<AST_ScalarIdentifier*>&& scope, AST_VectorIdentifier* portName)
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


//! Creates a plain C++ built-in type wrapper
//!
//! @param value  Actual built-in type value
//!
//! @return Created AST_Value
//!
AST_Value<bool>* AST::Create_Value (bool value)
{
  auto kind = Parsers::Kind::Value_Bool;
  return Create_Node<AST_Value<bool>>(kind, value);
}
//
//  End of: AST::Create_Value
//---------------------------------------------------------------------------



//! Creates an AST_Identifier node
//!
//! @param name         Identifier
//! @param leftIndex    Left index
//! @param rightIndex   Right index (can be empty for single bit identifier)
//!
//! @return Created AST_VectorIdentifier
//!
AST_VectorIdentifier* AST::Create_VectorIdentifier (std::string&& name, AST_IntegerExpr* leftIndex, AST_IntegerExpr* rightIndex)
{
  return Create_Node<AST_VectorIdentifier>(std::move(name), leftIndex, rightIndex);
}
//
//  End of: AST::Create_VectorIdentifier
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

  m_network.UniquifiedNamespace(m_uniquifiedNamespace);

  auto topModule = m_network.TopModule();

  CHECK_VALUE_NOT_NULL(topModule, "Network has no \"top\" node");

  topModule->UniquifyAsTop(*this);
//+  vector<AST_Parameter*> parameters;  // There is none for top module
//+  auto newTopModule = topModule->Uniquify(*this, parameters); // This is to have same parameter replacement algorithm on top as for sub-modules
//+  newTopModule->ChangeIdentifier(topModule->Identifier());  // Restore its original identifier (was changed by uniquification)
//+  m_network.SetTopModule(newTopModule); // Replace top module with uniquified one
}
//
//  End of: AST::Uniquify
//---------------------------------------------------------------------------




//===========================================================================
// End of AST.cpp
//===========================================================================
