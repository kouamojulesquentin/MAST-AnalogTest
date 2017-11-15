//===========================================================================
//                           AST_Network.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Network.hpp
//!
//! Declares AST_Network class
//!
//===========================================================================

#ifndef AST_NETWORK_H__24C8E604_335D_413A_F8B1_500B5310C455__INCLUDED_
  #define AST_NETWORK_H__24C8E604_335D_413A_F8B1_500B5310C455__INCLUDED_

#include "AST_Node.hpp"
#include <map>
#include <vector>

namespace Parsers
{
class AST;
class AST_Namespace;
class AST_Module;
class AST_ModuleIdentifier;
class AST_Visitor;

//! Represents test network
//!
class AST_Network final : public AST_Node
{
  // ---------------- Public Methods
  //
  public:
  ~AST_Network() = default;

  //! Visited part of the Visitor pattern
  //!
  void Accept (AST_Visitor& visitor) override;

  //! Adds module definition in namespace
  //!
  void AddModule(const AST_Namespace* moduleNamespace, AST_Module* module);

  //! Returns namespaces for which some modules have been defined
  //!
  std::vector<const AST_Namespace*> NamespacesWithDefinedModules() const;

  //! Returns all modules defined in namespace
  //!
  std::vector<AST_Module*> ModulesInNamespace(const AST_Namespace* modulesNamespace) const;

  //! Returns top module (the one that has an AccessLink) or the first parsed module if none has an AccessLink
  //!
  AST_Module* TopModule() const { return m_topModule ? m_topModule : m_firstModule; }

  //! Returns module matching identifier
  //!
  AST_Module* Module(const AST_ModuleIdentifier* identifier) const;

  // ---------------- Private Methods
  //
  private:
  friend class AST;
  AST_Network()
    : AST_Node (Kind::Network)
  {
  }

  // ---------------- Private Fields
  //
  private:
  using NameToModuleMapper_t       = std::map<std::string,          AST_Module*>; // String is for module name is usefull for fast search of modules in the namespace
  using NamespaceToModulesMapper_t = std::map<const AST_Namespace*, NameToModuleMapper_t>;

  NamespaceToModulesMapper_t m_modules;               //!< Network modules by namespace they are defined in
  AST_Module*                m_topModule   = nullptr; //!< Top module (the one with an AccessLink)
  AST_Module*                m_firstModule = nullptr; //!< In case of there is no module with an AccessLink, it will serve as top module
};
//
//  End of AST_Network class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_NETWORK_H__24C8E604_335D_413A_F8B1_500B5310C455__INCLUDED_

//===========================================================================
// End of AST_Network.hpp
//===========================================================================
