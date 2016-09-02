//===========================================================================
//                           NamesChecker.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file NamesChecker.cpp
//!
//! Implements class NamesChecker
//!
//===========================================================================

#include "NamesChecker.hpp"
#include "PathSelector.hpp"
#include "AccessInterfaceProtocol.hpp"
#include "Utility.hpp"

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
SystemModelCheckResult NamesChecker::Check ()
{
  if (!m_root)
  {
    ReportWarning("No SystemModel (sub-)tree to check");
  }
  else
  {
    ClearPaths();
    CheckParentNode(m_root.get());
  }

  auto result = MakeCheckResult();
  return result;
}
//
//  End of: NamesChecker::Check
//---------------------------------------------------------------------------



//! Checks name consistency of a parent node and sub-nodes
//!
void NamesChecker::CheckParentNode (const ParentNode* parent)
{
  CHECK_PARAMETER_NOT_NULL(parent, "Expect to be pass valid pointer");

  PushParentPath(parent);
  auto popAtExit = MakeScopeExit([=]() { PopParentPath(parent); });

  auto nextChild = parent->FirstChild();

  if (!nextChild)
  {
    return;
  }

  // ---------------- Prevent endless recursion with ill formed SystemModel
  //
  auto insertResult = m_processedNodes.insert(nextChild.get());
  if (!insertResult.second)
  {
    return;
  }

  set<string_view> childNames;
  set<string_view> ignoredNames;

  while (nextChild)
  {
    auto hasReportedAnError = CheckSiblingName(nextChild, childNames, ignoredNames);

    // ---------------- Recurse when child is also a parent
    //
    auto asParentNode = dynamic_pointer_cast<const ParentNode>(nextChild);
    if (asParentNode)
    {
      CheckParentNode(asParentNode.get());
    }
    else
    {
      auto leafPath = m_currentLogicalPath + nextChild->Name();

      if (!hasReportedAnError)
      {
        auto samePathCount = m_leafPaths.count(leafPath);

        if      (samePathCount == 1)
        {
          auto otherLeaf = m_leafPaths.find(leafPath)->second;

          ReportError(*nextChild, " Has same logical path \"" + leafPath + "\" than Register with id: " + to_string(otherLeaf->Identifier()));
        }
        else if (samePathCount != 0)
        {
          ReportError(*nextChild, " Has same logical path \""s + leafPath + "\" than " + to_string(samePathCount) + " other Registers");
        }
      }

      m_leafPaths.emplace(std::move(leafPath), nextChild.get());
    }

    nextChild = nextChild->NextSibling();
  }
}
//
//  End of: NamesChecker::CheckParentNode
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
//! @return true When an error has been reported, false otherwise
bool NamesChecker::CheckSiblingName (shared_ptr<const SystemModelNode> child, set<string_view>& childNames, set<string_view>& ignoredNames)
{
  auto initialErrorCount = ErrorsCount();

  auto        asParentNode = dynamic_pointer_cast<const ParentNode>(child);
  const auto& name         = child->Name();
  auto        noName       = name.empty() || (name == "unnamed");
  auto        ignored      = asParentNode && asParentNode->IgnoreForNodePath();

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

  return ErrorsCount() != initialErrorCount;
}
//
//  End of: NamesChecker::CheckSiblingName
//---------------------------------------------------------------------------


//! Clears paths related members
//!
void NamesChecker::ClearPaths ()
{
  m_currentPathNodes.clear();
  m_currentLogicalPath.clear();
  m_leafPaths.clear();
}
//
//  End of: NamesChecker::ClearPaths
//---------------------------------------------------------------------------


//! Removes parent node from path (if it is not ignored for path)
//!
void NamesChecker::PopParentPath (const ParentNode* parent)
{
  CHECK_PARAMETER_NOT_NULL(parent, "Should get only not nullptr");
  if (!parent->IgnoreForNodePath())
  {
    m_currentPathNodes.pop_back();
    RebuildLogicalPath();
  }
}
//
//  End of: NamesChecker::UpdateCurrentPath
//---------------------------------------------------------------------------


//! Append parent node to path (if it is not ignored for path)
//!
void NamesChecker::PushParentPath (const ParentNode* parent)
{
  CHECK_PARAMETER_NOT_NULL(parent, "Should get only not nullptr");
  if (!parent->IgnoreForNodePath())
  {
    m_currentPathNodes.push_back(parent);
    RebuildLogicalPath();
  }
}
//
//  End of: NamesChecker::UpdateCurrentPath
//---------------------------------------------------------------------------


//! Rebuilds current path name from, not ignored, ParentNode in path
//!
void NamesChecker::RebuildLogicalPath ()
{
  m_currentLogicalPath.clear();
  for (auto parent : m_currentPathNodes)
  {
    CHECK_VALUE_NOT_NULL(parent, "See PushParentPath why it saved nullptr");
    m_currentLogicalPath.append(parent->Name()).append(".");
  }
}
//
//  End of: NamesChecker::RebuildLogicalPath
//---------------------------------------------------------------------------



//===========================================================================
// End of NamesChecker.cpp
//===========================================================================
