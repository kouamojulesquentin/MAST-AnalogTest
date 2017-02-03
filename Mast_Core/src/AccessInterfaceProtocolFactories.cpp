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
std::unique_ptr<AccessInterfaceProtocolFactories> AccessInterfaceProtocolFactories::CreateInstanceWithDefaultFactories ()
{
  auto instance = unique_ptr<AccessInterfaceProtocolFactories>(new AccessInterfaceProtocolFactories());
  instance->InitializeWithDefaults();
  return instance;
}
//
//  End of: AccessInterfaceProtocolFactories::CreateInstanceWithDefaultFactories
//---------------------------------------------------------------------------



//! Creates an AccessInterfaceProtocol using factory indentified by a name and optional parameters
//!
shared_ptr<AccessInterfaceProtocol> AccessInterfaceProtocolFactories::CreateProtocol (const std::string& factoryName, const std::string& parameters) const
{
  shared_ptr<AccessInterfaceProtocol> protocol;

  auto pos = m_factories.find(factoryName);
  if (pos != m_factories.end())
  {
    protocol = pos->second(parameters);
  }
  else
  {
    THROW_INVALID_ARGUMENT("There is no factory registered with name: "sv + factoryName);
  }

  return protocol;
}
//
//  End of: AccessInterfaceProtocolFactories::CreateProtocol
//---------------------------------------------------------------------------




//! Fills up with default AccessInterfaceProtocol
//!
void AccessInterfaceProtocolFactories::InitializeWithDefaults ()
{
  m_factories["JTAG_Loopback"]       = [](const string& /* parameters */) { return make_shared<LoopbackAccessInterfaceProtocol>();     };
  m_factories["JTAG_SVF_Simulation"] = [](const string& /* parameters */) { return make_shared<SVF_SimulationProtocol>();              };
  m_factories["JTAG_SVF_Emulation"]  = [](const string& /* parameters */) { return make_shared<SVF_EmulationProtocol>();               };
  m_factories["Offline"]             = [](const string& /* parameters */) { return make_shared<OfflineProtocol>();                     };
  m_factories["STIL_Emulation"]      = [](const string& nbDerivations)    { return make_shared<STIL_EmulationProtocol>(nbDerivations); };
  m_factories["I2C_Emulation"]       = [](const string& parameters)       { return make_shared<I2C_EmulationProtocol>(parameters);     };
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
