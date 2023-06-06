//===========================================================================
//                           TriviumStreamerProtocolFactory.cpp
//===========================================================================
// Copyright (C) 2020 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file TriviumStreamerProtocolFactory.cpp
//!
//! Implements class TriviumStreamerProtocolFactory
//!
//===========================================================================

#include "TriviumStreamerProtocol.hpp"
#include "StreamerProtocolFactory.hpp" 
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
  bool StreamerProtocolFactory ()
  {
  
    auto& repo     = StreamerProtocolFactory::Instance();
    auto  factory  = [](const std::string& parameters)
    { return std::make_unique<TriviumStreamerProtocol>( parameters); };

   repo.RegisterCreator("Trivium", factory);

    return true;
  }

  //! Make the StreamerProtocol effectivelly registered
  //!
  //! @note As a "static" variable, it is initialized once when the corresponding DLL is loaded
  //!
  static bool registered = StreamerProtocolFactory();
} // End of unnamed namespace

//===========================================================================
// End of StreamerProtocolFactory.cpp
//===========================================================================
