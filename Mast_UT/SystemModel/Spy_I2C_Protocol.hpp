//===========================================================================
//                           Spy_I2C_Protocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Spy_I2C_Protocol.hpp
//!
//! Declares Spy_I2C_Protocol class
//!
//===========================================================================


#ifndef SPY_I2C_PROTOCOL_H__D5A21174_691E_49F5_4CB1_629C102E6DB9__INCLUDED_
  #define SPY_I2C_PROTOCOL_H__D5A21174_691E_49F5_4CB1_629C102E6DB9__INCLUDED_

#include "I2C_Player.hpp"
#include "BinaryVector.hpp"

namespace test
{
//! Spies calls to I2C based AccessInterfaceProtocol
//!
class Spy_I2C_Protocol final : public mast::I2C_Player
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Spy_I2C_Protocol() = default;
  Spy_I2C_Protocol() = delete;
  Spy_I2C_Protocol(std::initializer_list<uint32_t> addresses, std::experimental::string_view commandsPrefix = "")
    : I2C_Player(addresses, commandsPrefix)
  {  }

  //! Spies content how binary vector to SUT is transformed to I2C command while returning the BinaryVector unchanged
  //!
  virtual mast::BinaryVector DoAction(uint32_t derivationId, void* interfaceData, const mast::BinaryVector& toSutData) override;

  //! Gets the number of derivations supported by the specific protocol
  //!
  //! @note Derivation id 0 is reserved for reset operation, so protocol must support a least two derivations
  //!
  //! @return The number of supported derivation (including pseudo derivation 0 for reset)
  //!
  virtual uint32_t MaxSupportedDerivations() const override { return 2u; }

  const std::vector<std::string>& I2CCommands() const { return m_commands; }

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "I2C_Spy"; }

  //! Forces the ResetPort to be asserted on the target module
  //!
  virtual void DoReset(bool doSynchronousReset) override;

  // ---------------- Private  Fields
  //
  private:
  std::vector<std::string> m_commands; //!< Collected I2C commands issued from vectors "send" to SUT by SystemModelManager
  uint32_t                 m_resetCount = 0;
};
//
//  End of Spy_I2C_Protocol class declaration
//---------------------------------------------------------------------------
} // End of namespace test


#endif  // not defined SPY_I2C_PROTOCOL_H__D5A21174_691E_49F5_4CB1_629C102E6DB9__INCLUDED_

//===========================================================================
// End of Spy_I2C_Protocol.hpp
//===========================================================================
