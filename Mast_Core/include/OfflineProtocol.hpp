//===========================================================================
//                           OfflineProtocol.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file OfflineProtocol.hpp
//!
//! Declares OfflineProtocol class
//!
//===========================================================================


#ifndef Offline_PROTOCOL_H__7FB50440_82B9_47D2_19A1_8408D110D99B__INCLUDED_
  #define Offline_PROTOCOL_H__7FB50440_82B9_47D2_19A1_8408D110D99B__INCLUDED_

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
class MAST_CORE_EXPORT OfflineProtocol final : public AccessInterfaceProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~OfflineProtocol() = default;
  OfflineProtocol()
  {
    CleanUpFiles();
  }

  OfflineProtocol(std::experimental::string_view toSutFilePath, std::experimental::string_view fromSutFilePath)
    : m_toSutFilePath   (toSutFilePath)
    , m_fromSutFilePath (fromSutFilePath)
  {
    CleanUpFiles();
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

  //! Gets the number of derivations supported by the specific protocol
  //!
  //! @note Derivation id 0 is reserved for reset operation, so protocol must support a least two derivations
  //!
  //! @return The number of supported derivation (including pseudo derivation 0 for reset)
  //!
  virtual uint32_t MaxSupportedDerivations() const override { return 3u; }

  //! Returns readable type of protocol
  //!
  virtual std::experimental::string_view KindName() const override { return "Offline"; }

  //! Sets maximum time to wait for data from SUT
  //!
  void FromSutDataTimeout (std::chrono::milliseconds fromSutTimeout) { m_fromSutTimeout = fromSutTimeout; }

  //! Returns maximum time to wait for data from SUT
  //!
  std::chrono::milliseconds FromSutDataTimeout() const { return m_fromSutTimeout; }

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
  #if defined(_WIN32)
  std::string               m_toSutFilePath   = "NULL";                        //!< File used to "transmit" SVF commands
  #else
  std::string               m_toSutFilePath   = "/dev/null";                   //!< File used to "transmit" SVF commands
  #endif
  std::string               m_fromSutFilePath = "input.dat";                   //!< File used to "receive" from SUT bitstream
  std::ifstream::pos_type   m_lastPos         = 0;                             //!< Position of last bitstream line "received" from SUT
  std::chrono::milliseconds m_fromSutTimeout  = std::chrono::seconds(60);      //!< Timeout waiting for "from SUT data" (before an exception is thrown)
  std::chrono::milliseconds m_fromSutWait     = std::chrono::milliseconds(50); //!< Duration between two attempts to get data from SUT
};
//
//  End of OfflineProtocol class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined OfflinePROTOCOL_H__7FB50440_82B9_47D2_19A1_8408D110D99B__INCLUDED_

//===========================================================================
// End of OfflineProtocol.hpp
//===========================================================================
