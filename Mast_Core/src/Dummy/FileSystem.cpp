//===========================================================================
//                           FileSystem.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file FileSystem.cpp
//!
//! Implements dummy class FileSystem for platform that have no support for it
//!
//===========================================================================

#include "FileSystem.hpp"
#include "g3log/g3log.hpp"

using std::string;

using namespace mast;

//! Report not supported on current platform
//!
string FileSystem::CurrentPath()
{
  CHECK_FAILED("No support, for current platform, to tell current directory");
}

//! Tells whether some path is a directory or not
//!
bool FileSystem::IsDirectory(const string& /* path */)
{
  CHECK_FAILED("No support, for current platform, to tell whether a path match a directory");
}
//
//  End of: FileSystem::IsDirectory
//---------------------------------------------------------------------------




//===========================================================================
// End of FileSystem.cpp
//===========================================================================
