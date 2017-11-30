//===========================================================================
//                           Emulation_TranslatorProtocol.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Emulation_TranslatorProtocol.hpp
//!
//! Declares Emulation_TranslatorProtocol class
//!
//===========================================================================


#ifndef SVF_EMULATIONTRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
  #define SVF_EMULATIONTRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_

#include "AccessInterfaceTranslatorProtocol.hpp"
#include <fstream>
#include <string>
#include <experimental/string_view>

namespace mast
{
//! Implement loopback while logging callbacks that
class MAST_CORE_EXPORT Emulation_TranslatorProtocol final : public AccessInterfaceTranslatorProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Emulation_TranslatorProtocol() = default;
  Emulation_TranslatorProtocol() = default;

  //! Does any Transformation needed to execute the callback given as a parameter
  //!
  //! @param current_request    the callback request to translate
  //!
  //! @return Bits stream retrieved from SUT as result of the transformation
  virtual BinaryVector TransformationCallback(CallbackRequest current_request) override;

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "Emulation_Translator"; }

  // ---------------- Protected Methods
  //

  //! Logs commands
  //!
  void LogCommands(std::experimental::string_view commands);
};
//
//  End of SVF_EmulationProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined SVF_EMULATIONTRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
//===========================================================================
// End of SVF_EmulationProtocol.hpp
//===========================================================================
