//===========================================================================
//                           Dll.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Dll.cpp
//!
//! Implements class Dll for Windows platform
//!
//===========================================================================

#include "Dll.hpp"
#include "Utility.hpp"
#include "g3log/g3log.hpp"

#include <sstream>
#include <iostream>
#include <Windows.h>
#include <Winbase.h>

using std::experimental::string_view;
using std::string;
using std::vector;
using std::ostringstream;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;
using namespace mast;



//! Returns list of DLLs in specified directory
//!
//! @param directoryPath  Path to directory to search *.so
//!
vector<string> Dll::GetInDirectory (const std::string& directoryPath)
{
  vector<string> dlls;

  // ---------------- Create search expression
  //
  auto searchExpression = directoryPath;
  searchExpression.append("\\*.dll");

  WIN32_FIND_DATA findData;
  auto hFind = ::FindFirstFile(searchExpression.c_str(), &findData);

  CHECK_PARAMETER_NEQ(hFind, INVALID_HANDLE_VALUE, "Failed to open directory: " + directoryPath);

  auto atScopeExit = MakeScopeExit([hFind]() { ::FindClose(hFind); });

  auto isDll = [](string_view fileName) { return Utility::EndsWith(fileName, ".dll"sv); };

  do
  {
    string_view fileName(findData.cFileName);

    if (isDll(fileName))
    {
      dlls.emplace_back(fileName);
    }
  } while (::FindNextFile(hFind, &findData));

  return dlls;
}
//
//  End of: Dll::GetInDirectory
//---------------------------------------------------------------------------



//! Loads a DLL
//!
//! @param dllPath  Path to DLL to load.
//!                 If does not end with ".so", ".so" is appended to the path
//!                 If it does not exist and does not begin with "lib", path is prefixed with "lib"
//!
void Dll::Load (const string& dllPath)
{
  auto fixedPath = dllPath;

  // ---------------- Deal with possibly missing ".so" extension
  //
  if (   !Utility::FileExists(fixedPath)
      && !Utility::EndsWith(fixedPath, ".dll"sv))
  {
    LOG(INFO) << "Forcing extension \".dll\" to dll file: " << fixedPath;
    fixedPath.append(".dll");
  }

  // ---------------- Deal with possibly missing "lib" prefix
  //
  if (!Utility::FileExists(fixedPath))
  {
    auto   namePos = fixedPath.rfind("\\");
    string name    = (namePos != string::npos)
                     ? fixedPath.substr(namePos + 1u)
                     : fixedPath;

    if (!Utility::StartsWith(name, "lib"sv))
    {
      string newPath;
      if (namePos != string::npos)
      {
        newPath.append(fixedPath.substr(0, namePos + 1u));
      }
      newPath.append("lib").append(name);
      fixedPath.swap(newPath);
      LOG(INFO) << "Forcing prefix \"lib\" to dll file name: " << name;
    }
  }

  CHECK_FILE_EXISTS(fixedPath);

  // ---------------- Try to open the dll file
  //
  auto hinstance = ::LoadLibrary(fixedPath.c_str());

  if (!hinstance)
  {
    auto errorCode = ::GetLastError();

    ostringstream os;
    os << "Failed to load dll \"" << fixedPath << "\": " << errorCode;
    CHECK_FAILED(os.str());
  }
}
//
//  End of: Dll::Load
//---------------------------------------------------------------------------




//===========================================================================
// End of Dll.cpp
//===========================================================================
