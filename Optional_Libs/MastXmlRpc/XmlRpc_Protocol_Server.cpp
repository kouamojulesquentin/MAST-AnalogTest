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
#include "XmlRpc_Protocol_Common.hpp"

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
  class MastXmlRpcMethod : public xmlrpc_c::method
  {
    public:
    virtual ~MastXmlRpcMethod() {}

    MastXmlRpcMethod(Remote_Protocol* protocol, string methodName, bool logErrors, bool logInfos)
      : m_protocol   (protocol)
      , m_methodName (methodName)
      , m_logErrors  (logErrors)
      , m_logInfos   (logInfos)
    {
    }


    void Rethrow(xmlrpc_c::fault& exc) const
    {
      string message = MessagePrefix().append(exc.getDescription());
      if (m_logErrors)
      {
        std::cerr << message << std::endl;
      }
      throw xmlrpc_c::fault(message, exc.getCode());
    }

    void Rethrow(std::exception& exc) const
    {
      ThrowWithMessage(MessagePrefix().append(exc.what()));
    }

    void Rethrow() const
    {
      ThrowWithMessage(MessagePrefix().append("Unknown type exception"));
    }

    // ---------------- Private  Methods
    //
    string MessagePrefix() const
    {
      return string("While processing \"").append(m_methodName).append("\", got: ");
    }

    void ThrowWithMessage(const string& message) const
    {
      if (m_logErrors)
      {
        std::cerr << message << std::endl;
      }
      throw girerr::error(message);
    }

    void LogInfo(const string& message) const
    {
      if (m_logInfos)
      {
        std::cout << message << std::endl;
      }
    }

    // ---------------- Protected Fields
    //
    protected:
    Remote_Protocol* m_protocol;   //!< Effective protocol to use
    string           m_methodName; //!< Method name to format error message
    bool             m_logErrors;  //!< To enable logging of errors on std::cerr
    bool             m_logInfos;   //!< To enable logging of infos messages on std::cout
  };

  class DoReset_Executer : public MastXmlRpcMethod
  {
    public:

    DoReset_Executer (Remote_Protocol* protocol, bool logErrors, bool logInfos)
      : MastXmlRpcMethod (protocol, "DoReset", logErrors, logInfos)
    {
      this->_help = "Forces the ResetPort to be asserted on the target module";
      LogInfo("DoReset executer started");
    }

    //! Extracts client parameter, then forward call to actual Remote_Protocol
    //!
    void execute (const xmlrpc_c::paramList& paramList, xmlrpc_c::value* const pRetValue)
    {
      try
      {
        paramList.verifyEnd(2);

        uint32_t callId             = paramList.getInt     (0);   // This is for response check
        bool     doSynchronousReset = paramList.getBoolean (1);

        m_protocol->DoReset(doSynchronousReset);

        *pRetValue = xmlrpc_c::value_int(XML_RPC_SUCCESS);

        StructFields_t fields;
        fields[XML_RPC_FIELD_CALL_ID] = xmlrpc_c::value_int(callId);

        *pRetValue = xmlrpc_c::value_struct(fields);
      }
      catch(xmlrpc_c::fault& exc)
      {
        Rethrow(exc);
      }
      catch(std::exception& exc)
      {
        Rethrow(exc);
      }
      catch (...)
      {
        Rethrow();
      }
    }
  };


  class SendScanVector_Executer : public MastXmlRpcMethod
  {
    public:
    SendScanVector_Executer (Remote_Protocol* protocol, bool logErrors, bool logInfos)
      : MastXmlRpcMethod (protocol, "DoAction", logErrors, logInfos)
    {
      this->_help = "Forward request to actual remote protocol \"DoAction\"";
      LogInfo("SendScanVector executer started");
    }

    //! Extracts client parameters, then forward call to actual Remote_Protocol
    //!
    void execute (const xmlrpc_c::paramList& paramList, xmlrpc_c::value* const pRetValue)
    {
      try
      {
        // ---------------- Extract parameters
        //
        paramList.verifyEnd(4);

        uint32_t        callId    (paramList.getInt        (0)); // This is for response check
        string          command   (paramList.getString     (1));
        uint32_t        bitsCount (paramList.getInt        (2));
        vector<uint8_t> toSutData (paramList.getBytestring (3));

        // ---------------- Call DoAction
        //
        Remote_Protocol::DoActionReturn_t doActionResult = m_protocol->DoAction(command, bitsCount, toSutData);

        // ---------------- Prepare response
        //
        uint32_t               fromSutBitsCount = doActionResult.first;
        const vector<uint8_t>& fromSutData      = doActionResult.second;

        StructFields_t fields;

        fields[XML_RPC_FIELD_CALL_ID]       = xmlrpc_c::value_int(callId);
        fields[XML_RPC_FIELD_BITS_COUNT]    = xmlrpc_c::value_int(fromSutBitsCount);
        fields[XML_RPC_FIELD_FROM_SUT_DATA] = xmlrpc_c::value_bytestring(fromSutData);

        *pRetValue = xmlrpc_c::value_struct(fields);
      }
      catch(xmlrpc_c::fault& exc)
      {
        Rethrow(exc);
      }
      catch(std::exception& exc)
      {
        Rethrow(exc);
      }
      catch (...)
      {
        Rethrow();
      }
    }
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
  if (!Protocol())
  {
    string message("A valid Remote_Protocol must have been set before starting Xml-RPC server");
    if (m_logErrors)
    {
      std::cerr << message << std::endl;
    }
    throw std::runtime_error(message);
  }

  try
  {
    // ---------------- Register supported "call"
    //
    const xmlrpc_c::methodPtr pDoReset_Executer    (new DoReset_Executer(Protocol(),        m_logErrors, m_logInfos));
    const xmlrpc_c::methodPtr pScanVector_Executer (new SendScanVector_Executer(Protocol(), m_logErrors, m_logInfos));

    xmlrpc_c::registry myRegistry;

    myRegistry.addMethod(XML_RPC_COMMAND_DO_RESET,         pDoReset_Executer);
    myRegistry.addMethod(XML_RPC_COMMAND_SEND_SCAN_VECTOR, pScanVector_Executer);

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

    string message = string("XmlRpc_Protocol_Server ==> Something failed: ").append(exc.what());
    if (m_logErrors)
    {
      std::cerr << message << std::endl;
    }
    throw std::runtime_error(message);
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
