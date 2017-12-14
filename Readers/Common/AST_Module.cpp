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
        case Parsers::Kind::Instance:       AST_ParentNode::AppendChild(child, m_instances);       break;

        case Parsers::Kind::AccessLink :    SetChild(child, m_accessLink); break;

        default:  // Ignore all other for now
          break;
      }
    }
  }
}
//
//  End of: AST_Module::DispatchChildren
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
//  End of: AST_Module::FindScanRegister
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
//  End of: AST_Module::FindScanRegister
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
//  End of: AST_Module::FindScanRegister
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
//  End of: AST_Module::ScanInterface
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
//  End of: AST_Module::ScanInterface
//---------------------------------------------------------------------------


//! Uniquifies module using parameters overrides
//!
//! @note Unification consist to have a single object representing that particular module in some context.
//!       The result can be modified without affecting any other Module/Instance
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//! @param parameters   Actual parameter values - There should be no parameter reference in their values
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
  // ---------------- Update parameters with their override values
  //
  for (const auto& parameter : parameters)
  {
    auto pos = std::find_if(m_parameters.begin(), m_parameters.end(), [parameter](auto iter) { return iter->Name() == parameter->Name(); });
    if (pos != m_parameters.end())
    {
      *pos = parameter;   // Substitute parameter with actual value
    }
  }

  // ---------------- Replace remaining parameter references
  //                  (skip those that have been updated just above)
  //
  for (const auto& parameter : m_parameters)
  {
    auto pos = std::find(parameters.cbegin(), parameters.cend(), parameter);
    if (pos == parameters.cend())
    {
      //! @todo [JFC]-[November/22/2017]: In Uniquify_impl(): Replace parameter reference ==> May need to create a dependency graph to process them in proper order
      //!                                                     Replace also for local parameters
//+      if (parameter->HasParameterRef())
//+      {
//+        parameter->ReplaceParameterRef(m_parameters);
//+      }
    }
  }


  UniquifyScanOutPorts(astBuilder);
  UniquifyScanRegisters(astBuilder);
//+  UniquifyScanMuxs(astBuilder);
  UniquifyInstances(astBuilder);
}
//
//  End of: AST_Module::Uniquify
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
    auto clonedInstance = astBuilder.Clone_Instance(instance);
    instance = clonedInstance;  // Replace current (shared) by cloned (unique)

    //! @todo [JFC]-[November/22/2017]: In UniquifyInstances(): Replace parameter reference in instance parameters

    const auto& parameters       = instance->Parameters();
    const auto  moduleId         = instance->ModuleIdentifier();
    const auto  instanceModule   = network->Module(moduleId);

    { // Do clone, checking that there is no module circular dependencies (otherwise it would result in recursive loop !)
      bool alreadyCloned = std::find(sm_clonedModules.cbegin(), sm_clonedModules.cend(), instanceModule) != sm_clonedModules.cend();
      CHECK_FALSE(alreadyCloned, "Detected circular dependency regarding module \""s.append(instanceModule->Name()).append("\""));
      sm_clonedModules.push_back(instanceModule);
      auto        newModule        = instanceModule->Uniquify(astBuilder, parameters);
      sm_clonedModules.pop_back();
    }

    auto moduleIdentifier = astBuilder.Create_UniquifiedModuleIdentifier(newModule);

    newModule->m_parentModule = this;
    instance->UniquifiedModule(newModule, moduleIdentifier);
  }
}
//
//  End of: AST_Module::UniquifyInstances
//---------------------------------------------------------------------------


//! Uniquifies module ScanOutPorts
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//!
void AST_Module::UniquifyScanOutPorts (AST_Builder& astBuilder)
{
  for (auto& scanOutPort : m_scanOutPorts)
  {
    auto cloned = astBuilder.Clone_Port(scanOutPort);
    scanOutPort = cloned;  // Replace current (shared) by cloned (unique)
  }
}
//
//  End of: AST_Module::UniquifyScanRegisters
//---------------------------------------------------------------------------

//! Uniquifies module scan multiplexers
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//!
void AST_Module::UniquifyScanMuxes (AST_Builder& astBuilder)
{
  for (auto& scanMux : m_scanMuxes)
  {
    auto clonedScanMux = astBuilder.Clone_ScanMux(scanMux);
    scanMux = clonedScanMux;  // Replace current (shared) by cloned (unique)

    //! @todo [JFC]-[November/23/2017]: In UniquifyScanMuxes(): Replace parameter reference(s) of ScanMux
  }
}
//
//  End of: AST_Module::UniquifyScanMuxes
//---------------------------------------------------------------------------


//! Uniquifies module scan registers
//!
//! @param astBuilder   Interface to clone some kind of AST nodes (it is responsible for the memory management)
//!
void AST_Module::UniquifyScanRegisters (AST_Builder& astBuilder)
{
  for (auto& scanRegister : m_scanRegisters)
  {
    auto clonedScanRegister = astBuilder.Clone_ScanRegister(scanRegister);
    scanRegister = clonedScanRegister;  // Replace current (shared) by cloned (unique)

    //! @todo [JFC]-[November/23/2017]: In UniquifyScanRegisters(): Replace parameter reference(s) of ScanRegister
  }
}
//
//  End of: AST_Module::UniquifyScanRegisters
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_Module.cpp
//===========================================================================
