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
void SystemModelBuilder::AppendRegisters (uint32_t               count,
                                          const string&          baseName,
                                          const BinaryVector&    bypass,
                                          shared_ptr<ParentNode> parent)
{
  for (int ii = 0 ; ii < count ; ++ii)
  {
    auto regName = baseName + std::to_string(ii);
    m_model.CreateRegister (regName, bypass, parent);
  }
}
//
//  End of: SystemModelBuilder::AppendRegisters
//---------------------------------------------------------------------------


//! Creates a wrapper for TAP
//!
std::shared_ptr<Chain> SystemModelBuilder::Create_1500_Wrapper (string_view name, uint32_t maxDerivations)
{
  // ---------------- Create wrapper
  //
  if (name.empty())
  {
    name = DEFAULT_1500_NAME;
  }
  auto wrapper = m_model.CreateChain (name);

  // ---------------- Create MIB
  //
  auto        selectorReg  = m_model.CreateRegister ("SWIR"s + MIB_CTRL_EXT, BinaryVector::CreateFromBinaryString("01"));
  auto        pathSelector = make_shared<DefaultBinaryPathSelector>(selectorReg, 2);

  auto swirMib = Create_MIB("SWIR", pathSelector, selectorReg, MuxRegPlacement::BeforeMux);
  wrapper->AppendChild(swirMib);

  auto wirSize     = DefaultBinaryPathSelector::RegWidthForPathCount(maxDerivations + 1u);
  auto wirReg      = m_model.CreateRegister ("WIR_reg", BinaryVector(wirSize, 0));
  auto wirSelector = make_shared<DefaultBinaryPathSelector>(wirReg, 2);
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

//! Creates a simple 1149 tap node with two multiplexed registers
//!
//! @note - There are multiple "dynamic" registers
//!       - The control register is composed with multiple bits
//!
//! @param name         Name for top node
//!
//! @return Top node of system mode
//!
shared_ptr<AccessInterface> SystemModelBuilder::Create_TestCase_AccessInterface (string_view name)
{
  auto tap     = m_model.CreateTap      (name,       DEFAULT_IR_LEN, 3u);

  auto chain_1 = m_model.CreateChain    ("sut_1",    tap);
  auto reg_1   = m_model.CreateRegister ("static_1", BinaryVector(STATIC_TDR_LEN, 0), chain_1);

  auto chain_2 = m_model.CreateChain    ("sut_2",    tap);
  auto reg_2   = m_model.CreateRegister ("static_2", BinaryVector(STATIC_TDR_LEN, 0), chain_2);

  return tap;
}
//
//  End of: SystemModelBuilder::Create_TestCase_1500
//---------------------------------------------------------------------------


//! Creates a 1687 structure with multiple insertion bits configuration
//!
//! @note - There are multiple "dynamic" registers
//!       - The control register is composed with multiple bits
//!       - Many coding may be defined for segment selection
//!
//! @param name         Name for top node
//! @param chainsCount  Number of mux derivations
//!
//! @return Top node of created sub-tree
shared_ptr<AccessInterface> SystemModelBuilder::Create_TestCase_1500 (string_view name, uint32_t chainsCount)
{
  // ---------------- Create tap
  //
  uint32_t    irBitsCount   = 8;
  uint32_t    muxPathsCount = chainsCount + 1u; // +1 is for the (hidden) bypass register

  auto tap   = m_model.CreateTap      (name, irBitsCount, muxPathsCount);

  // ---------------- Append "SUT"
  //
  auto chain = m_model.CreateChain    ("sut", tap);
  auto reg   = m_model.CreateRegister ("static", BinaryVector(STATIC_TDR_LEN, 0), chain);

  // ---------------- Append 1500 wrapper
  //
  auto wrapper = Create_1500_Wrapper("", WRAPPED_CORES);
  chain->AppendChild(wrapper);

  // ---------------- Add 1500 wrapped cores (registers)
  //
  AppendRegisters(chainsCount, "dynamic_", BinaryVector(DYNAMIC_TDR_LEN, 0), wrapper);


  return tap;
}
//
//  End of: SystemModelBuilder::Create_TestCase_1500
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



//===========================================================================
// End of SystemModelBuilder.cpp
//===========================================================================
