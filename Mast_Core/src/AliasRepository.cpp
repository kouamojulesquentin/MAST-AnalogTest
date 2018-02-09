//===========================================================================
//                           AliasRepository.cpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AliasRepository.cpp
//!
//! Implements class AliasRepository
//!
//===========================================================================

#include "AliasRepository.hpp"
#include <algorithm>

using namespace mast;
using std::experimental::string_view;


//! Appends a alias for register(s)
//!
//! @param alias  "Register" alias to add to collection of aliases
//!
void AliasRepository::Append (RegistersAlias&& alias)
{
  CHECK_VALUE_NULL(FindRegisterAlias(alias.Name()), "Have already an alias with name: \""s + alias.Name() + "\"");

  m_registersAliases.emplace_back(std::move(alias));
}
//
//  End of: AliasRepository::Append
//---------------------------------------------------------------------------


//! Tries to find a Registers or VirtualRegister given an alias name
//!
//! @param aliasName  Alias name for the Register/VirtualRegister
//!
//! @return Found Register/VirtualRegister or nullptr when not found
RegisterInterface* AliasRepository::FindRegister (string_view aliasName)
{
  auto alias = FindRegisterAlias(aliasName);

  if (alias != nullptr)
  {
    return &alias->AliasedRegisters();
  }

  return nullptr;
}
//
//  End of: AliasRepository::FindRegister
//---------------------------------------------------------------------------



//! Tries to find a RegistersAlias given given alias name
//!
//! @param aliasName  Name of alias to look for
//!
//! @return found alias or nullptr when not found
RegistersAlias* AliasRepository::FindRegisterAlias (string_view aliasName)
{
  auto pos = std::find_if(m_registersAliases.begin(),
                          m_registersAliases.end(),
                          [aliasName](const auto& alias) { return alias.Name() == aliasName; });

  if (pos != m_registersAliases.end())
  {
    return &*pos;
  }

  return nullptr;
}
//
//  End of: AliasRepository::FindRegisterAlias
//---------------------------------------------------------------------------



//===========================================================================
// End of AliasRepository.cpp
//===========================================================================
