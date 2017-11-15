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

#include <memory>
#include <tuple>

namespace mast
{
class Chain;
class SystemModel;
class SystemModelNode;
class SystemModelBuilder;
} // End of namespace mast

namespace Parsers
{
//+class AST_AccessLink;
class AST_Instance;
class AST_Module;
class AST_Network;
//+class AST_Port;
//+class AST_ScanInterface;
//+class AST_ScanMux;
class AST_ScanRegister;
class AST_Source;

//! Generates Mast SystemModel from AST
//!
class AST_SystemModelGenerator final
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

  // ---------------- Private Methods
  //
  private:
  std::tuple<std::shared_ptr<mast::SystemModelNode>, const AST_Source*> Generate_Instance (const AST_Instance*     instance, const AST_Module* instanceModule);
  std::tuple<std::shared_ptr<mast::SystemModelNode>, const AST_Source*> Generate_Register (const AST_ScanRegister* scanRegister);

  std::shared_ptr<mast::SystemModelNode> Generate_Network (const AST_Network* network);
  void                                   Generate_Module (mast::Chain* chain, const AST_Module* module);

  // ---------------- Private Fields
  //
  private:
  std::shared_ptr<mast::SystemModel>        m_systemModel;       //!< SystemModel currently being built
  std::unique_ptr<mast::SystemModelBuilder> m_builder;           //!< Helper to build SystemModel nodes
  std::shared_ptr<mast::SystemModelNode>    m_parsedTopNode;     //!< SystemModel tree build from ICL file
  AST_Network*                              m_network = nullptr; //!< Test network AST used to generate SystemModel tree
};
//
//  End of AST_SystemModelGenerator class declaration
//---------------------------------------------------------------------------
} // End of namespace Parsers




#endif  // not defined AST_SYSTEMMODELGENERATOR_H__4AD33E64_B5F0_44D9_839C_E286323534C__INCLUDED_

//===========================================================================
// End of AST_SystemModelGenerator.hpp
//===========================================================================
