//===========================================================================
//                           C_API_export.h
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file C_API_export.h
//!
//! Defines DLL export dependant stuffs
//===========================================================================


#ifndef C_API_EXPORT_H__452C09DB_2D4E_4C81_EBAA_75D2A3E4805D__INCLUDED_
  #define C_API_EXPORT_H__452C09DB_2D4E_4C81_EBAA_75D2A3E4805D__INCLUDED_

  #if defined(WINDOWS)
    #include <windows.h>
    #ifdef C_API_EXPORTS
        #define C_API_EXPORT __declspec(dllexport)
    #else
        #define C_API_EXPORT __declspec(dllimport)
    #endif
  #else
      #define C_API_EXPORT
  #endif


#endif  // not defined C_API_EXPORT_H__452C09DB_2D4E_4C81_EBAA_75D2A3E4805D__INCLUDED_
//===========================================================================
// End of C_API_export.h
//===========================================================================



