//===========================================================================
//                           UT_ConfigureAlgorithmFactory.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_ConfigureAlgorithmFactory.cpp
//!
//! Implements test fixture for testing ConfigureAlgorithmFactory
//!
//===========================================================================

#include "UT_ConfigureAlgorithmFactory.hpp"

#include "ConfigureAlgorithmFactory.hpp"
#include "ConfigurationAlgorithm.hpp"
#include "ConfigureAlgorithm_LastOrDefault.hpp"
#include "ConfigureAlgorithm_LastOrDefault_Greedy.hpp"
#include "ConfigureAlgorithm_Last_Lazy.hpp"

#include <typeinfo>
#include <tuple>
#include <memory>

#include "Cpp_11_Traits.hpp"

using std::tuple;
using std::make_tuple;
using std::shared_ptr;
using std::make_unique;
using std::string;

using namespace mast;


//! Checks ConfigureAlgorithmFactory::Instance()
//!
void UT_ConfigureAlgorithmFactory::test_Instance ()
{
  // ---------------- Setup
  //
  ConfigureAlgorithmFactory* pInstance = nullptr;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (
                              pInstance = &ConfigureAlgorithmFactory::Instance();
                           );

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_NOT_NULLPTR (pInstance);

  TS_ASSERT_EQUALS (pInstance->RegisteredCreatorsCount(), 3u);   // This is to detect when one should consider adding a new test for a new default creation method
}



//! Checks ConfigureAlgorithmFactory::Create() when must be successful
//!
void UT_ConfigureAlgorithmFactory::test_Create_Success ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto  creatorId            = std::get<0>(data);
    auto  creatorParameters    = std::get<1>(data);
    auto& expectedProtocolType = std::get<2>(data);

    auto& instance = ConfigureAlgorithmFactory::Instance();

    shared_ptr<ConfigurationAlgorithm> protocol;

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (protocol = instance.Create(creatorId, creatorParameters));

    // ---------------- Verify
    //
    TS_ASSERT_NOT_NULLPTR (protocol);

    const auto& actualProtocolType = typeid(*protocol);

    TS_ASSERT_EQUALS (actualProtocolType, expectedProtocolType);
  };

  auto data =
  {
    make_tuple("last_or_default",        "",      std::cref(typeid(ConfigureAlgorithm_LastOrDefault))),        // 00
    make_tuple("last_or_default_greedy", "",      std::cref(typeid(ConfigureAlgorithm_LastOrDefault_Greedy))), // 01
    make_tuple("last_lazy",              "",      std::cref(typeid(ConfigureAlgorithm_Last_Lazy))),            // 02
    make_tuple("last_lazy",              "Hello", std::cref(typeid(ConfigureAlgorithm_Last_Lazy))),            // 03 (Hello is ignored)
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks ConfigureAlgorithmFactory::Create() when must detect an error
//!
void UT_ConfigureAlgorithmFactory::test_Create_Error ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto creatorId         = std::get<0>(data);
    auto creatorParameters = std::get<1>(data);

    auto& instance = ConfigureAlgorithmFactory::Instance();

    shared_ptr<ConfigurationAlgorithm> protocol;

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (protocol = instance.Create(creatorId, creatorParameters), std::exception);
  };

  auto data =
  {
    make_tuple("lASToRdEFAULT", ""), // 00: Wrong casing
    make_tuple("Foo",           ""), // 01: Unregistered creator name
    make_tuple("Last",          ""), // 02: Unregistered creator name (start of name is not sufficient)
    make_tuple("",              ""), // 03: Missing creator identifier
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks ConfigureAlgorithmFactory::Clear()
//!
void UT_ConfigureAlgorithmFactory::test_Clear ()
{
  // ---------------- Setup
  //
  auto& sut = ConfigureAlgorithmFactory::Instance();

  // ---------------- Exercise
  //
  sut.Clear();

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.RegisteredCreatorsCount(), 0u);
}


//! Checks ConfigureAlgorithmFactory::InitializeWithDefaults() when there are none before
//!
void UT_ConfigureAlgorithmFactory::test_InitializeWithDefaults ()
{
  // ---------------- Setup
  //
  auto& sut = ConfigureAlgorithmFactory::Instance();
  sut.Clear();  // Remove default factories added at construction

  // ---------------- Exercise
  //
  sut.InitializeWithDefaults();

  // ---------------- Verify
  //
  TS_ASSERT_GREATER_THAN_EQUALS (sut.RegisteredCreatorsCount(), 3u);
}



