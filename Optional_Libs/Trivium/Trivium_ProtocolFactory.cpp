//===========================================================================
//                           Trivum_ProtocolFactory.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Trivium_ProtocolFactory.cpp
//!
//! Implements class Trivium_EmulationProtocol
//!
//===========================================================================

#include "Trivium_TranslatorProtocol.hpp"
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
    auto  factory  = [](const string& key_and_iv) { return std::make_unique<Trivium_TranslatorProtocol>(key_and_iv); };

    repo.RegisterCreator("Trivium", factory);

    return true;
  }

  //! Make the AccessInterface protocol effectivelly registered
  //!
  //! @note As a "static" variable, it is initialized once when the corresponding DLL is loaded
  //!
  static bool registered = AccessInterfaceTranslatorProtocolFactory();
} // End of unnamed namespace

//===========================================================================
// End of TriviumProtocolFactory.cpp
//===========================================================================
