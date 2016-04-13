//===========================================================================
//                           AccessInterface.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterface.hpp
//!
//! Declares class AccessInterface
//!
//===========================================================================

#ifndef ACCESSINTERFACE_H__83C3E64E_7289_4AE9_9AA5_235446B1544__INCLUDED_
  #define ACCESSINTERFACE_H__83C3E64E_7289_4AE9_9AA5_235446B1544__INCLUDED_

#include "ParentNode.hpp"

namespace mast
{
//! Represents an access point to scan chain
//!
class AccessInterface : public ParentNode
{
  // ---------------- Public  Methods
  //
  public:
  ~AccessInterface() = default;
  AccessInterface()  = default;


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
//  End of AccessInterface class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined ACCESSINTERFACE_H__83C3E64E_7289_4AE9_9AA5_235446B1544__INCLUDED_

//===========================================================================
// End of AccessInterface.hpp
//===========================================================================
