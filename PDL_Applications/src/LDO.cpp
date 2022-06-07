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
#define en_on 		0x80 
#define pden_on 	0x40 
#define en_off 		0x00 
#define pden_off 	0x00 

  //! This algorithm increment "reg"
  //!
  void LDO_Demo ()
  {

    auto     registerPath = "LDO1_CTRL";
    auto     AnalogPort = "vout_s";
    auto     loopCount    = 5u;
    auto     i    = 0u;
    uint16_t initialValue = 1u;
    uint16_t curValue;

    uint8_t regvalue=0x0;
    
    regvalue = en_off+pden_on;
    
    std::cout << "\nStarting LDO Test\n";
    std::cout << "LDO off, Pullup on\n";
    iNote(iNoteType::Comment,"LDO off, Pullup on");
    iWrite("LDO1_CTRL", regvalue);
    iApply();

    std::cout << "ReadData from Analog Port\n";
    iNote(iNoteType::Comment,"ReadData from Analog Port");
    auto PortValue = iGetRefresh<uint8_t>(AnalogPort);
    iApply();


     std::cout << "LDO Test Finished\n" ;
  }
  //
  //  End of: Algo_Increment
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
  repo.RegisterAlgorithm("LDO_PDL", LDO_Demo);

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
