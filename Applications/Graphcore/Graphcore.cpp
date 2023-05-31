//===========================================================================
//                           graphcore.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Graphcore.cpp
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

void generate_patterns (string registerPath, uint16_t registerSize, uint16_t loopCount)
  {
   auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
   auto word_rand = std::bind(std::uniform_int_distribution<int>(0,1<<registerSize),
                           mt19937(seed));

    auto     i    = 0u;

//    std::cout << "\nMaking " << loopCount << " random iWrites on register " <<registerPath << "\n";

    while (i<loopCount)
   {
     iWrite(registerPath, word_rand());
     iApply();
     i++;
    }
  }


  void graphcore_8 ()
  {
    auto     register_name = "reg_8";
    auto     registerSize = 8;
    
     generate_patterns(register_name,registerSize,10); 

  }


  //
  //  End of: graphcore
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
  repo.RegisterAlgorithm("graphcore_8", graphcore_8);

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
// End of graphcore.cpp
//===========================================================================
