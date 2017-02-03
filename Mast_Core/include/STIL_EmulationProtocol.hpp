//===========================================================================
//                           STIL_EmulationProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file STIL_EmulationProtocol.hpp
//!
//! Declares STIL_EmulationProtocol class
//!
//===========================================================================


#ifndef STIL_EMULATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
  #define STIL_EMULATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_

#include "STIL_Player.hpp"
#include <fstream>
#include <string>
#include <experimental/string_view>

namespace mast
{
//! Implement loopback AccessInterfaceProtocol while logging STIL commands that
//! a real STIL protocol would issue
//!
class DLL_EXPORT STIL_EmulationProtocol final : public STIL_Player
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~STIL_EmulationProtocol() = default;
  STIL_EmulationProtocol() = delete;
  STIL_EmulationProtocol(uint32_t n_chains);

  //! Initializes a with number of derivations defined by a string
  //!
  STIL_EmulationProtocol(const std::string& nbChains);

  //! Does any action required to transfer scan data to and from SUT
  //!
  //! @param derivationId   Identifies the derivation to act for (zero based)
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfer to SUT
  //!
  //! @return Bitstream retrieved from SUT
  //!
  virtual BinaryVector DoAction(uint32_t derivationId, void* interfaceData, const BinaryVector& toSutData) override;


  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "STIL_Emulation"; }

  //! Forces the ResetPort to be asserted on the target module
  //!
  virtual void DoReset(bool doSynchronousReset) override;

  // ---------------- Protected Methods
  //

  //! Logs STIL commands
  //!
  void LogCommands(std::experimental::string_view commands);
};
//
//  End of STIL_EmulationProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined STIL_EMULATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
//===========================================================================
// End of STIL_EmulationProtocol.hpp
//===========================================================================
