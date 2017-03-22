//===========================================================================
//                           UT_PathSelectorFactory.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_PathSelectorFactory.cpp
//!
//! Implements test fixture for testing PathSelectorFactory
//!
//===========================================================================

#include "UT_PathSelectorFactory.hpp"

#include "PathSelectorFactory.hpp"
#include "PathSelector.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "DefaultNHotPathSelector.hpp"
#include "DefaultOneHotPathSelector.hpp"
#include "Register.hpp"

#include <typeinfo>
#include <tuple>
#include <memory>

#include "Cpp_11_Traits.hpp"
#include "Mast_Core_Traits.hpp"

using std::tuple;
using std::make_tuple;
using std::shared_ptr;
using std::make_shared;
using std::make_unique;
using std::string;

using namespace mast;


//! Checks PathSelectorFactory::Instance()
//!
void UT_PathSelectorFactory::test_Instance ()
{
  // ---------------- Setup
  //
  PathSelectorFactory* pInstance = nullptr;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (
                              pInstance = &PathSelectorFactory::Instance();
                           );

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_NOT_NULLPTR (pInstance);

  TS_ASSERT_EQUALS (pInstance->RegisteredCreatorsCount(), 6u);   // This is to detect when one should consider adding a new test for a new default creation method
}




//! Checks PathSelectorFactory::Create() for "standard" PathSelector when must be successful
//!
void UT_PathSelectorFactory::test_Create_Std_Success ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto        creatorId            = std::get<0>(data);
    auto        pathsCount           = std::get<1>(data);
    auto        properties           = std::get<2>(data);
    const auto& expectedSelectorType = std::get<3>(data);
    auto        associatedRegister   = make_shared<Register>("Ctrl", BinaryVector::CreateFromBinaryString("0b0000"), true);

    auto& instance = PathSelectorFactory::Instance();

    shared_ptr<PathSelector> pathSelector;

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (pathSelector = instance.Create(creatorId, pathsCount, properties, associatedRegister));

    // ---------------- Verify
    //
    TS_ASSERT_NOT_NULLPTR (pathSelector);

    const auto& actualSelectorType = typeid(*pathSelector);

    TS_ASSERT_EQUALS (actualSelectorType, expectedSelectorType);

    TS_ASSERT_EQUALS (pathSelector->SelectablePaths(), pathsCount);
    TS_ASSERT_EQUALS (pathSelector->Properties(),      properties);
    TS_ASSERT_FALSE  (pathSelector->CanSelectNone());
  };

  auto data =
  {
    make_tuple("Binary_noidle",  2u, SelectorProperty::None,         std::cref(typeid(DefaultBinaryPathSelector))), // 01
    make_tuple("Binary_noidle",  2u, SelectorProperty::InvertedBits, std::cref(typeid(DefaultBinaryPathSelector))), // 02
    make_tuple("One_Hot_noidle", 3u, SelectorProperty::None,         std::cref(typeid(DefaultOneHotPathSelector))), // 03
    make_tuple("One_Hot_noidle", 3u, SelectorProperty::ReverseOrder, std::cref(typeid(DefaultOneHotPathSelector))), // 04
    make_tuple("N_Hot_noidle",   4u, SelectorProperty::None,         std::cref(typeid(DefaultNHotPathSelector))),   // 05
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks PathSelectorFactory::Create() for "standard" PathSelector when must be successful and selectors can select no path
//!
void UT_PathSelectorFactory::test_Create_Std_Success_CanSelectNone ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto        creatorId            = std::get<0>(data);
    auto        pathsCount           = std::get<1>(data);
    auto        properties           = std::get<2>(data);
    const auto& expectedSelectorType = std::get<3>(data);
    auto        associatedRegister   = make_shared<Register>("Ctrl", BinaryVector::CreateFromBinaryString("0b000000"), true);

    auto& instance = PathSelectorFactory::Instance();

    shared_ptr<PathSelector> pathSelector;

    CxxTest::setAbortTestOnFail(true);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (pathSelector = instance.Create(creatorId, pathsCount, properties, associatedRegister));

    // ---------------- Verify
    //
    TS_ASSERT_NOT_NULLPTR (pathSelector);

    const auto& actualSelectorType = typeid(*pathSelector);

    TS_ASSERT_EQUALS (actualSelectorType, expectedSelectorType);

    TS_ASSERT_EQUALS (pathSelector->SelectablePaths(), pathsCount);
    TS_ASSERT_EQUALS (pathSelector->Properties(),      properties | SelectorProperty::CanSelectNone);
    TS_ASSERT_TRUE   (pathSelector->CanSelectNone());
    TS_ASSERT_FALSE  (pathSelector->IsSelected(1));
    TS_ASSERT_FALSE  (pathSelector->IsSelected(2));
  };

  auto data =
  {
    make_tuple("Binary",  4u, SelectorProperty::None,         std::cref(typeid(DefaultBinaryPathSelector))), // 00
    make_tuple("Binary",  4u, SelectorProperty::ReverseOrder, std::cref(typeid(DefaultBinaryPathSelector))), // 00
    make_tuple("One_Hot", 5u, SelectorProperty::None,         std::cref(typeid(DefaultOneHotPathSelector))), // 01
    make_tuple("N_Hot",   6u, SelectorProperty::None,         std::cref(typeid(DefaultNHotPathSelector))),   // 02
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks PathSelectorFactory::Create() for "standard" PathSelector when must name does not match a registered creation function
//!
void UT_PathSelectorFactory::test_Create_Std_Id_Error ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto     creatorId          = std::get<0>(data);
    uint32_t pathsCount         = 4u;
    auto     properties         = SelectorProperty::None;
    auto     associatedRegister = make_shared<Register>("Ctrl", BinaryVector::CreateFromBinaryString("0b000"), true);

    auto& instance = PathSelectorFactory::Instance();

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (auto pathSelector = instance.Create(creatorId, pathsCount, properties, associatedRegister), std::exception);
  };

  auto data =
  {
    make_tuple("Binary_NoIdle"), // 00: Wrong casing
    make_tuple("Foo"),           // 01: Unregistered creator name
    make_tuple("Last"),          // 02: Unregistered creator name (start of name is not sufficient)
    make_tuple(""),              // 03: Missing creator identifier
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks PathSelectorFactory::Create() for "standard" PathSelector when there is an issue with parameters
//!
void UT_PathSelectorFactory::test_Create_Std_Param_Error ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto creatorId          = std::get<0>(data);
    auto pathsCount         = std::get<1>(data);
    auto associatedRegister = std::get<2>(data);

    auto& instance = PathSelectorFactory::Instance();

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (auto pathSelector = instance.Create(creatorId, pathsCount, SelectorProperty::None, associatedRegister), std::exception);
  };

  auto invalidRegister = shared_ptr<Register>();
  auto data =
  {
    make_tuple("Binary",        9u, make_shared<Register>("Ctrl", BinaryVector::CreateFromBinaryString("0b00"), true)), // 00: too much derivations
    make_tuple("One_Hot",       4u, make_shared<Register>("Ctrl", BinaryVector::CreateFromBinaryString("0b00"), true)), // 01: too much derivations
    make_tuple("Binary_noidle", 2u, invalidRegister),                                                            // 02: nullptr associated register
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}

