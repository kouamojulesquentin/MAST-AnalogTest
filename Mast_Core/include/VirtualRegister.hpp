//===========================================================================
//                           VirtualRegister.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file VirtualRegister.hpp
//!
//! Declares class VirtualRegister
//!
//===========================================================================

#ifndef VIRTUALREGISTER_H__CFE68D04_B156_4401_9698_1E66847E7E70__INCLUDED_
  #define VIRTUALREGISTER_H__CFE68D04_B156_4401_9698_1E66847E7E70__INCLUDED_

#include "RegisterInterface.hpp"
#include "Register.hpp"
#include "IndexedRange.hpp"
#include "BitsOrdering.hpp"
#include <vector>
#include <functional>

namespace mast
{
class SystemModel;

//! Represents slice of some register
//!
struct RegisterSlice
{
  std::shared_ptr<Register> reg;    //!< The Register for which a slice is defined
  IndexedRange              range;  //!< Indices of bits in "sliced" register

  bool operator==(const RegisterSlice& rhs) const
  {
    return std::tie(reg, range) == std::tie(rhs.reg, rhs.range);
  }

  bool operator!=(const RegisterSlice& rhs) const
  {
    return !operator==(rhs);
  }
};

//! Proxy to a bunch of, ordered, Register Slice, providing Register like interface.
//!
//! @note This is firstly intended for PathSelector to select path driving bits of,
//!       potentially, several Register
//!
//! @note Register interface is not complete (only required methods are implemented)
//!
class VirtualRegister final : public RegisterInterface
{
  // ---------------- Public  Methods
  //
  public:
  ~VirtualRegister();
  VirtualRegister()  = default;


  explicit VirtualRegister(std::shared_ptr<Register> reg);

  //! Constructs with (first) register slice to proxied registers
  //!
  //! @param registerSlice  Defines which Register and bits range to proxy
  //!
  explicit VirtualRegister (const RegisterSlice& registerSlice) { Append(registerSlice); }

  //! Appends a register slice to proxied registers
  //!
  void Append(const RegisterSlice& registerSlice);

  // ---------------- Miscellaneous
  //
  std::experimental::string_view TypeName() const { return "VirtualRegister"; } //!< Returns readable type name

  // ---------------- Getters
  //
  bool               Empty()        const { return m_registers.empty(); };  //!< Returns VirtualRegister is empty (refering to no registers at all)
  uint32_t           SlicesCount()  const { return m_registers.size(); };   //!< Returns VirtualRegister numbers of register slice

  const std::vector<uint32_t>& Identifiers() const override { return m_identifiers; } //!< Returns register(s) unique identifier(s)

  uint32_t            BitsCount()         const override; //!< Returns VirtualRegister numbers of bits
  mast::BitsOrdering  BitsOrdering()      const override; //!< Returns BitsOrdering
  BinaryVector        LastCompareResult() const override; //!< Returns XOR of the value last read from SUT and the expected value. May contain x-values (for don't care).
  const BinaryVector& LastToSut()         const override; //!< Returns last sequence effectively sent to SUT
  const BinaryVector& NextToSut()         const override; //!< Returns next sequence to send to SUT
  const BinaryVector& LastFromSut()       const override; //!< Returns last sequence received from SUT

  void LastFromSut (BinaryVector& readData) const override; //!< Returns last sequence received from SUT
  void LastFromSut (uint8_t&      readData) const override; //!< Returns last sequence received from SUT
  void LastFromSut (uint16_t&     readData) const override; //!< Returns last sequence received from SUT
  void LastFromSut (uint32_t&     readData) const override; //!< Returns last sequence received from SUT
  void LastFromSut (uint64_t&     readData) const override; //!< Returns last sequence received from SUT
  void LastFromSut (int8_t&       readData) const override; //!< Returns last sequence received from SUT
  void LastFromSut (int16_t&      readData) const override; //!< Returns last sequence received from SUT
  void LastFromSut (int32_t&      readData) const override; //!< Returns last sequence received from SUT
  void LastFromSut (int64_t&      readData) const override; //!< Returns last sequence received from SUT

  // ---------------- Setters
  //
  void SetPending ();                                      //!< Sets number of pending to 1
  void SetToSut   (const BinaryVector& sequence) override; //!< Sets the bits sequence to send during the next iApply cycle

  //! Sets the bits sequence to send during the next iApply cycle from integral value
  //!
  template <typename T> void SetToSut (T newValue)
  {
    static_assert(std::is_integral<T>::value, "SetToSut requires BinaryVector or integral types");
  }

  void SetExpectedFromSut (BinaryVector sequence, BinaryVector dontCareMask) override;         //!< Sets expected sequence and don't care mask (when updating from SUT)

  // ---------------- Iterators
  //
  auto begin()  noexcept       { return m_registers.begin();  } //!< Returns an iterator to the first RegisterSlice
  auto begin()  const noexcept { return m_registers.begin();  } //!< Returns an iterator to the first RegisterSlice
  auto cbegin() const noexcept { return m_registers.cbegin(); } //!< Returns an iterator to the first RegisterSlice

  auto end()    noexcept       { return m_registers.end();    } //!< Returns an iterator to the element following the last RegisterSlice
  auto end()    const noexcept { return m_registers.end();    } //!< Returns an iterator to the element following the last RegisterSlice
  auto cend()   const noexcept { return m_registers.cend();   } //!< Returns an iterator to the element following the last RegisterSlice

  // ---------------- Private Methods
  //
  static BinaryVector GetSlice (const IndexedRange& range, mast::BitsOrdering bitsOrdering, const BinaryVector& binaryVector);
  static void         SetSlice (const IndexedRange& range, mast::BitsOrdering bitsOrdering,       BinaryVector& binaryVector, const BinaryVector& value);

  void                GetView  (BinaryVector& target, std::function<BinaryVector (const Register&)> getter) const;

  void                SetView  (std::function<BinaryVector (const Register&)>                getter,
                                std::function<void         (Register&, const BinaryVector&)> setter,
                                const BinaryVector&                                          value);

  void UpdateRegisters(std::function<void(const RegisterSlice&, const IndexedRange&)>);

  template<typename T> void LastFromSut_impl (T& readData) const
  {
    const auto& lastFromSut = LastFromSut();
    lastFromSut.Get(readData);
  }

  // ---------------- Private  Fields
  //
  private:
  std::vector<uint32_t>      m_identifiers; //!< Identifiers of each registers that make up the VirtualRegister
  std::vector<RegisterSlice> m_registers;   //!< Defines registers chunk that make up the VirtualRegister
  mutable BinaryVector       m_nextToSut;   //!< Last computed "next to sut"   - This is a member to return a reference needed by RegisterInterface (this also reduce allocations/deallocations)
  mutable BinaryVector       m_lastToSut;   //!< Last computed "last to sut"   - This is a member to return a reference needed by RegisterInterface (this also reduce allocations/deallocations)
  mutable BinaryVector       m_lastFromSut; //!< Last computed "last from sut" - This is a member to return a reference needed by RegisterInterface (this also reduce allocations/deallocations)
};
//
//  End of VirtualRegister class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined VIRTUALREGISTER_H__CFE68D04_B156_4401_9698_1E66847E7E70__INCLUDED_

//===========================================================================
// End of VirtualRegister.hpp
//===========================================================================
