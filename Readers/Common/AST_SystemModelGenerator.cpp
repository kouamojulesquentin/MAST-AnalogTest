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
using std::reference_wrapper;
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



//! Appends created children to a parent node
//!
//! @param parent           Parent to assign childre to
//! @param levelThreshold   Minimal count of created children to leave unappended
//!
void AST_SystemModelGenerator::AppendCreatedNodesToParent (ParentNode* parent, size_t levelThreshold)
{
  // ---------------- Append m_createdNodes to Chain
  //
  while (m_createdNodes.size() > levelThreshold)
  {
    auto child = m_createdNodes.top();

    parent->AppendChild(child);
    m_createdNodes.pop();
  }
}
//
//  End of: AST_SystemModelGenerator::AppendCreatedNodesToParent
//---------------------------------------------------------------------------


//! Creates linker path selector
//!
//! @param scanMux                ScanMux representing the linker
//! @param module                 Module the in which the ScanMux is defined
//! @param firstSelectionIsEmpty  When true, first mux selection is ignored in selection/deselection tables (and the linker must be set with can_select_none = true)
//!
shared_ptr<PathSelector> AST_SystemModelGenerator::Create_PathSelector (AST_ScanMux* scanMux, AST_Module* module, bool firstSelectionIsEmpty)
{
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
  std::tie(selectTable, deselectTable) = MakeSelectionTable(selections, selectorsBitsCount, firstSelectionIsEmpty);

  // ---------------- Prepare path selector
  //
  auto selectorProperties = firstSelectionIsEmpty ? SelectorProperty::CanSelectNone
                                                  : SelectorProperty::Std;

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

  return pathSelector;
}
//
//  End of: AST_SystemModelGenerator::Create_PathSelector
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



//! Follows top module source signal path
//!
//! @param module       Top module
//! @param scanOutPort  Top module output port to follow backward to create SystemModelNode
//!
void AST_SystemModelGenerator::FollowTopModulePath (AST_Module* topModule, const AST_Port* scanOutPort)
{
  auto      module                    = topModule;
  auto      source                    = scanOutPort->Source();
  auto      sourceSignals             = std::cref(source->Signals());
  auto      isSourcedByTopModuleInput = false;
  AST_Port* scanInPort                = nullptr;

  std::tie(isSourcedByTopModuleInput, scanInPort) = IsSourcedByModuleInput(topModule, sourceSignals);

  while (!isSourcedByTopModuleInput || !m_linkersContext.empty())
  {
    if (isSourcedByTopModuleInput)  // ==> !m_linkersContext.empty()
    {
      CHECK_VALUE_EQ(m_instancesContext.size(), 1u, "When reaching top module there should be only 1 instance context left, got "s.append(std::to_string(m_instancesContext.size())));

      std::tie(module, sourceSignals) = Process_ScanMux_Selection();
    }   // End of: if (isSourcedByTopModuleInput)
    else   // ==> !isSourcedByTopModuleInput
    {
      CHECK_VALUE_EQ(sourceSignals.get().size(), 1u, "Expecting source to be driven by exactly one signal");

      auto        isSourcedByModuleInput = false;
      AST_Port*   scanInPort             = nullptr;

      std::tie(isSourcedByModuleInput, scanInPort) = IsSourcedByModuleInput(module, sourceSignals);

      if (isSourcedByModuleInput)  // ==> Must move up the module hierarchy
      {
        std::tie(module, sourceSignals) = Process_Instance_Exit(scanInPort);
      }
      else   // ==> Look connection inside current module
      {
        const auto signal      = sourceSignals.get().front();
        const auto portScope   = signal->PortScope();
        const auto identifier  = signal->PortName();

        if (portScope.empty())  // ScanRegister or ScanMux ?
        {
          auto scanRegister = module->FindScanRegister(identifier);
          if (scanRegister != nullptr)
          {
            if (scanRegister->HasAssociatedRegister())       // Have we already gone to this path point ?
            {
              //! @todo [JFC]-[November/29/2017]: In FollowTopModulePath(): Deal with Linker "common point"
              //!
              CHECK_FAILED("Not Yet Supported: Linker \"common point\"");
              return; // ==> Stop before reaching module input port
            }

            sourceSignals = Process_ScanRegister(scanRegister);
          }
          else  // ScanMux
          {
            auto scanMux  = module->FindScanMux(identifier);
            CHECK_VALUE_NOT_NULL(scanMux, "Failed to find source entity (not a ScanMux)");
            sourceSignals = Process_ScanMux_Entry(scanMux, module);
          }
        }
        else   // Instance ?
        {
          CHECK_VALUE_EQ(portScope.size(), 1u, "Expecting to have single scope depth for instance");

          auto scope            = portScope.front();
          auto instance         = module->FindInstance(scope);
          CHECK_VALUE_NOT_NULL(instance, "Failed to find source entity (not an Instance)");

          auto moduleIdentifier = instance->ModuleIdentifier();
          auto instanceModule   = m_network->Module(moduleIdentifier);
          auto scanOutPort      = instanceModule->FindScanOutPort(identifier);

          sourceSignals = Process_Instance_Entry(instance, instanceModule, scanOutPort);
          module        = instanceModule;
        }

        std::tie(isSourcedByModuleInput, scanInPort) = IsSourcedByModuleInput(module, sourceSignals);
        if (isSourcedByModuleInput && (m_instancesContext.size() > 1u))
        {
          std::tie(module, sourceSignals) = Process_Instance_Exit(scanInPort);
        }
      } // End of: !isSourcedByModuleInput
    } // End of: !isSourcedByTopModuleInput

    isSourcedByTopModuleInput = false;
    if (module == m_network->TopModule())
    {
      std::tie(isSourcedByTopModuleInput, scanInPort) = IsSourcedByModuleInput(topModule, sourceSignals);
    }
  }
}
//
//  End of: AST_SystemModelGenerator::FollowTopModulePath
//---------------------------------------------------------------------------


