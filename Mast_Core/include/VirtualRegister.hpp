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
  };
//+  VirtualRegister(std::initializer_list<shared_ptr<RegisterSlice>> registers);

  //! Appends a register slice to proxied registers
  //!
  void Append(const RegisterSlice& registerSlice);

  // ---------------- Miscellaneous
  //
  std::experimental::string_view TypeName() const { return "VirtualRegister"; } //!< Returns readable type name

  // ---------------- Getters
  //
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
