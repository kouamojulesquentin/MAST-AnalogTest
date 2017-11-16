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

#include "UnresolvedPathSelector.hpp"
#include "SystemModel.hpp"
#include "SystemModelBuilder.hpp"
#include "Utility.hpp"
#include "g3log/g3log.hpp"

#include <stack>

using std::stack;
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



//! Generates a SystemModel (sub-)tree from AST_Network
//!
shared_ptr<mast::SystemModelNode> AST_SystemModelGenerator::Generate (AST_Network* network)
{
  m_network = CHECK_PARAMETER_NOT_NULL(network, "Expect valid, not nullptr AST_Network");

  m_parsedTopNode = Generate_Network(network);

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
AST_SystemModelGenerator::Generate_Instance (const AST_Instance* instance, const AST_Module* instanceModule)
{
  const auto& moduleInputPorts = instanceModule->ScanInPorts();

  CHECK_VALUE_NOT_EMPTY(moduleInputPorts,  "Expecting a module \""s.append(instanceModule->Name()).append("\"to have at least one ScanInPort"));

  // ---------------- Find instance source
  //
  auto moduleInputPort   = moduleInputPorts.front();
  auto moduleInputPortId = moduleInputPort->Identifier();
  auto instanceInputPort = instance->FindInputPort(moduleInputPortId);
  auto instanceSource    = instanceInputPort->Source();

  // ---------------- Create Chain to "encapsulate" instance sub-nodes
  //
  auto name  = instance->Name();
  auto chain = m_systemModel->CreateChain(name);

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
shared_ptr<mast::SystemModelNode> AST_SystemModelGenerator::Generate_Network (const AST_Network* network)
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
void AST_SystemModelGenerator::Generate_Module (mast::Chain* chain, const AST_Module* module)
{
  const auto& scanInPorts  = module->ScanInPorts();
  const auto& scanOutPorts = module->ScanOutPorts();

  CHECK_VALUE_NOT_EMPTY(scanInPorts,  "Expecting a module to have at least one ScanInPort");
  CHECK_VALUE_NOT_EMPTY(scanOutPorts, "Expecting a module to have at least one ScanOutPort");

  auto scanInPort  = scanInPorts.front();
  auto scanOutPort = scanOutPorts.front();
  auto source      = scanOutPort->Source();

  // Lamba: Tells whether some source match module input port (currently only considering first)
  auto isSourcedByModuleInput = [scanInPort](const AST_Source* source)
  {
    const auto& signals = source->Signals();

    CHECK_VALUE_EQ(signals.size(), 1u, "Expecting ScanOutPort source to be driven by exactly one signal");
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

  while (!isSourcedByModuleInput(source))
  {
    const auto& signals = source->Signals();

    CHECK_VALUE_EQ(signals.size(), 1u, "Expecting ScanOutPort source to be driven by exactly one signal");
    const auto signal      = signals.front();
    const auto portScope   = signal->PortScope();
    const auto identifier  = signal->PortName();

    std::shared_ptr<mast::SystemModelNode> createdNode;

    if (portScope.empty())  // scanRegister or ScanMux ?
    {
      auto scanRegister = module->FindScanRegister(identifier);
      if (scanRegister != nullptr)
      {
        std::tie(createdNode, source) = Generate_Register(scanRegister);
      }
      else
      {
        auto scanMux = module->FindScanMux(identifier);
        std::tie(createdNode, source) = Generate_ScanMux(scanMux);
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
//!
//! @return Created Linker and ScanMux AST_Source
tuple<std::shared_ptr<mast::SystemModelNode>, const AST_Source*>
AST_SystemModelGenerator::Generate_ScanMux (const AST_ScanMux* scanMux)
{
  // ---------------- Collect selector(s) path(s) including bits ranges
  // Paths are ordered and defined using SystemModel path syntax
  // Bits ranges are defined as pair of integers
  //
  const auto& selectors          = scanMux->Selectors();
  const auto  selectorsBitsCount = selectors.size();
//+  auto paths = MakePaths(selectors);

  // ---------------- Prepare table
  //
  auto const& selection = scanMux->Selections();
//+  auto        table     = MakeSelectionTable();

  // ---------------- Create Linker
  //
  auto pathSelector = make_shared<UnresolvedPathSelector>();

//+  const auto muxWidth = scanMux->Width();
  const auto name     = scanMux->BaseName();
  auto       linker   = m_systemModel->CreateLinker(name, pathSelector);

  // ---------------- Find "source" of linker
  //
  AST_Source* source = nullptr;


  CHECK_FAILED("Not Yet Supported: ScanMux");
  return make_tuple(linker, source);
}
//
//  End of: AST_SystemModelGenerator::Generate_Register
//---------------------------------------------------------------------------


//! Creates a SystemModel Register from an AST_ScanRegister
//!
//! @param scanRegister   ScanRegister to be converted to SystemModel Register
//!
//! @return Created Register and ScanRegister AST_Source
tuple<std::shared_ptr<mast::SystemModelNode>, const AST_Source*>
AST_SystemModelGenerator::Generate_Register (const AST_ScanRegister* scanRegister)
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

  return make_tuple(registerNode, registerSource);
}
//
//  End of: AST_SystemModelGenerator::Generate_Register
//---------------------------------------------------------------------------




//===========================================================================
// End of AST_SystemModelGenerator.cpp
//===========================================================================
