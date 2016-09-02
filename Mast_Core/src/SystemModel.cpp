//===========================================================================
//                           SystemModel.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModel.cpp
//!
//! Implements class SystemModel
//!
//===========================================================================

#include "SystemModel.hpp"
#include "SystemModelNode.hpp"
#include "Utility.hpp"
#include "DefaultTableBasedPathSelector.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "LoopbackAccessInterfaceProtocol.hpp"
#include "SystemModelCheckerVisitor.hpp"
#include <string>

using namespace mast;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;
using std::string;
using std::experimental::string_view;


//! Checks model coherence
//!
//! @note
//!   - Each node has one and only one parent (except root that has no parent)
//!   - Each parent node has at least one child otherwise a warning is issued
//!   - Each child is only appended once in its parent
//!   - Each node is reachable (no dangling node)
//!   - Unused id generate a warning
//!   - Each linker has a number of chidren that matches its selector or an warning is issued when there are to few
//!     children and an error when there are too much
//!
SystemModelCheckResult SystemModel::Check () const
{
  SystemModelCheckerVisitor checker(*this);

  auto result = checker.Check();

  return result;
}
//
//  End of: SystemModel::Check
//---------------------------------------------------------------------------


//! Creates a new AccessInterface node
//!
//! @note An is the root node if it is the first node to be created
shared_ptr<AccessInterface> SystemModel::CreateAccessInterface (string_view name, shared_ptr<AccessInterfaceProtocol> protocol)
{
  auto node = make_shared<AccessInterface > (name, protocol);

  RegisterNode(node);

  if (!m_root && m_autoRootNode)
  {
    m_root = node;
  }


  return node;
}
//
//  End of: SystemModel::CreateAccessInterface
//---------------------------------------------------------------------------



//! Creates a new Chain node
//!
//! @note A chain is the root node if it is the first node to be created
shared_ptr<Chain> SystemModel::CreateChain (string_view name, shared_ptr<ParentNode> parentNode)
{
  auto node = make_shared<Chain> (name);

  RegisterNode(node);

  if (!m_root && m_autoRootNode)
  {
    m_root = node;
  }

  if (parentNode)
  {
    parentNode->AppendChild(node);
  }

  return node;
}
//
//  End of: SystemModel::CreateChain
//---------------------------------------------------------------------------



//! Creates a new Linker node
//!
shared_ptr<Linker> SystemModel::CreateLinker (string_view name, shared_ptr<PathSelector> pathSelector, shared_ptr<ParentNode> parentNode)
{
  auto node = make_shared<Linker> (name, pathSelector);

  if (parentNode)
  {
    parentNode->AppendChild(node);
  }

  RegisterNode(node);
  return node;
}
//
//  End of: SystemModel::CreateLinker
//---------------------------------------------------------------------------


//! Creates a new Register node
//!
shared_ptr<Register> SystemModel::CreateRegister (string_view            name,
                                                  BinaryVector           bypassSequence,
                                                  shared_ptr<ParentNode> parentNode)
{
  auto node = make_shared<Register> (name, bypassSequence);

  if (parentNode)
  {
    parentNode->AppendChild(node);
  }

  RegisterNode(node);
  return node;
}
//
//  End of: SystemModel::CreateRegister
//---------------------------------------------------------------------------




//! Returns node associated with a node identifier
//!
std::shared_ptr<SystemModelNode> SystemModel::NodeWithId (SystemModelNode::NodeIdentifier identifier) const
{
  auto offset     = static_cast<decltype(m_identifierMapping.size())>(identifier);
  auto nodesCount = m_identifierMapping.size();

  if (offset >= nodesCount)
  {
    THROW_LOGIC_ERROR("There is no node registered for specified identifier");
  }

  auto node = m_identifierMapping[offset];

  if (!node)
  {
    THROW_LOGIC_ERROR("There is no more node registered for specified identifier");
  }

  return node;
}
//
//  End of: SystemModel::NodeWithId
//---------------------------------------------------------------------------


