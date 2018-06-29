//===========================================================================
//                           Debug_mux.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Debug_mux.cpp
//!
//! Implements PDL algorithms of tutorial 1
//!
//===========================================================================

#include "PDL_AlgorithmsRepository.hpp"
#include "CPP_API.hpp"
#include "g3log/g3log.hpp"

#include <iostream>
#include <random>
#include <functional>
#include <chrono>

using namespace mast;
using namespace std;


namespace
{

  //! This algorithm increment "reg"
  //!
  void Debug_mux ()
  {

    auto     registerPath = "reg_1";
    auto     registerSize = 12;
    auto     loopCount    = 5u;
    auto     i    = 1u;
    uint16_t curValue;
    uint16_t writtenValue;

   auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
   auto word_rand = std::bind(std::uniform_int_distribution<int>(0,1<<registerSize),
                           mt19937(seed));


    std::cout << "\nMaking " << loopCount << " random iWrites on register " <<registerPath << "\n";

    uint16_t initialValue = word_rand();
    iWrite(registerPath, initialValue);
    writtenValue = initialValue;
    iApply();
    std::cout << "\n Cycle "<< 0 << ": Wrote " <<std::hex<< initialValue ;
    
    while (i<loopCount)
    {
      initialValue=word_rand();
      iWrite(registerPath, initialValue);
    curValue=iGet<uint16_t>(registerPath);
      iApply();
     std::cout << "\n Cycle "<< i << ": Wrote "<<std::hex << initialValue ;
     std::cout << "\n       "<< i << ": Read " <<std::hex<< curValue ;
     if  (writtenValue == curValue)      std::cout << "\n  OK" ; else std::cout << "\n  NOT OK" ;
     writtenValue = initialValue;


     i++;
    }
     std::cout << "\n" ;
  }
  //
  //  End of: Debug_mux
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
  repo.RegisterAlgorithm("Debug_mux", Debug_mux);

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
// End of Debug_mux.cpp
//===========================================================================
