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
#include "Utility.hpp"

using std::string;
using std::experimental::string_view;
using std::shared_ptr;
using std::make_shared;
using std::make_pair;
using std::pair;
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
  auto res             = Create_PathSelector(SelectorKind::Binary, "SWIR"s + MIB_CTRL_EXT, 2u);
  auto swirSelectorReg = res.first;
  auto swirSelector    = res.second;
  auto swirMib         = Create_MIB("SWIR", swirSelector, swirSelectorReg, MuxRegPlacement::BeforeMux);
  wrapper->AppendChild(swirMib);

  // ---------------- WIR
  //
  auto totalDerivations = maxDerivations + 1u;   // +1 is to take into account bypass register (wirBypass)

       res         = Create_PathSelector(SelectorKind::Binary, "WIR_reg", totalDerivations);
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
shared_ptr<AccessInterface> SystemModelBuilder::Create_JTAG_TAP (string_view                         name,
                                                                 uint32_t                            irBitsCount,
                                                                 uint32_t                            muxPathsCount,
                                                                 shared_ptr<AccessInterfaceProtocol> protocol)
{
  CHECK_PARAMETER_NOT_ZERO (irBitsCount, "irBitsCount must be != 0");
  CHECK_PARAMETER_GT       (muxPathsCount, 1, "muxPathsCount must be > 1");

  auto noName     = name.empty();
  auto rootName   = noName ? DEFAULT_TAP_NAME         : name;
  auto irName     = noName ? DEFAULT_TAP_IR_NAME      : string(name) + DEFAULT_TAP_IR_EXT;
  auto muxName    = noName ? DEFAULT_TAP_MUX_NAME     : string(name) + DEFAULT_TAP_MUX_EXT;
  auto muxBpyName = noName ? DEFAULT_TAP_MUX_BPY_NAME : string(name) + DEFAULT_TAP_MUX_BPY_EXT;

  auto accessInterface = m_model.CreateAccessInterface(rootName, protocol);

  // ---------------- Create IR
  //
  auto irBypassSequence = BinaryVector(irBitsCount, 0xFF);
  auto ir               = m_model.CreateRegister(irName, irBypassSequence, true, accessInterface);

  // ---------------- Create path selector
  //
  // Select table is by default binary except for no path and 1st that use the bypass sequence
  auto selectTable = DefaultBinaryPathSelector::CreateSelectTable(irBitsCount, muxPathsCount, SelectorProperty::Binary_Default);
  selectTable[0] = irBypassSequence;  // Not used (path id zero)
  selectTable[1] = irBypassSequence;  // Bypass register

  // Deselect table is by default all bypass sequence
  auto deselectTable = DefaultTableBasedPathSelector::TablesType(muxPathsCount + 1u, irBypassSequence);

  auto pathSelector = make_shared<DefaultTableBasedPathSelector>(ir, muxPathsCount, selectTable, deselectTable);

  // ---------------- Create Linker
  //
  auto linker       = m_model.CreateLinker(muxName, pathSelector, accessInterface);

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
shared_ptr<Chain> SystemModelBuilder::Create_SIB (string_view              name,
                                           SelectorProperty sel_properties,
                                                  MuxRegPlacement          muxRegPlacement
                                                 )
  {
  auto SIB_properties= sel_properties | SelectorProperty::CanSelectNone;
  auto selectorRegName = string(name)+ MIB_CTRL_EXT;
 auto res=      Create_PathSelector(SelectorKind::Binary, selectorRegName, 1,SIB_properties);
  auto selectorReg = res.first;
  auto selector    = res.second;
 
   return Create_MIB(name, selector, selectorReg,muxRegPlacement );
  }
//
//  End of: SystemModelBuilder::Create_SIB
//---------------------------------------------------------------------------


//! Creates a path selector
//!
//! @param selectorKind         Kind of selector (Binary, One_Hot, N_Hot)
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



//===========================================================================
// End of SystemModelBuilder.cpp
//===========================================================================
