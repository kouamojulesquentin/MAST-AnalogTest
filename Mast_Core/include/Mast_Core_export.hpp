//===========================================================================
//                           Mast_Core_export.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Mast_Core_export.hpp
//!
//! Defines DLL export dependant stuffs
//===========================================================================

#ifndef MAST_CORE_EXPORT_H__C93E939D_8ED2_410B_17AA_BAE678CACFF6__INCLUDED_
  #define MAST_CORE_EXPORT_H__C93E939D_8ED2_410B_17AA_BAE678CACFF6__INCLUDED_

  #if defined(WINDOWS)
    #include <windows.h>
    #ifdef MAST_CORE_EXPORTS
        #define MAST_CORE_EXPORT __declspec(dllexport)
    #else
        #define MAST_CORE_EXPORT __declspec(dllimport)
    #endif
  #else
      #define MAST_CORE_EXPORT
  #endif


#endif  // not defined MAST_CORE_EXPORT_H__C93E939D_8ED2_410B_17AA_BAE678CACFF6__INCLUDED_
//===========================================================================
// End of Mast_Core_export.hpp
//===========================================================================



