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

#include <stdexcept>
#include <iostream>
#include <cstdlib>

using mast::Remote_Loopback_Protocol;
using mast::XmlRpc_Protocol_Server;

//! This is a trivial XmlRpc server with loopback protocol
//!
int main(int argc, const char* argv[])
{
  try
  {
    Remote_Loopback_Protocol protocol;
    XmlRpc_Protocol_Server   server(8080, &protocol);

    server.Start(); // Do not return - use Ctrl-C 'or kill -9 ' to exit
  }
  catch(std::exception& exc)  // Catch C++ standard exceptions
  {
    std::cerr << "XmlRpc Loopback Server got std::exception: " << exc.what();
    return EXIT_FAILURE;
  }
  catch (...)
  {
    std::cerr << "XmlRpc Loopback Server got unknown type exeption";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

//===========================================================================
// End of XmlRpc_Loopback_Server.cpp
//===========================================================================
