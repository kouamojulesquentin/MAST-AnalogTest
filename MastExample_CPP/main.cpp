//===========================================================================
//                           main.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file main.cpp
//!
//! Defines main for testcases application
//===========================================================================

#include "Session.hpp"
#include "AppFunctionAndNodePath_CPP.hpp"
#include "AppFunctionAndName_CPP.hpp"
#include "SystemModelAdapter_CPP.hpp"

#include "SystemModelManager.hpp"

#include <vector>
#include <string>
#include <iostream>
using std::vector;

using namespace mast;

extern std::string                               GetSitModelFilePath       (int argc, char* argv []);
extern std::vector<mast::AppFunctionAndName>     GetAlgorithmsNames        (int argc, char* argv []);
extern std::vector<mast::AppFunctionAndNodePath> GetAlgorithmsAndNodePaths (int argc, char* argv []);

//! Runs example
//!
int main (int argc, char* argv [])
{
  auto retCode = 0;

  try
  {
    auto session = Session (std::make_shared<SystemModelManagerMonitor>());
    auto sitFilePath = GetSitModelFilePath (argc, argv);

    if (sitFilePath.empty())
    {
      throw std::runtime_error("A valid SIT file path must be provided !");
    }

    auto algoAndNames = GetAlgorithmsNames  (argc, argv);

    auto options =  RunMastOptions::CheckModel
                  | RunMastOptions::PrintModelGraph
                  | RunMastOptions::LogManagerActivity;

    if (!algoAndNames.empty())
    {
      std::cout << "Have " << std::to_string(algoAndNames.size()) << " algorithm(s) identified by their names" << std::endl;
      RunMast(sitFilePath, algoAndNames, options);
    }
    else
    {
      auto algoAndPath = GetAlgorithmsAndNodePaths(argc, argv);
      if (!algoAndPath.empty())
      {
        std::cout << "Have " << std::to_string(algoAndPath.size()) << " algorithm(s) associated to node paths" << std::endl;
        RunMast(sitFilePath, algoAndPath, options);
      }
      else
      {
        throw std::runtime_error("Cannot run Mast without any algorithm to start");
      }
    }
    std::cout << "Mast example done\n";
  }
  catch(std::invalid_argument& exc) { retCode = -1; std::cout << exc.what(); }
  catch(std::out_of_range&     exc) { retCode = -1; std::cout << exc.what(); }
  catch(std::logic_error&      exc) { retCode = -1; std::cout << exc.what(); }
  catch(std::runtime_error&    exc) { retCode = -1; std::cout << exc.what(); }
  catch(std::exception&        exc) { retCode = -1; std::cout << exc.what(); }
  catch(...)                        { retCode = -1; std::cout << "Got non std::exception"; }

  return retCode;
}
//
//  End of: main
//---------------------------------------------------------------------------



//===========================================================================
// End of main.cpp
//===========================================================================



