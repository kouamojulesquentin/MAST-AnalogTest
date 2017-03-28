//===========================================================================
//                           PluginsRegistry.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PluginsRegistry.cpp
//!
//! Implements class PluginsRegistry
//!
//===========================================================================

#include "PluginsRegistry.hpp"

using std::vector;
using std::string;

using test::PluginsRegistry;

vector<string> PluginsRegistry::m_registeredPlugins;



//! Forgets any registered plugins
//!
void PluginsRegistry::Clear ()
{
  m_registeredPlugins.clear();
}
//
//  End of: PluginsRegistry::Clear
//---------------------------------------------------------------------------



//! Registers that some plugin has been loaded correctly
//!
//! @param pluginName   Plugin name
//!
void PluginsRegistry::Register (const string& pluginName)
{
  m_registeredPlugins.emplace_back(pluginName);
}
//
//  End of: PluginsRegistry::Register
//---------------------------------------------------------------------------


//! Returns currently registered plugins
//!
const vector<string>& PluginsRegistry::RegisteredPlugins()
{
  return m_registeredPlugins;
}
//
//  End of PluginsRegistry::RegisteredPlugins
//---------------------------------------------------------------------------



//===========================================================================
// End of PluginsRegistry.cpp
//===========================================================================
