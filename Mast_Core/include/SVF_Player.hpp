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
#include "Platform.hpp"

#include <string>


namespace mast
{
//! Constructs SVF commands for SVF related AccessInterfaceProtocol
//!
class DLL_EXPORT SVF_Player : public AccessInterfaceProtocol
{
  // ---------------- Public  Methods
  //
  public:
  ~SVF_Player() = default;
  SVF_Player()  = default;


  // ---------------- Protected Methods
  //
  protected:

  //! Creates an SVF command associated to derivation identifier and BinaryVector to send to SUT
  //!
  std::string CreateSVFCommand(uint32_t derivationId, const BinaryVector& toSutData);
};
//
//  End of SVF_Player class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SVF_PLAYER_H__CE0DE4BD_2B23_4BD0_82B1_DCE5A1CD5B2__INCLUDED_

//===========================================================================
// End of SVF_Player.hpp
//===========================================================================
