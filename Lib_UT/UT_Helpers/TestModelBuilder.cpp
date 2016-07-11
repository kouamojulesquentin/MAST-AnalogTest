//===========================================================================
//                           TestModelBuilder.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file TestModelBuilder.cpp
//!
//! Implements class TestModelBuilder
//!
//===========================================================================

#include "TestModelBuilder.hpp"
#include "SystemModel.hpp"
#include "SystemModelNode.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "BinaryVector_Traits.hpp"

using std::string;
using std::experimental::string_view;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;

using namespace std::string_literals;
using namespace mast;
using namespace test;


//! Appends several registers (with same length and initial content) to a parent
//!
void TestModelBuilder::AppendRegisters (uint32_t               count,
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
//  End of: TestModelBuilder::AppendRegisters
//---------------------------------------------------------------------------


//! Creates a MIB with a default (binary) selector
//!
std::shared_ptr<Chain> TestModelBuilder::Create_Default_MIB (string_view name, uint32_t maxDerivations)
{
  // ---------------- Prepare default selector
  //
  auto selectorRegName = name.empty() ? string(DEFAULT_MIB_NAME) + MIB_CTRL_EXT : string(name) + MIB_CTRL_EXT;
  auto selectorRegSize = DefaultBinaryPathSelector::RegWidthForPathCount(maxDerivations, false);
  auto selectorReg     = m_model.CreateRegister (selectorRegName, BinaryVector(selectorRegSize, 0), true);
  auto selector        = make_shared<DefaultBinaryPathSelector>(selectorReg, maxDerivations);

  // ---------------- Create the mib
  //
  auto mib = Create_MIB(name, selector, selectorReg, MuxRegPlacement::BeforeMux);

  return mib;
}
//
//  End of: TestModelBuilder::Create_Default_MIB
//---------------------------------------------------------------------------


//! Creates a SIB (1687) with a default (binary) selector
//!
std::shared_ptr<Chain> TestModelBuilder::Create_Default_SIB (string_view name)
{
  // ---------------- Prepare default selector
  //
  auto sibName = name.empty() ? string_view(DEFAULT_SIB_NAME) : name;

  auto selectorRegName = string(sibName) + SIB_CTRL_EXT;
  auto selectorRegSize = DefaultBinaryPathSelector::RegWidthForPathCount(1u, true);
  auto selectorReg     = m_model.CreateRegister (selectorRegName, BinaryVector(selectorRegSize, 0), true);
  auto selector        = make_shared<DefaultBinaryPathSelector>(selectorReg, 1, false, true);

  // ---------------- Create the sib (a mib with only one possible derivation)
  //
  auto sib = Create_MIB(sibName, selector, selectorReg, MuxRegPlacement::BeforeMux);

  return sib;
}
//
//  End of: TestModelBuilder::Create_Default_SIB
//---------------------------------------------------------------------------



//! Creates a default sut below a TAP
//!
shared_ptr<AccessInterface> TestModelBuilder::Create_Default_SUT (string_view name)
{
  // ---------------- Create tap
  //
  auto tap = m_model.CreateTap (name, DEFAULT_IR_LEN, DEFAULT_TDR_LEN);

  // ---------------- Append "SUT"
  //
  auto sut = m_model.CreateChain    ("sut", tap);
  auto reg = m_model.CreateRegister ("static", BinaryVector(STATIC_TDR_LEN, 0), sut);

  // ---------------- Forward node appender from tap to sut
  //
  tap->SetChildAppender(sut);

  return tap;
}
//
//  End of: TestModelBuilder::Create_Default_Sut
//---------------------------------------------------------------------------



//! Creates a "1500" style wrapper
//!
std::shared_ptr<Chain> TestModelBuilder::Create_1500_Wrapper (string_view name, uint32_t maxDerivations)
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
  auto swirSelectorReg = m_model.CreateRegister ("SWIR"s + MIB_CTRL_EXT, BinaryVector::CreateFromBinaryString("0"), true);
  auto swirSelector    = make_shared<DefaultBinaryPathSelector>(swirSelectorReg, 2);
  auto swirMib         = Create_MIB("SWIR", swirSelector, swirSelectorReg, MuxRegPlacement::BeforeMux);
  wrapper->AppendChild(swirMib);

  // ---------------- WIR
  //
  auto totalDerivations = maxDerivations + 1u;   // +1 is to take into account bypass register (wirBypass)
  auto wirSize          = DefaultBinaryPathSelector::RegWidthForPathCount(totalDerivations, false);
  auto wirReg           = m_model.CreateRegister ("WIR_reg", BinaryVector(wirSize, 0), true);
  auto wirSelector      = make_shared<DefaultBinaryPathSelector>(wirReg, totalDerivations);
  auto wirMib           = Create_MIB("WIR", wirSelector, wirReg, MuxRegPlacement::Remote);
  auto wirBypass        = m_model.CreateRegister ("WBY", BinaryVector(1, 0), wirMib);

  swirMib->AppendChild(wirMib);
  swirMib->AppendChild(wirReg);
  wrapper->SetChildAppender(wirMib);

  return wrapper;
}
//
//  End of: TestModelBuilder::Create_1500_Wrapper
//---------------------------------------------------------------------------

