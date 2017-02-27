//===========================================================================
//                           UT_XmlRpc_Protocol_Client.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_XmlRpc_Protocol_Client.hpp
//!
//! Declares test fixture class for testing XmlRpc_Protocol_Client
//!
//===========================================================================


#ifndef UT_XMLRPC_PROTOCOL_CLIENT_H__69A3F48E_5E68_4A78_4C91_DB58395D8B20__INCLUDED_
  #define UT_XMLRPC_PROTOCOL_CLIENT_H__69A3F48E_5E68_4A78_4C91_DB58395D8B20__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class XmlRpc_Protocol_Client
//!
//!
class UT_XmlRpc_Protocol_Client : public CxxTest::TestSuite
{
  public:
  ~UT_XmlRpc_Protocol_Client() = default;
   UT_XmlRpc_Protocol_Client() = default;

  // ================ Test Methods ================

  void test_Constructor_Default ();

  void test_SendDoReset_NoServer ();
  void test_SendScanVector_NoServer ();
};
//
//  End of UT_XmlRpc_Protocol_Client class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_XMLRPC_PROTOCOL_CLIENT_H__69A3F48E_5E68_4A78_4C91_DB58395D8B20__INCLUDED_
//===========================================================================
// End of UT_XmlRpc_Protocol_Client.hpp
//===========================================================================
