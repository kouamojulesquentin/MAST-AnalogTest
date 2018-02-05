//===========================================================================
//                           AST_Module.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Module.cpp
//!
//! Implements class AST_Module
//!
//===========================================================================

#include "AST_Module.hpp"
#include "AST_AccessLink.hpp"
#include "AST_Alias.hpp"
#include "AST_Attribute.hpp"
#include "AST_Instance.hpp"
#include "AST_Network.hpp"
#include "AST_ScanInterface.hpp"
#include "AST_ScanMux.hpp"
#include "AST_ScanRegister.hpp"
#include "AST_Parameter.hpp"
#include "AST_Port.hpp"
#include "AST_Visitor.hpp"
#include "AST_Builder.hpp"

#include <type_traits>
#include <algorithm>

using std::experimental::string_view;
using namespace Parsers;

std::vector<const AST_Module*> AST_Module::sm_clonedModules;

//! Visited part of the Visitor pattern
//!
void AST_Module::Accept (AST_Visitor& visitor)
{
  visitor.Visit_Module(this);
}



//! Returns, if exists attribute with specified name
//!
//! @param attributeName  Name of attribute to lookup
//!
AST_Attribute* AST_Module::Attribute (string_view attributeName)
{
  return FindNode(m_attributes, attributeName);
}
//
//  End of: AST_Module::Attribute
//---------------------------------------------------------------------------



//! Dispatches children to specific member (for ease of use)
//!
void AST_Module::DispatchChildren ()
{
  for (auto& child : UndispatchedChildren())
  {
    if (child != nullptr)
    {
      switch (child->GetKind())
      {
        case Parsers::Kind::Attribute:      AST_ParentNode::AppendChild(child, m_attributes);      break;
        case Parsers::Kind::LocalParameter: AST_ParentNode::AppendChild(child, m_localParameters); break;
        case Parsers::Kind::Parameter:      AST_ParentNode::AppendChild(child, m_parameters);      break;
        case Parsers::Kind::ScanInterface:  AST_ParentNode::AppendChild(child, m_scanInterfaces);  break;
        case Parsers::Kind::ScanInPort:     AST_ParentNode::AppendChild(child, m_scanInPorts);     break;
        case Parsers::Kind::ScanOutPort:    AST_ParentNode::AppendChild(child, m_scanOutPorts);    break;
        case Parsers::Kind::ScanMux:        AST_ParentNode::AppendChild(child, m_scanMuxes);       break;
        case Parsers::Kind::ScanRegister:   AST_ParentNode::AppendChild(child, m_scanRegisters);   break;
        case Parsers::Kind::DataInPort:     AST_ParentNode::AppendChild(child, m_dataInPorts);     break;
        case Parsers::Kind::DataOutPort:    AST_ParentNode::AppendChild(child, m_dataOutPorts);    break;
        case Parsers::Kind::Instance:       AST_ParentNode::AppendChild(child, m_instances);       break;
        case Parsers::Kind::Alias:          AST_ParentNode::AppendChild(child, m_aliases);         break;

        case Parsers::Kind::AccessLink :    SetChild(child, m_accessLink); break;

        default:  // Ignore all other for now
          break;
      }
    }
  }

  // ---------------- Check that each local parameter have a name different than "standard" (those that can be overridden) parameters
  //
  for (const auto localParameter : m_localParameters)
  {
    auto pos = std::find_if(m_parameters.begin(), m_parameters.end(), [localParameter](auto iter) { return iter->Name() == localParameter->Name(); });
    CHECK_PARAMETER_EQ(pos, m_parameters.end(), "Module \""s.append(Name()).append("\" local parameter name \"")
                                                            .append(localParameter->Name())
                                                            .append("\" clashes with overridable module parameter"));
  }
}
//
//  End of: AST_Module::DispatchChildren
//---------------------------------------------------------------------------


//! Searches for a DataOutputPort with specified identifier
//!
//! @param identifier   An identifier for DataOutputPort to find
//!
AST_Port* AST_Module::FindDataOutPort (const AST_Identifier* identifier)
{
  CHECK_PARAMETER_NOT_NULL(identifier, "Cannot find DataOutputPort from nullptr identifier");

  return FindNode(m_dataOutPorts, identifier);
}
//
//  End of: AST_Module::FindDataOutPort
//---------------------------------------------------------------------------


//! Searches for a ScanMux with specified identifier
//!
//! @param identifier   An identifier for ScanMux to find
//!
AST_ScanMux* AST_Module::FindScanMux (const AST_Identifier* identifier)
{
  CHECK_PARAMETER_NOT_NULL(identifier, "Cannot find ScanMux from nullptr identifier");

  return FindNode(m_scanMuxes, identifier);
}
//
//  End of: AST_Module::FindScanMux
//---------------------------------------------------------------------------


