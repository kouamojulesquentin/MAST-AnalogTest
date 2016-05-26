//===========================================================================
//                           SystemModelCheckerVisitor.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelCheckerVisitor.cpp
//!
//! Implements class SystemModelCheckerVisitor
//!
//===========================================================================

#include "SystemModelCheckerVisitor.hpp"
#include "PathSelector.hpp"

using namespace mast;
using std::string;
using std::experimental::string_view;
using std::shared_ptr;
using std::dynamic_pointer_cast;
using std::vector;
using std::ostringstream;



//! Checks SystemModel consistency
//!
//! @see CheckIdentifiers and CheckTree
//!
SystemModelCheckResult SystemModelCheckerVisitor::Check ()
{
  CheckIdentifiers();
  CheckTree();

  auto result = MakeCheckResult();
  return result;
}
//
//  End of: SystemModelCheckerVisitor::Check
//---------------------------------------------------------------------------



//! Checks consistency of identifiers mapping:
//!
//! @note - Each used identifier must refere to a node that has the very same identifier
//!       - Unused identifiers are collected as "info"
//!
void SystemModelCheckerVisitor::CheckIdentifiers ()
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
      os << "Identifier '" << id << "' is not used";
      ReportInfo(os.str());
    }
  }
}
//
//  End of: SystemModelCheckerVisitor::CheckIdentifiers
//---------------------------------------------------------------------------



//! Checks consistency of a parent node
//!
//! @note It checks that:
//!   - Each child has one and only one parent
//!   - Each child is only appended once in its parent
//!
//! @return true when this is the first time this child node has been checked.
//!
bool SystemModelCheckerVisitor::CheckChildNode (shared_ptr<const ParentNode> parent, shared_ptr<const SystemModelNode> child)
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
//  End of: SystemModelCheckerVisitor::CheckChildNode
//---------------------------------------------------------------------------



//! Checks consistency of a parent node
//!
//! @note It checks that:
//!   - Each parent node has at least one child otherwise a warning is issued
//!
void SystemModelCheckerVisitor::CheckParentNode (shared_ptr<const ParentNode> parent)
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
//  End of: SystemModelCheckerVisitor::CheckParentNode
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
void SystemModelCheckerVisitor::CheckTree ()
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
        ReportError(*node, " is not reachable by tree traversal");
      }
    }
  }
}
//
//  End of: SystemModelCheckerVisitor::CheckTree
//---------------------------------------------------------------------------



//! Builds up a SystemModelCheckResult from currently selected issues
//!
SystemModelCheckResult SystemModelCheckerVisitor::MakeCheckResult ()
{
  SystemModelCheckResult result;

  result.errors        = m_errors.str();
  result.warnings      = m_warnings.str();
  result.infos         = m_infos.str();

  result.errorsCount   = m_errorsCount;
  result.warningsCount = m_warningsCount;
  result.infosCount    = m_infosCount;

  return result;
}
//
//  End of: SystemModelCheckerVisitor::MakeCheckResult
//---------------------------------------------------------------------------


//! Reports an error, warning or info
//!
void SystemModelCheckerVisitor::Report (string_view message, uint32_t& counter, ostringstream& os)
{
  if (counter != 0)
  {
    os << std::endl;
  }

  os << "  - " << message;
  ++counter;
}
//
//  End of: SystemModelCheckerVisitor::ReportError
//---------------------------------------------------------------------------


//! Reports an error for a node
//!
void SystemModelCheckerVisitor::ReportError (const SystemModelNode& node, string_view message)
{
  ostringstream os;
  Stream(os, node) << message;

  ReportError(os.str());
}
//
//  End of: SystemModelCheckerVisitor::ReportError
//---------------------------------------------------------------------------


//! Reports a warning for a node
//!
void SystemModelCheckerVisitor::ReportWarning (const SystemModelNode& node, string_view message)
{
  ostringstream os;
  Stream(os, node) << message;

  ReportWarning(os.str());
}
//
//  End of: SystemModelCheckerVisitor::ReportWarning
//---------------------------------------------------------------------------



//! Helper that streams into os, an header, node name and identifier
//!
ostringstream& SystemModelCheckerVisitor::Stream (ostringstream& os, string_view header, const SystemModelNode& node)
{
  if (!header.empty())
  {
    os << header << " ";
  }

  os << node.TypeName() << " '" << node.Name() << "' (id: " << node.Identifier() << ")";
  return os;
}
//
//  End of: SystemModelCheckerVisitor::Stream
//---------------------------------------------------------------------------



//! Checks consistency specific to AccessInterface nodes
//!
void SystemModelCheckerVisitor::VisitAccessInterface (AccessInterface& accessInterface)
{
  //! @todo [JFC]-[May/24/2016]: Implement SystemModelCheckerVisitor::VisitAccessInterface()
  //!
}
//
//  End of: SystemModelCheckerVisitor::VisitAccessInterface
//---------------------------------------------------------------------------


//! Checks consistency specific to Chain nodes
//!
void SystemModelCheckerVisitor::VisitChain (Chain& )
{
  // Nothing to do yet
}
//
//  End of: SystemModelCheckerVisitor::VisitChain
//---------------------------------------------------------------------------


//! Checks consistency specific to Linker nodes
//!
//! @note Checked point are:
//!   -
//!   - Each linker has a number of children that matches its selector or an warning is issued when there are to few
//!     children and an error when there are too much
//!
void SystemModelCheckerVisitor::VisitLinker (Linker& linker)
{
  //! @todo [JFC]-[May/24/2016]: Implement SystemModelCheckerVisitor::VisitLinker()

  auto childrenCount = linker.DirectChildrenCount();
  auto pathSelector  = linker.Selector();
  if (!pathSelector)
  {
    ReportError(linker, " has no path selector");
  }
  else if (childrenCount != 0)  // No child has already been reported by CheckParentNode
  {
    auto selectablePaths = pathSelector->SelectablePaths();

    if      (selectablePaths == 0)
    {
      ReportError(linker, " has a selector that can select no path at all");
    }
    else if (selectablePaths == 1)
    {
      if (!pathSelector->CanSelectNone())
      {
        ReportWarning(linker, " has a selector that can select only 1 path (and cannot select none)");
      }
    }

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
//
//  End of: SystemModelCheckerVisitor::VisitLinker
//---------------------------------------------------------------------------


//! Checks consistency specific to Register nodes
//!
void SystemModelCheckerVisitor::VisitRegister (Register& )
{
  // Nothing to do yet
}
//
//  End of: SystemModelCheckerVisitor::VisitRegister
//---------------------------------------------------------------------------


//===========================================================================
// End of SystemModelCheckerVisitor.cpp
//===========================================================================
