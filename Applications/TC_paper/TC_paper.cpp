//===========================================================================
//                           TC_paper.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file TC_paper.cpp
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

void TC_example (string registerPath, uint16_t registerSize, uint16_t loopCount, int16_t Value)
  {

    auto     i    = 1u;

    std::cout << "\nMaking " << loopCount << " random iWrites on register " <<registerPath << "\n";

    iWrite(registerPath, Value);
    iApply();
    std::cout << "\n Cycle "<< 0 << ": Wrote " <<std::hex<< Value << std::endl;
    
    while (i<loopCount)
    {
     iWrite(registerPath, Value);
     iApply();
    // curValue=iGet<uint16_t>(registerPath);
   }
  }


  void TC_paper_1 ()
  {
    auto     register_name = "reg_";
    auto     registerSize = 8;
    auto     Value = 0x01;
    
     TC_example(register_name+to_string(Value),registerSize,1,Value); 

  }

  void TC_paper_2 ()
  {
    auto     register_name = "reg_";
    auto     registerSize = 8;
    auto     Value = 0x02;
    
     TC_example(register_name+to_string(Value),registerSize,1,Value); 

  }

  void TC_paper_3 ()
  {
    auto     register_name = "reg_";
    auto     registerSize = 8;
    auto     Value = 0x03;
    
     TC_example(register_name+to_string(Value),registerSize,1,Value); 

  }

  void TC_paper_4 ()
  {
    auto     register_name = "reg_";
    auto     registerSize = 8;
    auto     Value = 0x04;
    
     TC_example(register_name+to_string(Value),registerSize,1,Value); 

  }

  //
  //  End of: TC_paper
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
  repo.RegisterAlgorithm("TC_paper_1", TC_paper_1);
  repo.RegisterAlgorithm("TC_paper_2", TC_paper_2);
  repo.RegisterAlgorithm("TC_paper_3", TC_paper_3);
  repo.RegisterAlgorithm("TC_paper_4", TC_paper_4);

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
// End of TC_paper.cpp
//===========================================================================
