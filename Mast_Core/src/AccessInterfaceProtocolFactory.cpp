//===========================================================================
//                           AccessInterfaceProtocolFactory.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterfaceProtocolFactory.cpp
//!
//! Implements class AccessInterfaceProtocolFactory
//!
//===========================================================================

#include "AccessInterfaceProtocolFactory.hpp"
#include "Utility.hpp"

// For default AccessInterfaceProtocol
#include "OfflineProtocol.hpp"
#include "LoopbackAccessInterfaceProtocol.hpp"
#include "SVF_SimulationProtocol.hpp"
#include "SVF_EmulationProtocol.hpp"
#include "STIL_EmulationProtocol.hpp"
#include "I2C_EmulationProtocol.hpp"
#include "Remote_Protocol_Proxy.hpp"

#include <memory>


using std::unique_ptr;
using std::make_unique;
using std::string;
using std::experimental::string_view;

using namespace std::experimental::literals::string_view_literals;
using namespace mast;

using mast::AccessInterfaceProtocolFactory;

//! Called once to create single instance
//!
//! @note This is extracted from Instance() to separated initialization of defaults and request for instance
std::unique_ptr<AccessInterfaceProtocolFactory> AccessInterfaceProtocolFactory::CreateInstanceWithDefaultFactories ()
{
  auto instance = unique_ptr<AccessInterfaceProtocolFactory>(new AccessInterfaceProtocolFactory());
  instance->InitializeWithDefaults();
  return instance;
}
//
//  End of: AccessInterfaceProtocolFactory::CreateInstanceWithDefaultFactories
//---------------------------------------------------------------------------



//! Creates an AccessInterfaceProtocol using ceation function indentified by a name and optional parameters
//!
unique_ptr<AccessInterfaceProtocol> AccessInterfaceProtocolFactory::Create (const std::string& creatorId, const std::string& parameters) const
{
  auto instance = CreateImpl(creatorId, parameters);
  if (!instance)
  {
    THROW_INVALID_ARGUMENT("There is no creation method registered with name: "sv + creatorId);
  }

  return instance;
}
//
//  End of: AccessInterfaceProtocolFactory::Create
//---------------------------------------------------------------------------




//! Fills up with default AccessInterfaceProtocol
//!
void AccessInterfaceProtocolFactory::InitializeWithDefaults ()
{
  RegisterCreator("Loopback",       [](const string& /* parameters */) { return make_unique<LoopbackAccessInterfaceProtocol>();     });
  RegisterCreator("SVF_Simulation", [](const string& /* parameters */) { return make_unique<SVF_SimulationProtocol>();              });
  RegisterCreator("SVF_Emulation",  [](const string& /* parameters */) { return make_unique<SVF_EmulationProtocol>();               });
  RegisterCreator("Offline",             [](const string& /* parameters */) { return make_unique<OfflineProtocol>();                     });
  RegisterCreator("STIL_Emulation",      [](const string& nbEndPoints)    { return make_unique<STIL_EmulationProtocol>(nbEndPoints); });
  RegisterCreator("I2C_Emulation",       [](const string& parameters)       { return make_unique<I2C_EmulationProtocol>(parameters);     });
  RegisterCreator("RemoteProxy",         [](const string& parameters)       { return make_unique<Remote_Protocol_Proxy>(parameters);     });
}
//
//  End of: AccessInterfaceProtocolFactory::InitializeWithDefaults
//---------------------------------------------------------------------------


//! Returns sole instance
//!
//! @note It is thread safe
AccessInterfaceProtocolFactory& AccessInterfaceProtocolFactory::Instance ()
{
  static auto instance = CreateInstanceWithDefaultFactories();

  return *instance.get();
}
//
//  End of: AccessInterfaceProtocolFactory::Instance
//---------------------------------------------------------------------------

//===========================================================================
// End of AccessInterfaceProtocolFactory.cpp
//===========================================================================
