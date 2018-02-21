//===========================================================================
//                           AST_Helper.cpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_Helper.cpp
//!
//! Implements class AST_Helper
//!
//===========================================================================

#include "AST_Helper.hpp"
#include "AST_Instance.hpp"
#include "AST_Module.hpp"
#include "AST_Port.hpp"
#include "AST_ScalarIdentifier.hpp"
#include "AST_ScanRegister.hpp"
#include "AST_Signal.hpp"
#include "AST_Source.hpp"
#include "AST_VectorIdentifier.hpp"

#include "Utility.hpp"
#include "g3log/g3log.hpp"

#include <tuple>
#include <functional>
#include <experimental/string_view>

using std::vector;
using std::shared_ptr;
using std::unique_ptr;
using std::tuple;
using std::string;
using std::experimental::string_view;

using namespace mast;
using namespace Parsers;


// Follows a signal until it reaches a ScanRegister
//
// @param module  Module from which signal to follow is defined
// @param signal  Signal to follow
//
// @return  Found ScanRegister in case of success, nullptr otherwise
//
AST_ScanRegister* AST_Helper::FollowSignalTilScanRegister (AST_Module* module, AST_Signal* signal)
{
  if (signal->IsNumber())
  {
    return nullptr;
  }

  // ---------------- Traverse scope (instances)
  //
  const auto portScope     = signal->PortScope();
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

  // ---------------- Search locally (current module)
  //
  const auto identifier   = signal->PortName();
  const auto scanRegister = currentModule->FindScanRegister(identifier);

  if (scanRegister != nullptr)
  {
    return scanRegister;
  }

  // ---------------- Search through DataInPorts
  //
  auto dataInPort = currentModule->FindDataInPort(identifier);
  if (dataInPort != nullptr)
  {
    auto instanceOfModule = currentModule->FromInstance();
    if (instanceOfModule == nullptr)
    {
      LOG(WARNING) << "While traversing DataInPort \"" << dataInPort->Name() << "\", cannot follow I/O ports from top module ";
      return nullptr;
    }

    auto instanceInputPort = instanceOfModule->FindInputPort(identifier);
    if (instanceInputPort == nullptr)
    {
      LOG(WARNING) << "Failed to find input port \"" << identifier->AsText() <<  "\" for instance \"" << instanceOfModule->Name() << "\"";
      return nullptr;
    }

    auto source = instanceInputPort->Source();
    CHECK_VALUE_NOT_NULL(source, "An input port must have a valid source");
    const auto& sourceSignals = source->Signals();
    CHECK_VALUE_EQ(sourceSignals.size(), 1u, "While traversing input port \""s + instanceInputPort->Name() + "\" found a source not driven by a scalar signals ==> this is not yet supported");

    auto parentModule = module->ParentModule();
    CHECK_PARAMETER_NOT_NULL(parentModule, "Houps: After unification, all modules (except top one) must have a \"parent\" module. Module \""s + module->Name() + "\" has none");

    return FollowSignalTilScanRegister(parentModule, sourceSignals.front());
  }

  // ---------------- Search through DataOutPorts
  //
  auto dataOutPort = currentModule->FindDataOutPort(identifier);
  if (dataOutPort != nullptr)
  {
    auto source = dataOutPort->Source();
    if (source != nullptr)                // Has local source ?
    {
      const auto& sourceSignals = source->Signals();
      CHECK_VALUE_EQ(sourceSignals.size(), 1u, "While traversing output port \""s + dataOutPort->Name() + "\" found a source not driven by a scalar signals ==> this is not yet supported");

      return FollowSignalTilScanRegister(currentModule, sourceSignals.front());
    }
    else  // ==> Try to find connection in module instantiating this instance
    {
      // ---------------- Find instance of module instanciating module from which we want to follow DataOutPort
      //
      auto instanceOfModule = currentModule->FromInstance();
      if (instanceOfModule == nullptr)
      {
        LOG(WARNING) << "While traversing DataOutPort \"" << dataOutPort->Name() << "\", cannot follow I/O ports from top module ";
        return nullptr;
      }

      auto parentModule = currentModule->ParentModule();
      CHECK_PARAMETER_NOT_NULL(parentModule, "Houps: After unification, all modules (except top one) must have a \"parent\" module. Module \""s + currentModule->Name() + "\" has none");

      auto scanRegister = AST_Helper::ScanRegisterConnectedToInstancePort(parentModule, instanceOfModule->InstanceIdentifier(), dataOutPort->Identifier());
      return scanRegister;
    }
  }

  // ---------------- Report "Not found" with nullptr
  //
  return nullptr;
}
//
//  End of: AST_Helper::FollowSignalTilScanRegister
//---------------------------------------------------------------------------



