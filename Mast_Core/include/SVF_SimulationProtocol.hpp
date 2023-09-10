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
#include <chrono>
#include <experimental/string_view>

namespace mast
{
//! Implement AccessInterfaceProtocol by sending SVF commands to file and receiving
//! binary digits that represents bit values shifted from SUT
//!
//!
class MAST_CORE_EXPORT SVF_SimulationProtocol final : public SVF_Player
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~SVF_SimulationProtocol() = default;
  SVF_SimulationProtocol()
  {
    CleanUpFiles();
  }

  SVF_SimulationProtocol(std::experimental::string_view toSutFilePath, std::experimental::string_view fromSutFilePath)
    : m_toSutFilePath   (toSutFilePath)
    , m_fromSutFilePath (fromSutFilePath)
  {
    CleanUpFiles();
  }

  //! Does any callback required to transfer scan data to and from SUT
  //!
  //! @param channelId   Identifies the Channel to act for (zero based)
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfer to SUT
  //!
  //! @return Bitstream retrieved from SUT
  //!
  virtual BinaryVector DoCallback(RVFRequest Request, uint32_t channelId) override;

  //! Gets the number of Channel supported by the specific protocol
  //!
  //! @note Channel id 0 is reserved for reset operation, so protocol must support a least two Channels
  //!
  //! @return The number of supported Channels (including pseudo Channel 0 for reset)
  //!
  virtual uint32_t MaxSupportedChannels() const override { return 3u; }

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "SVF_Simulation"; }

  //! readable identifier for given Channel
  //!
  std::string CallbackId(int Channel) const { 
     switch (Channel){
       case 0: return "TRSRT"; 
       case 1: return "SIR"; 
       case 2: return "SRD"; 
       default: return "UNDEFINED";
       }
      }

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

  //! Forces the ResetPort to be asserted on the target module
  //!
  //! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
  //!
  virtual void DoReset(bool doSynchronousReset) override;

  // ---------------- Protected Methods
  //
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




#endif  // not defined SVF_SIMULATIONPROTOCOL_H__7FB50440_82B9_47D2_19A1_8408D110D99B__INCLUDED_

//===========================================================================
// End of SVF_SimulationProtocol.hpp
//===========================================================================
