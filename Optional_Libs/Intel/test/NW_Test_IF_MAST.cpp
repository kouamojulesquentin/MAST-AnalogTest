#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdlib.h>
#include "DFT_Ring_NW.h"
#include <sstream>
#include <fstream>

using namespace std;

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()


namespace
{
void send_packet(ClientSocket&           ring_input,
                 std::bitset<2>          opcode,
                 std::bitset<ID_WIDTH>   id,
                 std::bitset<DATA_WIDTH> data)
{
  ring_packet test_packet;
  test_packet.id     = id;
  test_packet.opcode = opcode;
  test_packet.data   = data;

  ring_input << test_packet;
  sleep(0.3);
}

int run_TEST_IF_start (int ring_start_port)
{
  FILE* C_to_ring_file;
  C_to_ring_file = fopen("./packets_to_ring.bin", "rb");

  if (C_to_ring_file != NULL)
    std::cout << "OPEN\n";
  else  
   {std::cerr << "FILE NOT FOUND\n"; exit(1);}
 
  ring_packet packet;

  try
  {
    ClientSocket ring_input("localhost", ring_start_port);

    while (true)
    {
      while (fread(&packet, sizeof(ring_packet), 1, C_to_ring_file) == 0);

      std::cout << "Sent packet Id 0x" << std::hex << packet.id.to_ulong() << " Opdcode 0x" << std::hex
                << packet.opcode.to_ulong() << " Data 0x" << std::hex << packet.data.to_ulong() << "\n";

      send_packet(ring_input, packet.opcode, packet.id, packet.data);
    }
  }
  catch(SocketException & e)
  {
    std::cout << "Exception was caught:" << e.description() << "\n";
  }

  std::cout << "Exiting\n";
  return 0;
}

} // End of unnamed namespace


int main (int /* argc */, char** /* argv */)
{
  run_TEST_IF_start(FIRST_PORT);

  return 0;
}
