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

namespace mast
{

//! Manages the system model tree
//!
//!
class SystemModel
{
  // ---------------- Public  Methods
  //
  public:
  ~SystemModel() = default;
  SystemModel()  = default;

  Tap*             CreateTap             (std::experimental::string_view name = mast::DEFAULT_TAP_NAME); //!< Creates a new Tap node
  AccessInterface* CreateAccessInterface (std::experimental::string_view name);                          //!< Creates a new AccessInterface node

  Chain*    CreateChain    (ParentNode* parentNode, std::experimental::string_view name);                                                       //!< Creates a new Chain node
  Linker*   CreateLinker   (ParentNode* parentNode, std::experimental::string_view name, PathSelector* pathSelector);                           //!< Creates a new Linker node
  Register* CreateRegister (ParentNode* parentNode, std::experimental::string_view name, uint32_t      bitsCount, BinaryVector bypassSequence); //!< Creates a new Register node

  AccessInterface* GetRoot() const { return m_root; }

  //! Releases the resources occupied by node data structure recursively
  //!
  void DestroyNode(SystemModelNode* node);
  //+ (JFC April/20/2016): How to report parent node that a node has been destroyed

  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private  Methods
  //
  private:
  void RegisterNode(SystemModelNode::NodeIdentifier identifier, SystemModelNode* node);  //!< Saves relation between node identifier and its instance

  // ---------------- Private  Fields
  //
  private:
  uint32_t         m_totalRegister        = 0;       //!< Total number of register in the model
  uint32_t         m_totalPendingRegister = 0;       //!< Number of registers currently "pending"
  AccessInterface* m_root                 = nullptr; //!< First (top) node of system model tree
};
//
//  End of SystemModel class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SYSTEMMODEL_H__A9ED8877_8B39_4480_2B8B_2E92C212179C__INCLUDED_

//===========================================================================
// End of SystemModel.hpp
//===========================================================================
