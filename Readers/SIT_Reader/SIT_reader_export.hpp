//===========================================================================
//                           SIT_reader_export.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SIT_reader_export.hpp
//!
//! Defines DLL export dependant stuffs
//===========================================================================

#ifndef SIT_READER_EXPORT_H__EDAC8B04_3A8E_4731_9B99_BD84F3E04533__INCLUDED_
  #define SIT_READER_EXPORT_H__EDAC8B04_3A8E_4731_9B99_BD84F3E04533__INCLUDED_

  #if defined(WINDOWS)
    #include <windows.h>
    #ifdef SIT_READER_EXPORTS
        #define SIT_READER_EXPORT __declspec(dllexport)
    #else
        #define SIT_READER_EXPORT __declspec(dllimport)
    #endif
  #else
      #define SIT_READER_EXPORT
  #endif



#endif  // not defined SIT_READER_EXPORT_H__EDAC8B04_3A8E_4731_9B99_BD84F3E04533__INCLUDED_
//===========================================================================
// End of SIT_reader_export.hpp
//===========================================================================



