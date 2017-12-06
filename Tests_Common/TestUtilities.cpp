//===========================================================================
//                           TestUtilities.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file TestUtilities.cpp
//!
//! Implements utilities for unit testing
//!
//===========================================================================

#include "TestUtilities.hpp"
#include "MastConfig.hpp"
#include "SystemModel.hpp"
#include "SystemModelNode.hpp"
#include "SystemModelBuilder.hpp"
#include "LoopbackAccessInterfaceProtocol.hpp"

#include <cxxtest/TestSuite.h>
#include <fstream>
#include <iostream>

using std::string;
using std::initializer_list;
using std::shared_ptr;
using std::make_shared;

using namespace mast;

extern string Exe_Dir_Path;

//! Returns file path given a test file name and names of sub-dir to it
//!
//! @param subPathsToFile Names of directory down to file to used in test
//! @param checkExists    When true, file reality is checked (useful for input files)
//!
string test::GetTestFilePath(const initializer_list<std::string>& subPathsToFile, bool checkExists)
{
  CxxTest::CountAssertDisabler assertDisabler;  // To not count the assertion below as a caller test assertion

  auto makePath = [](string first, initializer_list<string> otherParts)
  {
    string filePath(first);
    for (const auto& part : otherParts)
    {
      filePath.append(DIRECTORY_SEPARATOR).append(part);
    }
    return filePath;
  };

  auto filePath = makePath(Exe_Dir_Path, subPathsToFile);
  if (checkExists)
  {
    TS_ASSERT_FILE_EXISTS (filePath);
  }
  return filePath;
}

//! Gets full content of some text file except last new line
//!
//! @param filePath   Text file path to read
//!
string test::GetTextFileContent (const string& filePath)
{
  std::ifstream ifs(filePath);

  string content;
  content.reserve(1000);  // Limit number of reallocations

  string line;
  auto   first = true;

  while (std::getline(ifs, line))
  {
    if (!first)
    {
      content.append("\n");
    }
    else
    {
      first = false;
    }

    if (!line.empty())
    {
      line.pop_back();        // Remove platform EOL terminating character
      content.append(line);
      line.clear();
    }
  }

  return content;
}

//! Returns directory path of currently running test runner
//!
const string& test::GetRunnerDirectoryPath()
{
  return Exe_Dir_Path;
}

//! Adds a JTAG Tap to system mode, then append node to it
//!
void test::PrependWithTap (shared_ptr<SystemModel> sm, shared_ptr<SystemModelNode> node)
{
  auto protocol = make_shared<LoopbackAccessInterfaceProtocol> ();
  SystemModelBuilder builder(*sm);

  auto tap = builder.Create_JTAG_TAP("TAP", 8u, 2u, protocol);

  sm->ReplaceRoot(tap, false);
  tap->AppendChild(node);
}
//
//  End of: PrependWithTap
//---------------------------------------------------------------------------


namespace
{

} // End of unnamed namespace



//===========================================================================
// End of TestUtilities.cpp
//===========================================================================
