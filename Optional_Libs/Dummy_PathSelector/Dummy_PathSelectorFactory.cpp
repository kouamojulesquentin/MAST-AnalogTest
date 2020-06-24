//===========================================================================
//                           Dummy_PathSelectorFactory.cpp
//===========================================================================
// Copyright (C) 2020 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Dummy_PathSelectorFactory.cpp
//!
//! Implements class Dummy_PathSelectorFactory
//!
//===========================================================================

#include "Dummy_PathSelector.hpp"
#include "PathSelectorFactory.hpp" 
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
  bool PathSelectorFactory ()
  {
  
    auto& repo     = PathSelectorFactory::Instance();
    auto  factory  = [](uint32_t pathsCount, 
  					const std::string& parameters, 
					std::shared_ptr<Register> associatedRegister)
    { return std::make_unique<Dummy_PathSelector>(associatedRegister, pathsCount, parameters); };

   repo.RegisterCreator("Dummy", factory);

    return true;
  }

  //! Make the PathSelector effectivelly registered
  //!
  //! @note As a "static" variable, it is initialized once when the corresponding DLL is loaded
  //!
  static bool registered = PathSelectorFactory();
} // End of unnamed namespace

//===========================================================================
// End of PathSelectorFactory.cpp
//===========================================================================
