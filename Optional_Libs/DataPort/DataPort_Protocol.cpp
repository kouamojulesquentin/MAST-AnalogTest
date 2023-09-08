//===========================================================================
//                           DataPort_Protocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DataPort_Protocol.cpp
//!
//! Implements class DataPort_Protocol
//!
//===========================================================================

#include "DataPort_Protocol.hpp"
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


DataPort_Protocol::DataPort_Protocol(std::string portname)
{
 m_portname = portname;
}

//! This callback only causes a "strobe" to be issued for reading value on the Port
//! Only Loopback data is returned 
//!
BinaryVector DataPort_Protocol::DoCallback (std::string /*CallbackId*/, uint32_t /*channelId*/, void* /* interfaceData */, const BinaryVector& toSutData)
{
 auto c_portname = m_portname.c_str();
  RVFRequest request(DATAPORTREAD,(void *)c_portname);

  PushRequest(request);
  
                       /*NB: this is a BLOCKING call*/
  PopfromSut();


  return toSutData;
}



//! Forces the ResetPort to be asserted on the target module
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
void DataPort_Protocol::DoReset(bool /* doSynchronousReset */)
{
  LOG(INFO) << "DoReset does nothing";
}

//===========================================================================
// End of DataPort_Protocol.cpp
//===========================================================================
