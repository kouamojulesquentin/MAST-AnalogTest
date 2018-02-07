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

#include "Register.hpp"
#include "IndexedRange.hpp"
#include "BitsOrdering.hpp"
#include <vector>
#include <functional>

namespace mast
{
class SystemModel;

//! Proxy to a bunch of, ordered, Register Slice, providing Register like interface.
//!
//! @note This is firstly intended for PathSelector to select path driving bits of,
//!       potentially, several Register
//!
//! @note Register interface is not complete (only required methods are implemented)
//!
class MAST_CORE_EXPORT VirtualRegister final
{
  // ---------------- Public  Methods
  //
  public:
  ~VirtualRegister() = default;
  VirtualRegister()  = default;

  struct RegisterSlice
  {
    std::shared_ptr<Register> reg;
    IndexedRange              range;
    bool operator==(const RegisterSlice& rhs) const
    {
      return std::tie(reg, range) == std::tie(rhs.reg, rhs.range);
    }

    bool operator!=(const RegisterSlice& rhs) const
    {
      return !operator==(rhs);
    }
  };

  explicit VirtualRegister(std::shared_ptr<Register> reg);

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

  uint32_t           BitsCount()    const; //!< Returns VirtualRegister numbers of bits
  mast::BitsOrdering BitsOrdering() const; //!< Returns BitsOrdering
  BinaryVector       LastToSut()    const; //!< Returns last sequence effectively sent to SUT
  BinaryVector       NextToSut()    const; //!< Returns next sequence to send to SUT

  // ---------------- Setters
  //
  void SetPending ();                             //!< Sets number of pending to 1
  void SetToSut   (const BinaryVector& sequence); //!< Sets the bits sequence to send during the next iApply cycle

  //! Sets the bits sequence to send during the next iApply cycle from integral value
  //!
  template <typename T> void SetToSut (T newValue)
  {
    static_assert(std::is_integral<T>::value, "SetToSut requires BinaryVector or integral types");
  }

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

  BinaryVector        GetView  (std::function<BinaryVector (const Register&)> getter) const;
  void                SetView  (std::function<BinaryVector (const Register&)>                      getter,
                                std::function<void         (      Register&, const BinaryVector&)> setter,
                                const BinaryVector&                                                value);

  // ---------------- Private  Fields
  //
  private:
  std::vector<RegisterSlice> m_registers;
};
//
//  End of VirtualRegister class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined VIRTUALREGISTER_H__CFE68D04_B156_4401_9698_1E66847E7E70__INCLUDED_

//===========================================================================
// End of VirtualRegister.hpp
//===========================================================================
