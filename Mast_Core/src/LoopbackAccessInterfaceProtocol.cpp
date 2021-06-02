//===========================================================================
//                           LoopbackAccessInterfaceProtocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file LoopbackAccessInterfaceProtocol.cpp
//!
//! Implements class LoopbackAccessInterfaceProtocol
//!
//===========================================================================

#include "LoopbackAccessInterfaceProtocol.hpp"
#include "g3log/g3log.hpp"

using namespace mast;


//! Does any callback required to transfer scan data to and from SUT
//!
//! @param Channeld   Identifies the Channel to act for (zero based)
//! @param interfaceData  Application data stored in the AccessInterface
//! @param toSutData      Bits stream to transfer to SUT
//!
//! @return Bits stream retrieved from SUT
BinaryVector LoopbackAccessInterfaceProtocol::DoCallback (uint32_t /* ChannelId */, void* /* interfaceData */, const BinaryVector& toSutData)
{
  auto fromSut = toSutData;

  return fromSut;
}
//
//  End of: LoopbackAccessInterfaceProtocol::DoCallback
//---------------------------------------------------------------------------


//! Forces the ResetPort to be asserted on the target module
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
void LoopbackAccessInterfaceProtocol::DoReset(bool /* doSynchronousReset */)
{
  LOG(INFO) << "Reseting " << KindName() << " interface";
}


//===========================================================================
// End of LoopbackAccessInterfaceProtocol.cpp
//===========================================================================
