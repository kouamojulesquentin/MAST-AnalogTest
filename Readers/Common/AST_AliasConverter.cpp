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
#include "AST_Helper.hpp"

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
        auto scanRegister = AST_Helper::FollowSignalTilScanRegister(module, signal);
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
    auto associatedNode = instance->AssociatedChain().get();
    auto instanceModule = instance->UniquifiedModule();

    CHECK_VALUE_NOT_NULL(instanceModule, "Houps: Instance is not associated with an uniquified module");

    if (associatedNode != nullptr)  // Standard case is that for each instance there is an associated chain in SystemModel but for some (very simple) there are none
    {
      ConvertAliases(instanceModule, associatedNode);
    }
    else if (instanceModule->HasAliases())
    {
      LOG(INFO) << "Trying to pull up aliases defined in instance \"" << instance->Name() << "\" into its parent module \"" << module->Name() << "\"" ;
      ConvertAliases(instanceModule, parentNode);
    }
  }
}
//
//  End of: AST_AliasConverter::ConvertAliases
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
