//===========================================================================
//                           JTAG_to_I2C_TranslatorProtocol.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file JTAG_to_I2C_TranslatorProtocol.hpp
//!
//! Declares JTAG_to_I2C_TranslatorProtocol class
//!
//===========================================================================


#ifndef I2C_TO_JTAG_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
  #define I2C_TO_JTAG_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_

#include "T_2_T_TranslatorProtocol.hpp"
#include <fstream>
#include <string>
#include <experimental/string_view>

namespace mast
{
//! Implement loopback while logging callbacks that
class MAST_CORE_EXPORT JTAG_to_I2C_TranslatorProtocol final : public T_2_T_TranslatorProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~JTAG_to_I2C_TranslatorProtocol() = default;
  JTAG_to_I2C_TranslatorProtocol() = delete;

  JTAG_to_I2C_TranslatorProtocol(std::vector<uint32_t> addresses);

  //! Initializes with addresses and optional prefix defined by a string
  //!
  JTAG_to_I2C_TranslatorProtocol(const std::string& parameters);

  //! Does any Transformation needed to execute the callback given as a parameter
  //!
  //! @param current_request    the callback request to translate
  //!
  //! @return Bits stream retrieved from SUT as result of the transformation
  virtual BinaryVector TransformationCallback(RVFRequest current_request) override;

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "JTAG_to_I2C"; }


  // ---------------- Protected Methods
  //


  protected:

  //! Returns address for specified channel
  //!
  uint32_t GetAddress(uint32_t channelId) const;

  //! Creates an I2C command associated to channel identifier and BinaryVector to send to SUT
  //!
  std::string CreateI2CCommand(uint32_t channelId, const BinaryVector& toSutData);


  // ---------------- Private  Fields
  //
  std::string           m_commandPrefix; //!< Text leading I2C command (mainly used for logs in order to ease grep regex)
  std::vector<uint32_t> m_addresses;     //!< Addresses associated with channel ids (at offset 1 for channel 1)
};
//
//  End of SVF_EmulationProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined I2C_TO_JTAG_TRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
//===========================================================================
// End of I2C_TO_JTAG_TranslatorProtocol.hpp
//===========================================================================
