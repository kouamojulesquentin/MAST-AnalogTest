//===========================================================================
//                           Chain.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Chain.hpp
//!
//! Declares class Chain
//!
//===========================================================================

#ifndef CHAIN_H__461011B4_5E2_44E5_8B9F_F0B3C575D7B5__INCLUDED_
  #define CHAIN_H__461011B4_5E2_44E5_8B9F_F0B3C575D7B5__INCLUDED_

#include "ParentNode.hpp"

namespace mast
{
//! Represents a fix chain of registers
//!
//!
class Chain : public ParentNode
{
  // ---------------- Public  Methods
  //
  public:
  ~Chain() = default;
  Chain()  = default;


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
//  End of Chain class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined CHAIN_H__461011B4_5E2_44E5_8B9F_F0B3C575D7B5__INCLUDED_

//===========================================================================
// End of Chain.hpp
//===========================================================================
