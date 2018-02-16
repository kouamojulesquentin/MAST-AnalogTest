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
#include "AST_Alias.hpp"
#include "AST_Attribute.hpp"
#include "AST_BsdlInstructionRef.hpp"
#include "AST_ConcatNumber.hpp"
#include "AST_FileRef.hpp"
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
#include "AST_ScanInterfaceRef.hpp"
#include "AST_ScanMux.hpp"
#include "AST_ScanMuxSelection.hpp"
#include "AST_ScanRegister.hpp"
#include "AST_Signal.hpp"
#include "AST_Source.hpp"
#include "AST_String.hpp"
#include "AST_VectorIdentifier.hpp"

#include "AccessInterfaceProtocol.hpp"
#include "AccessInterfaceProtocolFactory.hpp"
#include "DefaultTableBasedPathSelector.hpp"
#include "UnresolvedPathSelector.hpp"
#include "RegistersAlias.hpp"
#include "SystemModel.hpp"
#include "SystemModelBuilder.hpp"
#include "BSDL_Reader.hpp"
#include "Utility.hpp"
#include "MastConfig.hpp"
#include "g3log/g3log.hpp"

#include <tuple>
#include <functional>
#include <experimental/string_view>

using std::stack;
using std::queue;
using std::vector;
using std::shared_ptr;
using std::unique_ptr;
using std::tuple;
using std::reference_wrapper;
using std::string;
using std::experimental::string_view;
using std::make_unique;
using std::make_shared;
using std::make_tuple;

using namespace mast;
using namespace Parsers;

const std::vector<AST_Signal*> AST_SystemModelGenerator::sm_noSignals;

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
  while (m_createdNodes.size() > levelThreshold)
  {
    auto child = std::get<0>(m_createdNodes.top());

    parent->AppendChild(child);
    m_createdNodes.pop();
  }
}
//
//  End of: AST_SystemModelGenerator::AppendCreatedNodesToParent
//---------------------------------------------------------------------------



//! Assigns newly created node to its parent unless it is a linker or representing top module.
//! In the latter case, it is pushed into a stack of not assigned yet nodes.
//!
//! @param node A newly created node
//!
void AST_SystemModelGenerator::AssignNewNode (shared_ptr<SystemModelNode> node)
{
  auto& context           = m_instancesContext.top();
  auto  parentNode        = context.parentNode;
  auto  linkerNode        = context.linkerNode;
  auto  parentMayBeLinker = linkerNode != nullptr;

  auto contextIsTopModule = m_instancesContext.size() == 1u;

  if (parentMayBeLinker || contextIsTopModule)
  {
    m_createdNodes.emplace(node, parentNode);
  }
  else
  {
    parentNode->PrependChild(node);
  }
}
//
//  End of: AST_SystemModelGenerator::AssignNewNode
//---------------------------------------------------------------------------


