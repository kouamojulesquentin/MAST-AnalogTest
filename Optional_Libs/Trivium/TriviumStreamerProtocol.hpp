//===========================================================================
//                           TriviumStreamerProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file TriviumStreamerProtocol.hpp
//!
//! Declares class TriviumStreamerProtocol, which does nothing but can be used to 
//! experiment with StreamProtocols
//!
//===========================================================================

#ifndef TRIVIUMSTREAMER_H__5A3B6A4C_E465_423E_768A_56FFA405CB8B__INCLUDED_
  #define TRIVIUMSTREAMER_H__5A3B6A4C_E465_423E_768A_56FFA405CB8B__INCLUDED_

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

#include <Python.h>
/*pyhelper.hpp : helper class that handles deferencing inside the destructor to help clean-up*/
#include "pyhelper.hpp"

namespace mast
{
class MAST_CORE_EXPORT TriviumStreamerProtocol : public StreamerProtocol
{
  // ---------------- Public  Methods
  //
  public:
  ~TriviumStreamerProtocol();
  TriviumStreamerProtocol()  = delete;

  TriviumStreamerProtocol(const std::string& parameters);

  //! Returns readable type of selector
  //!
  virtual std::experimental::string_view KindName() const override { return "Trivium"; };

  //! Creates a new Mask of MaskBits and returns it.
  //! It represents the Mask that will be applied to a stream of MaskBits
  //!
  //!
  virtual BinaryVector& NewMask (uint32_t MaskBits)  override;

  //! Applies the given Mask to the PlainText
  //!
  //!
  virtual const BinaryVector ApplyMask (BinaryVector PlainText,BinaryVector Mask)  const override;

 protected:

   BinaryVector  TRIVIUM_XOR_MASK(long int xor_bits);

  //! Space for internal utility functions
  //!
  //!Pyhton Objects
  const char TRIVIUM_PYTHON[100] = "trivium";
  CPyInstance hInstance;
  CPyObject pDict, pPython_class, pObject;

   //!Mask used for most recent coding
   BinaryVector m_Trinvium_Xor_Mask;
   //!Unused Mask bits from most recent coding
   BinaryVector m_Residual_Xor_Mask;

};
//
//  End of TriviumStreamerProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined TRIVIUMSTREAMER_H__5A3B6A4C_E465_423E_768A_56FFA405CB8B__INCLUDED_

//===========================================================================
// End of TriviumStreamerProtocol.hpp
//===========================================================================
