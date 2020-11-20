//===========================================================================
//                           DummyStreamerProtocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DummyStreamerProtocol.cpp
//!
//! Implements class DummyStreamerProtocol
//!
//===========================================================================

#include "DummyStreamerProtocol.hpp"
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

 DummyStreamerProtocol::DummyStreamerProtocol(const std::string& parameters)
  {std::ignore = parameters;
  }


   //! Creates a new Mask composed of only Zeros
  //!
  //!
  BinaryVector& DummyStreamerProtocol::NewMask (uint32_t MaskBits) 
{
 BinaryVector tmp(MaskBits,0b00000000);
  m_CurrentMask.Set(std::move(tmp));
  return m_CurrentMask;
}
//
//  End of: DummyStreamerProtocol::SelectionValue
//---------------------------------------------------------------------------

//! Makes an OR with the zeo mask: no effect
//!
//!
const BinaryVector DummyStreamerProtocol::ApplyMask (BinaryVector PlainText,BinaryVector Mask) const
 {
  CHECK_PARAMETER_EQ(PlainText.BitsCount(),Mask.BitsCount(),"Vectors must have the same size");
  return PlainText|Mask;
 }

//===========================================================================
// End of DummyStreamerProtocol.cpp
//===========================================================================
