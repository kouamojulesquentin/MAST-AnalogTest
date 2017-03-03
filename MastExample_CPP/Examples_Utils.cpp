//===========================================================================
//                           Examples_Utils.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Examples_Utils.cpp
//!
//! Implements utility functions to be used by examples
//===========================================================================

#include "Examples_Utils.hpp"
#include "Utility.hpp"

#include <iostream>

using std::string;
using std::experimental::string_view;

//! Gets from command line arguments, the one that match some prefix
//!
//! @param argc         Numbers of argument
//! @param argv         Arguments
//! @param paramPrefix  Arguments prefix to look for
//!
//! @return An argument with its prefix removed when found empty string otherwise
string GetFromArgs (int argc, char* argv[], string_view paramPrefix)
{
  string argumentValue;

  for (int ii = 1 ; ii < argc ; ++ii)
  {
    string_view argument(argv[ii]);

//+    std::cout << "argv[" << ii << "] = " << argument << std::endl;

    if (argument.find(paramPrefix) == 0)
    {
      argument.remove_prefix(paramPrefix.length());
      mast::Utility::TrimBoth(argument);
      argumentValue = string(argument);
      break;
    }
  }

  return argumentValue;
}
//
//  End of: GetFromArgs
//---------------------------------------------------------------------------


//===========================================================================
// End of Examples_Utils.cpp
//===========================================================================
