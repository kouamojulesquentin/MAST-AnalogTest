//===========================================================================
//                           AST_SystemModelGenerator.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_SystemModelGenerator.cpp
//!
//! Implements class AST_SystemModelGenerator
//!
//===========================================================================

#include "AST_SystemModelGenerator.hpp"

#include "AST_AccessLink.hpp"
#include "AST_Attribute.hpp"
#include "AST_Instance.hpp"
#include "AST_Module.hpp"
#include "AST_ModuleIdentifier.hpp"
#include "AST_Namespace.hpp"
#include "AST_Network.hpp"
#include "AST_Parameter.hpp"
#include "AST_ParameterRef.hpp"
#include "AST_Port.hpp"
#include "AST_ScalarIdentifier.hpp"
#include "AST_ScanInterface.hpp"
#include "AST_ScanMux.hpp"
#include "AST_ScanMuxSelection.hpp"
#include "AST_ScanRegister.hpp"
#include "AST_Signal.hpp"
#include "AST_Source.hpp"
#include "AST_String.hpp"
#include "AST_Value.hpp"
#include "AST_VectorIdentifier.hpp"

#include "DefaultTableBasedPathSelector.hpp"
#include "UnresolvedPathSelector.hpp"
#include "SystemModel.hpp"
#include "SystemModelBuilder.hpp"
#include "Utility.hpp"
#include "g3log/g3log.hpp"

#include <stack>
#include <functional>

using std::stack;
using std::vector;
using std::shared_ptr;
using std::unique_ptr;
using std::tuple;
using std::make_unique;
using std::make_shared;
using std::make_tuple;

using namespace mast;
using namespace Parsers;

//! Destructs AST_SystemModelGenerator
//!
AST_SystemModelGenerator::~AST_SystemModelGenerator ()
{
}
//
//  End of: AST_SystemModelGenerator::~AST_SystemModelGenerator
//---------------------------------------------------------------------------


//! Constructor...
AST_SystemModelGenerator::AST_SystemModelGenerator (shared_ptr<mast::SystemModel> systemModel)
  : m_systemModel (CHECK_PARAMETER_NOT_NULL(systemModel, "Expect valid, not nullptr SystemModel"))
  , m_builder     (make_unique<SystemModelBuilder>(*m_systemModel))
{
}
//
//  End of: AST_SystemModelGenerator::AST_SystemModelGenerator
//---------------------------------------------------------------------------



//! Returns source of signal(s)
//!
//! @param signals  Set of "associated" signal
//!
//! @note Only supports a single signal
//+AST_Source* AST_SystemModelGenerator::FindSourceOfSignals () const
//+{
//+  CHECK_VALUE_EQ(signals.size(), 1u, "Not Yet Supported: giving single source for more than one signals");

//+  const auto signal      = signals.front();
//+  const auto portScope   = signal->PortScope();
//+  const auto identifier  = signal->PortName();

//+}
//
//  End of: AST_SystemModelGenerator::FindSourceOfSignals
//---------------------------------------------------------------------------


//! Follows ScanMux selector signals to find driving ScanRegister(s)
//!
//! @param selectors  ScanMux selector signals
//! @param module     ScanMux parent module
//!
vector<AST_ScanRegister*>
AST_SystemModelGenerator::FindSelectorRegisters (const std::vector<Parsers::AST_Signal*>& selectors,
                                                 AST_Module*                              module) const
{
  vector<AST_ScanRegister*> scanRegisters;

  for (const auto& selector : selectors)
  {
    const auto portScope   = selector->PortScope();
    const auto identifier  = selector->PortName();

    CHECK_VALUE_EMPTY(portScope, "Not Yet Supported: Selector in different module (for ScanMux)");

    auto scanRegister = module->FindScanRegister(identifier);
    CHECK_PARAMETER_NOT_NULL(scanRegister, "Failed to find selector ScanRegister in module \""s.append(module->Name()).append("\""));

    scanRegisters.push_back(scanRegister);
  }

  return scanRegisters;
}
//
//  End of: AST_SystemModelGenerator::FindSelectorRegisters
//---------------------------------------------------------------------------


