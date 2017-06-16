//===========================================================================
//                           Tutorial_1.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Tutorial_1.cpp
//!
//! Implements PDL algorithms of tutorial 1
//!
//===========================================================================

#include "PDL_AlgorithmsRepository.hpp"
#include "CPP_API.hpp"
#include "g3log/g3log.hpp"

#include <iostream>

using namespace mast;


namespace
{
//! Increments the associated register value
//!
void Increment ()
{
  auto     registerPath = "reg";
  auto     loopCount    = 5u;
  uint16_t initialValue = 1u;

  LOG(INFO) << "\"Increment\" PDL algorithm Will do " << loopCount << " iWrites on register \"" << registerPath << "\"";

  for (int cycle = 0 ; cycle < loopCount ; ++cycle)
  {
    iWrite(registerPath, initialValue);
    auto curValue = iGet<uint16_t>(registerPath);

    LOG(INFO) << "Cycle " << cycle << ": Wrote " << initialValue ;
    LOG(INFO) << "Cycle"  << cycle << ": Read "  << curValue ;
    iApply();

    ++initialValue;
  }
}
//
//  End of: Increment
//---------------------------------------------------------------------------



//! Registers PDL algorithm functions in this file
//!
//! @note Names used from registration must be the same as found in SIT file
bool RegisterAlgorithms ()
{
  // ---------------- Get an handle on PDL algorithm repository
  //
  auto& repo = PDL_AlgorithmsRepository::Instance();

  // ---------------- Do register algorithm(s) with a name
  //
  repo.RegisterAlgorithm("Incr", Increment);

  return true;
}
//
//  End of: RegisterAlgorithms
//---------------------------------------------------------------------------


//! Make PDL algorithm functions in this file to be registered
//!
//! @note As a "static" variable, it is initialized once when the corresponding DLL is loaded
//!
bool registrated = RegisterAlgorithms();

} // End of unnamed namespace


//===========================================================================
// End of Tutorial_1.cpp
//===========================================================================
