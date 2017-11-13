//===========================================================================
//                           BitsOrdering.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file BitsOrdering.hpp
//!
//! Declares BitsOrdering enum
//!
//===========================================================================


#ifndef BITSORDERING_H__F1855073_2B31_4E37_4A8F_3101DC751C4__INCLUDED_
  #define BITSORDERING_H__F1855073_2B31_4E37_4A8F_3101DC751C4__INCLUDED_

namespace mast
{
  //! Defines how some bits are ordered i.e. whether the MSB is on the
  //! left or right hand side
  //!
  enum class BitsOrdering : uint8_t   // Limit to 8 bits because it will be used in every Register
  {
    Undefined,  //!< Ordering is not defined
    Downto,     //!< Indexes decrease from left to right as in Vec[7:0]
    Upto,       //!< Indexes increase from left to right as in Vec[0:7]
  };
} // End of namespace mast

#endif  // not defined BITSORDERING_H__F1855073_2B31_4E37_4A8F_3101DC751C4__INCLUDED_
//===========================================================================
// End of BitsOrdering.hpp
//===========================================================================



