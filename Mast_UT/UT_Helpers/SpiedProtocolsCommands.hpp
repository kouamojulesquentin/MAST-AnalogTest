//===========================================================================
//                           SpiedProtocolsCommands.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SpiedProtocolsCommands.hpp
//!
//! Declares SpiedProtocolsCommands class
//!
//===========================================================================


#ifndef SPIEDPROTOCOLSCOMMANDS_H__69515CB4_156B_45F1_6183_A7BFB22E1E5E__INCLUDED_
  #define SPIEDPROTOCOLSCOMMANDS_H__69515CB4_156B_45F1_6183_A7BFB22E1E5E__INCLUDED_

namespace test
{
//! Collects spied AccessInterfaceProtocol commands (as string)
//!
class SpiedProtocolsCommands final
{
  // ---------------- Public  Methods
  //
  public:
  ~SpiedProtocolsCommands() = default;
  SpiedProtocolsCommands()  = default;

  const std::vector<std::string>& Commands() const { return m_commands; }

  void SaveCommand(std::string command) { m_commands.emplace_back(std::move(command)); }

  // ---------------- Private  Fields
  //
  private:
  std::vector<std::string> m_commands; //!< Collected commands issued from vectors "send" to SUT by SystemModelManager
};
//
//  End of SpiedProtocolsCommands class declaration
//---------------------------------------------------------------------------
} // End of namespace test




#endif  // not defined SPIEDPROTOCOLSCOMMANDS_H__69515CB4_156B_45F1_6183_A7BFB22E1E5E__INCLUDED_

//===========================================================================
// End of SpiedProtocolsCommands.hpp
//===========================================================================
