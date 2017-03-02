//===========================================================================
//                           XmlRpc_Protocol_Client.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file XmlRpc_Protocol_Client.hpp
//!
//! Declares XmlRpc_Protocol_Client class
//!
//===========================================================================


#ifndef XMLRPC_PROTOCOL_CLIENT_H__29E27448_A7B6_4B00_418B_1E559AD020FA__INCLUDED_
  #define XMLRPC_PROTOCOL_CLIENT_H__29E27448_A7B6_4B00_418B_1E559AD020FA__INCLUDED_

#include "Remote_Protocol_Client.hpp"

namespace mast
{
//! Represents an remote protocol based upon Xml-RPC with a client role
//!
//!
class XmlRpc_Protocol_Client : public Remote_Protocol_Client
{
  // ---------------- Public  Methods
  //
  public:
  ~XmlRpc_Protocol_Client();

  XmlRpc_Protocol_Client()
    : Remote_Protocol_Client("http://localhost:8080/RPC2")
  {}

  //! Constructs using string encoded parameters
  XmlRpc_Protocol_Client(const std::string& parameters);

  //! Sends scan vector to System Under Test
  //!
  //! @param commandName  Command name (e.g. SIR, SDR, RST...)
  //! @param bitsCount    Number of valid bits in scan vector
  //! @param scanVector   Binary data to send to SUT (default is right aligned)
  //!
  //! @return Error code (0 means no error)
  virtual SendScanVectorReturn_t SendScanVector(const std::string&         commandName,
                                                uint32_t                   bitsCount,
                                                const std::vector<unsigned char>& toSutScanVector);

  //! Sends command for forcing the ResetPort to be asserted on the target module
  //!
  //! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
  //!
  virtual void SendDoReset(bool doSynchronousReset);
};
//
//  End of XmlRpc_Protocol_Client class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined XMLRPC_PROTOCOL_CLIENT_H__29E27448_A7B6_4B00_418B_1E559AD020FA__INCLUDED_

//===========================================================================
// End of XmlRpc_Protocol_Client.hpp
//===========================================================================
