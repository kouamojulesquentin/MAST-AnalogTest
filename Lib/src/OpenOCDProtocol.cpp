//===========================================================================
//                           OpenOCDProtocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file OpenOCDProtocol.cpp
//!
//! Implements class OpenOCDProtocol
//!
//===========================================================================

#include "OpenOCDProtocol.hpp"

#include <vector>

using namespace mast;
using std::vector;
using std::experimental::string_view;

//! Initializes OpenOCD "engine"
//!
OpenOCDProtocol::OpenOCDProtocol (string_view configFilePath)
{
  //! @todo [JFC]-[August/24/2016]: In OpenOCDProtocol(): Complete as necessary
  //!
}
//
//  End of: OpenOCDProtocol::OpenOCDProtocol
//---------------------------------------------------------------------------


//! Does any action required to transfer scan data to and from SUT
//!
//! @param derivationId   Identifies the derivation to act for (zero based)
//! @param interfaceData  Application data stored in the AccessInterface
//! @param toSutData      Bits stream to transfer to SUT
//!
//! @return Bitstream retrieved from SUT
//!
BinaryVector OpenOCDProtocol::DoAction (uint32_t /* derivationId */, void* /* interfaceData */, const BinaryVector& toSutData)
{
  auto bitsCount         = toSutData.BitsCount();
  auto fromSutDataBuffer = vector<uint8_t>(toSutData.BytesCount());

  //! @todo [JFC]-[August/24/2016]: In DoAction(): Do proper call to OpenOCD library
  //!
//+  xxx_TDR(bitsCount, toSutDatatoSutData.Data(), fromSutDataBuffer.data());

  auto fromSutData = BinaryVector(fromSutDataBuffer, bitsCount);
  return fromSutData;
}
//
//  End of: Spy_AccessInterfaceProtocols::DoAction
//---------------------------------------------------------------------------

//===========================================================================
// End of OpenOCDProtocol.cpp
//===========================================================================
