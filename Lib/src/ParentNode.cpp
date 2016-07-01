//===========================================================================
//                           ParentNode.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ParentNode.cpp
//!
//! Implements class ParentNode
//!
//===========================================================================

#include "ParentNode.hpp"
#include "Utility.hpp"
#include <deque>

using std::deque;
using std::shared_ptr;
using std::dynamic_pointer_cast;
using std::to_string;
using std::experimental::string_view;
using namespace mast;


//! Appends a new child node
//!
//! @param pChild Child to append as the last child
//!
//! @note It forms a singly linked list of children
//!
void ParentNode::AppendChild (std::shared_ptr<SystemModelNode> pChild)
{
  if (m_pOptionalChildAppender)
  {
    m_pOptionalChildAppender->AppendChild(pChild);
  }
  else if (m_pFirstChild)
  {
    m_pFirstChild->AppendSibling(pChild);
  }
  else
  {
    m_pFirstChild = pChild;
  }
}
//
//  End of ParentNode::AppendChild
//---------------------------------------------------------------------------


//! Returns current number of children
//!
//! @note It is intended for test or checking purpose (It is O(N) time complexity)
uint32_t ParentNode::DirectChildrenCount () const
{
  uint32_t count = 0;

  auto child = m_pFirstChild;
  while (child)
  {
    if (++count > 10000)
    {
      THROW_LOGIC_ERROR("There seems a child has been appended twice");
    }

    child = child->NextSibling();
  }
  return count;
}
//
//  End of: ParentNode::DirectChildrenCount
//---------------------------------------------------------------------------


//! Disconnects all direct children
//!
//! @return Formerly first child
//!
shared_ptr<SystemModelNode> ParentNode::DisconnectAllChildren ()
{
  auto disconnectedNode = m_pFirstChild;
  m_pFirstChild = nullptr;
  return disconnectedNode;
}
//
//  End of: ParentNode::DisconnectAllChildren
//---------------------------------------------------------------------------


//! Disconnects a child node
//!
//! @param child  The child node to remove
//!
void ParentNode::DisconnectChild (shared_ptr<SystemModelNode> child)
{
  CHECK_PARAMETER_NOT_NULL (child,         "Cannot remove 'nullptr' child");
  CHECK_VALUE_NOT_NULL     (m_pFirstChild, "Cannot disconnect a child when there is no child");

  auto     currentChild    = m_pFirstChild;
  auto     previousSibling = shared_ptr<SystemModelNode>();

  while (currentChild != child)
  {
    previousSibling = currentChild;
    currentChild    = currentChild->NextSibling();

    CHECK_VALUE_NOT_NULL (currentChild, "Node '"s + child->Name() + "' is not a child of '" + Name() + "'");
  }

  DisconnectSibling(previousSibling, currentChild);
}
//
//  End of: ParentNode::DisconnectChild
//---------------------------------------------------------------------------


//! Disconnects a derivation from the parent
//!
//! @param derivationId Identifies the derivation to disconnect
//!
//! @return Disconnected derivation
//!
shared_ptr<SystemModelNode> ParentNode::DisconnectDerivation (uint32_t derivationId)
{
  CHECK_PARAMETER_NOT_ZERO (derivationId,  "Cannot remove derivation '0'");
  CHECK_VALUE_NOT_NULL     (m_pFirstChild, "Cannot disconnect a derivation when there is no child");

  uint32_t currentId       = 1u;
  auto     currentChild    = m_pFirstChild;
  auto     previousSibling = shared_ptr<SystemModelNode>();

  while (currentId != derivationId)
  {
    previousSibling = currentChild;
    currentChild    = currentChild->NextSibling();

    CHECK_VALUE_NOT_NULL (currentChild, "Cannot disconnect derivation "s + to_string(derivationId) +
                                        " when there is/are only "       + to_string(currentId) + " derivation(s)");
    ++currentId;
  }

  DisconnectSibling(previousSibling, currentChild);

  return currentChild;
}
//
//  End of: ParentNode::DisconnectDerivation
//---------------------------------------------------------------------------



