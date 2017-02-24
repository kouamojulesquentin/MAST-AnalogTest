//===========================================================================
//                           XmlRpc_Protocol_Server.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file XmlRpc_Protocol_Server.cpp
//!
//! Implements class XmlRpc_Protocol_Server
//!
//===========================================================================


#include "XmlRpc_Protocol_Server.hpp"
#include "Remote_Protocol.hpp"

#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>

#include <cassert>
#include <stdexcept>
#include <iostream>

using std::string;
using std::vector;

using mast::XmlRpc_Protocol_Server;
using mast::Remote_Protocol;

namespace
{
  class DoReset_Executer : public xmlrpc_c::method
  {
    public:

    DoReset_Executer (Remote_Protocol* protocol)
      : m_protocol (protocol)
    {
      this->_help = "Forces the ResetPort to be asserted on the target module";
    }

    //! Extracts client parameter, then forward call to actual Remote_Protocol
    //!
    void execute (const xmlrpc_c::paramList& paramList, xmlrpc_c::value* const pRetValue)
    {
      //! @todo [JFC]-[February/23/2017]: In execute(): Catch errors
      //!

      paramList.verifyEnd(1);

      bool doSynchronousReset = (paramList.getBoolean (0));

      m_protocol->DoReset(doSynchronousReset);

      *pRetValue = xmlrpc_c::value_int(0);
    }

    // ---------------- Private  Fields
    //
    Remote_Protocol* m_protocol;  //!< Effective protocol to use
  };


  class SendScanVector_Executer : public xmlrpc_c::method
  {
    public:

    SendScanVector_Executer (Remote_Protocol* protocol)
      : m_protocol (protocol)
    {
      // signature and help strings are documentation -- the client
      // can query this information with a system.methodSignature and
      // system.methodHelp RPC.
  //+    this->_signature = "vector<unsigned char>:vector<unsigned char>";

      this->_help = "Forward request to actual remote protocol \"DoAction\"";
    }

    //! Extracts client parameters, then forward call to actual Remote_Protocol
    //!
    void execute (const xmlrpc_c::paramList& paramList, xmlrpc_c::value* const pRetValue)
    {
      //! @todo [JFC]-[February/23/2017]: In execute(): Catch errors

      paramList.verifyEnd(3);

      string          command   (paramList.getString     (0));
      uint32_t        bitsCount (paramList.getInt        (1));
      vector<uint8_t> toSutData (paramList.getBytestring (2));

      vector<uint8_t> fromSutData = m_protocol->DoAction(command, bitsCount, toSutData);

      *pRetValue = xmlrpc_c::value_bytestring(fromSutData);
    }

    // ---------------- Private  Fields
    //
    Remote_Protocol* m_protocol;  //!< Effective protocol to use
  };

} // End of unnamed namespace



//! Does nothing.
XmlRpc_Protocol_Server::~XmlRpc_Protocol_Server ()
{
}
//
//  End of: XmlRpc_Protocol_Server::~XmlRpc_Protocol_Server
//---------------------------------------------------------------------------



//! Starts server
//!
//! @note This is a blocking call that never return !!!
//!
void XmlRpc_Protocol_Server::Start ()
{
  try
  {
    if (!Protocol())
    {
      throw std::runtime_error("A valid Remote_Protocol must have been set before starting Xml-RPC server");
    }

    // ---------------- Register supported "call"
    //
    const xmlrpc_c::methodPtr pDoReset_Executer    (new DoReset_Executer(Protocol()));
    const xmlrpc_c::methodPtr pScanVector_Executer (new SendScanVector_Executer(Protocol()));

    xmlrpc_c::registry myRegistry;

    myRegistry.addMethod("XmlRpc_Protocol_Server.DoReset",        pDoReset_Executer);
    myRegistry.addMethod("XmlRpc_Protocol_Server.SendScanVector", pScanVector_Executer);

    // ---------------- Configure server
    //
    xmlrpc_c::serverAbyss::constrOpt serverOptions;
    serverOptions.registryP(&myRegistry).portNumber(PortNumber());

    xmlrpc_c::serverAbyss server(serverOptions);

    m_abyssServer = &server;  // This is used to terminate the server

    // ---------------- Start server
    //
    server.run();

    m_abyssServer = NULL;
  }
  catch (const std::exception& exc)
  {
    m_abyssServer = NULL;   // Make sure we do not point on destructed server !
                            //
    throw std::runtime_error(string("XmlRpc_Protocol_Server ==> Something failed: ") + exc.what());
  }
}
//
//  End of: XmlRpc_Protocol_Server::Start
//---------------------------------------------------------------------------



//! Stops running server
//!
//! @note Does nothing if the server is no currently running
void XmlRpc_Protocol_Server::Stop ()
{
  if (m_abyssServer)
  {
    m_abyssServer->terminate();
    m_abyssServer = NULL;
  }
}
//
//  End of: XmlRpc_Protocol_Server::Stop
//---------------------------------------------------------------------------


//===========================================================================
// End of XmlRpc_Protocol_Server.cpp
//===========================================================================