//! Generates a SystemModel (sub-)tree from AST_Network
//!
shared_ptr<mast::SystemModelNode> AST_SystemModelGenerator::Generate (AST_Network* network)
{
  m_network = CHECK_PARAMETER_NOT_NULL(network, "Expect valid, not nullptr AST_Network");

  m_parsedTopNode = Generate_Network(network);

  //! @todo [JFC]-[November/23/2017]: In Generate(): Resolve linkers
  //!
  CHECK_VALUE_EMPTY(m_unresolvedPathSelectors, "Not Yet Supported: Unresolved path selector");
//+  for (const auto& linker : m_unresolvedPathSelectors)
//+  {
//+  }
  return m_parsedTopNode;
}
//
//  End of: AST_SystemModelGenerator::Generate
//---------------------------------------------------------------------------


//! Creates a SystemModel nodes for a module instance
//!
//! @param instance   Instance to be converted to SystemModel nodes
//!
//! @return Created SystemModel sub-tree and Instance AST_Source
tuple<shared_ptr<mast::SystemModelNode>, const AST_Source*>
AST_SystemModelGenerator::Generate_Instance (AST_Instance* instance, AST_Module* instanceModule)
{
  const auto& moduleInputPorts = instanceModule->ScanInPorts();

  CHECK_VALUE_NOT_EMPTY(moduleInputPorts,  "Expecting a module \""s.append(instanceModule->Name()).append("\"to have at least one ScanInPort"));

  // ---------------- Find instance source
  //
  auto moduleInputPort   = moduleInputPorts.front();
  auto moduleInputPortId = moduleInputPort->Identifier();
  auto instanceInputPort = instance->FindInputPort(moduleInputPortId);
  auto instanceSource    = instanceInputPort->Source();

  //! @todo [JFC]-[November/17/2017]: In Generate_Instance(): Return all instance source!
  //!

  // ---------------- Create Chain to "encapsulate" instance sub-nodes
  //
  auto name  = instance->Name();
  auto chain = m_systemModel->CreateChain(name);

  //! @todo [JFC]-[November/17/2017]: In Generate_Instance(): Create chain only when there is effectively children !
  //! ==> Have module return children (in order)

  // ---------------- Instantiate module with instance parameters
  //
  CHECK_VALUE_EMPTY(instance->Parameters(), "Not Yet Supported: Instance parameters");
  Generate_Module(chain.get(), instanceModule);

  return make_tuple(chain, instanceSource);
}
//
//  End of: AST_SystemModelGenerator::Generate_Instance
//---------------------------------------------------------------------------


//! Creates complete SystemModel from AST_Network
//!
//! @param network  Network description
//!
//! @return Created SystemModel sub-tree
shared_ptr<mast::SystemModelNode> AST_SystemModelGenerator::Generate_Network (AST_Network* network)
{
  auto topModule = network->TopModule();
  CHECK_VALUE_NOT_NULL(topModule, "Cannot generate SystemModel nodes when network has no modules");

  if (topModule->HasAccessLink())
  {
    auto accessLink = topModule->AccessLink();
    auto type       = accessLink->Type();
    switch (type)
    {
      case AccessLinkType::STD_1149_1_2001:
      case AccessLinkType::STD_1149_1_2013:
        LOG(INFO) << "Creating STD_1149 AccessLink";
        CHECK_FAILED("Not Yet Supported: STD_1149 AccessLink");
        break;
      case AccessLinkType::Generic:
        LOG(INFO) << "Creating Generic" << accessLink->GenericIdentifier()->AsText() << " AccessLink";
        CHECK_FAILED("Not Yet Supported: Generic AccessLink");
        break;
      default:
        CHECK_FAILED("Unexpected AccessLink type");
        break;
    }
  }
  else
  {
    auto name  = topModule->Name();
    auto chain = m_systemModel->CreateChain(name);

    Generate_Module(chain.get(), topModule);
    return chain;
  }
}
//
//  End of: AST_SystemModelGenerator::Generate_Network
//---------------------------------------------------------------------------



