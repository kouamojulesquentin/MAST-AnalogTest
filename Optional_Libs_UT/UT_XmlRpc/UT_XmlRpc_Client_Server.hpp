//===========================================================================
//                           UT_XmlRpc_Client_Server.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_XmlRpc_Client_Server.hpp
//!
//! Declares test fixture class for testing XmlRpc_Protocol_Server
//!
//===========================================================================


#ifndef UT_XMLRPC_CLIENT_SERVER_H__E476C631_CB85_4679_AEA6_789312FBBFB__INCLUDED_
  #define UT_XMLRPC_CLIENT_SERVER_H__E476C631_CB85_4679_AEA6_789312FBBFB__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for classes XmlRpc_Protocol_Client and XmlRpc_Protocol_Server together
//!
class UT_XmlRpc_Client_Server : public CxxTest::TestSuite
{
  public:
  ~UT_XmlRpc_Client_Server() = default;
   UT_XmlRpc_Client_Server() = default;

  // ================ Test Methods ================

  void test_SendDoReset ();

};
//
//  End of UT_XmlRpc_Client_Server class declaration
//---------------------------------------------------------------------------

#endif  // not defined UT_XMLRPC_CLIENT_SERVER_H__E476C631_CB85_4679_AEA6_789312FBBFB__INCLUDED_
//===========================================================================
// End of UT_XmlRpc_Client_Server.hpp
//===========================================================================
