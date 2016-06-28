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
//! @param path Path of node relative to "prefix" node or "reference" node when there is no prefix
//!
//! @return Found node or nullptr
//!
shared_ptr<SystemModelNode> NodePathResolver::Resolve (string_view path)
{
  return ResolveFromNode(path, m_prefixNode);
}
//
//  End of: NodePathResolver::Resolve
//---------------------------------------------------------------------------


//! Finds node with relative path from a node
//!
//! @note For this method, no prefix is taken into account (search is done from referenceNode)
//!
//! @param path           Path of node relative to specified reference node
//! @param referenceNode  Actual reference node
//!
//! @return Found node or nullptr
shared_ptr<SystemModelNode> NodePathResolver::ResolveFromNode (string_view path, shared_ptr<ParentNode> referenceNode)
{
  shared_ptr<SystemModelNode> foundNode;
  auto                        notValidPath = false;

  if (path == ".")
  {
    foundNode = referenceNode;
  }
  else
  {
    deque<shared_ptr<ParentNode>> parentsToProcess;

    size_t startPos = 0;

    // ---------------- Sub-path extraction function
    //
    auto extractNextChunk = [&startPos, path, &notValidPath]()
    {
      if (startPos >= path.length())
      {
        return string_view();
      }

      size_t dotPos = path.find('.', startPos);

      // ---------------- Detect not valid multiple dots
      //
      if (dotPos == startPos)
      {
        notValidPath = true;
        startPos     = path.length();
        return string_view("<<< Not valid path ! >>>");
      }

      auto endPos = (dotPos == string_view::npos) ? path.length() : dotPos;
      auto length = endPos - startPos;
      auto chunk  = path.substr(startPos, length);

      startPos = endPos + 1u;
      return chunk;
    };

    // ---------------- Start init
    //
    auto currentPathChunk = extractNextChunk();
    auto nextParent       = referenceNode;

    // ---------------- Search loop of parent nodes
    //
    while (!foundNode && nextParent && !notValidPath)
    {
      auto currentChild = nextParent->FirstChild();
      while (currentChild && !notValidPath)
      {
        auto currentNodeAsParent = dynamic_pointer_cast<ParentNode>(currentChild);
        auto ignored             = currentNodeAsParent && currentNodeAsParent->IgnoreForNodePath();

        if (ignored)
        {
          parentsToProcess.push_back(currentNodeAsParent);
        }
        else if (currentChild->Name() == currentPathChunk)
        {
          currentPathChunk = extractNextChunk();

          if (currentPathChunk.empty())
          {
            foundNode = currentChild;
            break;
          }

          parentsToProcess.clear();     // Partial path found ==> ignore previously saved parent nodes

          if (!currentNodeAsParent)     // Found non terminal path chunk on terminal node?
          {
            break;
          }

          currentChild = nullptr; // We have found node for current level (so we can ignore siblings)
          parentsToProcess.push_back(currentNodeAsParent);  // Prepare for next parent node loop
        }

        if (currentChild)
        {
          currentChild = currentChild->NextSibling();
        }
      } // End of: siblings loop


      if (parentsToProcess.empty())
      {
        break;
      }

      nextParent = parentsToProcess.front();
      parentsToProcess.pop_front();
    }
  } // End of: parent nodes loop

  if (notValidPath)
  {
    foundNode = nullptr;
  }
  return foundNode;
}
//
//  End of: NodePathResolver::ResolveFromNode
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
    auto prefixNode = ResolveFromNode(prefix, m_referenceNode);

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
