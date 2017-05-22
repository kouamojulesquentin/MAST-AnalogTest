//===========================================================================
//                           Intel_Packet_Player.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Intel_Packet_Player.hpp
//!
//! Declares Intel_Packet_Player class
//!
//===========================================================================


#ifndef Intel_Packet_PLAYER_H__CE0DE4BD_2B23_4BD0_82B1_DCE5A1CD5B2__INCLUDED_
  #define Intel_Packet_PLAYER_H__CE0DE4BD_2B23_4BD0_82B1_DCE5A1CD5B2__INCLUDED_

#include "AccessInterfaceProtocol.hpp"

#include <string>
#include <vector>
#include <thread>
#include <queue>
#include "ServerSocket.h"
#include "SocketException.h"
#include "ClientSocket.h"
#include "DFT_Ring_NW.h"
#include "blockingqueue.hpp"

using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;

namespace mast
{
//! Constructs Intel_Packet commands for Intel_Packet related AccessInterfaceProtocol
//!
class Intel_Packet_Player : public AccessInterfaceProtocol
{
  // ---------------- Public  Methods
  //
  public:
  ~Intel_Packet_Player();
  Intel_Packet_Player() = delete;
  Intel_Packet_Player(std::vector<uint32_t> Region_addresses);

  //! Returns true when TRST JTAG port is supported
  //!
  bool  SupportTRST() const { return m_supportTRST; }

  //! Sets whether TRST JTAG port is supported
  //!
  void  SupportTRST (bool supportTRST) { m_supportTRST = supportTRST; }


  //! Sends a stream of Intel Packets from BinaryVector to send to SUT and returns received data
  //!
  BinaryVector Send_Intel_Packet_Stream(uint32_t derivationId, const BinaryVector& toSutData) const;

  //! Creates an Intel_Packet reset command
  //!
  void CreateResetIntel_PacketCommand (bool doSynchronousReset) const ;

 void send_packet(std::bitset<2> opcode, std::bitset<ID_WIDTH> id, std::bitset<DATA_WIDTH> data) const;
 ring_packet receive_packet() const;


  // ---------------- Private  Fields
  //
  bool m_supportTRST = false; //!< When true, TRST instruction is used for reset unless synchronous reset is requested
  uint32_t m_n_chains = 0; //!< When true, TRST instruction is used for reset unless synchronous reset is requested
  blocking_queue<ring_packet> *outgoing_packets;
  blocking_queue<ring_packet> *incoming_packet_queue;

  std::vector<std::bitset<ID_WIDTH>> region_id;
  std::vector<std::bitset<ADDR_WIDTH>> region_addr;

  std::thread out_client;
  std::thread in_server;


};
//
//  End of Intel_Packet_Player class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined Intel_Packet_PLAYER_H__CE0DE4BD_2B23_4BD0_82B1_DCE5A1CD5B2__INCLUDED_

//===========================================================================
// End of Intel_Packet_Player.hpp
//===========================================================================
