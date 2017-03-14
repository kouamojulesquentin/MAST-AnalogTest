//===========================================================================
//                           Intel_Packet_Player.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Intel_Packet_Player.cpp
//!
//! Implements class Intel_Packet_Player
//!
//===========================================================================

#include "Intel_Packet_Player.hpp"
#include "SVFVector.hpp"
#include "Utility.hpp"
#include "g3log/g3log.hpp"

#include <experimental/string_view>
#include <sstream>
#include <fstream>
#include <thread>
#include <queue>

#include "ClientSocket.h"
#include "SocketException.h"
#include "DFT_Ring_NW.h"
#include "blockingqueue.hpp"

using namespace mast;
using namespace std;
using std::string;
using std::experimental::string_view;
using std::ostringstream;


#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

void outgoing_packet_client(blocking_queue<ring_packet>& outgoing_packets);
void incoming_packet_server(blocking_queue<ring_packet>& incoming_packet_queue);

Intel_Packet_Player::Intel_Packet_Player(std::vector<uint32_t> Region_addresses)
 {
 auto n_chains = Region_addresses.size();
 m_n_chains = Region_addresses.size();
 std::bitset<ID_WIDTH> available_id=0x1;

  outgoing_packets = new blocking_queue<ring_packet>(20);
  incoming_packet_queue = new  blocking_queue<ring_packet> (20);

  /*First (Reset) region*/
  region_id.push_back(0);
  region_addr.push_back(0);

 for (unsigned int i =0;i<n_chains;i++)
  {
  region_id.push_back(available_id);
  region_addr.push_back(FIRST_REGION_ADDR);
  }

 /*Create emulation backend Interface*/

  in_server=std::thread (incoming_packet_server,std::ref(*incoming_packet_queue));

 out_client=std::thread (outgoing_packet_client,std::ref(*outgoing_packets));

  out_client.detach();
  in_server.detach();


 /*end of emulation backend*/
}

 Intel_Packet_Player::~Intel_Packet_Player()
  {}

void Intel_Packet_Player::send_packet(std::bitset<2> opcode, std::bitset<ID_WIDTH> id, std::bitset<DATA_WIDTH> data) const
 {
       ring_packet test_packet;
      test_packet.id = id;
      test_packet.opcode = opcode;
     test_packet.data = data;

    this->outgoing_packets->put(test_packet);
    return;
 }

ring_packet Intel_Packet_Player::receive_packet() const
 {

    auto packet = incoming_packet_queue->take();
    return packet;
 }

void outgoing_packet_client(blocking_queue<ring_packet>& outgoing_packets)
 {

  ofstream  to_ring_file ("packets_to_ring.bin", ios::out | ios::binary);

  try
    {
  while(true)
   {
    if (outgoing_packets.size()>0)
     {
    auto packet = outgoing_packets.take(); //get first packet
     to_ring_file.write((char*)&packet,sizeof(ring_packet));
     to_ring_file.flush();

     }
   }
   }
  catch ( SocketException& e )
    {
      std::cout << "Outgoing client exception was caught:" << e.description() << "\n";
    }
 }


void incoming_packet_server(blocking_queue<ring_packet>& incoming_packet_queue)
 {

  /*Open file in W mode to empty it*/
  ofstream  from_ring_file ("packets_from_ring.bin", ios::out | ios::binary);
  from_ring_file.close();

  FILE *C_from_ring_file;
  C_from_ring_file=fopen("packets_from_ring.bin","rb");
    try
    {

      while(true)
      {
      ring_packet packet;

    while (fread(&packet,sizeof(ring_packet),1,C_from_ring_file)==0);
      incoming_packet_queue.put(packet);
      }
   }
  catch ( SocketException& e )
    {
      std::cout << "Incoming server Exception was caught:" << e.description() << "\nExiting.\n";
    }

 }

//! Creates an Intel_Packet command associated to derivation identifier and BinaryVector to send to SUT
//!
BinaryVector Intel_Packet_Player::Send_Intel_Packet_Stream (uint32_t derivationId, const BinaryVector& toSutData) const
{
  BinaryVector from_SUT;

  if (derivationId == 0)
  {
    return from_SUT;
  }

  if (derivationId > m_n_chains)
    THROW_INVALID_ARGUMENT("DerivationId must be comprised between '0' (for Reset) and the total number of chains");

  std::bitset <ID_WIDTH>   stream_id = region_id[derivationId];
//+  std::bitset <ADDR_WIDTH> Phy_Addr = region_addr[derivationId];
  /*Assign target region a stream id */
  send_packet(SELECT, stream_id, FIRST_REGION_ADDR);
  receive_packet(); //SELECT packet comes back identical from ring

  /*Select SCANCTL*/
  send_packet(CONTROL, stream_id, 0x10);
  receive_packet(); //CONTROL packet comes back from ring with previous CONTROL value
  /*Enable scan on region*/
  send_packet(DATA, stream_id, 0x03);
  receive_packet(); //DATA packet comes back from ring with previous SCANCTL value

  /*Select SCANDATA */
  send_packet(CONTROL, stream_id, 0x11);
  receive_packet(); //CONTROL packet comes back from ring with previous CONTROL value (here: SCANCTL, 0x10);

//+  std::vector <uint8_t> packets_to_send;

  /*Send Data: manually handle covnersion to network ordering (big endian)*/
  unsigned int          offset = toSutData.BitsCount() - DATA_WIDTH;
  std::vector <uint8_t> nw_order_data;
  int                   sent_packets = 0;
  BinaryVector          slice;
  ring_packet           packet;

  while (offset != 0)
  {
    uint8_t packet_data;

    slice = toSutData.Slice(offset, DATA_WIDTH);
    slice.Get(packet_data);

    offset -= DATA_WIDTH;
    send_packet(DATA, stream_id, packet_data);
    sent_packets++;

    /*Retrieve data from received packets*/
    packet = receive_packet();
    packet_data = static_cast <uint8_t>(packet.data.to_ulong());
    nw_order_data.insert(nw_order_data.begin(), packet_data);
  }


  /*manually handle conversion  from network ordering (big endian)*/

  for (auto cur_byte : nw_order_data)
    from_SUT.Append(cur_byte);


  /*release stream id*/
  send_packet(SELECT, 0x0, FIRST_REGION_ADDR);
  receive_packet(); //SELECT packet comes back identical from ring

  return from_SUT;
}
//
//  End of: Intel_Packet_Player::CreateIntel_PacketCommand
//---------------------------------------------------------------------------


//! Creates an Intel_Packet reset command
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
void Intel_Packet_Player::CreateResetIntel_PacketCommand (bool /* doSynchronousReset */) const
{
  LOG(INFO) << "CreateResetIntel_PacketCommand does nothing!";
}
//
//  End of: Intel_Packet_Player::CreateIntel_PacketCommand
//---------------------------------------------------------------------------


//===========================================================================
// End of Intel_Packet_Player.cpp
//===========================================================================
