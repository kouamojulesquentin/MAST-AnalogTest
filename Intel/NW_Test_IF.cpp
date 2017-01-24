#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdlib.h>
#include "DFT_Ring_NW.h"
#include <sstream>

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
  test_packet.id = id;
  test_packet.opcode = opcode;
  test_packet.data = data;

  std::cout << "Sent test_packet Id 0x" << std::hex << test_packet.id.to_ulong() << " Opdcode 0x" << std::hex
            << test_packet.opcode.to_ulong() << " Data 0x" << std::hex << test_packet.data.to_ulong() << "\n";

  ring_input << test_packet;
  sleep(0.3);
}


int run_TEST_IF_start ( int ring_start_port )
{
  try
    {

      ClientSocket ring_input ( "localhost",ring_start_port );


     send_packet(ring_input,SELECT,0x2,FIRST_REGION_ADDR);
     send_packet(ring_input,CONTROL,0x2,0x10);
     send_packet(ring_input,DATA,0x2,0x03);
     send_packet(ring_input,CONTROL,0x2,0x11);
     send_packet(ring_input,DATA,0x2,0x11);
     send_packet(ring_input,DATA,0x2,0x22);
     send_packet(ring_input,SELECT,0x0,FIRST_REGION_ADDR);

    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

   std::cout << "Exiting\n";
  return 0;
}

} // End of unnamed namespace


int main (int /* argc */, char ** /* argv */)
{
  std :: string run_region = "xterm -e ./NW_Region ";
  run_region.append(SSTR(FIRST_PORT)+" "+SSTR(FIRST_REGION_ADDR));
  run_region.append("&");
  std :: string run_enpoint = "xterm -e ./NW_Test_IF_end ";
  run_enpoint.append(SSTR(LAST_PORT)+" "+SSTR(SERVER_ADDR));
  run_enpoint.append("&");
  system (run_enpoint.c_str ());
  sleep(1);
  std::cout << run_region <<"\n";
  system (run_region.c_str ());
  sleep(1);

  run_TEST_IF_start (FIRST_PORT );

  return 0;
}