//! Generates a SystemModel (sub-)tree from AST_Network
//!
//! @param network  Scan network to convert to MAST SystemModel
//!
shared_ptr<mast::ParentNode> AST_SystemModelGenerator::Generate (AST_Network* network)
{
  m_network = CHECK_PARAMETER_NOT_NULL(network, "Expect valid, not nullptr AST_Network");

  m_parsedTopNode = Generate_Network(network);

  return m_parsedTopNode;
}
//
//  End of: AST_SystemModelGenerator::Generate
//---------------------------------------------------------------------------




//! Creates complete SystemModel from AST_Network
//!
//! @param network  Network description
//!
//! @return Created SystemModel sub-tree
//!
shared_ptr<mast::ParentNode> AST_SystemModelGenerator::Generate_Network (AST_Network* network)
{
  auto topModule = network->TopModule();
  CHECK_VALUE_NOT_NULL(topModule, "Cannot generate SystemModel nodes when network has no modules");

  shared_ptr<ParentNode> topNode;

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

    Generate_TopModule(chain.get(), topModule);

    topNode = chain;
  }

  //! @todo [JFC]-[November/23/2017]: In Generate(): Resolve linkers
  //!
  CHECK_VALUE_EMPTY(m_unresolvedPathSelectors, "Not Yet Supported: Unresolved path selector");
//+  for (const auto& linker : m_unresolvedPathSelectors)
//+  {
//+  }

  return topNode;
}
//
//  End of: AST_SystemModelGenerator::Generate_Network
//---------------------------------------------------------------------------



