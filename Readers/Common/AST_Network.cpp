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
    CHECK_VALUE_NULL(m_topModule, "Support only a single \"top\" module");
    m_topModule = module;
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
