//===========================================================================
//                           STIL_Player.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file STIL_Player.hpp
//!
//! Declares STIL_Player class
//!
//===========================================================================


#ifndef STIL_PLAYER_H__CE0DE4BD_2B23_4BD0_82B1_DCE5A1CD5B2__INCLUDED_
  #define STIL_PLAYER_H__CE0DE4BD_2B23_4BD0_82B1_DCE5A1CD5B2__INCLUDED_

#include "AccessInterfaceProtocol.hpp"
#include "Platform.hpp"

#include <string>


namespace mast
{
//! Constructs STIL commands for STIL related AccessInterfaceProtocol
//!
class DLL_EXPORT STIL_Player : public AccessInterfaceProtocol
{
  // ---------------- Public  Methods
  //
  public:
  ~STIL_Player() = default;
  STIL_Player() = delete;
  STIL_Player(uint32_t n_chains) {m_n_chains = n_chains;}

  //! Returns true when TRST JTAG port is supported
  //!
  bool  SupportTRST() const { return m_supportTRST; }

  //! Sets whether TRST JTAG port is supported
  //!
  void  SupportTRST (bool supportTRST) { m_supportTRST = supportTRST; }


  //! Creates an STIL command associated to derivation identifier and BinaryVector to send to SUT
  //!
  std::vector<std::string> CreateSTILCommand(uint32_t derivationId, const BinaryVector& toSutData) const;

  //! Creates an STIL reset command
  //!
  std::string CreateResetSTILCommand (bool doSynchronousReset) const ;


  // ---------------- Private  Fields
  //
  bool m_supportTRST = false; //!< When true, TRST instruction is used for reset unless synchronous reset is requested
  uint32_t m_n_chains = 0; //!< When true, TRST instruction is used for reset unless synchronous reset is requested
};
//
//  End of STIL_Player class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined STIL_PLAYER_H__CE0DE4BD_2B23_4BD0_82B1_DCE5A1CD5B2__INCLUDED_

//===========================================================================
// End of STIL_Player.hpp
//===========================================================================
