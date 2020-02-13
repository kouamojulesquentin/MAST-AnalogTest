//===========================================================================
//                           Simulation_TranslatorProtocol.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Simulation_TranslatorProtocol.hpp
//!
//! Declares Simulation_TranslatorProtocol class
//!
//===========================================================================


#ifndef SIMULATIONTRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
  #define SIMULATIONTRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_

#include "AccessInterfaceTranslatorProtocol.hpp"
#include <fstream>
#include <string>
#include <chrono>
#include <experimental/string_view>

namespace mast
{
//! Implement loopback while logging callbacks that
class MAST_CORE_EXPORT Simulation_TranslatorProtocol final : public AccessInterfaceTranslatorProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Simulation_TranslatorProtocol() = default;
  Simulation_TranslatorProtocol()
  {
    CleanUpFiles();
  }

  Simulation_TranslatorProtocol(std::experimental::string_view toSutFilePath, std::experimental::string_view fromSutFilePath)
    : m_toSutFilePath   (toSutFilePath)
    , m_fromSutFilePath (fromSutFilePath)
  {
    CleanUpFiles();
  }

  //! Does any Transformation needed to execute the callback given as a parameter
  //!
  //! @param current_request    the callback request to translate
  //!
  //! @return Bits stream retrieved from SUT as result of the transformation
  virtual BinaryVector TransformationCallback(CallbackRequest current_request) override;

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "Simulation_Translator"; }


  //! Sets maximum time to wait for data from SUT
  //!
  void FromSutDataTimeout (std::chrono::milliseconds fromSutTimeout) { m_fromSutTimeout = fromSutTimeout; }

  //! Returns maximum time to wait for data from SUT
  //!
  std::chrono::milliseconds FromSutDataTimeout() const { return m_fromSutTimeout; }

  //! Sets duration between two attempts to get data from SUT
  //!
  void FromSutDataWait (std::chrono::milliseconds fromSutWait) { m_fromSutWait = fromSutWait; }

  //! Returns duration between two attempts to get data from SUT
  //!
  std::chrono::milliseconds FromSutDataWait() const { return m_fromSutWait; }

  // ---------------- Protected Methods
  //

  //! Logs commands
  //!
  void LogCommands(std::experimental::string_view commands);

  protected:

  //! Deletes content of exchange files
  //!
  void CleanUpFiles ();

  //! Sends SVF command to SUT
  //!
  virtual void SendCommand(std::experimental::string_view command);

  //! Retrieves bitstrem data from SUT
  //!
  virtual BinaryVector FetchDataFromSut();

  // ---------------- Private  Fields
  //
  private:
  std::string               m_toSutFilePath   = "data_to_rtl.svf";      //!< File used to "transmit" SVF commands
  std::string               m_fromSutFilePath = "data_from_rtl.dat";  //!< File used to "receive" from SUT bitstream
  std::ifstream::pos_type   m_lastPos         = 0;                             //!< Position of last bitstream line "received" from SUT
  std::chrono::milliseconds m_fromSutTimeout  = std::chrono::seconds(60);      //!< Timeout waiting for "from SUT data" (before an exception is thrown)
  std::chrono::milliseconds m_fromSutWait     = std::chrono::milliseconds(50); //!< Duration between two attempts to get data from SUT

};
//
//  End of SVF_SimulationProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined SimulationTRANSLATIONPROTOCOL_H__6829475D_EA0B_4F3E_9991_492038F295A0__INCLUDED_
//===========================================================================
// End of Simulation_TranslationProtocol.hpp
//===========================================================================
