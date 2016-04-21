//===========================================================================
//                           Utility.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Utility.hpp
//!
//! Declares class Utility
//!
//===========================================================================



#ifndef UTILITY_H__AB0B55F8_1F3A_4D8D_893_CA234E5BFD9D__INCLUDED_
  #define UTILITY_H__AB0B55F8_1F3A_4D8D_893_CA234E5BFD9D__INCLUDED_

namespace mast
{
//! Provides functions utilities that are not related to a specific class hierarchy
//!
class Utility final
{
  // ---------------- Public  Methods
  //
  public:
  ~Utility() = delete;
  Utility()  = delete;

  //! Computes the minimal number of bytes to hold a number of bits
  //!
  static uint32_t BytesCountFromBitsCount(uint32_t bitsCount) { return (bitsCount + 7) / 8; }
};
//
//  End of Utility class declaration
//---------------------------------------------------------------------------
} // End of namespace mast


#endif  // not defined UTILITY_H__AB0B55F8_1F3A_4D8D_893_CA234E5BFD9D__INCLUDED_

//===========================================================================
// End of Utility.hpp
//===========================================================================
