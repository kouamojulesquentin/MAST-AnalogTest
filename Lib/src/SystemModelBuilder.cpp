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

using std::string;
using std::experimental::string_view;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;

using namespace std::string_literals;
using namespace mast;


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



//! Creates a "1500" style wrapper
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
//  End of: SystemModelBuilder::Create_1500_Wrapper
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
