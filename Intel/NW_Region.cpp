#include "ServerSocket.h"
#include "SocketException.h"
#include "ClientSocket.h"
#include <string>
#include <stdlib.h>
#include "DFT_Ring_NW.h"

using namespace std;

namespace
{
std::bitset<DATA_WIDTH> shift_8bit_dirty( std::bitset<DATA_WIDTH*4>& data, std::bitset<DATA_WIDTH> new_data)
{
  std::bitset<DATA_WIDTH> tmp;
  int i;

 for (i=0;i<DATA_WIDTH;i++)
   tmp[i]=data[i];
 data = data >>   DATA_WIDTH;
 for (i=0;i<DATA_WIDTH;i++)
   data[i+3*DATA_WIDTH]=new_data[i];

 return tmp;
}

int run_Region (int region_port, int Addr)
{
  std::bitset <ADDR_WIDTH>Phys_Addr = Addr;
  std::cout << "running Region on port " << region_port << "....\n";
  std::cout << "With Physical address " << Phys_Addr << "....\n";

  std::bitset<DATA_WIDTH*4> ScanData      = 0xAABBCCDD;
  std::bitset<DATA_WIDTH>   ScanData_Addr = 0x11;
  std::bitset<DATA_WIDTH>   ScanCtl       = 0;
  std::bitset<DATA_WIDTH>   ScanCtl_Addr  = 0x10;

  //Region status registers
  std::bitset<ID_WIDTH>   ID          = 0;
  std::bitset<DATA_WIDTH> Current_Ctl = 0;
  std::bitset<DATA_WIDTH> tmp;

  try
  {
    // Create the socket
    ServerSocket in_server(region_port);
    ClientSocket region_out("localhost", region_port + 1);

    cout << "ID 0x" << hex << ID.to_ulong() << " Current_Ctl 0x" << hex << Current_Ctl.to_ulong() << " ScanData 0x"
         << hex << ScanData.to_ulong() << " ScanCtl 0x" << hex << ScanCtl.to_ulong() << "\n";

    while (true)
    {
      ServerSocket region_input;
      in_server.accept(region_input);

      try
      {
        while (true)
        {
          ring_packet test_packet;
          region_input >> test_packet;

          int opcode = (int) test_packet.opcode.to_ulong();

          switch (opcode)
          {
            case SELECT :
              if (test_packet.data == Phys_Addr)
                ID = test_packet.id;
              break;
            case CONTROL :
              if (test_packet.id == ID)
              {
                tmp = Current_Ctl;
                Current_Ctl = test_packet.data;
                test_packet.data = tmp;
              }
              break;
            case DATA :
              tmp = test_packet.data;
              if (Current_Ctl == ScanData_Addr)
              {
                test_packet.data = shift_8bit_dirty(ScanData, test_packet.data);
              }
              if (Current_Ctl == ScanCtl_Addr)
              {
                ScanCtl = tmp;
                test_packet.data = ScanCtl;
              }
              break;
            default :
              break;
          }
          region_out << test_packet;

          cout << "ID 0x" << hex << ID.to_ulong() << " Current_Ctl 0x" << hex << Current_Ctl.to_ulong()
               << " ScanData 0x" << hex << ScanData.to_ulong() << " ScanCtl 0x" << hex << ScanCtl.to_ulong() << "\n";
        }
      }
      catch(SocketException&)
      {
      }
    }
  }
  catch(SocketException & e)
  {
    std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
  }

  return 0;
}

} // End of unnamed namespace



int main ( int /* argc */, char **argv )
{
  run_Region(atoi(argv[1]), atoi(argv[2]));
  return 0;
}
