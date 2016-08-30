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

#include <memory>
#include <set>

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

  void CheckParentNode  (std::shared_ptr<const ParentNode> parent);
  void CheckSiblingName (std::shared_ptr<const SystemModelNode>, std::set<string_view>& childNames, std::set<string_view>& ignoredNames);

  // ---------------- Private  Fields
  //
  private :
  std::shared_ptr<ParentNode>      m_root;           //!< First (top) node of system model tree
  std::set<const SystemModelNode*> m_processedNodes; //!< Helper to detect loop within the SystemModel
};
//
//  End of NamesChecker class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined NAMESCHECKER_H__79FD76EF_7B97_4234_FD95_8B9C834098BB__INCLUDED_
//===========================================================================
// End of NamesChecker.hpp
//===========================================================================
