//===========================================================================
//                           NodePathResolver.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file NodePathResolver.cpp
//!
//! Implements class NodePathResolver
//!
//===========================================================================

#include "NodePathResolver.hpp"
#include "Utility.hpp"
#include "Register.hpp"

using std::shared_ptr;
using std::dynamic_pointer_cast;
using std::string;
using std::experimental::string_view;
using std::to_string;
using namespace mast;

//! Initializes the resolver with a reference node
//!
//! @param referenceNode  A node that will be reference for resolving path (if nullptr, ReferenceNode() setter must be called before any call to Resolve)
//!
NodePathResolver::NodePathResolver(shared_ptr<ParentNode> referenceNode)
  : m_referenceNode (referenceNode)
  , m_prefixNode    (referenceNode)
{
}
//
//  End of NodePathResolver
//---------------------------------------------------------------------------


//! Sets reference node to search from
//!
void NodePathResolver::ReferenceNode (std::shared_ptr<ParentNode> referenceNode)
{
  m_referenceNode = referenceNode;
  m_prefixNode    = referenceNode;
  m_prefix.clear();
}
//
//  End of: NodePathResolver::ReferenceNode
//---------------------------------------------------------------------------


//! Finds node with relative path from "prefix" or "reference" node
//!
//! @note Use a cache of already found path to speed up lookup of frequent path
//!       It uses default std::map compare functor but could probably be optimized with
//!       a functor that compare string in reverse order (paths deferentiation is often
//!       located near their last characters)
//!
//! @param path Path of node relative to "prefix" node or "reference" node when there is no prefix
//!
//! @return Found node or nullptr
//!
SystemModelNode* NodePathResolver::Resolve (string_view path) const
{
  auto pos = m_cache.find(path.to_string());
  if (pos != m_cache.end())
  {
    auto foundNode = pos->second;
    return foundNode;
  }

  CHECK_VALUE_NOT_NULL(m_prefixNode, "Resolver has not been initialized with a reference node");

  auto foundNode = m_prefixNode->FindNode(path);
  if (foundNode)
  {
    m_cache[path.to_string()] = foundNode.get();
  }

  return foundNode.get();
}
//
//  End of: NodePathResolver::Resolve
//---------------------------------------------------------------------------



//! Finds Register with relative path from "prefix" or "reference" node
//!
//! @param path Path of Register relative to "prefix" node or "reference" node when there is no prefix
//!
//! @return    Found Register
//! @exception std::invalid_argument when path do not denote a Register
RegisterInterface* NodePathResolver::ResolveAsRegister (string_view registerPath) const
{
  auto node = Resolve(registerPath);
  auto reg  = dynamic_cast<RegisterInterface*>(node);

  if (!reg)
  {
    THROW_INVALID_ARGUMENT("Path: '"s + registerPath.to_string() + "' does not refer to a Register" );
  }
  return reg;
}
//
//  End of: NodePathResolver::ResolveAsRegister
//---------------------------------------------------------------------------


//! Changes path prefix (relative to reference node)
//!
//! @note An empty prefix (or '.') will restore search path relative to reference node
//!
//! @param prefix   New path prefix (from reference node)
//!
void NodePathResolver::SetPrefix (string prefix)
{
  m_cache.clear();

  if (prefix.empty() || (prefix == "."))
  {
    m_prefixNode = m_referenceNode;
    m_prefix.clear();
  }
  else
  {
    CHECK_VALUE_NOT_NULL(m_referenceNode, "Reference node must be set before calling SetPrefix");
    auto prefixNode = m_referenceNode->FindNode(prefix);

    CHECK_VALUE_NOT_NULL(prefixNode, "Prefix: '"s + prefix + "' is not valid (cannot find matching node)");

    auto prefixNodeAsParentNode = dynamic_pointer_cast<ParentNode>(prefixNode);
    CHECK_VALUE_NOT_NULL(prefixNodeAsParentNode, "Prefix: '"s + prefix + "' must refer to a parent node");

    m_prefixNode = prefixNodeAsParentNode;
    m_prefix     = std::move(prefix);
  }
}
//
//  End of: NodePathResolver::SetPrefix
//---------------------------------------------------------------------------



//===========================================================================
// End of NodePathResolver.cpp
//===========================================================================
