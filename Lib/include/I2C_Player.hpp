//===========================================================================
//                           I2C_Player.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file I2C_Player.hpp
//!
//! Declares I2C_Player class
//!
//===========================================================================


#ifndef I2C_PLAYER_H__8B42501C_8D39_4E13_938C_F7447B1DC5C3__INCLUDED_
  #define I2C_PLAYER_H__8B42501C_8D39_4E13_938C_F7447B1DC5C3__INCLUDED_

#include "AccessInterfaceProtocol.hpp"
#include "Platform.hpp"

#include <string>
#include <vector>
#include <initializer_list>


namespace mast
{
//! Constructs I2C commands for I2C related AccessInterfaceProtocol
//!
class DLL_EXPORT I2C_Player : public AccessInterfaceProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~I2C_Player() = default;
  I2C_Player() = delete;

  I2C_Player(std::initializer_list<uint32_t> addresses, std::experimental::string_view commandsPrefix = "");
  I2C_Player(std::vector<uint32_t>           addresses, std::experimental::string_view commandsPrefix = "");


  std::string CommandsPrefix() const { return m_commandPrefix; }  //!< Returns current commands prefix

  void  CommandsPrefix (std::string commandsPrefix) { m_commandPrefix = commandsPrefix; } //!< Changes current commands prefix (must include spaces when necessary)

  // ---------------- Protected Methods
  //
  protected:

  //! Creates an I2C command associated to derivation identifier and BinaryVector to send to SUT
  //!
  std::string CreateI2CCommand(uint32_t derivationId, const BinaryVector& toSutData);

  //! Returns address for specified derivation
  //!
  uint32_t GetAddress(uint32_t derivationId) const;

  // ---------------- Private  Fields
  //
  std::string           m_commandPrefix; //!< Text leading I2C command (mainly used for logs in order to ease grep regex)
  std::vector<uint32_t> m_addresses;     //!< Addresses associated with derivation ids (at offset 1 for derivation 1)
};
//
//  End of I2C_Player class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined I2C_PLAYER_H__8B42501C_8D39_4E13_938C_F7447B1DC5C3__INCLUDED_

//===========================================================================
// End of I2C_Player.hpp
//===========================================================================
