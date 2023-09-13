//===========================================================================
//                           SystemModelBuilder.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelBuilder.cpp
//!
//! Implements class SystemModelBuilder
//!
//===========================================================================

#include "SystemModelBuilder.hpp"
#include "SystemModel.hpp"
#include "SystemModelNode.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "DefaultOneHotPathSelector.hpp"
#include "DefaultNHotPathSelector.hpp"
#include "AccessInterfaceProtocol.hpp"
#include "AccessInterfaceRawProtocol.hpp"
#include "BrocadeSelector.hpp"
#include "Utility.hpp"
#include "EnumsUtility.hpp"
#include "Dummy_TranslatorProtocol.hpp"


using std::string;
using std::experimental::string_view;
using std::shared_ptr;
using std::make_shared;
using std::make_pair;
using std::pair;
using std::dynamic_pointer_cast;
using std::initializer_list;

using namespace std::string_literals;
using namespace mast;

namespace
{
//! Defines how an AccessInterface looks like
//!
enum class AccessInterfaceAssessment
{
  Undefined,        //!< The kind of AccessInterface has not been recognized
  JTAG_TAP,         //!< The AccessInterface has been recognized as a JTAG TAP
  Chained_JTAG_TAP, //!< The AccessInterface has been recognized as an already chain of JTAG TAPs
};


//! Tries to assess the kind of an AccessInterface
//!
//! @note Only recognizes JTAG TAP and chained JTAG TAP
//!
AccessInterfaceAssessment AssessAccessInterfaceType (shared_ptr<AccessInterface> ai)
{
  auto type = AccessInterfaceAssessment::Undefined;

  auto firstChild  = ai->FirstChild();
  auto secondChild = firstChild->NextSibling();

  auto firstChildAsChain = dynamic_pointer_cast<Chain>(firstChild);

  // ---------------- Already a chain of JTAG TAPs ?
  //
  if (firstChildAsChain)
  {
    // ---------------- Check that second child is also a chain
    //
    auto secondChildAsChain = dynamic_pointer_cast<Chain>(secondChild);
    if (!secondChildAsChain)
    {
      return AccessInterfaceAssessment::Undefined;
    }

    // ---------------- Check that there is only 2 chains
    //
    if (secondChildAsChain->NextSibling())
    {
      return AccessInterfaceAssessment::Undefined;
    }

    // ---------------- Check that first chain has only IRs
    //
    auto hasOnlyIRs = firstChildAsChain->HasChildren() && HasOnlyChilrenOfType<Register>(firstChildAsChain);
    if (!hasOnlyIRs)
    {
      return AccessInterfaceAssessment::Undefined;
    }

    // ---------------- Check that second chain has only muxes (Linkers)
    //
    auto hasOnlyMuxes = secondChildAsChain->HasChildren() && HasOnlyChilrenOfType<Linker>(secondChildAsChain);
    if (!hasOnlyMuxes)
    {
      return AccessInterfaceAssessment::Undefined;
    }
    type = AccessInterfaceAssessment::Chained_JTAG_TAP;
  }
  else  // Only a JTAG TAP
  {
    // ---------------- Check that first child is only a single IR
    //
    auto firstChildAsRegister = dynamic_pointer_cast<Register>(firstChild);
    if (!firstChildAsRegister)
    {
      return AccessInterfaceAssessment::Undefined;
    }

    // ---------------- Check that second child is only a single mux (Linker)
    //
    auto secondChildAsLinker = dynamic_pointer_cast<Linker>(secondChild);
    if (!secondChildAsLinker || secondChildAsLinker->NextSibling())
    {
      return AccessInterfaceAssessment::Undefined;
    }
    type = AccessInterfaceAssessment::JTAG_TAP;
  }

  return type;
}
//
//  End of: AssessAccessInterfaceType
//---------------------------------------------------------------------------


} // End of unnamed namespace


//! Appends several registers (with same length and initial content) to a parent
//!
void SystemModelBuilder::AppendRegisters (uint32_t               count,
                                          const string&          baseName,
                                          const BinaryVector&    bypass,
                                          shared_ptr<ParentNode> parent)
{
  for (uint32_t ii = 0 ; ii < count ; ++ii)
  {
    auto regName = baseName + std::to_string(ii);
    m_model.CreateRegister (regName, bypass, parent);
  }
}
//
//  End of: SystemModelBuilder::AppendRegisters
//---------------------------------------------------------------------------


//! Appends several registers (with same length and initial content) to a parent
//!
void SystemModelBuilder::AppendRegisters (uint32_t            count,
                                          const string&       baseName,
                                          const BinaryVector& bypass,
                                          string_view         parentPath)
{
  auto parentNode = dynamic_pointer_cast<ParentNode>(m_model.Root()->FindNode(parentPath));;
  CHECK_VALUE_NOT_NULL(parentNode, "Cannot find parent node with path: " + parentPath);

  AppendRegisters(count, baseName, bypass, parentNode);
}
//
//  End of: SystemModelBuilder::AppendRegisters
//---------------------------------------------------------------------------



