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
#include "SystemModelNode.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "LoopbackAccessInterfaceProtocol.hpp"
#include "SVF_RawPlayer.hpp"
#include "I2C_RawPlayer.hpp"
#include "Spy_Emulation_Translator.hpp"
#include "BinaryVector_Traits.hpp"
#include "JTAG_to_I2C_TranslatorProtocol.hpp"
#include "JTAG_BitBang_TranslatorProtocol.hpp"

using std::string;
using std::experimental::string_view;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;

using namespace std::string_literals;
using std::initializer_list;
using namespace mast;
using namespace test;



//! Appends several chains (with same name prefix) to a parent
//!
void TestModelBuilder::AppendChains (uint32_t                     count,
                                     const string&                baseName,
                                     shared_ptr<mast::ParentNode> parent)
{
  for (uint32_t ii = 0 ; ii < count ; ++ii)
  {
    auto name = baseName + std::to_string(ii);
    m_model.CreateChain (name, parent);
  }
}
//
//  End of: TestModelBuilder::AppendChains
//---------------------------------------------------------------------------



//! Creates a MIB with a default (binary) selector
//!
std::shared_ptr<Chain> TestModelBuilder::Create_Default_MIB (string_view name, uint32_t maxEndPoints)
{
  // ---------------- Prepare default selector
  //
  auto selectorRegName = name.empty() ? string(DEFAULT_MIB_NAME) + MIB_CTRL_EXT : string(name) + MIB_CTRL_EXT;

  auto res         = m_builder.Create_PathSelector(SelectorKind::Binary, selectorRegName, maxEndPoints);
  auto selectorReg = res.first;
  auto selector    = res.second;

  // ---------------- Create the mib
  //
  auto mib = m_builder.Create_MIB(name, selector, selectorReg, MuxRegPlacement::BeforeMux);

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
  auto res             = m_builder.Create_PathSelector(SelectorKind::Binary, selectorRegName, 1u, SelectorProperty::CanSelectNone);
  auto selectorReg     = res.first;
  auto selector        = res.second;

  // ---------------- Create the sib (a mib with only one possible endpoint)
  //
  auto sib = m_builder.Create_MIB(sibName, selector, selectorReg, MuxRegPlacement::BeforeMux);

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
  auto tap = Create_JTAG_TAP (name, DEFAULT_IR_LEN, DEFAULT_TDR_LEN);

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


//! Creates a new Tap node
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
shared_ptr<AccessInterface> TestModelBuilder::Create_JTAG_TAP (string_view name, uint32_t irBitsCount, uint32_t muxPathsCount)
{
  auto protocol = make_shared<LoopbackAccessInterfaceProtocol > ();

  return m_builder.Create_JTAG_TAP(name, irBitsCount, muxPathsCount, protocol);
}
//
//  End of: TestModelBuilder::Create_JTAG_TAP
//---------------------------------------------------------------------------

//! Creates a new Tap node
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

shared_ptr<AccessInterface> TestModelBuilder::Create_JTAG_TAP (string_view name, uint32_t irBitsCount, uint32_t muxPathsCount,shared_ptr<AccessInterfaceProtocol> protocol)
{
  return m_builder.Create_JTAG_TAP(name, irBitsCount, muxPathsCount, protocol);
}
//
//  End of: TestModelBuilder::Create_JTAG_TAP
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

  auto tap     = Create_JTAG_TAP    (name,       DEFAULT_IR_LEN, muxDrPathCount);

  auto chain_1 = m_model.CreateChain    ("sut_1",    tap);
  auto reg_1   = m_model.CreateRegister ("static_1", BinaryVector(STATIC_TDR_LEN, 0), chain_1);

  auto chain_2 = m_model.CreateChain    ("sut_2",    tap);
  auto reg_2   = m_model.CreateRegister ("static_2", BinaryVector(STATIC_TDR_LEN, 0), chain_2);

  return tap;
}
//
//  End of: TestModelBuilder::Create_TestCase_AccessInterface
//---------------------------------------------------------------------------


