//===========================================================================
//                           UT_AccessInterfaceProtocolFactory.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_AccessInterfaceProtocolFactory.cpp
//!
//! Implements test fixture for testing AccessInterfaceProtocolFactory
//!
//===========================================================================


#include "UT_AccessInterfaceProtocolFactory.hpp"

#include "AccessInterfaceProtocolFactory.hpp"
#include "AccessInterfaceProtocol.hpp"
#include "LoopbackAccessInterfaceProtocol.hpp"
#include "SVF_SimulationProtocol.hpp"
#include "SVF_EmulationProtocol.hpp"
#include "OfflineProtocol.hpp"
#include "STIL_EmulationProtocol.hpp"
#include "I2C_EmulationProtocol.hpp"

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


//! Checks AccessInterfaceProtocolFactory::Instance()
//!
void UT_AccessInterfaceProtocolFactory::test_Instance ()
{
  // ---------------- Setup
  //
  AccessInterfaceProtocolFactory* pInstance = nullptr;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (
                              pInstance = &AccessInterfaceProtocolFactory::Instance();
                           );

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_NOT_NULLPTR (pInstance);

  TS_ASSERT_EQUALS (pInstance->RegisteredCreatorsCount(), 9u);   // This is to detect when one should consider adding a new test for a new default creation method
}



//! Checks AccessInterfaceProtocolFactory::Create() when must be successful
//!
void UT_AccessInterfaceProtocolFactory::test_Create_Success ()
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

    auto& instance = AccessInterfaceProtocolFactory::Instance();

    shared_ptr<AccessInterfaceProtocol> protocol;

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
    make_tuple("Loopback",       "",                  std::cref(typeid(LoopbackAccessInterfaceProtocol))), // 00
    make_tuple("Loopback",       "Hello",             std::cref(typeid(LoopbackAccessInterfaceProtocol))), // 01 Hello is ignored
    make_tuple("SVF_Simulation", "",                  std::cref(typeid(SVF_SimulationProtocol))),          // 02
    make_tuple("SVF_Emulation",  "",                  std::cref(typeid(SVF_EmulationProtocol))),           // 03
    make_tuple("Offline",             "",                  std::cref(typeid(OfflineProtocol))),                 // 04
    make_tuple("STIL_Emulation",      "4",                 std::cref(typeid(STIL_EmulationProtocol))),          // 05
    make_tuple("STIL_Emulation",      "+5",                std::cref(typeid(STIL_EmulationProtocol))),          // 06
    make_tuple("STIL_Emulation",      "0xA",               std::cref(typeid(STIL_EmulationProtocol))),          // 07
    make_tuple("I2C_Emulation",       "0x32, 0x3333",      std::cref(typeid(I2C_EmulationProtocol))),           // 08
    make_tuple("I2C_Emulation",       "0x32, 0x33, [I2C]", std::cref(typeid(I2C_EmulationProtocol))),           // 09
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AccessInterfaceProtocolFactory::Create() when must detect an error
//!
void UT_AccessInterfaceProtocolFactory::test_Create_Error ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto creatorId         = std::get<0>(data);
    auto creatorParameters = std::get<1>(data);

    auto& instance = AccessInterfaceProtocolFactory::Instance();

    shared_ptr<AccessInterfaceProtocol> protocol;

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (protocol = instance.Create(creatorId, creatorParameters), std::exception);
  };

  auto data =
  {
    make_tuple("LoopbacK",  ""),                             // 00: Wrong casing
    make_tuple("Foo",            ""),                             // 01: Unregistered creator name
    make_tuple("STIL",           ""),                             // 02: Unregistered creator name (start of name is not sufficient)
    make_tuple("STIL_Emulation", ""),                             // 03: Missing parameter
    make_tuple("STIL_Emulation", "five"),                         // 04: Parameter is not a number
    make_tuple("STIL_Emulation", " "),                            // 05: Parameter is not a number
    make_tuple("I2C_Emulation",  "0x32, i2c"),                    // 06: Must have at least two addresses
    make_tuple("I2C_Emulation",  "0x32, i2c, 0x33"),              // 07: Prefix is not last parameter
    make_tuple("I2C_Emulation",  "0x32, 0x33, i2c, :Hello:"),     // 08: Prefix is not last parameter
    make_tuple("I2C_Emulation",  "0x32, 0xFFFFFFFF0, 0x34, i2c"), // 09: Too big number
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks AccessInterfaceProtocolFactory::Clear()
//!
void UT_AccessInterfaceProtocolFactory::test_Clear ()
{
  // ---------------- Setup
  //
  auto& sut = AccessInterfaceProtocolFactory::Instance();

  // ---------------- Exercise
  //
  sut.Clear();

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.RegisteredCreatorsCount(), 0u);
}


