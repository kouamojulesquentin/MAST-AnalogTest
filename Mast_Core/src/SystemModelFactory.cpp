//===========================================================================
//                           SystemModelFactory.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file SystemModelFactory.cpp
//!
//! Implements class SystemModelFactory
//!
//===========================================================================

#include "SystemModelFactory.hpp"
#include "SystemModel.hpp"
#include "ParentNode.hpp"
#include "Utility.hpp"

using std::shared_ptr;
using std::unique_ptr;
using std::make_unique;
using std::string;
using std::experimental::string_view;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;

using namespace mast;



//! Called once to create single instance
//!
//! @note This is extracted from Instance() to separate initialization
//!       of defaults and request for instance
unique_ptr<SystemModelFactory> SystemModelFactory::CreateFactory ()
{
  auto instance = unique_ptr<SystemModelFactory>(new SystemModelFactory());
  instance->InitializeWithDefaults();
  return instance;
}
//
//  End of: SystemModelFactory::CreateFactoryWithDefaultCreators
//---------------------------------------------------------------------------



//! Must not be called. Use version that takes a SystemModel& instead!
//!
shared_ptr<ParentNode> SystemModelFactory::Create (const std::string& /* creatorId */, const std::string& /* parameters */) const
{
  CHECK_FAILED("Use Create version that takes a SystemModel& !");
}
//
//  End of: SystemModelFactory::Create
//---------------------------------------------------------------------------



//! Creates an ParentNode using creation function identified by a name and optional parameters
//!
//! @param creatorId    Name that identifies the MAST system model creator function
//! @param systemModel  System model that manages creation of SystemModelNode
//! @param parameters   Parameters that can, optionally, be used by creator function
//!
shared_ptr<ParentNode> SystemModelFactory::Create (const string& creatorId, SystemModel& systemModel, const string& parameters) const
{
  auto instance = CreateImpl(creatorId, systemModel, parameters);
  if (!instance)
  {
    THROW_INVALID_ARGUMENT("There is no model creation method registered with name: "sv + creatorId);
  }

  return instance;
}
//
//  End of: SystemModelFactory::Create
//---------------------------------------------------------------------------


//! Fills up with default SystemModel creator functions
//!
//! @note There is no default creator yet
//!
void SystemModelFactory::InitializeWithDefaults ()
{
//+  RegisterCreator("", [](const string& /* parameters */) { return   });
}
//
//  End of: SystemModelFactory::InitializeWithDefaults
//---------------------------------------------------------------------------


//! Returns sole instance
//!
//! @note It is thread safe
SystemModelFactory& SystemModelFactory::Instance ()
{
  static auto instance = CreateFactory();

  return *instance.get();
}
//
//  End of: SystemModelFactory::Instance
//---------------------------------------------------------------------------



//===========================================================================
// End of SystemModelFactory.cpp
//===========================================================================
