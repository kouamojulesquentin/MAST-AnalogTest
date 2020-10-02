//===========================================================================
//                           PathSelectorFactory.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file PathSelectorFactory.cpp
//!
//! Implements class PathSelectorFactory
//!
//===========================================================================

#include "PathSelectorFactory.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "DefaultNHotPathSelector.hpp"
#include "DefaultOneHotPathSelector.hpp"
#include "EnumsUtility.hpp"
#include "Utility.hpp"

using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;
using std::string;
using std::experimental::string_view;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;

using mast::PathSelector;
using mast::DefaultBinaryPathSelector;
using mast::DefaultNHotPathSelector;
using mast::DefaultOneHotPathSelector;
using mast::PathSelectorFactory;


//! Constructor...
PathSelectorFactory::PathSelectorFactory ()
{
}
//
//  End of: PathSelectorFactory::PathSelectorFactory
//---------------------------------------------------------------------------


//! Removes any, registered, factory
//!
void PathSelectorFactory::Clear ()
{
  m_stdCreators.clear();
  m_customCreators.clear();
  m_fullCustomCreators.clear();
  m_HybridCustomCreators.clear();
  m_GenericCustomCreators.clear();
}
//
//  End of: PathSelectorFactory::Clear
//---------------------------------------------------------------------------




//! Called once to create single instance
//!
//! @note This is extracted from Instance() to separate initialization of defaults and request for instance
unique_ptr<PathSelectorFactory> PathSelectorFactory::CreateFactoryWithDefaultCreators ()
{
  auto instance = unique_ptr<PathSelectorFactory>(new PathSelectorFactory());
  instance->InitializeWithDefaults();
  return instance;
}
//
//  End of: PathSelectorFactory::CreateFactoryWithDefaultCreators
//---------------------------------------------------------------------------


//! Creates an PathSelector using registered creation function
//!
//! @param creatorId            A name that identified registered creation function
//! @param pathsCount           Number of managed paths (including, optional, bypass register)
//! @param properties           Properties of the selector (bit order can be reverse or it can use negative logic)
//! @param associatedRegister   Register that is used to drive the path multiplexer
//!
unique_ptr<PathSelector> PathSelectorFactory::Create (const string&        creatorId,
                                                      uint32_t             pathsCount,
                                                      SelectorProperty     properties,
                                                      shared_ptr<Register> associatedRegister) const
{
  auto instance = CreateImpl(m_stdCreators, creatorId, pathsCount, properties, associatedRegister);
  if (!instance)
  {
    THROW_INVALID_ARGUMENT("There is no \"Standard\" creation method registered with name: "sv + creatorId);
  }

  return instance;
}
//
//  End of: PathSelectorFactory::Create
//---------------------------------------------------------------------------


//! Creates an PathSelector using registered creation function
//!
//! @param creatorId    A name that identified registered creation function
//! @param pathsCount   Number of managed paths (including, optional, bypass register)
//! @param parameters   String of (optional) parameters
//!
unique_ptr<PathSelector> PathSelectorFactory::Create (const string& creatorId, uint32_t pathsCount, const string& parameters) const
{
  auto instance = CreateImpl(m_fullCustomCreators, creatorId, pathsCount, parameters);
  if (!instance)
  {
    THROW_INVALID_ARGUMENT("There is no \"Full Custom\" creation method registered with name: "sv + creatorId);
  }

  return instance;
}
//
//  End of: PathSelectorFactory::Create
//---------------------------------------------------------------------------


//! Creates an PathSelector using registered creation function
//!
//! @param creatorId           A name that identified registered creation function
//! @param pathsCount          Number of managed paths (including, optional, bypass register)
//! @param parameters          String of (optional) parameters
//! @param associatedRegister  Register that is used to drive the path multiplexer
//!
unique_ptr<PathSelector> PathSelectorFactory::Create (const string& creatorId, uint32_t pathsCount, const string& parameters, shared_ptr<Register> associatedRegister) const
{
  auto instance = CreateImpl(m_customCreators, creatorId, pathsCount, parameters, associatedRegister);
  if (!instance)
  {
    THROW_INVALID_ARGUMENT("There is no \"Custom\" creation method registered with name: "sv + creatorId);
  }

  return instance;
}
//
//  End of: PathSelectorFactory::Create
//---------------------------------------------------------------------------



