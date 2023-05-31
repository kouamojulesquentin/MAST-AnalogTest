//===========================================================================
//                           UT_GenericAccessInterfaceProtocol.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_GenericAccessInterfaceProtocol.cpp
//!
//! Implements test fixture for testing GenericAccessInterfaceProtocol
//!
//===========================================================================

#include "UT_GenericAccessInterfaceProtocol.hpp"
#include "GenericAccessInterfaceProtocol.hpp"
#include "BinaryVector_Traits.hpp"

#include <cxxtest/ValueTraits.h>
#include <string>
#include <sstream>
#include <vector>

using std::string;
using std::ostringstream;
using std::initializer_list;
using std::vector;

using namespace std::string_literals;
using namespace mast;

using Primitive = GenericAccessInterfaceProtocol::Primitive;
using Action    = GenericAccessInterfaceProtocol::Action;

namespace
{
//! Creates a GenericAccessInterfaceProtocol suitable for following tests
//!
GenericAccessInterfaceProtocol CreateSut (string& loggedActions)
{
  struct PrimitiveParam
  {
    PrimitiveParam(const string& p_context, const BinaryVector& p_toSutData, BinaryVector& p_fromSutData)
      : context     (p_context)
      , toSutData   (p_toSutData)
      , fromSutData (p_fromSutData)
    {  }

    const string&       context;
    const BinaryVector& toSutData;
          BinaryVector& fromSutData;
  };

  auto primitive_0  = [&](void* param)
  {
    const auto& text = *reinterpret_cast<const string*>(param);
    loggedActions += "Primitive_0("s + text + ")"s ;
  };

  auto logPrimitive = [](uint32_t primitiveId, void* data)
  {
    const auto& param = *reinterpret_cast<PrimitiveParam*>(data);
    param.fromSutData = param.toSutData;

    ostringstream os;
    os << "Primitive_" << std::dec << primitiveId << "(" << param.context << ", " << param.toSutData.DataAsMixString() << ")";
    return os.str();
  };

  auto primitive_1  = [&loggedActions, logPrimitive](void* data)
  {
    loggedActions += logPrimitive(1u, data);
  };

  auto primitive_2  = [&loggedActions, logPrimitive](void* data)
  {
    loggedActions += logPrimitive(2u, data);
  };

  auto action_0 = [](const std::vector<Primitive>& primitives, void* data, const BinaryVector&)
  {
    primitives[0](data);
    return BinaryVector();
  };

  auto channelAction = [](const std::vector<Primitive>& primitives, uint32_t channelId, void* data, const BinaryVector& toSutData)
  {
    const string&  context = *reinterpret_cast<const string*>(data);
    BinaryVector   fromSutData;
    PrimitiveParam param(context, toSutData, fromSutData);

    primitives[channelId](&param);

    return param.fromSutData;
  };

  auto action_1 = [channelAction](const std::vector<Primitive>& primitives, void* data, const BinaryVector& toSutData)
  {
    return channelAction(primitives, 1u, data, toSutData);
  };

  auto action_2 = [channelAction](const std::vector<Primitive>& primitives, void* data, const BinaryVector& toSutData)
  {
    return channelAction(primitives, 2u, data, toSutData);
  };

  vector<Primitive> primitives = {primitive_0, primitive_1, primitive_2};
  vector<Action>    actions    = {action_0,    action_1,    action_2};

  auto sut = GenericAccessInterfaceProtocol(actions, primitives);
  return sut;
}
//
//  End of: CreateSut
//---------------------------------------------------------------------------
} // End of unnamed namespace

//! Initializes test (called for each test)
void UT_GenericAccessInterfaceProtocol::setUp ()
{
}


//! Checks GenericAccessInterfaceProtocol constructor with initializer_list
//!
void UT_GenericAccessInterfaceProtocol::test_Constructor_Initializer_List ()
{
  // ---------------- Setup
  //
  auto primitive_0  = [&](void*) { };
  auto primitive_1  = [&](void*) { };
  auto primitive_2  = [&](void*) { };

  auto action_0 = [](const std::vector<Primitive>&, void*, const BinaryVector&) { return BinaryVector(); };
  auto action_1 = [](const std::vector<Primitive>&, void*, const BinaryVector&) { return BinaryVector(); };
  auto action_2 = [](const std::vector<Primitive>&, void*, const BinaryVector&) { return BinaryVector(); };

  initializer_list<Primitive> primitives = {primitive_0, primitive_1, primitive_2};
  initializer_list<Action>    actions    = {action_0,    action_1,    action_2};

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (GenericAccessInterfaceProtocol(actions, primitives));
}


//! Checks GenericAccessInterfaceProtocol constructor with initializer_list but too few actions
//!
void UT_GenericAccessInterfaceProtocol::test_Constructor_Initializer_List_TooFew_Primitives ()
{
  // ---------------- Setup
  //
  auto primitive_0 = [&](void*) { };

  auto action_0 = [](const std::vector<Primitive>&, void*, const BinaryVector&) { return BinaryVector(); };
  auto action_1 = [](const std::vector<Primitive>&, void*, const BinaryVector&) { return BinaryVector(); };
  auto action_2 = [](const std::vector<Primitive>&, void*, const BinaryVector&) { return BinaryVector(); };

  initializer_list<Primitive> primitives = {primitive_0};
  initializer_list<Action>    actions    = {action_0,    action_1,    action_2};

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (GenericAccessInterfaceProtocol(actions, primitives), std::exception);
}