//! Creates a SystemModel nodes for a module
//!
//! @param chain  Chain in which SystemModel notes are created
//! @param module Module to be converted to SystemModel nodes
//!
void AST_SystemModelGenerator::Generate_Module (mast::Chain* chain, AST_Module* module)
{
  const auto& scanInPorts  = module->ScanInPorts();
  const auto& scanOutPorts = module->ScanOutPorts();

  CHECK_VALUE_NOT_EMPTY(scanInPorts,  "Expecting a module to have at least one ScanInPort");
  CHECK_VALUE_NOT_EMPTY(scanOutPorts, "Expecting a module to have at least one ScanOutPort");

  auto scanInPort  = scanInPorts.front();
  auto scanOutPort = scanOutPorts.front();
  auto source      = scanOutPort->Source();

  // Lamba: Tells whether some source match module input port (currently only considering first)
  auto isSourcedByModuleInput = [scanInPort](const vector<AST_Signal*>& signals)
  {
    CHECK_VALUE_EQ(signals.size(), 1u, "Expecting source to be driven by exactly one signal");
    const auto signal      = signals.front();
    const auto portScope   = signal->PortScope();
    const auto identifier  = signal->PortName();

    if (!portScope.empty())
    {
      return false;
    }
    return identifier->Name() == scanInPort->Name();
  };

  stack<shared_ptr<SystemModelNode>> children;
  auto sourceSignals = std::cref(source->Signals());

  //! @todo [JFC]-[November/17/2017]: Create a stack of source signals poping it once following it
  //!                                 leads to one of module input
  //! i.e. while (!sourceStack.empty() || !isSourcedByModuleInput(sourceSignals))
  while (!isSourcedByModuleInput(sourceSignals))
  {
    const auto& signals = source->Signals();

    CHECK_VALUE_EQ(signals.size(), 1u, "Expecting ScanOutPort source to be driven by exactly one signal");
    const auto signal      = signals.front();
    const auto portScope   = signal->PortScope();
    const auto identifier  = signal->PortName();

    std::shared_ptr<mast::SystemModelNode> createdNode;

    if (portScope.empty())  // ScanRegister or ScanMux ?
    {
      auto scanRegister = module->FindScanRegister(identifier);
      if (scanRegister != nullptr)
      {
        std::tie(createdNode, source) = Generate_ScanRegister(scanRegister);
        sourceSignals = std::cref(source->Signals());
      }
      else  // ScanMux
      {
        auto scanMux = module->FindScanMux(identifier);
        std::tie(createdNode, sourceSignals) = Generate_ScanMux(scanMux, module);
      }
    }
    else   // Instance ?
    {
      CHECK_VALUE_EQ(portScope.size(), 1u, "Expecting to have single scope depth for instance");

      auto scope          = portScope.front();
      auto instance       = module->FindInstance(scope);
      auto moduleId       = instance->ModuleIdentifier();
      auto instanceModule = m_network->Module(moduleId);

      std::tie(createdNode, source) = Generate_Instance(instance, instanceModule);
      //! @todo [JFC]-[November/17/2017]: In Generate_Module(): get back all instance source
      //!

      sourceSignals = std::cref(source->Signals());
    }

    if (createdNode)
    {
      children.emplace(createdNode);
    }
  }

  // ---------------- Append children to Chain
  //
  while (!children.empty())
  {
    auto child = children.top();

    chain->AppendChild(child);
    children.pop();
  }
}
//
//  End of: AST_SystemModelGenerator::Generate_Module
//---------------------------------------------------------------------------


//! Creates a SystemModel Linker and path selector from an AST_ScanMux
//!
//! @param scanMux  ScanMux to be converted to SystemModel Linker
//! @param module   ScanMux parent module
//!
//! @return Created Linker and ScanMux AST_Source
AST_SystemModelGenerator::Generate_ScanMux_Result_t
AST_SystemModelGenerator::Generate_ScanMux (AST_ScanMux* scanMux, AST_Module* module)
{
  CHECK_FALSE(scanMux->IsBusMux(), "Not Yet Supported: ScanMux for buses");

  // ---------------- Collect selector(s) path(s) including bits ranges
  // Paths are ordered and defined using SystemModel path syntax
  // Bits ranges are defined as pair of integers
  //
  const auto& selectors          = scanMux->Selectors();
  const auto  selectorsBitsCount = selectors.size();

  auto selectorRegisters = FindSelectorRegisters(selectors, module);
  CHECK_VALUE_EQ(selectorRegisters.size(), 1u, "Only support single ScanRegister as ScanMux selector");

  // ---------------- Prepare selection/deselection tables
  //
  vector<BinaryVector> selectTable;
  vector<BinaryVector> deselectTable;

  auto const& selections = scanMux->Selections();
  std::tie(selectTable, deselectTable) = MakeSelectionTable(selections, selectorsBitsCount);

  // ---------------- Prepare path selector
  //
  auto selectorProperties = SelectorProperty::Std;
  auto selectorRegister   = selectorRegisters.front(); //! @todo [JFC]-[November/23/2017]: Support multiple selector registers
  auto modelRegister      = selectorRegister->AssociatedRegister();
  bool unresolved         = modelRegister ? false : true;

  shared_ptr<PathSelector> pathSelector;
  if (unresolved)
  {
    auto unresolvedPathSelector = make_shared<UnresolvedPathSelector>();
//+    unresolvedPathSelector->SelectorRegister(selectorRegister);
    unresolvedPathSelector->SelectionTables(std::move(selectTable), std::move(deselectTable));
    m_unresolvedPathSelectors.emplace_back(unresolvedPathSelector);

    pathSelector = unresolvedPathSelector;
  }
  else
  {
    auto pathsCount = selectTable.size() - 1u;
    pathSelector    = make_shared<DefaultTableBasedPathSelector>(modelRegister, pathsCount, selectTable, deselectTable, selectorProperties);
  }

  // ---------------- Create Linker
  //
  const auto name     = scanMux->BaseName();
  auto       linker   = m_systemModel->CreateLinker(name, pathSelector);

  // ---------------- Define "source" of linker
  //
  const auto  firstSelection  = selections.front();
  const auto& selectedSignals = firstSelection->SelectedSignals();

  return make_tuple(linker, std::cref(selectedSignals));
}
//
//  End of: AST_SystemModelGenerator::Generate_ScanMux
//---------------------------------------------------------------------------



