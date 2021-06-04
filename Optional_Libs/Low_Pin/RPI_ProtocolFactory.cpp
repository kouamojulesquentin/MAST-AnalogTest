//===========================================================================
//                           RPI_ProtocolFactory.cpp
//===========================================================================
// Copyright (C) 2020 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file RPI_ProtocolFactory.cpp
//!
//! Implements class RPI_EmulationProtocol
//!
//===========================================================================

#include "RPI_TranslatorProtocol.hpp"
#include "AccessInterfaceTranslatorProtocolFactory.hpp" 
#include "g3log/g3log.hpp"

#include "Utility.hpp"
#include <thread>
#include <chrono>
#include <memory>
 
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
  bool AccessInterfaceTranslatorProtocolFactory ()
  {
    auto& repo     = AccessInterfaceTranslatorProtocolFactory::Instance();
    auto  factory  = [](const string& /* parameters */)  { return std::make_unique<RPI_TranslatorProtocol>(); };


   repo.RegisterCreator("NXP_RPI", factory);

    return true;
  }

  //! Make the AccessInterface protocol effectivelly registered
  //!
  //! @note As a "static" variable, it is initialized once when the corresponding DLL is loaded
  //!
  static bool registered = AccessInterfaceTranslatorProtocolFactory();
} // End of unnamed namespace

//===========================================================================
// End of RPI_ProtocolFactory.cpp
//===========================================================================
