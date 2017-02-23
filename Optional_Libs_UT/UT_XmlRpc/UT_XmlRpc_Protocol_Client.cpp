//===========================================================================
//                           UT_XmlRpc_Protocol_Client.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_XmlRpc_Protocol_Client.cpp
//!
//! Implements test fixture for testing XmlRpc_Protocol_Client
//!
//===========================================================================


#include "UT_XmlRpc_Protocol_Client.hpp"
#include "XmlRpc_Protocol_Client.hpp"
#include "xmlrpc-c/girerr.hpp" // For error exception

#include <memory>
#include <cxxtest/ValueTraits.h>

using std::unique_ptr;
using std::make_unique;
using std::string;
using std::vector;

using mast::XmlRpc_Protocol_Client;


//! Checks XmlRpc_Protocol_Client constructor
//!
void UT_XmlRpc_Protocol_Client::test_Constructor_Default ()
{
  // ---------------- Setup
  //
  unique_ptr<XmlRpc_Protocol_Client> sut;

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut = make_unique<XmlRpc_Protocol_Client>());

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut->ServerUrl(), "http://localhost:8080/RPC2");
}



//! Checks XmlRpc_Protocol_Client::SendDoReset() when no server is listening
//!
void UT_XmlRpc_Protocol_Client::test_SendDoReset_NoServer ()
{
  // ---------------- Setup
  //
  XmlRpc_Protocol_Client sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.SendDoReset(false), girerr::error);
}


//! Checks XmlRpc_Protocol_Client::SendScanVector() when no server is listening
//!
void UT_XmlRpc_Protocol_Client::test_SendScanVector_NoServer ()
{
  // ---------------- Setup
  //
  string                 commandName("SIR");
  uint32_t               bitsCount = 9;
  vector<uint8_t>        scanData{0x01, 0xC3};
  XmlRpc_Protocol_Client sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.SendScanVector(commandName, bitsCount, scanData), girerr::error);
}



//===========================================================================
// End of UT_XmlRpc_Protocol_Client.cpp
//===========================================================================
