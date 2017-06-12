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
#include "g3log/g3log.hpp"
#include <dirent.h>

using std::string;

using namespace mast;

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
