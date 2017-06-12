//===========================================================================
//                           Plugins.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Plugins.cpp
//!
//! Implements class Plugins
//!
//===========================================================================

#include "Plugins.hpp"
#include "Dll.hpp"
#include "g3log/g3log.hpp"
#include "Utility.hpp"
#include "MastConfig.hpp"

using std::string;
using std::vector;

using namespace mast;


//! Returns all plugins found in specified directory
//!
//! @param directoryPath  Path to directory holding plugin files (*.dll or *.so)
//!
vector<string> Plugins::GetPlugins (const string& directoryPath)
{
  return Dll::GetInDirectory(directoryPath);
}
//
//  End of: Plugins::GetPlugins
//---------------------------------------------------------------------------


//! Loads a plugin file
//!
//! @param pluginPath Path to the plugin to load
//!
string Plugins::LoadPlugin (const string& pluginPath)
{
  return Dll::Load(pluginPath);
}
//
//  End of: Plugins::LoadPlugin
//---------------------------------------------------------------------------



//! Loads all plugins found in specified directory
//!
//! @param directoryPath  Path to directory holding plugins files (*.dll or *.so)
//!
uint32_t Plugins::LoadPlugins (const string& directoryPath)
{
  uint32_t loadedCount = 0;

  auto dllFiles = Dll::GetInDirectory(directoryPath);
  auto dllPath  = directoryPath;
  dllPath.append(DIRECTORY_SEPARATOR);
  auto baseLength = dllPath.length();

  for (const auto& dllFile : dllFiles)
  {
    try
    {
      dllPath.resize(baseLength);   // Keep only directory base path (with ending separator)
      dllPath.append(dllFile);
      LoadPlugin(dllPath);
      ++loadedCount;
      LOG(INFO) << "Have loaded plugin: " << dllPath;
    }
    catch(std::runtime_error& exc)  // Catch C++ standard exceptions
    {
      LOG(WARNING) << "Failed to load \"" << dllFile << "\" as a plugin DLL "
                   << "(only plugins should be in directory \"" << directoryPath << "\")"
                   << "==> " << exc.what();
    }
  }

  if (loadedCount == 0)
  {
    LOG(INFO) << "No plugin(s) found in directory: " << directoryPath;
  }
  else
  {
    LOG(INFO) << "Have loaded " << loadedCount << " plugin(s) from directory: " << directoryPath;
  }
  return loadedCount;
}
//
//  End of: Plugins::LoadPlugins
//---------------------------------------------------------------------------

//! Tries loading a plugin file with hint path
//!
//! @param hintPath   Hint for path when pluginPath cannot be found
//! @param pluginPath Path to the plugin to load
//!
string Plugins::TryLoadPlugin (const string& hintPath, const string& pluginPath)
{
  return Dll::TryLoad(hintPath, pluginPath);
}
//
//  End of: Plugins::LoadPlugin
//---------------------------------------------------------------------------


//===========================================================================
// End of Plugins.cpp
//===========================================================================
