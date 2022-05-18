//===========================================================================
//                           OpenOCD_EmulationProtocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file OpenOCD_ProtocolFactory.cpp
//!
//! Implements class OpenOCD_EmulationProtocol
//!
//===========================================================================

#include "OpenOCDProtocol.hpp"
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
    auto  factory  = [](const string& Open_OCD_params) { return std::make_unique<OpenOCDProtocol>(Open_OCD_params); };

    repo.RegisterCreator("SVF_openOCD", factory);

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
