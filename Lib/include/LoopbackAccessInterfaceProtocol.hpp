//===========================================================================
//                           LoopbackAccessInterfaceProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file LoopbackAccessInterfaceProtocol.hpp
//!
//! Declares LoopbackAccessInterfaceProtocol class
//!
//===========================================================================

#ifndef LOOPBACKACCESSINTERFACEPROTOCOL_H__7EA95A6E_19D8_4B68_B28A_6C501BCEC0FA__INCLUDED_
  #define LOOPBACKACCESSINTERFACEPROTOCOL_H__7EA95A6E_19D8_4B68_B28A_6C501BCEC0FA__INCLUDED_

#include "AccessInterfaceProtocol.hpp"
#include "Utility.hpp"

namespace mast
{
//! Implements AccessInterfaceProtocol that only loop back.
//!
//! @note This is mainly provided for tests purposes
//!
class DLL_EXPORT LoopbackAccessInterfaceProtocol : public AccessInterfaceProtocol
{
  // ---------------- Public  Methods
  //
  public:
  ~LoopbackAccessInterfaceProtocol() = default;
  LoopbackAccessInterfaceProtocol()  = default;

  //! Does any action required to transfer scan data to and from SUT
  //!
  //! @param derivationId   Identifies the derivation to act for (zero based)
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfer to SUT
  //!
  //! @return Bits stream retrieved from SUT
  virtual BinaryVector DoAction(uint32_t derivationId, void* interfaceData, const BinaryVector& toSutData) override;

  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
};
//
//  End of LoopbackAccessInterfaceProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined LOOPBACKACCESSINTERFACEPROTOCOL_H__7EA95A6E_19D8_4B68_B28A_6C501BCEC0FA__INCLUDED_

//===========================================================================
// End of LoopbackAccessInterfaceProtocol.hpp
//===========================================================================
