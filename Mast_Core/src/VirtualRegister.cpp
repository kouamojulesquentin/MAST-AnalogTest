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
#include "Register.hpp"
#include "Utility.hpp"

using namespace mast;
using std::shared_ptr;

//! Releases resources
//!
//! @note Put in implemetation file to force construction of VTable !
VirtualRegister::~VirtualRegister ()
{
}
//
//  End of: VirtualRegister::~VirtualRegister
//---------------------------------------------------------------------------



//! Constructs from a single Register
//!
//! @param reg  Register treated as VirtualRegister
//!
VirtualRegister::VirtualRegister (shared_ptr<Register> reg)
{
  CHECK_PARAMETER_NOT_NULL(reg,             "Cannot create VirtualRegister from nullptr Register");
  CHECK_PARAMETER_GTE(reg->BitsCount(), 1u, "Register must have at least one bit");

  IndexedRange range(reg->BitsCount() - 1u, 0);

  m_registers.emplace_back(RegisterSlice{reg, range});
  m_identifiers.emplace_back(reg->Identifier());
}
//
//  End of: VirtualRegister::VirtualRegister
//---------------------------------------------------------------------------




//! Appends a register slice to proxied registers
//!
//! @param registerSlice  Defines which Register and bits range to proxy
//!
void VirtualRegister::Append (const RegisterSlice& registerSlice)
{
  auto reg = registerSlice.reg;

  CHECK_PARAMETER_NOT_NULL (reg,                                                            "Cannot append nullptr Register");
  CHECK_PARAMETER_NOT_ZERO (reg->BitsCount(),                                               "Cannot append Register with width zero");
  CHECK_PARAMETER_NEQ      (reg->BitsOrdering(),            mast::BitsOrdering::Undefined,  "Bits ordering must be defined");
  CHECK_PARAMETER_LTE      (registerSlice.range.Width(),    registerSlice.reg->BitsCount(), "Range cannot be larger than Register");
  CHECK_PARAMETER_LT       (registerSlice.range.MaxIndex(), registerSlice.reg->BitsCount(), "Range cannot extend past the Register width");

  if (!m_registers.empty())
  {
    CHECK_PARAMETER_EQ (reg->BitsOrdering(), m_registers.back().reg->BitsOrdering(), "Bits ordering must be the same as previous registers");
  }

  m_registers.emplace_back(registerSlice);
  m_identifiers.emplace_back(reg->Identifier());
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


//! Creates slice from BinaryVector respecting bits ordering
//!
//! @note Must adapt to design numbering of register bits to BinaryVector bits ordering
//!       e.g. For a BinaryVector of 19 bits (with bits identified as 0 to 18 internally):
//!         - A requested downto range [7, 2] is translated to BinaryVector bits index [11, 16]
//!         - A requested upto   range [2, 7] is translated to BinaryVector bits index [2, 7]
//!
//! @param range          Bits range (as defined as ICL)
//! @param bitsOrdering   Tells whether indexed are defined downto or upto (from left to right)
//! @param binaryVector   A BinaryVector to get a slice from
//!
//! @return A BinaryVector slice
BinaryVector VirtualRegister::GetSlice (const IndexedRange& range, mast::BitsOrdering bitsOrdering, const BinaryVector& binaryVector)
{
  CHECK_PARAMETER_FALSE(binaryVector.IsEmpty(), "Cannot get slice of empty BinaryVector");

  if (bitsOrdering == mast::BitsOrdering::Upto)
  {
    return binaryVector.Slice(range);
  }

  auto lastBitIndex = binaryVector.BitsCount() - 1u;
  auto right        = lastBitIndex - range.right;
  auto left         = lastBitIndex - range.left;
  auto adaptedRange = IndexedRange(left, right);

  return binaryVector.Slice(adaptedRange);
}
//
//  End of: VirtualRegister::GetSlice
//---------------------------------------------------------------------------


//! Returns "view" of "interfaced" Register using function to get their actual field
//!
//! @note Target is built by chunks, so it is not thread safe to call that method with
//!       same target from different threads
//!
//! @param target  Target of result view
//! @param getter  Function to get a specific value from the Registers
//!
void VirtualRegister::GetView (BinaryVector& target, std::function<BinaryVector(const Register&)> getter) const
{
  CHECK_VALUE_NOT_EMPTY(m_registers, "VirtualRegister must be interface to at least one register before requesting some values");

  auto bitsOrdering = m_registers.front().reg->BitsOrdering();

  target.Clear();
  for (const auto& registerSlice : m_registers)
  {
    auto slice = GetSlice(registerSlice.range, bitsOrdering, getter(*registerSlice.reg));
    target.Append(slice);
  }
}
//
//  End of: VirtualRegister::GetView
//---------------------------------------------------------------------------



//! Returns XOR of the value last read from SUT and the expected value. May contain x-values (for don't care)
//!
BinaryVector VirtualRegister::LastCompareResult () const
{
  BinaryVector lastCompareResult;
  GetView(lastCompareResult, [](const Register& reg) { return reg.LastCompareResult(); });
  return lastCompareResult;
}
//
//  End of: VirtualRegister::LastCompareResult
//---------------------------------------------------------------------------


//! Returns last sequence received from SUT
//!
const BinaryVector& VirtualRegister::LastFromSut () const
{

  GetView(m_lastFromSut, [](const Register& reg) { return reg.LastFromSut(); });
  return m_lastFromSut;
}
//
//  End of: VirtualRegister::LastFromSut
//---------------------------------------------------------------------------


void VirtualRegister::LastFromSut (BinaryVector& readData) const { GetView(readData, [](const Register& reg) { return reg.LastFromSut(); }); }
void VirtualRegister::LastFromSut (uint8_t&      readData) const { LastFromSut_impl(readData); }
void VirtualRegister::LastFromSut (uint16_t&     readData) const { LastFromSut_impl(readData); }
void VirtualRegister::LastFromSut (uint32_t&     readData) const { LastFromSut_impl(readData); }
void VirtualRegister::LastFromSut (uint64_t&     readData) const { LastFromSut_impl(readData); }
void VirtualRegister::LastFromSut (int8_t&       readData) const { LastFromSut_impl(readData); }
void VirtualRegister::LastFromSut (int16_t&      readData) const { LastFromSut_impl(readData); }
void VirtualRegister::LastFromSut (int32_t&      readData) const { LastFromSut_impl(readData); }
void VirtualRegister::LastFromSut (int64_t&      readData) const { LastFromSut_impl(readData); }

//! Returns last sequence effectively sent to SUT
//!
const BinaryVector& VirtualRegister::LastToSut () const
{
  GetView(m_lastToSut, [](const Register& reg) { return reg.LastToSut(); });
  return m_lastToSut;
}
//
//  End of: VirtualRegister::LastToSut
//---------------------------------------------------------------------------


//! Returns next sequence to send to SUT
//!
const BinaryVector& VirtualRegister::NextToSut () const
{
  GetView(m_nextToSut, [](const Register& reg) { return reg.NextToSut(); });
  return m_nextToSut;
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


//! Sets BinaryVector slice respecting bits ordering
//!
//! @note Must adapt to design numbering of register bits to BinaryVector bits ordering
//!       e.g. For a BinaryVector of 19 bits (with bits identified as 0 to 18 internally):
//!         - A requested downto range [7, 2] is translated to BinaryVector bits index [11, 16]
//!         - A requested upto   range [2, 7] is translated to BinaryVector bits index [2, 7]
//!
//! @param range          Bits range (as defined as ICL)
//! @param bitsOrdering   Tells whether indexed are defined downto or upto (from left to right)
//! @param binaryVector   A BinaryVector to set slice
//! @param value          Value to set in BinaryVector slice
//!
void VirtualRegister::SetSlice (const IndexedRange& range, mast::BitsOrdering bitsOrdering, BinaryVector& binaryVector, const BinaryVector& value)
{
  CHECK_PARAMETER_FALSE (binaryVector.IsEmpty(), "Cannot get slice of empty BinaryVector");
  CHECK_VALUE_EQ        (range.Width(), value.BitsCount(), "Expected range width to be equal to value width");

  if (bitsOrdering == mast::BitsOrdering::Upto)
  {
    binaryVector.SetSlice(range.left, value);
  }
  else
  {
    auto lastBitIndex = binaryVector.BitsCount() - 1u;
    auto left         = lastBitIndex - range.left;

    binaryVector.SetSlice(left, value);
  }
}
//
//  End of: VirtualRegister::SetSlice
//---------------------------------------------------------------------------


//! Sets "view" of "interfaced" Registers using function to set their actual field
//!
//! @param setter   Function to set a specific value from the Registers
//!
void VirtualRegister::SetView (std::function<BinaryVector (const Register&)>                      getter, // Getter is to update the register by changing only a slice
                               std::function<void         (      Register&, const BinaryVector&)> setter,
                               const BinaryVector&                                                value)
{
  auto bitsOrdering = m_registers.front().reg->BitsOrdering();

  auto updateRegister = [&](const RegisterSlice& registerSlice, const IndexedRange& valueRange)
  {
    auto slicedValue = GetSlice(valueRange, bitsOrdering, value);
    auto regValue    = getter(*registerSlice.reg);    // Current value (to update)

    SetSlice(registerSlice.range, bitsOrdering, regValue, slicedValue);

    setter(*registerSlice.reg, regValue);
  };

  if (bitsOrdering == mast::BitsOrdering::Upto)
  {
    auto valueRange = IndexedRange(0);
    for (const auto& registerSlice : m_registers)
    {
      valueRange.right += registerSlice.range.Width() - 1u;

      updateRegister(registerSlice, valueRange);

      valueRange = IndexedRange(valueRange.right + 1u);
    }
  }
  else
  {
    auto valueRange = IndexedRange(BitsCount() - 1u);
    for (const auto& registerSlice : m_registers)
    {
      valueRange.right -= registerSlice.range.Width() - 1u;

      updateRegister(registerSlice, valueRange);

      valueRange = IndexedRange(valueRange.right - 1u);
    }
  }
}
//
//  End of: VirtualRegister::SetView
//---------------------------------------------------------------------------


//! Sets the bits sequence to send during the next iApply cycle
//!
//! @param sequence Sequence of bits to set to proxied registers slices
//!
void VirtualRegister::SetToSut (const BinaryVector& sequence)
{
  CHECK_PARAMETER_FALSE(sequence.IsEmpty(), "Can not set to SUT value with empty vector");
  CHECK_VALUE_NOT_EMPTY(m_registers,        "VirtualRegister must be 'interface' to at least one register before calling SetToSut");

  SetView([](const Register& reg) { return reg.NextToSut(); },
          [](      Register& reg, const BinaryVector& value) { reg.SetToSut(value); },
          sequence);
}
//
//  End of: VirtualRegister::SetToSut
//---------------------------------------------------------------------------






//===========================================================================
// End of VirtualRegister.cpp
//===========================================================================
