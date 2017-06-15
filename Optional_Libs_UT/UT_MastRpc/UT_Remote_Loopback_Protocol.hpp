//===========================================================================
//                           UT_Remote_Loopback_Protocol.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_Remote_Loopback_Protocol.hpp
//!
//! Declares test fixture class for testing Remote_Loopback_Protocol
//!
//===========================================================================


#ifndef UT_REMOTE_LOOPBACK_PROTOCOL_H__8751C42B_3940_44E4_6F87_16F2487E726A__INCLUDED_
  #define UT_REMOTE_LOOPBACK_PROTOCOL_H__8751C42B_3940_44E4_6F87_16F2487E726A__INCLUDED_

#include <cxxtest/TestSuite.h>

//! Unit tests for class Remote_Loopback_Protocol
//!
//!
class UT_Remote_Loopback_Protocol : public CxxTest::TestSuite
{
  public:
  ~UT_Remote_Loopback_Protocol() = default;
   UT_Remote_Loopback_Protocol() = default;

  // ================ Test Methods ================

  void test_Constructor ();
  void test_DoReset ();
  void test_DoCallback ();
};
//
//  End of UT_Remote_Loopback_Protocol class declaration
//---------------------------------------------------------------------------


#endif  // not defined UT_REMOTE_LOOPBACK_PROTOCOL_H__8751C42B_3940_44E4_6F87_16F2487E726A__INCLUDED_
//===========================================================================
// End of UT_Remote_Loopback_Protocol.hpp
//===========================================================================
