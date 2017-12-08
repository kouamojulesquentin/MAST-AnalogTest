#include "ServerSocket.h"
#include "SocketException.h"
#include "ClientSocket.h"
#include <string>
#include <stdlib.h>
#include "DFT_Ring_NW.h"
#include <fstream>

using namespace std;

namespace
{
int run_NW_TEST_IF_end ( int ring_end_port, int Addr )
{
  std::bitset<ADDR_WIDTH> Phys_Addr = Addr;
  std::cout << "running Ring endpoint on port "<< ring_end_port<<"....\n";
  std::cout << "With Physical address "<< Phys_Addr<<"....\n";
  std::ofstream  from_ring_file ("./packets_from_ring.bin", ios::out | ios::binary);

  try
    {
      // Create the socket
      ServerSocket in_server ( ring_end_port );

               std::cout << "Ring endpoint received:\n";
      while ( true )
  {

    ServerSocket ring_end;
    in_server.accept ( ring_end );

    try
      {
        while ( true )
    {
      ring_packet data;
      ring_end >> data;
               std::cout<< " 0x"<<std::hex<<data.id.to_ulong()<< " 0x"<<std::hex<<data.opcode.to_ulong() << " 0x" <<std::hex<<data.data.to_ulong() << "\n";
     from_ring_file.write((char*)&data,sizeof(ring_packet));
                 from_ring_file.flush();

    }
      }
    catch ( SocketException& ) {}

  }
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }

  return 0;
}
} // End of unnamed namespace


int main (int /* argc */, char **argv)
{
  run_NW_TEST_IF_end(atoi(argv[1]), atoi(argv[2]));
  return 0;
}
