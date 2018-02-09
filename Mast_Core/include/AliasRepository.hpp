//===========================================================================
//                           AliasRepository.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AliasRepository.hpp
//!
//! Declares AliasRepository class
//!
//===========================================================================

#ifndef ALIASREPOSITORY_H__DFBF54A1_133F_48EB_FFA4_80AF51A19E3E__INCLUDED_
  #define ALIASREPOSITORY_H__DFBF54A1_133F_48EB_FFA4_80AF51A19E3E__INCLUDED_

#include "RegistersAlias.hpp"
#include <vector>
#include <experimental/string_view>

namespace mast
{
//! Provides collection of aliases
//!
//! @note It currently supports RegistersAlias
//!
class AliasRepository final
{
  // ---------------- Public Methods
  //
  public:
  ~AliasRepository() = default;
  AliasRepository()  = default;

  //! Returns true when there is at least one defined alias
  //!
  bool  HasAliases() const { return !m_registersAliases.empty(); }

  //! Appends a new alias to the collection of managed aliases
  //!
  void Append(RegistersAlias&& alias);

  //! Returns current aliases for (virtual) registers
  //!
  const std::vector<RegistersAlias>& RegistersAliases() const { return m_registersAliases; }

  //! Returns current aliases for (virtual) registers
  //!
  std::vector<RegistersAlias>& RegistersAliases() { return m_registersAliases; }


  //! Tries to find a RegistersAlias given given alias name
  //!
  RegistersAlias* FindRegisterAlias(std::experimental::string_view aliasName);

  //! Tries to find a Registers or VirtualRegister given an alias name
  //!
  RegisterInterface* FindRegister(std::experimental::string_view aliasName);


  // ---------------- Private Fields
  //
  private:
  std::vector<RegistersAlias> m_registersAliases;   //!< Aliases for (virtual) registers
};
//
//  End of AliasRepository class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined ALIASREPOSITORY_H__DFBF54A1_133F_48EB_FFA4_80AF51A19E3E__INCLUDED_

//===========================================================================
// End of AliasRepository.hpp
//===========================================================================
