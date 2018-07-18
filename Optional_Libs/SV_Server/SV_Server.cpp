#include <stdio.h>
#include <stdlib.h>

#include "Remote_Loopback_Protocol.hpp"
#include "XmlRpc_Protocol_Server.hpp"
#include "DPI_Protocol.hpp"
#include "MTQueue.hpp"

/*INCLUDING the CPP otherwise linking fails*/
#include "DPI_interface.hpp"

#include <stdexcept>
#include <iostream>
#include <cstdlib>

#include <stdio.h>
#include <stdlib.h>

#include <svdpi.h>
#include "vc_hdrs.h"

using mast::Remote_Loopback_Protocol;
using mast::XmlRpc_Protocol_Server;
using mast::DPI_Protocol;
using namespace std;


int start_server ()
{
  try
  {
    Remote_Loopback_Protocol test_protocol;
    DPI_Protocol protocol;
    XmlRpc_Protocol_Server   server(8080, &protocol);
 
    std::cout  << "DPI: starting RPC server\n";
    server.LogInfos (true);
    server.LogErrors(true);
    server.Start(); // Do not return - use Ctrl-C 'or kill -9 ' to exit
  }
  catch(std::exception& exc)  // Catch C++ standard exceptions
  {
    std::cerr << "SystemVerilog DPI Server got std::exception: " << exc.what();
    return EXIT_FAILURE;
  }
  catch (...)
  {
    std::cerr << "SystemVerilog DPI Server got unknown type exeption";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