//! Checks PathSelectorFactory::Clear()
//!
void UT_PathSelectorFactory::test_Clear ()
{
  // ---------------- Setup
  //
  auto& sut = PathSelectorFactory::Instance();

  // ---------------- Exercise
  //
  sut.Clear();

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.RegisteredCreatorsCount(), 0u);
}


//! Checks PathSelectorFactory::InitializeWithDefaults() when there are none before
//!
void UT_PathSelectorFactory::test_InitializeWithDefaults ()
{
  // ---------------- Setup
  //
  auto& sut = PathSelectorFactory::Instance();
  sut.Clear();  // Remove default factories added at construction

  // ---------------- Exercise
  //
  sut.InitializeWithDefaults();

  // ---------------- Verify
  //
  TS_ASSERT_GREATER_THAN_EQUALS (sut.RegisteredCreatorsCount(), 3u);
}



//! Checks PathSelectorFactory::RegisterCreator() when adding a new one on top of default one
//!
void UT_PathSelectorFactory::test_RegisterCreator_NewOne_when_Defaults ()
{
  // ---------------- Setup
  //
  auto& sut         = PathSelectorFactory::Instance();
  auto  nbFactories = sut.RegisteredCreatorsCount();

  auto newFactory = [](uint32_t pathsCount, SelectorProperty properties, shared_ptr<Register> associatedRegister)
                    { return make_unique<DefaultBinaryPathSelector>(associatedRegister, pathsCount, properties); };

  // ---------------- Exercise
  //
  sut.RegisterCreator("Foo", newFactory);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  auto newNbFactories = sut.RegisteredCreatorsCount();
  TS_ASSERT_EQUALS (newNbFactories, nbFactories + 1u);

  uint32_t pathsCount         = 4u;
  auto     properties         = SelectorProperty::None;
  auto     associatedRegister = make_shared<Register>("Ctrl", BinaryVector::CreateFromBinaryString("0b000"), true);

  auto selector = sut.Create("Foo", pathsCount, properties, associatedRegister);
  TS_ASSERT_NOT_NULLPTR (selector);

  const auto&       actualSelectorType = typeid(*selector);
  TS_ASSERT_EQUALS (actualSelectorType,  typeid(DefaultBinaryPathSelector));

  auto asDefaultBinaryPathSelector = std::dynamic_pointer_cast<DefaultBinaryPathSelector>(shared_ptr<PathSelector>(std::move(selector)));
  TS_ASSERT_NOT_NULLPTR (asDefaultBinaryPathSelector);
}


