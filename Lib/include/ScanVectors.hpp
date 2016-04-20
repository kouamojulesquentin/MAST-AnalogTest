//===========================================================================
//                           ScanVectors.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ScanVectors.hpp
//!
//! Declares BinaryVector and
//===========================================================================



#ifndef SCANVECTORS_H__3E0880BD_14C4_4089_BA8F_A382FB9EE011__INCLUDED_
  #define SCANVECTORS_H__3E0880BD_14C4_4089_BA8F_A382FB9EE011__INCLUDED_
#include <vector>
#include <string>

namespace mast
{

class SVFVector;

//! Contains bitstream vector in compact binary format
//!
class BinaryVector final
{
  // ---------------- Public  Methods
  //
  public:
  ~BinaryVector() = default;
  BinaryVector()  = default;
  BinaryVector(const SVFVector& svfVector);   //!< Converts a SVFVector to a BinaryVector

  BinaryVector& operator<< (const BinaryVector& rhs);       //!< Appends another scan vector
  BinaryVector  operator+  (const BinaryVector& rhs) const; //!< Concatenate two scan vectors

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
  std::vector<uint8_t> m_data;              //!< Bytes formatted bit stream
  uint32_t             m_usedBits = 0;      //!< Number of effective bits (last byte may be not all used)
};
//
//  End of BinaryVector class declaration
//---------------------------------------------------------------------------


//! Contains bitstream vector in SVR string format (using ASCII '1' for 1 and '0' for 0)
//!
class SVFVector final
{
  // ---------------- Public  Methods
  //
  public:
  ~SVFVector() = default;
  SVFVector()  = default;
  SVFVector(const BinaryVector& binaryVector);  //!< Converts a BinaryVector to a SVFVector


  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private  Methods
  //
  private:

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


#endif  // not defined SCANVECTORS_H__3E0880BD_14C4_4089_BA8F_A382FB9EE011__INCLUDED_

//===========================================================================
// End of BinaryVector.hpp
//===========================================================================
