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
#include "XmlRpc_Protocol_Common.hpp"

#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>

#include <sstream>

using std::string;
using std::vector;
using std::pair;
using std::make_pair;

using mast::XmlRpc_Protocol_Client;
using mast::Remote_Protocol_Client;


//! Destructor does nothing special
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
}
//
//  End of: XmlRpc_Protocol_Client::XmlRpc_Protocol_Client
//---------------------------------------------------------------------------


//! Sends command for forcing the ResetPort to be asserted on the target module
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
void XmlRpc_Protocol_Client::SendDoReset (bool doSynchronousReset)
{
  const string methodName(XML_RPC_COMMAND_DO_RESET);

  xmlrpc_c::paramList params;

  params.add(xmlrpc_c::value_boolean (doSynchronousReset));

  xmlrpc_c::clientSimple client;
  xmlrpc_c::value        result;

  client.call(ServerUrl(), methodName, params, &result);
}
//
//  End of: XmlRpc_Protocol_Client::DoReset
//---------------------------------------------------------------------------



//! Sends scan vector to System Under Test
//!
//! @param commandName  Command name (SIR, SDR, RST...)
//! @param bitsCount    Number of valid bits in scan vector
//! @param scanVector   Binary data to send to SUT (default is right aligned)
//!
//! @return from SUT scan vector
Remote_Protocol_Client::SendScanVectorReturn_t
XmlRpc_Protocol_Client::SendScanVector (const string& commandName, uint32_t bitsCount, const vector<uint8_t>& toSutScanVector)
{
  const string methodName(XML_RPC_COMMAND_SEND_SCAN_VECTOR);

  xmlrpc_c::paramList params;

  params.add(xmlrpc_c::value_string     (commandName));
  params.add(xmlrpc_c::value_int        (bitsCount));
  params.add(xmlrpc_c::value_bytestring (toSutScanVector));

  xmlrpc_c::value        result;
  xmlrpc_c::clientSimple client;

  client.call(ServerUrl(), methodName, params, &result);

  xmlrpc_c::value_struct retValues = xmlrpc_c::value_struct(result);
  std::map<string, xmlrpc_c::value> retValuesMapping = static_cast<StructFields_t>(retValues);

  xmlrpc_c::value_int        xml_bitsCount   = retValuesMapping.at(XML_RPC_FIELD_BITS_COUNT);
  xmlrpc_c::value_bytestring xml_fromSutData = retValuesMapping.at(XML_RPC_FIELD_FROM_SUT_DATA);

  uint32_t        fromSutBitsCount  = xml_bitsCount.cvalue();
  vector<uint8_t> fromSutScanVector = xml_fromSutData.cvalue();

  return make_pair(fromSutBitsCount, fromSutScanVector);
}
//
//  End of: XmlRpc_Protocol_Client::SendScanVector
//---------------------------------------------------------------------------






//===========================================================================
// End of XmlRpc_Protocol_Client.cpp
//===========================================================================