//! Checks PathSelectorFactory::RegisterCreator() when adding a new one when none are registeres
//!
void UT_PathSelectorFactory::test_RegisterCreator_NewOne_when_None ()
{
  // ---------------- Setup
  //
  auto& sut         = PathSelectorFactory::Instance();
  auto  newFactory  = [](uint32_t pathsCount, SelectorProperty properties, shared_ptr<Register> associatedRegister)
                      { return make_unique<DefaultBinaryPathSelector>(associatedRegister, pathsCount, properties); };

  sut.Clear();

  // ---------------- Exercise
  //
  sut.RegisterCreator("Foo", newFactory);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  auto newNbFactories = sut.RegisteredCreatorsCount();
  TS_ASSERT_EQUALS (newNbFactories, 1u);

  uint32_t pathsCount         = 4u;
  auto     properties         = SelectorProperty::None;
  auto     associatedRegister = make_shared<Register>("Ctrl", BinaryVector::CreateFromBinaryString("0b000"), true);

  auto selector = sut.Create("Foo", pathsCount, properties, associatedRegister);
  TS_ASSERT_NOT_NULLPTR (selector);

  const auto&       actualSelectorType = typeid(*selector);
  TS_ASSERT_EQUALS (actualSelectorType,  typeid(DefaultBinaryPathSelector));

  auto asDefaultBinaryPathSelector = std::dynamic_pointer_cast<DefaultBinaryPathSelector>(shared_ptr<PathSelector>(std::move(selector)));
  TS_ASSERT_NOT_NULLPTR (asDefaultBinaryPathSelector);
}

//! Checks PathSelectorFactory::RegisterCreator() when replacing a default one
//!
void UT_PathSelectorFactory::test_RegisterCreator_Replace_Default ()
{
  // ---------------- Setup
  //
  auto& sut = PathSelectorFactory::Instance();
  sut.Clear();  // Remove default factories added at construction
  sut.InitializeWithDefaults();

  auto  nbFactories = sut.RegisteredCreatorsCount();
  auto  newFactory  = [](uint32_t pathsCount, SelectorProperty properties, shared_ptr<Register> associatedRegister)
                      { return make_unique<DefaultBinaryPathSelector>(associatedRegister, pathsCount, properties); };

  // ---------------- Exercise
  //
  sut.RegisterCreator("Binary", newFactory);

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  auto newNbFactories = sut.RegisteredCreatorsCount();
  TS_ASSERT_EQUALS (newNbFactories, nbFactories);

  uint32_t pathsCount         = 4u;
  auto     properties         = SelectorProperty::None;
  auto     associatedRegister = make_shared<Register>("Ctrl", BinaryVector::CreateFromBinaryString("0b000"), true);

  auto selector = sut.Create("Binary", pathsCount, properties, associatedRegister);
  TS_ASSERT_NOT_NULLPTR (selector);

  const auto&       actualSelectorType = typeid(*selector);
  TS_ASSERT_EQUALS (actualSelectorType,  typeid(DefaultBinaryPathSelector));

  auto asDefaultBinaryPathSelector = std::dynamic_pointer_cast<DefaultBinaryPathSelector>(shared_ptr<PathSelector>(std::move(selector)));
  TS_ASSERT_NOT_NULLPTR (asDefaultBinaryPathSelector);
}

//===========================================================================
// End of UT_PathSelectorFactory.cpp
//===========================================================================
