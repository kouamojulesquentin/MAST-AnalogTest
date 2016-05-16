//===========================================================================
//                           Platform.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Platform.hpp
//!
//! Defines platform dependant stuffs
//===========================================================================


#ifndef PLATFORM_H__31EE6B85_BB3D_4B14_E288_56C12101BFE0__INCLUDED_
  #define PLATFORM_H__31EE6B85_BB3D_4B14_E288_56C12101BFE0__INCLUDED_

  #if defined(WINDOWS)
    #include <windows.h>
    #ifdef BUILD_DLL
        #define DLL_EXPORT __declspec(dllexport)
    #else
        #define DLL_EXPORT __declspec(dllimport)
    #endif
  #else
      #define DLL_EXPORT
  #endif



#endif  // not defined PLATFORM_H__31EE6B85_BB3D_4B14_E288_56C12101BFE0__INCLUDED_
//===========================================================================
// End of Platform.hpp
//===========================================================================



