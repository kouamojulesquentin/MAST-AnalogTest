//===========================================================================
//                           NamesChecker.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file NamesChecker.hpp
//!
//! Declares NamesChecker class
//!
//===========================================================================

#ifndef NAMESCHECKER_H__79FD76EF_7B97_4234_FD95_8B9C834098BB__INCLUDED_
  #define NAMESCHECKER_H__79FD76EF_7B97_4234_FD95_8B9C834098BB__INCLUDED_

#include "ParentNode.hpp"
#include "Checker.hpp"
#include <unordered_map>

#include <memory>
#include <set>
#include <vector>

namespace mast
{
//! Checks name coherency of a SystemModel (sub-)tree
//!
class DLL_EXPORT NamesChecker final : public Checker
{
  // ---------------- Public  Methods
  //
  public:
  ~NamesChecker() = default;
  NamesChecker()  = delete;
  NamesChecker(std::shared_ptr<ParentNode> root)
    : m_root (root)
  {}

  //! Checks SystemModelNodes name consistency
  //!
  static SystemModelCheckResult Check(std::shared_ptr<ParentNode> root) { return NamesChecker(root).Check(); };

  //! Checks SystemModelNodes name consistency
  //!
  virtual SystemModelCheckResult Check() override;

  // ---------------- Private  Methods
  //
  private:
  using string_view = std::experimental::string_view;

  bool CheckSiblingName (std::shared_ptr<const SystemModelNode> child,
                         std::set<string_view>&                 childNames,
                         std::set<string_view>&                 ignoredNames);

  void CheckParentNode    (const ParentNode* parent);
  void PopParentPath      (const ParentNode* parent);
  void PushParentPath     (const ParentNode* parent);
  void ClearPaths         ();
  void RebuildLogicalPath ();

  // ---------------- Private  Fields
  //
  private:
  using PathsMap_t = std::unordered_multimap<std::string, const SystemModelNode*>;

  std::shared_ptr<ParentNode>      m_root;               //!< First (top) node of system model tree
  std::set<const SystemModelNode*> m_processedNodes;     //!< Helper to detect loop within the SystemModel
  std::vector<const ParentNode*>   m_currentPathNodes;   //!< ParentNode that represent current path
  std::string                      m_currentLogicalPath; //!< Accrued path to current ParentNode (dot separated)
  PathsMap_t                       m_leafPaths;          //!< Saves logical path of tree leaves (Registers)
};
//
//  End of NamesChecker class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined NAMESCHECKER_H__79FD76EF_7B97_4234_FD95_8B9C834098BB__INCLUDED_
//===========================================================================
// End of NamesChecker.hpp
//===========================================================================
