//===========================================================================
//                           Emulation_TranslatorProtocol.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file JTAG_BitBang_TranslatorProtocol.hpp
//!
//! Declares JTAG_BitBang_TranslatorProtocol class
//!
//===========================================================================


#ifndef JTAG_BITBANG_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
  #define JTAG_BITBANG_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_

#include "T_2_E_TranslatorProtocol.hpp"
#include <fstream>
#include <string>
#include <experimental/string_view>
#include "ParentNode.hpp"

namespace mast
{
//! Implements a Big-Bang T-2-E translator
class MAST_CORE_EXPORT JTAG_BitBang_TranslatorProtocol final : public T_2_E_TranslatorProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~JTAG_BitBang_TranslatorProtocol() = default;
  JTAG_BitBang_TranslatorProtocol() = delete;
 JTAG_BitBang_TranslatorProtocol(std::experimental::string_view BB_Reg_Name)   : T_2_E_TranslatorProtocol(BB_Reg_Name) {}

//  JTAG_BitBang_TranslatorProtocol(std::vector<uint32_t> addresses);

  //! Initializes with addresses and optional prefix defined by a string
  //!
//  JTAG_BitBang_TranslatorProtocol(const std::string& parameters);

  //! Does any Transformation needed to execute the callback given as a parameter
  //!
  //! @param current_request    the callback request to translate
  //!
  //! @return Bits stream retrieved from SUT as result of the transformation
  virtual BinaryVector TransformationCallback(RVFRequest current_request) override;
  
  virtual BinaryVector PDL_translator( std::experimental::string_view CallbackId, BinaryVector	   ToSutVector) override;


  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "JTAG_BitBang"; }


  // ---------------- Protected Methods
  //


  protected:

};
//
//  End of JTAG_BitBang_Protocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined JTAG_BITBANG_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
//===========================================================================
// End of JTAG_BitBang_TranslatorProtocol.hpp
//===========================================================================
