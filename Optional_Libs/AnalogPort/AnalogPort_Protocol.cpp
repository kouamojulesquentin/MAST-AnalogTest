//===========================================================================
//                           AnalogPort_Protocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AnalogPort_Protocol.cpp
//!
//! Implements class AnalogPort_Protocol
//!
//===========================================================================

#include "AnalogPort_Protocol.hpp"
#include "g3log/g3log.hpp"

#include "Utility.hpp"
#include <thread>
#include <chrono>

using std::experimental::string_view;
using std::string;
using std::ofstream;
using std::ifstream;
using namespace mast;
using namespace std::string_literals;


AnalogPort_Protocol::AnalogPort_Protocol(std::string portname)
{
 m_portname = portname;
}

//! This callback only causes a "strobe" to be issued for reading value on the Port
//! Only Loopback data is returned 
//!
BinaryVector AnalogPort_Protocol::DoCallback (std::string /*CallbackId*/, uint32_t /*channelId*/, void* /* interfaceData */, const BinaryVector& toSutData)
{
 auto c_portname = m_portname.c_str();
  RVFRequest request(IMEASUREVOLTAGE,(void *)c_portname);

  PushRequest(request);
  
                       /*NB: this is a BLOCKING call*/
  PopfromSut();


  return toSutData;
}



//! Forces the ResetPort to be asserted on the target module
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
void AnalogPort_Protocol::DoReset(bool /* doSynchronousReset */)
{
  LOG(INFO) << "DoReset does nothing";
}

//===========================================================================
// End of AnalogPort_Protocol.cpp
//===========================================================================
