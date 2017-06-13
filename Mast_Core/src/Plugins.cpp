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

#include <algorithm>
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
  vector<string> notToLoadPaths;
  auto loadedPath = LoadPluginsExcept(directoryPath, notToLoadPaths);
  return loadedPath.size();
}
//
//  End of: Plugins::LoadPlugins
//---------------------------------------------------------------------------



//! Loads all plugins found in specified directory except for those specified (usually already loaded)
//!
//! @param directoryPath  Path to directory holding plugins files (*.dll or *.so)
//! @param notToLoadPaths Paths of plugins to ignore (to not load)
//!
//! @return List of loaded plugins
vector<string> Plugins::LoadPluginsExcept (const string& directoryPath, const vector<string>& notToLoadPaths)
{
  vector<string> loadedPlugins;

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

      // ---------------- Ignore this file if it is in the ignore list
      //
      if (std::find(notToLoadPaths.cbegin(), notToLoadPaths.cend(), dllPath) != notToLoadPaths.cend())
      {
        LOG(DEBUG) << "Ignoring plugin file: " << dllPath;
        continue;
      }

      LoadPlugin(dllPath);
      loadedPlugins.emplace_back(dllPath);
      LOG(DEBUG) << "Have loaded plugin: " << dllPath;
    }
    catch(std::runtime_error& exc)  // Catch C++ standard exceptions
    {
      LOG(WARNING) << "Failed to load \"" << dllFile << "\" as a plugin DLL "
                   << "(only plugins should be in directory \"" << directoryPath << "\")"
                   << "==> " << exc.what();
    }
  }

  if (loadedPlugins.empty())
  {
    LOG(DEBUG) << "No plugin(s) found in directory: " << directoryPath;
  }
  else
  {
    LOG(DEBUG) << "Have loaded " << loadedPlugins.size() << " plugin(s) from directory: " << directoryPath;
  }
  return loadedPlugins;
}
//
//  End of: Plugins::LoadPluginsExcept
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
