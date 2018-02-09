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
#include "RegistersAlias.hpp"
#include "Utility.hpp"
#include "RegisterInterface.hpp"
#include "AliasRepository.hpp"

#include <deque>

using std::deque;
using std::shared_ptr;
using std::dynamic_pointer_cast;
using std::experimental::string_view;
using std::to_string;
using std::make_unique;

using namespace mast;

//! Releases resources
//!
//! @note Explicitly defining it in source file is to deal with unique_ptr deleter request for sizeof(T) !
ParentNode::~ParentNode ()
{
}
//
//  End of: ParentNode::~ParentNode
//---------------------------------------------------------------------------


//! Constructor...
ParentNode::ParentNode (string_view name)
  : SystemModelNode(name)
{
}
//
//  End of: ParentNode::ParentNode
//---------------------------------------------------------------------------




//! Appends a new "Register" alias to the collection of aliases defined for that ParentNode
//!
void ParentNode::AddAlias (RegistersAlias&& alias)
{
  if (!m_aliases)
  {
    m_aliases = make_unique<AliasRepository>();
  }

  m_aliases->Append(std::move(alias));
}
//
//  End of: ParentNode::AddAlias
//---------------------------------------------------------------------------



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



//! Returns ParentNode that will effectively append childs in place of this ParentNode
//!
shared_ptr<ParentNode> ParentNode::DeepestChildAppender () const
{
  auto appender = m_pOptionalChildAppender;

  while (appender)
  {
    auto nextAppender = appender->m_pOptionalChildAppender;
    if (!nextAppender)
    {
      break;
    }
    appender = nextAppender;
  }
  return appender;
}
//
//  End of: ParentNode::DeepestChildAppender
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


//! Disconnects a endpoint from the parent
//!
//! @param endpointId Identifies the endpoint to disconnect
//!
//! @return Disconnected endpoint
//!
shared_ptr<SystemModelNode> ParentNode::DisconnectEndPoint (uint32_t endpointId)
{
  CHECK_PARAMETER_NOT_ZERO (endpointId,  "Cannot remove endpoint '0'");
  CHECK_VALUE_NOT_NULL     (m_pFirstChild, "Cannot disconnect a endpoint when there is no child");

  uint32_t currentId       = 1u;
  auto     currentChild    = m_pFirstChild;
  auto     previousSibling = shared_ptr<SystemModelNode>();

  while (currentId != endpointId)
  {
    previousSibling = currentChild;
    currentChild    = currentChild->NextSibling();

    CHECK_VALUE_NOT_NULL (currentChild, "Cannot disconnect endpoint "s + to_string(endpointId) +
                                        " when there is/are only "       + to_string(currentId) + " endpoint(s)");
    ++currentId;
  }

  DisconnectSibling(previousSibling, currentChild);

  return currentChild;
}
//
//  End of: ParentNode::DisconnectEndPoint
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

  if (sibling == m_pOptionalChildAppender)
  {
    m_pOptionalChildAppender = nullptr;
  }
}
//
//  End of: ParentNode::DisconnectSibling
//---------------------------------------------------------------------------


//! Finds a child of current parent node
//!
//! @note It searches from direct children then from parent nodes ignored in search path
//!
//! @param childName  Searched child name (without any hierarchy)
//!
SystemModelNode* ParentNode::FindChild (string_view childName)
{
  // ---------------- Direct children
  //
  auto currentChild = m_pFirstChild;

  while (currentChild != nullptr)
  {
    if (currentChild->Name() == childName)
    {
      return currentChild.get();
    }
    currentChild = currentChild->NextSibling();
  }

  // ---------------- Seach from children of parent node ignored in paths
  //
  currentChild = m_pFirstChild;

  while (currentChild != nullptr)
  {
    auto currentNodeAsParent = dynamic_cast<ParentNode*>(currentChild.get());
    if ((currentNodeAsParent != nullptr) && currentNodeAsParent->IgnoreForNodePath())
    {
      auto foundChild = currentNodeAsParent->FindChild(childName);
      if (foundChild != nullptr)
      {
        return foundChild;
      }
    }
    currentChild = currentChild->NextSibling();
  }

  return nullptr;
}
//
//  End of: ParentNode::FindChild
//---------------------------------------------------------------------------





//! Finds node with path relative to this node
//!
//! @param path  Path of node relative to this node
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


//! Finds a register or virtual register with relative path from this parent node
//!
//! @note For this method, no prefix is taken into account (search is done from current node)
//! @note Register aliases have higher precedence than direct or indirect children - This avoid
//!       considering (possibly recursively) children of "ignored in path" children
//!
//! @param path   Path of node relative to current node
//!
//! @return Found register or virtual register or nullptr when not found
RegisterInterface* ParentNode::FindRegister (string_view path)
{
  // ---------------- Split parent path and register (or alias) name
  //
  auto parentPath          = Utility::FrontTokens(path, ".");
  auto registerOrAliasName = Utility::BackToken(path,   ".");

  if (parentPath.length() == path.length()) // Is this really a hierarchical path ?
  {
    parentPath = string_view();
  }

  CHECK_PARAMETER_NOT_EMPTY(registerOrAliasName, "Cannot find register with empty name");

  // ---------------- Search for parent node
  //
  auto parentNode = parentPath.empty() ? this : dynamic_cast<ParentNode*>(FindNode(parentPath).get());

  if (parentNode == nullptr)
  {
    return nullptr;
  }

  // ---------------- Search within aliases
  //
  if (parentNode->HasAliases())
  {
    auto foundRegister = parentNode->m_aliases->FindRegister(registerOrAliasName);
    if (foundRegister != nullptr)
    {
      return foundRegister;
    }
  }

  // ---------------- Search within children
  //
  auto foundNode     = parentNode->FindChild(registerOrAliasName);
  auto foundRegister = dynamic_cast<RegisterInterface*>(foundNode);

  return foundRegister;
}
//
//  End of ParentNode::FindRegister
//---------------------------------------------------------------------------


//! Returns true when ParentNode has at least one defined alias, false otherwise
//!
bool ParentNode::HasAliases () const
{
  return m_aliases ?  m_aliases->HasAliases() : false;
}
//
//  End of: ParentNode::HasAliases
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


//! Adds new child node before current children
//!
//! @note It forms a singly linked list of children
//! @note If added child has sibbling, then all of them will be placed before
//!       current first child
//!
//! @param pChild Child to add as the first child
//!
void ParentNode::PrependChild (std::shared_ptr<SystemModelNode> pChild)
{
  if (m_pOptionalChildAppender)
  {
    m_pOptionalChildAppender->PrependChild(pChild);
  }
  else if (m_pFirstChild)
  {
    pChild->AppendSibling(m_pFirstChild);
    m_pFirstChild = pChild;
  }
  else
  {
    m_pFirstChild = pChild;
  }
}
//
//  End of ParentNode::PrependChild
//---------------------------------------------------------------------------



//! Returns current aliases for (virtual) registers
//!
//! @return Register aliases if any or nullptr if has none
const std::vector<RegistersAlias>* ParentNode::RegistersAliases () const
{
  return m_aliases ? &m_aliases->RegistersAliases() : nullptr;
}
//
//  End of: ParentNode::RegistersAliases
//---------------------------------------------------------------------------


//===========================================================================
// End of ParentNode.cpp
//===========================================================================
