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

using mast::Remote_Protocol_Client;
using mast::Remote_Loopback_Protocol;
using mast::XmlRpc_Protocol_Client;
using mast::XmlRpc_Protocol_Server;
using std::string;
using std::vector;

namespace
{
//! Wraps client and server along with server background thread
//!
struct ClientServerContext
{
  ~ClientServerContext() { Stop(); }
  ClientServerContext()
   : server(8080, &protocol)
  {
  }

  Remote_Loopback_Protocol protocol;
  XmlRpc_Protocol_Server   server;
  XmlRpc_Protocol_Client   client;
  bool                     gotException = false;
  string                   exceptionMessage;
  std::thread              serverThread;

  void StartServerThread()
  {
    serverThread = std::thread([this]() { ThreadFunction(); });
  }

  void Stop()
  {
    server.Stop();
    if (serverThread.joinable())
    {
      serverThread.join();
    }
  }

  void ThreadFunction()
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
  }
};
// End of: struct ClientServerContext
} // End of unnamed namespace


//! Checks XmlRpc_Protocol_Client::SendDoReset() along with XmlRpc_Protocol_Server response
//!
void UT_XmlRpc_Client_Server::test_SendDoReset ()
{
  // ---------------- Setup
  //
  ClientServerContext ctx;

  ctx.StartServerThread();

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (ctx.client.SendDoReset(false));

  // ---------------- Verify
  //
  TS_ASSERT_FALSE  (ctx.gotException);
  TS_ASSERT_EQUALS (ctx.exceptionMessage, "");
}


//! Checks XmlRpc_Protocol_Client::SendScanVector() along with XmlRpc_Protocol_Server response
//!
void UT_XmlRpc_Client_Server::test_SendScanVector_Single ()
{
  // ---------------- Setup
  //
  ClientServerContext ctx;

  ctx.StartServerThread();

  string          commandName("SDR");
  uint32_t        bitsCount = 19;
  vector<uint8_t> toSutData {0x05, 0xC3, 0x81};

  Remote_Protocol_Client::SendScanVectorReturn_t sendResult;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sendResult = ctx.client.SendScanVector(commandName, bitsCount, toSutData));

  // ---------------- Verify
  //
  ctx.Stop();

  const auto  fromSutBitsCount = sendResult.first;
  const auto& fromSutData      = sendResult.second;

  TS_ASSERT_FALSE  (ctx.gotException);
  TS_ASSERT_EQUALS (ctx.exceptionMessage, "");
  TS_ASSERT_EQUALS (fromSutBitsCount,     bitsCount);
  TS_ASSERT_EQUALS (fromSutData,          toSutData);
}


//! Checks XmlRpc_Protocol_Client::SendScanVector() along with XmlRpc_Protocol_Server response for multiple-consecutive calls
//!
void UT_XmlRpc_Client_Server::test_SendScanVector_Multiple ()
{
  // ---------------- Setup
  //
  ClientServerContext ctx;

  ctx.StartServerThread();

  string                  commandName("SDR");
  uint32_t                bitsCount = 19;
  vector<vector<uint8_t>> fromSutDatas;
  vector<vector<uint8_t>> toSutDatas
  {
    {0x05, 0xC3, 0x81},
    {0x06, 0xC4, 0x82},
    {0x07, 0xC5, 0x83},
    {0x08, 0xC6, 0x84},
  };

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING
  (
    for (const auto& toSutData : toSutDatas)
    {
      auto result = ctx.client.SendScanVector(commandName, bitsCount, toSutData);

      const auto  fromSutBitsCount = result.first;
      const auto& fromSutData      = result.second;

      TS_ASSERT_EQUALS (fromSutBitsCount, bitsCount);
      fromSutDatas.emplace_back(std::move(fromSutData));
    }
  );

  // ---------------- Verify
  //
  ctx.Stop();

  TS_ASSERT_FALSE  (ctx.gotException);
  TS_ASSERT_EQUALS (ctx.exceptionMessage, "");
  TS_ASSERT_EQUALS (fromSutDatas, toSutDatas);
}


//===========================================================================
// End of UT_XmlRpc_Client_Server.cpp
//===========================================================================
