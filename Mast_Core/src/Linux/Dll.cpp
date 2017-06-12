//===========================================================================
//                           Dll.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Dll.cpp
//!
//! Implements class Dll for Linux platform
//!
//===========================================================================

#include "Dll.hpp"
#include "Utility.hpp"
#include "g3log/g3log.hpp"

#include <sstream>
#include <dlfcn.h>
#include <sys/types.h>
#include <dirent.h>

#include <experimental/string_view>
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

  DIR* handle = ::opendir(directoryPath.c_str());

  CHECK_PARAMETER_NOT_NULL(handle, "Failed to open directory: " + directoryPath);

  auto atScopeExit = MakeScopeExit([handle]() { ::closedir(handle); });

  struct dirent* dirp;
  while ((dirp = ::readdir(handle)))
  {
    auto fileName = string_view(dirp->d_name);
    if (Utility::EndsWith(fileName, ".so"sv))
    {
      dlls.emplace_back(fileName);
    }
  }

  return dlls;
}
//
//  End of: Dll::GetInDirectory
//---------------------------------------------------------------------------

//! Tries loading a DLL
//!
//! @param hintPath Hint for path when dllPath cannot be found
//! @param dllPath  Path to DLL to load.
//!                 If does not end with ".so", ".so" is appended to the path
//!                 If it does not exist and does not begin with "lib", path is prefixed with "lib"
//!
//! @return Effective path of loaded DLL, empty string otherwise
//!
string Dll::TryLoad (const std::string& pathHint, const string& dllPath)
{
  auto fixedPath = dllPath;

  auto checkFileExist = [pathHint](string& fixedPath)
  {
    auto fileExists        = false;
    auto effectiveFilePath = ""s;

    tie(fileExists, effectiveFilePath) = FileExists(pathHint, fixedPath);
    if (fileExists)
    {
      fixedPath = effectiveFilePath;
    }
    return fileExists;
  };

  auto fileExists = checkFileExist(fixedPath);

  // ---------------- Deal with possibly missing ".so" extension
  //
  if (!fileExists && !Utility::EndsWith(fixedPath, ".so"sv))
  {
    LOG(INFO) << "Forcing extension \".so\" to dll file: " << fixedPath;
    fixedPath.append(".so");
    fileExists = checkFileExist(fixedPath);
  }

  // ---------------- Deal with possibly missing "lib" prefix
  //
  if (!fileExists)
  {
    auto   namePos = fixedPath.rfind("/");
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

      fileExists = checkFileExist(fixedPath);
    }
  }

  if (!Utility::FileExists(fixedPath))
  {
    fixedPath.clear();  // To report failure of loading dll file
  }
  else
  {
    // ---------------- Try to open the dll file
    //
    auto handle = ::dlopen(fixedPath.c_str(), RTLD_NOW);

    if (!handle)
    {
      LOG(INFO) << "Failed to load, as a DLL, file: \"" << fixedPath << "\", got error: " << ::dlerror();
      fixedPath.clear();
    }
  }

  return fixedPath;
}
//
//  End of: Dll::TryLoad
//---------------------------------------------------------------------------




//===========================================================================
// End of Dll.cpp
//===========================================================================
