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
#include "BinaryVector.hpp"

namespace mast
{
class SystemModel;

//! Represents a register in the scan chain
//!
//!
class DLL_EXPORT Register : public SystemModelNode
{
  // ---------------- Public  Methods
  //
  public:
  ~Register() = default;
  Register()  = delete;
  Register(std::experimental::string_view name, mast::BinaryVector bypassSequence);
  friend SystemModel;

  // ---------------- Miscellaneous
  //
  virtual void Accept (SystemModelVisitor& visitor) override; //!< Visited part of the Visitor pattern

  virtual std::experimental::string_view TypeName() const override { return "Register"; } //!< Returns readable type name

  //!< Checks that last received sequence equals expected one
  //!<
  //!< @note  It increments internal mismatch count in case of failure
  //!<
  //!< @return  true when received sequence equals expected one, false otherwise
  bool CheckAgainstExpected();

  // ---------------- Getters
  //
  const BinaryVector& BypassSequence()    const { return m_bypass;            } //!< Returns bypass sequence
  const BinaryVector& ExpectedFromSut()   const { return m_expectedFromSut;   } //!< Returns expected sequence
  const BinaryVector& LastFromSut()       const { return m_lastFromSut;       } //!< Returns last sequence received from SUT
  const BinaryVector& NextToSut()         const { return m_nextToSut;         } //!< Returns next sequence to send to SUT
  const BinaryVector& LastToSut()         const { return m_lastToSut;         } //!< Returns last sequence effectively sent to SUT
  bool                MustCheckExpected() const { return m_mustCheckExpected; } //!< Returns true when received data must be checked against expected data
  uint32_t            Mismatches()        const { return m_mismatches;        } //!< Returns current mismatch count

  // ---------------- Setters
  //
  void SetFromSut         (BinaryVector sequence);                                             //!< Sets last sequence of bits that have been shifted from SUT
  void SetToSut           (BinaryVector sequence) { m_nextToSut       = std::move(sequence); } //!< Sets the bits sequence to send during the next iApply cycle
  void SetExpectedFromSut (BinaryVector sequence) { m_expectedFromSut = std::move(sequence); } //!< Sets expected sequence (when updating from SUT)
  void SetBypass          (BinaryVector sequence) { m_bypass          = std::move(sequence); } //!< Sets sequence to shift into the sut when no iApply cycle has been defined on the register

  void SetCheckExpected   (bool checkExpected) { m_mustCheckExpected  = checkExpected; } //!< Sets whether data updated from SUT must be check agains expected data
  void ResetMismatches    ()                   { m_mismatches = 0; }                     //!< Clears the mismatch count

  // ---------------- Private  Fields
  //
  private:
  bool         m_pendingSelect     = false; //!< True when the tdr value has been changed following a selection action
  bool         m_mustCheckExpected = false; //!< When true, it triggers a check of received vs expected data during the following shift from sut
  uint32_t     m_mismatches        = 0;     //!< Number of mismatches following IEEE 1687 rules
  BinaryVector m_nextToSut;                 //!< Sequence of bits that should be shifted into SUT (during the next iApply cycle)
  BinaryVector m_lastToSut;                 //!< Last sent sequence of bits: It stores the status of the SUT (SIBs, etc...) after an apply cycle
  BinaryVector m_lastFromSut;               //!< Last sequence of bits that have been shifted from SUT
  BinaryVector m_expectedFromSut;           //!< Sequence of expected bits when scanning from SUT
  BinaryVector m_bypass;                    //!< Sequence to shift into the sut when no iApply cycle has been defined on the register
};
//
//  End of Register class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined REGISTER_H__AC94642C_6C5D_4241_B69A_C5DE634D3EAA__INCLUDED_

//===========================================================================
// End of Register.hpp
//===========================================================================
