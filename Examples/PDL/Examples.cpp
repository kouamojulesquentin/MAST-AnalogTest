//===========================================================================
//                           Examples.cpp
//===========================================================================
// Copyright (C) 2023 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Examples.cpp
//!
//! Implements PDL algorithms for Examples
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

  LOG(INFO) << "PDL algorithm \"Incr\" Will do " << loopCount << " iWrites on register \"" << registerPath << "\"";

  for (int cycle = 0 ; cycle < loopCount ; ++cycle)
  {
    iWrite(registerPath, initialValue);
    auto curValue = iGet<uint16_t>(registerPath);

    LOG(INFO) << "PDL algorithm \"Incr\": Cycle " << cycle << ": Wrote " << initialValue ;
    LOG(INFO) << "PDL algorithm \"Incr\": Cycle " << cycle << ": Read "  << curValue ;
    iApply();

    ++initialValue;
  }
}
//
//  End of: Increment
//---------------------------------------------------------------------------

  //! This algorithm increment "reg"
  //!
  void Algo_Increment ()
  {

    auto     registerPath = "reg";
    auto     loopCount    = 5u;
    auto     i    = 0u;
    uint16_t initialValue = 1u;
    uint16_t curValue;

   curValue=iGetRefresh<uint16_t>(registerPath);
    std::cout << "\nINCREMENT Running " << loopCount << " iWrites on register " <<registerPath << "\n";
    iNote(iNoteType::Comment,"Let's go!");
    while (i++<loopCount)
    {
      iWrite(registerPath, initialValue);
      iApply();
      curValue=iGet<uint16_t>(registerPath);
     std::cout << "\nINCREMENT Cycle "<< i << ": Wrote " << initialValue ;
     std::cout << "\nINCREMENT       "<< i << ": Read " <<std::hex<< curValue <<"\n" ;

      ++initialValue;
    }
     std::cout << "\n" ;
  }
  //
  //  End of: Algo_Increment
  //---------------------------------------------------------------------------


  //! This algorithm decrement "regLO"
  //!
  void Algo_Decrement ()
  {
    auto     registerPath = "reg";
    auto     loopCount    = 5u;
    uint16_t initialValue = 60u;
    uint16_t curValue;
    auto     i    = 0u;

    curValue=iGetRefresh<uint16_t>(registerPath);
    std::cout << "\nDECREMENT Running " << loopCount << " iWrites on register " <<registerPath << "\n";
    while (i++<loopCount)
    {
      iWrite(registerPath, initialValue);
      curValue=iGet<uint16_t>(registerPath);
      std::cout << "\nDECREMENT Cycle "<< i << ": Wrote " << initialValue ;
      std::cout << "\nDECREMENT       "<< i << ": Read " << curValue ;
      iApply();

      --initialValue;
    }
  }
  //
  //  End of: Algo_Decrement
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
  repo.RegisterAlgorithm("Console_Incr", Algo_Increment);
  repo.RegisterAlgorithm("Console_Decr", Algo_Decrement);

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
// End of Examples.cpp
//===========================================================================
