//===========================================================================
//                           Register.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Register.hpp
//!
//! Declares class Register
//!
//===========================================================================

#ifndef REGISTER_H__AC94642C_6C5D_4241_B69A_C5DE634D3EAA__INCLUDED_
  #define REGISTER_H__AC94642C_6C5D_4241_B69A_C5DE634D3EAA__INCLUDED_

#include "SystemModelNode.hpp"
#include <vector>

namespace mast
{
//! Represents a register in the scan chain
//!
//!
class Register : public SystemModelNode
{
  // ---------------- Public  Methods
  //
  public:
  ~Register() = default;
  Register()  = default;


  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
  using RegisterBits = std::vector<unsigned char>;

  const uint32_t m_bitsCount            = 0; //!< Exact number of bits in the register
  const uint32_t m_bytesCount           = 0; //!< Number of bytes necessary to represent all the bytes
  bool           m_manageAging          = false;
  uint32_t       m_pendingSelectCount   = 0;
  bool           m_checkExpected        = false;
  bool           m_checkExpectedFailure = false;
  RegisterBits   m_sequenceToSend;           //!< Sequence of bits that should be shifted out to SUT
  RegisterBits   m_lastSentSequence;         //!< Last sequence of bits that have already been changed
  RegisterBits   m_lastReceivedSequence;     //!< Last sequence of bits that have been shifted in from SUT
  RegisterBits   m_expectedSequence;         //!< Sequence of expected bits when scanning from SUT
};
//
//  End of Register class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined REGISTER_H__AC94642C_6C5D_4241_B69A_C5DE634D3EAA__INCLUDED_

//===========================================================================
// End of Register.hpp
//===========================================================================
