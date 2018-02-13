//===========================================================================
//                           NodePathResolver.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file NodePathResolver.hpp
//!
//! Declares NodePathResolver class
//!
//===========================================================================

#ifndef NODEPATHRESOLVER_H__CC7B2EE6_BC6_4C29_8380_19E010CCA737__INCLUDED_
  #define NODEPATHRESOLVER_H__CC7B2EE6_BC6_4C29_8380_19E010CCA737__INCLUDED_

#include "ParentNode.hpp"
#include "Mast_Core_export.hpp"

#include <memory>
#include <string>
#include <map>
#include <experimental/string_view>

namespace mast
{
class RegisterInterface;

//! Associates paths with actual nodes
//! @note Path are defined relative to a, fix, reference node
//!       User can register at will a prefix that is a path relative to reference node
class MAST_CORE_EXPORT NodePathResolver final
{
  // ---------------- Public  Methods
  //
  public:
  ~NodePathResolver() = default;

  //! Initializes the resolver with a reference node
  //!
  NodePathResolver(std::shared_ptr<ParentNode> referenceNode);

  //! Returns associated "reference" node
  //!
  std::shared_ptr<ParentNode> ReferenceNode() const { return m_referenceNode; }

  //! Sets reference node to search from
  //!
  void  ReferenceNode (std::shared_ptr<ParentNode> referenceNode);

  //! Finds node with relative path from "prefix" or "reference" node
  //!
  SystemModelNode* Resolve (std::experimental::string_view path) const;


  //! Finds Register with relative path from "prefix" or "reference" node
  //!
  RegisterInterface* ResolveAsRegister (std::experimental::string_view registerPath) const;

  //! Tries to finds Register with relative path from "prefix" or "reference" node
  //!
  RegisterInterface* TryResolveAsRegister (std::experimental::string_view registerPath) const;

  //! Returns current path prefix
  //!
  std::string Prefix() const { return m_prefix; }

  //! Changes path prefix
  //!
  void  SetPrefix (std::string prefix);

  // ---------------- Private  Fields
  //
  private:
  using NodeCache_t     = std::map<std::string, SystemModelNode*>;
  using RegisterCache_t = std::map<std::string, RegisterInterface*>;

  std::shared_ptr<ParentNode> m_referenceNode;  //!< Reference node for paths
  std::shared_ptr<ParentNode> m_prefixNode;     //!< Node associated with prefix (equals m_rootNode when prefix is empty)
  mutable NodeCache_t         m_nodesCache;     //!< Cache to speed up resolving recurrent node paths
  mutable RegisterCache_t     m_registersCache; //!< Cache to speed up resolving recurrent register paths
  std::string                 m_prefix;         //!< Path prefix (relative path from reference node)
};
//
//  End of NodePathResolver class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined NODEPATHRESOLVER_H__CC7B2EE6_BC6_4C29_8380_19E010CCA737__INCLUDED_

//===========================================================================
// End of NodePathResolver.hpp
//===========================================================================
