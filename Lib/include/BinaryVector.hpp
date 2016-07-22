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
#include <initializer_list>
#include <experimental/string_view>

namespace mast
{

class SVFVector;
class BinaryVectorRef;


//! Tells how, non complete, data have their useful bits aligned
//!
enum class BitsAlignment
{
  Right,    //!< Bits are right aligned (e.g. for one bit, in a byte, it can be masked with value 0x01)
  Left,     //!< Bits are left aligned (e.g. for one bit, in a byte, it can be masked with value 0x80)
};

//! Tells whether BinaryVector size (number of bits) can change once constructed
//!
enum class SizeProperty
{
  NotFixed,    //!< BinaryVector bits count can be changed at will
  Fixed,       //!< After construction, the BinaryVector bits count cannot be changed (this property is not copied)
  FixedOnCopy, //!< Same as Fixed, except the property is copied (and moved)
};

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
  BinaryVector(const BinaryVector& rhs, SizeProperty sizeProperty) : BinaryVector(rhs) { m_sizeProperty = sizeProperty; } //!< Copy constructor with fixed size

  explicit BinaryVector(uint32_t bitsCount, uint8_t fillPattern = 0, SizeProperty sizeProperty = SizeProperty::NotFixed);          //!< Initializes with constant pattern for all bits

  BinaryVector(BinaryVector&& rhs) noexcept;    //!< Move constructor
  BinaryVector(BinaryVector&& rhs, SizeProperty sizeProperty) noexcept : BinaryVector(std::move(rhs)) { m_sizeProperty = sizeProperty; }    //!< Move constructor with fixed size

  using string_view = std::experimental::string_view;

  static BinaryVector CreateFromBinaryString (string_view bits, SizeProperty sizeProperty = SizeProperty::NotFixed);   //!< Creates a BinaryVector from text binary representation
  static BinaryVector CreateFromHexString    (string_view bits, SizeProperty sizeProperty = SizeProperty::NotFixed);   //!< Creates a BinaryVector from text hexadecimal representation
  static BinaryVector CreateFromString       (string_view bits, SizeProperty sizeProperty = SizeProperty::NotFixed);   //!< Creates a BinaryVector from mixed hexadecimal and binary representation


  std::string DataAsBinaryString(string_view quadSeparator = "_",
                                 string_view octoSeparator = ":",
                                 uint32_t    ssbytesPerLine  = 0,
                                 string_view eolSeparator  = ","
                                ) const; //!< Gets content as formatted binary string

//+  std::string DataAsDecimalString(string_view quadSeparator  = "_",
//+                                  string_view octoSeparator  = ":",
//+                                  uint32_t    ssbytesPerLine = 0,
//+                                  string_view eolSeparator   = ","
//+                                ) const; //!< Gets content as formatted decimal string

  std::string DataAsHexString (std::experimental::string_view quadSeparator = "_",
                               std::experimental::string_view octoSeparator = ":",
                               uint32_t                       bytesPerLine  = 0,
                               std::experimental::string_view eolSeparator  = ","
                              ) const;         //!< Gets content as formatted hexadecimal string

  std::string DataAsMixString (uint32_t                       hexStyleThreshold = 8,
                               std::experimental::string_view quadSeparator     = "_",
                               std::experimental::string_view octoSeparator     = ":",
                               uint32_t                       bytesPerLine      = 0,
                               std::experimental::string_view eolSeparator      = ","
                              ) const;         //!< Gets content as mix of hex and binary digits (size dependent)

  BinaryVector& operator=(const BinaryVector&);   //!< Copy assignment
  BinaryVector& operator=(BinaryVector&&);        //!< Move assignment

  // Bitwise operators and methods
  BinaryVector  operator~() const; //!< Returns another BinaryVector with every bits toggles
  BinaryVector& ToggleBits();      //!< Toggles (flips) every bits of the vector

  BinaryVector& operator&= (const BinaryVector& rhs); //!< Bitwise and assignment
  BinaryVector& operator|= (const BinaryVector& rhs); //!< Bitwise or assignment
  BinaryVector& operator^= (const BinaryVector& rhs); //!< Bitwise xor assignment

  // Logical operators
  bool operator==(const BinaryVector& rhs) const;                                 //!< Compares to other for equality (excepted fixed size property)
  bool operator!=(const BinaryVector& rhs) const { return !operator==(rhs); };    //!< Compares to other for inequality

  // Append operator and methods
  BinaryVector& operator<< (const BinaryVector& rhs) { return this->Append(rhs); }; //!< Appends another BinaryVector
  BinaryVector  operator+  (const BinaryVector& rhs) const;                         //!< Concatenate two BinaryVector

  BinaryVector& Append(const BinaryVector& rhs); //!< Appends another scan vector

  BinaryVector& Append(uint8_t  value, uint8_t numberOfBits = 8,  BitsAlignment alignment = BitsAlignment::Right); //!< Appends  8 bits value the BinaryVector
  BinaryVector& Append(uint16_t value, uint8_t numberOfBits = 16, BitsAlignment alignment = BitsAlignment::Right); //!< Appends 16 bits value the BinaryVector
  BinaryVector& Append(uint32_t value, uint8_t numberOfBits = 32, BitsAlignment alignment = BitsAlignment::Right); //!< Appends 32 bits value the BinaryVector
  BinaryVector& Append(uint64_t value, uint8_t numberOfBits = 64, BitsAlignment alignment = BitsAlignment::Right); //!< Appends 64 bits value the BinaryVector

