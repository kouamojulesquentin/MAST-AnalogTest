//===========================================================================
//                           Remote_Loopback_TranslatorProtocol.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Remote_Loopback_TranslatorProtocol.hpp
//!
//! Declares Remote_Loopback_TranslatorProtocol class
//!
//===========================================================================


#ifndef REMOTE_LOOPBACK_TRANSLATORPROTOCOL_H__8F7FF1F5_9EBA_4A17_E8B6_20B6A814A9B__INCLUDED_
  #define REMOTE_LOOPBACK_TRANSLATORPROTOCOL_H__8F7FF1F5_9EBA_4A17_E8B6_20B6A814A9B__INCLUDED_

#include "AccessInterfaceTranslatorProtocol.hpp"

namespace mast
{
//! Acts as a Remote_Protocol by just looping back SUT data vector and doing nothing for reset
//!
class Remote_Loopback_TranslatorProtocol final : public AccessInterfaceTranslatorProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Remote_Loopback_TranslatorProtocol() = default;

  Remote_Loopback_TranslatorProtocol() = default;

  //! Does any Transformation needed to execute the callback given as a parameter
  //!
  //! @param current_request    the callback request to translate
  //!
  //! @return Bits stream retrieved from SUT as result of the transformation
  virtual BinaryVector TransformationCallback(RVFRequest current_request) override;

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "Remote_LoopbackTranslator"; }

};
//
//  End of Remote_Loopback_TranslatorProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined REMOTE_LOOPBACK_TRANSLATORPROTOCOL_H__8F7FF1F5_9EBA_4A17_E8B6_20B6A814A9B__INCLUDED_

//===========================================================================
// End of Remote_Loopback_TranslatorProtocol.hpp
//===========================================================================