//! Disconnects a node from a chain of siblings
//!
//! @param previous   Node before the node to disconnect (nullptr if sibling is first child)
//! @param sibbling   The node to disconnect
//!
void ParentNode::DisconnectSibling (shared_ptr<SystemModelNode> beforeNode, shared_ptr<SystemModelNode> sibling)
{
  auto nextSibling = sibling->NextSibling();
  if (beforeNode)
  {
    beforeNode->SetNextSibling(nextSibling);
  }
  else
  {
    m_pFirstChild = nextSibling;
  }
}
//
//  End of: ParentNode::DisconnectSibling
//---------------------------------------------------------------------------


//! Tests whether a node is a direct child
//!
//! @param node The node to see if it is a direct child
//!
//! @return True if node is a direct child false otherwise
//!
bool ParentNode::HasDirectChild (std::shared_ptr<SystemModelNode> node) const
{
  CHECK_PARAMETER_NOT_NULL (node, "Invalid node: 'nullptr'");

  auto currentChild = m_pFirstChild;

  while (currentChild)
  {
    if (currentChild == node)
    {
      return true;
    }

    currentChild = currentChild->NextSibling();
  }

  return false;
}
//
//  End of: ParentNode::HasDirectChild
//---------------------------------------------------------------------------



//! Finds node with relative path from a node
//!
//! @note For this method, no prefix is taken into account (search is done from referenceNode)
//!
//! @param path           Path of node relative to specified reference node
//! @param referenceNode  Actual reference node
//!
//! @return Found node or nullptr
shared_ptr<SystemModelNode> ParentNode::FindNode (string_view path)
{
  shared_ptr<SystemModelNode> foundNode;
  auto                        notValidPath = false;

  if (path == ".")
  {
    foundNode = shared_from_this();
  }
  else
  {
    deque<shared_ptr<ParentNode>> parentsToProcess;   // Use deque instead of queue to have access to clear method!

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
    auto nextParent       = shared_from_this();

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
//  End of: ParentNode::FindNode
//---------------------------------------------------------------------------


//! Searches down the hierarchy, the parent of specified node
//!
//! @note DO NOT CALL on instances not managed by a shared_ptr
//!
//! @param child  A node for which parent node is searched for.
//!
//! @return Parent node in case of success, nullptr otherwise
//!
shared_ptr<ParentNode> ParentNode::FindParentOfNode (shared_ptr<SystemModelNode> child)
{
  CHECK_PARAMETER_NOT_NULL (child, "Invalid node: 'nullptr'");

  // ---------------- Check that this instance is managed by a shared_ptr
  //
  shared_ptr<ParentNode> currentParent;
  try
  {
    currentParent = shared_from_this();
  }
  catch(std::exception&)
  {
    THROW_LOGIC_ERROR("FindParentOfNode CANNOT BE CALLED on instances not managed by a shared_ptr");
  }

  // ---------------- Search loop
  //
  shared_ptr<ParentNode> foundParent;

  auto currentChild = currentParent->FirstChild();

  while (currentChild)
  {
    if (currentChild == child)
    {
      foundParent = currentParent;
      break;
    }
    currentChild = currentChild->NextSibling();
  }

  //! @todo [JFC]-[June/24/2016]: In FindParentOfNode(): Change implementation to do a real breadth first search
  //! ==> Use a queue of parent nodes
  currentChild = currentParent->FirstChild();
  while (!foundParent && currentChild)
  {
    auto childAsParent = dynamic_pointer_cast<ParentNode>(currentChild);
    if (childAsParent)
    {
      foundParent = childAsParent->FindParentOfNode(child);
    }
    currentChild = currentChild->NextSibling();
  }

  return foundParent;
}
//
//  End of: ParentNode::FindParentOfNode
//---------------------------------------------------------------------------


//===========================================================================
// End of ParentNode.cpp
//===========================================================================
