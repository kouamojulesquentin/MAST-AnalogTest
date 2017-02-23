//===========================================================================
//                           XmlRpc_Protocol_Server.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file XmlRpc_Protocol_Server.hpp
//!
//! Declares XmlRpc_Protocol_Server class
//!
//===========================================================================


#ifndef XMLRPC_PROTOCOL_SERVER_H__A7E7B29D_AF3A_422A_75BA_2A443AA9CEBD__INCLUDED_
  #define XMLRPC_PROTOCOL_SERVER_H__A7E7B29D_AF3A_422A_75BA_2A443AA9CEBD__INCLUDED_

#include "Remote_Protocol_Server.hpp"

#include <string>
#ifndef uint32_t
#include <stdint.h>     // Make it compilable as C++03
#endif


namespace mast
{
//! Acts as a remote server based upon Xml-RPC protocol
//!
class XmlRpc_Protocol_Server : public Remote_Protocol_Server
{
  // ---------------- Public  Methods
  //
  public:
  ~XmlRpc_Protocol_Server();

  XmlRpc_Protocol_Server(uint32_t portNumber, Remote_Protocol* protocol)
    : Remote_Protocol_Server (portNumber, protocol)
  {}

  XmlRpc_Protocol_Server()
  {}

  //! Constructs using string encoded parameters
//+  XmlRpc_Protocol_Server(const std::string& parameters)
//+    : Remote_Protocol_Server (parameters)
//+  {
//+  }

  //! Starts server
  //! @note This is a blocking call that never return !!!
  virtual void Start();
};
//
//  End of XmlRpc_Protocol_Server class declaration
//---------------------------------------------------------------------------
} // End of namespace mast



#endif  // not defined XMLRPC_PROTOCOL_SERVER_H__A7E7B29D_AF3A_422A_75BA_2A443AA9CEBD__INCLUDED_

//===========================================================================
// End of XmlRpc_Protocol_Server.hpp
//===========================================================================