//! Creates a simple 1149 tap node with two multiplexed registers
//!
//! @note - There are multiple "dynamic" registers
//!       - The control register is composed with multiple bits
//!
//! @param name         Name for top node
//!
//! @return Top node of system mode
//!
shared_ptr<AccessInterface> TestModelBuilder::Create_TestCase_AccessInterface (string_view name)
{
  uint32_t muxDrPathCount = 3u;

  auto tap     = m_model.CreateTap      (name,       DEFAULT_IR_LEN, muxDrPathCount);

  auto chain_1 = m_model.CreateChain    ("sut_1",    tap);
  auto reg_1   = m_model.CreateRegister ("static_1", BinaryVector(STATIC_TDR_LEN, 0), chain_1);

  auto chain_2 = m_model.CreateChain    ("sut_2",    tap);
  auto reg_2   = m_model.CreateRegister ("static_2", BinaryVector(STATIC_TDR_LEN, 0), chain_2);

  return tap;
}
//
//  End of: TestModelBuilder::Create_TestCase_1500
//---------------------------------------------------------------------------


//! Creates a MIB structure (with multiple insertion bits configuration)
//!
//! @note - There are multiple "dynamic" registers
//!       - The control register is composed with multiple bits
//!
//! @param name           Name for top node
//! @param registersCount Number of MIB mux derivations
//!
//! @return Top node of created sub-tree
//!
shared_ptr<AccessInterface> TestModelBuilder::Create_TestCase_MIB (string_view name, uint32_t registersCount)
{
  // ---------------- Create SUT
  //
  auto tap = Create_Default_SUT(name);

  // ---------------- Append MIB
  //
  auto mib = Create_Default_MIB("", registersCount);
  tap->AppendChild(mib);
  tap->SetChildAppender(mib);

  // ---------------- Add registers
  //
  AppendRegisters(registersCount, "dynamic_", BinaryVector(DYNAMIC_TDR_LEN, 0), tap);

  return tap;
}
//
//  End of: TestModelBuilder::Create_TestCase_MIB
//---------------------------------------------------------------------------


//! Creates a SIB (1687) structure (single insertion bits configuration)
//!
//! @note - There is single "dynamic" register
//!       - The control register has a single bit
//!
//! @param name Name for top node
//!
//! @return Top node of created sub-tree
//!
shared_ptr<AccessInterface> TestModelBuilder::Create_TestCase_1687 (string_view name)
{
  // ---------------- Create SUT
  //
  auto tap = Create_Default_SUT(name);

  // ---------------- Append SIB
  //
  auto sib = Create_Default_SIB(DEFAULT_SIB_NAME);
  tap->AppendChild(sib);
  tap->SetChildAppender(sib);

  // ---------------- Add register
  //
  m_model.CreateRegister("dynamic", BinaryVector(DYNAMIC_TDR_LEN, 0), tap);

  return tap;
}
//
//  End of: TestModelBuilder::Create_TestCase_1687
//---------------------------------------------------------------------------


