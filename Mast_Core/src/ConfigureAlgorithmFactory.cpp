//===========================================================================
//                           ConfigureAlgorithmFactory.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ConfigureAlgorithmFactory.cpp
//!
//! Implements class ConfigureAlgorithmFactory
//!
//===========================================================================

#include "ConfigureAlgorithmFactory.hpp"
#include "ConfigureAlgorithm_LastOrDefault.hpp"
#include "ConfigureAlgorithm_LastOrDefault_Greedy.hpp"
#include "ConfigureAlgorithm_Last_Lazy.hpp"
#include "Utility.hpp"

using std::unique_ptr;
using std::make_unique;
using std::string;
using std::experimental::string_view;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;

using mast::ConfigureAlgorithmFactory;
using mast::ConfigurationAlgorithm;


//! Called once to create single instance
//!
//! @note This is extracted from Instance() to separate initialization of defaults and request for instance
unique_ptr<ConfigureAlgorithmFactory> ConfigureAlgorithmFactory::CreateFactoryWithDefaultCreators ()
{
  auto instance = unique_ptr<ConfigureAlgorithmFactory>(new ConfigureAlgorithmFactory());
  instance->InitializeWithDefaults();
  return instance;
}
//
//  End of: ConfigureAlgorithmFactory::CreateFactoryWithDefaultCreators
//---------------------------------------------------------------------------



//! Creates an ConfigurationAlgorithm using ceation function indentified by a name and optional parameters
//!
unique_ptr<ConfigurationAlgorithm> ConfigureAlgorithmFactory::Create (const string& creatorId, const string& parameters) const
{
  auto instance = CreateImpl(creatorId, parameters);
  if (!instance)
  {
    THROW_INVALID_ARGUMENT("There is no creation method registered with name: "sv + creatorId);
  }

  return instance;
}
//
//  End of: ConfigureAlgorithmFactory::Create
//---------------------------------------------------------------------------



//! Fills up with default ConfigurationAlgorithm
//!
void ConfigureAlgorithmFactory::InitializeWithDefaults ()
{
  RegisterCreator("LastOrDefault",        [](const string& /* parameters */) { return make_unique<ConfigureAlgorithm_LastOrDefault>();        });
  RegisterCreator("LastOrDefault_Greedy", [](const string& /* parameters */) { return make_unique<ConfigureAlgorithm_LastOrDefault_Greedy>(); });
  RegisterCreator("Last_Lazy",            [](const string& /* parameters */) { return make_unique<ConfigureAlgorithm_Last_Lazy>();            });
}
//
//  End of: ConfigureAlgorithmFactory::InitializeWithDefaults
//---------------------------------------------------------------------------


//! Returns sole instance
//!
//! @note It is thread safe
ConfigureAlgorithmFactory& ConfigureAlgorithmFactory::Instance ()
{
  static auto instance = CreateFactoryWithDefaultCreators();

  return *instance.get();
}
//
//  End of: ConfigureAlgorithmFactory::Instance
//---------------------------------------------------------------------------



//===========================================================================
// End of ConfigureAlgorithmFactory.cpp
//===========================================================================
