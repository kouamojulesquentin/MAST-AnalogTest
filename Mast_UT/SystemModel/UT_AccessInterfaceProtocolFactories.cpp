//===========================================================================
//                           UT_AccessInterfaceProtocolFactories.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_AccessInterfaceProtocolFactories.cpp
//!
//! Implements test fixture for testing AccessInterfaceProtocolFactories
//!
//===========================================================================


#include "UT_AccessInterfaceProtocolFactories.hpp"

#include "AccessInterfaceProtocolFactories.hpp"
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
using std::make_shared;
using std::string;

using namespace mast;


//! Checks AccessInterfaceProtocolFactories::Instance()
//!
void UT_AccessInterfaceProtocolFactories::test_Instance ()
{
  // ---------------- Setup
  //
  AccessInterfaceProtocolFactories* pInstance = nullptr;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (
                              pInstance = &AccessInterfaceProtocolFactories::Instance();
                           );

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_NOT_NULLPTR (pInstance);

  TS_ASSERT_EQUALS (pInstance->RegisteredFactoriesCount(), 6u);   // This is to detect when a test must be added for a new default factory
}



//! Checks AccessInterfaceProtocolFactories::CreateProtocol() when must be successful
//!
void UT_AccessInterfaceProtocolFactories::test_CreateProtocol_Success ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto  factoryName          = std::get<0>(data);
    auto  factoryParameters    = std::get<1>(data);
    auto& expectedProtocolType = std::get<2>(data);

    auto& instance = AccessInterfaceProtocolFactories::Instance();

    shared_ptr<AccessInterfaceProtocol> protocol;

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (protocol = instance.CreateProtocol(factoryName, factoryParameters));

    // ---------------- Verify
    //
    TS_ASSERT_NOT_NULLPTR (protocol);

    const auto& actualProtocolType = typeid(*protocol);

    TS_ASSERT_EQUALS (actualProtocolType, expectedProtocolType);
  };

  auto data =
  {
    make_tuple("JTAG_Loopback",       "",                  std::cref(typeid(LoopbackAccessInterfaceProtocol))), // 00
    make_tuple("JTAG_Loopback",       "Hello",             std::cref(typeid(LoopbackAccessInterfaceProtocol))), // 01 Hello is ignored
    make_tuple("JTAG_SVF_Simulation", "",                  std::cref(typeid(SVF_SimulationProtocol))),          // 02
    make_tuple("JTAG_SVF_Emulation",  "",                  std::cref(typeid(SVF_EmulationProtocol))),           // 03
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


//! Checks AccessInterfaceProtocolFactories::CreateProtocol() when must detect an error
//!
void UT_AccessInterfaceProtocolFactories::test_CreateProtocol_Error ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto factoryName       = std::get<0>(data);
    auto factoryParameters = std::get<1>(data);

    auto& instance = AccessInterfaceProtocolFactories::Instance();

    shared_ptr<AccessInterfaceProtocol> protocol;

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (protocol = instance.CreateProtocol(factoryName, factoryParameters), std::exception);
  };

  auto data =
  {
    make_tuple("JTAG_LoopbacK",  ""),                             // 00: Wrong casing
    make_tuple("Foo",            ""),                             // 01: Unregistered factory name
    make_tuple("STIL",           ""),                             // 02: Unregistered factory name (start of name is not sufficient)
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


//! Checks AccessInterfaceProtocolFactories::Clear()
//!
void UT_AccessInterfaceProtocolFactories::test_Clear ()
{
  // ---------------- Setup
  //
  auto& sut = AccessInterfaceProtocolFactories::Instance();

  // ---------------- Exercise
  //
  sut.Clear();

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.RegisteredFactoriesCount(), 0u);
}


//! Checks AccessInterfaceProtocolFactories::InitializeWithDefaults() when there are none before
//!
void UT_AccessInterfaceProtocolFactories::test_InitializeWithDefaults ()
{
  // ---------------- Setup
  //
  auto& sut = AccessInterfaceProtocolFactories::Instance();
  sut.Clear();  // Remove default factories added at construction

  // ---------------- Exercise
  //
  sut.InitializeWithDefaults();

  // ---------------- Verify
  //
  TS_ASSERT_GREATER_THAN_EQUALS (sut.RegisteredFactoriesCount(), 6u);
}



