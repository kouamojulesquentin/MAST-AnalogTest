//===========================================================================
//                           StreamerProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file StreamerProtocol.hpp
//!
//! Declares StreamerProtocol interface and common implementation
//!
//===========================================================================


#ifndef STREAMERPROTOCOL_H__9B76DEAF_2DDC_4FFA_9F8A_EC2E62431F68__INCLUDED_
  #define STREAMERPROTOCOL_H__9B76DEAF_2DDC_4FFA_9F8A_EC2E62431F68__INCLUDED_

#include <cstdint>
#include <memory>
#include <experimental/string_view>

namespace mast
{



class SystemModelVisitor;
class VirtualRegister;
class BinaryVector;

class StreamerProtocol
{
  // ---------------- Public  Methods
  //
  public:

  //! Returns the current Mask for streaming
  //!
  //!
  const BinaryVector& CurrentMask () {return m_CurrentMask;};

  //! Creates a new Mask of MaskBits and returns it.
  //!
  //!
  virtual BinaryVector& NewMask (uint32_t MaskBits) =0;

  //! Applies the given Mask to the PlainText
  //!
  //!
  virtual const BinaryVector ApplyMask (BinaryVector PlainText,BinaryVector Mask) const =0;
  
  // ---------------- Protected Methods
  //
  virtual ~StreamerProtocol() = default;
  StreamerProtocol()  = default;


  //! Returns readable type of StreamerProtocol 
  //!
  virtual std::experimental::string_view KindName() const = 0;

  // ---------------- Private  Fields
  //
  BinaryVector m_CurrentMask;       //!< Defines selector properies
};
//
//  End of StreamerProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined STREAMERPROTOCOL_H__9B76DEAF_2DDC_4FFA_9F8A_EC2E62431F68__INCLUDED_

//===========================================================================
// End of StreamerProtocol.hpp
//===========================================================================
