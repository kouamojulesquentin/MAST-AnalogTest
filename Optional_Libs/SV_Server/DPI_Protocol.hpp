//===========================================================================
//                           DPI_Protocol.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DPI_Protocol.hpp
//!
//! Declares DPI_Protocol class
//!
//===========================================================================


#ifndef DPI_PROTOCOL_H__8F7FF1F5_9EBA_4A17_E8B6_20B6A814A9B__INCLUDED_
  #define DPI_PROTOCOL_H__8F7FF1F5_9EBA_4A17_E8B6_20B6A814A9B__INCLUDED_

#include "Remote_Protocol.hpp"
#include <map>
using std::string;
using std::vector;
using std::pair;

using std::make_pair;

enum SVF_COMMANDS {TRST, SIR, SDR};

namespace mast
{
//! Acts as a Remote_Protocol by just looping back SUT data vector and doing nothing for reset
//!
class DPI_Protocol : public Remote_Protocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~DPI_Protocol();

  DPI_Protocol();
  
  int findCallbackId(std::string commandname);
  //! Does action according to command and scan vector
  //!
  //! @param commandName  Command name (SIR, SDR, RST...)
  //! @param bitsCount    Number of valid bits in scan vector
  //! @param scanVector   Binary data to send to SUT (default is right aligned)
  //!
  //! @return data scanned out from SUT
//  virtual std::pair<uint32_t, std::vector<uint8_t> >
//  DoAction(const std::string& commandName, uint32_t bitsCount, const std::vector<uint8_t>& toSutScanVector);
  virtual Remote_Protocol::DoCallbackReturn_t
  DoCallback(const std::string& commandName, uint32_t bitsCount, const std::vector<uint8_t>& toSutScanVector);

  //! Forces the ResetPort to be asserted on the target module
  //!
  //! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
  //!
  virtual void DoReset(bool doSynchronousReset);
  
  private:
  std::map<std::string,int> SupportedCallbacks;
};
//
//  End of DPI_Protocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined DPI_PROTOCOL_H__8F7FF1F5_9EBA_4A17_E8B6_20B6A814A9B__INCLUDED_

//===========================================================================
// End of DPI_Protocol.hpp
//===========================================================================
