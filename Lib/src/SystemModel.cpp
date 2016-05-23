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
#include "DefaultBinaryPathSelector.hpp"
#include "AccessInterfaceProtocol_1149_1.hpp"
#include "SystemModelCheckerVisitor.hpp"

using namespace mast;
using std::shared_ptr;
using std::make_shared;
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

  if (!m_root)
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

  if (!m_root)
  {
    if (parentNode)
    {
      THROW_LOGIC_ERROR("Unexpected parent node when there in root node yet");
    }
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
  if (!m_root)
  {
    THROW_LOGIC_ERROR("There is no root node yet");
  }

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
  if (!m_root)
  {
    THROW_LOGIC_ERROR("There is no root node yet");
  }

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


//! Creates a new Tap node
//!  ______________________________
//! |                              |
//! |     (ACCESS_I:Tap)           |
//! |      /      \                |
//! |     /       _\__________     |
//! | [REG:Ir]  /Linker:Dr_Mux\    |
//! |           ---------------    |
//! |             /                |
//! |        [REG:Bypass]          |
//! |                              |
//!  ------------------------------
shared_ptr<AccessInterface> SystemModel::CreateTap (string_view name,
                                                    uint32_t    irBitsCount,
                                                    uint32_t    muxPathsCount)
{
  if (name.empty())
  {
    name = DEFAULT_TAP_NAME;
  }

  auto protocol        = make_shared<AccessInterfaceProtocol_1149_1>();
  auto accessInterface = CreateAccessInterface(name, protocol);

  // ---------------- Create IR
  //
  auto irBypassSequence = BinaryVector(irBitsCount, 0xFF);
  auto ir               = CreateRegister(DEFAULT_TAP_IR_NAME, irBypassSequence, accessInterface);

  // ---------------- Create Linker
  //
  auto pathSelector = make_shared<DefaultBinaryPathSelector>(ir, muxPathsCount);
  auto linker       = CreateLinker(DEFAULT_TAP_MUX_NAME, pathSelector, accessInterface);

  // ---------------- Create bypass register
  //
  auto bypassBypassSequence = BinaryVector(1, 0xFF);
  auto bypassRegister       = CreateRegister(DEFAULT_TAP_MUX_BPY_NAME, bypassBypassSequence, linker);

  // ---------------- Set AccessInterface to forward append to the linker
  //
  accessInterface->SetChildAppender(linker);

  return accessInterface;
}
//
//  End of: SystemModel::CreateTap
//---------------------------------------------------------------------------


//! Returns node associated with a node identifier
//!
std::shared_ptr<SystemModelNode> SystemModel::GetNode (SystemModelNode::NodeIdentifier identifier) const
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
//  End of: SystemModel::GetNode
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

  auto identifier = node->GetIdentifier();
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
    THROW_LOGIC_ERROR("Does not support out of order node registration");
  }
}
//
//  End of: SystemModel::RegisterNode
//---------------------------------------------------------------------------


//===========================================================================
// End of SystemModel.cpp
//===========================================================================