//! Saves relation between a node identifier and a SystemModelNode instance
//!
//! @param identifier   Unique identifier assigned to a node (must start from zero and incrementing)
//! @param node         Node to register
//!
void SystemModel::RegisterNode (std::shared_ptr<SystemModelNode> node)
{
  if (!node)
  {
    THROW_INVALID_ARGUMENT("Cannot register node from nullptr");
  }

  auto identifier = node->Identifier();
  auto offset     = static_cast<decltype(m_identifierMapping.size())>(identifier);
  auto nodesCount = m_identifierMapping.size();

  if (offset == nodesCount)
  {
    m_identifierMapping.push_back(node);
  }
  else if (offset < nodesCount)   // Recycling an identifier ?
  {
    auto nodeAtOffset = m_identifierMapping[offset];
    if (nodeAtOffset)
    {
      THROW_LOGIC_ERROR("A node is already registered for specified identifier");
    }

    m_identifierMapping[offset] = node;
  }
  else  // Out of order registration
  {
    THROW_LOGIC_ERROR("Some node(s) have been created without using SystemModel (which is not supported)");
  }
}
//
//  End of: SystemModel::RegisterNode
//---------------------------------------------------------------------------


//! Disconnects a node from SystemModel tree
//!
//! @note Post-condition: the node cannot be retrieved from model by tree traversal of the SystemModel
//!       but it is still managed by the SystemModel (its identifier is still valid)
//!       It can then be latter reconnected somewhere else in the tree
//!
//! @param node         The node to disconnect
//! @param parentNode   Starting point to search node direct parent (root if nullptr)
//!
void SystemModel::DisconnectNode (shared_ptr<SystemModelNode> node, shared_ptr<ParentNode> parentNode)
{
  CHECK_PARAMETER_NOT_NULL(node, "Invalid nullptr");

  if (node == m_root)
  {
    m_root = nullptr;
  }
  else
  {
    if (!parentNode)
    {
      CHECK_VALUE_NOT_NULL(m_root, "Cannot disconnect a node from 'no parent' when there is also no root node");
      parentNode = m_root;
    }

    if (!parentNode->HasDirectChild(node))
    {
      parentNode = parentNode->FindParentOfNode(node);
    }

    CHECK_VALUE_NOT_NULL(parentNode, "Cannot find parent of node '"s + node->Name() + "' to disconnect");

    parentNode->DisconnectChild(node);
  }
}
//
//  End of: SystemModel::DisconnectNode
//---------------------------------------------------------------------------


//! Removes node from those managed by SystemModel
//!
//! @param node The node to remove
//!
//! @note When removing the node, It should not have a parent nor siblings
//!       Caller is responsible to maintain model tree consistency
//!
//! @note Post-condition: the node cannot be retrieved from model using its identifier
//!
//! @see ParentNode DisconnectXxx methods
//!
void SystemModel::RemoveNodeFromModel (shared_ptr<SystemModelNode> node)
{
  CHECK_PARAMETER_NOT_NULL(node, "Cannot remove node nullptr");

  auto id     = node->Identifier();
  auto offset = static_cast<TIdentifierMapping::size_type>(id);

  if (offset >= m_identifierMapping.size())
  {
    THROW_LOGIC_ERROR("Invalid node identifier: cannot remove a node that is not managed by the SystemModel");
  }

  if (m_identifierMapping[offset] != node)
  {
    THROW_LOGIC_ERROR("Cannot remove a node that is no more managed by the SystemModel");
  }

  m_identifierMapping[offset] = nullptr;
}
//
//  End of: SystemModel::RemoveNodeFromModel
//---------------------------------------------------------------------------



//! Replaces (or set first) root node
//!
//! @note Caller is responsible for keeping system model coherency
//!
//! @param newRoot              Parent node that will be new root node
//! @param removeBeforeReplace  True if node is discarded from those managed by the SystemModel
//!
//! @return Old root node
shared_ptr<ParentNode> SystemModel::ReplaceRoot (std::shared_ptr<ParentNode> newRoot, bool removeBeforeReplace)
{
  CHECK_PARAMETER_NOT_NULL(newRoot, "Cannot replace root node with nullptr");

  auto oldRoot = m_root;
  if (removeBeforeReplace && m_root)
  {
    RemoveNodeFromModel(m_root);
  }

  m_root = newRoot;
  return oldRoot;
}
//
//  End of: SystemModel::ReplaceRoot
//---------------------------------------------------------------------------


//===========================================================================
// End of SystemModel.cpp
//===========================================================================
