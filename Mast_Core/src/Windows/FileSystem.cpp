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
#include "g3log/g3log.hpp"
#include <FileAPI.h>

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