//! Appends not yet assigned node to Linker first selection, provided there is some
//! to assign to Linker selection
//!
//! @internal
//! @note   Supposing that assigned node stack is filled up like (last one is nearest from scan input):
//!           [1] [L] [3] [4] [C] [6] [7] [8]
//!                ^
//!         where:
//!           - brackets pair represents a node,
//!           - [L] is the node for the Linker
//!           - [C] is the node (most often a Chain) that was reached following all Linker selection paths
//!           - For SIB, [L] & [C] are next to each other ==> First Linker selection is empty ==> [C] and the following are sibling of the Linker
//!
//!         The algorithm purpose is to extract the nodes between [L] and [C]:
//!           1 - extracts 2nd part into a temporary stack          ==> [8] [7] [6] [C]
//!           2 - extracts 1st part into another temporary stack    ==> [4] [3]
//!           3 - Append 1st part Linker 1st selection              ==> [4] [3] - note that they are assign in that order - (inserting a Chain in between in that case)
//!           4 - Restore 2nd part to not yet assigned nodes stack  ==> [1] [L] [C] [6] [7] [8]
//!               or assign previous siblings to common, parent     ==> e.g. Parent children: [8]->[7]->[6]->[C]
//!               ==> Candidates must be reversed before being able to "splice" them
//!               ==> They must also been inserted BEFORE nodes assigned to parent when traversing connectivity backward
//!               ==> They must also been inserted AFTER nodes assigned when dealing with Linker 1st selection (traversing connectivity forward)
//! @endinternal
//!
//! @param commonLinkerNode   The first SystemModelNode encounter following all linker selections path
//!
//! @return True when first Linker selection is empty, false otherwise
bool AST_SystemModelGenerator::AssignNodesToLinkerFirstSelection (shared_ptr<SystemModelNode> commonLinkerNode)
{
  auto firstSelectionIsEmpty = true;

  auto& linkerContext          = m_linkersContext.top();
  auto  linker                 = linkerContext.linker;
  auto  linkerNodesLevel_first = linkerContext.linkerNodesLevel_first;

  CHECK_VALUE_GTE(m_createdNodes.size(), linkerNodesLevel_first, "Have appended more nodes than expected (for 1st Linker selection)");
  auto createdForFirst = m_createdNodes.size() - linkerNodesLevel_first;

  // Adjust real child count for 1st selection (dealing with common node on paths)
  //
  if (createdForFirst != 0)
  {
    // 2nd part extraction
    stack<CreatedNodes_t>  nodesAsLinkerSiblings;
    while (m_createdNodes.size() > linkerNodesLevel_first)
    {
      auto nodeAndParent = m_createdNodes.top();
      nodesAsLinkerSiblings.push(nodeAndParent);
      m_createdNodes.pop();

      auto node = std::get<0>(nodeAndParent);
      if (node == commonLinkerNode)
      {
        break;
      }
    }

    // 1st part extraction
    stack<shared_ptr<SystemModelNode>>  nodesForFirstSelection;
    while (m_createdNodes.size() > linkerNodesLevel_first)
    {
      auto node = std::get<0>(m_createdNodes.top());  // Get rid of "alternate" parent node (this is in fact resolved as the Linker)
      nodesForFirstSelection.push(node);
      m_createdNodes.pop();
    }

    // Append 1st part Linker 1st selection (inserting a Chain when there is more than a single node)
    createdForFirst = nodesForFirstSelection.size();
    if (createdForFirst != 0)
    {
      firstSelectionIsEmpty = false;
      if (createdForFirst == 1)
      {
        linker->PrependChild(nodesForFirstSelection.top());
      }
      else
      {
        auto chain = Create_ChainForLinker(linker, 0);
        linker->PrependChild(chain);
        while (!nodesForFirstSelection.empty())
        {
          chain->PrependChild(nodesForFirstSelection.top());
          nodesForFirstSelection.pop();
        }
      }
    }

    // Restore 2nd part to not yet assigned nodes stack
    // or prepend or save for splicing them as linker, previous, sibblings
    stack<CreatedNodes_t>  nodesToSpliceAsLinkerPreviousSiblings;
    auto linkerParentNode = linkerContext.linkerParentNode;
    while (!nodesAsLinkerSiblings.empty())
    {
      auto& nodeAndParent = nodesAsLinkerSiblings.top();

      auto node       = std::get<0>(nodeAndParent);
      auto parentNode = std::get<1>(nodeAndParent);
      CHECK_VALUE_NOT_NULL(parentNode, "Parent node must always be set in m_createdNodes stack");

      if (parentNode == linkerParentNode)  // Linker sibling ?
      {
        auto hasAlreadySplicePoint = m_parentsSplicePoint.find(parentNode) != m_parentsSplicePoint.end();
        if (hasAlreadySplicePoint)
        {
          nodesToSpliceAsLinkerPreviousSiblings.push(nodeAndParent);
        }
        else
        {
          parentNode->PrependChild(node);                        // Create splice point (can only splice after some node)
          m_parentsSplicePoint.insert({parentNode, node.get()}); // Save splice point
        }
      }
      else
      {
        m_createdNodes.push(nodeAndParent);
      }
      nodesAsLinkerSiblings.pop();
    }

    // Splice, remaining, linker siblings
    while (!nodesToSpliceAsLinkerPreviousSiblings.empty())
    {
      auto& nodeAndParent = nodesToSpliceAsLinkerPreviousSiblings.top();

      auto node       = std::get<0>(nodeAndParent);
      auto parentNode = std::get<1>(nodeAndParent);

      auto spliceNode = m_parentsSplicePoint[parentNode];
      CHECK_PARAMETER_NOT_NULL(spliceNode, "Houps, there should be a valid splice point node");

      spliceNode->SpliceSibling(node);
      nodesToSpliceAsLinkerPreviousSiblings.pop();
    }
  }

  return firstSelectionIsEmpty;
}
//
//  End of: AST_SystemModelGenerator::AssignNodesToLinkerFirstSelection
//---------------------------------------------------------------------------



