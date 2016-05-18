//===========================================================================
//                           AccessInterfaceProtocol_1149_1.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterfaceProtocol_1149_1.hpp
//!
//! Declares AccessInterfaceProtocol_1149_1 class
//!
//===========================================================================


#ifndef ACCESSINTERFACEPROTOCOL_1149_1_H__A048AC53_C7DE_47DE_E5AB_F9A8AB33D31F__INCLUDED_
  #define ACCESSINTERFACEPROTOCOL_1149_1_H__A048AC53_C7DE_47DE_E5AB_F9A8AB33D31F__INCLUDED_

#include "AccessInterfaceProtocol.hpp"

namespace mast
{
//! Implements AccessInterfaceProtocol for tap (1149.1)
//!
class AccessInterfaceProtocol_1149_1 : public AccessInterfaceProtocol
{
  // ---------------- Public  Methods
  //
  public:
  ~AccessInterfaceProtocol_1149_1() = default;
  AccessInterfaceProtocol_1149_1()  = default;

  //! Does any action required to transfert scan data to and from SUT
  //!
  //! @param derivationId   Identifies the derivation to act for (zero based)
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfert to SUT
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
//  End of AccessInterfaceProtocol_1149_1 class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined ACCESSINTERFACEPROTOCOL_1149_1_H__A048AC53_C7DE_47DE_E5AB_F9A8AB33D31F__INCLUDED_

//===========================================================================
// End of AccessInterfaceProtocol_1149_1.hpp
//===========================================================================
