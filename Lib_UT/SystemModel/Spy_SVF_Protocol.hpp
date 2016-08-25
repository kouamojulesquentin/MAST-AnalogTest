//===========================================================================
//                           Spy_SVF_Protocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Spy_SVF_Protocol.hpp
//!
//! Declares Spy_SVF_Protocol class
//!
//===========================================================================


#ifndef SPY_SVF_PROTOCOL_H__7720E2EF_FEE3_4D05_C4B6_A5E4CB999B6__INCLUDED_
  #define SPY_SVF_PROTOCOL_H__7720E2EF_FEE3_4D05_C4B6_A5E4CB999B6__INCLUDED_

#include "SVF_Player.hpp"
#include "BinaryVector.hpp"

namespace test
{
//! Spies calls to SVF based AccessInterfaceProtocol
//!
class Spy_SVF_Protocol final : public mast::SVF_Player
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Spy_SVF_Protocol() = default;
  Spy_SVF_Protocol()  = default;

  //! Spies content how binary vector to SUT is transformed to SVF command while returning the BinaryVector unchanged
  //!
  virtual mast::BinaryVector DoAction(uint32_t derivationId, void* interfaceData, const mast::BinaryVector& toSutData) override;

  const std::vector<std::string>& SVFCommands() const { return m_commands; }

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "SVF_Spy"; }

  // ---------------- Private  Fields
  //
  private:
  std::vector<std::string> m_commands; //!< Collected SVF commands issued from vectors "send" to SUT by SystemModelManager
};
//
//  End of Spy_SVF_Protocol class declaration
//---------------------------------------------------------------------------
} // End of namespace test


#endif  // not defined SPY_SVF_PROTOCOL_H__7720E2EF_FEE3_4D05_C4B6_A5E4CB999B6__INCLUDED_

//===========================================================================
// End of Spy_SVF_Protocol.hpp
//===========================================================================