//! Checks AccessInterfaceProtocolFactory::InitializeWithDefaults() when there are none before
//!
void UT_AccessInterfaceProtocolFactory::test_InitializeWithDefaults ()
{
  // ---------------- Setup
  //
  auto& sut = AccessInterfaceProtocolFactory::Instance();
  sut.Clear();  // Remove default factories added at construction

  // ---------------- Exercise
  //
  sut.InitializeWithDefaults();

  // ---------------- Verify
  //
  TS_ASSERT_GREATER_THAN_EQUALS (sut.RegisteredCreatorsCount(), 6u);
}



//! Checks AccessInterfaceProtocolFactory::RegisterCreator() when adding a new one on top of default one
//!
void UT_AccessInterfaceProtocolFactory::test_RegisterCreator_NewOne_when_Defaults ()
{
  // ---------------- Setup
  //
  auto& sut         = AccessInterfaceProtocolFactory::Instance();
  auto  nbFactories = sut.RegisteredCreatorsCount();

  auto  newFactory  = [](const string& nbChannels) { return make_unique<STIL_EmulationProtocol>(nbChannels); };

  // ---------------- Exercise
  //
  sut.RegisterCreator("Foo", newFactory);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  auto newNbFactories = sut.RegisteredCreatorsCount();
  TS_ASSERT_EQUALS (newNbFactories, nbFactories + 1u);

  auto protocol = shared_ptr<AccessInterfaceProtocol>(std::move(sut.Create("Foo", "12")));
  TS_ASSERT_NOT_NULLPTR (protocol);

  const auto&       actualProtocolType = typeid(*protocol);
  TS_ASSERT_EQUALS (actualProtocolType,  typeid(STIL_EmulationProtocol));

  auto asSTILL_EmulationProtocol = std::dynamic_pointer_cast<STIL_EmulationProtocol>(protocol);
  TS_ASSERT_NOT_NULLPTR (asSTILL_EmulationProtocol);

  TS_ASSERT_EQUALS (asSTILL_EmulationProtocol->MaxSupportedChannels(), 12u);
}


//! Checks AccessInterfaceProtocolFactory::RegisterCreator() when adding a new one when none are registeres
//!
void UT_AccessInterfaceProtocolFactory::test_RegisterCreator_NewOne_when_None ()
{
  // ---------------- Setup
  //
  auto& sut         = AccessInterfaceProtocolFactory::Instance();
  auto  newFactory  = [](const string& nbChannels) { return make_unique<STIL_EmulationProtocol>(nbChannels); };

  sut.Clear();

  // ---------------- Exercise
  //
  sut.RegisterCreator("Foo", newFactory);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  auto newNbFactories = sut.RegisteredCreatorsCount();
  TS_ASSERT_EQUALS (newNbFactories, 1u);

  auto protocol = shared_ptr<AccessInterfaceProtocol>(std::move(sut.Create("Foo", "13")));
  TS_ASSERT_NOT_NULLPTR (protocol);

  const auto&       actualProtocolType = typeid(*protocol);
  TS_ASSERT_EQUALS (actualProtocolType,  typeid(STIL_EmulationProtocol));

  auto asSTILL_EmulationProtocol = std::dynamic_pointer_cast<STIL_EmulationProtocol>(protocol);
  TS_ASSERT_NOT_NULLPTR (asSTILL_EmulationProtocol);

  TS_ASSERT_EQUALS (asSTILL_EmulationProtocol->MaxSupportedChannels(), 13u);
}

//! Checks AccessInterfaceProtocolFactory::RegisterCreator() when replacing a default one
//!
void UT_AccessInterfaceProtocolFactory::test_RegisterCreator_Replace_Default ()
{
  // ---------------- Setup
  //
  auto& sut = AccessInterfaceProtocolFactory::Instance();
  sut.Clear();  // Remove default factories added at construction
  sut.InitializeWithDefaults();

  auto  nbFactories = sut.RegisteredCreatorsCount();
  auto  newFactory  = [](const string& nbChannels) { return make_unique<STIL_EmulationProtocol>(nbChannels); };

  // ---------------- Exercise
  //
  sut.RegisterCreator("STIL_Emulation", newFactory);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  auto newNbFactories = sut.RegisteredCreatorsCount();
  TS_ASSERT_EQUALS (newNbFactories, nbFactories);

  auto protocol = sut.Create("STIL_Emulation", "14");
  TS_ASSERT_NOT_NULLPTR (protocol);

  const auto&       actualProtocolType = typeid(*protocol);
  TS_ASSERT_EQUALS (actualProtocolType,  typeid(STIL_EmulationProtocol));

  auto asSTILL_EmulationProtocol = dynamic_cast<STIL_EmulationProtocol*>(protocol.get());
  TS_ASSERT_NOT_NULLPTR (asSTILL_EmulationProtocol);

  TS_ASSERT_EQUALS (asSTILL_EmulationProtocol->MaxSupportedChannels(), 14u);
}

//===========================================================================
// End of UT_AccessInterfaceProtocolFactory.cpp
//===========================================================================
