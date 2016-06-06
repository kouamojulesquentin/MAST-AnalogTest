//===========================================================================
//                           FromSutUpdater.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file FromSutUpdater.cpp
//!
//! Implements class FromSutUpdater
//!
//===========================================================================

#include "FromSutUpdater.hpp"
#include "Utility.hpp"
#include <sstream>

using std::ostringstream;
using namespace mast;



//! Updates system model from vector shifted from SUT and ordered list active registers at the time of JTAG shift
//!
//! @param activeRegisters  Registers that were active when the bitstream was streamed out of SUT
//! @param fromSutVector    Collected register bits from SUT
//!
void FromSutUpdater::UpdateRegisters (const std::vector<NodeIdentifier>& activeRegisters, const BinaryVector& fromSutVector)
{
  uint32_t offset = 0u;

  for (const auto& regId : activeRegisters)
  {
    auto reg = m_sm.RegisterWithId(regId);
    CHECK_VALUE_NOT_NULL(reg, "Nodes identifiers must refer to valid Register nodes");

    auto bitsCount = reg->BitsCount();

    if ((offset + bitsCount) > fromSutVector.BitsCount())
    {
      ostringstream os;
      os << "fromSutVector is too small ==> Cannot update Register \"" << reg->Name() << "\" (Id: "  << regId << ")";
      THROW_LOGIC_ERROR(os.str());
    }
    //! @todo [JFC]-[June/03/2016]: Use a BinaryVector_View instead of plain BinaryVector
    //!
    auto slice     = fromSutVector.Slice(offset, bitsCount);
    reg->SetFromSut(slice);
    reg->UpdateLastToSut();
    reg->ResetPending();

    offset += bitsCount;
  }

  if (offset != fromSutVector.BitsCount())
  {
    ostringstream os;
    os << "fromSutVector has not been completly consumed (" << fromSutVector.BitsCount() - offset << " bits left)";
    THROW_LOGIC_ERROR(os.str());
  }
}
//
//  End of: FromSutUpdater::UpdateRegisters
//---------------------------------------------------------------------------


//===========================================================================
// End of FromSutUpdater.cpp
//===========================================================================