//! Checks AccessInterfaceProtocolFactories::AddFactory() when adding a new one on top of default one
//!
void UT_AccessInterfaceProtocolFactories::test_AddFactory_NewOne_when_Defaults ()
{
  // ---------------- Setup
  //
  auto& sut         = AccessInterfaceProtocolFactories::Instance();
  auto  nbFactories = sut.RegisteredFactoriesCount();

  auto  newFactory  = [](const string& nbDerivations) { return make_shared<STIL_EmulationProtocol>(nbDerivations); };

  // ---------------- Exercise
  //
  sut.AddFactory("Foo", newFactory);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  auto newNbFactories = sut.RegisteredFactoriesCount();
  TS_ASSERT_EQUALS (newNbFactories, nbFactories + 1u);

  auto protocol = sut.CreateProtocol("Foo", "12");
  TS_ASSERT_NOT_NULLPTR (protocol);

  const auto&       actualProtocolType = typeid(*protocol);
  TS_ASSERT_EQUALS (actualProtocolType,  typeid(STIL_EmulationProtocol));

  auto asSTILL_EmulationProtocol = std::dynamic_pointer_cast<STIL_EmulationProtocol>(protocol);
  TS_ASSERT_NOT_NULLPTR (asSTILL_EmulationProtocol);

  TS_ASSERT_EQUALS (asSTILL_EmulationProtocol->MaxSupportedDerivations(), 12u);
}


//! Checks AccessInterfaceProtocolFactories::AddFactory() when adding a new one when none are registeres
//!
void UT_AccessInterfaceProtocolFactories::test_AddFactory_NewOne_when_None ()
{
  // ---------------- Setup
  //
  auto& sut         = AccessInterfaceProtocolFactories::Instance();
  auto  newFactory  = [](const string& nbDerivations) { return make_shared<STIL_EmulationProtocol>(nbDerivations); };

  sut.Clear();

  // ---------------- Exercise
  //
  sut.AddFactory("Foo", newFactory);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  auto newNbFactories = sut.RegisteredFactoriesCount();
  TS_ASSERT_EQUALS (newNbFactories, 1u);

  auto protocol = sut.CreateProtocol("Foo", "13");
  TS_ASSERT_NOT_NULLPTR (protocol);

  const auto&       actualProtocolType = typeid(*protocol);
  TS_ASSERT_EQUALS (actualProtocolType,  typeid(STIL_EmulationProtocol));

  auto asSTILL_EmulationProtocol = std::dynamic_pointer_cast<STIL_EmulationProtocol>(protocol);
  TS_ASSERT_NOT_NULLPTR (asSTILL_EmulationProtocol);

  TS_ASSERT_EQUALS (asSTILL_EmulationProtocol->MaxSupportedDerivations(), 13u);
}

//! Checks AccessInterfaceProtocolFactories::AddFactory() when replacing a default one
//!
void UT_AccessInterfaceProtocolFactories::test_AddFactory_Replace_Default ()
{
  // ---------------- Setup
  //
  auto& sut = AccessInterfaceProtocolFactories::Instance();
  sut.Clear();  // Remove default factories added at construction
  sut.InitializeWithDefaults();

  auto  nbFactories = sut.RegisteredFactoriesCount();
  auto  newFactory  = [](const string& nbDerivations) { return make_shared<STIL_EmulationProtocol>(nbDerivations); };

  // ---------------- Exercise
  //
  sut.AddFactory("STIL_Emulation", newFactory);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  auto newNbFactories = sut.RegisteredFactoriesCount();
  TS_ASSERT_EQUALS (newNbFactories, nbFactories);

  auto protocol = sut.CreateProtocol("STIL_Emulation", "14");
  TS_ASSERT_NOT_NULLPTR (protocol);

  const auto&       actualProtocolType = typeid(*protocol);
  TS_ASSERT_EQUALS (actualProtocolType,  typeid(STIL_EmulationProtocol));

  auto asSTILL_EmulationProtocol = std::dynamic_pointer_cast<STIL_EmulationProtocol>(protocol);
  TS_ASSERT_NOT_NULLPTR (asSTILL_EmulationProtocol);

  TS_ASSERT_EQUALS (asSTILL_EmulationProtocol->MaxSupportedDerivations(), 14u);
}

//===========================================================================
// End of UT_AccessInterfaceProtocolFactories.cpp
//===========================================================================