//! Creates a multi-chain MIB with control register placed before the mux (Linker)
//!
//! @note - There are multiple "dynamic" registers
//!       - The control register is composed with multiple bits
//!
//! @param name         Name for top node
//! @param chainsCount  Number of MIB mux derivations
//!
//! @return Top node of created sub-tree
shared_ptr<AccessInterface> TestModelBuilder::Create_TestCase_MIB_Multichain_Pre (string_view name, uint32_t chainsCount)
{
  auto tap         = m_model.CreateTap(name, 8u, 3u);

  // ---------------- Append MIB with control register before mux
  //
  auto mibCtrl     = m_model.CreateRegister("MIB_ctrl", BinaryVector::CreateFromBinaryString("00"), tap);
  auto mibSelector = make_shared<DefaultBinaryPathSelector>(mibCtrl, chainsCount);
  auto mibMux      = m_model.CreateLinker("MIB_mux", mibSelector, tap);

  // ---------------- Add wrapped cores (registers)
  //
  AppendRegisters(chainsCount, "dynamic_", BinaryVector(DYNAMIC_TDR_LEN, 0), mibMux);

  return tap;
}
//
//  End of: TestModelBuilder::Create_TestCase_MIB_Multichain_Pre
//---------------------------------------------------------------------------


//! Creates a multi-chain MIB with control register placed before the mux (Linker)
//!
//! @note - There are multiple "dynamic" registers
//!       - The control register is composed with multiple bits
//!
//! @param name         Name for top node
//! @param chainsCount  Number of MIB mux derivations
//!
//! @return Top node of created sub-tree
shared_ptr<AccessInterface> TestModelBuilder::Create_TestCase_MIB_Multichain_Post (string_view name, uint32_t chainsCount)
{
  auto tap         = m_model.CreateTap(name, 8u, 3u);

  // ---------------- Append MIB with control register before mux
  //
  auto mibCtrl     = m_model.CreateRegister("MIB_ctrl", BinaryVector::CreateFromBinaryString("00"), nullptr);
  auto mibSelector = make_shared<DefaultBinaryPathSelector>(mibCtrl, chainsCount);
  auto mibMux      = m_model.CreateLinker("MIB_mux", mibSelector, tap);

  tap->AppendChild(mibCtrl);

  // ---------------- Add wrapped cores (registers)
  //
  AppendRegisters(chainsCount, "dynamic_", BinaryVector(DYNAMIC_TDR_LEN, 0), mibMux);

  return tap;
}
//
//  End of: TestModelBuilder::Create_TestCase_MIB_Multichain_Post
//---------------------------------------------------------------------------


//! Creates a 1687 structure with multiple insertion bits configuration
//!
//! @note - There are multiple "dynamic" registers
//!       - The control register is composed with multiple bits
//!
//! @param name         Name for top node
//! @param chainsCount  Number of mux derivations (excluding bypass register)
//!
//! @return Top node of created sub-tree
shared_ptr<AccessInterface> TestModelBuilder::Create_TestCase_1500 (string_view name, uint32_t chainsCount)
{
  // ---------------- Create SUT
  //
  auto tap = Create_Default_SUT(name);

  // ---------------- Append 1500 wrapper
  //
  auto wrapper = Create_1500_Wrapper("", chainsCount);
  tap->AppendChild(wrapper);

  // ---------------- Add 1500 wrapped cores (registers)
  //
  AppendRegisters(chainsCount, "dynamic_", BinaryVector(DYNAMIC_TDR_LEN, 0), wrapper);

  return tap;
}
//
//  End of: TestModelBuilder::Create_TestCase_1500
//---------------------------------------------------------------------------


