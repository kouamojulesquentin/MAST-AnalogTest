//===========================================================================
//                           UT_XmlRpc_Protocol_Server.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_XmlRpc_Protocol_Server.cpp
//!
//! Implements test fixture for testing XmlRpc_Protocol_Server
//!
//===========================================================================


#include "UT_XmlRpc_Protocol_Server.hpp"
#include "XmlRpc_Protocol_Server.hpp"

#include <memory>
#include <cxxtest/ValueTraits.h>

using std::unique_ptr;
using std::make_unique;

using mast::XmlRpc_Protocol_Server;


//! Checks XmlRpc_Protocol_Server default constructor
//!
void UT_XmlRpc_Protocol_Server::test_Constructor_Default ()
{
  // ---------------- Setup
  //
  unique_ptr<XmlRpc_Protocol_Server> sut;

  CxxTest::setAbortTestOnFail(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut = make_unique<XmlRpc_Protocol_Server>());

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS  (sut->PortNumber(), 8080u);
  TS_ASSERT_NULLPTR (sut->Protocol());
}


//! Checks XmlRpc_Protocol_Server::Start() when no protocol has been provided
//!
void UT_XmlRpc_Protocol_Server::test_Start_when_no_Protocol ()
{
  // ---------------- Setup
  //
  XmlRpc_Protocol_Server sut;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Start(), std::exception);
}


//===========================================================================
// End of UT_XmlRpc_Protocol_Server.cpp
//===========================================================================
