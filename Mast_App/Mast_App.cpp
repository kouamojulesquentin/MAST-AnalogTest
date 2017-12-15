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


//+ (begin JFC December/15/2017): Workaround ==> It is useful to avoid corrupted istream while ICL_Lexer operate (right at the start of parsing
//+                               I've not found the real cause of the problem, so there is a high risk that it will blew up again
//+                               I've only seen the problem for Linux but I'm not using the same gcc version (4.9.3 on Windows, 5.4 on Linux)
#include "ICL_Reader.hpp"
#include <memory>
std::shared_ptr<mast::SystemModel> sm_not_used;
ICL::ICL_Reader                    workaround(sm_not_used);
//+ (end   JFC December/15/2017):



//! Runs Mast according to command line arguments
//!
int main (int argc, const char* argv[])
{
  int retCode = 0;
  try
  {
    MastEnvironment mast;

    //  Detailed version:
//+    mast.ParseOptions(argc, argv);
//+    mast.LoadPlugins();
//+    mast.CreateSystemModel();
//+    mast.CreateManager();
//+    mast.CreateApplications();
//+    mast.Start();

    //  Single line version:
    mast.Start(argc, argv);
  }
  catch(std::exception& exc) { retCode = -1; cout << "\n\n" << exc.what() << endl; }
  catch(...)                 { retCode = -1; cout << "\n\nUnexpected error" << endl; }

  return retCode;
}
//
//  End of: main
//---------------------------------------------------------------------------


//===========================================================================
// End of Mast_App.cpp
//===========================================================================
