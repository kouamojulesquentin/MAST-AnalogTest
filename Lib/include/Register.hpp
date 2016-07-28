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
class DLL_EXPORT Register : public SystemModelNode
{
  // ---------------- Public  Methods
  //
  public:
  ~Register() = default;
  Register()  = delete;
  Register(std::experimental::string_view name, mast::BinaryVector bypassSequence, bool holdValue = false);
  friend SystemModel;

  // ---------------- Miscellaneous
  //
  virtual void Accept (SystemModelVisitor& visitor) override; //!< Visited part of the Visitor pattern

  virtual std::experimental::string_view TypeName() const override { return "Register"; } //!< Returns readable type name

  // ---------------- Getters
  //
  uint32_t            BitsCount()         const { return m_bypass.BitsCount(); } //!< Returns Register numbers of bits
  const BinaryVector& BypassSequence()    const { return m_bypass;             } //!< Returns bypass sequence
  const BinaryVector& ExpectedFromSut()   const { return m_expectedFromSut;    } //!< Returns expected sequence
  const BinaryVector& LastFromSut()       const { return m_lastFromSut;        } //!< Returns last sequence received from SUT
  const BinaryVector& LastReadFromSut()   const { return m_lastReadFromSut;    } //!< Returns last sequence received from SUT when it was in pending read state
  const BinaryVector& NextToSut()         const { return m_nextToSut;          } //!< Returns next sequence to send to SUT
  const BinaryVector& LastToSut()         const { return m_lastToSut;          } //!< Returns last sequence effectively sent to SUT
  bool                HoldValue()         const { return m_holdValue;          } //!< Returns true when bypass value is maintained equal to nextToSut (The value will not be changed while the register is selected)
  bool                MustCheckExpected() const { return m_mustCheckExpected;  } //!< Returns true when received data must be checked against expected data
  uint32_t            Mismatches()        const { return m_mismatches;         } //!< Returns current mismatch count
  bool                IsPendingForRead()  const { return m_pendingRead;        } //!< Returns true when there is a pending request to read the Register from SUT
  virtual bool        IsPending()         const override;                        //!< Returns true if register is pending for read or for write
  virtual uint32_t    PendingCount()      const override;                        //!< Returns number of pending registers down the hierarchy

  // ---------------- Setters
  //
  void         SetFromSut         (BinaryVector sequence);                                                                              //!< Sets last sequence of bits that have been shifted from SUT
  void         SetExpectedFromSut (BinaryVector sequence)               { m_expectedFromSut = std::move(sequence); }                    //!< Sets expected sequence (when updating from SUT)
  void         SetBypass          (BinaryVector sequence)               { m_bypass          = std::move(sequence); }                    //!< Sets sequence to shift into the sut when no iApply cycle has been defined on the register
  void         SetHoldValue       (bool         holdValue       = true) { m_holdValue       = holdValue;           }                    //!< Set whether bypass value is maintain equal to nextToSut
  void         SetPendingForRead  (bool         pendingForRead  = true) { m_pendingRead     = pendingForRead;    }                      //!< Set whether there is a pending request or not for read value from SUT
  void         SetPendingForWrite (bool         pendingForWrite = true) { SystemModelNode::SetPendingCount(pendingForWrite ? 1u : 0); } //!< Sets pending requests for updating SUT (write operation)
  void         ResetPendingWrite  ()                                    { SystemModelNode::ResetPending(); }                            //!< Resets any pending requests for updating SUT (write operation)
  virtual void ResetPending       () override;                                                                                          //!< Resets read and write pending

  //! Sets the bits sequence to send during the next iApply cycle
  //!
  void SetToSut (BinaryVector&& sequence)
  {
    if (m_holdValue)
    {
      m_bypass = sequence;
    }
    m_nextToSut = std::move(sequence);
  }

  //! Sets the bits sequence to send during the next iApply cycle
  //!
  void SetToSut (const BinaryVector& sequence)
  {
    if (m_holdValue)
    {
      m_bypass = sequence;
    }
    m_nextToSut = sequence;
  }

  //! Returns last sequence received from SUT as integral value
  //!
  void LastFromSut (BinaryVector& value) const { value = m_lastFromSut; }

  //! Returns last sequence received from SUT as integral value
  //!
  template<typename T> void LastFromSut (T& value) const
  {
    static_assert(std::is_integral<T>::value, "LastFromSut requires integral types");
    m_lastFromSut.Get(value);
  }

  //! Returns last sequence received from SUT when it was in pending read state - with output reference
  //!
  void LastReadFromSut (BinaryVector& value) const { value = m_lastReadFromSut; }

  //! Returns last sequence received from SUT when it was in pending read state - as integral value
  //!
  template<typename T> void LastReadFromSut (T& value) const
  {
    static_assert(std::is_integral<T>::value, "LastReadFromSut requires integral types");
    m_lastReadFromSut.Get(value);
  }


  //! Sets expected sequence (when updating from SUT) from integral value
  //!
  template<typename T> void SetExpectedFromSut (T newValue)
  {
    static_assert(std::is_integral<T>::value, "SetExpectedFromSut requires BinaryVector or integral types");
    m_expectedFromSut.Set(newValue);
  }

  //! Sets the bits sequence to send during the next iApply cycle from integral value
  //!
  template <typename T> void SetToSut (T newValue)
  {
    static_assert(std::is_integral<T>::value, "SetToSut requires BinaryVector or integral types");
    m_nextToSut.Set(newValue);

    if (m_holdValue)
    {
      m_bypass.Set(newValue);
    }
  }

  void SetCheckExpected   (bool checkExpected) { m_mustCheckExpected  = checkExpected; } //!< Sets whether data updated from SUT must be check agains expected data
  void ResetMismatches    ()                   { m_mismatches = 0; }                     //!< Clears the mismatch count

  void UpdateLastToSut() { m_lastToSut = m_nextToSut; } //!< Updates "last to sut" field from "next to sut" field (should be called when "next to sut" has effectively been shifted to SUT)

  // ---------------- Private  Fields
  //
  private:
  bool         m_pendingRead       = false; //!< True when there is a pending request to read register value from SUT
  bool         m_holdValue         = false; //!< When true, force bypass value to be equal to nextToSut (The value will not be changed while the register is selected)
  bool         m_mustCheckExpected = false; //!< When true, it triggers a check of received vs expected data during the following shift from sut
  uint32_t     m_mismatches        = 0;     //!< Number of mismatches following IEEE 1687 rules
  BinaryVector m_nextToSut;                 //!< Sequence of bits that should be shifted into SUT (during the next iApply cycle)
  BinaryVector m_lastToSut;                 //!< Last sent sequence of bits: It stores the status of the SUT (SIBs, etc...) after an apply cycle
  BinaryVector m_lastFromSut;               //!< Last sequence of bits that have been shifted from SUT
  BinaryVector m_lastReadFromSut;           //!< Last sequence of bits that have been shifted from SUT when pending read is true
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
