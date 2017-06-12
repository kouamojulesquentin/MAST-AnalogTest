//===========================================================================
//                           FileSystem.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file FileSystem.hpp
//!
//! Declare utilities to deal with file system in a portable way (cannot use
//! c++14/17 nor boost)
//!
//===========================================================================


#ifndef FILESYSTEM_H__F77EA26F_9CD1_453F_F6AC_C62E811458F6__INCLUDED_
  #define FILESYSTEM_H__F77EA26F_9CD1_453F_F6AC_C62E811458F6__INCLUDED_

#include "Mast_Core_export.hpp"
#include <string>

namespace mast
{
  class MAST_CORE_EXPORT FileSystem final
  {
    public:
    FileSystem() = delete;

    //! Tells whether some path is a directory or not
    //!
    static bool IsDirectory(const std::string& path);
  };
} // End of namespace mast

#endif  // not defined FILESYSTEM_H__F77EA26F_9CD1_453F_F6AC_C62E811458F6__INCLUDED_
//===========================================================================
// End of FileSystem.hpp
//===========================================================================