//! Creates a SystemModel Register from an AST_ScanRegister
//!
//! @param scanRegister   ScanRegister to be converted to SystemModel Register
//!
//! @return Created Register and ScanRegister AST_Source
tuple<std::shared_ptr<mast::SystemModelNode>, const AST_Source*>
AST_SystemModelGenerator::Generate_ScanRegister (AST_ScanRegister* scanRegister)
{
  auto registerSource = scanRegister->ScanInSource();
  auto name           = scanRegister->BaseName();
  auto bitsCount      = scanRegister->BitsCount();
  auto resetValue     = scanRegister->ResetValue();

  BinaryVector bypassValue;
  if (resetValue != nullptr)
  {
    bypassValue = resetValue->AsBinaryVector();
  }
  else
  {
    uint8_t fillPattern = 0;
    bypassValue = BinaryVector(bitsCount, fillPattern, SizeProperty::Fixed);
  }

  auto holdValue    = false;
  auto registerNode = m_systemModel->CreateRegister(name, bypassValue, holdValue);

  scanRegister->AssociatedRegister(registerNode);

  return make_tuple(registerNode, registerSource);
}
//
//  End of: AST_SystemModelGenerator::Generate_ScanRegister
//---------------------------------------------------------------------------



//! Creates selection/deselection tables for ScanMux (for table based PathSelector)
//!
//! @param selections         ScanMux selection values
//! @param expectedBitsCount  Bits count of selector register(s) - this is used only for value width check
//!
AST_SystemModelGenerator::SelectionTables_t AST_SystemModelGenerator::MakeSelectionTable (const vector<AST_ScanMuxSelection*>& selections, size_t expectedBitsCount) const
{
  vector<BinaryVector> selectTable;
  selectTable.emplace_back(expectedBitsCount); // Dummy entry for not used path identifier zero

  for (const auto& selection : selections)
  {
    const auto& values = selection->SelectionsValues();
    CHECK_VALUE_NOT_EMPTY(values, "Must have at least one value");
    CHECK_VALUE_EQ(values.size(), 1u, "Not Yet Supported: Concat number list (for ScanMux Selection)");

    const auto& value               = values.front();
    auto        valueAsBinaryVector = BinaryVector::CreateFromString(value);
    CHECK_PARAMETER_EQ(valueAsBinaryVector.BitsCount(), expectedBitsCount, "Unexpected selection bits count");
    selectTable.emplace_back(std::move(valueAsBinaryVector));
  }

  //! @todo [JFC]-[November/17/2017]: Support Concat number list in AST_SystemModelGenerator::MakeSelectionTable()

  vector<BinaryVector> deselectTable = selectTable;
  for (auto& deselectValue : deselectTable)
  {
    deselectValue.ToggleBits();
  }

  return make_tuple(selectTable, deselectTable);
}
//
//  End of: AST_SystemModelGenerator::MakeSelectionTable
//---------------------------------------------------------------------------





//===========================================================================
// End of AST_SystemModelGenerator.cpp
//===========================================================================
