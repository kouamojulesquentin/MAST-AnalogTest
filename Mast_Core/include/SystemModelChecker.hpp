//===========================================================================
//                           SystemModelChecker.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelChecker.hpp
//!
//! Declares SystemModelChecker class
//!
//===========================================================================


#ifndef SYSTEMMODELCHECKER_H__BB1485FA_44D5_40BA_E5A8_C23F1A1BCC6__INCLUDED_
  #define SYSTEMMODELCHECKER_H__BB1485FA_44D5_40BA_E5A8_C23F1A1BCC6__INCLUDED_

#include "CheckResult.hpp"
#include "SystemModelVisitor.hpp"
#include "SystemModel.hpp"
#include "SystemModelCheckerBase.hpp"

#include <memory>
#include <vector>
#include <set>

namespace mast
{
//! Visits all nodes of a SystemModel to detect coherence issues
//!
//! @note This is intended to be used by SystemModel::Check()
//!
class SystemModelChecker final : public SystemModelCheckerBase, public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  ~SystemModelChecker() = default;
  SystemModelChecker()  = delete;
  SystemModelChecker(const SystemModel& model)
    : m_root              (model.Root())
    , m_identifierMapping (model.IdentifierMapping())
  {}


  virtual void VisitAccessInterface (AccessInterface& accessInterface) override;
  virtual void VisitAccessInterfaceTranslator (AccessInterfaceTranslator&accessInterfaceTranslator) override;
  virtual void VisitChain           (Chain&           chain)           override;
  virtual void VisitLinker          (Linker&          linker)          override;
  virtual void VisitRegister        (Register&        reg)             override;

  //! Checks SystemModel consistency
  //!
  //! @see CheckIdentifiers and CheckTree
  //!
  static CheckResult Check(const SystemModel& model) { return SystemModelChecker(model).Check(); };

  //! Checks SystemModel consistency
  //!
  //! @see CheckIdentifiers and CheckTree
  //!
  virtual CheckResult Check() override;

  //! Checks consistency of identifiers:
  //!
  //! @note - Each used identifier must refere to a node that has the very same identifier
  //!       - Unused identifiers are collected as "info"
  //!
  void CheckIdentifiers ();

  //! Checks consistency of SystemModel tree structure
  //!
  //! @note
  //!   - Each parent node has at least one child otherwise a warning is issued
  //!   - Each node is managed by the SystemModel
  //!   - Each node is reachable (no dangling node)
  //!   - Each node has one and only one parent (except root that has no parent)
  //!   - Each child is only appended once in its parent
  //!   - Each linker has a number of children that matches its selector or an warning is issued when there are to few
  //!     children and an error when there are too much
  //!
  void CheckTree ();

  // ---------------- Private  Methods
  //
  private:
  using string_view = std::experimental::string_view;

  void CheckAccessInterface     ();
  void CheckParentNode          (std::shared_ptr<const ParentNode> parent);
  bool CheckChildNode           (std::shared_ptr<const ParentNode> parent, std::shared_ptr<const SystemModelNode> child);
  void CheckNumberOfEndPoints (std::shared_ptr<AccessInterface>  accessInterface);
  void CheckSiblingName         (std::shared_ptr<SystemModelNode>, std::set<string_view>& childNames, std::set<string_view>& ignoredNames);

  // ---------------- Private  Fields
  //
  private:
  using TIdentifierMapping = SystemModel::TIdentifierMapping;
  struct CollectedNodeInfo
  {
    std::shared_ptr<const ParentNode>      parent;
    std::shared_ptr<const SystemModelNode> node;
  };

  using TCollectedNodeInfo = std::vector<CollectedNodeInfo>;

  TCollectedNodeInfo          m_collectedNodeInfo; //!< Collects nodes info when scanning tree structure
  std::shared_ptr<ParentNode> m_root;              //!< First (top) node of system model tree
  TIdentifierMapping          m_identifierMapping; //!< Maps a node identifier to a node instance
};
//
//  End of SystemModelChecker class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined SYSTEMMODELCHECKER_H__BB1485FA_44D5_40BA_E5A8_C23F1A1BCC6__INCLUDED_

//===========================================================================
// End of SystemModelChecker.hpp
//===========================================================================
