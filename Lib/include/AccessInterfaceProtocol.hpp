//===========================================================================
//                           AccessInterfaceProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterfaceProtocol.hpp
//!
//! Declares class AccessInterfaceProtocol
//!
//===========================================================================


#ifndef ACCESSINTERFACEPROTOCOL_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_
  #define ACCESSINTERFACEPROTOCOL_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

namespace mast
{
//! Defines interface for protocols used by AccessInterface
//!
class AccessInterfaceProtocol final
{
  // ---------------- Public  Methods
  //
  public:

  //! Does any action required to transfert scan data to and from SUT
  //!
  //! @param derivationId   Identifies the derivation to act for (zero based)
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfert to SUT
  //!
  //! @return Bits stream retrieved from SUT
  virtual BinaryVector DoAction(uint32_t derivationId, void* interfaceData, const BinaryVector& toSutData) = 0;

  // ---------------- Protected Methods
  //
  protected:
  virtual ~AccessInterfaceProtocol() = default;
  AccessInterfaceProtocol()          = default;
};
//
//  End of AccessInterfaceProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined ACCESSINTERFACEPROTOCOL_H__B0FE4245_A913_4634_F1BD_725570BFDC80__INCLUDED_

//===========================================================================
// End of AccessInterfaceProtocol.hpp
//===========================================================================
