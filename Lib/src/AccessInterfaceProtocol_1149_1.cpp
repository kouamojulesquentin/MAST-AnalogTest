//===========================================================================
//                           AccessInterfaceProtocol_1149_1.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterfaceProtocol_1149_1.cpp
//!
//! Implements class AccessInterfaceProtocol_1149_1
//!
//===========================================================================

#include "AccessInterfaceProtocol_1149_1.hpp"

using namespace mast;


//! Does any action required to transfer scan data to and from SUT
//!
//! @param derivationId   Identifies the derivation to act for (zero based)
//! @param interfaceData  Application data stored in the AccessInterface
//! @param toSutData      Bits stream to transfer to SUT
//!
//! @return Bits stream retrieved from SUT
BinaryVector AccessInterfaceProtocol_1149_1::DoAction (uint32_t /* derivationId */, void* /* interfaceData */, const BinaryVector& toSutData)
{
  auto fromSut = toSutData;

  return fromSut;
}
//
//  End of: AccessInterfaceProtocol_1149_1::DoAction
//---------------------------------------------------------------------------




//===========================================================================
// End of AccessInterfaceProtocol_1149_1.cpp
//===========================================================================
