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
class MAST_CORE_EXPORT SVF_EmulationProtocol final : public SVF_Player
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~SVF_EmulationProtocol() = default;
  SVF_EmulationProtocol() = default;

  //! Does any callback required to transfer scan data to and from SUT
  //!
  //! @param channelId   Identifies the Channel to act for (zero based)
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfer to SUT
  //!
  //! @return Bitstream retrieved from SUT
  //!
  virtual BinaryVector DoCallback(uint32_t channelId, void* interfaceData, const BinaryVector& toSutData) override;

  //! Gets the number of Channel supported by the specific protocol
  //!
  //! @note Channel id 0 is reserved for reset operation, so protocol must support a least two Channels
  //!
  //! @return The number of supported Channels (including pseudo Channel 0 for reset)
  //!
  virtual uint32_t MaxSupportedChannels() const override { return 3u; }

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "SVF_Emulation"; }

  //! readable identifier for given Channel
  //!
  std::string CallbackId(int Channel) { 
     switch (Channel){
       case 0: return "TRSRT"; 
       case 1: return "SIR"; 
       case 2: return "SRD"; 
       default: return "UNDEFINED";
       }
      }

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
