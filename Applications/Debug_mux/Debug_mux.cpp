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

void Check_Register (string registerPath, uint16_t registerSize, uint16_t loopCount)
  {

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
     iApply();
     curValue=iGet<uint16_t>(registerPath);

     std::cout << "\n Cycle "<< i << ": Wrote "<<std::hex << initialValue ;
     std::cout << "\n       "<< i << ": Read " <<std::hex<< curValue ;
     if  (writtenValue == curValue)      std::cout << "\n  OK" ; else std::cout << "\n  NOT OK" ;
     writtenValue = initialValue;
     i++;
    }
     curValue=iGetRefresh<uint16_t>(registerPath);
    std::cout << "\n       "<< i << ": Read " <<std::hex<< curValue ;
     if  (writtenValue == curValue)      std::cout << "\n  OK" ; else std::cout << "\n  NOT OK" ;
     std::cout << "\n" ;
  }


  void Debug_mux ()
  {
    auto     register_1 = "reg_1";
    auto     register_2 = "reg_2";
    auto     registerSize = 8;
    bool mismatch = false;

    uint16_t curValue_1;
    uint16_t writtenValue_1;
    uint16_t curValue_2;
    uint16_t writtenValue_2;

   auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
   auto word_rand = std::bind(std::uniform_int_distribution<int>(0,1<<registerSize),
                           mt19937(seed));


    std::cout << "\nMaking random iWrites on registers  " <<register_1 <<" and "<< register_1 << "\n";

    writtenValue_1 = word_rand();
    writtenValue_2 = word_rand();
    iWrite(register_1, writtenValue_1);
    iWrite(register_2, writtenValue_2);
    iApply();
    
     curValue_1=iGetRefresh<uint16_t>(register_1);
     curValue_2=iGetRefresh<uint16_t>(register_2);

     if  (writtenValue_1 == curValue_1)      std::cout << register_1 << "  OK\n"; else
        { std::cout << register_1 << " ERROR: wrote "<<writtenValue_1<<" read "<<curValue_1<<"\n";mismatch= true;}
     if  (writtenValue_2 == curValue_2)      std::cout << register_2 << "  OK\n"; else
       {std::cout << register_2 << " ERROR: wrote "<<writtenValue_2<<" read "<<curValue_2<<"\n";;mismatch= true;}
       
     if (mismatch)
      {
       if (curValue_1 == writtenValue_2) std::cout << "\tPossible Stuck at 1 in Mux\n";
       if (curValue_2 == writtenValue_1) std::cout << "\tPossible Stuck at 0 in Mux\n";
      }  

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