//! Searches connection down to ScanRegister, starting from an instance Port
//!
//! @note Search is not done the usual way (using sources) but the aways around (it is then less direct)
//! @note Expecting ScanRegister capture source to be connected to the instance port
//!
//! @param module       Module where the instance is defined
//! @param instanceId   Instance id which port is expected to be connected to a ScanRegister
//!                     When nullptr, search is done only from module, direct, ScanRegisters
//! @param portId       Instance port name
//!
//! @return Found ScanRegister or nullptr when there is no (or we cannot find) connection to a ScanRegister
//!
AST_ScanRegister* AST_Helper::ScanRegisterConnectedToInstancePort (const AST_Module* module, const AST_ScalarIdentifier* instanceId, const AST_VectorIdentifier* portId)
{
  // ---------------- Search from local ScanRegisters
  //
  for (const auto scanRegister : module->ScanRegisters())
  {
    const auto  captureSource = scanRegister->CaptureSource();
    const auto& sourceSignals = captureSource->Signals();

    CHECK_VALUE_EQ(sourceSignals.size(), 1u, "While traversing ScanRegister CaptureSource \""s + captureSource->AsText() + "\" found it is not driven by a scalar signals ==> this is not yet supported");
    const auto signal = sourceSignals.front();

    if (signal->PortName()->BaseName() == portId->Name())
    {
      return scanRegister;
    }
  }

  // ---------------- Check there is an instance id to look for
  //
  if (instanceId == nullptr)
  {
    return nullptr;
  }

  // ---------------- Search from local instances
  //
  for (const auto instance : module->Instances())
  {
    // ---------------- Skip the very instance we search port connection to ScanRegister
    //
    if (instance->InstanceIdentifier()->Name() == instanceId->Name())
    {
      continue;
    }

    // ---------------- Search in instance input ports
    //
    for (const auto port : instance->InputPorts())
    {
      const auto  portSource    = port->Source();
      const auto& sourceSignals = portSource->Signals();

      CHECK_VALUE_EQ(sourceSignals.size(), 1u, "While traversing input port \""s + port->Name() + "\" found a source not driven by a scalar signals ==> this is not yet supported");
      const auto signal = sourceSignals.front();

      const auto  portName  = signal->PortName();
      const auto& portScope = signal->PortScope();

      if (portName->BaseName() != portId->Name())
      {
        continue; // Not port name we are looking for
      }

      if (portScope.size() != 1)
      {
        continue; // This is not a port connected to our "from" port
      }

      auto scopePart = portScope.front();
      if (scopePart->Name() == instanceId->Name())   // Do check the scope
      {
        // ---------------- Search down in instance module its input port connections
        //
        auto instanceModule = instance->UniquifiedModule();
        auto instancePortId = port->Identifier();
        auto dataInPort     = instanceModule->FindDataInPort(port->Identifier());
        if (dataInPort != nullptr)    // Is there really a DataInPort in that module ?
        {
          return ScanRegisterConnectedToInstancePort(instanceModule, nullptr, instancePortId);
        }
      }
    }
  }
  return nullptr;
}
//
//  End of: AST_Helper::ScanRegisterConnectedToInstancePort
//---------------------------------------------------------------------------


//===========================================================================
// End of AST_Helper.cpp
//===========================================================================
