//===========================================================================
//                           Spy_STIL_Protocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Spy_STIL_Protocol.hpp
//!
//! Declares Spy_STIL_Protocol class
//!
//===========================================================================


#ifndef SPY_STIL_PROTOCOL_H__7720E2EF_FEE3_4D05_C4B6_A5E4CB999B6__INCLUDED_
  #define SPY_STIL_PROTOCOL_H__7720E2EF_FEE3_4D05_C4B6_A5E4CB999B6__INCLUDED_

#include "STIL_Player.hpp"
#include "SpiedProtocolsCommands.hpp"
#include "BinaryVector.hpp"

#include <memory>
#include <string>

namespace test
{
//! Spies calls to STIL based AccessInterfaceProtocol
//!
class Spy_STIL_Protocol final : public mast::STIL_Player
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Spy_STIL_Protocol() = default;
  Spy_STIL_Protocol(std::shared_ptr<SpiedProtocolsCommands> spiedCommands,uint32_t n_chains)
    : STIL_Player(n_chains) {m_spiedCommands = spiedCommands;}
  Spy_STIL_Protocol(uint32_t n_chains)
    : STIL_Player(n_chains) {m_spiedCommands = std::make_shared<SpiedProtocolsCommands>();}

  //! Spies content how binary vector to SUT is transformed to STIL command while returning the BinaryVector unchanged
  //!
  virtual mast::BinaryVector DoCallback(uint32_t endpointId, void* interfaceData, const mast::BinaryVector& toSutData) override;

  const std::vector<std::string>& STILCommands() const { return m_spiedCommands->Commands(); }

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "STIL_Spy"; }

  //! Forces the ResetPort to be asserted on the target module
  //!
  virtual void DoReset(bool doSynchronousReset) override;

  // ---------------- Private  Methods
  //
  //! Saves STIL commands
  //!
  void SaveCommands(std::experimental::string_view commands);

  //! Resets STIL commands list
  //!
  //! @param commands New line separated commands
  //!
  void resetCommands ();

  // ---------------- Private  Fields
  //
  private:
  std::shared_ptr<SpiedProtocolsCommands> m_spiedCommands; //!< Collected STIL commands issued from vectors "send" to SUT by SystemModelManager
};
//
//  End of Spy_STIL_Protocol class declaration
//---------------------------------------------------------------------------
} // End of namespace test


#endif  // not defined SPY_STIL_PROTOCOL_H__7720E2EF_FEE3_4D05_C4B6_A5E4CB999B6__INCLUDED_

//===========================================================================
// End of Spy_STIL_Protocol.hpp
//===========================================================================
