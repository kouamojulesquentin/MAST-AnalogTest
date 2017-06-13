//===========================================================================
//                           FileSystem.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file FileSystem.cpp
//!
//! Implements FileSystem utilities for Linux platform
//!
//===========================================================================

#include "FileSystem.hpp"
#include "Utility.hpp"
#include "g3log/g3log.hpp"
#include <dirent.h>
#include <unistd.h>

using std::string;

using namespace mast;


//! Returns the absolute path of the current working directory, obtained as if
//! (in native format) by POSIX getcwd
//!
string FileSystem::CurrentPath()
{
  auto buffer = ::getcwd(nullptr, 0);

  CHECK_VALUE_NOT_NULL(buffer, "Failed to get current working directory. Got errno: "s + std::to_string(errno));

  auto atExit      = MakeScopeExit([buffer](){ std::free(buffer);});
  auto currentPath = string(buffer);
  return currentPath;
}

//! Tells whether some path is a directory or not
//!
bool FileSystem::IsDirectory(const string& path)
{
  if (path.empty())
  {
    return false;
  }

  auto pDir        = ::opendir (path.c_str());
  auto isDirectory = pDir != nullptr;

  if (pDir)
  {
    ::closedir(pDir);
  }

  return isDirectory;
}
//
//  End of: FileSystem::IsDirectory
//---------------------------------------------------------------------------




//===========================================================================
// End of FileSystem.cpp
//===========================================================================