  BinaryVector& AppendBits (bool bitIsOne, uint32_t count); //!< Appends 0 or 1 bit N times

  BinaryVector& AppendChunks (uint8_t numberOfBits, BitsAlignment alignment, std::initializer_list<uint8_t> chunks); // Appends from a list of uint8_t ordered from msb to lsb

  void          Clear();              //!< Clears all content

  void          Get(uint8_t&  value) const; //!< Reads unsigned  8 bits value from BinaryVector
  void          Get(uint16_t& value) const; //!< Reads unsigned 16 bits value from BinaryVector
  void          Get(uint32_t& value) const; //!< Reads unsigned 32 bits value from BinaryVector
  void          Get(uint64_t& value) const; //!< Reads unsigned 64 bits value from BinaryVector

  void          Get(int8_t&  value) const; //!< Reads signed  8 bits value from BinaryVector
  void          Get(int16_t& value) const; //!< Reads signed 16 bits value from BinaryVector
  void          Get(int32_t& value) const; //!< Reads signed 32 bits value from BinaryVector
  void          Get(int64_t& value) const; //!< Reads signed 64 bits value from BinaryVector

  void          Set(uint8_t  value); //!< Assigns unsigned  8 bits value the BinaryVector
  void          Set(uint16_t value); //!< Assigns unsigned 16 bits value the BinaryVector
  void          Set(uint32_t value); //!< Assigns unsigned 32 bits value the BinaryVector
  void          Set(uint64_t value); //!< Assigns unsigned 64 bits value the BinaryVector

  void          Set(int8_t  value); //!< Assigns signed  8 bits value the BinaryVector
  void          Set(int16_t value); //!< Assigns signed 16 bits value the BinaryVector
  void          Set(int32_t value); //!< Assigns signed 32 bits value the BinaryVector
  void          Set(int64_t value); //!< Assigns signed 64 bits value the BinaryVector

  void SetBit    (uint32_t bitOffset); //!< Sets specified bit (zero based)
  void ClearBit  (uint32_t bitOffset); //!< Clears specified bit (zero based)
  void ToggleBit (uint32_t bitOffset); //!< Toggles specified bit (zero based)

  void           FixSize(bool fixSize) { m_sizeProperty = fixSize ? SizeProperty::Fixed : SizeProperty::NotFixed; } //!< Sets whether the number of used bits cannot be changed

  bool           HasFixedSize() const { return m_sizeProperty != SizeProperty::NotFixed; } //!< Returns true if number of used bits cannot be changed
  bool           IsEmpty()      const { return m_data.empty();}                            //!< Returns true when there is no bit in the BinaryVector, false otherwise
  uint32_t       BitsCount()    const { return m_usedBits;    }                            //!< Returns total number of valid bits in the BinaryVector
  uint32_t       BytesCount()   const { return m_data.size(); }                            //!< Returns total number of valid bits in the BinaryVector
  const uint8_t* Data()         const { return m_data.data(); }                            //!< Returns pointer on raw bits stream data (only valid as long as content is not modified)

  BinaryVector    Slice    (uint32_t firstBitOffset, uint32_t bitsCount) const; //!< Returns a slice from BinaryVector
//+  BinaryVector_View Slice_View (uint32_t firstBitOffset, uint32_t bitsCount) const; //!< Returns a reference to a slice from BinaryVector

  // ---------------- Private  Methods
  //
  void MaskLastByte ();
  uint8_t                   MergeToByte (uint32_t lsbOffset, uint8_t lsbBitsCount, bool asSigned = false) const;
  template<typename T> void SetUnsigned (T value);
  template<typename T> void SetSigned   (T value);

  bool FixedSize()  const { return m_sizeProperty == SizeProperty::Fixed; }
  bool IsNegative() const { return (m_data[0] & 0x80) != 0; }


  uint8_t LastByteBitsCount() const
  {
    if (m_usedBits == 0) return 0;

    auto bitsCount = m_usedBits % 8u;
    if (bitsCount == 0) bitsCount = 8u;

    return bitsCount;
  }



  // ---------------- Private  Fields
  //
  private:
  std::vector<uint8_t> m_data;                                  //!< Bytes formatted bit stream
  uint32_t             m_usedBits     = 0;                      //!< Number of effective bits (last byte may be not all used)
  SizeProperty         m_sizeProperty = SizeProperty::NotFixed; //!< When true, the number of used bits cannot be changed (once constructed)
};
//
//  End of BinaryVector class declaration
//---------------------------------------------------------------------------

//! Bitwise 'and'
//!
inline BinaryVector operator& (const BinaryVector& lhs, const BinaryVector& rhs)
{
  auto result = lhs;
  result &= rhs;
  return result;
}

//! Bitwise 'or'
//!
inline BinaryVector operator| (const BinaryVector& lhs, const BinaryVector& rhs)
{
  auto result = lhs;
  result |= rhs;
  return result;
}

//! Bitwise 'xor'
//!
inline BinaryVector operator^ (const BinaryVector& lhs, const BinaryVector& rhs)
{
  auto result = lhs;
  result ^= rhs;
  return result;
}


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
