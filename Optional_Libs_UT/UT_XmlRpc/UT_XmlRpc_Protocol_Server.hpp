//===========================================================================
//                           UT_XmlRpc_Protocol_Server.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_XmlRpc_Protocol_Server.hpp
//!
//! Declares test fixture class for testing XmlRpc_Protocol_Server
//!
//===========================================================================


#ifndef UT_XMLRPC_PROTOCOL_SERVER_H__ED09C153_5DFE_4405_3B3_F1256FB52AAE__INCLUDED_
  #define UT_XMLRPC_PROTOCOL_SERVER_H__ED09C153_5DFE_4405_3B3_F1256FB52AAE__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class XmlRpc_Protocol_Server
//!
//!
class UT_XmlRpc_Protocol_Server : public CxxTest::TestSuite
{
  public:
  ~UT_XmlRpc_Protocol_Server() = default;
   UT_XmlRpc_Protocol_Server() = default;

  // ================ Test Methods ================

  void test_Constructor_Default ();

  void test_Start_when_no_Protocol ();

};
//
//  End of UT_XmlRpc_Protocol_Server class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_XMLRPC_PROTOCOL_SERVER_H__ED09C153_5DFE_4405_3B3_F1256FB52AAE__INCLUDED_
//===========================================================================
// End of UT_XmlRpc_Protocol_Server.hpp
//===========================================================================
