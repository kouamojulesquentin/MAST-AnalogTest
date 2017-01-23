// Definition of the ServerSocket class
#include <bitset>

#ifndef DFT_Ring_NW_H
#define DFT_Ring_NW_H

#define FIRST_PORT 30000
#define N_REGIONS  1 
#define LAST_PORT FIRST_PORT+N_REGIONS

#define ID_WIDTH 2
#define DATA_WIDTH 8
#define ADDR_WIDTH DATA_WIDTH

#define SERVER_ADDR 1
#define FIRST_REGION_ADDR SERVER_ADDR+1

#define SELECT		0x0
#define CONTROL		0x1
#define DATA		0x2



class ring_packet
 {
  public :
  ~ring_packet() = default;
  ring_packet() = default;
  ring_packet( std::bitset<ID_WIDTH> a_id, std::bitset<ID_WIDTH> a_opcode, std::bitset<DATA_WIDTH> a_data)
    {id=a_id; opcode= a_opcode; data= a_data;};
  
  std::bitset<ID_WIDTH> id;
  std::bitset<ID_WIDTH> opcode;
  std::bitset<DATA_WIDTH> data;
 };

#endif