//! Searches for a ScanInputPort with specified identifier
//!
//! @param identifier   An identifier for ScanInputPort to find
//!
AST_Port* AST_Module::FindScanInPort (const AST_Identifier* identifier)
{
  CHECK_PARAMETER_NOT_NULL(identifier, "Cannot find ScanInputPort from nullptr identifier");

  return FindNode(m_scanInPorts, identifier);
}
//
//  End of: AST_Module::FindScanInPort
//---------------------------------------------------------------------------


//! Searches for a ScanOutputPort with specified identifier
//!
//! @param identifier   An identifier for ScanOutputPort to find
//!
AST_Port* AST_Module::FindScanOutPort (const AST_Identifier* identifier)
{
  CHECK_PARAMETER_NOT_NULL(identifier, "Cannot find ScanOutputPort from nullptr identifier");

  return FindNode(m_scanOutPorts, identifier);
}
//
//  End of: AST_Module::FindScanOutPort
//---------------------------------------------------------------------------


//! Searches for a ScanRegister with specified identifier
//!
//! @param identifier   An identifier for ScanRegister to find
//!
AST_ScanRegister* AST_Module::FindScanRegister (const AST_Identifier* identifier)
{
  CHECK_PARAMETER_NOT_NULL(identifier, "Cannot find ScanRegister from nullptr identifier");

  return FindNode(m_scanRegisters, identifier);
}
//
//  End of: AST_Module::FindScanRegister
//---------------------------------------------------------------------------


//! Searches for a Instance with specified identifier
//!
//! @param identifier   An identifier for Instance to find
//!
AST_Instance* AST_Module::FindInstance (const AST_Identifier* identifier)
{
  CHECK_PARAMETER_NOT_NULL(identifier, "Cannot find Instance from nullptr identifier");
  return FindNode(m_instances, identifier);
}
//
//  End of: AST_Module::FindInstance
//---------------------------------------------------------------------------


//! Returns module ScanInterface with specified identifier
//!
//! @param identifier   An identifier for ScanInterface to find
//!
AST_ScanInterface* AST_Module::FindScanInterface (const AST_Identifier* identifier)
{
  CHECK_PARAMETER_NOT_NULL(identifier, "Cannot find ScanInterface from nullptr identifier");
  return FindNode(m_scanInterfaces, identifier);
}
//
//  End of: AST_Module::FindScanInterface
//---------------------------------------------------------------------------


//! Returns module ScanInterface with specified identifier
//!
//! @param name   Name for ScanInterface to find
//!
AST_ScanInterface* AST_Module::FindScanInterface (string_view interfaceName)
{
  return FindNode(m_scanInterfaces, interfaceName);
}
//
//  End of: AST_Module::FindScanInterface
//---------------------------------------------------------------------------



//! Moves local parameter to parameter set
//!
//! @note It must be call after instance parameter override
void AST_Module::JoinParameters ()
{
  m_parameters.insert(m_parameters.end(), m_localParameters.begin(), m_localParameters.end());

  m_localParameters.clear();
}
//
//  End of: AST_Module::JoinParameters
//---------------------------------------------------------------------------



//! Resolves Parameter Reference in members excepted for Parameters that are dealt with specifically and at different time point
//!
void AST_Module::Resolve ()
{
  AST_Parameter::ResolveItems(m_dataInPorts,   m_parameters);
  AST_Parameter::ResolveItems(m_dataOutPorts,  m_parameters);
  AST_Parameter::ResolveItems(m_scanInPorts,   m_parameters);
  AST_Parameter::ResolveItems(m_scanOutPorts,  m_parameters);
  AST_Parameter::ResolveItems(m_scanRegisters, m_parameters);
  AST_Parameter::ResolveItems(m_scanMuxes,     m_parameters);
  AST_Parameter::ResolveItems(m_attributes,    m_parameters);
}
//
//  End of: AST_Module::Resolve
//---------------------------------------------------------------------------



//! Uniquifies module using parameters overrides
//!
//! @note Unification consist to have a single object representing that particular module in some context.
//!       The result can be modified without affecting any other Module/Instance
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//! @param parameters   Actual instance parameter values
//!
//! @return New and unique AST_Module
//!
AST_Module* AST_Module::Uniquify (AST_Builder& astBuilder, const std::vector<AST_Parameter*>& parameters)
{
  auto clone = astBuilder.Clone_Module(this);

  clone->Uniquify_impl(astBuilder, parameters);

  return clone;
}
//
//  End of: AST_Module::Uniquify
//---------------------------------------------------------------------------




