//===========================================================================
//                           BinaryVector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file BinaryVector.hpp
//!
//! Declares class BinaryVector
//===========================================================================


#ifndef BINARYVECTOR_H__D9E20B3E_BDBE_4212_D8B5_24812F7355B1__INCLUDED_
  #define BINARYVECTOR_H__D9E20B3E_BDBE_4212_D8B5_24812F7355B1__INCLUDED_

#include <vector>

namespace mast
{
//! Contains bitstream vector in compact binary format
//!
class BinaryVector final
{
  // ---------------- Public  Methods
  //
  public:
  ~BinaryVector() = default;
  BinaryVector()  = default;

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
  std::vector<char> m_data;              //!< Bytes formatted bit stream
  uint32_t          m_usedBits = 0;      //!< Number of effective bits (last byte may be not all used)
};
//
//  End of BinaryVector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined BINARYVECTOR_H__D9E20B3E_BDBE_4212_D8B5_24812F7355B1__INCLUDED_

//===========================================================================
// End of BinaryVector.hpp
//===========================================================================
