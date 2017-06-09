//===========================================================================
//                           Plugins.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Plugins.hpp
//!
//! Declares Plugins class
//!
//===========================================================================


#ifndef PLUGINS_H__EEF4BAD3_11BA_451A_629D_B5CA8437CD1__INCLUDED_
  #define PLUGINS_H__EEF4BAD3_11BA_451A_629D_B5CA8437CD1__INCLUDED_

#include "Mast_Core_export.hpp"

#include <string>
#include <vector>


namespace mast
{
//! Manages search and load of plugins DLL for MAST
//!
//! @note MAST plugins are expected to register their feature by themselve using MAST factories
class MAST_CORE_EXPORT Plugins final
{
  // ---------------- Public  Methods
  //
  public:

  //! Returns all plugins found in specified directory
  //!
  static std::vector<std::string> GetPlugins(const std::string& directoryPath);

  //! Loads all plugins found in specified directory
  //!
  static uint32_t LoadPlugins(const std::string& directoryPath);

  //! Loads a plugin file
  //!
  static void LoadPlugin(const std::string& pluginPath);

  // ---------------- Private  Methods
  //
  private:
  ~Plugins() = default;
  Plugins()  = default;
};
//
//  End of Plugins class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined PLUGINS_H__EEF4BAD3_11BA_451A_629D_B5CA8437CD1__INCLUDED_

//===========================================================================
// End of Plugins.hpp
//===========================================================================