//! Checks ConfigureAlgorithmFactory::RegisterCreator() when adding a new one on top of default one
//!
void UT_ConfigureAlgorithmFactory::test_RegisterCreator_NewOne_when_Defaults ()
{
  // ---------------- Setup
  //
  auto& sut         = ConfigureAlgorithmFactory::Instance();
  auto  nbFactories = sut.RegisteredCreatorsCount();

  auto  newFactory  = [](const string& /* unused */) { return make_unique<ConfigureAlgorithm_Last_Lazy>(); };

  // ---------------- Exercise
  //
  sut.RegisterCreator("Foo", newFactory);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  auto newNbFactories = sut.RegisteredCreatorsCount();
  TS_ASSERT_EQUALS (newNbFactories, nbFactories + 1u);

  auto configureAlgo = shared_ptr<ConfigurationAlgorithm>(std::move(sut.Create("Foo", "")));
  TS_ASSERT_NOT_NULLPTR (configureAlgo);

  const auto&       actualProtocolType = typeid(*configureAlgo);
  TS_ASSERT_EQUALS (actualProtocolType,  typeid(ConfigureAlgorithm_Last_Lazy));

  auto asConfigureAlgorithm_Last_Lazy = std::dynamic_pointer_cast<ConfigureAlgorithm_Last_Lazy>(configureAlgo);
  TS_ASSERT_NOT_NULLPTR (asConfigureAlgorithm_Last_Lazy);
}


//! Checks ConfigureAlgorithmFactory::RegisterCreator() when adding a new one when none are registeres
//!
void UT_ConfigureAlgorithmFactory::test_RegisterCreator_NewOne_when_None ()
{
  // ---------------- Setup
  //
  auto& sut         = ConfigureAlgorithmFactory::Instance();
  auto  newFactory  = [](const string& /* unused */) { return make_unique<ConfigureAlgorithm_Last_Lazy>(); };

  sut.Clear();

  // ---------------- Exercise
  //
  sut.RegisterCreator("Foo", newFactory);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  auto newNbFactories = sut.RegisteredCreatorsCount();
  TS_ASSERT_EQUALS (newNbFactories, 1u);

  auto configureAlgo = shared_ptr<ConfigurationAlgorithm>(std::move(sut.Create("Foo", "")));
  TS_ASSERT_NOT_NULLPTR (configureAlgo);

  const auto&       actualProtocolType = typeid(*configureAlgo);
  TS_ASSERT_EQUALS (actualProtocolType,  typeid(ConfigureAlgorithm_Last_Lazy));

  auto asConfigureAlgorithm_Last_Lazy = std::dynamic_pointer_cast<ConfigureAlgorithm_Last_Lazy>(configureAlgo);
  TS_ASSERT_NOT_NULLPTR (asConfigureAlgorithm_Last_Lazy);
}

//! Checks ConfigureAlgorithmFactory::RegisterCreator() when replacing a default one
//!
void UT_ConfigureAlgorithmFactory::test_RegisterCreator_Replace_Default ()
{
  // ---------------- Setup
  //
  auto& sut = ConfigureAlgorithmFactory::Instance();
  sut.Clear();  // Remove default factories added at construction
  sut.InitializeWithDefaults();

  auto  nbFactories = sut.RegisteredCreatorsCount();
  auto  newFactory  = [](const string& /* unused */) { return make_unique<ConfigureAlgorithm_Last_Lazy>(); };

  // ---------------- Exercise
  //
  sut.RegisterCreator("last_lazy", newFactory);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  auto newNbFactories = sut.RegisteredCreatorsCount();
  TS_ASSERT_EQUALS (newNbFactories, nbFactories);

  auto configureAlgo = sut.Create("last_lazy", "");
  TS_ASSERT_NOT_NULLPTR (configureAlgo);

  const auto&       actualProtocolType = typeid(*configureAlgo);
  TS_ASSERT_EQUALS (actualProtocolType,  typeid(ConfigureAlgorithm_Last_Lazy));

  auto asConfigureAlgorithm_Last_Lazy = dynamic_cast<ConfigureAlgorithm_Last_Lazy*>(configureAlgo.get());
  TS_ASSERT_NOT_NULLPTR (asConfigureAlgorithm_Last_Lazy);
}

//===========================================================================
// End of UT_ConfigureAlgorithmFactory.cpp
//===========================================================================
