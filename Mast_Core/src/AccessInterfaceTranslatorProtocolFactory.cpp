//===========================================================================
//                           AccessInterfaceTranslatorProtocolFactory.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file AccessInterfaceTranslatorProtocolFactory.cpp
//!
//! Implements class AccessInterfaceTranslatorProtocolFactory
//!
//===========================================================================

#include "AccessInterfaceTranslatorProtocolFactory.hpp"
#include "Utility.hpp"

// For default AccessInterfaceTranslatorProtocol
#include "Emulation_TranslatorProtocol.hpp"
#include "JTAG_to_I2C_TranslatorProtocol.hpp"
#include "Dummy_TranslatorProtocol.hpp"

#include <memory>

#include "AccessInterfaceTranslatorProtocolFactory.hpp" //dummy to force compilation

using std::unique_ptr;
using std::make_unique;
using std::string;
using std::experimental::string_view;

using namespace std::experimental::literals::string_view_literals;
using namespace mast;

using mast::AccessInterfaceTranslatorProtocolFactory;

//! Called once to create single instance
//!
//! @note This is extracted from Instance() to separated initialization of defaults and request for instance
std::unique_ptr<AccessInterfaceTranslatorProtocolFactory> AccessInterfaceTranslatorProtocolFactory::CreateInstanceWithDefaultFactories ()
{
  auto instance = unique_ptr<AccessInterfaceTranslatorProtocolFactory>(new AccessInterfaceTranslatorProtocolFactory());
  instance->InitializeWithDefaults();
  return instance;
}
//
//  End of: AccessInterfaceTranslatorProtocolFactory::CreateInstanceWithDefaultFactories
//---------------------------------------------------------------------------



//! Creates an AccessInterfaceTranslatorProtocol using ceation function indentified by a name and optional parameters
//!
unique_ptr<AccessInterfaceTranslatorProtocol> AccessInterfaceTranslatorProtocolFactory::Create (const std::string& creatorId, const std::string& parameters) const
{
  auto instance = CreateImpl(creatorId, parameters);
  if (!instance)
  {
    THROW_INVALID_ARGUMENT("There is no creation method registered with name: "sv + creatorId);
  }

  return instance;
}
//
//  End of: AccessInterfaceTranslatorProtocolFactory::Create
//---------------------------------------------------------------------------




//! Fills up with default AccessInterfaceTranslatorProtocol
//!
void AccessInterfaceTranslatorProtocolFactory::InitializeWithDefaults ()
{
  RegisterCreator("Emulation",       [](const string& /* parameters */) { return make_unique<Emulation_TranslatorProtocol>();     });
  RegisterCreator("JTAG_to_I2C",            [](const string& parameters)       { return make_unique<JTAG_to_I2C_TranslatorProtocol>(parameters);     });
  RegisterCreator("Dummy",       [](const string&  parameters ) { return make_unique<Dummy_TranslatorProtocol>(parameters);     });
}
//
//  End of: AccessInterfaceTranslatorProtocolFactory::InitializeWithDefaults
//---------------------------------------------------------------------------


//! Returns sole instance
//!
//! @note It is thread safe
AccessInterfaceTranslatorProtocolFactory& AccessInterfaceTranslatorProtocolFactory::Instance ()
{
  static auto instance = CreateInstanceWithDefaultFactories();

  return *instance.get();
}
//
//  End of: AccessInterfaceTranslatorProtocolFactory::Instance
//---------------------------------------------------------------------------

//===========================================================================
// End of AccessInterfaceTranslatorProtocolFactory.cpp
//===========================================================================
