//===========================================================================
//                           Remote_Loopback_Protocol.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Remote_Loopback_Protocol.hpp
//!
//! Declares Remote_Loopback_Protocol class
//!
//===========================================================================


#ifndef REMOTE_LOOPBACK_PROTOCOL_H__8F7FF1F5_9EBA_4A17_E8B6_20B6A814A9B__INCLUDED_
  #define REMOTE_LOOPBACK_PROTOCOL_H__8F7FF1F5_9EBA_4A17_E8B6_20B6A814A9B__INCLUDED_

#include "Remote_Protocol.hpp"

namespace mast
{
//! Acts as a Remote_Protocol by just looping back SUT data vector and doing nothing for reset
//!
class Remote_Loopback_Protocol : public Remote_Protocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Remote_Loopback_Protocol();

  Remote_Loopback_Protocol();

  //! Does action according to command and scan vector
  //!
  //! @param commandName  Command name (SIR, SDR, RST...)
  //! @param bitsCount    Number of valid bits in scan vector
  //! @param scanVector   Binary data to send to SUT (default is right aligned)
  //!
  //! @return data scanned out from SUT
  virtual std::pair<uint32_t, std::vector<uint8_t> >
  DoCallback(const std::string& commandName, uint32_t bitsCount, const std::vector<uint8_t>& toSutScanVector);

  //! Forces the ResetPort to be asserted on the target module
  //!
  //! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
  //!
  virtual void DoReset(bool doSynchronousReset);
};
//
//  End of Remote_Loopback_Protocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined REMOTE_LOOPBACK_PROTOCOL_H__8F7FF1F5_9EBA_4A17_E8B6_20B6A814A9B__INCLUDED_

//===========================================================================
// End of Remote_Loopback_Protocol.hpp
//===========================================================================
