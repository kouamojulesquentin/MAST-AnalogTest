//===========================================================================
//                           Advantest_SmartRDI_Protocol.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Advantest_SmartRDI_Protocol.hpp
//!
//! Declares Advantest_SmartRDI_Protocol class
//!
//===========================================================================

#ifndef ADVANTEST_SMARTRDI_PROTOCOL_H__57EB270_A896_48AA_81BF_136BDC95B1C__INCLUDED_
  #define ADVANTEST_SMARTRDI_PROTOCOL_H__57EB270_A896_48AA_81BF_136BDC95B1C__INCLUDED_

#include "Remote_Protocol.hpp"

namespace mast
{
//! Implement Remote_Protocol for ADVANTEST SmartRDI
//!
class Advantest_SmartRDI_Protocol : public Remote_Protocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Advantest_SmartRDI_Protocol();

  Advantest_SmartRDI_Protocol();

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
//  End of Advantest_SmartRDI_Protocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined ADVANTEST_SMARTRDI_PROTOCOL_H__57EB270_A896_48AA_81BF_136BDC95B1C__INCLUDED_
//===========================================================================
// End of Advantest_SmartRDI_Protocol.hpp
//===========================================================================
