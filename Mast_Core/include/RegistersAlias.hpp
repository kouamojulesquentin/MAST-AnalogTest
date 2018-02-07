//===========================================================================
//                           RegistersAlias.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file RegistersAlias.hpp
//!
//! Declares RegistersAlias class
//!
//===========================================================================

#ifndef REGISTERSALIAS_H__8ED126BC_2EEC_4446_5B5_B23B362B92D5__INCLUDED_
  #define REGISTERSALIAS_H__8ED126BC_2EEC_4446_5B5_B23B362B92D5__INCLUDED_

#include "VirtualRegister.hpp"
#include <string>

namespace mast
{
//! A named virtual registers - chunk of register(s)
//!
//! This is used to group registers bits into some kind of buses
//!
class RegistersAlias final
{
  // ---------------- Public Methods
  //
  public:
  ~RegistersAlias() = default;
  RegistersAlias()  = delete;

  //! Constructs a RegistersAlias
  //!
  //! @param baseName         Alias name (without any indices) - must be non empty
  //! @param virtualRegister  Associated virtual register - must be not empty
  //!
  RegistersAlias(std::string&& baseName, VirtualRegister&& virtualRegister);

  //! Returns alias name (without nay indices)
  //!
  const std::string& Name() const { return m_baseName; }

  //! Returns aliased registers bits as a VirtualRegister
  //!
  VirtualRegister& AliasedRegisters() { return m_virtualRegister; }

  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private Methods
  //
  private:

  // ---------------- Private Fields
  //
  private:
  const std::string m_baseName;        //!< Alias name (without ICL optional bits range)
  VirtualRegister   m_virtualRegister; //!< Virtual register associated with that alias (several alias can refer to the same register bits)
};
//
//  End of RegistersAlias class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined REGISTERSALIAS_H__8ED126BC_2EEC_4446_5B5_B23B362B92D5__INCLUDED_

//===========================================================================
// End of RegistersAlias.hpp
//===========================================================================
