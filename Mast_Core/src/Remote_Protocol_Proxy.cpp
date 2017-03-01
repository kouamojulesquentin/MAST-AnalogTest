//===========================================================================
//                           Remote_Protocol_Proxy.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Remote_Protocol_Proxy.cpp
//!
//! Implements class Remote_Protocol_Proxy
//!
//===========================================================================

#include "Remote_Protocol_Proxy.hpp"
#include "Remote_Protocol_Client.hpp"
#include "RemoteProtocolFactory.hpp"
#include "Utility.hpp"

using std::unique_ptr;
using std::string;
using std::experimental::string_view;
using std::vector;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;
using namespace mast;


//! Releases actual Remote_Protocol_Client
//!
Remote_Protocol_Proxy::~Remote_Protocol_Proxy ()
{
}


//! Constructs from actual Remote_Protocol_Client and associated commands
//!
//! @param remoteProtocol Actual Remote_Protocol_Client (must be not nullptr)
//! @param commands       Associated commands (first one for reset and next ones for each supported derivations)
//!
Remote_Protocol_Proxy::Remote_Protocol_Proxy (unique_ptr<Remote_Protocol_Client> remoteProtocol, vector<string> commands)
  : m_remoteProtocol (std::move(remoteProtocol))
  , m_commands       (std::move(commands))
{
  CHECK_PARAMETER_NOT_NULL(m_remoteProtocol.get(), "Must provide valid Remote_Protocol_Client");
  CHECK_PARAMETER_GTE(m_commands.size(), 2,  "There must be at least two associated commands (including one for reset)");
}
//
//  End of: Remote_Protocol_Proxy::Remote_Protocol_Proxy
//---------------------------------------------------------------------------


//! Constructs from string parameters defining actual Remote_Protocol_Client and parameters
//!
//! @note Parameters are TEMPORARILY formatted like this:
//!       REMOTE_PROTOCOL_PROXY_PARAMETERS ::= COMMANDS [, KIND_NAME], CREATOR_ID [, REMOTE_CLIENT_PARAMETERS]
//!       COMMANDS                         ::= STRING_LIST
//!       KIND_NAME                        ::= |KIND:STRING|
//!       CREATOR_ID                       ::= |ID:STRING|
//!       REMOTE_CLIENT_PARAMETERS         ::= STRING_LIST
//!
//!       Example: RST, SIR, SDR, |KIND:Remote_Loopback|, |ID:XmlRpc|, http://localhost:8080/RPC2
//!
//!
//! @param parameters Parameters defining actual Remote_Protocol_Client,
//!                   associated commands and optional kindName
//!
Remote_Protocol_Proxy::Remote_Protocol_Proxy (const string& parameters)
{
  CHECK_PARAMETER_NOT_EMPTY(parameters, "Remote_Protocol_Proxy must contain at least a set of commands and an identifier to create Remote_Protocol_Client (got empty parameters)");
  string_view parsedParams(parameters);

  const auto notFound = string_view::npos;

  // ---------------- Define utility to extract a part of parameters
  //
  auto findPart = [&](string_view prefix)
  {
    auto text   = ""s;
    auto begin  = parsedParams.find(prefix);
    auto end    = notFound;

    if (begin != notFound)
    {
      auto middle = begin + prefix.length();
      end         = parsedParams.find("|", middle);

      if (end != notFound)
      {
        text = string(parsedParams.substr(middle, end - middle));
      }
    }
    return make_tuple(begin, end, text);
  };

  // ---------------- Get Kind Name
  //
  auto kindBegin = notFound;
  auto kindEnd   = notFound;
  std::tie(kindBegin, kindEnd, m_kindName) = findPart("|KIND:"sv);
  CHECK_PARAMETER_GT(kindBegin, 5u, "Missing RemoteProtocolFactory commands");

  // ---------------- Get creator id
  //
  auto creatorBegin = notFound;
  auto creatorEnd   = notFound;
  auto creatorId    = ""s;
  std::tie(creatorBegin, creatorEnd, creatorId) = findPart("|ID:"sv);

  CHECK_PARAMETER_NOT_EMPTY(creatorId, "Missing RemoteProtocolFactory creation function identifier");
  CHECK_PARAMETER_GT(creatorBegin, 5u, "Missing RemoteProtocolFactory commands");

  // ---------------- Get creator parameters
  //
  auto remoteProtocolParameters = parsedParams.substr(creatorEnd + 1u);
  Utility::TrimBoth(remoteProtocolParameters);
  if (!remoteProtocolParameters.empty() && (remoteProtocolParameters[0] == ','))
  {
    remoteProtocolParameters.remove_prefix(1u);
    Utility::TrimLeft(remoteProtocolParameters);
  }

  auto& factory    = RemoteProtocolFactory::Instance();
  m_remoteProtocol = factory.Create(creatorId, string(remoteProtocolParameters));

  CHECK_PARAMETER_NOT_NULL(m_remoteProtocol.get(), "Failed to create a remote protocol with id: "sv + creatorId);

  // ---------------- Get commands
  //
  auto commandsEnd = (kindBegin != notFound) ? kindBegin - 1u : creatorBegin - 1u;
  auto commands    = Utility::Split(parsedParams.substr(0, commandsEnd), ","sv);
  for (const auto& command : commands)
  {
    if (!command.empty())
    {
      m_commands.emplace_back(command.cbegin(), command.length());
    }
  }
  CHECK_PARAMETER_GTE(m_commands.size(), 2,  "Could not associate at least two commands (including one for reset)");
}


//! Loopbacks "to SUT data" logging SVF command(s) that would be issued if it was really an operating protocol
//!
BinaryVector Remote_Protocol_Proxy::DoAction (uint32_t derivationId, void* interfaceData, const BinaryVector& toSutData)
{
  CHECK_PARAMETER_NULL (interfaceData, "Interface data is not supported by remote protocols (there is no sharing of address space)");
  CHECK_PARAMETER_LT   (derivationId, m_commands.size(), "Derivation id must not be greater than supported commands");

  auto command       = m_commands[derivationId];
  auto binaryToSut   = toSutData.DataRightAligned();
  auto binaryFromSut = m_remoteProtocol->SendScanVector(command, toSutData.BitsCount(), binaryToSut);
  auto fromSutData   = BinaryVector::CreateFromRightAlignedBuffer(std::move(binaryFromSut), toSutData.BitsCount());

  return fromSutData;
}


//! Forces the ResetPort to be asserted on the target module
//!
//! @param doSynchronousReset   When true, reset shall be done by issuing a synchronous reset sequence
//!
void Remote_Protocol_Proxy::DoReset (bool doSynchronousReset)
{
  m_remoteProtocol->SendDoReset(doSynchronousReset);
}
//
//  End of: Remote_Protocol_Proxy::DoReset
//---------------------------------------------------------------------------



//! Returns readable type of protocol (I2C, SVF_Simu, OpenOCD, SPI...)
//!
string_view Remote_Protocol_Proxy::KindName () const
{
  if (m_kindName.empty() && (m_remoteProtocol != nullptr))
  {
    m_kindName = m_remoteProtocol->KindName();
  }

  return m_kindName;
}
//
//  End of: Remote_Protocol_Proxy::KindName
//---------------------------------------------------------------------------


//===========================================================================
// End of Remote_Protocol_Proxy.cpp
//===========================================================================
