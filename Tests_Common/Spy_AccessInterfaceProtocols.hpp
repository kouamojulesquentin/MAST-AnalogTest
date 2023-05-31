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
  virtual mast::BinaryVector DoCallback(uint32_t channelId, void* interfaceData, const mast::BinaryVector& toSutData) override;

  const std::vector<mast::BinaryVector>& ToSutVectors() const { return m_toSutVectors; }

  //! Gets the number of channels supported by the specific protocol
  //!
  //! @note Channel id 0 is reserved for reset operation, so protocol must support a least two channels
  //!
  //! @return The number of supported channel (including pseudo channel 0 for reset)
  //!
  virtual uint32_t MaxSupportedChannels() const override { return 10u; }  // 10 is just used to be able to report that an AccessInterface as fewer channel than is supported by its interface

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "Loopback_Spy"; }

  //! Forces the ResetPort to be asserted on the target module
  //!
  virtual void DoReset(bool doSynchronousReset) override;

  // ---------------- Private  Fields
  //
  private:
  std::vector<mast::BinaryVector> m_toSutVectors; //!< Collected vectors "send" to SUT by SystemModelManager
  uint32_t                        m_resetCount = 0;
};
//
//  End of Spy_AccessInterfaceProtocols class declaration
//---------------------------------------------------------------------------
} // End of namespace test




#endif  // not defined SPY_ACCESSINTERFACEPROTOCOLS_H__97588AB0_EF4_487C_30A1_1B5D899EDEEB__INCLUDED_

//===========================================================================
// End of Spy_AccessInterfaceProtocols.hpp
//===========================================================================