//! Converts register aliases defined in instance module to VirtualRegisters
//!
//! @param module       Module for which aliases will be converted
//! @param parentNode   SystemModelNode associated with processed module
//!
void AST_SystemModelGenerator::ConvertAliases (AST_Module* module, ParentNode* parentNode)
{
  // Lamba: Follows a signal until it reaches a ScanRegister
  auto followSignalTilScanRegister = [](AST_Module* module, AST_Signal* signal)
  {
    if (signal->IsNumber())
    {
      return static_cast<AST_ScanRegister*>(nullptr);
    }

    const auto portScope  = signal->PortScope();
    const auto identifier = signal->PortName();
    auto       regModule  = module;

    if (!portScope.empty())   // ==> Represents a register in sub-instance ?
    {
      auto instanceModule = module;
      for (const auto scopeIdentifier : portScope)
      {
        auto foundInstance = instanceModule->FindInstance(scopeIdentifier);
        if (foundInstance == nullptr)
        {
          break;
        }
        instanceModule = foundInstance->UniquifiedModule();
      }
      regModule = instanceModule;
    }

    auto scanRegister = regModule->FindScanRegister(identifier);
    return scanRegister;
  };

  // Lamba: Creates a register slice for ScanRegister and signal
  //
  auto makeRegisterSlice = [](AST_ScanRegister* scanRegister, AST_Signal* signal)
  {
    auto reg = scanRegister->AssociatedRegister();

    CHECK_VALUE_NOT_NULL(reg, "Houps: While converting alias, detected a ScanRegister that has not been converted to SystemModel Register");

    uint32_t leftIndex  = reg->BitsCount() - 1u;
    uint32_t rightIndex = 0u;

    const auto identifier = signal->PortName();
    if (!identifier->IsScalar())
    {
      leftIndex  = identifier->LeftIndex();
      rightIndex = identifier->IsMultiBits() ? identifier->RightIndex() : leftIndex;
    }

    RegisterSlice regSlice{reg, IndexedRange{leftIndex, rightIndex}};
    return regSlice;
  };

  // ---------------- Local aliases
  //
  if (parentNode != nullptr)
  {
    const auto& aliases = module->Aliases();
    for (auto alias : aliases)
    {
      VirtualRegister virtualReg;

      const auto& signals = alias->Signals();
      for (auto signal : signals)
      {
        auto scanRegister = followSignalTilScanRegister(module, signal);
        if (scanRegister == nullptr)
        {
          LOG(WARNING) << "Alias \"" << alias->Name() << "\" in module \"" << module->Name() << "\" does not lead to a scan register ==> This is not yet supported";
        }
        else
        {
          virtualReg.Append(makeRegisterSlice(scanRegister, signal));
        }
      }

      if (!virtualReg.Empty())
      {
        RegistersAlias regAlias(string(alias->Identifier()->BaseName()), std::move(virtualReg));
        parentNode->AddAlias(std::move(regAlias));
      }
    }
  }

  // ---------------- Instances aliases
  //
  // Lamba: Follows a signal until it reaches a ScanRegister
  //
  // @param module  Module from which signal to follow is defined
  // @param signal  Signal to follow
  //
  // @return  Found ScanRegister in case of success, nullptr otherwise
  //
  std::function<AST_ScanRegister*(AST_Module* module, AST_Signal* signal)>
  followSignalTilScanRegister_2 = [&followSignalTilScanRegister_2](AST_Module* module, AST_Signal* signal)
  {
    if (signal->IsNumber())
    {
      return static_cast<AST_ScanRegister*>(nullptr);
    }

    const auto portScope     = signal->PortScope();
    const auto identifier    = signal->PortName();
    auto       currentModule = module;

    if (!portScope.empty())   // ==> Represents a register in sub-instance ?
    {
      auto instanceModule = module;
      for (const auto scopeIdentifier : portScope)
      {
        auto foundInstance = instanceModule->FindInstance(scopeIdentifier);
        if (foundInstance == nullptr)
        {
          break;
        }
        instanceModule = foundInstance->UniquifiedModule();
      }
      currentModule = instanceModule;
    }

    auto scanRegister = currentModule->FindScanRegister(identifier);
    if (scanRegister != nullptr)
    {
      return scanRegister;
    }

    auto dataInPort = currentModule->FindDataInPort(identifier);
    if (dataInPort != nullptr)
    {
      auto instanceOfModule  = currentModule->FromInstance();
      if (instanceOfModule == nullptr)
      {
        LOG(WARNING) << "While processing alias, cannot follow I/O ports from top module";
        return static_cast<AST_ScanRegister*>(nullptr);
      }
      auto instanceInputPort = instanceOfModule->FindInputPort(identifier);
      if (instanceInputPort == nullptr)
      {
        LOG(WARNING) << "Failed to find input port \"" << identifier->AsText() <<  "\" for instance \"" << instanceOfModule->Name() << "\" ==> alias will be ignored";
        return static_cast<AST_ScanRegister*>(nullptr);
      }

      auto source = instanceInputPort->Source();
      CHECK_VALUE_NOT_NULL(source, "An input port must have a valid source");
      const auto& sourceSignals = source->Signals();
      CHECK_VALUE_EQ(sourceSignals.size(), 1u, "While processing alias, can only deal with scalar signals");

      auto parentModule = module->ParentModule();
      CHECK_PARAMETER_NOT_NULL(parentModule, "Houps: After unification, all module (except top one) must have a \"parent\" module");

      return followSignalTilScanRegister_2(parentModule, sourceSignals.front());
    }

    auto dataOutPort = currentModule->FindDataOutPort(identifier);
    if (dataOutPort != nullptr)
    {
      auto source = dataOutPort->Source();
      if (source != nullptr)
      {
        const auto& sourceSignals = source->Signals();
        CHECK_VALUE_EQ(sourceSignals.size(), 1u, "While processing alias, can only deal with scalar signals");

        return followSignalTilScanRegister_2(currentModule, sourceSignals.front());
      }
      else
      {
//+          CHECK_VALUE_NOT_NULL(source, "Instance \"" + instance->Name() + "\" output port \""s + dataOutPort->Name() + "\" has no source");
        return static_cast<AST_ScanRegister*>(nullptr);
      }
    }

    return static_cast<AST_ScanRegister*>(nullptr);
  };

  const auto& instances = module->Instances();
  for (auto instance : instances)
  {
    auto associatedNode = instance->AssociatedChain();
    auto instanceModule = instance->UniquifiedModule();

    CHECK_VALUE_NOT_NULL(instanceModule, "Houps: Instance is not associated with an uniquified module");

    if (associatedNode != nullptr)
    {
      ConvertAliases(instanceModule, associatedNode.get());
    }
    else if (instanceModule->HasAliases())
    {
      LOG(INFO) << "Trying to pull up aliases defined in instance \"" << instance->Name() << "\" into its parent module \"" << module->Name() << "\"" ;
      const auto& aliases = instanceModule->Aliases();
      for (const auto& alias : aliases)
      {
        VirtualRegister virtualReg;

        const auto& signals = alias->Signals();
        for (auto signal : signals)
        {
          auto scanRegister = followSignalTilScanRegister_2(instanceModule, signal);
          if (scanRegister != nullptr)
          {
            virtualReg.Append(makeRegisterSlice(scanRegister, signal));
          }
        }

        if (!virtualReg.Empty())
        {
          RegistersAlias regAlias(string(alias->Identifier()->BaseName()), std::move(virtualReg));
          parentNode->AddAlias(std::move(regAlias));
        }
      }
    }
  }
}
//
//  End of: AST_SystemModelGenerator::ConvertAliases
//---------------------------------------------------------------------------