//! Creates a "1500" style wrapper
//!
std::shared_ptr<Chain> SystemModelBuilder::Create_1500_Wrapper (string_view name, uint32_t maxChannels)
{
  // ---------------- Create wrapper
  //
  if (name.empty())
  {
    name = DEFAULT_1500_NAME;
  }
  auto wrapper = m_model.CreateChain (name);

  // ---------------- SWIR
  //
  auto res             = Create_PathSelector(SelectorKind::Binary, "SWIR"s + MIB_CTRL_EXT, 2u);
  auto swirSelectorReg = res.first;
  auto swirSelector    = res.second;
  auto swirMib         = Create_MIB("SWIR", swirSelector, swirSelectorReg, MuxRegPlacement::BeforeMux);
  wrapper->AppendChild(swirMib);

  // ---------------- WIR
  //
  auto totalChannels = maxChannels + 1u;   // +1 is to take into account bypass register (wirBypass)

       res         = Create_PathSelector(SelectorKind::Binary, "WIR_reg", totalChannels);
  auto wirReg      = res.first;
  auto wirSelector = res.second;
  auto wirMib      = Create_MIB("WIR", wirSelector, wirReg, MuxRegPlacement::Remote);
  auto wirBypass   = m_model.CreateRegister ("WBY", BinaryVector(1, 0), wirMib);

  swirMib->AppendChild(wirMib);
  swirMib->AppendChild(wirReg);
  wrapper->SetChildAppender(wirMib);

  return wrapper;
}
//
//  End of: SystemModelBuilder::Create_1500_Wrapper
//---------------------------------------------------------------------------


//! Moves up to 5 TAPs under a "Master" TAP that provides dynamic selection of the "Slave" TAPs
//!
//! @param masterProtocol An access interface protocol for the "Master" TAP
//! @param slaveProtocol  An access interface protocol for the "Slave" TAP
//! @param taps           "Slave" TAPs
//!
shared_ptr<AccessInterfaceTranslator> SystemModelBuilder::Create_Brocade (shared_ptr<AccessInterfaceProtocol>           masterProtocol,
                                                      shared_ptr<AccessInterfaceProtocol>           slaveProtocol,
                                                      initializer_list<shared_ptr<AccessInterface>> taps)
{
  // ---------------- Check parameters
  //
  CHECK_PARAMETER_NOT_NULL(masterProtocol, "Expect a valid AccessInterfaceProtocol for the 'Master' AccessInterface");
  CHECK_PARAMETER_NOT_NULL(slaveProtocol,  "Expect a valid AccessInterfaceProtocol for the 'Slave' AccessInterface");  
  CHECK_PARAMETER_RANGE(taps.size(), 1u, 5u, "Brocade support only from 1 to 5 'slave' TAPs");
  
  for (auto tap : taps)
  {
    auto aiType = AssessAccessInterfaceType(tap);
    CHECK_TRUE(aiType == AccessInterfaceAssessment::JTAG_TAP, "Cannot handle non JTAG TAP slaves");
  }

  // ---------------- Prepare Brocade mux
  //
  auto brocadeChain  = m_model.CreateAccessInterfaceTranslator ("Brocade",std::make_unique<Dummy_TranslatorProtocol>(""),nullptr);
  auto masterAi      = m_model.CreateAccessInterface("Master_AI", masterProtocol, brocadeChain);
  auto slaveAi       = m_model.CreateAccessInterface("Slave_AI",  slaveProtocol,  brocadeChain);
  auto masterCtrlReg = m_model.CreateRegister("Brocade_CTRL", BinaryVector(8u), true, masterAi);
  auto irChain       = m_model.CreateChain  ("IR",     slaveAi);
  auto drChain       = m_model.CreateChain  ("DR",     slaveAi);
  auto selector      = make_shared<BrocadeSelector>(masterCtrlReg, taps.size());
  auto irLinker      = m_model.CreateLinker ("IR_Mux", selector, irChain);
  auto drLinker      = m_model.CreateLinker ("DR_Mux", selector, drChain);


  brocadeChain->RegisterInterface(masterAi);
  brocadeChain->RegisterInterface(slaveAi);

  masterAi->IgnoreForNodePath(true);
  slaveAi->IgnoreForNodePath(true);
  irChain->IgnoreForNodePath(true);
  irChain->SetChildAppender(irLinker);

  drChain->IgnoreForNodePath(true);
  drChain->SetChildAppender(drLinker);

  // ---------------- Connect slave TAPs
  //
  auto renameNodes = [](shared_ptr<AccessInterface> tap, shared_ptr<Register> ir, shared_ptr<Linker> drMux, uint32_t tapOrder)
  {
    auto setTapName =     tap->Name().empty()
                      || (tap->Name() == "TAP")
                      || (tap->Name() == "Tap")
                      || (tap->Name() == "tap")
                      || (tap->Name() == "1149_1_TAP");
    //! @todo [JFC]-[September/30/2016]: In Create_Brocade(): Compare case insensitive ==> Need utility
    //!
    auto tapName = setTapName ? "TAP"s + std::to_string(tapOrder) : tap->Name();

    ir    ->SetName(tapName + ".IR");
    drMux ->SetName(tapName);
  };

  uint32_t tapNum = 1u;
  for (auto tap : taps)
  {
    // ---------------- Disconnect pieces from AccessInterface
    //
    auto mux = dynamic_pointer_cast<Linker>   (tap->DisconnectChannel(2u));
    auto ir  = dynamic_pointer_cast<Register> (tap->DisconnectChannel(1u));

    // ---------------- Reconnect to Brocade muxes
    //
    irLinker->AppendChild(ir);
    drLinker->AppendChild(mux);

    // ---------------- Adjust names for paths
    //
    mux->IgnoreForNodePath(false);
    renameNodes(tap, ir, mux, tapNum++);

    // ---------------- Get rid of AccessInterface
    //
    m_model.RemoveNodeFromModel(tap);
  }

//  m_model.ReplaceRoot(brocadeChain, false);
  return brocadeChain;
}
//
//  End of: SystemModelBuilder::Create_Brocade
//---------------------------------------------------------------------------


