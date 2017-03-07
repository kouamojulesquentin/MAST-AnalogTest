//===========================================================================
//                           DefaultMastMain.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DefaultMastMain.cpp
//!
//! Implements default MAST "main" application
//!
//===========================================================================


#include "Session.hpp"
#include "AppFunctionAndNodePath_CPP.hpp"
#include "AppFunctionAndName_CPP.hpp"
#include "SystemModelAdapter_CPP.hpp"
#include "DefaultMastMain.hpp"
#include "Utility.hpp"

#include <vector>
#include <string>
#include <iostream>
using std::vector;
using std::string;

using namespace mast;

extern void                          Init                      (int argc, char* argv []); //!< Does special init for specific example (can do nothing)
extern string                        GetSitModelFilePath       (int argc, char* argv []); //!< Returns path for SIT file describing System Model
extern vector<AppFunctionAndName>    GetAlgorithmsNames        (int argc, char* argv []); //!< Returns associations between a PDL algorithm function and a name
extern vector<AppFunctionAndNodePath> GetAlgorithmsAndNodePaths (int argc, char* argv []); //!< Returns associations be ween a PDL algorithm function and a node path


using namespace mast;

//! Runs some mast example main
//!
//! @note This is just a skeleton that can be shared between different kind of examples
//! @note - Init may do something to initialize the example
//!       - GetSitModelFilePath must return a valid SIT file path
//!       - Either GetAlgorithmsNames or GetAlgorithmsAndNodePaths must return non empty association
//!
int DefaultMastMain (int argc, char* argv [])
{
  auto retCode = 0;

  try
  {
    auto session     = Session(true);  // True enable logging

    Init(argc, argv);

    auto sitFilePath = GetSitModelFilePath (argc, argv);

    if (sitFilePath.empty())
    {
      throw std::runtime_error("A valid SIT file path must be provided !");
    }

    if (!Utility::FileExists(sitFilePath))
    {
      auto withPathPrefix = string("SIT/") + sitFilePath;
      if (Utility::FileExists(withPathPrefix))
      {
        sitFilePath = withPathPrefix;
      }

      if (!Utility::FileExists(sitFilePath))
      {
        throw std::runtime_error(string("Cannot find SIT file: ") + sitFilePath);
      }
    }

    std::cout << "Using SIT file: " << sitFilePath << std::endl;

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
    std::cout << std::endl << "Mast example done" << std::endl;
  }
  catch(std::invalid_argument& exc) { retCode = -1; std::cout << exc.what() << std::endl; }
  catch(std::out_of_range&     exc) { retCode = -1; std::cout << exc.what() << std::endl; }
  catch(std::logic_error&      exc) { retCode = -1; std::cout << exc.what() << std::endl; }
  catch(std::runtime_error&    exc) { retCode = -1; std::cout << exc.what() << std::endl; }
  catch(std::exception&        exc) { retCode = -1; std::cout << exc.what() << std::endl; }
  catch(...)                        { retCode = -1; std::cout << "Got non std::exception" << std::endl; }

  return retCode;
}
//
//  End of: DefaultMastMain
//---------------------------------------------------------------------------



//===========================================================================
// End of DefaultMastMain.cpp
//===========================================================================
