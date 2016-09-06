//===========================================================================
//                           Zybo.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Zybo.hpp
//!
//! Declares functions for demos using Zybo board
//!
//===========================================================================


#ifndef ZYBO_H__9398CFAE_487B_47F1_5B82_379918C2340C__INCLUDED_
  #define ZYBO_H__9398CFAE_487B_47F1_5B82_379918C2340C__INCLUDED_

#include <vector>

namespace mast
{
  struct ApplicationAssociation;
}

namespace test
{
  std::vector<mast::ApplicationAssociation> Zybo_CreateApplicationsDescriptor();
} // End of namespace mast

#endif  // not defined ZYBO_H__9398CFAE_487B_47F1_5B82_379918C2340C__INCLUDED_
//===========================================================================
// End of Zybo.hpp
//===========================================================================



