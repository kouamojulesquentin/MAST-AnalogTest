//===========================================================================
//                           XmlRpc_Protocol_Common.hpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file XmlRpc_Protocol_Common.hpp
//!
//! Declares commonalities between client and server classes
//!
//===========================================================================


#ifndef XMLRPC_PROTOCOL_COMMON_H__772E6CE1_403F_44B6_63AB_26D8E15D7EF6__INCLUDED_
  #define XMLRPC_PROTOCOL_COMMON_H__772E6CE1_403F_44B6_63AB_26D8E15D7EF6__INCLUDED_

#include <map>
#include <string>
#include <xmlrpc-c/base.hpp>

#define XML_RPC_COMMAND_DO_RESET         "XmlRpc_Protocol_Server.DoReset"
#define XML_RPC_COMMAND_SEND_SCAN_VECTOR "XmlRpc_Protocol_Server.SendScanVector"

typedef std::map<std::string, xmlrpc_c::value> StructFields_t; //!< Defines with what struct fields are streamed with XML-RPC

// Name for returning "struct" fields
#define XML_RPC_FIELD_BITS_COUNT    "bitsCount"
#define XML_RPC_FIELD_FROM_SUT_DATA "fromSutData"

// Error code used by server to report and issue to the client
//
// It uses positive values to avoid conflict with xmlrpc-c framework negative error codes
//
#define XML_RPC_SUCCESS                       0
//+#define XML_RPC_ERROR_UNKNWOW_EXCEPTION       100
//+#define XML_RPC_ERROR_STD_EXCEPTION           101
//+#define XML_RPC_ERROR_MISSING_REMOTE_PROTOCOL 102

#endif  // not defined XMLRPC_PROTOCOL_COMMON_H__772E6CE1_403F_44B6_63AB_26D8E15D7EF6__INCLUDED_
//===========================================================================
// End of XmlRpc_Protocol_Common.hpp
//===========================================================================



