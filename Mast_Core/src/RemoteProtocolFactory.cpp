//===========================================================================
//                           RemoteProtocolFactory.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file RemoteProtocolFactory.cpp
//!
//! Implements class RemoteProtocolFactory
//!
//===========================================================================

#include "RemoteProtocolFactory.hpp"
#include "Utility.hpp"

using std::unique_ptr;
using std::string;
using std::experimental::string_view;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;

using mast::RemoteProtocolFactory;
using mast::Remote_Protocol_Client;


//! Called once to create single instance
//!
//! @note This is extracted from Instance() to separate initialization of defaults and request for instance
unique_ptr<RemoteProtocolFactory> RemoteProtocolFactory::CreateFactoryWithDefaultCreators ()
{
  auto instance = unique_ptr<RemoteProtocolFactory>(new RemoteProtocolFactory());
  instance->InitializeWithDefaults();
  return instance;
}
//
//  End of: RemoteProtocolFactory::CreateFactoryWithDefaultCreators
//---------------------------------------------------------------------------



//! Creates an AccessInterfaceProtocol using ceation function indentified by a name and optional parameters
//!
unique_ptr<Remote_Protocol_Client> RemoteProtocolFactory::Create (const string& creatorId, const string& parameters) const
{
  auto instance = CreateImpl(creatorId, parameters);
  if (!instance)
  {
    THROW_INVALID_ARGUMENT("There is no creation method registered with name: "sv + creatorId);
  }

  return instance;
}
//
//  End of: RemoteProtocolFactory::Create
//---------------------------------------------------------------------------




//! Fills up with default AccessInterfaceProtocol
//!
//! @note Does nothing (no default creation function yet)
void RemoteProtocolFactory::InitializeWithDefaults ()
{
}
//
//  End of: RemoteProtocolFactory::InitializeWithDefaults
//---------------------------------------------------------------------------


//! Returns sole instance
//!
//! @note It is thread safe
RemoteProtocolFactory& RemoteProtocolFactory::Instance ()
{
  static auto instance = CreateFactoryWithDefaultCreators();

  return *instance.get();
}
//
//  End of: RemoteProtocolFactory::Instance
//---------------------------------------------------------------------------



//===========================================================================
// End of RemoteProtocolFactory.cpp
//===========================================================================