//! Moves up to 5 TAPs under a "Master" TAP that provides dynamic selection of the "Slave" TAPs
//!
//! @param TranslatorProtocol the top-level access interface translator protocol (ex; Emulation)
//! @param masterProtocol An access interface protocol for the "Master" TAP
//! @param slaveProtocol  An access interface protocol for the "Slave" TAP
//! @param taps           "Slave" TAPs
//!
shared_ptr<AccessInterfaceTranslator> SystemModelBuilder::Create_Brocade (shared_ptr<AccessInterfaceTranslatorProtocol>  TopProtocol,
                                                      shared_ptr<AccessInterfaceProtocol>           masterProtocol,
                                                      shared_ptr<AccessInterfaceProtocol>           slaveProtocol,
                                                      initializer_list<shared_ptr<AccessInterface>> taps)
{
  // ---------------- Check parameters
  //
  CHECK_PARAMETER_NOT_NULL(TopProtocol, "Expect a valid AccessInterfaceProtocol for the 'TopProtocol' AccessInterfaceTranslator");
  CHECK_PARAMETER_NOT_NULL(masterProtocol, "Expect a valid AccessInterfaceProtocol for the 'Master' AccessInterface");
  CHECK_PARAMETER_NOT_NULL(slaveProtocol,  "Expect a valid AccessInterfaceProtocol for the 'Slave' AccessInterface");  
  CHECK_PARAMETER_RANGE(taps.size(), 1u, 5u, "Brocade support only from 1 to 5 'slave' TAPs");
  auto master_is_raw = std::dynamic_pointer_cast<AccessInterfaceRawProtocol>(masterProtocol);
  CHECK_PARAMETER_NOT_NULL(master_is_raw,  "Expect a Raw  AccessInterfaceProtocol for the 'Master' AccessInterface");  
  auto slave_is_raw = std::dynamic_pointer_cast<AccessInterfaceRawProtocol>(slaveProtocol);
  CHECK_PARAMETER_NOT_NULL(slave_is_raw,  "Expect a Raw  AccessInterfaceProtocol for the 'Slave' AccessInterface");  
auto top_is_translator = std::dynamic_pointer_cast<AccessInterfaceTranslatorProtocol>(TopProtocol);
  CHECK_PARAMETER_NOT_NULL(top_is_translator,  "Expect an AccessInterfaceTranslatorProtocol for the 'top' Protocol");  


  for (auto tap : taps)
  {
    auto aiType = AssessAccessInterfaceType(tap);
    CHECK_TRUE(aiType == AccessInterfaceAssessment::JTAG_TAP, "Cannot handle non JTAG TAP slaves");
  }

  // ---------------- Prepare Brocade mux
  //
  auto brocadeTop  = m_model.CreateAccessInterfaceTranslator("Brocade",TopProtocol);
  auto masterAi      = m_model.CreateAccessInterface("Master_AI", masterProtocol, brocadeTop);
  brocadeTop->RegisterInterface(masterAi);
  auto slaveAi       = m_model.CreateAccessInterface("Slave_AI",  slaveProtocol,  brocadeTop);
  brocadeTop->RegisterInterface(slaveAi);
  auto masterCtrlReg = m_model.CreateRegister("Brocade_CTRL", BinaryVector(8u), true, masterAi);
  auto irChain       = m_model.CreateChain  ("IR",     slaveAi);
  auto drChain       = m_model.CreateChain  ("DR",     slaveAi);
  auto selector      = make_shared<BrocadeSelector>(masterCtrlReg, taps.size());
  auto irLinker      = m_model.CreateLinker ("IR_Mux", selector, irChain);
  auto drLinker      = m_model.CreateLinker ("DR_Mux", selector, drChain);

  masterAi->IgnoreForNodePath(true);
  slaveAi->IgnoreForNodePath(true);
  irChain->IgnoreForNodePath(true);
  irChain->SetChildAppender(irLinker);

  drChain->IgnoreForNodePath(true);
  drChain->SetChildAppender(drLinker);

  // ---------------- Connect slave TAPs
  //
  auto renameNodes = [](shared_ptr<AccessInterface> tap, shared_ptr<Register> ir, shared_ptr<Linker> drMux, uint32_t tapOrder)
  {
    auto setTapName =     tap->Name().empty()
                      || (tap->Name() == "TAP")
                      || (tap->Name() == "Tap")
                      || (tap->Name() == "tap")
                      || (tap->Name() == "1149_1_TAP");
    //! @todo [JFC]-[September/30/2016]: In Create_Brocade(): Compare case insensitive ==> Need utility
    //!
    auto tapName = setTapName ? "TAP"s + std::to_string(tapOrder) : tap->Name();

    ir    ->SetName(tapName + ".IR");
    drMux ->SetName(tapName);
  };

  uint32_t tapNum = 1u;
  for (auto tap : taps)
  {
    // ---------------- Diconnect pieces from AccessInterface
    //
    auto mux = dynamic_pointer_cast<Linker>   (tap->DisconnectChannel(2u));
    auto ir  = dynamic_pointer_cast<Register> (tap->DisconnectChannel(1u));

    // ---------------- Reconnect to Brocade muxes
    //
    irLinker->AppendChild(ir);
    drLinker->AppendChild(mux);

    // ---------------- Adjust names for paths
    //
    mux->IgnoreForNodePath(false);
    renameNodes(tap, ir, mux, tapNum++);

    // ---------------- Get rid of AccessInterface
    //
    m_model.RemoveNodeFromModel(tap);
  }

//  m_model.ReplaceRoot(brocadeTop, false);
  return brocadeTop;

}

