//===========================================================================
//                           AccessInterfaceProtocolFactories.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterfaceProtocolFactories.cpp
//!
//! Implements class AccessInterfaceProtocolFactories
//!
//===========================================================================

#include "AccessInterfaceProtocolFactories.hpp"
#include "Utility.hpp"

// For default AccessInterfaceProtocol
#include "OfflineProtocol.hpp"
#include "LoopbackAccessInterfaceProtocol.hpp"
#include "SVF_SimulationProtocol.hpp"
#include "SVF_EmulationProtocol.hpp"
#include "STIL_EmulationProtocol.hpp"
#include "I2C_EmulationProtocol.hpp"

#include <memory>


using std::unique_ptr;
using std::shared_ptr;
using std::make_shared;
using std::string;
using std::experimental::string_view;

using namespace std::experimental::literals::string_view_literals;
using namespace mast;

using mast::AccessInterfaceProtocolFactories;

//! Called once to create single instance
//!
//! @note This is extracted from Instance() to separated initialization of defaults and request for instance
std::unique_ptr<AccessInterfaceProtocolFactories> AccessInterfaceProtocolFactories::CreateInstanceWithDefaultFactories ()
{
  auto instance = unique_ptr<AccessInterfaceProtocolFactories>(new AccessInterfaceProtocolFactories());
  instance->InitializeWithDefaults();
  return instance;
}
//
//  End of: AccessInterfaceProtocolFactories::CreateInstanceWithDefaultFactories
//---------------------------------------------------------------------------



//! Creates an AccessInterfaceProtocol using ceation function indentified by a name and optional parameters
//!
shared_ptr<AccessInterfaceProtocol> AccessInterfaceProtocolFactories::Create (const std::string& creatorId, const std::string& parameters) const
{
  auto instance = CreateImpl(creatorId, parameters);
  if (!instance)
  {
    THROW_INVALID_ARGUMENT("There is no creation method registered with name: "sv + creatorId);
  }

  return instance;
}
//
//  End of: AccessInterfaceProtocolFactories::Create
//---------------------------------------------------------------------------




//! Fills up with default AccessInterfaceProtocol
//!
void AccessInterfaceProtocolFactories::InitializeWithDefaults ()
{
  RegisterCreator("JTAG_Loopback",       [](const string& /* parameters */) { return make_shared<LoopbackAccessInterfaceProtocol>();     });
  RegisterCreator("JTAG_SVF_Simulation", [](const string& /* parameters */) { return make_shared<SVF_SimulationProtocol>();              });
  RegisterCreator("JTAG_SVF_Emulation",  [](const string& /* parameters */) { return make_shared<SVF_EmulationProtocol>();               });
  RegisterCreator("Offline",             [](const string& /* parameters */) { return make_shared<OfflineProtocol>();                     });
  RegisterCreator("STIL_Emulation",      [](const string& nbDerivations)    { return make_shared<STIL_EmulationProtocol>(nbDerivations); });
  RegisterCreator("I2C_Emulation",       [](const string& parameters)       { return make_shared<I2C_EmulationProtocol>(parameters);     });
}
//
//  End of: AccessInterfaceProtocolFactories::InitializeWithDefaults
//---------------------------------------------------------------------------


//! Returns sole instance
//!
//! @note It is thread safe
AccessInterfaceProtocolFactories& AccessInterfaceProtocolFactories::Instance ()
{
  static auto instance = CreateInstanceWithDefaultFactories();

  return *instance.get();
}
//
//  End of: AccessInterfaceProtocolFactories::Instance
//---------------------------------------------------------------------------

//===========================================================================
// End of AccessInterfaceProtocolFactories.cpp
//===========================================================================
