//===========================================================================
//                           SVF_EmulationProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SVF_EmulationProtocol.hpp
//!
//! Declares SVF_EmulationProtocol class
//!
//===========================================================================


#ifndef SVF_EMULATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
  #define SVF_EMULATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_

#include "SVF_Player.hpp"
#include <fstream>
#include <string>
#include <experimental/string_view>

namespace mast
{
//! Implement loopback AccessInterfaceProtocol while logging SVF commands that
//! a real SVF protocol would issue
//!
class DLL_EXPORT SVF_EmulationProtocol final : public SVF_Player
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~SVF_EmulationProtocol() = default;
  SVF_EmulationProtocol() = default;

  //! Does any action required to transfer scan data to and from SUT
  //!
  //! @param derivationId   Identifies the derivation to act for (zero based)
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfer to SUT
  //!
  //! @return Bitstream retrieved from SUT
  //!
  virtual BinaryVector DoAction(uint32_t derivationId, void* interfaceData, const BinaryVector& toSutData) override;

  //! Gets the number of derivations supported by the specific protocol
  //!
  //! @note Derivation id 0 is reserved for reset operation, so protocol must support a least two derivations
  //!
  //! @return The number of supported derivation (including pseudo derivation 0 for reset)
  //!
  virtual uint32_t MaxSupportedDerivations() const override { return 3u; }

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "SVF_Emulation"; }

  //! Forces the ResetPort to be asserted on the target module
  //!
  virtual void DoReset(bool doSynchronousReset) override;

  // ---------------- Protected Methods
  //

  //! Logs SVF commands
  //!
  void LogCommands(std::experimental::string_view commands);
};
//
//  End of SVF_EmulationProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined SVF_EMULATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
//===========================================================================
// End of SVF_EmulationProtocol.hpp
//===========================================================================