//! Moves up to 5 TAPs under a "Master" TAP that provides dynamic selection of the "Slave" TAPs
//! NB: this version also looks for PDL Functions registred with the TAPs and 
//!     changes their assignement to the new System Model 
//!
//! @param TranslatorProtocol the top-level access interface translator protocol (ex; Emulation)
//! @param masterProtocol An access interface protocol for the "Master" TAP
//! @param slaveProtocol  An access interface protocol for the "Slave" TAP
//! @param taps           Vector of "Slave" TAPs
//!
shared_ptr<AccessInterfaceTranslator> SystemModelBuilder::Create_Brocade (shared_ptr<AccessInterfaceTranslatorProtocol>  TopProtocol,
                                                      shared_ptr<AccessInterfaceProtocol>           masterProtocol,
                                                      shared_ptr<AccessInterfaceProtocol>           slaveProtocol,
                                                      std::vector<shared_ptr<AccessInterface>> taps,
						      std::vector<mast::AppFunctionNameAndNode>& namesAndNodes )
{
  // ---------------- Check parameters
  //
  CHECK_PARAMETER_NOT_NULL(TopProtocol, "Expect a valid AccessInterfaceProtocol for the 'TopProtocol' AccessInterfaceTranslator");
  CHECK_PARAMETER_NOT_NULL(masterProtocol, "Expect a valid AccessInterfaceProtocol for the 'Master' AccessInterface");
  CHECK_PARAMETER_NOT_NULL(slaveProtocol,  "Expect a valid AccessInterfaceProtocol for the 'Slave' AccessInterface");  
  CHECK_PARAMETER_RANGE(taps.size(), 1u, 5u, "Brocade support only from 1 to 5 'slave' TAPs");
  auto master_is_raw = std::dynamic_pointer_cast<AccessInterfaceRawProtocol>(masterProtocol);
  CHECK_PARAMETER_NOT_NULL(master_is_raw,  "Expect a Raw  AccessInterfaceProtocol for the 'Master' AccessInterface");  
  auto slave_is_raw = std::dynamic_pointer_cast<AccessInterfaceRawProtocol>(slaveProtocol);
  CHECK_PARAMETER_NOT_NULL(slave_is_raw,  "Expect a Raw  AccessInterfaceProtocol for the 'Slave' AccessInterface");  
auto top_is_translator = std::dynamic_pointer_cast<AccessInterfaceTranslatorProtocol>(TopProtocol);
  CHECK_PARAMETER_NOT_NULL(top_is_translator,  "Expect an AccessInterfaceTranslatorProtocol for the 'top' Protocol");  


  for (auto tap : taps)
  {
    auto aiType = AssessAccessInterfaceType(tap);
    CHECK_TRUE(aiType == AccessInterfaceAssessment::JTAG_TAP, "Cannot handle non JTAG TAP slaves");
  }

  // ---------------- Prepare Brocade mux
  //
  auto brocadeTop  = m_model.CreateAccessInterfaceTranslator("Brocade",TopProtocol);
  auto masterAi      = m_model.CreateAccessInterface("Master_AI", masterProtocol, brocadeTop);
  brocadeTop->RegisterInterface(masterAi);
  auto slaveAi       = m_model.CreateAccessInterface("Slave_AI",  slaveProtocol,  brocadeTop);
  brocadeTop->RegisterInterface(slaveAi);
  auto masterCtrlReg = m_model.CreateRegister("Brocade_CTRL", BinaryVector(8u), true, masterAi);
  auto irChain       = m_model.CreateChain  ("IR",     slaveAi);
  auto drChain       = m_model.CreateChain  ("DR",     slaveAi);
  auto selector      = make_shared<BrocadeSelector>(masterCtrlReg, taps.size());
  auto irLinker      = m_model.CreateLinker ("IR_Mux", selector, irChain);
  auto drLinker      = m_model.CreateLinker ("DR_Mux", selector, drChain);

  masterAi->IgnoreForNodePath(true);
  slaveAi->IgnoreForNodePath(true);
  irChain->IgnoreForNodePath(true);
  irChain->SetChildAppender(irLinker);

  drChain->IgnoreForNodePath(true);
  drChain->SetChildAppender(drLinker);

  // ---------------- Connect slave TAPs
  //
  auto renameNodes = [](shared_ptr<AccessInterface> tap, shared_ptr<Register> ir, shared_ptr<Linker> drMux, uint32_t tapOrder)
  {
    auto setTapName =     tap->Name().empty()
                      || (tap->Name() == "TAP")
                      || (tap->Name() == "Tap")
                      || (tap->Name() == "tap")
                      || (tap->Name() == "1149_1_TAP");
    //! @todo [JFC]-[September/30/2016]: In Create_Brocade(): Compare case insensitive ==> Need utility
    //!
    auto tapName = setTapName ? "TAP"s + std::to_string(tapOrder) : tap->Name();

    ir    ->SetName(tapName + ".IR");
    drMux ->SetName(tapName);
  };

  uint32_t tapNum = 1u;
  for (auto tap : taps)
  {
    // ---------------- Diconnect pieces from AccessInterface
    //
    auto mux = dynamic_pointer_cast<Linker>   (tap->DisconnectChannel(2u));
    auto ir  = dynamic_pointer_cast<Register> (tap->DisconnectChannel(1u));

    // ---------------- Reconnect to Brocade muxes
    //
    irLinker->AppendChild(ir);
    drLinker->AppendChild(mux);

    // ---------------- Adjust names for paths
    //
    mux->IgnoreForNodePath(false);
    renameNodes(tap, ir, mux, tapNum++);
    
    //--if tap has any PDL function registred, change the reference to the new Mux representing this tap
 //  for (auto registred_node : namesAndNodes)
   for (size_t pos=0;pos<namesAndNodes.size();pos++)
     {
     
 //      if (registred_node.node->Name()==tap->Name())
       if (namesAndNodes[pos].node->Name()==tap->Name())
        {
	  auto asParentNode = dynamic_pointer_cast <ParentNode>(mux);
       
    //   registred_node.node = asParentNode;
       namesAndNodes[pos].node= asParentNode;
//	LOG(DEBUG)<<"Registred Node " <<registred_node.node->Name()<< "[" << registred_node.node->Identifier() << "]" 
//	<<" has " << registred_node.node->DirectChildrenCount() <<" children";

    
	}
     }

    // ---------------- Get rid of AccessInterface
    //
    m_model.RemoveNodeFromModel(tap);
  }

//  m_model.ReplaceRoot(brocadeTop, false);
  return brocadeTop;

}

