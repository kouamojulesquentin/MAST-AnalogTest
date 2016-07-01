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


//! Changes path prefix (relative to reference node)
//!
//! @note An empty prefix (or '.') will restore search path relative to reference node
//!
//! @param prefix   New path prefix (from reference node)
//!
void NodePathResolver::SetPrefix (string prefix)
{
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
