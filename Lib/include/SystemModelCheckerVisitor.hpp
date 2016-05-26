//===========================================================================
//                           SystemModelCheckerVisitor.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelCheckerVisitor.hpp
//!
//! Declares SystemModelCheckerVisitor class
//!
//===========================================================================


#ifndef SYSTEMMODELCHECKERVISITOR_H__51E1518C_6330_4646_7A6_B51ECBAB1C6A__INCLUDED_
  #define SYSTEMMODELCHECKERVISITOR_H__51E1518C_6330_4646_7A6_B51ECBAB1C6A__INCLUDED_

#include "SystemModelCheckResult.hpp"
#include "SystemModelVisitor.hpp"
#include "SystemModel.hpp"

#include <memory>
#include <vector>
#include <sstream>

namespace mast
{
//! Visits all nodes of a SystemModel to detect coherence issues
//!
//! @note This is intended to be used by SystemModel::Check()
//!
class DLL_EXPORT SystemModelCheckerVisitor final : public SystemModelVisitor
{
  // ---------------- Public  Methods
  //
  public:
  ~SystemModelCheckerVisitor() = default;
  SystemModelCheckerVisitor()  = delete;
  SystemModelCheckerVisitor(const SystemModel& model)
    : m_root              (model.Root())
    , m_identifierMapping (model.IdentifierMapping())
  {}


  virtual void VisitAccessInterface (AccessInterface& accessInterface) override;
  virtual void VisitChain           (Chain&           chain)           override;
  virtual void VisitLinker          (Linker&          linker)          override;
  virtual void VisitRegister        (Register&        reg)             override;

  //! Checks SystemModel consistency
  //!
  //! @see CheckIdentifiers and CheckTree
  //!
  SystemModelCheckResult Check();

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

  //! Builds up a SystemModelCheckResult from currently selected issues
  //!
  SystemModelCheckResult  MakeCheckResult();

  // ---------------- Private  Methods
  //
  private:
  void CheckParentNode (std::shared_ptr<const ParentNode> parent);
  bool CheckChildNode  (std::shared_ptr<const ParentNode> parent, std::shared_ptr<const SystemModelNode> child);

  void Report (std::experimental::string_view  message, uint32_t& counter, std::ostringstream& os);

  void ReportInfo    (std::experimental::string_view message) { Report(message, m_infosCount,    m_infos);    }
  void ReportWarning (std::experimental::string_view message) { Report(message, m_warningsCount, m_warnings); }
  void ReportError   (std::experimental::string_view message) { Report(message, m_errorsCount,   m_errors);   }

  void ReportWarning (const SystemModelNode& node, std::experimental::string_view message);
  void ReportError   (const SystemModelNode& node, std::experimental::string_view message);

  static std::ostringstream& Stream(std::ostringstream& os, std::experimental::string_view header, const SystemModelNode& node);
  static std::ostringstream& Stream(std::ostringstream& os, const SystemModelNode& node) { return Stream(os, "", node); }

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
  TCollectedNodeInfo                m_collectedNodeInfo; //!< Collects nodes info when scanning tree structure

  std::shared_ptr<const ParentNode> m_root;              //!< First (top) node of system model tree
  TIdentifierMapping                m_identifierMapping; //!< Maps a node identifier to a node instance
  std::ostringstream                m_infos;             //!< Collects info messages
  std::ostringstream                m_warnings;          //!< Collects warning messages
  std::ostringstream                m_errors;            //!< Collects error messages
  uint32_t                          m_infosCount    = 0; //!< Total number of collected info messages
  uint32_t                          m_warningsCount = 0; //!< Total number of collected warnings messages
  uint32_t                          m_errorsCount   = 0; //!< Total number of collected errors messages
};
//
//  End of SystemModelCheckerVisitor class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SYSTEMMODELCHECKERVISITOR_H__51E1518C_6330_4646_7A6_B51ECBAB1C6A__INCLUDED_

//===========================================================================
// End of SystemModelCheckerVisitor.hpp
//===========================================================================
