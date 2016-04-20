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
  using RegisterBits = std::vector<uint8_t>;

  ~Register() = default;
  Register()  = delete;
  Register(std::string name, uint32_t bitsCount, RegisterBits bypassSequence);


  void                SetSequenceToSend (RegisterBits sequenceToSend) { m_sequenceToSend = sequenceToSend; }  //!< Sets the bits sequence to send during the next iApply cycle
  const RegisterBits& GetLastReceivedSequence() const { return m_lastReceivedSequence; }

  virtual void Accept (SystemModelVisitor& visitor) override; //!< Visited part of the Visitor pattern

  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:

  const uint32_t     m_bitsCount            = 0;       //!< Exact number of bits in the register
  const uint32_t     m_bytesCount           = 0;       //!< Number of bytes necessary to represent all the bytes
  bool               m_pendingSelect        = false;   //!< True when the tdr value has been changed following a selection action
  bool               m_checkExpected        = false;   //!< When true, it triggers a check of received vs expected data during the following shift from sut
  uint32_t           m_priority             = 0;    //!< Defines priority for configuration
  uint32_t           m_mismatches           = 0;       //!< Number of mismatches following IEEE 1687 rules
  RegisterBits       m_sequenceToSend;                 //!< Sequence of bits that should be shifted into SUT (during the next iApply cycle)
  RegisterBits       m_lastSentSequence;               //!< Last sent sequence of bits: It stores the status of the SUT (SIBs, etc...) after an apply cycle
  RegisterBits       m_lastReceivedSequence;           //!< Last sequence of bits that have been shifted from SUT
  RegisterBits       m_expectedSequence;               //!< Sequence of expected bits when scanning from SUT
  const RegisterBits m_bypassSequence;                 //!< Sequence to shift into the sut when no iApply cycle has been defined on the register
  void*              m_applicationData      = nullptr; //!< Application specific data (semantic managed by the application)
};
//
//  End of Register class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined REGISTER_H__AC94642C_6C5D_4241_B69A_C5DE634D3EAA__INCLUDED_

//===========================================================================
// End of Register.hpp
//===========================================================================
