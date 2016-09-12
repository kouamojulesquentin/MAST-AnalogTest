//===========================================================================
//                           check_license.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file check_license.hpp
//!
//! Declares function check_license
//!
//===========================================================================

#ifndef H_MAST_CHECK_LICENSE
#define H_MAST_CHECK_LICENSE

//! returns TRUE if license check succeeds, FALSE otherwise
//! license file is supposed to be on current directory
//! if environment variable MAST_LICENSE_PATH is not set
//!
bool check_license();

#endif
