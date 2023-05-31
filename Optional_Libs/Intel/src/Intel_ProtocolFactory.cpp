//===========================================================================
//                           Intel_EmulationProtocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Intel_ProtocolFactory.cpp
//!
//! Implements class Intel_EmulationProtocol
//!
//===========================================================================

#include "Intel_EmulationProtocol.hpp"
#include "AccessInterfaceProtocolFactory.hpp" 
#include "g3log/g3log.hpp"

#include "Utility.hpp"
#include <thread>
#include <chrono>

using std::experimental::string_view;
using std::string;
using std::ofstream;
using std::ifstream;
using namespace mast;
using namespace std::string_literals;

namespace
{
  //! Registers the AccessInterface protocol
  //!
  //! @note Names used from registration must be the same as found in SIT file
  bool RegisterAccessInterfaceProtocolFactory ()
  {
    auto& repo     = AccessInterfaceProtocolFactory::Instance();
    auto  factory  = [](const string& region_addresses) { return make_unique<Intel_EmulationProtocol>("10"); };

    repo.RegisterCreator("Intel_Exp", factory);

    return true;
  }

  //! Make the AccessInterface protocol effectivelly registered
  //!
  //! @note As a "static" variable, it is initialized once when the corresponding DLL is loaded
  //!
  static bool registered = RegisterAccessInterfaceProtocolFactory();
} // End of unnamed namespace

//===========================================================================
// End of IntelProtocolFactory.cpp
//===========================================================================
