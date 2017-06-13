//===========================================================================
//                           UT_FileSystem.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_FileSystem.cpp
//!
//! Implements test fixture for testing FileSystem
//!
//===========================================================================

#include "UT_FileSystem.hpp"
#include "FileSystem.hpp"
#include "TestUtilities.hpp"

#include <cxxtest/ValueTraits.h>

using mast::FileSystem;

//! Checks FileSystem::IsDirectory() when giving empty path
//!
void UT_FileSystem::test_IsDirectory_when_EmptyPath ()
{
  // ---------------- Exercise
  //
  auto isDirectory = FileSystem::IsDirectory("");

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (isDirectory);
}


//! Checks FileSystem::IsDirectory() when it is not
//!
void UT_FileSystem::test_IsDirectory_when_ItIsNot ()
{
  // ---------------- Exercise
  //
  auto isDirectory = FileSystem::IsDirectory("There is little chance this is a real directory !");

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (isDirectory);
}


//! Checks FileSystem::IsDirectory() when it is indeed a directory
//!
void UT_FileSystem::test_IsDirectory_when_ItIs ()
{
  // ---------------- Setup
  //
  auto runnerPath = test::GetRunnerDirectoryPath();

  // ---------------- Exercise
  //
  auto isDirectory = FileSystem::IsDirectory(runnerPath);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (isDirectory);
}


//! Checks FileSystem::CurrentPath()
//!
void UT_FileSystem::test_CurrentPath ()
{
  // ---------------- Exercise
  //
  auto currentWorkingDirectory = FileSystem::CurrentPath();

  // ---------------- Verify
  //
  TS_ASSERT_FALSE (currentWorkingDirectory.empty());
}



//===========================================================================
// End of UT_FileSystem.cpp
//===========================================================================
