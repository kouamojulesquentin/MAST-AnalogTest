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
#include <deque>

using std::shared_ptr;
using std::dynamic_pointer_cast;
using std::deque;
using std::string;
using std::experimental::string_view;
using namespace mast;

//! Initializes the resolver with a reference node
//!
//! @param referenceNode  A node that will be reference for resolving path (must be not nullptr)
//!
NodePathResolver::NodePathResolver(shared_ptr<ParentNode> referenceNode)
  : m_referenceNode (referenceNode)
  , m_prefixNode    (referenceNode)
{
  CHECK_PARAMETER_NOT_NULL(referenceNode, "NodePathResolver must be constructed with a valid reference node");
}
//
//  End of NodePathResolver
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
shared_ptr<SystemModelNode> NodePathResolver::Resolve (string_view path)
{
  auto pos = m_cache.find(path.to_string());
  if (pos != m_cache.end())
  {
    auto foundNode = pos->second;
    return foundNode;
  }

  auto foundNode = m_prefixNode->FindNode(path);
  if (foundNode)
  {
    m_cache[path.to_string()] = foundNode;
  }

  return foundNode;
}
//
//  End of: NodePathResolver::Resolve
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
