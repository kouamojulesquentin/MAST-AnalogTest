//===========================================================================
//                           UT_XmlRpc_Client_Server.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_XmlRpc_Client_Server.cpp
//!
//! Implements test fixture for testing XmlRpc_Protocol_Server
//!
//===========================================================================

#include "UT_XmlRpc_Client_Server.hpp"
#include "XmlRpc_Protocol_Client.hpp"
#include "XmlRpc_Protocol_Server.hpp"
#include "Remote_Loopback_Protocol.hpp"

#include <thread>
#include <stdexcept>
#include <cxxtest/ValueTraits.h>

using mast::Remote_Loopback_Protocol;
using mast::XmlRpc_Protocol_Client;
using mast::XmlRpc_Protocol_Server;
using std::string;
using std::vector;


//! Checks XmlRpc_Protocol_Client::SendDoReset() along with XmlRpc_Protocol_Server response
//!
void UT_XmlRpc_Client_Server::test_SendDoReset ()
{
  // ---------------- Setup
  //
  Remote_Loopback_Protocol protocol;
  XmlRpc_Protocol_Server   server(8080, &protocol);
  XmlRpc_Protocol_Client   client;
  bool                     gotException = false;
  string                   exceptionMessage;


  auto threadFunction = [&]()
  {
    try
    {
      server.Start();
    }
    catch(std::exception& exc)  // Catch C++ standard exceptions
    {
      gotException = true;
      exceptionMessage.append(exc.what());
    }
    catch (...)
    {
      gotException = true;
      exceptionMessage.append("Got unknown type exception");
    }
  };

  std::thread serverThread(threadFunction);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (client.SendDoReset(false));

  // ---------------- Verify
  //
  if (serverThread.joinable())
  {
    serverThread.join();
  }

  TS_ASSERT_FALSE  (gotException);
  TS_ASSERT_EQUALS (exceptionMessage, "");
}



//===========================================================================
// End of UT_XmlRpc_Client_Server.cpp
//===========================================================================
