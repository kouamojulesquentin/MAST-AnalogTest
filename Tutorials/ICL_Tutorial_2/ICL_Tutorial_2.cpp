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
//! Sets mode register chunk successively to red, green and blue values
//!
void DoTest_Modes (string_view registerPath)
{
  auto red           = "4'b0011"sv;
  auto green         = "4'b0100"sv;
  auto blue          = "4'b1000"sv;
  auto previousColor = "4'b0000"sv;

  for (const auto& color : {red, green, blue})
  {
    iWrite(registerPath, color);

    auto curValue    = iGet(registerPath, StringType::Binary);
    auto curValueBV  = BinaryVector::CreateFromString(curValue);
    auto curValueIcl = curValueBV.DataAsICLMixString();

    LOG(INFO) << "PDL algorithm Wrote color: " << color       << " to   " << registerPath;
    LOG(INFO) << "PDL algorithm Read:        " << curValueIcl << " from " << registerPath;
    iApply();

    if (curValueIcl != previousColor)
    {
      LOG(ERROR_LVL) << "Failed to read back previous color " << previousColor << " from " << registerPath;
    }
    previousColor = color;
  }
}
//
//  End of: DoTest_Modes
//---------------------------------------------------------------------------


//! Sets data register chunk with different values
//!
//! @note Data is taken from same register than another test for modes !
void DoTest_Data (string_view registerPath)
{
  auto previousData = "3'b001"sv; // LSB is one because register is reset with value 1 that is also LSB of data !

  for (const auto& data : {"3'b000"sv, "3'b001"sv, "3'b010"sv, "3'b110"sv})
  {
    iWrite(registerPath, data);

    auto curValue    = iGet(registerPath, StringType::Binary);
    auto curValueBV  = BinaryVector::CreateFromString(curValue);
    auto curValueIcl = curValueBV.DataAsICLMixString();

    LOG(INFO) << "PDL algorithm Wrote data:  " << data        << " to   " << registerPath;
    LOG(INFO) << "PDL algorithm Read:        " << curValueIcl << " from " << registerPath;
    iApply();

    if (curValueIcl != previousData)
    {
      LOG(ERROR_LVL) << "Failed to read back previous data " << previousData << " from " << registerPath;
    }
    previousData = data;
  }
}
//
//  End of: DoTest_Data
//---------------------------------------------------------------------------




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
  repo.RegisterAlgorithm("Test_Modes_1", []() { DoTest_Modes("Daisy.WI1.mode"sv); });
  repo.RegisterAlgorithm("Test_Modes_2", []() { DoTest_Modes("Daisy.WI2.mode"sv); });
  repo.RegisterAlgorithm("Test_Data",    []() { DoTest_Data ("Daisy.WI2.data"sv); });

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
