//===========================================================================
//                           BinaryVector.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file BinaryVector.hpp
//!
//! Declares BinaryVector and
//===========================================================================



#ifndef SCANVECTORS_H__3E0880BD_14C4_4089_BA8F_A382FB9EE011__INCLUDED_
  #define SCANVECTORS_H__3E0880BD_14C4_4089_BA8F_A382FB9EE011__INCLUDED_

#include "Platform.hpp"
#include <vector>
#include <string>
#include <experimental/string_view>

namespace mast
{

class SVFVector;
class BinaryVectorRef;

//! Contains bitstream vector in compact binary format
//!
class DLL_EXPORT BinaryVector final
{
  // ---------------- Public  Methods
  //
  public:
  ~BinaryVector() = default;
  BinaryVector()  = default;
  BinaryVector(const BinaryVector& rhs);  //!< Copy constructor
//+  explicit BinaryVector(const SVFVector& svfVector);   //!< Converts a SVFVector to a BinaryVector
  explicit BinaryVector(uint32_t bitsCount, uint8_t fillPattern = 0);                   //!< Initializes with constant pattern for all bits
  BinaryVector(BinaryVector&& rhs) noexcept;

  static BinaryVector CreateFromBinaryString (std::experimental::string_view bits);   //!< Creates a BinaryVector from text binary representation
  static BinaryVector CreateFromHexString    (std::experimental::string_view bits);   //!< Creates a BinaryVector from text hexadecimal representation

  std::string DataAsBinaryString(std::experimental::string_view byteSeparator   = ":",
                                 std::experimental::string_view nibbleSeparator = "_",
                                 uint32_t                       bytesPerLine    = 0,
                                 std::experimental::string_view eolSeparator    = ","
                                ) const; //!< Gets content as formatted binary string

  BinaryVector& operator=(const BinaryVector&);
  BinaryVector& operator=(BinaryVector&&) noexcept;

  BinaryVector  operator~() const; //!< Returns another BinaryVector with every bits toggles
  BinaryVector& ToggleBits();      //!< Toggles (flips) every bits of the vector

  BinaryVector& operator<< (const BinaryVector& rhs) { return this->Append(rhs); }; //!< Appends another BinaryVector
  BinaryVector  operator+  (const BinaryVector& rhs) const;                         //!< Concatenate two BinaryVector

  bool operator==(const BinaryVector& rhs) const;                                 //!< Compares to other for equality
  bool operator!=(const BinaryVector& rhs) const { return !operator==(rhs); };    //!< Compares to other for inequality

  BinaryVector& Append(const BinaryVector& rhs); //!< Appends another scan vector

  BinaryVector& Append(uint8_t  value, uint8_t numberOfBits = 8); //!< Appends  8 bits value the BinaryVector
  BinaryVector& Append(uint16_t value);                           //!< Appends 16 bits value the BinaryVector
  BinaryVector& Append(uint32_t value);                           //!< Appends 32 bits value the BinaryVector
  BinaryVector& Append(uint64_t value);                           //!< Appends 64 bits value the BinaryVector

  void          Clear();             //!< Clears all content
  void          Set(uint8_t  value); //!< Assigns  8 bits value the BinaryVector
  void          Set(uint16_t value); //!< Assigns 16 bits value the BinaryVector
  void          Set(uint32_t value); //!< Assigns 32 bits value the BinaryVector
  void          Set(uint64_t value); //!< Assigns 64 bits value the BinaryVector

  bool           IsEmpty()    const { return m_data.empty();}      //!< Returns true when there is no bit in the BinaryVector, false otherwise
  uint32_t       BitsCount()  const { return m_usedBits;    }      //!< Returns total number of valid bits in the BinaryVector
  uint32_t       BytesCount() const { return m_data.size(); }      //!< Returns total number of valid bits in the BinaryVector
  const uint8_t* Data()       const { return m_data.data(); }      //!< Returns pointer on raw bits stream data (only valid as long as content is not modified)

  BinaryVector    Slice    (uint32_t firstBitOffset, uint32_t bitsCount) const; //!< Returns a slice from BinaryVector
//+  BinaryVectorRef SliceRef (uint32_t firstBitOffset, uint32_t bitsCount) const; //!< Returns a reference to a slice from BinaryVector

  // ---------------- Private  Fields
  //
  private:
  std::vector<uint8_t> m_data;              //!< Bytes formatted bit stream
  uint32_t             m_usedBits = 0;      //!< Number of effective bits (last byte may be not all used)
};
//
//  End of BinaryVector class declaration
//---------------------------------------------------------------------------


//! Represents a slice within a BinaryVector
//!
class BinaryVectorRef final
{
  // ---------------- Public  Methods
  //
  public:
  ~BinaryVectorRef() = default;
  BinaryVectorRef()  = delete;
  BinaryVectorRef(const BinaryVector& binaryVector, uint32_t firstBit, uint32_t lastBit)
    : m_binaryVector (binaryVector)
    , m_firstBit     (firstBit)
    , m_lastBit      (lastBit)
  {}

  friend BinaryVector;

  // ---------------- Private  Fields
  //
  private:
  const BinaryVector& m_binaryVector;
  const uint32_t      m_firstBit = 0;
  const uint32_t      m_lastBit  = 0;
};
//
//  End of BinaryVectorRef class declaration
//---------------------------------------------------------------------------




} // End of namespace mast


#endif  // not defined SCANVECTORS_H__3E0880BD_14C4_4089_BA8F_A382FB9EE011__INCLUDED_

//===========================================================================
// End of BinaryVector.hpp
//===========================================================================
