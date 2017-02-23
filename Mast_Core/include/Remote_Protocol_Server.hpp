//===========================================================================
//                           Remote_Protocol_Server.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Remote_Protocol_Server.hpp
//!
//! Declares Remote_Protocol_Server class
//!
//===========================================================================


#ifndef REMOTE_PROTOCOL_SERVER_H__17CD0EF2_7C46_4D3F_7889_E6D02F65DAEB__INCLUDED_
  #define REMOTE_PROTOCOL_SERVER_H__17CD0EF2_7C46_4D3F_7889_E6D02F65DAEB__INCLUDED_

#include <string>
#include <vector>

#ifndef uint32_t
#include <stdint.h>     // Make it compilable as C++03
#endif

namespace mast
{
class Remote_Protocol;

//! Represents a remote protocol with client role for RPC
//!
//! @note This is an abstract base class
//!
//! @note MUST be compilable by C++03 only compilers !
//!
class Remote_Protocol_Server
{
  // ---------------- Public  Methods
  //
  public:
  virtual ~Remote_Protocol_Server() {}

  Remote_Protocol_Server(uint32_t portNumber, Remote_Protocol* pProtocol)
    : m_portNumber (portNumber)
    , m_pProtocol  (pProtocol)
  {}


  Remote_Protocol_Server()
    : m_portNumber (8080)
    , m_pProtocol  (0)
  {
  }

  //! Starts server
  //! @note This is a blocking call that never return !!!
  virtual void Start() = 0;

  //! Returns port number to listen to
  //!
  uint32_t  PortNumber() const { return m_portNumber; }

  //! Sets port number to listen
  //!
  //! @note Must be called before calling Start (or defined at construction)
  //!
  void  PortNumber (uint32_t portNumber) { m_portNumber = portNumber; }

  //! Returns currently assigned Remote_Protocol
  //!
  Remote_Protocol*  Protocol() const { return m_pProtocol; }

  //! Sets actual Remote_Protocol to use when forwarding commands
  //!
  //! @note Must be called before calling Start (or defined at construction)
  //!
  void  Protocol (Remote_Protocol* protocol) { m_pProtocol = protocol; }

  // ---------------- Private  Fields
  //
  private:
  uint32_t         m_portNumber; //!< Port number to listen to for clients
  Remote_Protocol* m_pProtocol;  //!< Current actual Access Interface Protocol (lifetime must be managed by code controlling the server instance)
};
//
//  End of Remote_Protocol_Server class declaration
//---------------------------------------------------------------------------
} // End of namespace mast

#endif  // not defined REMOTE_PROTOCOL_SERVER_H__17CD0EF2_7C46_4D3F_7889_E6D02F65DAEB__INCLUDED_

//===========================================================================
// End of Remote_Protocol_Server.hpp
//===========================================================================
