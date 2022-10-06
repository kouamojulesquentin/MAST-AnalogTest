//===========================================================================
//                           FTDI_TranslatorProtocol.hpp
//===========================================================================
// Copyright (C) 2020 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file FTDI_TranslatorProtocol.hpp
//!
//! Declares FTDI_TranslatorProtocol class
//!
//===========================================================================


#ifndef FTDI_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
  #define FTDI_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_

#include "T_2_T_TranslatorProtocol.hpp"
#include <fstream>
#include <string>
#include <experimental/string_view>
#include "function.h"

namespace mast
{
//! Implement loopback while logging callbacks that
class MAST_CORE_EXPORT FTDI_TranslatorProtocol final : public T_2_T_TranslatorProtocol
{
  // ---------------- Public  Methods
  //
  public:
 
  
  //! Constructor with no parameters
  //!
   FTDI_TranslatorProtocol();
  
  // Constructor with parameters defined by a string
  //
  //FTDI_TranslatorProtocol(const std::string& parameters);

  //! Destructor
  //!
   ~FTDI_TranslatorProtocol() override;

  //! Does any Transformation needed to execute the callback given as a parameter
  //!
  //! @param current_request    the callback request to translate
  //!
  //! @return Bits stream retrieved from SUT as result of the transformation
  virtual BinaryVector TransformationCallback(RVFRequest current_request) override;

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "FTDI"; }


  // ---------------- Protected Methods
  //


  protected:

  //! Space for internal utility functions
  //!
  

  // ---------------- Private  Fields for internal uses
  //
  //libFTDI fields
  struct ftdi_context *ftdi;  
  jtag_context jtag;

};
//
//  End of FTDI_TranslatorProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined FTDI_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
//===========================================================================
// End of FTDI_TranslatorProtocol.hpp
//===========================================================================
