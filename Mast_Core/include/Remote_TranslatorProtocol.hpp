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


#ifndef REMOTE_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
  #define REMOTE_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_

#include "T_2_T_TranslatorProtocol.hpp"
#include <fstream>
#include <string>
#include <experimental/string_view>

namespace mast
{
//! Implement loopback while logging callbacks that
class MAST_CORE_EXPORT Remote_TranslatorProtocol final : public T_2_T_TranslatorProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Remote_TranslatorProtocol() = default;
  Remote_TranslatorProtocol() = delete;

  Remote_TranslatorProtocol(std::vector<uint32_t> addresses);

  //! Initializes with addresses and optional prefix defined by a string
  //!
  Remote_TranslatorProtocol(const std::string& parameters);

  //! Does any Transformation needed to execute the callback given as a parameter
  //!
  //! @param current_request    the callback request to translate
  //!
  //! @return Bits stream retrieved from SUT as result of the transformation
  virtual BinaryVector TransformationCallback(CallbackRequest current_request) override;

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "REMOTE"; }


  // ---------------- Protected Methods
  //


  protected:

  //! Returns address for specified endpoint
  //!
  uint32_t GetAddress(uint32_t endpointId) const;

  //! Creates an I2C command associated to endpoint identifier and BinaryVector to send to SUT
  //!
  std::string CreateI2CCommand(uint32_t endpointId, const BinaryVector& toSutData);


  // ---------------- Private  Fields
  //
  std::string           m_commandPrefix; //!< Text leading I2C command (mainly used for logs in order to ease grep regex)
  std::vector<uint32_t> m_addresses;     //!< Addresses associated with endpoint ids (at offset 1 for endpoint 1)
};
//
//  End of SVF_EmulationProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined REMOTE_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
//===========================================================================
// End of REMOTE_TranslatorProtocol.hpp
//===========================================================================