//! Creates a simple 1149 tap node with two multiplexed registers, with an Emulation_Translator as top level
//!
//! @note - There are multiple "dynamic" registers
//!       - The control register is composed with multiple bits
//!
//! @param name         Name for top node
//!
//! @return Top node of system mode
//!
shared_ptr<AccessInterfaceTranslator> TestModelBuilder::Create_TestCase_Emulation_Translator (string_view name)
{
  uint32_t muxDrPathCount = 3u;
  
  auto raw_protocol = make_shared<SVF_RawPlayer>();

  auto tap     = m_builder.Create_JTAG_TAP    ("Tap",       DEFAULT_IR_LEN, muxDrPathCount,raw_protocol);

  auto chain_1 = m_model.CreateChain    ("sut_1",    tap);
  auto reg_1   = m_model.CreateRegister ("static_1", BinaryVector(STATIC_TDR_LEN, 0), chain_1);

  auto chain_2 = m_model.CreateChain    ("sut_2",    tap);
  auto reg_2   = m_model.CreateRegister ("static_2", BinaryVector(STATIC_TDR_LEN, 0), chain_2);

  auto translator     = m_model.CreateAccessInterfaceTranslator    (name, make_shared<Spy_Emulation_Translator>());

  translator->RegisterInterface(tap);

  translator->AppendChild(tap);

  m_model.ReplaceRoot(translator,false);
  
  return translator;
}
//
//  End of: TestModelBuilder::Create_TestCase_Emulation_Translator
//---------------------------------------------------------------------------

//! Creates a simple 1149 tap node with two multiplexed registers, with an Emulation_Translator as top level
//! but with a JTAG_to_I2C translator in the middle
//!
//! @note - There are multiple "dynamic" registers
//!       - The control register is composed with multiple bits
//!
//! @param name         Name for top node
//!
//! @return Top node of system mode
//!
shared_ptr<AccessInterfaceTranslator> TestModelBuilder::Create_TestCase_JTAG_to_I2C_Translator (string_view name)
{
  uint32_t muxDrPathCount = 3u;
  auto I2C_Adresses   = initializer_list<uint32_t>{ 0x30u, 0x31u, 0x32u,0x33u,0x34u  };

  auto raw_protocol = make_shared<SVF_RawPlayer>();

  auto tap     = Create_JTAG_TAP    ("Tap",       DEFAULT_IR_LEN, muxDrPathCount,raw_protocol);

  auto chain_1 = m_model.CreateChain    ("sut_1",    tap);
  auto reg_1   = m_model.CreateRegister ("static_1", BinaryVector(STATIC_TDR_LEN, 0), chain_1);

  auto chain_2 = m_model.CreateChain    ("sut_2",    tap);
  auto reg_2   = m_model.CreateRegister ("static_2", BinaryVector(STATIC_TDR_LEN, 0), chain_2);

  auto top_translator     = m_model.CreateAccessInterfaceTranslator    ("Emulation", make_shared<Spy_Emulation_Translator>());
  auto jtag_2_i2c_translator     = m_model.CreateAccessInterfaceTranslator    (name, make_shared<JTAG_to_I2C_TranslatorProtocol>(I2C_Adresses));

  m_model.ReplaceRoot(top_translator,false);
  top_translator->AppendChild(jtag_2_i2c_translator);
  jtag_2_i2c_translator->AppendChild(tap);

  top_translator->RegisterTranslator(jtag_2_i2c_translator);

  jtag_2_i2c_translator->RegisterInterface(tap);

  return top_translator;
}
//
//  End of: TestModelBuilder::Create_TestCase_AccessInterface
//---------------------------------------------------------------------------

