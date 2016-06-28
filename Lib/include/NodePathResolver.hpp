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
#include "Platform.hpp"

#include <memory>
#include <string>
#include <experimental/string_view>

namespace mast
{
//! Associates paths with actual nodes
//! @note Path are defined relative to a, fix, reference node
//!       User can register at will a prefix that is a path relative to reference node
class DLL_EXPORT NodePathResolver final
{
  // ---------------- Public  Methods
  //
  public:
  ~NodePathResolver() = default;

  //! Initializes the resolver with a reference node
  //!
  NodePathResolver(std::shared_ptr<ParentNode> referenceNode);

  //! Finds node with relative path from "prefix" or "reference" node
  //!
  std::shared_ptr<SystemModelNode> Resolve(std::experimental::string_view path);

  static std::shared_ptr<SystemModelNode> ResolveFromNode (std::experimental::string_view path, std::shared_ptr<ParentNode> referenceNode);


  //! Returns current path prefix
  //!
  std::string Prefix() const { return m_prefix; }

  //! Changes path prefix
  //!
  void  SetPrefix (std::string prefix);

  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
  std::shared_ptr<ParentNode> m_referenceNode; //!< Reference node for paths
  std::shared_ptr<ParentNode> m_prefixNode;    //!< Node associated with prefix (equals m_rootNode when prefix is empty)
  std::string                 m_prefix;        //!< Path prefix (relative path from reference node)
};
//
//  End of NodePathResolver class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined NODEPATHRESOLVER_H__CC7B2EE6_BC6_4C29_8380_19E010CCA737__INCLUDED_

//===========================================================================
// End of NodePathResolver.hpp
//===========================================================================
