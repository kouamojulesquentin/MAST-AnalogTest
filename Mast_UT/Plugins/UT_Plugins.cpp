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

#define TS_ASSERT_STR_CONTAINS(text, sub_text)                                                              \
    if ((text).find((sub_text)) == std::string::npos)                                                       \
    {                                                                                                       \
      TS_FAIL(string(#text).append(": ").append((text)).append(", does not contain: ").append((sub_text))); \
    }                                                                                                       \
    else                                                                                                    \
    {                                                                                                       \
      TS_ASSERT_TRUE (true);                                                                                \
    }


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



//! Checks Plugins::LoadPlugin() when file does not exist
//!
void UT_Plugins::test_LoadPlugin_not_Exists ()
{
  // ---------------- Setup
  //
  auto pluginPath = PluginPath("This_file_should_not_exist_as_a_dll"s);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (Plugins::LoadPlugin(pluginPath), std::exception);
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
  //! @todo [JFC]-[June/13/2017]: When available, unload the library (and check that if possible by system (calling specific function))
  //!
}


//! Checks Plugins::TryLoadPlugin() when file does not exist
//!
void UT_Plugins::test_TryLoadPlugin_not_Exists ()
{
  // ---------------- Setup
  //
  auto hintPath   = "This_directory_path_should_not_exist"s;
  auto pluginPath = "This_file_should_not_exist_as_a_dll"s;

  PluginsRegistry::Clear();

  // ---------------- Exercise
  //
  auto effectivePluginPath = Plugins::TryLoadPlugin(hintPath, pluginPath);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE (effectivePluginPath.empty());
  const auto& registered = PluginsRegistry::RegisteredPlugins();
  TS_ASSERT_TRUE (registered.empty());
}


//! Checks Plugins::TryLoadPlugin() when file does exist
//!
void UT_Plugins::test_TryLoadPlugin_Exists ()
{
  // ---------------- Setup
  //
  auto hintPath   = PluginPath("");
  auto pluginPath = "Fake_Plugin_1"s;

  PluginsRegistry::Clear();

  // ---------------- Exercise
  //
  auto effectivePluginPath = Plugins::TryLoadPlugin(hintPath, pluginPath);

  // ---------------- Verify
  //
  TS_ASSERT_STR_CONTAINS (effectivePluginPath, pluginPath);
  TS_ASSERT_STR_CONTAINS (effectivePluginPath, hintPath);

  //+ (JFC June/13/2017): When unloading dll is in place ==> Check that it is effectively loaded
//+  CxxTest::setAbortTestOnFail(true);
//+  const auto& registered = PluginsRegistry::RegisteredPlugins();
//+  TS_ASSERT_EQUALS (registered.size(), 1u);
//+  TS_ASSERT_EQUALS (registered[0],     "Fake_Plugin_1");
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


//! Checks Plugins::LoadPluginsExcept() when directory does not exist
//!
void UT_Plugins::test_TryLoadPluginsExcept_not_Exists ()
{
  // ---------------- Setup
  //
  auto           pluginsDirectory = "This_directory_path_should_not_exist"s;
  vector<string> ignored;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (Plugins::LoadPluginsExcept(pluginsDirectory, ignored), std::exception);
}


//! Checks Plugins::LoadPluginsExcept() when plugin is ignored
//!
void UT_Plugins::test_TryLoadPluginsExcept_Exists_Ignored ()
{
  // ---------------- Setup
  //
  auto           pluginsDirectory = PluginPath();
  vector<string> loaded;
  auto           effectivePluginPath = Plugins::LoadPlugin(PluginPath("Fake_Plugin_1"s));
  vector<string> ignored { effectivePluginPath };

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (loaded = Plugins::LoadPluginsExcept(pluginsDirectory, ignored));

  // ---------------- Verify
  //
  TS_ASSERT_GREATER_THAN_EQUALS (loaded.size(), 0u);
}


//! Checks Plugins::LoadPluginsExcept() when plugin is not ignored
//!
void UT_Plugins::test_TryLoadPluginsExcept_Exists_not_Ignored ()
{
  // ---------------- Setup
  //
  auto           pluginsDirectory = PluginPath();
  vector<string> loaded;
  vector<string> ignored { "This_does_not_match_a_file_path" };

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (loaded = Plugins::LoadPluginsExcept(pluginsDirectory, ignored));

  // ---------------- Verify
  //
  TS_ASSERT_GREATER_THAN_EQUALS (loaded.size(), 1u);
}


//===========================================================================
// End of UT_Plugins.cpp
//===========================================================================
