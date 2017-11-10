//===========================================================================
//                           Fake_Plugin_1.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Fake_Plugin_1.cpp
//!
//! Implements a fake plugin for unit test to check it is loaded correctly
//!
//===========================================================================

#include "PluginsRegistry.hpp"
#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;

namespace
{
  class AutoRegistration final
  {
    public:
    ~AutoRegistration() = default;
    AutoRegistration()
    {
      test::PluginsRegistry::Register("Fake_Plugin_1");
    }
  };

  AutoRegistration autoRegistration;
} // End of unnamed namespace




//===========================================================================
// End of Fake_Plugin_1.cpp
//===========================================================================