//! Creates a system model with 6 hierachical levels
//!
//! @note It is intended to be use in tests like:
//!         - Locating nodes (from path, parent of child...)
//!         - Disconnecting node
//!         - Name checking
//!         - ...
//!         -
shared_ptr<AccessInterface> TestModelBuilder::Create_UnitTestCase_6_Levels ()
{
  auto tap    = m_model.CreateTap("Tap", 6u, 2u);
  auto tapMux = m_model.LinkerWithId(2u);
  tapMux->IgnoreForNodePath(true);

  auto chain_0 = m_model.CreateChain("Chain_0", tap);

  // ---------------- Level 1
  //
  m_model.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("0001_1"), chain_0);
  auto chain_1_1 = m_model.CreateChain("Chain", chain_0);
  auto chain_1_2 = m_model.CreateChain("Chain", chain_0);
  m_model.CreateRegister("Reg_4", BinaryVector::CreateFromBinaryString("1000"), chain_0);
  chain_1_1->IgnoreForNodePath(true);

  // ---------------- Level 2
  //
  m_model.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("0010_1"),  chain_1_2);
  m_model.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("0010_10"), chain_1_2);
  auto chain_2_3 = m_model.CreateChain("Chain", chain_1_2);
  auto chain_2_4 = m_model.CreateChain("Chain_2", chain_1_1);

  m_model.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("0010"), chain_1_1);
  m_model.CreateRegister("Reg_3", BinaryVector::CreateFromBinaryString("0011"), chain_1_1);

  // ---------------- Level 3
  //
  m_model.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("1000"), chain_2_3);
  m_model.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("1001"), chain_2_3);
  m_model.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("0011_11"), chain_2_4);

  // ---------------- Another unique in level two
  //
  m_model.CreateRegister("Reg_5", BinaryVector::CreateFromBinaryString("0010_101"), chain_1_2);

  return tap;
}
//
//  End of: TestModelBuilder::Create_UnitTestCase_6_Levels
//---------------------------------------------------------------------------


//! Creates a MIB sub-tree
//!
//! @param name               Name associated with MIB top node
//! @param selector           The selector that manage mux state
//! @param selectorRegister   The selector register that will drive the mux
//! @param muxRegPlacement       Whether mux is placed before or after the register that will drive it
//!
shared_ptr<Chain> TestModelBuilder::Create_MIB (string_view              name,
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
//  End of: TestModelBuilder::Create_MIB
//---------------------------------------------------------------------------


//! Creates very simple MIB structure
//!
shared_ptr<mast::Chain> TestModelBuilder::Create_Simple_MIB (std::experimental::string_view rootName)
{
  auto root         = m_model.CreateChain    (rootName);
  auto regStatic    = m_model.CreateRegister ("static",    BinaryVector(STATIC_TDR_LEN, 0x00, SizeProperty::Fixed),  root);
  auto mib          = m_model.CreateChain    ("MIB",       root);
  auto mib_Ctrl     = m_model.CreateRegister ("MIB_Ctrl",  BinaryVector(2U, 0u, SizeProperty::Fixed), true, mib);
  auto pathSelector = make_shared<DefaultBinaryPathSelector>(mib_Ctrl, 4u);
  auto mux          = m_model.CreateLinker   ("MIB_Mux",   pathSelector, mib);

  m_model.CreateRegister ("dynamic_0", BinaryVector(DYNAMIC_TDR_LEN), mux);
  m_model.CreateRegister ("dynamic_1", BinaryVector(DYNAMIC_TDR_LEN), mux);
  m_model.CreateRegister ("dynamic_2", BinaryVector(DYNAMIC_TDR_LEN), mux);
  m_model.CreateRegister ("dynamic_3", BinaryVector(DYNAMIC_TDR_LEN), mux);

  return root;
}
//
//  End of TestModelBuilder::Create_Simple_MIB
//---------------------------------------------------------------------------




//===========================================================================
// End of TestModelBuilder.cpp
//===========================================================================
