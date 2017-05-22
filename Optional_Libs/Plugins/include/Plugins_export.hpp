//===========================================================================
//                           Plugins_export.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Plugins_export.hpp
//!
//! Defines DLL export dependant stuffs
//===========================================================================

#ifndef PLUGINS_EXPORT_H__ADF2524_327A_4FE2_FFBB_3B59FB1D4887__INCLUDED_
  #define PLUGINS_EXPORT_H__ADF2524_327A_4FE2_FFBB_3B59FB1D4887__INCLUDED_

  #if defined(WINDOWS)
    #include <windows.h>
    #ifdef PLUGINS_EXPORTS
        #define PLUGINS_EXPORT __declspec(dllexport)
    #else
        #define PLUGINS_EXPORT __declspec(dllimport)
    #endif
  #else
      #define PLUGINS_EXPORT
  #endif



#endif  // not defined PLUGINS_EXPORT_H__ADF2524_327A_4FE2_FFBB_3B59FB1D4887__INCLUDED_
//===========================================================================
// End of Plugins_export.hpp
//===========================================================================



