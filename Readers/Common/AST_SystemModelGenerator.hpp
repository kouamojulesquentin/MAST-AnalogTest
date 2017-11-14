//===========================================================================
//                           AST_SystemModelGenerator.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_SystemModelGenerator.hpp
//!
//! Declares AST_SystemModelGenerator class
//!
//===========================================================================

#ifndef AST_SYSTEMMODELGENERATOR_H__4AD33E64_B5F0_44D9_839C_E286323534C__INCLUDED_
  #define AST_SYSTEMMODELGENERATOR_H__4AD33E64_B5F0_44D9_839C_E286323534C__INCLUDED_

#include "AST_Visitor.hpp"
#include <memory>

namespace mast
{
class SystemModel;
class SystemModelNode;
class SystemModelBuilder;
} // End of namespace mast

namespace Parsers
{
//! Generates Mast SystemModel from AST
//!
class AST_SystemModelGenerator final : public AST_Visitor
{
  // ---------------- Public Methods
  //
  public:
  ~AST_SystemModelGenerator();
  AST_SystemModelGenerator() = delete;
  AST_SystemModelGenerator(std::shared_ptr<mast::SystemModel> systemModel);

  //! Generates a SystemModel (sub-)tree from AST_Network
  //!
  std::shared_ptr<mast::SystemModelNode> Generate(AST_Network* network);

  virtual void Visit_AccessLink    (AST_AccessLink*    instance)      override;
  virtual void Visit_Instance      (AST_Instance*      instance)      override;
  virtual void Visit_Network       (AST_Network*       network)       override;
  virtual void Visit_Module        (AST_Module*        module)        override;
  virtual void Visit_Port          (AST_Port*          port)          override;
  virtual void Visit_ScanInterface (AST_ScanInterface* scanInterface) override;
  virtual void Visit_ScanMux       (AST_ScanMux*       scanMux)       override;
  virtual void Visit_ScanRegister  (AST_ScanRegister*  scanRegister)  override;
  virtual void Visit_Source        (AST_Source*        source)        override;

  // ---------------- Private Methods
  //
  private:

  // ---------------- Private Fields
  //
  private:
  std::shared_ptr<mast::SystemModel>        m_systemModel;                //!< SystemModel currently being built
  std::unique_ptr<mast::SystemModelBuilder> m_builder;                    //!< Helper to build SystemModel nodes
  std::shared_ptr<mast::SystemModelNode>    m_parsedTopNode;              //!< SystemModel tree build from ICL file
  std::shared_ptr<mast::SystemModelNode>    m_lastCreatedNode;            //!< Serves as return value for Visit_xxx
  const AST_Source*                         m_lastEntitySource = nullptr; //!< Serves as return value for Visit_xxx
  AST_Network*                              m_network          = nullptr; //!< Test network AST used to generate SystemModel tree
};
//
//  End of AST_SystemModelGenerator class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_SYSTEMMODELGENERATOR_H__4AD33E64_B5F0_44D9_839C_E286323534C__INCLUDED_

//===========================================================================
// End of AST_SystemModelGenerator.hpp
//===========================================================================