//! Checks GenericAccessInterfaceProtocol constructor with initializer_list but too few actions
//!
void UT_GenericAccessInterfaceProtocol::test_Constructor_Initializer_List_TooFew_Actions ()
{
  // ---------------- Setup
  //
  auto primitive_0  = [&](void*) { };
  auto primitive_1  = [&](void*) { };
  auto primitive_2  = [&](void*) { };

  auto action_0 = [](const std::vector<Primitive>&, void*, const BinaryVector&) { return BinaryVector(); };

  initializer_list<Primitive> primitives = {primitive_0, primitive_1, primitive_2};
  initializer_list<Action>    actions    = {action_0};

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (GenericAccessInterfaceProtocol(actions, primitives), std::exception);
}


//! Checks GenericAccessInterfaceProtocol constructor with initializer_list
//!
void UT_GenericAccessInterfaceProtocol::test_Constructor_Vector ()
{
  // ---------------- Setup
  //
  auto primitive_0  = [&](void*) { };
  auto primitive_1  = [&](void*) { };
  auto primitive_2  = [&](void*) { };

  auto action_0 = [](const std::vector<Primitive>&, void*, const BinaryVector&) { return BinaryVector(); };
  auto action_1 = [](const std::vector<Primitive>&, void*, const BinaryVector&) { return BinaryVector(); };
  auto action_2 = [](const std::vector<Primitive>&, void*, const BinaryVector&) { return BinaryVector(); };

  vector<Primitive> primitives = {primitive_0, primitive_1, primitive_2};
  vector<Action>    actions    = {action_0,    action_1,    action_2};

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (GenericAccessInterfaceProtocol(actions, primitives));
}


//! Checks GenericAccessInterfaceProtocol constructor with vectors but too few actions
//!
void UT_GenericAccessInterfaceProtocol::test_Constructor_Vector_TooFew_Primitives ()
{
  // ---------------- Setup
  //
  auto primitive_0 = [&](void*) { };

  auto action_0 = [](const std::vector<Primitive>&, void*, const BinaryVector&) { return BinaryVector(); };
  auto action_1 = [](const std::vector<Primitive>&, void*, const BinaryVector&) { return BinaryVector(); };
  auto action_2 = [](const std::vector<Primitive>&, void*, const BinaryVector&) { return BinaryVector(); };

  vector<Primitive> primitives = {primitive_0};
  vector<Action>    actions    = {action_0,    action_1,    action_2};

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (GenericAccessInterfaceProtocol(actions, primitives), std::exception);
}


//! Checks GenericAccessInterfaceProtocol constructor with vectors but too few actions
//!
void UT_GenericAccessInterfaceProtocol::test_Constructor_Vector_TooFew_Actions ()
{
  // ---------------- Setup
  //
  auto primitive_0  = [&](void*) { };
  auto primitive_1  = [&](void*) { };
  auto primitive_2  = [&](void*) { };

  auto action_0 = [](const std::vector<Primitive>&, void*, const BinaryVector&) { return BinaryVector(); };

  vector<Primitive> primitives = {primitive_0, primitive_1, primitive_2};
  vector<Action>    actions    = {action_0};

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (GenericAccessInterfaceProtocol(actions, primitives), std::exception);
}




//! Checks GenericAccessInterfaceProtocol::DoCallback with channel identifier 0
//!
void UT_GenericAccessInterfaceProtocol::test_DoCallback_0 ()
{
  // ---------------- Setup
  //
  auto loggedActions = ""s;
  auto sut           = CreateSut(loggedActions);
  auto text          = "Hello"s;
  auto toSutData     = BinaryVector();

  // ---------------- Exercise
  //
  auto gotVector = sut.DoCallback(0u, &text, toSutData);

  // ---------------- Verify
  //
  TS_ASSERT_TRUE   (gotVector.IsEmpty());
  TS_ASSERT_EQUALS (loggedActions, "Primitive_0(Hello)"s);
}


//! Checks GenericAccessInterfaceProtocol::DoCallback with channel identifier 1
//!
void UT_GenericAccessInterfaceProtocol::test_DoCallback_1 ()
{
  // ---------------- Setup
  //
  auto loggedActions = ""s;
  auto sut           = CreateSut(loggedActions);
  auto context       = "SIR"s;
  auto toSutData     = BinaryVector::CreateFromString("0x01");

  // ---------------- Exercise
  //
  auto gotVector = sut.DoCallback(1u, &context, toSutData);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (gotVector,     toSutData);
  TS_ASSERT_EQUALS (loggedActions, "Primitive_1(SIR, 0x01)"s);
}


//! Checks GenericAccessInterfaceProtocol::DoCallback with channel identifier 2
//!
void UT_GenericAccessInterfaceProtocol::test_DoCallback_2 ()
{
  // ---------------- Setup
  //
  auto loggedActions = ""s;
  auto sut           = CreateSut(loggedActions);
  auto context       = "SDR"s;
  auto toSutData     = BinaryVector::CreateFromString("/x3636/b1/xC0C0_C0C0");

  // ---------------- Exercise
  //
  auto gotVector = sut.DoCallback(2u, &context, toSutData);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (gotVector,     toSutData);
  TS_ASSERT_EQUALS (loggedActions, "Primitive_2(SDR, 0x3636_E060:6060_/b0)"s);
}


//! Checks GenericAccessInterfaceProtocol::DoCallback with channel identifier 3
//!
void UT_GenericAccessInterfaceProtocol::test_DoCallback_3 ()
{
  // ---------------- Setup
  //
  auto loggedActions = ""s;
  auto sut           = CreateSut(loggedActions);
  auto context       = "SDR"s;
  auto toSutData     = BinaryVector::CreateFromString("/x3636/b1/xC0C0_C0C0");

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.DoCallback(3u, &context, toSutData), std::exception);
}

//===========================================================================
// End of UT_GenericAccessInterfaceProtocol.cpp
//===========================================================================