//! Creates a "generated" chain to force a single child for Linker selection
//!
//! @param linker         Linker to create a chain for
//! @param selectionId    Selection id for which the Chain is created
//!
shared_ptr<mast::Chain> AST_SystemModelGenerator::Create_ChainForLinker (const Linker* linker, size_t selectionId)
{
  auto name  = linker->Name();
  name.append("_").append(std::to_string(selectionId));
  auto chain = m_systemModel->CreateChain(name);
  chain->IgnoreForNodePath(true);
  return chain;
}
//
//  End of: AST_SystemModelGenerator::Create_ChainForLinker
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


  VirtualRegister virtualRegister;

  for (const auto& selectorItem : selectorRegisters)
  {
    auto selectorRegister = std::get<0>(selectorItem);
    auto hasRange         = std::get<1>(selectorItem);
    auto leftIndex        = std::get<2>(selectorItem);
    auto rightIndex       = std::get<3>(selectorItem);

    auto modelRegister    = selectorRegister->AssociatedRegister();

    CHECK_VALUE_NOT_NULL(modelRegister, "Not Yet Supported: Unresolved path selector (when mux selector ScanRegister is not yet transformed to Register)");

    modelRegister->SetHoldValue(true);

    auto range = hasRange ? IndexedRange{leftIndex, rightIndex}
                          : IndexedRange{modelRegister->BitsCount() - 1u, 0u};

    RegisterSlice registerSlice{modelRegister, range};

    virtualRegister.Append(registerSlice);
  }

  auto pathsCount   = selectTable.size() - 1u;
  auto pathSelector = make_shared<DefaultTableBasedPathSelector>(virtualRegister,
                                                                 pathsCount,
                                                                 std::move(selectTable),
                                                                 std::move(deselectTable),
                                                                 selectorProperties);

  return pathSelector;
}
//
//  End of: AST_SystemModelGenerator::Create_PathSelector
//---------------------------------------------------------------------------



//! Creates a AccessInterfaceProtocol for top module TAP
//!
//! @param topModule  Network top module with JTAG TAP kind AccessLink
//!
std::unique_ptr<AccessInterfaceProtocol> AST_SystemModelGenerator::Create_Protocol (AST_Module* topModule)
{
  if (m_protocolName.empty())
  {
    const auto protocolNameAttribute       = topModule->Attribute("ACCESS_LINK_PROTOCOL_NAME");
    const auto protocolParametersAttribute = topModule->Attribute("ACCESS_LINK_PROTOCOL_PARAMETERS");

    CHECK_PARAMETER_NOT_NULL(protocolNameAttribute, "Missing protocol name for tap: \""s.append(topModule->AccessLink()->Name()).append("\""));

    m_protocolName = protocolNameAttribute->ValueAsText();
    if (protocolParametersAttribute != nullptr)
    {
      m_protocolParameters = protocolParametersAttribute->ValueAsText();
    }
  }

  LOG(INFO) << "Using JTAG TAP with protocol: " << m_protocolName << " and parameters: " << m_protocolParameters;

  auto& factory  = AccessInterfaceProtocolFactory::Instance();
  auto  protocol = factory.Create(m_protocolName, m_protocolParameters);

  CHECK_VALUE_NOT_NULL(protocol.get(), "Failed to create a protocol with name \""s.append(m_protocolName).append("\""));
  return protocol;
}
//
//  End of: AST_SystemModelGenerator::Create_Protocol
//---------------------------------------------------------------------------






//! Finds ScanInterface defined by ScanInterface reference in a module
//!
//! @param module             Module from which search operates
//! @param instanceRef        Optional Instance reference (when the ScanInterface is defined in an Instance module)
//! @param scanInterfaceName  Name of the ScanInterface to look for
//!
tuple<AST_Instance*, AST_Module*, AST_ScanInterface*>
AST_SystemModelGenerator::FindScanInterface (AST_Module*           module,
                                             AST_ScalarIdentifier* instanceRef,
                                             const string&         scanInterfaceName)
{
  if (instanceRef != nullptr)
  {
    auto instance         = module->FindInstance(instanceRef);
    CHECK_VALUE_NOT_NULL(instance, "Cannot find instance \""s.append(instanceRef->AsText()).append("\""));

    auto moduleIdentifier = instance->ModuleIdentifier();
    auto instanceModule   = m_network->Module(moduleIdentifier);
    CHECK_VALUE_NOT_NULL(instanceModule, "Cannot find module instance \""s.append(instanceRef->AsText()).append("\""));

    auto scanInterface = instanceModule->FindScanInterface(scanInterfaceName);
    CHECK_VALUE_NOT_NULL(scanInterface, "Cannot find ScanInterface \""s.append(scanInterfaceName)
                                        .append("\" in instance \"").append(instanceRef->AsText()).append("\""));

    return make_tuple(instance, instanceModule, scanInterface);
  }

  // ==> Scan interface must be in module definition
  auto scanInterface = module->FindScanInterface(scanInterfaceName);

  CHECK_VALUE_NOT_NULL(scanInterface, "Cannot find ScanInterface \""s.append(scanInterfaceName)
                                      .append("\" in module\"").append(module->Name()).append("\""));

  return make_tuple(nullptr, module, scanInterface);
}
//
//  End of: AST_SystemModelGenerator::FindScanInterface
//---------------------------------------------------------------------------



//! Finds a ScanOutPort defined in a ScanInterface
//!
//! @param module         Module in which the ScanInterface is defined
//! @param scanInterface  ScanInterface defining ScanOutPort
//!
const AST_Port* AST_SystemModelGenerator::FindScanOutPort (AST_Module* module, const AST_ScanInterface* scanInterface)
{
  const auto& ports = scanInterface->Ports();

  for (const auto& port : ports)
  {
    const auto modulePort = module->FindScanOutPort(port->Identifier());
    if (modulePort != nullptr)
    {
      return modulePort;
    }
  }

  CHECK_FAILED("Failed to find ScanOutPort defined by ScanInterface \""s.append(scanInterface->Name())
               .append("\" in module \"").append(module->Name()).append("\""));
}
//
//  End of: AST_SystemModelGenerator::FindScanOutPort
//---------------------------------------------------------------------------


