//===========================================================================
//                           RPI_TranslatorProtocol.hpp
//===========================================================================
// Copyright (C) 2020 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file RPI_TranslatorProtocol.hpp
//!
//! Declares RPI_TranslatorProtocol class
//!
//===========================================================================


#ifndef RPI_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
  #define RPI_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_

#include "T_2_T_TranslatorProtocol.hpp"
#include <fstream>
#include <string>
#include <experimental/string_view>

namespace mast
{
//! Implement loopback while logging callbacks that
class MAST_CORE_EXPORT RPI_TranslatorProtocol final : public T_2_T_TranslatorProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~RPI_TranslatorProtocol() = default;
  
  
  //! Constructor with no parameters
  //!
   RPI_TranslatorProtocol();
  
  // Constructor with parameters defined by a string
  //
  //RPI_TranslatorProtocol(const std::string& parameters);

  //! Does any Transformation needed to execute the callback given as a parameter
  //!
  //! @param current_request    the callback request to translate
  //!
  //! @return Bits stream retrieved from SUT as result of the transformation
  virtual BinaryVector TransformationCallback(RVFRequest current_request) override;

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "NXP_RPI"; }


  // ---------------- Protected Methods
  //


  protected:

  //! Space for internal utility functions
  //!
  

  // ---------------- Private  Fields for internal uses
  //
};
//
//  End of RPI_TranslatorProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined RPI_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
//===========================================================================
// End of RPI_TranslatorProtocol.hpp
//===========================================================================
