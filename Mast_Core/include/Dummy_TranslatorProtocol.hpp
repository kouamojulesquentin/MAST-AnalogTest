//===========================================================================
//                           Dummy_TranslatorProtocol.hpp
//===========================================================================
// Copyright (C) 2019 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Dummy_TranslatorProtocol.hpp
//!
//! Declares Dummy_TranslatorProtocol class
//!
//===========================================================================


#ifndef DUMMY_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
  #define DUMMY_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_

#include "T_2_T_TranslatorProtocol.hpp"
#include <fstream>
#include <string>
#include <experimental/string_view>

namespace mast
{
//! Implement loopback while logging callbacks that
class MAST_CORE_EXPORT Dummy_TranslatorProtocol final : public T_2_T_TranslatorProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Dummy_TranslatorProtocol() = default;
  Dummy_TranslatorProtocol() = delete;

   //! Constructor with parameters defined by a string
  //!
  Dummy_TranslatorProtocol(const std::string& parameters);

  //! Does any Transformation needed to execute the callback given as a parameter
  //!
  //! @param current_request    the callback request to translate
  //!
  //! @return Bits stream retrieved from SUT as result of the transformation
  virtual BinaryVector TransformationCallback(CallbackRequest current_request) override;

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "Dummy"; }


  // ---------------- Protected Methods
  //


  protected:

  //! Space for internal utility functions
  //!
  

  // ---------------- Private  Fields for internal uses
  //
};
//
//  End of Dummy_TranslatorProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined DUMMY_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
//===========================================================================
// End of Dummy_TranslatorProtocol.hpp
//===========================================================================
