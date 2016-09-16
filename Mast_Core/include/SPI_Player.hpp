//===========================================================================
//                           SPI_Player.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SPI_Player.hpp
//!
//! Declares SPI_Player class
//!
//===========================================================================


#ifndef SPI_PLAYER_H__8B42501C_8D39_4E13_938C_F7447B1DC5C3__INCLUDED_
  #define SPI_PLAYER_H__8B42501C_8D39_4E13_938C_F7447B1DC5C3__INCLUDED_

#include "AccessInterfaceProtocol.hpp"
#include "Platform.hpp"

#include <string>
#include <vector>
#include <initializer_list>


namespace mast
{
//! Constructs SPI commands for SPI related AccessInterfaceProtocol
//!
class DLL_EXPORT SPI_Player : public AccessInterfaceProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~SPI_Player() = default;
  SPI_Player() = delete;

  SPI_Player(std::initializer_list<uint32_t> chipSelectCommands, std::initializer_list<uint32_t> readCommands, std::initializer_list<uint32_t> writeCommands, std::experimental::string_view commandsPrefix = "");
  SPI_Player(std::vector<uint32_t> chipSelectCommands, std::vector<uint32_t> readCommands, std::vector<uint32_t> writeCommands, std::experimental::string_view commandsPrefix = "");

  //! Gets the number of derivations supported by the specific protocol
  //!
  //! @note Derivation id 0 is reserved for reset operation, so protocol must support a least two derivations
  //!
  //! @return The number of supported derivation (including pseudo derivation 0 for reset)
  //!
  virtual uint32_t MaxSupportedDerivations() const override { return (m_readCommands.size() > m_writeCommands.size()) ? m_readCommands.size(): m_writeCommands.size(); }

  std::string CommandsPrefix() const { return m_commandPrefix; }  //!< Returns current commands prefix

  void  CommandsPrefix (std::string commandsPrefix) { m_commandPrefix = commandsPrefix; } //!< Changes current commands prefix (must include spaces when necessary)

  // ---------------- Protected Methods
  //
  protected:

  //! Creates an SPI command associated to derivation identifier and BinaryVector to send to SUT
  //!
  std::string CreateSPICommand(uint32_t derivationId, const BinaryVector& toSutData);

	//! Returns the chip select command for a specified derivation
  //!
  uint32_t GetChipSelectCommand(uint32_t derivationId) const;

  //! Returns read command for a specified derivation
  //!
  uint32_t GetReadCommand(uint32_t derivationId) const;

  //! Returns write command for a specified derivation
  //!
  uint32_t GetWriteCommand(uint32_t derivationId) const;

  // ---------------- Private  Fields
  //
  std::string           m_commandPrefix; //!< Text leading SPI command (mainly used for logs in order to ease grep regex)
	std::vector<uint32_t> m_chipSelectCommands;		 //!< Chip select (GPIOs) combination associated with derivation ids (at offset 1 for derivation 1). If the chip select value for a derivation is zero, it is assumed the adapter has one embedded chip select in its SPI module, not passing by GPIOs.
  std::vector<uint32_t> m_readCommands;  //!< Read commands associated with derivation ids (at offset 1 for derivation 1)
  std::vector<uint32_t> m_writeCommands; //!< Write commands associated with derivation ids (at offset 1 for derivation 1)
};
//
//  End of SPI_Player class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined SPI_PLAYER_H__8B42501C_8D39_4E13_938C_F7447B1DC5C3__INCLUDED_

//===========================================================================
// End of SPI_Player.hpp
//===========================================================================
