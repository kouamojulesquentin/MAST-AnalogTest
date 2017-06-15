//===========================================================================
//                           UT_Remote_Protocol_Proxy.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_Remote_Protocol_Proxy.hpp
//!
//! Declares test fixture class for testing Remote_Protocol_Proxy
//!
//===========================================================================


#ifndef UT_REMOTE_PROTOCOL_PROXY_H__8F1A2BCE_6798_4459_638A_5EDB67646B0E__INCLUDED_
  #define UT_REMOTE_PROTOCOL_PROXY_H__8F1A2BCE_6798_4459_638A_5EDB67646B0E__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class Remote_Protocol_Proxy
//!
//!
class UT_Remote_Protocol_Proxy : public CxxTest::TestSuite
{
  public:
  ~UT_Remote_Protocol_Proxy() = default;
   UT_Remote_Protocol_Proxy() = default;

  // ================ Test Methods ================

  void test_Constructor_RemoteClient ();
  void test_Constructor_StringParameters_Success ();
  void test_Constructor_StringParameters_Failure ();

  void test_DoReset_Sync ();
  void test_DoReset_ASync ();

  void test_DoCallback ();
  void test_DoCallback_invalid_EndPointId ();
  void test_DoCallback_invalid_InterfaceData ();

  // ================ Other Methods ================

  void setUp(); //!< Initializes test (called for each test)

  void tearDown();  //!< Cleanups test (called for each test)

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Private  Fields
  //
  private:
};
//
//  End of UT_Remote_Protocol_Proxy class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_REMOTE_PROTOCOL_PROXY_H__8F1A2BCE_6798_4459_638A_5EDB67646B0E__INCLUDED_
//===========================================================================
// End of UT_Remote_Protocol_Proxy.hpp
//===========================================================================
