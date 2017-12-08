//===========================================================================
//                           Spy_Emulation_Translator.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Spy_Emulation_Translator.hpp
//!
//! Declares Spy_Emulation_Translator class
//!
//===========================================================================


#ifndef Spy_Emulation_Translator_H__7720E2EF_FEE3_4D05_C4B6_A5E4CB999B6__INCLUDED_
  #define Spy_Emulation_Translator_H__7720E2EF_FEE3_4D05_C4B6_A5E4CB999B6__INCLUDED_

#include "SVF_RawPlayer.hpp"
#include "SpiedProtocolsCommands.hpp"
#include "BinaryVector.hpp"
#include "CallbackRequest.hpp"

#include <memory>
#include <string>

using namespace mast;

namespace test
{
//! Spies calls to SVF based AccessInterfaceProtocol
//!
class Spy_Emulation_Translator final : public mast::AccessInterfaceTranslatorProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Spy_Emulation_Translator() = default;
  Spy_Emulation_Translator(std::shared_ptr<SpiedProtocolsCommands> spiedCommands)
    : m_spiedCommands(spiedCommands) {}
  Spy_Emulation_Translator()
    : m_spiedCommands(std::make_shared<SpiedProtocolsCommands>()) {}

  //! Spies content how callback to SUT is emulated while returning the BinaryVector unchanged
  //!
  virtual mast::BinaryVector TransformationCallback(CallbackRequest current_request) override;

  const std::vector<std::string>& Commands() const { return m_spiedCommands->Commands(); }

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "Emulation_Spy"; }

  // ---------------- Private  Methods
  //
  //! Saves SVF commands
  //!
  void SaveCommands(std::experimental::string_view commands);

  // ---------------- Private  Fields
  //
  private:
  std::shared_ptr<SpiedProtocolsCommands> m_spiedCommands; //!< Collected SVF commands issued from vectors "send" to SUT by SystemModelManager
};
//
//  End of Spy_Emulation_Translator class declaration
//---------------------------------------------------------------------------
} // End of namespace test


#endif  // not defined Spy_Emulation_Translator_H__7720E2EF_FEE3_4D05_C4B6_A5E4CB999B6__INCLUDED_

//===========================================================================
// End of Spy_Emulation_Translator.hpp
//===========================================================================
