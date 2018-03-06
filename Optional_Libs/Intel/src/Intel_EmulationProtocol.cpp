//===========================================================================
//                           Intel_EmulationProtocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Intel_EmulationProtocol.cpp
//!
//! Implements class Intel_EmulationProtocol
//!
//===========================================================================

#include "Intel_EmulationProtocol.hpp"
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

Intel_EmulationProtocol::Intel_EmulationProtocol(std::vector<uint32_t> Region_addresses)
  : Intel_Packet_Player(Region_addresses)
{
}

Intel_EmulationProtocol::Intel_EmulationProtocol(std::string initialisation_string)
  : Intel_Packet_Player(std::vector<uint32_t>(0x10))
{
}

//! Loopbacks "to SUT data" logging Intel command(s) that would be issued if it was really an operating protocol
//!
BinaryVector Intel_EmulationProtocol::DoCallback (uint32_t endpointId, void* /* interfaceData */, const BinaryVector& toSutData)
{

  auto fromSutData = Send_Intel_Packet_Stream(endpointId, toSutData);

  return fromSutData;
}



//! Forces the ResetPort to be asserted on the target module
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
void Intel_EmulationProtocol::DoReset(bool /* doSynchronousReset */)
{
  LOG(INFO) << "DoReset does nothing";
}

//===========================================================================
// End of Intel_EmulationProtocol.cpp
//===========================================================================
