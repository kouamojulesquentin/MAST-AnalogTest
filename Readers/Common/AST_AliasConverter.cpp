//===========================================================================
//                           AST_AliasConverter.cpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AST_AliasConverter.cpp
//!
//! Implements class AST_AliasConverter
//!
//===========================================================================

#include "AST_AliasConverter.hpp"
#include "AST_Alias.hpp"
#include "AST_Instance.hpp"
#include "AST_Module.hpp"
#include "AST_Port.hpp"
#include "AST_ScalarIdentifier.hpp"
#include "AST_ScanRegister.hpp"
#include "AST_Signal.hpp"
#include "AST_Source.hpp"
#include "AST_VectorIdentifier.hpp"

#include "RegistersAlias.hpp"
#include "SystemModel.hpp"
#include "Utility.hpp"
#include "MastConfig.hpp"
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


//! Converts aliases, starting from given AST module
//!
//! @param module       Module for which aliases will be converted
//! @param parentNode   SystemModelNode associated with processed module
//!
void AST_AliasConverter::ConvertAliases (AST_Module* module, ParentNode* parentNode)
{
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
        auto scanRegister = FollowSignalTilScanRegister(module, signal);
        if (scanRegister == nullptr)
        {
          LOG(WARNING) << "Alias \"" << alias->Name() << "\" in module \"" << module->Name() << "\" does not lead to a scan register ==> This is not yet supported";
        }
        else
        {
          virtualReg.Append(MakeRegisterSlice(scanRegister, signal));
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
  const auto& instances = module->Instances();
  for (auto instance : instances)
  {
    auto associatedNode = instance->AssociatedChain();
    auto instanceModule = instance->UniquifiedModule();

    CHECK_VALUE_NOT_NULL(instanceModule, "Houps: Instance is not associated with an uniquified module");

    if (associatedNode != nullptr)  // Standard case is that for each instance there is an associated chain in SystemModel but for some (very simple) there are none
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
          auto scanRegister = FollowSignalTilScanRegister(instanceModule, signal);
          if (scanRegister != nullptr)
          {
            virtualReg.Append(MakeRegisterSlice(scanRegister, signal));
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
//  End of: AST_AliasConverter::ConvertAliases
//---------------------------------------------------------------------------



// Follows a signal until it reaches a ScanRegister
//
// @param module  Module from which signal to follow is defined
// @param signal  Signal to follow
//
// @return  Found ScanRegister in case of success, nullptr otherwise
//
AST_ScanRegister* AST_AliasConverter::FollowSignalTilScanRegister (AST_Module* module, AST_Signal* signal)
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

    return FollowSignalTilScanRegister(parentModule, sourceSignals.front());
  }

  auto dataOutPort = currentModule->FindDataOutPort(identifier);
  if (dataOutPort != nullptr)
  {
    auto source = dataOutPort->Source();
    if (source != nullptr)
    {
      const auto& sourceSignals = source->Signals();
      CHECK_VALUE_EQ(sourceSignals.size(), 1u, "While processing alias, can only deal with scalar signals");

      return FollowSignalTilScanRegister(currentModule, sourceSignals.front());
    }
    else
    {
//+          CHECK_VALUE_NOT_NULL(source, "Instance \"" + instance->Name() + "\" output port \""s + dataOutPort->Name() + "\" has no source");
      return static_cast<AST_ScanRegister*>(nullptr);
    }
  }

  return static_cast<AST_ScanRegister*>(nullptr);
}
//
//  End of: AST_AliasConverter::FollowSignalTilScanRegister
//---------------------------------------------------------------------------



//! Creates a register slice for ScanRegister and signal
//!
RegisterSlice AST_AliasConverter::MakeRegisterSlice (AST_ScanRegister* scanRegister, AST_Signal* signal)
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
}
//
//  End of: AST_AliasConverter::MakeRegisterSlice
//---------------------------------------------------------------------------



//===========================================================================
// End of AST_AliasConverter.cpp
//===========================================================================
