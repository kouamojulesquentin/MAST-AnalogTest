//===========================================================================
//                           SVFVector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SVFVector.hpp
//!
//! Declares SVFVector class
//!
//===========================================================================


#ifndef SVFVECTOR_H__7556F5F5_5280_4133_9BAA_389FCC347CE5__INCLUDED_
  #define SVFVECTOR_H__7556F5F5_5280_4133_9BAA_389FCC347CE5__INCLUDED_

#include "BinaryVector.hpp"
#include <experimental/string_view>

namespace mast
{
//! Contains bitstream vector in SVF string (using ASCII '1' for 1 and '0' for 0)
//!
//! @note Internal representation is such as The bit order for scan data follows the convention that the least
//!       significant bit (right-most bit) is the first bit scanned into the hardware for TDI and SMASK scan data and
//!       is the first bit scanned out for TDO and MASK data.
//!       This bit ordering is consistent with the IEEE 1149.1 convention.
//!
class DLL_EXPORT SVFVector final
{
  // ---------------- Public  Methods
  //
  public:
  ~SVFVector() = default;
  SVFVector()  = default;

  SVFVector(std::experimental::string_view svfString, uint32_t bitsCount); //!< Converts a plain SVF string stream to SVFVector
  SVFVector(const BinaryVector&            binaryVector);                  //!< Converts a BinaryVector to a SVFVector

  BinaryVector       ToBinaryVector() const;         //!< Create a BinaryVector from content

  bool               IsEmpty()   const { return m_data.empty();}             //!< Returns true when there is no bit in the SVFVector, false otherwise
  uint32_t           BitsCount() const { return m_usedBits; }                //!< Returns total number of valid bits in the SVFVector
  const std::string& Data()      const { return m_data;     }                //!< Returns string SVF representation of bits (righ aligned)

  bool operator==(const SVFVector& rhs) const;                               //!< Compares to other for equality
  bool operator!=(const SVFVector& rhs) const { return !operator==(rhs); };  //!< Compares to other for inequality

  // ---------------- Private  Methods
  //
  static uint8_t     CharToNibble (char achar);
  static std::string CleanSvfString (std::experimental::string_view svfString, uint32_t bitsCount);

  // ---------------- Private  Fields
  //
  private:
  std::string m_data;              //!< ASCII formatted bit stream
  uint32_t    m_usedBits = 0;      //!< Number of effective bits (last byte may be not all used)
};
//
//  End of SVFVector class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SVFVECTOR_H__7556F5F5_5280_4133_9BAA_389FCC347CE5__INCLUDED_

//===========================================================================
// End of SVFVector.hpp
//===========================================================================