//! Creates a new Tap node using implicit binary coding
//!
//!  @param name            Name given to the tap
//!  @param irBitsCount     IR number of bits (at least one)
//!  @param muxPathsCount   DR number of path (at least two)
//!
//!  ______________________________
//! |                              |
//! |     (ACCESS_I:Tap)           |
//! |      /      \                |
//! |     /       _\__________     |
//! | [REG:Ir]  /Linker:Dr_Mux\    |
//! |           ---------------    |
//! |             /                |
//! |        [REG:Bypass]          |
//! |                              |
//!  ------------------------------
//!
shared_ptr<AccessInterface> SystemModelBuilder::Create_JTAG_TAP (string_view                         name,
                                                                 uint32_t                            irBitsCount,
                                                                 uint32_t                            muxPathsCount,
                                                                 shared_ptr<AccessInterfaceProtocol> protocol)
{

 return Create_JTAG_TAP(name,irBitsCount,muxPathsCount,protocol,std::vector<mast::BinaryVector>());
}
//
//  End of: SystemModelBuilder::Create_JTAG_TAP
//---------------------------------------------------------------------------

//! Creates a new Tap node using provided coding
//!
//!  @param name            Name given to the tap
//!  @param irBitsCount     IR number of bits (at least one)
//!  @param muxPathsCount   DR number of path (at least two)
//!
//!  ______________________________
//! |                              |
//! |     (ACCESS_I:Tap)           |
//! |      /      \                |
//! |     /       _\__________     |
//! | [REG:Ir]  /Linker:Dr_Mux\    |
//! |           ---------------    |
//! |             /                |
//! |        [REG:Bypass]          |
//! |                              |
//!  ------------------------------
//!
shared_ptr<AccessInterface> SystemModelBuilder::Create_JTAG_TAP (string_view                            name,
                                                                 uint32_t                               irBitsCount,
                                                                 uint32_t                               muxPathsCount,
                                                                 shared_ptr<AccessInterfaceProtocol>    protocol,
                                                                 const std::vector<mast::BinaryVector>& IR_coding)
{
  CHECK_PARAMETER_NOT_ZERO (irBitsCount, "irBitsCount must be != 0");
  CHECK_PARAMETER_GT       (muxPathsCount, 1, "muxPathsCount must be > 1");

  auto noName     = name.empty();
  auto rootName   = noName ? DEFAULT_TAP_NAME         : name;
  auto irName     = noName ? DEFAULT_TAP_IR_NAME      : string(name) + DEFAULT_TAP_IR_EXT;
  auto muxName    = noName ? DEFAULT_TAP_MUX_NAME     : string(name) + DEFAULT_TAP_MUX_EXT;
  auto muxBpyName = noName ? DEFAULT_TAP_MUX_BPY_NAME : string(name) + DEFAULT_TAP_MUX_BPY_EXT;

  auto accessInterface = m_model.CreateAccessInterface(rootName, protocol);

  // ---------------- Create IR and selection/deselection tables
  //
  BinaryVector irBypassSequence;
  std::vector<BinaryVector> selectTable;
  if (IR_coding.empty())
  {
    // Select table is by default binary except for no path and 1st that use the bypass sequence
    selectTable      = DefaultBinaryPathSelector::CreateSelectTable(irBitsCount, muxPathsCount, SelectorProperty::Binary_Default);
    irBypassSequence = BinaryVector(irBitsCount, 0xFF);

    selectTable[0] = irBypassSequence;  // Not used (path id zero)
    selectTable[1] = irBypassSequence;  // Bypass register
  }
  else
  {
    selectTable      = IR_coding;
    irBypassSequence = IR_coding[0];

    selectTable.insert(selectTable.begin(), irBypassSequence); // Bypass sequence must be used also for 0 in selectTable
  }

  auto deselectTable = DefaultTableBasedPathSelector::TablesType(muxPathsCount + 1u, irBypassSequence); // Deselect table is by default all bypass sequence

  auto ir = m_model.CreateRegister(irName, irBypassSequence, true, accessInterface);

  // ---------------- Create path selector
  //
  auto pathSelector = make_shared<DefaultTableBasedPathSelector>(ir, muxPathsCount, std::move(selectTable), std::move(deselectTable));

  // ---------------- Create Linker
  //
  auto linker = m_model.CreateLinker(muxName, pathSelector, accessInterface);
  linker->SetKeepOpen(true);

  // ---------------- Create bypass register
  //
  auto bypassBypassSequence = BinaryVector(1, 0xFF);
  auto bypassRegister       = m_model.CreateRegister(muxBpyName, bypassBypassSequence, linker);

  // ---------------- Set AccessInterface to forward append to the linker
  //
  accessInterface->SetChildAppender(linker);

  return accessInterface;
}
//
//  End of: SystemModelBuilder::Create_JTAG_TAP
//---------------------------------------------------------------------------