//! Creates a simple 1149 tap node with two multiplexed registers, with an Emulation_Translator as top level
//! but with a JTAG_to_I2C translator in the middle
//!
//! @note - There are multiple "dynamic" registers
//!       - The control register is composed with multiple bits
//!
//! @param name         Name for top node
//!
//! @return Top node of system mode
//!
shared_ptr<AccessInterfaceTranslator> TestModelBuilder::Create_TestCase_JTAG_BitBang_Translator (string_view name)
{
  uint32_t muxDrPathCount = 3u;
  auto I2C_Adresses   = initializer_list<uint32_t>{ 0x30u, 0x31u, 0x32u,0x33u,0x34u  };

  auto raw_protocol = make_shared<SVF_RawPlayer>();
  auto top_tap     = Create_JTAG_TAP    ("Top-Tap",       DEFAULT_IR_LEN, 2u,raw_protocol);

  auto etap     = Create_JTAG_TAP    ("eTap",       DEFAULT_IR_LEN, muxDrPathCount, make_shared<SVF_RawPlayer>());

  auto chain_1 = m_model.CreateChain    ("sut_1",    etap);
  auto reg_1   = m_model.CreateRegister ("static_1", BinaryVector(STATIC_TDR_LEN, 0), chain_1);

  auto chain_2 = m_model.CreateChain    ("sut_2",    etap);
  auto reg_2   = m_model.CreateRegister ("static_2", BinaryVector(STATIC_TDR_LEN, 0), chain_2);

  auto top_translator     = m_model.CreateAccessInterfaceTranslator    ("Emulation", make_shared<Spy_Emulation_Translator>());

  auto jtag_bitbang_translator     = m_model.CreateAccessInterfaceTranslator    (name, make_shared<JTAG_BitBang_TranslatorProtocol>());

  m_model.ReplaceRoot(top_translator,false);

  top_translator->AppendChild(top_tap);
  top_translator->RegisterInterface(top_tap);
  

  auto bitbang_chain = m_model.CreateChain    ("BB_chain");
  auto bitbang_reg   = m_model.CreateRegister ("BB_Reg", BinaryVector(5, 0), bitbang_chain);
  bitbang_chain->AppendChild(jtag_bitbang_translator);

  top_tap->AppendChild(bitbang_chain);
  jtag_bitbang_translator->AppendChild(etap);


  jtag_bitbang_translator->RegisterInterface(etap);

  return top_translator;
}
//
//  End of: TestModelBuilder::Create_TestCase_AccessInterface
//---------------------------------------------------------------------------

//! Creates a simple I2C Access Interface with two registers, with an Emulation_Translator as top level
//!
//! @note - There are multiple "dynamic" registers
//!       - The control register is composed with multiple bits
//!
//! @param name         Name for top node
//!
//! @return Top node of system mode
//!
shared_ptr<AccessInterfaceTranslator> TestModelBuilder::Create_TestCase_I2C_Emulation_Translator (string_view name)
{
  uint32_t muxDrPathCount = 3u;
  
  auto regsCount = 3;
  auto regsBitsCount = 4;

    auto I2C_Adresses   = initializer_list<uint32_t>{ 0x30u, 0x31u, 0x32u,0x33u,0x34u  };
  auto raw_protocol = make_shared<I2C_RawPlayer>(I2C_Adresses);
  auto ai   = m_model.CreateAccessInterface(name, raw_protocol);


  // ---------------- Add Registers
  //
  m_builder.AppendRegisters(regsCount, "i2c_register_", BinaryVector(regsBitsCount, 0), ai);


  auto translator     = m_model.CreateAccessInterfaceTranslator    (name, make_shared<Spy_Emulation_Translator>());

//  raw_protocol->SetParentTranslator(translator);
   translator->RegisterInterface(ai);


  translator->AppendChild(ai);

  m_model.SetRoot(translator);
  
  return translator;
}
//
//  End of: TestModelBuilder::I2C_Emulation_Translator
//---------------------------------------------------------------------------

