//===========================================================================
//                           Mast_App.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Mast_App.cpp
//!
//! Implements Mast main function
//!
//===========================================================================

#include "MastEnvironment.hpp"
#include <stdexcept>
#include <iostream>

using std::cout;
using std::endl;

using namespace mast;

//! Runs Mast according to command line arguments
//!
int main (int argc, const char* argv[])
{
  int retCode = 0;
  try
  {
    MastEnvironment mast;

    mast.ParseOptions(argc, argv);
    mast.LoadPlugins();
    mast.CreateSystemModel();
    mast.CreateManager();
    mast.CreateApplications();
  }
  catch(std::exception& exc) { retCode = -1; cout << "\n\nMast got: " << exc.what() << endl; }
  catch(...)                 { retCode = -1; cout << "\n\nMast got non std::exception" << endl; }

  return retCode;
}
//
//  End of: main
//---------------------------------------------------------------------------


//===========================================================================
// End of Mast_App.cpp
//===========================================================================