//! Uniquifies module using parameters overrides
//!
//! @note Unification consist to have a single object representing that particular module in some context.
//!       The result can be modified without affecting any other Module/Instance
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//! @param parameters   Actual parameter values - There should be no parameter reference in their values
//!
void AST_Module::Uniquify_impl (AST_Builder& astBuilder, const std::vector<AST_Parameter*>& parameters)
{
  UniquifyOrUpdateParameters(astBuilder, parameters);
  AST_Parameter::ResolveItems(m_parameters, m_parameters, astBuilder);

  UniquifyItemsWithParameterRef(m_attributes, astBuilder);

  UniquifyItems(m_scanRegisters, astBuilder);
  UniquifyItems(m_scanMuxes,     astBuilder);
  UniquifyItems(m_dataInPorts,   astBuilder);
  UniquifyItems(m_dataOutPorts,  astBuilder);
  UniquifyItems(m_scanInPorts,   astBuilder);
  UniquifyItems(m_scanOutPorts,  astBuilder);

  UniquifyInstances(astBuilder);

  Resolve();
}
//
//  End of: AST_Module::Uniquify
//---------------------------------------------------------------------------



//! Uniquifies as the top module
//!
//! @note Should be called only on top module !
//! @note Unification consist to have a single object representing that particular module in some context.
//!       The result can be modified without affecting any other Module/Instance
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//!
void AST_Module::UniquifyAsTop (AST_Builder& astBuilder)
{
  sm_clonedModules.clear(); // Reset circular dependencies detection memo

  JoinParameters();
  AST_Parameter::ResolveItems(m_parameters, m_parameters, astBuilder);
  Resolve();
  UniquifyInstances(astBuilder);
}
//
//  End of: AST_Module::UniquifyAsTop
//---------------------------------------------------------------------------


//! Uniquifies module instances
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//!
void AST_Module::UniquifyInstances (AST_Builder& astBuilder)
{
  auto network = astBuilder.Network();

  for (auto& instance : m_instances)
  {
    instance = instance->UniquifiedClone(astBuilder);
    instance->Resolve(astBuilder, m_parameters);

    const auto& instanceParameters = instance->Parameters();
    const auto  moduleId           = instance->ModuleIdentifier();
    const auto  instanceModule     = network->Module(moduleId);

    // Do clone, checking that there is no module circular dependencies (otherwise it would result in recursive loop !)
    bool alreadyCloned = std::find(sm_clonedModules.cbegin(), sm_clonedModules.cend(), instanceModule) != sm_clonedModules.cend();
    CHECK_FALSE(alreadyCloned, "Detected circular dependency regarding module \""s.append(instanceModule->Name()).append("\""));
    sm_clonedModules.push_back(instanceModule);

    auto newModule = instanceModule->Uniquify(astBuilder, instanceParameters);
    sm_clonedModules.pop_back();

    auto moduleIdentifier = astBuilder.Create_UniquifiedModuleIdentifier(newModule);

    newModule->m_parentModule = this;
    instance->UniquifiedModule(newModule, moduleIdentifier);
  }
}
//
//  End of: AST_Module::UniquifyInstances
//---------------------------------------------------------------------------



//! Uniquifies parameters except those overriden by instance parameters
//!
//! @note Parameters that do not refers to other parameters are not cloned either (their is no need as their value will not be changed)
//!
//! @param astBuilder   Interface to clone parameters (it is responsible for the memory management)
//! @param parameters   Actual parameter values - There should be no parameter reference in their values
//!
void AST_Module::UniquifyOrUpdateParameters (AST_Builder& astBuilder, const std::vector<AST_Parameter*>& instanceParameters)
{
  for (auto& parameter : m_parameters)
  {
    auto pos = std::find_if(instanceParameters.begin(), instanceParameters.end(), [parameter](auto iter)
               {
                 return iter->Name() == parameter->Name();
               });

    if (pos != instanceParameters.end())
    {
      auto instanceParameter = *pos;

      CHECK_TRUE(RepresentsSameTypes(parameter, instanceParameter), "Instance parameter \""s.append(instanceParameter->Name())
                                                                                            .append("\" of type \"")
                                                                                            .append(instanceParameter->IsString() ? "string" : "number")
                                                                                            .append("\" must override a Module parameter of same type.\n")
                                                                                            .append("The parameter_def included in a parameter_override "
                                                                                                    "element shall match both the name and type of a "
                                                                                                    "parameter_def within the module being instantiated."));

      parameter = instanceParameter;   // Substitute parameter with actual value (from instance)
    }

    if (parameter->HasParameterRef())
    {
      parameter = parameter->UniquifiedClone(astBuilder);
    }
  }

  // ---------------- Local Parameters
  //
  for (auto& parameter : m_localParameters)
  {
    if (parameter->HasParameterRef())
    {
      parameter = parameter->UniquifiedClone(astBuilder);
    }
  }

  // ---------------- Join local instanceParameters with others
  //                  (this simplifies things and can be done because there will be no more override)
  //
  JoinParameters();
}
//
//  End of: AST_Module::UniquifyOrUpdateParameters
//---------------------------------------------------------------------------

//===========================================================================
// End of AST_Module.cpp
//===========================================================================
