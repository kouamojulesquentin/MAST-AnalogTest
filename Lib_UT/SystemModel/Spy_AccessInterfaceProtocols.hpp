//===========================================================================
//                           Spy_AccessInterfaceProtocols.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Spy_AccessInterfaceProtocols.hpp
//!
//! Declares Spy_AccessInterfaceProtocols class
//!
//===========================================================================


#ifndef SPY_ACCESSINTERFACEPROTOCOLS_H__97588AB0_EF4_487C_30A1_1B5D899EDEEB__INCLUDED_
  #define SPY_ACCESSINTERFACEPROTOCOLS_H__97588AB0_EF4_487C_30A1_1B5D899EDEEB__INCLUDED_

#include "BinaryVector.hpp"
#include "AccessInterfaceProtocol.hpp"

namespace test
{
//! Spies calls to AccessInterfaceProtocol
//!
class Spy_AccessInterfaceProtocols final : public mast::AccessInterfaceProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Spy_AccessInterfaceProtocols() = default;
  Spy_AccessInterfaceProtocols()  = default;

  //! Spies content of parameter toSutData and return it unchanged
  //!
  virtual mast::BinaryVector DoAction(uint32_t derivationId, void* interfaceData, const mast::BinaryVector& toSutData) override;

  const std::vector<mast::BinaryVector>& ToSutVectors() const { return m_toSutVectors; }

  // ---------------- Private  Fields
  //
  private:
  std::vector<mast::BinaryVector> m_toSutVectors; //!< Collected vectors "send" to SUT by SystemModelManager
};
//
//  End of Spy_AccessInterfaceProtocols class declaration
//---------------------------------------------------------------------------
} // End of namespace test




#endif  // not defined SPY_ACCESSINTERFACEPROTOCOLS_H__97588AB0_EF4_487C_30A1_1B5D899EDEEB__INCLUDED_

//===========================================================================
// End of Spy_AccessInterfaceProtocols.hpp
//===========================================================================
