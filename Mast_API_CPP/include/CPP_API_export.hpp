//===========================================================================
//                           CPP_API_export.h
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file CPP_API_export.hpp
//!
//! Defines DLL export dependant stuffs
//===========================================================================


#ifndef CPP_API_EXPORT_H__CBB29D37_C44B_4F53_DFBC_68E18131839__INCLUDED_
  #define CPP_API_EXPORT_H__CBB29D37_C44B_4F53_DFBC_68E18131839__INCLUDED_

  #if defined(WINDOWS)
    #include <windows.h>
    #ifdef CPP_API_EXPORTS
        #define CPP_API_EXPORT __declspec(dllexport)
    #else
        #define CPP_API_EXPORT __declspec(dllimport)
    #endif
  #else
      #define CPP_API_EXPORT
  #endif


#endif  // not defined CPP_API_EXPORT_H__CBB29D37_C44B_4F53_DFBC_68E18131839__INCLUDED_

//===========================================================================
// End of CPP_API_export.hpp
//===========================================================================