//! Creates a SystemModel nodes for network top module
//!
//! @param chain      Chain in which SystemModel nodes are created
//! @param topModule  Network top module to be converted to SystemModel nodes
//!
void AST_SystemModelGenerator::Generate_TopModule (mast::Chain* chain, AST_Module* topModule)
{
  const auto& scanInPorts  = topModule->ScanInPorts();
  const auto& scanOutPorts = topModule->ScanOutPorts();

  CHECK_VALUE_NOT_EMPTY(scanInPorts,  "Expecting a top module to have at least one ScanInPort");
  CHECK_VALUE_NOT_EMPTY(scanOutPorts, "Expecting a top module to have at least one ScanOutPort");

  InstanceContext context;

  context.instance          = nullptr;    // Instance is implicit
  context.parentModule      = topModule;
  context.parentNode        = chain;
  context.createdNodesLevel = 0;

  m_instancesContext.push(context); // Context is popped when path reaches instance input

  for (const auto& scanOutPort : scanOutPorts)
  {
    CHECK_VALUE_EMPTY(m_linkersContext, "No linker should be processed when dealing with top node ScanOutPort");

    FollowTopModulePath(topModule, scanOutPort);
    AppendCreatedNodesToParent(chain, 0u);
  }
}
//
//  End of: AST_SystemModelGenerator::Generate_TopModule
//---------------------------------------------------------------------------



//! Tells whether some signals are connected to one of module ScanInPort
//!
//! @param module   Module in which signals are defined
//! @param signals  Signals to test for connection to module ScanInPort (only one is supported)
//!
tuple<bool, AST_Port*> AST_SystemModelGenerator::IsSourcedByModuleInput (const AST_Module* module, const vector<AST_Signal*>& signals) const
{
  CHECK_VALUE_EQ(signals.size(), 1u, "Expecting source to be driven by exactly one signal, got "s.append(std::to_string(signals.size())));

  const auto  signal            = signals.front();
  const auto& moduleScanInPorts = module->ScanInPorts();
  const auto  portScope         = signal->PortScope();
  const auto  identifier        = signal->PortName();

  if (!portScope.empty())
  {
    return make_tuple(false, nullptr);
  }

  auto pos = std::find_if(moduleScanInPorts.cbegin(),
                          moduleScanInPorts.cend(),
                          [name = identifier->Name()](auto item) { return item->Name() == name; });

  bool isSourcedByModuleInput = pos != moduleScanInPorts.cend();

  return isSourcedByModuleInput ? make_tuple(true,  *pos)
                                : make_tuple(false, nullptr);
}
//
//  End of: AST_SystemModelGenerator::IsSourcedByModuleInput
//---------------------------------------------------------------------------




//! Process "entering" in a module instance
//!
//! @param instance         Instance to process
//! @param instanceModule   Module that defines the instance
//! @param scanOutPort      Port by which scan path is followed (backward)
//!
//! @return Signals to follow to reach previous entity in test network
//!
AST_SystemModelGenerator::SourceSignalsRef_t
AST_SystemModelGenerator::Process_Instance_Entry (AST_Instance* instance, AST_Module* instanceModule, const AST_Port* scanOutPort)
{
  const auto& moduleInputPorts = instanceModule->ScanInPorts();
  CHECK_VALUE_NOT_EMPTY(moduleInputPorts,  "Expecting an instance module \""s.append(instanceModule->Name()).append("\"to have at least one ScanInPort"));

  InstanceContext context;

  context.instance     = instance;
  context.parentModule = instanceModule;
  context.parentNode   = m_instancesContext.top().parentNode; // By default, there is no change in parent node (for case when no Chain is created)

  // ---------------- Create a Chain as necessary
  //
  auto chain = instance->AssociatedChain();
  if (!chain)
  {
    CHECK_VALUE_EMPTY(instance->Parameters(), "Not Yet Supported: Instance parameters");

    // ---------------- Create Chain to "encapsulate" instance sub-nodes
    //
    auto name = instance->Name();
    chain     = m_systemModel->CreateChain(name);

    instance->AssociatedChain(chain);   // To detect already created Chain for the instance
    m_createdNodes.push(chain);
    context.parentNode = chain.get();
  }

  context.createdNodesLevel = m_createdNodes.size();

  m_instancesContext.push(context);

  // ---------------- Return signal to follow to reach first instance entity
  //
  auto source        = scanOutPort->Source();
  auto sourceSignals = std::cref(source->Signals());

  return sourceSignals;
}
//
//  End of: AST_SystemModelGenerator::Process_Instance
//---------------------------------------------------------------------------



