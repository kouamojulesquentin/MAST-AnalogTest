//===========================================================================
//                           Remote_Protocol.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Remote_Protocol.hpp
//!
//! Declares Remote_Protocol class
//!
//===========================================================================


#ifndef REMOTE_PROTOCOL_H__81CCF51A_A12C_4F30_6095_3BF74A4C80D__INCLUDED_
  #define REMOTE_PROTOCOL_H__81CCF51A_A12C_4F30_6095_3BF74A4C80D__INCLUDED_

#include <vector>
#include <string>
#include <utility>
#ifndef uint32_t
#include <stdint.h>     // Make it compilable as C++03
#endif

namespace mast
{
//! Represents a remote access interface protocol
//!
//! @note This is an abstract base class
//! @note MUST be compilable by C++03 only compilers !
class Remote_Protocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Remote_Protocol() {}
  Remote_Protocol()  {}

  typedef std::pair<uint32_t, std::vector<uint8_t> > DoActionReturn_t;

  //! Does action according to command and scan vector
  //!
  //! @param commandName  Command name (SIR, SDR, RST...)
  //! @param bitsCount    Number of valid bits in scan vector
  //! @param scanVector   Binary data to send to SUT (default is right aligned)
  //!
  //! @return data scanned out from SUT
  virtual DoActionReturn_t
  DoAction(const std::string& commandName, uint32_t bitsCount, const std::vector<uint8_t>& toSutScanVector) = 0;

  //! Forces the ResetPort to be asserted on the target module
  //!
  //! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
  //!
  virtual void DoReset(bool doSynchronousReset) = 0;
};
//
//  End of Remote_Protocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined REMOTE_PROTOCOL_H__81CCF51A_A12C_4F30_6095_3BF74A4C80D__INCLUDED_

//===========================================================================
// End of Remote_Protocol.hpp
//===========================================================================