//! Creates a MIB sub-tree
//!
//! @param name               Name associated with MIB top node
//! @param selector           The selector that manage mux state
//! @param selectorRegister   The selector register that will drive the mux
//! @param muxRegPlacement       Whether mux is placed before or after the register that will drive it
//!
shared_ptr<Chain> SystemModelBuilder::Create_MIB (string_view              name,
                                                  shared_ptr<PathSelector> selector,
                                                  shared_ptr<Register>     selectorRegister,
                                                  MuxRegPlacement          muxRegPlacement
                                                 )
{
  // ---------------- Create top node
  //
  if (name.empty())
  {
    name = DEFAULT_MIB_NAME;
  }

  auto mibName = string(name);
  auto mib     = m_model.CreateChain(mibName);
  
  mib->IgnoreForNodePath(true); //In ICL, the Chain does not exist
  
  // ---------------- Create linker
  //
  auto linker = m_model.CreateLinker(mibName + MIB_MUX_EXT, selector);

  // ---------------- Append linker and optionally its selector register
  //
  if (muxRegPlacement == MuxRegPlacement::AfterMux)
  {
    mib->AppendChild(linker);
    mib->AppendChild(selectorRegister);
  }
  else if (muxRegPlacement == MuxRegPlacement::BeforeMux)
  {
    mib->AppendChild(selectorRegister);
    mib->AppendChild(linker);
  }
  else
  {
    mib->AppendChild(linker); // Remote control reg is already in the tree ==> append only the MUX
  }

  mib->SetChildAppender(linker);

  return mib;
}
//
//  End of: SystemModelBuilder::Create_MIB
//---------------------------------------------------------------------------


