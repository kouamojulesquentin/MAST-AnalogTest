//===========================================================================
//                           StreamerProtocolFactory.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file StreamerProtocolFactory.cpp
//!
//! Implements class StreamerProtocolFactory
//!
//===========================================================================

#include "StreamerProtocolFactory.hpp"
#include "TransparentStreamerProtocol.hpp"
#include "EnumsUtility.hpp"
#include "Utility.hpp"

using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;
using std::string;
using std::experimental::string_view;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;

using mast::StreamerProtocol;
using mast::TransparentStreamerProtocol;
using mast::StreamerProtocolFactory;


//! Constructor...
StreamerProtocolFactory::StreamerProtocolFactory ()
{
}
//
//  End of: StreamerProtocolFactory::StreamerProtocolFactory
//---------------------------------------------------------------------------


//! Removes any, registered, factory
//!
void StreamerProtocolFactory::Clear ()
{
  m_stdCreators.clear();
}
//
//  End of: StreamerProtocolFactory::Clear
//---------------------------------------------------------------------------




//! Called once to create single instance
//!
//! @note This is extracted from Instance() to separate initialization of defaults and request for instance
unique_ptr<StreamerProtocolFactory> StreamerProtocolFactory::CreateFactoryWithDefaultCreators ()
{
  auto instance = unique_ptr<StreamerProtocolFactory>(new StreamerProtocolFactory());
  instance->InitializeWithDefaults();
  return instance;
}
//
//  End of: StreamerProtocolFactory::CreateFactoryWithDefaultCreators
//---------------------------------------------------------------------------


//! Creates an StreamerProtocol using registered creation function
//!
//! @param creatorId            A name that identified registered creation function
//! @param parameters   String of (optional) parameters
//!
unique_ptr<StreamerProtocol> StreamerProtocolFactory::Create (const string&        creatorId,
						     const std::string& parameters) const
{
  auto instance = CreateImpl(m_stdCreators, creatorId, parameters);
  if (!instance)
  {
    THROW_INVALID_ARGUMENT("There is no Streamer protocol registered with name: "sv + creatorId);
  }

  return instance;
}
//
//  End of: StreamerProtocolFactory::Create
//---------------------------------------------------------------------------


//! Fills up with default StreamerProtocol
//!
//! @note By definition, there is no default "Full Custom" creators
//!
void StreamerProtocolFactory::InitializeWithDefaults ()
{
  // ---------------- Standard
  //

  // ---------------- Custom
  //
  m_stdCreators["TransparentStreamer"] = [](const std::string& parameters)
                                    { return make_unique<TransparentStreamerProtocol>(parameters); };
}
//
//  End of: StreamerProtocolFactory::InitializeWithDefaults
//---------------------------------------------------------------------------


//! Returns sole instance
//!
//! @note It is thread safe
StreamerProtocolFactory& StreamerProtocolFactory::Instance ()
{
  static auto instance = CreateFactoryWithDefaultCreators();

  return *instance.get();
}
//
//  End of: StreamerProtocolFactory::Instance
//---------------------------------------------------------------------------



//! Returns the number of factories currently registered (associated with a name)
//!
size_t StreamerProtocolFactory::RegisteredCreatorsCount () const
{
  return  m_stdCreators.size();
}
//
//  End of: StreamerProtocolFactory::RegisteredCreatorsCount
//---------------------------------------------------------------------------


//===========================================================================
// End of StreamerProtocolFactory.cpp
//===========================================================================
