//===========================================================================
//                           SystemModelChecker.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelChecker.cpp
//!
//! Implements class SystemModelChecker
//!
//===========================================================================

#include "SystemModelChecker.hpp"
#include "PathSelector.hpp"
#include "AccessInterfaceProtocol.hpp"
#include "NamesChecker.hpp"

using namespace mast;
using std::string;
using std::to_string;
using std::experimental::string_view;
using std::shared_ptr;
using std::dynamic_pointer_cast;
using std::vector;
using std::set;
using std::ostringstream;


//! Checks SystemModel consistency
//!
//! @see CheckIdentifiers and CheckTree
//!
SystemModelCheckResult SystemModelChecker::Check ()
{
  CheckIdentifiers();
  CheckTree();

  auto nameCheckerResult = NamesChecker::Check(m_root);
  auto result            = MakeCheckResult();

  result.Merge(nameCheckerResult);
  return result;
}
//
//  End of: SystemModelChecker::Check
//---------------------------------------------------------------------------


//! Checks that root is an AccessInterface or a Chain with only AccessInterface
//! and there is no more AccessInterface below the hierarchy
//!
void SystemModelChecker::CheckAccessInterface ()
{
  auto rootAsAI    = dynamic_pointer_cast<AccessInterface > (m_root);
  auto rootAsChain = dynamic_pointer_cast<Chain>(m_root);

  if (rootAsAI)
  {
    CheckNumberOfEndPoints(rootAsAI);
//+    CheckNoAccessInterfaceBellow(rootAsAI);
  }
  else if (rootAsChain)
  {
    auto aiCount = 0u;
    auto child = rootAsChain->FirstChild();
    while (child)
    {
      auto childAsAi = dynamic_pointer_cast<AccessInterface>(child);
      if (childAsAi)
      {
        ++aiCount;
        CheckNumberOfEndPoints(childAsAi);
//+        CheckNoAccessInterfaceBellow(childAsAi);
      }
      else
      {
        ReportError(*child, ", children of Chain root node is not an AccessInterface");
      }
      child = child->NextSibling();
    }

    if (aiCount == 0)
    {
      ReportError(*rootAsChain, " has no AccessInterface child");
    }
  }
  else
  {
    ReportError(*m_root, " is neither an AccessInterface nor a Chain");
  }
}
//
//  End of: SystemModelChecker::CheckAccessInterface
//---------------------------------------------------------------------------



//! Checks consistency of identifiers mapping:
//!
//! @note - Each used identifier must refere to a node that has the very same identifier
//!       - Unused identifiers are collected as "info"
//!
void SystemModelChecker::CheckIdentifiers ()
{
  auto idCount = m_identifierMapping.size();

  for (size_t id = 0 ; id < idCount ; ++id)
  {
    auto node = m_identifierMapping[id];
    if (node)
    {
      if (node->Identifier() != static_cast<SystemModelNode::NodeIdentifier>(id))
      {
        ostringstream os;
        Stream(os, "Found", *node) << "' while expecting it to have id: '" << id << "'";
        ReportError(os.str());
      }
    }
    else
    {
      ostringstream os;
      os << "Identifier '" << id << "' is not associated with a node";
      ReportInfo(os.str());
    }
  }
}
//
//  End of: SystemModelChecker::CheckIdentifiers
//---------------------------------------------------------------------------



//! Checks consistency of a parent node
//!
//! @note It checks that:
//!   - Each child has one and only one parent
//!   - Each child is only appended once in its parent
//!
//! @return true when this is the first time this child node has been checked.
//!
bool SystemModelChecker::CheckChildNode (shared_ptr<const ParentNode> parent, shared_ptr<const SystemModelNode> child)
{
  auto checkedOnce = true;

  // ---------------- Check identifier mapping consistency for that node
  //
  auto id = static_cast<TCollectedNodeInfo::size_type>(child->Identifier());

  if (    (id >= m_collectedNodeInfo.size())
      ||  !m_identifierMapping[id]
     )
  {
    ReportError(*child, " is not managed by SystemModel");
  }
  else if (child != m_identifierMapping[id])
  {
    auto otherNode = m_identifierMapping[id];

    ostringstream os;
    Stream(os, *child);
    Stream(os, " has same identifier as", *otherNode) << " ==> it is probably not managed by SystemModel";

    ReportError(os.str());
  }
  else   // Stop checking when not managed by SystemModel
  {
    auto& nodeInfos = m_collectedNodeInfo[id];

    checkedOnce = nodeInfos.node == nullptr;

    // ---------------- Check only one parent
    //
    if (nodeInfos.parent)
    {
      ostringstream os;

      Stream(os, *child);

      if (nodeInfos.parent != parent)
      {
        Stream(os, ", child of",         *parent);
        Stream(os, ", is also child of", *nodeInfos.parent);
      }
      else if (child == child->NextSibling())
      {
        Stream(os, " has been appended twice in a row to", *parent);
      }
      else
      {
        Stream(os, " has been appended twice to", *parent);
      }

      ReportError(os.str());

      checkedOnce = false;
    }
    else
    {
      nodeInfos.parent = parent;
    }

    nodeInfos.node = child;
  }

  return checkedOnce;
}
//
//  End of: SystemModelChecker::CheckChildNode
//---------------------------------------------------------------------------


