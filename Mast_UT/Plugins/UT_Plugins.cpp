//===========================================================================
//                           UT_Plugins.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_Plugins.cpp
//!
//! Implements test fixture for testing Plugins
//!
//===========================================================================

#include "UT_Plugins.hpp"
#include "Plugins.hpp"
#include "PluginsRegistry.hpp"
#include "MastConfig.hpp"
#include "g3log/g3log.hpp"

#include <algorithm>
#include <vector>
#include <string>
#include <experimental/string_view>
#include <cxxtest/ValueTraits.h>

using std::experimental::string_view;
using std::string;
using std::vector;
using mast::Plugins;
using test::PluginsRegistry;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;

extern string Exe_Dir_Path;

namespace
{
  //! Returns path for plugins directory or for a plugin name
  //!
  string PluginPath(string_view pluginName = ""sv)
  {
    string pluginsDirectory;
    pluginsDirectory.append(Exe_Dir_Path).append(DIRECTORY_SEPARATOR).append(PLUGINS_DIRECTORY_NAME);

    if (pluginName.empty())
    {
      return pluginsDirectory;
    }

    auto   path = pluginsDirectory;
    path.append(DIRECTORY_SEPARATOR).append(pluginName.cbegin(), pluginName.cend());

    return path;
  }
}


//! Checks Plugins::GetPlugins()
//!
void UT_Plugins::test_GetPlugins ()
{
  // ---------------- Setup
  //
  auto pluginsDirectory = PluginPath();

  // ---------------- Exercise
  //
  auto plugins = Plugins::GetPlugins(pluginsDirectory);

  // ---------------- Verify
  //
  TS_ASSERT_GREATER_THAN_EQUALS (plugins.size(), 1u);

  auto predicate = [](const string& item) { return item.find("libFake_Plugin_1"s) != string::npos; };
  auto pos       = std::find_if(plugins.cbegin(), plugins.cend(), predicate);

  auto foundFakePlugin = pos != plugins.cend();
  TS_ASSERT_TRUE (foundFakePlugin);
}



//! Checks Plugins::LoadPlugin()
//!
void UT_Plugins::test_LoadPlugin ()
{
  // ---------------- Setup
  //
  auto pluginPath = PluginPath("Fake_Plugin_1"s);

  PluginsRegistry::Clear();

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (Plugins::LoadPlugin(pluginPath));

  // ---------------- Verify
  //
  const auto& registered = PluginsRegistry::RegisteredPlugins();

  TS_ASSERT_EQUALS (registered.size(), 1u);
  TS_ASSERT_EQUALS (registered[0],     "Fake_Plugin_1");
}


//! Checks Plugins::LoadPlugins()
//!
void UT_Plugins::test_LoadPlugins ()
{
  // ---------------- Setup
  //
  auto pluginsDirectory = PluginPath();
  auto loadedCount      = 0u;

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (loadedCount = Plugins::LoadPlugins(pluginsDirectory));

  // ---------------- Verify
  //
  TS_ASSERT_GREATER_THAN_EQUALS (loadedCount, 1u);
}


//===========================================================================
// End of UT_Plugins.cpp
//===========================================================================
