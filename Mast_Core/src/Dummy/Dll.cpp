//===========================================================================
//                           Dll.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Dll.cpp
//!
//! Implements dummy class Dll for platform that have no support for it
//!
//===========================================================================

#include "Dll.hpp"
#include "Utility.hpp"
#include "g3log/g3log.hpp"


#include <experimental/string_view>
using std::experimental::string_view;
using std::string;
using std::vector;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;
using namespace mast;



//! Returns empty list of DLLs
//!
//! @param directoryPath  Path to directory to search *.so
//!
vector<string> Dll::GetInDirectory (const std::string& directoryPath)
{
  CHECK_FAILED("No support, for current platform, to get Dll in directory: "s + directoryPath);
  LOG(WARNING) << "No support, for current platform, to get Dll in directory: \"" << directoryPath << "\"" ;
  return vector<string>();
}
//
//  End of: Dll::GetInDirectory
//---------------------------------------------------------------------------



//! Does nothing instead of loading a DLL
//!
//! @param dllPath  Path to DLL to load.
//!                 If does not end with ".so", ".so" is appended to the path
//!                 If it does not exist and does not begin with "lib", path is prefixed with "lib"
//!
void Dll::Load (const string& dllPath)
{
  CHECK_FAILED("No support, for current platform, to load Dll: "s + dllPath);
  LOG(WARNING) << "No support, for current platform, to load Dll: \"" << dllPath << "\"" ;
}
//
//  End of: Dll::Load
//---------------------------------------------------------------------------




//===========================================================================
// End of Dll.cpp
//===========================================================================
