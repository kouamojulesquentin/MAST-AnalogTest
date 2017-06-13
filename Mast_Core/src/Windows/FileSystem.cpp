//===========================================================================
//                           FileSystem.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file FileSystem.cpp
//!
//! Implements FileSystem utilities for Windows platform
//!
//===========================================================================

#include "FileSystem.hpp"
#include "Utility.hpp"
#include "g3log/g3log.hpp"
#include <FileAPI.h>
#include <direct.h>

using std::string;

using namespace mast;

//! Returns the absolute path of the current working directory, obtained as if
//! (in native format) by POSIX getcwd
//!
string FileSystem::CurrentPath()
{
  auto buffer = ::_getcwd(nullptr, 0);

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

  auto attribs = ::GetFileAttributesA(path.c_str());
  if (attribs == INVALID_FILE_ATTRIBUTES)
  {
    return false;
  }
  return (attribs & FILE_ATTRIBUTE_DIRECTORY) != 0;
}
//
//  End of: FileSystem::IsDirectory
//---------------------------------------------------------------------------




//===========================================================================
// End of FileSystem.cpp
//===========================================================================
