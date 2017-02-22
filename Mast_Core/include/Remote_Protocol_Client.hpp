//===========================================================================
//                           Remote_Protocol_Client.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Remote_Protocol_Client.hpp
//!
//! Declares Remote_Protocol_Client class
//!
//===========================================================================


#ifndef REMOTE_PROTOCOL_CLIENT_H__569C639E_4551_4FA9_B38E_4515A0748085__INCLUDED_
  #define REMOTE_PROTOCOL_CLIENT_H__569C639E_4551_4FA9_B38E_4515A0748085__INCLUDED_

#include <string>
#include <vector>

namespace mast
{
//! Represents a remote protocol with client role for RPC
//!
//! @note This is an abstract base class
//!
class Remote_Protocol_Client
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Remote_Protocol_Client() {}

  Remote_Protocol_Client(const std::string& serverUrl)
    : m_serverUrl (serverUrl)
    , m_kindName  ("Remote_Client")  // This is the default when not yet got actual name from a server
  {
  }

  //! Sends scan vector to System Under Test
  //!
  //! @param commandName  Command name (SIR, SDR, RST...)
  //! @param scanVector   Binary data to send to SUT (default is right aligned)
  //!
  //! @return Error code (0 means no error)
  virtual int SendScanVector(const std::string& commandName, const std::vector<unsigned char>& scanVector) = 0;

  //! Returns current remote server URL
  //!
  std::string ServerUrl() const { return m_serverUrl; }


  //! Returns readable type of protocol (Remote_I2C, Remote_Loopback, Remote_Advantest_ATE...)
  //!
  virtual std::string KindName() const { return m_kindName; }

  // ---------------- Protected Methods
  //
  protected:

  // ---------------- Private  Methods
  //
  private:
  Remote_Protocol_Client() {}

  // ---------------- Private  Fields
  //
  private:
  std::string m_serverUrl;  //!< Url for connection to remote protocol server
  std::string m_kindName;   //!< Should be retrieved from call to a server
};
//
//  End of Remote_Protocol_Client class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined REMOTE_PROTOCOL_CLIENT_H__569C639E_4551_4FA9_B38E_4515A0748085__INCLUDED_

//===========================================================================
// End of Remote_Protocol_Client.hpp
//===========================================================================