//! Creates a SIB sub-tree: a 1-node MIB using a SefaultBinary Selector
//!
//! @param name               Name associated with SIB top node
//! @param sel_properties    Properties of the SIB: only InvertedBits is taken into account
//! @param muxRegPlacement       Whether mux is placed before or after the register that will drive it
//!
shared_ptr<Chain> SystemModelBuilder::Create_SIB (string_view      name,
                                                  SelectorProperty sel_properties,
                                                  MuxRegPlacement  muxRegPlacement
                                                 )
{
  auto SIB_properties  = sel_properties | SelectorProperty::CanSelectNone;
  auto selectorRegName = string(name)+ MIB_CTRL_EXT;
  auto res             = Create_PathSelector(SelectorKind::Binary, selectorRegName, 1,SIB_properties);
  auto selectorReg     = res.first;
  auto selector        = res.second;

  return Create_MIB(name, selector, selectorReg,muxRegPlacement );
}
//
//  End of: SystemModelBuilder::Create_SIB
//---------------------------------------------------------------------------


//! Creates a path selector
//!
//! @param selectorKind         Kind of selector (Binary, One_Hot, N_Hot,Unresolved)
//! @param associatedRegister   Register that is used to drive the path multiplexer
//! @param pathsCount           Number of managed paths (including, optional, bypass register)
//! @param properties           Properties of the selector (bit order can be reverse or it can use negative logic)
//!
//! @return Newly created path selector
//!
shared_ptr<PathSelector> SystemModelBuilder::Create_PathSelector (SelectorKind         selectorKind,
                                                                  shared_ptr<Register> associatedRegister,
                                                                  uint32_t             pathsCount,
                                                                  SelectorProperty     properties)
{
  CHECK_PARAMETER_NOT_NULL(associatedRegister, "Associated register is mandatory (or call the version that take the register name instead)");

  switch (selectorKind)
  {
    case SelectorKind::Binary:
      return make_shared<DefaultBinaryPathSelector>(associatedRegister, pathsCount, properties);
    case SelectorKind::One_Hot:
      return make_shared<DefaultOneHotPathSelector>(associatedRegister, pathsCount, properties);
    case SelectorKind::N_Hot:
      return make_shared<DefaultNHotPathSelector>(associatedRegister, pathsCount, properties);
    default:
      THROW_INVALID_ARGUMENT("Can only support Binary, One_Hot and N_Hot type path selector");
      break;
  }
}
//
//  End of: SystemModelBuilder::Create_PathSelector
//---------------------------------------------------------------------------


//! Creates a path selector, creating its associated register
//!
//! @param selectorKind   Kind of selector (Binary, One_Hot, N_Hot)
//! @param registerName   Name of Register that is used to drive the path multiplexer
//! @param pathsCount     Number of managed paths (including, optional, bypass register)
//! @param properties     Properties of the selector (bit order can be reverse or it can use negative logic)
//!
//! @return Newly created path selector and its associated register
//!
pair<shared_ptr<Register>, shared_ptr<PathSelector>> SystemModelBuilder::Create_PathSelector (SelectorKind     selectorKind,
                                                                                              string_view      registerName,
                                                                                              uint32_t         pathsCount,
                                                                                              SelectorProperty properties)
{
  // ---------------- Create associated register
  //
  BinaryVector registerInitialValue;
  switch (selectorKind)
  {
    case SelectorKind::Binary:
      registerInitialValue = DefaultBinaryPathSelector::AssociatedRegisterInitialValue(pathsCount, properties);
      break;
    case SelectorKind::One_Hot:
      registerInitialValue = DefaultOneHotPathSelector::AssociatedRegisterInitialValue(pathsCount, properties);
      break;
    case SelectorKind::N_Hot:
      registerInitialValue = DefaultNHotPathSelector::AssociatedRegisterInitialValue(pathsCount, properties);
      break;
    default:
      THROW_INVALID_ARGUMENT("Can only support Binary, One_Hot and N_Hot type path selector");
      break;
  }

  auto holdValue = true;
  auto associatedRegister = m_model.CreateRegister (registerName, registerInitialValue, holdValue);

  // ---------------- Create selector
  //
  auto selector = Create_PathSelector(selectorKind, associatedRegister, pathsCount, properties);

  // ---------------- Return both
  //
  return make_pair(associatedRegister, selector);
}
//
//  End of: SystemModelBuilder::Create_PathSelector
//---------------------------------------------------------------------------