//! Processes the fact that we reach a ScanInPort of an instance/module.
//! This include appending created children while processing this instance (when possible)
//!
//! @param scanInPort   Port by which the instance input was reached
//!
//! @return New processing context
AST_SystemModelGenerator::ProcessingContext_t AST_SystemModelGenerator::Process_Instance_Exit (AST_Port* scanInPort)
{
  CHECK_VALUE_NOT_EMPTY(m_instancesContext, "Houps not balanced push/pop on instance context");

  auto&       context           = m_instancesContext.top();
  auto        parentNode        = context.parentNode;
  auto        parentIsLinker    = context.parentIsLinker;
  auto        instance          = context.instance;
  auto        portIdentifier    = scanInPort->Identifier();
  auto        instanceInputPort = instance->FindInputPort(portIdentifier);
  auto        instanceSource    = instanceInputPort->Source();
  const auto& sourceSignals     = instanceSource->Signals();

  if (parentNode && !parentIsLinker)
  {
    AppendCreatedNodesToParent(parentNode, context.createdNodesLevel);
  }

  // ---------------- Restore previous context
  //
  m_instancesContext.pop();
  CHECK_VALUE_NOT_EMPTY(m_instancesContext, "Must have at least an (implicit) instance context for top module");

  auto& parentContext = m_instancesContext.top();
  auto  module        = parentContext.parentModule;
  if (!parentIsLinker)
  {
    parentNode = parentContext.parentNode;
  }

  return make_tuple(module, cref(sourceSignals));
}
//
//  End of: AST_SystemModelGenerator::Process_InstanceExit
//---------------------------------------------------------------------------





//! Creates a SystemModel Linker from an AST_ScanMux (with temporary path selector)
//!
//! @param scanMux  ScanMux to be converted to SystemModel Linker
//! @param module   ScanMux parent module
//!
//! @return ScanMux source signals for 1st selection
//!
AST_SystemModelGenerator::SourceSignalsRef_t
AST_SystemModelGenerator::Process_ScanMux_Entry (AST_ScanMux* scanMux, AST_Module* module)
{
  CHECK_FALSE(scanMux->IsBusMux(), "Not Yet Supported: ScanMux for buses");

  // ---------------- Create Linker
  //
  auto        unresolvedPathSelector = make_shared<UnresolvedPathSelector>();
  const auto& name                   = scanMux->BaseName();
  auto        linker                 = m_systemModel->CreateLinker(name, unresolvedPathSelector);

  m_createdNodes.push(linker);

  auto& instanceContext = m_instancesContext.top();

  LinkerContext linkerContext;

  linkerContext.instanceContext      = instanceContext;
  linkerContext.processedScanMux     = scanMux;
  linkerContext.processedSelectionId = 0;
  linkerContext.linkerNodesLevel     = m_createdNodes.size();
  linkerContext.linker               = linker.get();

  m_linkersContext.push(linkerContext);

  instanceContext.parentIsLinker = true;

  const auto& sourceSignals = scanMux->Selections().front()->SelectedSignals();
  return std::cref(sourceSignals);
}
//
//  End of: AST_SystemModelGenerator::Process_ScanMux_Entry
//---------------------------------------------------------------------------


