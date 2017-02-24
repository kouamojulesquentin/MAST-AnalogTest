//===========================================================================
//                           XmlRpc_Loopback_Server.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file XmlRpc_Loopback_Server.cpp
//!
//! Xml-Rpc server example using a Remote_Loopback_Protocol
//!
//===========================================================================

#include "Remote_Loopback_Protocol.hpp"
#include "XmlRpc_Protocol_Server.hpp"

using mast::Remote_Loopback_Protocol;
using mast::XmlRpc_Protocol_Server;

//! This is a trivial XmlRpc server with loopback protocol
//!
int main(int argc, const char* argv[])
{
  Remote_Loopback_Protocol protocol;
  XmlRpc_Protocol_Server   server(8080, &protocol);

  server.Start(); // Do not return - use Ctrl-C 'or kill -9 ' to exit
  return 0;
}

//===========================================================================
// End of XmlRpc_Loopback_Server.cpp
//===========================================================================
