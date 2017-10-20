//===========================================================================
//                           VirtualRegister.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file VirtualRegister.cpp
//!
//! Implements class VirtualRegister
//!
//===========================================================================

#include "VirtualRegister.hpp"
#include "Utility.hpp"

using namespace mast;



//! Appends a register slice to proxied registers
//!
//! @param registerSlice  Defines which Register and bits range to proxy
//!
void VirtualRegister::Append (const RegisterSlice& registerSlice)
{
  CHECK_PARAMETER_NOT_NULL (registerSlice.reg,                                                "Cannot append nullptr Register");
  CHECK_PARAMETER_NOT_ZERO (registerSlice.reg->BitsCount(),                                   "Cannot append Register with width zero");
  CHECK_PARAMETER_NEQ      (registerSlice.reg->BitsOrdering(), mast::BitsOrdering::Undefined, "Bits ordering must be defined");
  CHECK_PARAMETER_LTE      (registerSlice.range.Width(),    registerSlice.reg->BitsCount(),   "Range cannot be larger than Register");
  CHECK_PARAMETER_LT       (registerSlice.range.MaxIndex(), registerSlice.reg->BitsCount(),   "Range cannot extend past the Register width");

  if (!m_registers.empty())
  {
    CHECK_PARAMETER_EQ (registerSlice.reg->BitsOrdering(), m_registers.back().reg->BitsOrdering(), "Bits ordering must be the same as previous registers");
  }

  m_registers.emplace_back(registerSlice);
}
//
//  End of: VirtualRegister::Append
//---------------------------------------------------------------------------



//! Returns VirtualRegister numbers of bits
//!
uint32_t VirtualRegister::BitsCount () const
{
  uint32_t bitsCount = 0;

  for (const auto& regSlice : m_registers)
  {
    bitsCount += regSlice.range.Width();
  }
  return bitsCount;
}
//
//  End of: VirtualRegister::BitsCount
//---------------------------------------------------------------------------



//! Returns last sequence effectively sent to SUT
//!
BinaryVector VirtualRegister::LastToSut () const
{
  return BinaryVector();
}
//
//  End of: VirtualRegister::LastToSut
//---------------------------------------------------------------------------



//! Returns BitsOrdering
//!
mast::BitsOrdering VirtualRegister::BitsOrdering () const
{
  if (m_registers.empty())
  {
    return mast::BitsOrdering::Undefined;
  }

  return m_registers.front().reg->BitsOrdering();
}
//
//  End of: VirtualRegister::BitsOrdering
//---------------------------------------------------------------------------



//! Returns next sequence to send to SUT
//!
BinaryVector VirtualRegister::NextToSut () const
{
  return BinaryVector();
}
//
//  End of: VirtualRegister::NextToSut
//---------------------------------------------------------------------------



//! Sets number of pending to 1 to all registers
//!
void VirtualRegister::SetPending ()
{
  for (const auto& regSlice : m_registers)
  {
    regSlice.reg->SetPending();
  }
}
//
//  End of: VirtualRegister::SetPending
//---------------------------------------------------------------------------



//! Sets the bits sequence to send during the next iApply cycle
//!
//! @param sequence Sequence of bits to set to proxied registers slices
//!
void VirtualRegister::SetToSut (const BinaryVector& sequence)
{
}
//
//  End of: VirtualRegister::SetToSut
//---------------------------------------------------------------------------






//===========================================================================
// End of VirtualRegister.cpp
//===========================================================================
