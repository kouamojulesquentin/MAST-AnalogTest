//===========================================================================
//                           ICL_Tutorial_1.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ICL_Tutorial_1.cpp
//!
//! Implements PDL algorithms of ICL tutorial 1
//!
//===========================================================================

#include "PDL_AlgorithmsRepository.hpp"
#include "BinaryVector.hpp"
#include "CPP_API.hpp"
#include "g3log/g3log.hpp"

#include <string>
#include <experimental/string_view>
#include <iostream>

using std::string;
using std::experimental::string_view;
using namespace std::experimental::literals::string_view_literals;
using namespace mast;

namespace
{
//! CPP Wrapping of PDL commands
//!
void PDL_routine ()
{
  auto CDR = "CDR";
  auto CODEC_DRC = "CODEC_DRC";
  auto PATTERN_COUNTER = "PATTERN_COUNTER";
  auto OCC = "OCC";
  auto WDR = "WDR";
  auto MISR = "MISR";

  iWrite (CDR,"0b0011010101000000");
  iApply();
  iWrite (CODEC_DRC,"0b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000"sv);
  iApply();
  iWrite (PATTERN_COUNTER, 40);
  iApply();

  iWrite (OCC,"0b100000000000"sv);
  iApply();
  iWrite (WDR,"0b000000000000000010011100000011100001110100110111000010001111111000001110100101110001110100110100110011101011100110010100");
  iApply();
  iWrite (WDR,"0b000000000000110101001010011101011111110111011110001000111101010110110100011011101110101101111110001000101101110110010011");
  iApply();
  //  iRunLoop 800;
  iRead (MISR,"0b01100011001010000100101001110111");
  iApply();
  iWrite (PATTERN_COUNTER, 40);
  iApply();
  iWrite (OCC,"0b100000000000");
  iApply();
  iWrite (WDR,"0b000000000000000010000100100011110011010000110110001111010000011101000111001101011111110101001011100010110001000110110100");
  iApply();
  iWrite (WDR,"0b000000000000110101011010111001111110001111001101001001001111101011100001001011111111110000000000111011100011010101111101");
  iApply();
//  iRunLoop 800;
  iRead (MISR,"0b11000001100001010000011101110111");
  iApply();
  iWrite (PATTERN_COUNTER, 40);
  iApply();
  iWrite (OCC,"0b100000000000");
  iApply();
  iWrite (WDR,"0b000000000000000010000000000101011111011011100110000100111001000001011010111100010100101011011001101010001010101111110101");
  iApply();
  iWrite (WDR,"0b000000000000110101001100100010000010001111001001010001110100100000011011110100010101101111001100101101100001111101001001");
  iApply();
//  iRunLoop 800;
  iRead (MISR,"0b10100110000100001001111001100011");
  iApply();
  iWrite (PATTERN_COUNTER, 40);
  iApply();
  iWrite (OCC,"0b100000000000");
  iApply();
  iWrite (WDR,"0b000000000000000010011001010111011001111010110011100111111011101100101011110001010110110111101000000001000011110000101011");
  iApply();
  iWrite (WDR,"0b000000000000110101010001011100111010100100011001001010110010001111100011101001011100110011010111111100100100101010000101");
  iApply();
//  iRunLoop 800;
  iRead (MISR,"0b10110011001000001011010000111101");
  iApply();
  iWrite (PATTERN_COUNTER, 40);
  iApply();
  iWrite (OCC,"0b100000000000");
  iApply();
  iWrite (WDR,"0b000000000000000010010011010101100101000111001011101000100110100010011101111000101010101101101001110101001000101011101011");
  iApply();
  iWrite (WDR,"0b000000000000110101011110111110101001011010110100110101011110010011000110010100000101010000110001110011011100101001100111");
  iApply();
//  iRunLoop 800;
  iRead (MISR,"0b10000000001000011111111111111011");
  iApply();
}

//! Registers PDL algorithm functions in this file
//!
//! @note Names used from registration must be the same as found in ICL file
//! @note Both tests will run concurrently
bool RegisterAlgorithms ()
{
  // ---------------- Get an handle on PDL algorithm repository
  //
  auto& repo = PDL_AlgorithmsRepository::Instance();

  // ---------------- Do register algorithm(s) with a name
  //
  repo.RegisterAlgorithm("PDL_routine", []() { PDL_routine(); });

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
// End of ICL_Tutorial_1.cpp
//===========================================================================
