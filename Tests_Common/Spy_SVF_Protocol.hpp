//===========================================================================
//                           Spy_SVF_Protocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Spy_SVF_Protocol.hpp
//!
//! Declares Spy_SVF_Protocol class
//!
//===========================================================================


#ifndef SPY_SVF_PROTOCOL_H__7720E2EF_FEE3_4D05_C4B6_A5E4CB999B6__INCLUDED_
  #define SPY_SVF_PROTOCOL_H__7720E2EF_FEE3_4D05_C4B6_A5E4CB999B6__INCLUDED_

#include "SVF_Player.hpp"
#include "SpiedProtocolsCommands.hpp"
#include "BinaryVector.hpp"

#include <memory>
#include <string>

namespace test
{
//! Spies calls to SVF based AccessInterfaceProtocol
//!
class Spy_SVF_Protocol final : public mast::SVF_Player
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Spy_SVF_Protocol() = default;
  Spy_SVF_Protocol(std::shared_ptr<SpiedProtocolsCommands> spiedCommands)
    : m_spiedCommands(spiedCommands) {}
  Spy_SVF_Protocol()
    : m_spiedCommands(std::make_shared<SpiedProtocolsCommands>()) {}

  //! Spies content how binary vector to SUT is transformed to SVF command while returning the BinaryVector unchanged
  //!
  virtual mast::BinaryVector DoCallback(std::string CallbackId, uint32_t channelId, void* interfaceData, const mast::BinaryVector& toSutData) override;

  const std::vector<std::string>& SVFCommands() const { return m_spiedCommands->Commands(); }

  //! Gets the number of channels supported by the specific protocol
  //!
  //! @note Channel id 0 is reserved for reset operation, so protocol must support a least two channels
  //!
  //! @return The number of supported channel (including pseudo channel 0 for reset)
  //!
  virtual uint32_t MaxSupportedChannels() const override { return 3u; }

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "SVF_Spy"; }

  //! Forces the ResetPort to be asserted on the target module
  //!
  virtual void DoReset(bool doSynchronousReset) override;

  // ---------------- Private  Methods
  //
  //! Saves SVF commands
  //!
  void SaveCommands(std::experimental::string_view commands);

  // ---------------- Private  Fields
  //
  private:
  std::shared_ptr<SpiedProtocolsCommands> m_spiedCommands; //!< Collected SVF commands issued from vectors "send" to SUT by SystemModelManager
};
//
//  End of Spy_SVF_Protocol class declaration
//---------------------------------------------------------------------------
} // End of namespace test


#endif  // not defined SPY_SVF_PROTOCOL_H__7720E2EF_FEE3_4D05_C4B6_A5E4CB999B6__INCLUDED_

//===========================================================================
// End of Spy_SVF_Protocol.hpp
//===========================================================================
