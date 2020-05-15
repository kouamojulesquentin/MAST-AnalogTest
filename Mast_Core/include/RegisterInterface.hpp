//===========================================================================
//                           RegisterInterface.hpp
//===========================================================================
// Copyright (C) 2018 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file RegisterInterface.hpp
//!
//! Declares class RegisterInterface
//!
//===========================================================================

#ifndef REGISTERINTERFACE_H__5F8AB709_8B30_4CEE_D0B3_9C7B201CCA64__INCLUDED_
  #define REGISTERINTERFACE_H__5F8AB709_8B30_4CEE_D0B3_9C7B201CCA64__INCLUDED_

#include "BinaryVector.hpp"
#include "BitsOrdering.hpp"

#include <vector>

namespace mast
{
class SystemModel;

//! Represents interface for common operations on register and register like objects
//!
class RegisterInterface
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~RegisterInterface() = default;

  // ---------------- Getters
  //
  virtual const std::vector<uint32_t>& Identifiers()       const = 0; //!< Returns register(s) unique identifier
  virtual uint32_t                     BitsCount()         const = 0; //!< Returns RegisterInterface numbers of bits
  virtual mast::BitsOrdering           BitsOrdering()      const = 0; //!< Returns BitsOrdering
  virtual const BinaryVector&          LastToSut()         const = 0; //!< Returns last sequence effectively sent to SUT
  virtual const BinaryVector&          NextToSut()         const = 0; //!< Returns next sequence to send to SUT
  virtual BinaryVector                 LastCompareResult() const = 0; //!< Returns XOR of the value last read from SUT and the expected value. May contain x-values (for don't care).
  virtual const BinaryVector&          LastFromSut()       const = 0; //!< Returns last sequence received from SUT
  virtual bool isBlackBox()        const = 0; //!< Returns true if register is a BlackBox

  virtual void LastFromSut (BinaryVector& readData) const = 0; //!< Returns last sequence received from SUT
  virtual void LastFromSut (uint8_t&      readData) const = 0; //!< Returns last sequence received from SUT
  virtual void LastFromSut (uint16_t&     readData) const = 0; //!< Returns last sequence received from SUT
  virtual void LastFromSut (uint32_t&     readData) const = 0; //!< Returns last sequence received from SUT
  virtual void LastFromSut (uint64_t&     readData) const = 0; //!< Returns last sequence received from SUT
  virtual void LastFromSut (int8_t&       readData) const = 0; //!< Returns last sequence received from SUT
  virtual void LastFromSut (int16_t&      readData) const = 0; //!< Returns last sequence received from SUT
  virtual void LastFromSut (int32_t&      readData) const = 0; //!< Returns last sequence received from SUT
  virtual void LastFromSut (int64_t&      readData) const = 0; //!< Returns last sequence received from SUT

//+  virtual const BinaryVector& BypassSequence()    const = 0; //!< Returns bypass sequence
//+  virtual const BinaryVector& ExpectedFromSut()   const = 0; //!< Returns expected sequence
//+  virtual const BinaryVector& LastReadFromSut()   const = 0; //!< Returns last sequence received from SUT when it was in pending read state
//+  virtual const BinaryVector& DontCareMask()      const = 0; //!< Returns the don't care mask (1 for care, 0 for don't care)
//+  virtual bool                HoldValue()         const = 0; //!< Returns true when bypass value is maintained equal to nextToSut (The value will not be changed while the register is selected)

  // ---------------- Setters
  //
//+  virtual void SetPending () = 0;                             //!< Sets number of pending to 1
  virtual void SetToSut           (const BinaryVector& sequence) = 0;                            //!< Sets the bits sequence to send during the next iApply cycle
  virtual void SetExpectedFromSut (BinaryVector        sequence, BinaryVector dontCareMask) = 0; //!< Sets expected sequence and don't care mask (when updating from SUT)

//+  virtual template <typename T> void SetToSut (T newValue) = 0; //!< Sets the bits sequence to send during the next iApply cycle from integral value

//+  virtual void SetFromSut         (BinaryVector sequence) = 0;                            //!< Sets last sequence of bits that have been shifted from SUT
//+  virtual void SetBypass          (BinaryVector sequence) = 0;                            //!< Sets sequence to shift into the sut when no iApply cycle has been defined on the register
//+  virtual void SetExpectedFromSut (BinaryVector sequence) = 0;                            //!< Sets expected sequence (when updating from SUT)
//+  virtual void SetHoldValue       (bool holdValue = true) = 0;                            //!< Set whether bypass value is maintain equal to nextToSut

  // ---------------- Protected Methods
  //
  RegisterInterface() = default;
};
//
//  End of RegisterInterface class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined REGISTERINTERFACE_H__5F8AB709_8B30_4CEE_D0B3_9C7B201CCA64__INCLUDED_
//===========================================================================
// End of RegisterInterface.hpp
//===========================================================================