//! Creates an PathSelector using registered creation function
//!
//! @param creatorId            A name that identified registered creation function
//! @param pathsCount           Number of managed paths (including, optional, bypass register)
//! @param properties           Properties of the selector (bit order can be reverse or it can use negative logic)
//! @param associatedRegisters  Registers that are used to drive the path multiplexer
//!
unique_ptr<PathSelector> PathSelectorFactory::Create (const std::string& creatorId, 
						      uint32_t pathsCount, 
						      const std::string& parameters, 
						      std::vector<std::shared_ptr<Register>> associatedRegisters
) const
{
  auto instance = CreateImpl(m_HybridCustomCreators, creatorId, pathsCount, parameters, associatedRegisters);
  if (!instance)
  {
    THROW_INVALID_ARGUMENT("There is no \"Hybrid\" creation method registered with name: "sv + creatorId);
  }

  return instance;
}
//
//  End of: PathSelectorFactory::Create
//---------------------------------------------------------------------------

//! Creates an PathSelector using registered creation function
//!
//! @param creatorId            A name that identified registered creation function
//! @param pathsCount           Number of managed paths (including, optional, bypass register)
//! @param properties           Properties of the selector (bit order can be reverse or it can use negative logic)
//! @param associatedRegisters  Registers that are used to drive the path multiplexer
//!
unique_ptr<PathSelector> PathSelectorFactory::Create (const std::string& creatorId, 
						      uint32_t pathsCount, 
						      const std::string& parameters, 
						      std::vector<std::shared_ptr<SystemModelNode>> associatedNodes
) const
{
  auto instance = CreateImpl(m_GenericCustomCreators, creatorId, pathsCount, parameters, associatedNodes);
  if (!instance)
  {
    THROW_INVALID_ARGUMENT("There is no \"Generic\" creation method registered with name: "sv + creatorId);
  }

  return instance;
}
//
//  End of: PathSelectorFactory::Create
//---------------------------------------------------------------------------

//! Fills up with default PathSelector
//!
//! @note By definition, there is no default "Full Custom" creators
//!
void PathSelectorFactory::InitializeWithDefaults ()
{
  // ---------------- Standard
  //
  #define MAKE_LAMBDA_STD(selectorClass, forcedProperty)                                               \
  [](uint32_t pathsCount, SelectorProperty selectorProperty, shared_ptr<Register> associatedRegister)  \
  { return make_unique<selectorClass>(associatedRegister, pathsCount, selectorProperty | forcedProperty); }

  m_stdCreators["Binary"]         = MAKE_LAMBDA_STD(DefaultBinaryPathSelector, SelectorProperty::CanSelectNone);
  m_stdCreators["Binary_noidle"]  = MAKE_LAMBDA_STD(DefaultBinaryPathSelector, SelectorProperty::None);
  m_stdCreators["One_Hot"]        = MAKE_LAMBDA_STD(DefaultOneHotPathSelector, SelectorProperty::CanSelectNone);
  m_stdCreators["One_Hot_noidle"] = MAKE_LAMBDA_STD(DefaultOneHotPathSelector, SelectorProperty::None);
  m_stdCreators["N_Hot"]          = MAKE_LAMBDA_STD(DefaultNHotPathSelector,   SelectorProperty::CanSelectNone);
  m_stdCreators["N_Hot_noidle"]   = MAKE_LAMBDA_STD(DefaultNHotPathSelector,   SelectorProperty::None);

  // ---------------- Custom
  //
  m_customCreators["Table_Based"] = [](uint32_t pathsCount, const string& parameters, shared_ptr<Register> associatedRegister)
                                    { return make_unique<DefaultTableBasedPathSelector>(associatedRegister, pathsCount, SelectorProperty::None, parameters); };
}
//
//  End of: PathSelectorFactory::InitializeWithDefaults
//---------------------------------------------------------------------------


//! Returns sole instance
//!
//! @note It is thread safe
PathSelectorFactory& PathSelectorFactory::Instance ()
{
  static auto instance = CreateFactoryWithDefaultCreators();

  return *instance.get();
}
//
//  End of: PathSelectorFactory::Instance
//---------------------------------------------------------------------------



//! Returns the number of factories currently registered (associated with a name)
//!
size_t PathSelectorFactory::RegisteredCreatorsCount () const
{
  return  m_stdCreators.size()
        + m_customCreators.size()
        + m_fullCustomCreators.size();
}
//
//  End of: PathSelectorFactory::RegisteredCreatorsCount
//---------------------------------------------------------------------------


//===========================================================================
// End of PathSelectorFactory.cpp
//===========================================================================
