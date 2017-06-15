//===========================================================================
//                           SVF_Player.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SVF_Player.hpp
//!
//! Declares SVF_Player class
//!
//===========================================================================


#ifndef SVF_PLAYER_H__CE0DE4BD_2B23_4BD0_82B1_DCE5A1CD5B2__INCLUDED_
  #define SVF_PLAYER_H__CE0DE4BD_2B23_4BD0_82B1_DCE5A1CD5B2__INCLUDED_

#include "AccessInterfaceProtocol.hpp"
#include "Mast_Core_export.hpp"

#include <string>


namespace mast
{
//! Constructs SVF commands for SVF related AccessInterfaceProtocol
//!
class MAST_CORE_EXPORT SVF_Player : public AccessInterfaceProtocol
{
  // ---------------- Public  Methods
  //
  public:
  ~SVF_Player() = default;
  SVF_Player()  = default;

  //! Returns true when TRST JTAG port is supported
  //!
  bool  SupportTRST() const { return m_supportTRST; }

  //! Sets whether TRST JTAG port is supported
  //!
  void  SupportTRST (bool supportTRST) { m_supportTRST = supportTRST; }


  //! Creates an SVF command associated to endpoint identifier and BinaryVector to send to SUT
  //!
  std::string CreateSVFCommand(uint32_t endpointId, const BinaryVector& toSutData) const;

  //! Creates an SVF reset command
  //!
  std::string CreateResetSVFCommand (bool doSynchronousReset) const ;

  // ---------------- Private  Fields
  //
  bool m_supportTRST = false; //!< When true, TRST instruction is used for reset unless synchronous reset is requested
};
//
//  End of SVF_Player class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SVF_PLAYER_H__CE0DE4BD_2B23_4BD0_82B1_DCE5A1CD5B2__INCLUDED_

//===========================================================================
// End of SVF_Player.hpp
//===========================================================================
