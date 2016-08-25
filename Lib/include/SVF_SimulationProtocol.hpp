//===========================================================================
//                           SVF_SimulationProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SVF_SimulationProtocol.hpp
//!
//! Declares SVF_SimulationProtocol class
//!
//===========================================================================


#ifndef SVF_SIMULATIONPROTOCOL_H__7FB50440_82B9_47D2_19A1_8408D110D99B__INCLUDED_
  #define SVF_SIMULATIONPROTOCOL_H__7FB50440_82B9_47D2_19A1_8408D110D99B__INCLUDED_

#include "SVF_Player.hpp"
#include <fstream>
#include <string>
#include <experimental/string_view>

namespace mast
{
//! Implement AccessInterfaceProtocol by sending SVF commands to file and receiving
//! binary digits that represents bit values shifted from SUT
//!
//!
class DLL_EXPORT SVF_SimulationProtocol final : public SVF_Player
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~SVF_SimulationProtocol() = default;
  SVF_SimulationProtocol() = default;
  SVF_SimulationProtocol(std::experimental::string_view toSutFilePath, std::experimental::string_view fromSutFilePath)
    : m_toSutFilePath   (toSutFilePath)
    , m_fromSutFilePath (fromSutFilePath)
  {
  }

  //! Does any action required to transfer scan data to and from SUT
  //!
  //! @param derivationId   Identifies the derivation to act for (zero based)
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfer to SUT
  //!
  //! @return Bitstream retrieved from SUT
  //!
  virtual BinaryVector DoAction(uint32_t derivationId, void* interfaceData, const BinaryVector& toSutData) override;

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "SVF_Simulation"; }

  // ---------------- Protected Methods
  //
  protected:
  //! Sends SVF command to SUT
  //!
  virtual void SendCommand(std::experimental::string_view command);

  //! Retrieves bitstrem data from SUT
  //!
  virtual BinaryVector FetchDataFromSut();

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
  std::string             m_toSutFilePath   = "to_RTL/data_to_rtl.svf";     //!< File used to "transmit" SVF commands
  std::string             m_fromSutFilePath = "from_RTL/data_from_rtl.dat"; //!< File used to "receive" from SUT bitstream
  std::ifstream::pos_type m_lastPos         = 0;                            //!< Position of last bitstream line "received" from SUT
};
//
//  End of SVF_SimulationProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined SVF_SIMULATIONPROTOCOL_H__7FB50440_82B9_47D2_19A1_8408D110D99B__INCLUDED_

//===========================================================================
// End of SVF_SimulationProtocol.hpp
//===========================================================================
