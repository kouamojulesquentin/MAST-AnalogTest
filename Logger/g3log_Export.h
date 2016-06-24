//===========================================================================
//                           g3log_Export.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file g3log_Export.hpp
//!
//! Defines DLL export stuffs for Windows build
//===========================================================================


#ifndef G3LOG_EXPORT_H__AAA05159_A646_48E1_6A2_15F84DF0BB5D__INCLUDED_
  #define G3LOG_EXPORT_H__AAA05159_A646_48E1_6A2_15F84DF0BB5D__INCLUDED_

  #if defined(WINDOWS)
    #include <windows.h>
    #ifdef BUILD_DLL
        #define G3LOG_DLL_EXPORT __declspec(dllexport)
    #else
        #define G3LOG_DLL_EXPORT __declspec(dllimport)
    #endif
  #else
      #define G3LOG_DLL_EXPORT
  #endif

#endif  // not defined G3LOG_EXPORT_H__AAA05159_A646_48E1_6A2_15F84DF0BB5D__INCLUDED_
//===========================================================================
// End of g3log_Export.hpp
//===========================================================================



