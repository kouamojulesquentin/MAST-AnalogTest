//===========================================================================
//                           AST_Network.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Network.cpp
//!
//! Implements class AST_Network
//!
//===========================================================================

#include "AST_Network.hpp"
#include "AST_Module.hpp"
#include "AST_ModuleIdentifier.hpp"
#include "AST_Namespace.hpp"
#include "AST_Visitor.hpp"

#include "Utility.hpp"
#include "g3log/g3log.hpp"

#include <memory>
#include <tuple>

using std::vector;
using std::make_pair;
using std::tie;

using namespace Parsers;

//! Visited part of the Visitor pattern
//!
void AST_Network::Accept (AST_Visitor& visitor)
{
  visitor.Visit_Network(this);
}

//! Adds module definition in namespace
//!
//! @param moduleNamespace  Namespace in which module is defined
//! @param module           Defined module
//!
void AST_Network::AddModule (const AST_Namespace* moduleNamespace, AST_Module* module)
{
  CHECK_PARAMETER_NOT_NULL(moduleNamespace, "Invalid nullptr module namespace ");
  CHECK_PARAMETER_NOT_NULL(module,          "Invalid nullptr for module");

  // ---------------- Retrieve modules for that namespace
  //
  auto& modulesMap = m_modules[moduleNamespace];

  // ---------------- Insert or replace module (if a module is already defined with that name)
  //
  const auto& moduleName = module->Name();
  auto&       previous   = modulesMap[moduleName];

  if (previous != nullptr)
  {
    LOG(WARNING) << "Replacing previously defined module: " << moduleNamespace->AsText() << moduleName;
  }
  previous = module;

  if (module->HasAccessLink())
  {
    CHECK_TRUE (   (m_topModule == nullptr)
                || !m_topModule->HasAccessLink(), "Must have only one module with an AccessLink");

    m_topModule   = module;
    m_firstModule = nullptr;
  }
  else if (m_firstModule == nullptr)
  {
    m_firstModule = module; // For cases when there is no module with an AccessLink
  }
}
//
//  End of: AST_Network::AddModule
//---------------------------------------------------------------------------



//! Returns namespaces for which some modules have been defined
//!
vector<const AST_Namespace*> AST_Network::NamespacesWithDefinedModules () const
{
  vector<const AST_Namespace*> namespaces;
  namespaces.reserve(m_modules.size());

  for (const auto& value : m_modules)
  {
    namespaces.push_back(value.first);
  }

  return namespaces;
}
//
//  End of: AST_Network::NamespacesWithDefinedModules
//---------------------------------------------------------------------------



//! Returns module matching identifier
//!
//! @param identifier Identifier for module to search for
//!
AST_Module* AST_Network::Module (const AST_ModuleIdentifier* identifier) const
{
  CHECK_PARAMETER_NOT_NULL(identifier, "Cannot search Module from nullptr identifier");


  // ---------------- Find collection of modules in given namespace
  //
  auto namespaceNode = identifier->Namespace();
  auto modulesPos    = m_modules.find(namespaceNode);

  CHECK_PARAMETER_NEQ(modulesPos, m_modules.cend(), "Have no namespace with name \""s.append(namespaceNode->Name()).append("\""));

  const auto& modules = modulesPos->second;

  // ---------------- Find module in collection of modules
  //
  auto        moduleId   = identifier->ModuleIdentifier();
  const auto& moduleName = moduleId->Name();
  auto        modulePos  = modules.find(moduleName);

  CHECK_PARAMETER_NEQ(modulePos, modules.cend(), "Have no module with name \""s.append(moduleName).append("\" in namespace \"").append(namespaceNode->Name()).append("\""));

  auto module = modulePos->second;
  return module;
}
//
//  End of: AST_Network::Module
//---------------------------------------------------------------------------



//! Returns all modules defined in namespace
//!
//! @param modulesNamespace   Namespace for which to returns modules defined in
//!
//! @note overriden modules are not returned as they are discarded
//!
vector<AST_Module*> AST_Network::ModulesInNamespace (const AST_Namespace* modulesNamespace) const
{
  vector<AST_Module*> modules;

  auto pos = m_modules.find(modulesNamespace);
  if (pos != m_modules.cend())
  {
    const auto& modulesMap = pos->second;

    modules.reserve(modulesMap.size());

    for (const auto& value : modulesMap)
    {
      modules.push_back(value.second);
    }
  }

  return modules;
}
//
//  End of: AST_Network::ModulesInNamespace
//---------------------------------------------------------------------------



//===========================================================================
// End of AST_Network.cpp
//===========================================================================
