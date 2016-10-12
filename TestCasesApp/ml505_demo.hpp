//===========================================================================
//                           ml505_demo.hpp
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


#ifndef ML505_DEMO_H__9398CFAE_487B_47F1_5B82_379918C2340C__INCLUDED_
  #define ML505_DEMO_H__9398CFAE_487B_47F1_5B82_379918C2340C__INCLUDED_

#include <vector>

namespace mast
{
  struct AppFunctionAndNodePath;
}

namespace test
{
  std::vector<mast::AppFunctionAndNodePath> ml505_CreateApplicationsDescriptor();
} // End of namespace mast

#endif  // not defined ML505_DEMO_H__9398CFAE_487B_47F1_5B82_379918C2340C__INCLUDED_
//===========================================================================
// End of ml505_demo.hpp
//===========================================================================