//! Follows ScanMux selector signals to find driving ScanRegister(s)
//!
//! @param selectors  ScanMux selector signals
//! @param module     ScanMux parent module
//!
//! @return Ordered Sets of driving ScanRegisters along with whether it has bits span (left and right)
//!
vector<tuple<AST_ScanRegister*, bool, uint32_t, uint32_t>>
AST_SystemModelGenerator::FindSelectorRegisters (const std::vector<Parsers::AST_Signal*>& selectors,
                                                 AST_Module*                              module) const
{
  vector<tuple<AST_ScanRegister*, bool, uint32_t, uint32_t>> scanRegisters;

  // ---------------- Follow each selector signal
  //
  for (const auto& selector : selectors)
  {
    const auto identifier = selector->PortName();
    const auto portScope  = selector->PortScope();

    AST_ScanRegister* scanRegister = nullptr;

    if (portScope.empty())
    {
      scanRegister = module->FindScanRegister(identifier);
    }
    else
    {
      CHECK_VALUE_LTE(portScope.size(), 1u, "Not Yet Supported: Far away selector ScanRegister");

      auto selectorModule = module;
      for (const auto scopeIdentifier : portScope)
      {
        auto foundInstance = selectorModule->FindInstance(scopeIdentifier);
        if (foundInstance == nullptr)
        {
          break;
        }
        selectorModule = foundInstance->UniquifiedModule();
      }

      scanRegister = selectorModule->FindScanRegister(identifier);
      if (scanRegister == nullptr)  // ==> Probably need to follow DataOutPort source !
      {
        auto dataOutPort = selectorModule->FindDataOutPort(identifier);
        CHECK_VALUE_NOT_NULL(dataOutPort, "Failed to find DataOutPort in module \""s.append(selectorModule->Name()).append("\""));

        auto source = dataOutPort->Source();
        CHECK_VALUE_NOT_NULL(source, "Failed to find DataOutPort source in module \""s.append(selectorModule->Name()).append("\""));

        auto sourceSignals = source->Signals();

        CHECK_VALUE_EQ(sourceSignals.size(), 1u, "Expecting find DataOutPort source in module \""s.append(selectorModule->Name()).append("\" to be driven by exactly 1 signal"));

        auto portName = sourceSignals.front()->PortName();
        CHECK_VALUE_NOT_NULL(portName, "Expecting DataOutPort in module \""s.append(selectorModule->Name()).append("\" to have a valid source signal port name"));

        scanRegister = selectorModule->FindScanRegister(portName);
      }
    }

    CHECK_VALUE_NOT_NULL(scanRegister, "Failed to find ScanRegister selector for scan_mux in module \""s.append(module->Name()).append("\""));

    bool     hasRange   = false;
    uint32_t leftIndex  = 0u;
    uint32_t rightIndex = 0u;

    if (identifier->IsKind(Kind::VectorIdentifier))
    {
      auto asVectorIdentifier = static_cast<const AST_VectorIdentifier*>(identifier);

      std::tie(hasRange, leftIndex, rightIndex) = asVectorIdentifier->Range();
    }

    scanRegisters.emplace_back(scanRegister, hasRange, leftIndex, rightIndex);
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

  while (   (module != nullptr)
         && (!isSourcedByTopModuleInput || !m_linkersContext.empty()))
  {
    if (isSourcedByTopModuleInput)  // ==> !m_linkersContext.empty()
    {
      CHECK_VALUE_EQ(m_instancesContext.size(), 1u, "When reaching top module there should be only 1 instance context left, got "s.append(std::to_string(m_instancesContext.size())));

      std::tie(module, sourceSignals) = Process_ScanMux_EndOfSelectionPath(nullptr);
      if (module == nullptr)
      {
        return;
      }
    }
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
            if (!scanRegister->HasAssociatedRegister())       // Have we already gone to this path point ?
            {
              sourceSignals = Process_ScanRegister(scanRegister);
            }
            else if (!m_linkersContext.empty())
            {
              std::tie(module, sourceSignals) = Process_ScanMux_EndOfSelectionPath(scanRegister->AssociatedRegister());
            }
            else
            {
              return;
            }
          }
          else  // ScanMux
          {
            auto scanMux  = module->FindScanMux(identifier);
            CHECK_VALUE_NOT_NULL(scanMux, "Failed to find source entity (not a ScanMux)");

            if (scanMux->HasAssociatedLinker()) // ==> If we already passed onto that linker, we must be in context of another linker processing
            {
              std::tie(module, sourceSignals) = Process_ScanMux_EndOfSelectionPath(scanMux->AssociatedLinker());
            }
            else
            {
              std::tie(module, sourceSignals) = Process_ScanMux_Entry(scanMux, module);
            }
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

          std::tie(module, sourceSignals) = Process_Instance_Entry(instance, instanceModule, scanOutPort);
        }

        if (module == nullptr)
        {
          return;
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

  AST_Module::ResetCircularDependencyTracker();

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
        topNode = Generate_JTAGTap(topModule);
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

    SavePDLAssociations(nullptr, topModule, chain); // No real instance for top module

    Generate_TopModule(chain.get(), topModule);

    topNode = chain;
  }

  //! @todo [JFC]-[November/23/2017]: In Generate(): Resolve linkers
  //!
  CHECK_VALUE_EMPTY(m_unresolvedPathSelectors, "Not Yet Supported: Unresolved path selector");
//+  for (const auto& linker : m_unresolvedPathSelectors)
//+  {
//+  }

  ConvertAliases(topModule, topNode.get());

  return topNode;
}
//
//  End of: AST_SystemModelGenerator::Generate_Network
//---------------------------------------------------------------------------


//! Creates a SystemModel nodes for network top module
//!
//! @param topModule  Network top module with AccessLink to be converted to SystemModel nodes
//!
shared_ptr<ParentNode> AST_SystemModelGenerator::Generate_JTAGTap (AST_Module* topModule)
{
  CHECK_PARAMETER_NOT_NULL(topModule, "Need a valid \"top\" module");
  auto accessLink = topModule->AccessLink();
  CHECK_PARAMETER_NOT_NULL(accessLink, "\"Top\" module must have a valid AccessLink");

  // ---------------- Capture BSDL file name
  //
  auto bsdlFileRef = accessLink->BSDL();
  CHECK_VALUE_NOT_NULL(bsdlFileRef, "AccessLink must refer to a valid BSDL name");
  auto bsdlFileName = bsdlFileRef->Name();

  // ---------------- Capture BSDL instructions names
  //
  vector<string_view> instructionsNames;

  const auto& bsdlInstructionsRef = accessLink->BsdlInstructionsRef();
  for (const auto bsdlInstruction : bsdlInstructionsRef)
  {
    instructionsNames.push_back(bsdlInstruction->Name());
  }

  // ---------------- Parse BSDL
  //
  auto bsdlFilePath = ResolveBSDL_FilePath(bsdlFileName);
  auto bsdlContent  = Utility::ReadTextFile(bsdlFilePath);

  BSDL_Reader bsdlReader;
  bsdlReader.Parse(bsdlContent, instructionsNames);
  const auto& selectTableValues = bsdlReader.SelectTable();

  vector<BinaryVector> selectTable;
  for (const auto& selectValue : selectTableValues)
  {
    selectTable.push_back(BinaryVector::CreateFromBinaryString(selectValue));
  }

  // ---------------- Create TAP node
  //
  const auto& tapName       = accessLink->Name();
  auto        irBitsCount   = bsdlReader.IrBitsCount();
  auto        muxPathsCount = bsdlInstructionsRef.size() + 1u;  // +1 is for the bypass register
  auto        protocol      = Create_Protocol(topModule);

  auto tap = m_builder->Create_JTAG_TAP(tapName, irBitsCount, muxPathsCount, std::move(protocol), selectTable);

  // ---------------- Build nodes below tap
  //
  Generate_JTAGTapChildren(tap.get(), topModule, bsdlInstructionsRef);

  SavePDLAssociations(nullptr, topModule, tap); // No real instance for top module

  return tap;
}
//
//  End of: AST_SystemModelGenerator::Generate_JTAGTap
//---------------------------------------------------------------------------


//! Generates SystemModel sub-tree under JTAG TAP
//!
//! @param tap                  Tap AccessInterface
//! @param topModule            Module in which AccessLink is defined
//! @param bsdlInstructionsRef  BSDL instruction references (tells which ScanInterface to deal with)
//!
void AST_SystemModelGenerator::Generate_JTAGTapChildren (AccessInterface*                            tap,
                                                         AST_Module*                                 topModule,
                                                         const std::vector<AST_BsdlInstructionRef*>& bsdlInstructionsRef)
{
  InstanceContext context;
  context.instance          = nullptr;    // Instance is implicit
  context.parentModule      = topModule;
  context.parentNode        = tap;
  context.createdNodesLevel = 0;
  m_instancesContext.push(context); // Context is popped when path reaches instance input


  for (const auto bsdlInstruction : bsdlInstructionsRef)
  {
    const auto& scanInterfacesRef = bsdlInstruction->ScanInterfacesRef();

    for (const auto scanInterfaceRef : scanInterfacesRef)
    {
      const auto& interfacesNames = scanInterfaceRef->ScanInterfaceNames();
      CHECK_FALSE(interfacesNames.size() > 1u, "Only support one ScanInteface per BSDL instruction");

      const auto& scopedInterfaceName = interfacesNames.front();
      const auto  instanceRef         = std::get<0>(scopedInterfaceName);
      const auto& scanInterfaceName   = std::get<1>(scopedInterfaceName);

      AST_ScanInterface* scanInterface  = nullptr;
      AST_Module*        instanceModule = nullptr;
      AST_Instance*      instance       = nullptr;

      std::tie(instance, instanceModule, scanInterface) = FindScanInterface(topModule, instanceRef, scanInterfaceName);

      auto scanOutPort = FindScanOutPort(instanceModule, scanInterface);

      CHECK_VALUE_EMPTY(m_linkersContext, "No linker should be processed when dealing with top node ScanOutPort");

      if (instance != nullptr)
      {
        Process_Instance_Entry(instance, instanceModule, scanOutPort);

        FollowTopModulePath(instanceModule, scanOutPort);
      }
      else
      {
        FollowTopModulePath(topModule, scanOutPort);
      }
      AppendCreatedNodesToParent(tap, 0u);
    }
  }
}
//
//  End of: AST_SystemModelGenerator::Generate_JTAGTapChildren
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
//! @return New processing context
//!
AST_SystemModelGenerator::ProcessingContext_t
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
    // ---------------- Create Chain to "encapsulate" instance sub-nodes
    //
    auto name = instance->Name();
    chain     = m_systemModel->CreateChain(name);

    instance->AssociatedChain(chain);   // To detect already created Chain for the instance
    AssignNewNode(chain);
    SavePDLAssociations(instance, instanceModule, chain);
    context.parentNode = chain.get();
  }
  else if (instanceModule->IsScanOutPortMarked(scanOutPort))
  {
    CHECK_VALUE_NOT_EMPTY(m_linkersContext, "Unexpected path again through instance output port outside of Linker context");

    return Process_ScanMux_EndOfSelectionPath(instance->AssociatedChain());
  }

  instanceModule->MarkScanOutPort(scanOutPort);

  context.createdNodesLevel = m_createdNodes.size();

  m_instancesContext.push(context);

  // ---------------- Return signal to follow to reach first instance entity
  //
  auto        source        = scanOutPort->Source();
  const auto& sourceSignals = source->Signals();

  return make_tuple(instanceModule, cref(sourceSignals));
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
AST_SystemModelGenerator::ProcessingContext_t
AST_SystemModelGenerator::Process_Instance_Exit (AST_Port* scanInPort)
{
  CHECK_VALUE_NOT_EMPTY(m_instancesContext, "Houps not balanced push/pop on instance context");

  auto& context           = m_instancesContext.top();
  auto  instance          = context.instance;
  auto  portIdentifier    = scanInPort->Identifier();
  auto  instanceInputPort = instance->FindInputPort(portIdentifier);

  if (instanceInputPort == nullptr)   // Is this special top level instance referred by an AccessLink ?
  {
    auto instanceModule = instance->UniquifiedModule();
    auto scanInPort     = instanceModule->FindScanInPort(portIdentifier);
    CHECK_PARAMETER_NOT_NULL(scanInPort, "Cannot find instance module Port: \""s.append(portIdentifier->Name()).append("\""));
    return make_tuple(nullptr, cref(sm_noSignals));   // This is to report end of scan path !
  }

  auto        instanceSource = instanceInputPort->Source();
  const auto& sourceSignals  = instanceSource->Signals();

  // ---------------- Restore previous context
  //
  m_instancesContext.pop();
  CHECK_VALUE_NOT_EMPTY(m_instancesContext, "Must have at least an (implicit) instance context for top module");

  auto module = m_instancesContext.top().parentModule;

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
AST_SystemModelGenerator::ProcessingContext_t
AST_SystemModelGenerator::Process_ScanMux_Entry (AST_ScanMux* scanMux, AST_Module* module)
{
  CHECK_FALSE(scanMux->IsBusMux(),            "Not Yet Supported: ScanMux for buses");
  CHECK_FALSE(scanMux->HasAssociatedLinker(), "Cannot create another linker for ScanMux \""s.append(scanMux->BaseName()).append("\""));

  // ---------------- Create Linker
  //
  auto        unresolvedPathSelector = make_shared<UnresolvedPathSelector>();
  const auto& name                   = scanMux->BaseName();
  auto        linker                 = m_systemModel->CreateLinker(name, unresolvedPathSelector);

  scanMux->AssociatedLinker(linker);

  AssignNewNode(linker);

  auto& instanceContext  = m_instancesContext.top();
  auto  linkerParentNode = instanceContext.parentNode;

  instanceContext.linkerNode = linker.get();

  LinkerContext linkerContext;

  linkerContext.instancesContext       = m_instancesContext;
  linkerContext.processedScanMux       = scanMux;
  linkerContext.processedSelectionId   = 0;
  linkerContext.linkerNodesLevel_first = m_createdNodes.size();
  linkerContext.linkerNodesLevel       = m_createdNodes.size();
  linkerContext.linker                 = linker.get();
  linkerContext.linkerParentNode       = linkerParentNode;

  m_linkersContext.push(std::move(linkerContext));

  const auto& sourceSignals = scanMux->Selections().front()->SelectedSignals();
  return make_tuple(module, cref(sourceSignals));
}
//
//  End of: AST_SystemModelGenerator::Process_ScanMux_Entry
//---------------------------------------------------------------------------






//! Does what is needed to process when we reached end of ScanMux selection
//!
//! @return New processing context
//!
AST_SystemModelGenerator::ProcessingContext_t
AST_SystemModelGenerator::Process_ScanMux_EndOfSelectionPath (shared_ptr<mast::SystemModelNode> commonLinkerNode)
{
  CHECK_VALUE_NOT_EMPTY(m_linkersContext, "There is no linker context to process");

  auto&       linkerContext        = m_linkersContext.top();
  auto        linker               = linkerContext.linker;
  auto        scanMux              = linkerContext.processedScanMux;
  auto        createdNodesLevel    = linkerContext.linkerNodesLevel;
  auto        processedSelectionId = linkerContext.processedSelectionId;
  auto const& selections           = scanMux->Selections();
  auto        module               = linkerContext.instancesContext.top().parentModule;

  CHECK_VALUE_GTE(m_createdNodes.size(), createdNodesLevel, "Have appended more nodes than expected");
  auto newCreated = m_createdNodes.size() - createdNodesLevel;

  if (processedSelectionId == 0)
  {
    linkerContext.linkerNodesLevel = m_createdNodes.size();
  }
  else if (newCreated > 1u)
  {
    auto chain = Create_ChainForLinker(linker, processedSelectionId);
    linker->AppendChild(chain);
    AppendCreatedNodesToParent(chain.get(), createdNodesLevel);
  }
  else if (newCreated != 0)
  {
    AppendCreatedNodesToParent(linker, createdNodesLevel);
  }

  ++processedSelectionId;

  m_instancesContext = linkerContext.instancesContext;

  bool lastLinkerSelection = processedSelectionId >= selections.size();
  if (lastLinkerSelection)
  {
    auto firstSelectionIsEmpty = AssignNodesToLinkerFirstSelection(commonLinkerNode);

    // ---------------- Assign proper PathSelector
    //
    auto pathSelector = Create_PathSelector(scanMux, module, firstSelectionIsEmpty);
    linker->ReplacePathSelector(pathSelector);

    // ---------------- Deal with parent nodes
    //
    auto parentNode = linkerContext.linkerParentNode;

    auto& instanceContext = m_instancesContext.top();
    instanceContext.parentNode = parentNode;
    instanceContext.linkerNode = nullptr;

    auto sourceSignals = std::cref(selections.front()->SelectedSignals()); // Will cause detection of instance input port or previously created node that is just before the linker

    m_linkersContext.pop();
    if (m_linkersContext.empty())
    {
      return make_tuple(nullptr, cref(sourceSignals));  // To report end of path processing
    }

    return Process_ScanMux_EndOfSelectionPath(nullptr);
  }

  ++linkerContext.processedSelectionId;
  auto sourceSignals = std::cref(selections[processedSelectionId]->SelectedSignals());
  return make_tuple(module, cref(sourceSignals));
}
//
//  End of: AST_SystemModelGenerator::Process_ScanMux_EndOfSelectionPath
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
  auto source      = scanRegister->ScanInSource();
  auto name        = scanRegister->BaseName();
  auto bitsCount   = scanRegister->BitsCount();
  auto resetValue  = scanRegister->ResetValue();
  auto bypassValue = BinaryVector(bitsCount, 0);

  if (resetValue != nullptr)
  {
    try
    {
      bypassValue = resetValue->AsBinaryVector(bitsCount);
    }
    catch(std::invalid_argument&)
    {
      auto message = "Reset value "s;
      message += resetValue->IsFullySized() ? resetValue->AsBinaryVector().DataAsICLMixString()
                                            : resetValue->AsText();
      message += " is too large for register \"" + name + "\" with " + std::to_string(bitsCount) + " bit(s)";
      CHECK_FAILED(message);
    }
  }
  bypassValue.FixSize(true);

  auto holdValue    = false;
  auto registerNode = m_systemModel->CreateRegister(name, bypassValue, holdValue);

  scanRegister->AssociatedRegister(registerNode);

  AssignNewNode(registerNode);

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

    const auto value               = values.front();
    auto       valueAsBinaryVector = value->AsBinaryVector(expectedBitsCount);
    selectTable.emplace_back(std::move(valueAsBinaryVector));
  }

  //! @todo [JFC]-[November/17/2017]: Support Concat number list in AST_SystemModelGenerator::MakeSelectionTable()

  auto isFirst = true;
  vector<BinaryVector> deselectTable = selectTable;
  for (auto& deselectValue : deselectTable)
  {
    if (isFirst)
    {
      isFirst = false;
    }
    else
    {
      deselectValue.ToggleBits();
    }
  }

  return make_tuple(selectTable, deselectTable);
}
//
//  End of: AST_SystemModelGenerator::MakeSelectionTable
//---------------------------------------------------------------------------



