//===========================================================================
//                           SystemModel.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModel.hpp
//!
//! Declares class SystemModel
//!
//===========================================================================

#ifndef SYSTEMMODEL_H__A9ED8877_8B39_4480_2B8B_2E92C212179C__INCLUDED_
  #define SYSTEMMODEL_H__A9ED8877_8B39_4480_2B8B_2E92C212179C__INCLUDED_

#include "SystemModelNodes.hpp"
#include "CheckResult.hpp"
#include <memory>
#include <vector>
#include <experimental/string_view>

namespace mast
{

//! Manages the system model tree
//!
class MAST_CORE_EXPORT SystemModel
{
  // ---------------- Public  Methods
  //
  public:
  ~SystemModel() = default;
  SystemModel()  = default;
  //! Initializes the system model with automatic root node setting (or not) with first created AccessInterface or Chain
  //!
  SystemModel(bool autoRootNode) : m_autoRootNode(autoRootNode) {}

  friend class SystemModelChecker;

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
  CheckResult Check() const;


  //! Creates a new AccessInterface node
  //!
  std::shared_ptr<AccessInterface> CreateAccessInterface (std::experimental::string_view           name,
                                                          std::shared_ptr<AccessInterfaceProtocol> protocol,
                                                          std::shared_ptr<ParentNode>              parentNode = nullptr);

  // ---------------- Creates a new Chain node
  //
  std::shared_ptr<Chain>    CreateChain    (std::experimental::string_view name,
                                            std::shared_ptr<ParentNode>    parentNode = nullptr);

  //! Creates a new Linker node
  //!
  std::shared_ptr<Linker>   CreateLinker   (std::experimental::string_view name,
                                            std::shared_ptr<PathSelector>  pathSelector,
                                            std::shared_ptr<ParentNode>    parentNode = nullptr);

  //! Creates a new Register node
  //!
  std::shared_ptr<Register> CreateRegister (std::experimental::string_view name,
                                            BinaryVector                   bypassSequence,
                                            std::shared_ptr<ParentNode>    parentNode = nullptr);

  std::shared_ptr<Register> CreateRegister (std::experimental::string_view name,
                                            BinaryVector                   bypassSequence,
                                            bool                           holdValue,
                                            std::shared_ptr<ParentNode>    parentNode = nullptr)
  {
    auto reg = CreateRegister(name, bypassSequence, parentNode);
    reg->SetHoldValue(holdValue);
    return reg;
  }

  //! Creates a new AccessInterfaceTranslator node
  //!
  std::shared_ptr<AccessInterfaceTranslator> CreateAccessInterfaceTranslator (std::experimental::string_view           name,
                                                          std::shared_ptr<AccessInterfaceTranslatorProtocol> protocol,
                                                          std::shared_ptr<ParentNode>              parentNode = nullptr
                                                          );

  //! Sets root node
  //!
  void SetRoot(std::shared_ptr<ParentNode> newRoot) { ReplaceRoot(newRoot, true); };

  //! Returns root node
  //!
  std::shared_ptr<ParentNode> Root() const { return m_root; }

  //! Replaces root node
  //!
  std::shared_ptr<ParentNode> ReplaceRoot(std::shared_ptr<ParentNode> newRoot, bool removeBeforeReplace);

  using NodeIdentifier = SystemModelNode::NodeIdentifier;

  std::shared_ptr<SystemModelNode> NodeWithId            (NodeIdentifier identifier) const;                                                                                //!< Returns node associated with a node identifier
  std::shared_ptr<AccessInterface> AccessInterfaceWithId (NodeIdentifier identifier) const { return std::dynamic_pointer_cast<AccessInterface> (NodeWithId(identifier)); } //!< Returns AccessInterface associated with a node identifier
  std::shared_ptr<Register>        RegisterWithId        (NodeIdentifier identifier) const { return std::dynamic_pointer_cast<Register>        (NodeWithId(identifier)); } //!< Returns Register associated with a node identifier
  std::shared_ptr<Linker>          LinkerWithId          (NodeIdentifier identifier) const { return std::dynamic_pointer_cast<Linker>          (NodeWithId(identifier)); } //!< Returns Linker associated with a node identifier
  std::shared_ptr<Chain>           ChainWithId           (NodeIdentifier identifier) const { return std::dynamic_pointer_cast<Chain>           (NodeWithId(identifier)); } //!< Returns Chain associated with a node identifier

  //! Disconnects a node from SystemModel tree
  //!
  void DisconnectNode(std::shared_ptr<SystemModelNode> node, std::shared_ptr<ParentNode> parentNode = nullptr);


  //! Removes a node from those managed by SystemModel (without much check)
  //!
  void RemoveNodeFromModel(std::shared_ptr<SystemModelNode> node);


  uint32_t RegistersCount() const { return m_totalRegisters; }

  // ---------------- Private  Methods
  //
  private:
  void RegisterNode(std::shared_ptr<SystemModelNode> node);  //!< Saves relation between node identifier and its instance
  void RemoveParentNode(std::shared_ptr<ParentNode> parent); //!< Releases the resources occupied by a node and its child recursively

  using TIdentifierMapping = std::vector<std::shared_ptr<SystemModelNode>>;

  TIdentifierMapping IdentifierMapping() const { return m_identifierMapping; }

  // ---------------- Private  Fields
  //
  private:
  uint32_t                    m_totalRegisters        = 0;    //!< Total number of registers in the model
  uint32_t                    m_totalPendingRegisters = 0;    //!< Number of registers currently "pending"
  bool                        m_autoRootNode          = true; //!< When true (at construction) root node is set with first created AccessInterface or Chain nodes
  std::shared_ptr<ParentNode> m_root;                         //!< First (top) node of system model tree
  TIdentifierMapping          m_identifierMapping;            //!< Maps a node identifier to a node instance
};
//
//  End of SystemModel class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SYSTEMMODEL_H__A9ED8877_8B39_4480_2B8B_2E92C212179C__INCLUDED_

//===========================================================================
// End of SystemModel.hpp
//===========================================================================
