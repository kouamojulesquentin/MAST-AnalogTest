//===========================================================================
//                           Dll.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Dll.cpp
//!
//! Implements parts class Dll that are platform independent
//!
//===========================================================================

#include "Dll.hpp"
#include "Utility.hpp"
#include "MastConfig.hpp"
#include "g3log/g3log.hpp"

using std::experimental::string_view;
using std::string;
using std::tuple;
using std::make_tuple;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;
using namespace mast;

//! Returns true if file exist with or without a path hint
//!
tuple<bool, string> Dll::FileExists (const string& pathHint, const string& filePath)
{
  if (Utility::FileExists(filePath))
  {
    return make_tuple(true, filePath);
  }

  auto pathWithHint = pathHint + DIRECTORY_SEPARATOR + filePath;
//+  LOG(INFO) << "pathHint: pathWithHint";
  if (Utility::FileExists(pathWithHint))
  {
    return make_tuple(true, pathWithHint);
  }

  return make_tuple(false, ""s);
}
//
//  End of: Dll::FileExists
//---------------------------------------------------------------------------


//! Loads a DLL
//!
//! @param dllPath  Path to DLL to load.
//!                 If does not end with ".so", ".so" is appended to the path
//!                 If it does not exist and does not begin with "lib", path is prefixed with "lib"
//!
string Dll::Load (const string& dllPath)
{
  auto loadedDll = TryLoad("", dllPath);
  if (loadedDll.empty())
  {
    CHECK_FAILED("Failed to load Dll: "s + dllPath);
  }
  return loadedDll;
}
//
//  End of: Dll::Load
//---------------------------------------------------------------------------




//===========================================================================
// End of Dll.cpp
//===========================================================================
