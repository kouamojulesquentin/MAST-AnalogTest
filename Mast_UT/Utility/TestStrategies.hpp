//===========================================================================
//                           TestStrategies.h
//===========================================================================
// Copyright (C) 2015 Jean-Francois Coulon. All rights reserved.
//               2017 Michele Portolan, TIMA             
// 
// Project : C++11 blocking queues
// based on : Multi-Edit extension DLL
//
//! @file TestStrategies.hpp
//!
//! Declares what defines strategies for testing some sut
//!
// Some Tags are inserted for Doxygen (JavaDoc like) doc generator.
//===========================================================================
// Created by JFC on April/30/2015
//===========================================================================


#ifndef TESTSTRATEGIES_H__C9890BD0_636E_4073_92AC_6745C8B2D338__INCLUDED_
  #define TESTSTRATEGIES_H__C9890BD0_636E_4073_92AC_6745C8B2D338__INCLUDED_

//! Defines method/stragegy to transfer item/value to something
//!
enum class DataTransferStrategy
{
  Copy, Move, Emplace
};

#endif  // not defined TESTSTRATEGIES_H__C9890BD0_636E_4073_92AC_6745C8B2D338__INCLUDED_
//===========================================================================
// End of TestStrategies.h
//===========================================================================