//! Creates a MIB structure (with multiple insertion bits configuration)
//!
//! @note - There are multiple "dynamic" registers
//!       - The control register is composed with multiple bits
//!
//! @param name           Name for top node
//! @param regsCount      Number of MIB mux endpoints
//! @param regsBitsCount  Number of bits of each registers
//!
//! @return Top node of created sub-tree
//!
shared_ptr<AccessInterface> TestModelBuilder::Create_TestCase_MIB (string_view name,
                                                                   uint32_t    regsCount,
                                                                   uint32_t    regsBitsCount)
{
  // ---------------- Create SUT
  //
  auto tap = Create_Default_SUT(name);

  // ---------------- Append MIB
  //
  auto mib = Create_Default_MIB("", regsCount);
  tap->AppendChild(mib);
  tap->SetChildAppender(mib);

  // ---------------- Add registers
  //
  m_builder.AppendRegisters(regsCount, "dynamic_", BinaryVector(regsBitsCount, 0), tap);

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
//! @param name           Name for top node
//! @param regsCount      Number of MIB mux endpoints
//! @param regsBitsCount  Number of bits of each registers
//!
//! @return Top node of created sub-tree
shared_ptr<AccessInterface> TestModelBuilder::Create_TestCase_MIB_Multichain_Pre (string_view name,
                                                                                  uint32_t    regsCount,
                                                                                  uint32_t    regsBitsCount)
{
  auto tap         = Create_JTAG_TAP(name, 8u, 3u);

  // ---------------- Append MIB with control register before mux
  //
  auto res         = m_builder.Create_PathSelector(SelectorKind::Binary, "MIB_ctrl", regsCount);
  auto mibCtrl     = res.first;
  auto mibSelector = res.second;

  tap->AppendChild(mibCtrl);
  auto mibMux      = m_model.CreateLinker("MIB_mux", mibSelector, tap);

  // ---------------- Add wrapped cores (registers)
  //
  m_builder.AppendRegisters(regsCount, "dynamic_", BinaryVector(regsBitsCount, 0), mibMux);

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
//! @param name           Name for top node
//! @param regsCount      Number of MIB mux endpoints
//! @param regsBitsCount  Number of bits of each registers
//!
//! @return Top node of created sub-tree
shared_ptr<AccessInterface> TestModelBuilder::Create_TestCase_MIB_Multichain_Post (string_view name,
                                                                                   uint32_t    regsCount,
                                                                                   uint32_t    regsBitsCount)
{
  auto tap         = Create_JTAG_TAP(name, 8u, 3u);

  // ---------------- Append MIB with control register before mux
  //
  auto res         = m_builder.Create_PathSelector(SelectorKind::Binary, "MIB_ctrl", regsCount);
  auto mibCtrl     = res.first;
  auto mibSelector = res.second;
  auto mibMux      = m_model.CreateLinker("MIB_mux", mibSelector, tap);

  tap->AppendChild(mibCtrl);

  // ---------------- Add wrapped cores (registers)
  //
  m_builder.AppendRegisters(regsCount, "dynamic_", BinaryVector(regsBitsCount, 0), mibMux);

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
//! @param name           Name for top node
//! @param regsCount      Number of mux endpoints (excluding bypass register)
//! @param regsBitsCount  Number of bits of each registers
//!
//! @return Top node of created sub-tree
shared_ptr<AccessInterface> TestModelBuilder::Create_TestCase_1500 (string_view name,
                                                                    uint32_t    regsCount,
                                                                    uint32_t    regsBitsCount)
{
  // ---------------- Create SUT
  //
  auto tap = Create_Default_SUT(name);

  // ---------------- Append 1500 wrapper
  //
  SystemModelBuilder m_builder(m_model);
  auto wrapper = m_builder.Create_1500_Wrapper("", regsCount);
  tap->AppendChild(wrapper);

  // ---------------- Add 1500 wrapped cores (registers)
  //
  m_builder.AppendRegisters(regsCount, "dynamic_", BinaryVector(regsBitsCount, 0), wrapper);

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
  auto tap    = Create_JTAG_TAP("Tap", 6u, 2u);
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


shared_ptr<AccessInterfaceTranslator> TestModelBuilder::Create_Brocade (std::shared_ptr<AccessInterfaceTranslatorProtocol>  TopProtocol,
                                                      std::shared_ptr<AccessInterfaceProtocol>           masterProtocol,
                                                      std::shared_ptr<AccessInterfaceProtocol>           slaveProtocol,
                                                      std::initializer_list<std::shared_ptr<AccessInterface>> taps)
{
 return m_builder.Create_Brocade(TopProtocol,masterProtocol,slaveProtocol,taps);
}


//===========================================================================
// End of TestModelBuilder.cpp
//===========================================================================
