//===========================================================================
//                           Tap.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Tap.hpp
//!
//! Declares class Tap
//!
//===========================================================================

#ifndef TAP_H__E62C6689_A493_41D3_FDA6_4A9CD299FDD7__INCLUDED_
  #define TAP_H__E62C6689_A493_41D3_FDA6_4A9CD299FDD7__INCLUDED_

#include "AccessInterface.hpp"

namespace mast
{
//! Represent JTAG Test Access Point
//!
class Tap : public AccessInterface
{
  // ---------------- Public  Methods
  //
  public:
  ~Tap() = default;
  Tap()  = default;

  virtual void Accept (SystemModelVisitor& visitor) override; //!< Visited part of the Visitor pattern

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
//  End of Tap class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined TAP_H__E62C6689_A493_41D3_FDA6_4A9CD299FDD7__INCLUDED_

//===========================================================================
// End of Tap.hpp
//===========================================================================
