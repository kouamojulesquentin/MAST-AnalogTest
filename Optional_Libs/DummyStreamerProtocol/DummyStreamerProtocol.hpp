//===========================================================================
//                           DummyStreamerProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DummyStreamerProtocol.hpp
//!
//! Declares class DummyStreamerProtocol, which does nothing but can be used to 
//! experiment with StreamProtocols
//!
//===========================================================================

#ifndef DUMMYSTREAMER_H__5A3B6A4C_E465_423E_768A_56FFA405CB8B__INCLUDED_
  #define DUMMYSTREAMER_H__5A3B6A4C_E465_423E_768A_56FFA405CB8B__INCLUDED_

#include "PathSelector.hpp"
#include "BinaryVector.hpp"
#include "VirtualRegister.hpp"
#include "Mast_Core_export.hpp"
#include "EnumsUtility.hpp"
#include "StreamerProtocol.hpp"

#include <vector>
#include <memory>
#include <experimental/string_view>
#include <ostream>

namespace mast
{
class MAST_CORE_EXPORT DummyStreamerProtocol : public StreamerProtocol
{
  // ---------------- Public  Methods
  //
  public:
  ~DummyStreamerProtocol() = default;
  DummyStreamerProtocol()  = delete;

  DummyStreamerProtocol(const std::string& parameters);

  //! Returns readable type of selector
  //!
  virtual std::experimental::string_view KindName() const override { return "DummyStreamer"; };

  //! Creates a new Mask of MaskBits and returns it.
  //! It represents the Mask that will be applied to a stream of MaskBits
  //!
  //!
  virtual BinaryVector& NewMask (uint32_t MaskBits)  override;

  //! Applies the given Mask to the PlainText
  //!
  //!
  virtual const BinaryVector ApplyMask (BinaryVector PlainText,BinaryVector Mask)  const override;


};
//
//  End of DummyStreamerProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined DummySTREAMER_H__5A3B6A4C_E465_423E_768A_56FFA405CB8B__INCLUDED_

//===========================================================================
// End of DummyStreamerProtocol.hpp
//===========================================================================
