//===========================================================================
//                           Dll.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Dll.hpp
//!
//! Declares functions for managing DLLs
//!
//===========================================================================


#ifndef DLL_H__328B051D_756A_4D35_788F_837D15FD2819__INCLUDED_
  #define DLL_H__328B051D_756A_4D35_788F_837D15FD2819__INCLUDED_

#include "Plugins_export.hpp"
#include <string>
#include <vector>

namespace mast
{
  class PLUGINS_EXPORT Dll final
  {
    public:
    Dll() = delete;

    //! Returns list of DLLs in specified directory
    //!
    static std::vector<std::string> GetInDirectory(const std::string& directoryPath);

    //! Loads a Dll
    //!
    static void Load(const std::string& dllPath);
  };
} // End of namespace mast

#endif  // not defined DLL_H__328B051D_756A_4D35_788F_837D15FD2819__INCLUDED_

//===========================================================================
// End of Dll.hpp
//===========================================================================