//! Checks the an interface has no more endpoint than is supported by its protocol
//!
//! @note Does no check when there is no protocol (this is checked everywhere)
//!
void SystemModelChecker::CheckNumberOfEndPoints (shared_ptr<AccessInterface> accessInterface)
{
  auto protocol = accessInterface->Protocol();

  if (protocol)
  {
    auto maxEndPoints = protocol->MaxSupportedEndPoints();
    auto childrenCount  = accessInterface->DirectChildrenCount();

    if (childrenCount >= maxEndPoints) // Max endpoints includes pseudo endpoint reserved to do "Reset" action
    {
      auto message =   " has too many children ("                + to_string(childrenCount)
                     + ") ; its protocol supports a maximum of " + to_string(maxEndPoints)
                     + " endpoints (including one 'pseudo endpoint' reserved for reset action)";
      ReportError(*accessInterface, message);
    }
    else if (maxEndPoints > (childrenCount + 1u))
    {
      auto diff = maxEndPoints - childrenCount;
      if (diff < 100u)  // Do not report for virtually unlimited maxEndPoints
      {
        ReportInfo(*accessInterface,   " has only "                                                + to_string(childrenCount)
                                     + " children even though its protocol supports a maximum of " + to_string(maxEndPoints)
                                     + " endpoints (including one 'pseudo endpoint' reserved for reset action)");
      }
    }
  }
}
//
//  End of: SystemModelChecker::CheckNumberOfEndPoints
//---------------------------------------------------------------------------



//! Checks consistency of a parent node
//!
//! @note It checks that
//!   - Each parent node has at least one child otherwise a warning is issued
//! @note Names are checked elsewhere
//!
void SystemModelChecker::CheckParentNode (shared_ptr<const ParentNode> parent)
{
  auto nextChild = parent->FirstChild();

  if (!nextChild)
  {
    ReportWarning(*parent, " has no child");
  }
  else
  {
    while (nextChild)
    {
      if (nextChild == parent)
      {
        ReportError(*nextChild, " has been appended into itself");
      }
      else
      {
        auto checkedOnce = CheckChildNode(parent, nextChild);

        if (!checkedOnce)
        {
          break;  // Avoid infinite loop when a node has been appended twice (same parent or not)
        }
        else
        {
          nextChild->Accept(*this);   // Do check specific to node type

          // ---------------- Recurse when child is also a parent
          //
          auto asParentNode = dynamic_pointer_cast<const ParentNode>(nextChild);
          if (asParentNode)
          {
            CheckParentNode(asParentNode);
          }
        }
      }

      nextChild = nextChild->NextSibling();
    }
  }
}
//
//  End of: SystemModelChecker::CheckParentNode
//---------------------------------------------------------------------------


//! Checks name of a child node relative to its previous sibling(s)
//!
//! @note It checks for no name and duplicate names
//!       - For no name, it is acceptable unless for parent nodes that are not declared to be ignored for node paths.
//!         Otherwise, it is just an info
//!       - For duplicates, it is acceptable for parent nodes that are declared to be ignored for node paths.
//!         Otherwise it is a warning or info
//!
//! @param [in]      child        Child node to check
//! @param [in, out] childNames   Set names of already processed children that are not ignored path
//! @param [in, out] ignoredNames Set names of already processed children that are ignored for paths
//!
void SystemModelChecker::CheckSiblingName (shared_ptr<SystemModelNode> child, set<string_view>& childNames, set<string_view>& ignoredNames)
{
  auto asParentNode = dynamic_pointer_cast<const ParentNode>(child);
  auto name         = child->Name();
  auto noName       = name.empty() || (name == "unnamed");
  auto ignored      = asParentNode && asParentNode->IgnoreForNodePath();

  if (noName)
  {
    auto isParent = asParentNode != nullptr;

    if      (!isParent) ReportInfo    (*child, " Has no valid name");
    else if (!ignored)  ReportWarning (*child, " Has no valid name, even though it is parent node not ignored for node paths");

  }
  else
  {
    auto sameNameAsIgnored    = ignoredNames.count(name) != 0;
    auto sameNameAsNotIgnored = childNames.count(name)   != 0;

         if (sameNameAsNotIgnored && !ignored) ReportError (*child, " Has same name as a previous sibling");
    else if (sameNameAsNotIgnored && ignored)  ReportInfo  (*child, " Has same name as a previous sibling (it is ignored for node paths)");
    else if (sameNameAsIgnored    && !ignored) ReportInfo  (*child, " Has same name as a previous sibling (that is ignored for node paths)");
    else if (sameNameAsIgnored    && ignored)  ReportInfo  (*child, " Has same name as previous sibling (both are ignored for node paths)");

    ignored ? ignoredNames.emplace(name)
            : childNames.emplace(name);
  }
}
//
//  End of: SystemModelChecker::CheckSiblingName
//---------------------------------------------------------------------------