//! Chains (merges) a 1149.1 AccessInterface into another 1149.1 AccessInterface.
//!
//! @note - If first tap is already a daisy chain, second tap is daisy chained after last already chained tap
//!       - Paths relative to first tap must include 1st tap name ; if none is provided, "TAP1" is provided)
//!       - Paths relative to second tap must include 2nd tap name ; if none is provided, "TAPx" is provided (where is
//!         reflect the position of the tap in the chain of taps)
//!
//! @param tap1 First tap
//! @param tap2 Second tap
//!
void SystemModelBuilder::DaisyChain_JTAG_TAPS (shared_ptr<AccessInterface> tap1, shared_ptr<AccessInterface> tap2)
{
  CHECK_PARAMETER_NOT_NULL(tap1, "Cannot chain tap with nullptr");
  CHECK_PARAMETER_NOT_NULL(tap2, "Cannot chain tap from nullptr");

  // ---------------- Check/manage protocols
  //
  auto protocol1 = tap1->Protocol();
  auto protocol2 = tap2->Protocol();
  if (protocol1 && protocol2)
  {
    CHECK_TRUE(protocol1->KindName() == protocol2->KindName(), "To daisy chain two AccessInterface, they must have same type of protocol");
  }
  else if (!protocol1)
  {
    tap1->SetProtocol(protocol2);
  }

  auto aiType1 = AssessAccessInterfaceType(tap1);
  auto aiType2 = AssessAccessInterfaceType(tap2);

  CHECK_TRUE(   (aiType1 == AccessInterfaceAssessment::JTAG_TAP)
             || (aiType1 == AccessInterfaceAssessment::Chained_JTAG_TAP) , "First AccessInterface must be a JTAG TAP or a chain of JTAG TAP");


  CHECK_TRUE(aiType2 == AccessInterfaceAssessment::JTAG_TAP, "First AccessInterface must be a JTAG TAP");

  auto renameNodes = [](shared_ptr<AccessInterface> tap, shared_ptr<Register> ir, shared_ptr<Linker> drMux, uint32_t tapOrder)
  {
    auto setTapName =     tap->Name().empty()
                      || (tap->Name() == "TAP")
                      || (tap->Name() == "1149_1_TAP");

    auto tapName = setTapName ? "TAP"s + std::to_string(tapOrder) : tap->Name();

    tap->SetName("Chained_TAP");

    ir    ->SetName(tapName + ".IR");
    drMux ->SetName(tapName);
  };

  // ---------------- Prepare daisy chain
  //
  if (aiType1 == AccessInterfaceAssessment::JTAG_TAP)
  {
    auto mux = dynamic_pointer_cast<Linker>   (tap1->DisconnectChannel(2u));;
    auto ir  = dynamic_pointer_cast<Register> (tap1->DisconnectChannel(1u));;

    auto irChain  = m_model.CreateChain("IR_DaisyChain",     tap1);
    auto muxChain = m_model.CreateChain("DR_Mux_DaisyChain", tap1);

    irChain->AppendChild(ir);
    muxChain->AppendChild(mux);
    tap1->SetChildAppender(mux); // Restore child appender (reset when having disconnected the mux)

    // ---------------- Adjust paths management
    //
    irChain->IgnoreForNodePath(true);
    muxChain->IgnoreForNodePath(true);
    mux->IgnoreForNodePath(false);

    renameNodes(tap1, ir, mux, 1u);
  }

  // ---------------- Chain second tap
  //
  auto mux = dynamic_pointer_cast<Linker>   (tap2->DisconnectChannel(2u));
  auto ir  = dynamic_pointer_cast<Register> (tap2->DisconnectChannel(1u));

  auto irChain  = dynamic_pointer_cast<Chain>(tap1->FirstChild());
  CHECK_VALUE_NOT_NULL(irChain, "Houps tap1 is not already a chain of JTAG TAP");

  auto muxChain = dynamic_pointer_cast<Chain>(irChain->NextSibling());
  CHECK_VALUE_NOT_NULL(muxChain, "Houps tap1 is not already a chain of JTAG TAP");

  irChain->AppendChild(ir);
  muxChain->AppendChild(mux);

  // ---------------- Adjust names for paths
  //
  mux->IgnoreForNodePath(false);
  auto tapsCount = irChain->DirectChildrenCount();
  renameNodes(tap2, ir, mux, tapsCount);

  // ---------------- Get rid of second tap
  //
  m_model.RemoveNodeFromModel(tap2);
}
//
//  End of: SystemModelBuilder::DaisyChain_JTAG_TAPS
//---------------------------------------------------------------------------



//! Chains a bunch of 1149.1 AccessInterfaces
//!
//! @param taps The taps to chain
//!
void SystemModelBuilder::DaisyChain_JTAG_TAPS (initializer_list<std::shared_ptr<AccessInterface>> taps)
{
  CHECK_PARAMETER_GTE(taps.size(), 2u, "Cannot chain a single AccessInterface");

  auto first = taps.begin();
  auto next  = first + 1u;

  while (next != taps.end())
  {
    DaisyChain_JTAG_TAPS(*first, *next);
    ++next;
  }
}
//
//  End of: SystemModelBuilder::DaisyChain_JTAG_TAPS
//---------------------------------------------------------------------------



//===========================================================================
// End of SystemModelBuilder.cpp
//===========================================================================
