//===========================================================================
//                           XmlRpc_Protocol_Client.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file XmlRpc_Protocol_Client.cpp
//!
//! Implements class XmlRpc_Protocol_Client
//!
//===========================================================================


#include "XmlRpc_Protocol_Client.hpp"
//+#include "g3log/g3log.hpp"   // Need C++11 (or 14 ?)

#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>

using std::string;
using std::vector;

using mast::XmlRpc_Protocol_Client;


//! `Releases ...`
XmlRpc_Protocol_Client::~XmlRpc_Protocol_Client ()
{
}
//
//  End of: XmlRpc_Protocol_Client::~XmlRpc_Protocol_Client
//---------------------------------------------------------------------------






//! Constructs using string encoded parameters
//!
XmlRpc_Protocol_Client::XmlRpc_Protocol_Client (const std::string& parameters)
  : Remote_Protocol_Client(parameters)
{
//+  LOG(INFO) << "Xml-Rpc Remote server: " << ServerUrl();
}
//
//  End of: XmlRpc_Protocol_Client::XmlRpc_Protocol_Client
//---------------------------------------------------------------------------



//! Sends scan vector to System Under Test
//!
//! @param commandName  Command name (SIR, SDR, RST...)
//! @param scanVector   Binary data to send to SUT (default is right aligned)
//!
//! @return Error code (0 means no error)
int XmlRpc_Protocol_Client::SendScanVector (const std::string& commandName, const std::vector<unsigned char>& scanVector)
{
  const string methodName("XmlRpc_Protocol_Server.SendScanVector");

  xmlrpc_c::paramList params;
  params.add(xmlrpc_c::value_bytestring(scanVector));

  xmlrpc_c::value        result;
  xmlrpc_c::clientSimple client;

  client.call(ServerUrl(), methodName, params, &result);

  xmlrpc_c::value_int retCode = xmlrpc_c::value_int(result);

  return retCode;
}
//
//  End of: XmlRpc_Protocol_Client::SendScanVector
//---------------------------------------------------------------------------






//===========================================================================
// End of XmlRpc_Protocol_Client.cpp
//===========================================================================
