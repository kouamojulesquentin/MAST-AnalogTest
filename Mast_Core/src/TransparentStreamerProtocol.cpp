//===========================================================================
//                           TransparentStreamerProtocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file TransparentStreamerProtocol.cpp
//!
//! Implements class TransparentStreamerProtocol
//!
//===========================================================================

#include "TransparentStreamerProtocol.hpp"
#include "Register.hpp"
#include "Utility.hpp"

#include <stdexcept>
#include <limits>
#include <sstream>

using std::ostringstream;
using std::string;
using std::experimental::string_view;
using std::shared_ptr;

using namespace mast;


   //! Creates a new Mask composed of only Zeros
  //!
  //!
  BinaryVector& TransparentStreamerProtocol::NewMask (uint32_t MaskBits) 
{
 BinaryVector tmp(MaskBits,0b00000000);
  m_CurrentMask.Set(std::move(tmp));
  return m_CurrentMask;
}
//
//  End of: TransparentStreamerProtocol::SelectionValue
//---------------------------------------------------------------------------

//! Makes an OR with the zeo mask: no effect
//!
//!
const BinaryVector TransparentStreamerProtocol::ApplyMask (BinaryVector PlainText,BinaryVector Mask) const
 {
  CHECK_PARAMETER_EQ(PlainText.BitsCount(),Mask.BitsCount(),"Vectors must have the same size");
  return PlainText|Mask;
 }

//===========================================================================
// End of TransparentStreamerProtocol.cpp
//===========================================================================
