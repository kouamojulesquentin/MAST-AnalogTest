//===========================================================================
//                           g3log_debug.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file g3log_debug.hpp
//!
//! Declares macro helpers to debug g3log
//!
//===========================================================================


#ifndef G3LOG_DEBUG_H__BA42ED87_51F3_4006_A99_54F6D1F77E6D__INCLUDED_
  #define G3LOG_DEBUG_H__BA42ED87_51F3_4006_A99_54F6D1F77E6D__INCLUDED_

#include <chrono>
#include <iostream>

#ifndef COUT_LOG_ENABLED
#define COUT_LOG_ENABLED false
#endif  // not define COUT_LOG_ENABLE

#define TIMESTAMP std::chrono::high_resolution_clock::now().time_since_epoch().count()
#define STREAM_TIMESTAMP std::endl << "[" << TIMESTAMP << "] "
#define COUT_LOG if(COUT_LOG_ENABLED) std::cout
#define COUT_LOG_TIMESTAMP  COUT_LOG << STREAM_TIMESTAMP
#define CERR_LOG std::cerr
#define CERR_LOG_TIMESTAMP  CERR_LOG << STREAM_TIMESTAMP

#endif  // not defined G3LOG_DEBUG_H__BA42ED87_51F3_4006_A99_54F6D1F77E6D__INCLUDED_
//===========================================================================
// End of g3log_debug.hpp
//===========================================================================



