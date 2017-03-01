//===========================================================================
//                           Remote_Protocol_Proxy.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Remote_Protocol_Proxy.hpp
//!
//! Declares Remote_Protocol_Proxy class
//!
//===========================================================================


#ifndef REMOTE_PROTOCOL_PROXY_H__94002874_A289_4E2C_6D87_8A98ABC6A2A7__INCLUDED_
  #define REMOTE_PROTOCOL_PROXY_H__94002874_A289_4E2C_6D87_8A98ABC6A2A7__INCLUDED_

#include "AccessInterfaceProtocol.hpp"
#include <vector>
#include <memory>

namespace mast
{

class Remote_Protocol_Client;

//! Adapts a Remote_Protocol_Client to AccessInterfaceProtocol
//!
class Remote_Protocol_Proxy : public AccessInterfaceProtocol
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Remote_Protocol_Proxy();

  Remote_Protocol_Proxy() = delete;

  //! Constructs from actual Remote_Protocol_Client and associated commands
  //!
  Remote_Protocol_Proxy(std::unique_ptr<Remote_Protocol_Client> remoteProtocol, std::vector<std::string> commands);

  //! Constructs from string parameters defining actual Remote_Protocol_Client
  //!
  Remote_Protocol_Proxy(const std::string& parameters);

  //! Does any action required to transfer scan data to and from SUT
  //!
  //! @param derivationId   Identifies the derivation to act for (zero based)
  //!                       Derivation 0 is reserved for reset operation
  //! @param interfaceData  Application data stored in the AccessInterface
  //! @param toSutData      Bits stream to transfer to SUT
  //!
  //! @return Bits stream retrieved from SUT
  virtual BinaryVector DoAction(uint32_t derivationId, void* interfaceData, const BinaryVector& toSutData);

  //! Gets the number of derivations supported by the specific protocol
  //!
  //! @note Derivation id 0 is reserved for reset operation, so protocol must support a least two derivations
  //!
  //! @return The number of supported derivation (including pseudo derivation 0 for reset)
  //!
  virtual uint32_t MaxSupportedDerivations() const { return m_commands.size(); };

  //! Returns readable type of protocol (I2C, SVF_Simu, OpenOCD, SPI...)
  //!
  virtual std::experimental::string_view KindName() const;

  //! Forces the ResetPort to be asserted on the target module
  //!
  //! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
  //!
  virtual void DoReset(bool doSynchronousReset);

  // ---------------- Private  Fields
  //
  private:
  std::unique_ptr<Remote_Protocol_Client> m_remoteProtocol; //!< Wrapped Remote_Protocol_Client
  std::vector<std::string>                m_commands;       //!< Commands associated with each derivation
  mutable std::string                     m_kindName;       //!< Mutable because value will be provided (once) by wrapped Remote_Protocol_Client
};
//
//  End of Remote_Protocol_Proxy class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined REMOTE_PROTOCOL_PROXY_H__94002874_A289_4E2C_6D87_8A98ABC6A2A7__INCLUDED_

//===========================================================================
// End of Remote_Protocol_Proxy.hpp
//===========================================================================
