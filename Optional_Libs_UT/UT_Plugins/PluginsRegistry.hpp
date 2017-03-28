//===========================================================================
//                           PluginsRegistry.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PluginsRegistry.hpp
//!
//! Declares PluginsRegistry class
//!
//===========================================================================


#ifndef PLUGINSREGISTRY_H__7E9CFCAB_843E_4242_CA2_8004EE0DC83F__INCLUDED_
  #define PLUGINSREGISTRY_H__7E9CFCAB_843E_4242_CA2_8004EE0DC83F__INCLUDED_

#include "Platform.hpp"

#include <vector>
#include <string>

namespace test
{
//! Provides a way to check registered plugins
//!
class DLL_EXPORT PluginsRegistry final
{
  // ---------------- Public  Methods
  //
  public:
  ~PluginsRegistry() = delete;
  PluginsRegistry()  = delete;

  //! Registers that some plugin has been loaded correctly
  //!
  static void Register(const std::string& pluginName);

  //! Forgets any registered plugins
  //!
  static void Clear();

  //! Returns currently registered plugins
  //!
  static const std::vector<std::string>& RegisteredPlugins();

  // ---------------- Private  Fields
  //
  private:
  static std::vector<std::string>  m_registeredPlugins;
};
//
//  End of PluginsRegistry class declaration
//---------------------------------------------------------------------------
} // End of namespace test




#endif  // not defined PLUGINSREGISTRY_H__7E9CFCAB_843E_4242_CA2_8004EE0DC83F__INCLUDED_

//===========================================================================
// End of PluginsRegistry.hpp
//===========================================================================