//! Does what is needed to process when we reached end of ScanMux selection
//!
//! @return New processing context
//!
AST_SystemModelGenerator::ProcessingContext_t AST_SystemModelGenerator::Process_ScanMux_Selection ()
{
  auto&       linkerContext        = m_linkersContext.top();
  auto        linker               = linkerContext.linker;
  auto        scanMux              = linkerContext.processedScanMux;
  auto        createdNodesLevel    = linkerContext.linkerNodesLevel;
  auto        processedSelectionId = linkerContext.processedSelectionId;
  auto const& selections           = scanMux->Selections();
  auto        module               = linkerContext.instanceContext.parentModule;

  CHECK_VALUE_GTE(m_createdNodes.size(), createdNodesLevel, "Have appended more nodes than expected");
  auto newCreated = m_createdNodes.size() - createdNodesLevel;

  if (processedSelectionId == 0)
  {
    auto firstSelectionIsEmpty = newCreated == 0;
    auto pathSelector          = Create_PathSelector(scanMux, module, firstSelectionIsEmpty);
    linker->ReplacePathSelector(pathSelector);
  }

  if (newCreated > 1u)
  {
    auto name  = linker->Name();
    name.append("_").append(std::to_string(processedSelectionId));
    auto chain = m_systemModel->CreateChain(name);
    chain->IgnoreForNodePath(true);
    linker->AppendChild(chain);
    AppendCreatedNodesToParent(chain.get(), createdNodesLevel);
  }
  else if (newCreated != 0)
  {
    AppendCreatedNodesToParent(linker, createdNodesLevel);
  }

  ++processedSelectionId;

  auto        sourceSignals = std::cref(selections[processedSelectionId]->SelectedSignals());
  ParentNode* parentNode    = linker;

  if (module != m_network->TopModule())
  {
    auto context = linkerContext.instanceContext;
    context.parentIsLinker = true;
    m_instancesContext.push(context);
  }


  bool lastLinkerSelection = processedSelectionId >= selections.size();
  if (lastLinkerSelection)
  {
    m_linkersContext.pop();
    m_instancesContext.top().parentNode = linkerContext.instanceContext.parentNode;

    module        = m_instancesContext.top().parentModule;
    parentNode    = m_instancesContext.top().parentNode;
    sourceSignals = std::cref(selections.front()->SelectedSignals()); // Will cause detection of instance input port or previously created node that is just before the linker

    AppendCreatedNodesToParent(parentNode, m_instancesContext.top().createdNodesLevel);
  }
  else
  {
    ++linkerContext.processedSelectionId;
    sourceSignals = std::cref(selections[processedSelectionId]->SelectedSignals());
  }
  return make_tuple(module, cref(sourceSignals));
}
//
//  End of: AST_SystemModelGenerator::Process_ScanMux_Selection
//---------------------------------------------------------------------------


//! Creates a SystemModel Register from an AST_ScanRegister
//!
//! @param scanRegister   ScanRegister to be converted to SystemModel Register
//!
//! @return Signals to follow to reach previous entity in test network (moving backward)
//!
AST_SystemModelGenerator::SourceSignalsRef_t
AST_SystemModelGenerator::Process_ScanRegister (AST_ScanRegister* scanRegister)
{
  auto source     = scanRegister->ScanInSource();
  auto name       = scanRegister->BaseName();
  auto bitsCount  = scanRegister->BitsCount();
  auto resetValue = scanRegister->ResetValue();

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

  m_createdNodes.push(registerNode);

  return std::cref(source->Signals());
}
//
//  End of: AST_SystemModelGenerator::Process_ScanRegister
//---------------------------------------------------------------------------


//! Creates selection/deselection tables for ScanMux (for table based PathSelector)
//!
//! @param selections             ScanMux selection values
//! @param expectedBitsCount      Bits count of selector register(s) - this is used only for value width check
//! @param firstSelectionIsEmpty  When true, first mux selection is ignored in selection/deselection tables (and the linker must be set with can_select_none = true)
//!
AST_SystemModelGenerator::SelectionTables_t AST_SystemModelGenerator::MakeSelectionTable(const vector<AST_ScanMuxSelection*>& selections,
                                                                                         size_t                               expectedBitsCount,
                                                                                         bool                                 firstSelectionIsEmpty) const
{
  vector<BinaryVector> selectTable;
  selectTable.emplace_back(expectedBitsCount); // Dummy entry for not used path identifier zero

  auto skip = firstSelectionIsEmpty;

  for (const auto& selection : selections)
  {
    if (skip)
    {
      skip = false;
      continue;
    }

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
