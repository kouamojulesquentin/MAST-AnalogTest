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

  for (const auto& color : {red, green, blue})
  {
    iWrite(registerPath, color);

    LOG(INFO) << "PDL algorithm Wrote color: " << color       << " to   " << registerPath;
    iApply();

    auto curValue    = iGet(registerPath, StringType::Binary);
    auto curValueBV  = BinaryVector::CreateFromString(curValue);
    auto curValueIcl = curValueBV.DataAsICLMixString();
    LOG(INFO) << "PDL algorithm Read:        " << curValueIcl << " from " << registerPath;

    if (curValueIcl != color)
    {
      LOG(ERROR_LVL) << "Failed to read back color " << color << " from " << registerPath << ", got " << curValueIcl;
    }
  }
}
//
//  End of: DoTest_Modes
//---------------------------------------------------------------------------


//! Sets data register chunk with different values
//!
//! @note Data is taken from same register than another test for modes !
void DoTest_Data (string_view instancePath)
{
  // Lamba: Gets current register value as ICL formatted string
  auto iGetAsIcl = [instancePath](auto registerPath)
  {
    auto current    = iGet(registerPath, StringType::Binary);
    auto currentBV  = BinaryVector::CreateFromString(current);
    auto currentIcl = currentBV.DataAsICLMixString();
    LOG(INFO) << "PDL algorithm Read:        " << currentIcl << " from " << instancePath << "." << registerPath;
    return currentIcl;
  };

  iPrefix(instancePath);
  auto registerPath = "data"sv;
  auto previousData = "3'b001"sv; // LSB is one because register is reset with value 1 that is also LSB of data !

  for (const auto& data : {"3'b000"sv, "3'b001"sv, "3'b010"sv, "3'b110"sv})
  {
    iWrite(registerPath, data);
    LOG(INFO) << "PDL algorithm Wrote data:  " << data        << " to   " << instancePath << "." << registerPath;

    auto curValueIcl = iGetAsIcl(registerPath);

    iApply();

    if (curValueIcl != previousData)
    {
      LOG(ERROR_LVL) << "Failed to read back previous data " << previousData << " from " << registerPath;
    }
    previousData = data;
  }

  // ---------------- Check effect of last write
  //
  auto curOkayIcl = iGetAsIcl("okay");  // okay is on bit 0
  auto curDoneIcl = iGetAsIcl("done");  // done is on bit 1
  if (curOkayIcl != "1'b0")
  {
    LOG(ERROR_LVL) << "Failed to read back proper value for okay, got \"" << curOkayIcl << "\" instead of \"1'b0\"";
  }

  if (curDoneIcl != "1'b1")
  {
    LOG(ERROR_LVL) << "Failed to read back proper value for okay, got \"" << curDoneIcl << "\" instead of \"1'b1\"";
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
  repo.RegisterAlgorithm("Test_Data",    []() { DoTest_Data ("Daisy.WI2"sv); });

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
