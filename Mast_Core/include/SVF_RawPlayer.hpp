//===========================================================================
//                           SVF_RawPlayer.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SVF_RawPlayer.hpp
//!
//! Declares SVF_RawPlayer class
//!
//===========================================================================


#ifndef SVF_RawPlayer_H__CE0DE4BD_2B23_4BD0_82B1_DCE5A1CD5B2__INCLUDED_
  #define SVF_RawPlayer_H__CE0DE4BD_2B23_4BD0_82B1_DCE5A1CD5B2__INCLUDED_

#include "AccessInterfaceRawProtocol.hpp"
#include "Mast_Core_export.hpp"
#include "CallbackIds.hpp"

#include <string>
#include <memory>


namespace mast
{
#define SIR_OVERHEAD_CYCLES 		6
#define SDR_OVERHEAD_CYCLES 		5
#define nTRST_OVERHEAD_CYCLES 		3

//! Constructs SVF commands for SVF related AccessInterfaceProtocol
//!
class MAST_CORE_EXPORT SVF_RawPlayer final: public AccessInterfaceRawProtocol
{
  // ---------------- Public  Methods
  //
  public:
  ~SVF_RawPlayer() = default;
  SVF_RawPlayer()  = default;

  //! Returns true when TRST JTAG port is supported
  //!
  bool  SupportTRST() const { return m_supportTRST; }

  //! Sets whether TRST JTAG port is supported
  //!
  void  SupportTRST (bool supportTRST) { m_supportTRST = supportTRST; }


  //! readable identifier for given channel
  //!
  std::string CallbackForChannel(int Channel) const { 
     switch (Channel){
       case 0: return TRST; 
       case 1: return SIR; 
       case 2: return SDR; 
       default: return UNDEFINED;
       }
      }

  //! Does any callback required to transfer scan data to and from SUT
  //!
  //! @param channelId   Identifies the channel to act for (zero based)
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfer to SUT
  //!
  //! @return Bitstream retrieved from SUT
  //!
  virtual BinaryVector DoCallback(RVFRequest Request, uint32_t channelId) override;

  //! Gets the number of channels supported by the specific protocol
  //!
  //! @note Channel id 0 is reserved for reset operation, so protocol must support a least two channels
  //!
  //! @return The number of supported channel (including pseudo channel 0 for reset)
  //!
  virtual uint32_t MaxSupportedChannels() const override { return 3u; }

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "SVF_RAW"; }


  //! Forces the ResetPort to be asserted on the target module
  //!
  virtual void DoReset(bool doSynchronousReset) override;

  // ---------------- Protected Methods
  //

  // ---------------- Private  Fields
  //
  bool m_supportTRST = false; //!< When true, TRST instruction is used for reset unless synchronous reset is requested
};
//
//  End of SVF_RawPlayer class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SVF_RawPlayer_H__CE0DE4BD_2B23_4BD0_82B1_DCE5A1CD5B2__INCLUDED_

//===========================================================================
// End of SVF_RawPlayer.hpp
//===========================================================================