//! Checks consistency of SystemModel tree structure
//!
//! @note
//!   - Each node has one and only one parent (except root that has no parent)
//!   - Each parent node has at least one child otherwise a warning is issued
//!   - Each child is only appended once in its parent
//!   - Each node is reachable (no dangling node)
//!   - Each linker has a number of chidren that matches its selector or an warning is issued when there are to few
//!     children and an error when there are too much
//!
void SystemModelChecker::CheckTree ()
{
  if (!m_root)
  {
    ReportWarning("SystemModel has no root node");
    return;
  }

  if (m_identifierMapping.empty())
  {
    ReportError("There is no mapping between identifiers and nodes");
  }

  m_collectedNodeInfo.clear();      // Allow multiple call to CheckTree
  m_collectedNodeInfo.resize(m_identifierMapping.size());

  m_root->Accept(*this);            // Process parent node specifically

  // ---------------- AccessInterface check
  //
  CheckAccessInterface();
  CheckParentNode(m_root);

  // ---------------- Check that each node is reachable (no dangling node)
  //
  auto idCount = m_identifierMapping.size();

  for (size_t id = 0 ; id < idCount ; ++id)
  {
    auto node = m_identifierMapping[id];
    if (node && (node != m_root))
    {
      auto& collectedNodeInfo = m_collectedNodeInfo[id];

      if (!collectedNodeInfo.node)
      {
        ReportError(*node, " has not been seen by checker tree traversal (may be unconnected)");
      }
    }
  }
}
//
//  End of: SystemModelChecker::CheckTree
//---------------------------------------------------------------------------



//! Checks consistency specific to AccessInterface nodes
//!
void SystemModelChecker::VisitAccessInterface (AccessInterface& accessInterface)
{
  auto protocol = accessInterface.Protocol();

  if (!protocol)
  {
    ReportWarning(accessInterface, " has no associated AccessInterfaceProtocol");
  }
}
//
//  End of: SystemModelChecker::VisitAccessInterface
//---------------------------------------------------------------------------


//! Checks consistency specific to AccessInterfaceTranslator nodes
//!
void SystemModelChecker::VisitAccessInterfaceTranslator (AccessInterfaceTranslator& )
{
 //TODO: check child is an AccessInterface with a Raw Protocol
return;
}
//
//  End of: SystemModelChecker::VisitAccessInterfaceTranslator
//---------------------------------------------------------------------------


//! Checks consistency specific to Chain nodes
//!
void SystemModelChecker::VisitChain (Chain& )
{
  // Nothing to do yet
}
//
//  End of: SystemModelChecker::VisitChain
//---------------------------------------------------------------------------


//! Checks consistency specific to Linker nodes
//!
//! @note Checked point are:
//!   -
//!   - Each linker has a number of children that matches its selector or an warning is issued when there are to few
//!     children and an error when there are too much
//!
void SystemModelChecker::VisitLinker (Linker& linker)
{
  //! @todo [JFC]-[May/24/2016]: Implement SystemModelChecker::VisitLinker()

  auto childrenCount = linker.DirectChildrenCount();
  auto pathSelector  = linker.Selector();
  if (!pathSelector)
  {
    ReportError(linker, " has no path selector");
  }
  else
  {
    auto selectablePaths = pathSelector->SelectablePaths();

    if      (selectablePaths == 0)
    {
      ReportWarning(linker, " has a selector that can select no path at all");
    }
    else if (selectablePaths == 1)
    {
      if (!pathSelector->CanSelectNone())
      {
        ReportWarning(linker, " has a selector that can select only 1 path (and cannot select none)");
      }
    }

    if (childrenCount != 0)  // No child has already been reported by CheckParentNode
    {
      if (childrenCount < selectablePaths)
      {
        ostringstream os;
        Stream(os, linker) << " has only "  << childrenCount   << (childrenCount == 1 ? " child" : " children");
        os << ", even though it can select " << selectablePaths << " paths";

        ReportWarning(os.str());
      }
      else if (childrenCount > selectablePaths)
      {
        ostringstream os;
        Stream(os, linker) << " has "  << childrenCount << (childrenCount == 1 ? " child" : " children");
        os << ", even though it can only select " << selectablePaths << " paths";

        ReportError(os.str());
      }
    }
  }
}
//
//  End of: SystemModelChecker::VisitLinker
//---------------------------------------------------------------------------


//! Checks consistency specific to Register nodes
//!
void SystemModelChecker::VisitRegister (Register& )
{
  // Nothing to do yet
}
//
//  End of: SystemModelChecker::VisitRegister
//---------------------------------------------------------------------------


//===========================================================================
// End of SystemModelChecker.cpp
//===========================================================================