//! For each attribute with name "PDL", associate the algorithm name with the Chain node
//!
//! @note PDL attributes can be defined at Module and/or Instance levels
//!
//! @param instance     ICL instance for which a Chain has been created (optional)
//! @param module       Processed module
//! @param parentNode   Associated node in SystemModel
//!
void AST_SystemModelGenerator::SavePDLAssociations (AST_Instance* instance, AST_Module* module, shared_ptr<ParentNode> parentNode)
{
  // ---------------- Local associations
  //
  vector<AppFunctionNameAndNode> algorithmAssociations; // Use local associations for fast and simple search for override management
  auto hasAlreadyAssociationFor = [&algorithmAssociations](const auto& algorithmId)
  {
    auto sameName = [&algorithmId](const auto& item){return item.appName == algorithmId;};

    return std::find_if(algorithmAssociations.begin(), algorithmAssociations.end(), sameName) != algorithmAssociations.cend();
  };


  auto processAttributes = [&](const auto& attributes)
  {
    for (const auto attribute : attributes)
    {
      if (attribute->Name() == "PDL")
      {
        auto algorithmId = attribute->ValueAsText();
        if (!hasAlreadyAssociationFor(algorithmId))
        {
          constexpr uint32_t line = 0;                                      // File line in which the attribute is defined is not yet supported
          algorithmAssociations.emplace_back(algorithmId, parentNode, line);
        }
      }
    }
  };

  // ---------------- At Module level
  //
  if (module)
  {
    processAttributes(module->Attributes());
  }

  // ---------------- At instance level
  //
  if (instance)
  {
    processAttributes(instance->Attributes());
  }

  // ---------------- Merge with associations for network
  //
  for (auto&& algorithmAssociation : algorithmAssociations)
  {
    m_algorithmAssociations.emplace_back(std::move(algorithmAssociation));
  }
}
//
//  End of: AST_SystemModelGenerator::SavePDLAssociations
//---------------------------------------------------------------------------



//! Resolves actual path for BSDL file
//!
//! @param bsdlName   Name for BSDL file found in AccessLink
//!
string AST_SystemModelGenerator::ResolveBSDL_FilePath (const string& bsdlName)
{
  // ---------------- Try with just the name
  //
  if (Utility::FileExists(bsdlName))
  {
    return bsdlName;
  }

  // ---------------- Try using search paths (with or without forced .bsd extension)
  //
  for (const auto& searchPath : m_filesSearchPaths)
  {
    auto filePath = searchPath;
    filePath.append(DIRECTORY_SEPARATOR).append(bsdlName);

    if (Utility::FileExists(filePath))
    {
      return filePath;
    }

    filePath.append(".bsd");
    if (Utility::FileExists(filePath))
    {
      return filePath;
    }
  }

  CHECK_FAILED("Cannot find BSDL file with name: \""s.append(bsdlName).append("\""));
}
//
//  End of: AST_SystemModelGenerator::ResolveBSDL_FilePath
//---------------------------------------------------------------------------



//===========================================================================
// End of AST_SystemModelGenerator.cpp
//===========================================================================
