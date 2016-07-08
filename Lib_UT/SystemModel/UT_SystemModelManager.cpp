//===========================================================================
//                           UT_SystemModelManager.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SystemModelManager.cpp
//!
//! Implements test fixture for testing SystemModelManager
//!
//===========================================================================

#include "UT_SystemModelManager.hpp"
#include "SystemModelManager.hpp"
#include "SystemModelBuilder.hpp"
#include "GenericAccessInterfaceProtocol.hpp"
#include "Spy_AccessInterfaceProtocols.hpp"
#include "Spy_SVF_Protocol.hpp"
#include "Spy_I2C_Protocol.hpp"
#include "GmlPrinterVisitor.hpp"
#include "BinaryVector_Traits.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "ConfigureAlgorithm_LastOrDefault_Greedy.hpp"
#include "ConfigureAlgorithm_Last_Lazy.hpp"
#include "g3log/g3log.hpp"

#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <thread>
#include <atomic>
#include <chrono>

using std::make_shared;
using std::make_tuple;
using std::string;
using std::experimental::string_view;
using std::ostringstream;
using std::vector;
using namespace std::chrono_literals;
using namespace std::string_literals;
using namespace mast;
using namespace test;

using Primitive = GenericAccessInterfaceProtocol::Primitive;
using Action    = GenericAccessInterfaceProtocol::Action;

namespace
{

//! Creates test case "1500", setting some register "next to sut" values
//!
std::shared_ptr<AccessInterface> Create_TestCase_1500 (SystemModel& sm, string_view name = "Tap", bool reportGml = false)
{
  SystemModelBuilder builder(sm);

  auto ai        = builder.Create_TestCase_1500(name, 4u);
  auto ir        = sm.RegisterWithId(1u);
  auto bpy       = sm.RegisterWithId(3u);
  auto regStatic = sm.RegisterWithId(5u);
  auto regDyn_0  = sm.RegisterWithId(14u);
  auto regDyn_1  = sm.RegisterWithId(15u);
  auto regDyn_2  = sm.RegisterWithId(16u);
  auto regDyn_3  = sm.RegisterWithId(17u);

  regStatic->SetToSut  (BinaryVector(STATIC_TDR_LEN,  0x05));
  regDyn_0->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x09));
  regDyn_1->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x0A));
  regDyn_2->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0xB0));
  regDyn_3->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0xC0));

  regStatic->SetBypass (BinaryVector(STATIC_TDR_LEN,  0x36));
  regDyn_0->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x40));
  regDyn_1->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x41));
  regDyn_2->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x42));
  regDyn_3->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x43));

  if (reportGml)
  {
    TS_TRACE (GmlPrinterVisitor::Graph(ai, "Testcase_1500"));
  }

  return ai;
}
//
//  End of: Create_TestCase_1500
//---------------------------------------------------------------------------


//! Creates test case "MIB_Multichain_Pre"
//!
std::shared_ptr<AccessInterface> Create_TestCase_MIB_Multichain_Pre (SystemModel& sm, bool reportGml = false)
{
  SystemModelBuilder builder(sm);

  auto tap = builder.Create_TestCase_MIB_Multichain_Pre("TAP", 4u);

  auto regDyn_0  = sm.RegisterWithId(6u);
  auto regDyn_1  = sm.RegisterWithId(7u);
  auto regDyn_2  = sm.RegisterWithId(8u);
  auto regDyn_3  = sm.RegisterWithId(9u);

  regDyn_0->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x60));
  regDyn_1->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x61));
  regDyn_2->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x62));
  regDyn_3->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x63));

  regDyn_0->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x40));
  regDyn_1->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x41));
  regDyn_2->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x42));
  regDyn_3->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x43));

  if (reportGml)
  {
    TS_TRACE (GmlPrinterVisitor::Graph(tap, "MIB_Multichain_Pre"));
  }

  return tap;
}
//
//  End of: Create_TestCase_MIB_Multichain_Pre
//---------------------------------------------------------------------------


//! Creates test case "MIB_Multichain_Post"
//!
std::shared_ptr<AccessInterface> Create_TestCase_MIB_Multichain_Post (SystemModel& sm, bool reportGml = false)
{
  SystemModelBuilder builder(sm);

  auto tap = builder.Create_TestCase_MIB_Multichain_Post("TAP", 4u);

  auto regDyn_0  = sm.RegisterWithId(6u);
  auto regDyn_1  = sm.RegisterWithId(7u);
  auto regDyn_2  = sm.RegisterWithId(8u);
  auto regDyn_3  = sm.RegisterWithId(9u);

  regDyn_0->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x60));
  regDyn_1->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x61));
  regDyn_2->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x62));
  regDyn_3->SetToSut   (BinaryVector(DYNAMIC_TDR_LEN, 0x63));

  regDyn_0->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x40));
  regDyn_1->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x41));
  regDyn_2->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x42));
  regDyn_3->SetBypass  (BinaryVector(DYNAMIC_TDR_LEN, 0x43));

  if (reportGml)
  {
    TS_TRACE (GmlPrinterVisitor::Graph(tap, "MIB_Multichain_Post"));
  }

  return tap;
}
//
//  End of: Create_TestCase_MIB_Multichain_Post
//---------------------------------------------------------------------------

//! Creates a GenericAccessInterfaceProtocol suitable for following tests
//!
std::shared_ptr<GenericAccessInterfaceProtocol> CreateGenericAccessInterfaceProtocol (string& loggedActions)
{
  struct PrimitiveParam
  {
    PrimitiveParam(const BinaryVector& p_toSutData, BinaryVector& p_fromSutData)
      : toSutData   (p_toSutData)
      , fromSutData (p_fromSutData)
    {  }

    const BinaryVector& toSutData;
          BinaryVector& fromSutData;
  };

  auto primitive_0  = [&](void*)
  {
    loggedActions += "S2R : RESET()\n"s ;
  };

  auto logPrimitive = [](uint32_t primitiveId, void* data)
  {
    const auto& param = *reinterpret_cast<PrimitiveParam*>(data);
    param.fromSutData = param.toSutData;

    ostringstream os;
    os << "S2R : " << primitiveId << " " << param.toSutData.DataAsMixString() << "\n";
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

  auto derivationAction = [](const std::vector<Primitive>& primitives, uint32_t derivationId, void* /* data */, const BinaryVector& toSutData)
  {
    BinaryVector   fromSutData;
    PrimitiveParam param(toSutData, fromSutData);

    primitives[derivationId](&param);

    return param.fromSutData;
  };

  auto action_1 = [derivationAction](const std::vector<Primitive>& primitives, void* data, const BinaryVector& toSutData)
  {
    return derivationAction(primitives, 1u, data, toSutData);
  };

  auto action_2 = [derivationAction](const std::vector<Primitive>& primitives, void* data, const BinaryVector& toSutData)
  {
    return derivationAction(primitives, 2u, data, toSutData);
  };

  vector<Primitive> primitives = {primitive_0, primitive_1, primitive_2};
  vector<Action>    actions    = {action_0,    action_1,    action_2};

  auto protocol = make_shared<GenericAccessInterfaceProtocol>(actions, primitives);
  return protocol;
}
//
//  End of: CreateSut
//---------------------------------------------------------------------------

} // End of unnamed namespace

//! Initializes test (called for each test)
void UT_SystemModelManager::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}


//! Checks SystemModelManager constructor when there are no node in system model
//!
void UT_SystemModelManager::test_Constructor_EmptySystemModel ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (SystemModelManager sut(sm), std::exception);
}

//! Checks SystemModelManager constructor when there are no node in system model
//!
void UT_SystemModelManager::test_Constructor_SystemModel_Without_AI ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);

  auto root = builder.Create_Simple_MIB();

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (SystemModelManager sut(sm), std::exception);
}

//! Checks SystemModelManager constructor when root is an AccessInterface
//!
void UT_SystemModelManager::test_Constructor_Root_is_AI ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  SystemModelBuilder builder(sm);

  auto root = builder.Create_TestCase_AccessInterface();

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (SystemModelManager sut(sm));
}


//! Checks SystemModelManager constructor when root is a chain with a single AccessInterface
//!
void UT_SystemModelManager::test_Constructor_Root_is_Chain_1_AI ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto root = sm.CreateChain("root");
  SystemModelBuilder builder(sm);

  auto ai   = builder.Create_TestCase_AccessInterface();

  root->AppendChild(ai);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (SystemModelManager sut(sm));
}


//! Checks SystemModelManager constructor when root is a chain with 3 AccessInterfaces
//!
void UT_SystemModelManager::test_Constructor_Root_is_Chain_3_AI ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto root = sm.CreateChain("root");
  SystemModelBuilder builder(sm);

  auto ai_1 = builder.Create_TestCase_AccessInterface();
  auto ai_2 = builder.Create_TestCase_AccessInterface();
  auto ai_3 = builder.Create_TestCase_AccessInterface();

  root->AppendChild(ai_1);
  root->AppendChild(ai_2);
  root->AppendChild(ai_3);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (SystemModelManager sut(sm));
}


//! Checks SystemModelManager DoDataCycles when using AccessInterface testcase
//!
void UT_SystemModelManager::test_DoDataCycles_AccessInterface ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  SystemModelBuilder builder(sm);

  auto ai    = builder.Create_TestCase_AccessInterface();
  auto ir    = sm.RegisterWithId(1u);
  auto bpy   = sm.RegisterWithId(3u);
  auto reg_1 = sm.RegisterWithId(5u);
  auto reg_2 = sm.RegisterWithId(7u);

  reg_1->SetToSut   (BinaryVector(STATIC_TDR_LEN, 0x0A));
  reg_2->SetToSut   (BinaryVector(STATIC_TDR_LEN, 0xB0));
  reg_1->SetBypass  (BinaryVector(STATIC_TDR_LEN, 0x41));
  reg_2->SetBypass  (BinaryVector(STATIC_TDR_LEN, 0x42));

  auto spy = make_shared<Spy_AccessInterfaceProtocols>();
  ai->SetProtocol (spy);

//+  TS_TRACE (GmlPrinterVisitor::Graph(ai, "Testcase_AccessInterface"));

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  auto gotSutVectors = spy->ToSutVectors();

  std::vector<mast::BinaryVector> expected
  {
    BinaryVector::CreateFromString("/x02"),   // 00 : IR
    BinaryVector::CreateFromString("/xB0B0"), // 01 : DR (reg_2)
    BinaryVector::CreateFromString("/x01"),   // 02 : IR
    BinaryVector::CreateFromString("/x0A0A"), // 03 : DR (reg_1)
    BinaryVector::CreateFromString("/xFF"),   // 04 : IR
  };

  TS_ASSERT_EQUALS (gotSutVectors, expected);
}


//! Checks SystemModelManager DoDataCycles when using "1500" testcase
//!
void UT_SystemModelManager::test_DoDataCycles_1500 ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  auto ai  = Create_TestCase_1500(sm, "Tap", false);
  auto spy = make_shared<Spy_AccessInterfaceProtocols>();
  ai->SetProtocol (spy);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  auto gotSutVectors = spy->ToSutVectors();

  std::vector<mast::BinaryVector> expected
  {
    BinaryVector::CreateFromString("/x01"),                 // 00 : IR
    BinaryVector::CreateFromString("/x0505/b10"),           // 01 : DR
    BinaryVector::CreateFromString("/x3636/b0:100"),        // 02 : DR
    BinaryVector::CreateFromString("/x3636/b1/xC0C0_C0C0"), // 03 : DR
    BinaryVector::CreateFromString("/x3636/b0:011"),        // 04 : DR
    BinaryVector::CreateFromString("/x3636/b1/xB0B0_B0B0"), // 05 : DR
    BinaryVector::CreateFromString("/x3636/b0:010"),        // 06 : DR
    BinaryVector::CreateFromString("/x3636/b1/x0A0A_0A0A"), // 07 : DR
    BinaryVector::CreateFromString("/x3636/b0:001"),        // 08 : DR
    BinaryVector::CreateFromString("/x3636/b0/x0909_0909"), // 09 : DR
    BinaryVector::CreateFromString("/x3636/b1/x4040_4040"), // 10 : DR
    BinaryVector::CreateFromString("/x3636/b1:000"),        // 11 : DR
    BinaryVector::CreateFromString("/x3636/b0:000"),        // 15 : DR
    BinaryVector::CreateFromString("/xFF"),                 // 16 : IR
  };

  TS_ASSERT_EQUALS (gotSutVectors, expected);
}


//! Checks SystemModelManager DoDataCycles when using "1500" testcase and SVF protocol
//!
void UT_SystemModelManager::test_DoDataCycles_1500_SVF ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  auto ai  = Create_TestCase_1500(sm);
  auto spy = make_shared<Spy_SVF_Protocol>();
  ai->SetProtocol (spy);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  auto gotSvfCommands = spy->SVFCommands();

  vector<string> expected
  {
     "SIR 8 TDI(01);\n",                   // 00
     "SDR 18 TDI(001416);\n",              // 01
     "SDR 20 TDI(036364);\n",              // 02
     "SDR 49 TDI(006C6DC0C0C0C0);\n",      // 03
     "SDR 20 TDI(036363);\n",              // 04
     "SDR 49 TDI(006C6DB0B0B0B0);\n",      // 05
     "SDR 20 TDI(036362);\n",              // 06
     "SDR 49 TDI(006C6D0A0A0A0A);\n",      // 07
     "SDR 20 TDI(036361);\n",              // 08
     "SDR 49 TDI(006C6C09090909);\n",      // 09
     "SDR 49 TDI(006C6D40404040);\n",      // 10
     "SDR 20 TDI(036368);\n",              // 11
     "SDR 20 TDI(036360);\n",              // 15
     "SIR 8 TDI(FF);\n",                   // 16
  };

  TS_ASSERT_EQUALS (gotSvfCommands, expected);
}


//! Checks SystemModelManager DoDataCycles when using "1500" testcase and I2C protocol
//!
void UT_SystemModelManager::test_DoDataCycles_1500_I2C ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  auto ai        = Create_TestCase_1500(sm, "I2C");
  auto addresses = { 0x00u, 0x41u, 0x42u };
  auto spy       = make_shared<Spy_I2C_Protocol>(addresses, "S2R ");
  ai->SetProtocol (spy);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  auto gotI2cCommands = spy->I2CCommands();

  vector<string> expected
  {
     "S2R I2C_READ(0x41)\n"
     "S2R I2C_WRITE(0x41, 0x01)\n",
     "S2R I2C_READ(0x42)\n"
     "S2R I2C_WRITE(0x42, 0x0505_/b10)\n",
     "S2R I2C_READ(0x42)\n"
     "S2R I2C_WRITE(0x42, 0x3636_4)\n",
     "S2R I2C_READ(0x42)\n"
     "S2R I2C_WRITE(0x42, 0x3636_E060:6060_/b0)\n",
     "S2R I2C_READ(0x42)\n"
     "S2R I2C_WRITE(0x42, 0x3636_3)\n",
     "S2R I2C_READ(0x42)\n"
     "S2R I2C_WRITE(0x42, 0x3636_D858:5858_/b0)\n",
     "S2R I2C_READ(0x42)\n"
     "S2R I2C_WRITE(0x42, 0x3636_2)\n",
     "S2R I2C_READ(0x42)\n"
     "S2R I2C_WRITE(0x42, 0x3636_8505:0505_/b0)\n",
     "S2R I2C_READ(0x42)\n"
     "S2R I2C_WRITE(0x42, 0x3636_1)\n",
     "S2R I2C_READ(0x42)\n"
     "S2R I2C_WRITE(0x42, 0x3636_0484:8484_/b1)\n",
     "S2R I2C_READ(0x42)\n"
     "S2R I2C_WRITE(0x42, 0x3636_A020:2020_/b0)\n",
     "S2R I2C_READ(0x42)\n"
     "S2R I2C_WRITE(0x42, 0x3636_8)\n",
     "S2R I2C_READ(0x42)\n"
     "S2R I2C_WRITE(0x42, 0x3636_0)\n",
     "S2R I2C_READ(0x41)\n"
     "S2R I2C_WRITE(0x41, 0xFF)\n"
  };

  TS_ASSERT_EQUALS (gotI2cCommands, expected);
}


//! Checks SystemModelManager DoDataCycles when using "1500" testcase and Generic (table base) protocol
//!
void UT_SystemModelManager::test_DoDataCycles_1500_Generic ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto loggedActions = ""s;
  auto protocol      = CreateGenericAccessInterfaceProtocol(loggedActions);
  auto ai = Create_TestCase_1500(sm, "Generic");
  ai->SetProtocol (protocol);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  string expected
  {
     "S2R : 1 0x01\n"
     "S2R : 2 0x0505_/b10\n"
     "S2R : 2 0x3636_4\n"
     "S2R : 2 0x3636_E060:6060_/b0\n"
     "S2R : 2 0x3636_3\n"
     "S2R : 2 0x3636_D858:5858_/b0\n"
     "S2R : 2 0x3636_2\n"
     "S2R : 2 0x3636_8505:0505_/b0\n"
     "S2R : 2 0x3636_1\n"
     "S2R : 2 0x3636_0484:8484_/b1\n"
     "S2R : 2 0x3636_A020:2020_/b0\n"
     "S2R : 2 0x3636_8\n"
     "S2R : 2 0x3636_0\n"
     "S2R : 1 0xFF\n"
  };

  TS_ASSERT_EQUALS (loggedActions, expected);
}



//! Checks SystemModelManager DoDataCycles when using "1500" testcase and greedy
//! selection algorithm
void UT_SystemModelManager::test_DoDataCycles_1500_Greedy ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  auto ai = Create_TestCase_1500(sm, "Tap", false);

  auto spy = make_shared<Spy_AccessInterfaceProtocols>();
  ai->SetProtocol (spy);

  auto configureAlgo = make_shared<ConfigureAlgorithm_LastOrDefault_Greedy>();
  SystemModelManager sut(sm, configureAlgo);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  auto gotSutVectors = spy->ToSutVectors();

  std::vector<mast::BinaryVector> expected
  {
    BinaryVector::CreateFromString("/x01"),                 // 00 : IR
    BinaryVector::CreateFromString("/x0505/b10"),           // 01 : DR
    BinaryVector::CreateFromString("/x3636/b0:100"),        // 02 : DR
    BinaryVector::CreateFromString("/x3636/b1/xC0C0_C0C0"), // 03 : DR
    BinaryVector::CreateFromString("/x3636/b0:011"),        // 04 : DR
    BinaryVector::CreateFromString("/x3636/b1/xB0B0_B0B0"), // 05 : DR
    BinaryVector::CreateFromString("/x3636/b0:010"),        // 06 : DR
    BinaryVector::CreateFromString("/x3636/b1/x0A0A_0A0A"), // 07 : DR
    BinaryVector::CreateFromString("/x3636/b0:001"),        // 08 : DR
    BinaryVector::CreateFromString("/x3636/b1/x0909_0909"), // 09 : DR
    BinaryVector::CreateFromString("/x3636/b0:000"),        // 10 : DR
    BinaryVector::CreateFromString("/xFF"),                 // 11 : IR
  };

  TS_ASSERT_EQUALS (gotSutVectors, expected);
}


//! Checks SystemModelManager DoDataCycles when using "1500" testcase and lazy
//! selection algorithm
void UT_SystemModelManager::test_DoDataCycles_1500_Lazy ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  auto ai = Create_TestCase_1500(sm, "Tap", false);

  auto spy = make_shared<Spy_AccessInterfaceProtocols>();
  ai->SetProtocol (spy);

  auto configureAlgo = make_shared<ConfigureAlgorithm_Last_Lazy>();
  SystemModelManager sut(sm, configureAlgo);

//+  auto monitor       = make_shared<SystemModelManagerMonitor>();
//+  monitor->MonitorAfterConfiguration(true);
//+  monitor->MonitorBeforeConfiguration(true);
//+  monitor->GmlBasePath("1500_Lazy");
//+  SystemModelManager sut(sm, configureAlgo, monitor);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  auto gotSutVectors = spy->ToSutVectors();

  std::vector<mast::BinaryVector> expected
  {
    BinaryVector::CreateFromString("/x01"),                 // 00 : IR
    BinaryVector::CreateFromString("/x0505/b10"),           // 01 : DR
    BinaryVector::CreateFromString("/x3636/b0:100"),        // 02 : DR
    BinaryVector::CreateFromString("/x3636/b1/xC0C0_C0C0"), // 03 : DR
    BinaryVector::CreateFromString("/x3636/b0:011"),        // 04 : DR
    BinaryVector::CreateFromString("/x3636/b1/xB0B0_B0B0"), // 05 : DR
    BinaryVector::CreateFromString("/x3636/b0:010"),        // 06 : DR
    BinaryVector::CreateFromString("/x3636/b1/x0A0A_0A0A"), // 07 : DR
    BinaryVector::CreateFromString("/x3636/b0:001"),        // 08 : DR
    BinaryVector::CreateFromString("/x3636/b0/x0909_0909"), // 09 : DR
  };

  TS_ASSERT_EQUALS (gotSutVectors, expected);
}


//! Checks SystemModelManager DoDataCycles when using "MIB_Multichain_Pre" testcase
//!
void UT_SystemModelManager::test_DoDataCycles_MIB_Multichain_Pre ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  auto ai = Create_TestCase_MIB_Multichain_Pre(sm, false);

  auto spy = make_shared<Spy_AccessInterfaceProtocols>();
  ai->SetProtocol (spy);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  auto gotSutVectors = spy->ToSutVectors();

  std::vector<mast::BinaryVector> expected
  {
    BinaryVector::CreateFromString("0x02"),        // 00 : IR
    BinaryVector::CreateFromString("0x6060_6060"), // 01 : DR
    BinaryVector::CreateFromString("0x01"),        // 02 : IR
    BinaryVector::CreateFromString("0b11"),        // 03 : DR
    BinaryVector::CreateFromString("0x02"),        // 04 : IR
    BinaryVector::CreateFromString("0x6363_6363"), // 05 : DR
    BinaryVector::CreateFromString("0x01"),        // 06 : IR
    BinaryVector::CreateFromString("0b10"),        // 07 : DR
    BinaryVector::CreateFromString("0x02"),        // 08 : IR
    BinaryVector::CreateFromString("0x6262_6262"), // 09 : DR
    BinaryVector::CreateFromString("0x01"),        // 10 : IR
    BinaryVector::CreateFromString("0b01"),        // 11 : DR
    BinaryVector::CreateFromString("0x02"),        // 12 : IR
    BinaryVector::CreateFromString("0x6161_6161"), // 13 : DR
    BinaryVector::CreateFromString("0x4141_4141"), // 14 : DR
    BinaryVector::CreateFromString("0x01"),        // 15 : IR
    BinaryVector::CreateFromString("0b00"),        // 16 : DR
    BinaryVector::CreateFromString("0xFF"),        // 17 : IR
  };

  TS_ASSERT_EQUALS (gotSutVectors, expected);
}


//! Checks SystemModelManager DoDataCycles when using "MIB_Multichain_Pre" testcase
//! with greedy selection algorithm
//!
void UT_SystemModelManager::test_DoDataCycles_MIB_Multichain_Pre_Greedy ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  auto ai = Create_TestCase_MIB_Multichain_Pre(sm, false);

  auto spy = make_shared<Spy_AccessInterfaceProtocols>();
  ai->SetProtocol (spy);

  auto configureAlgo = make_shared<ConfigureAlgorithm_LastOrDefault_Greedy>();
  SystemModelManager sut(sm, configureAlgo);

//+  auto monitor       = make_shared<SystemModelManagerMonitor>();
//+  monitor->MonitorAfterConfiguration(true);
//+  monitor->MonitorBeforeConfiguration(true);
//+  monitor->GmlBasePath("MIB_Multichain_Pre_Greedy");
//+  SystemModelManager sut(sm, configureAlgo, monitor);
//+  g3::logEnabled(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

//+  g3::logEnabled(false);

  // ---------------- Verify
  //
  auto gotSutVectors = spy->ToSutVectors();

  std::vector<mast::BinaryVector> expected
  {
    BinaryVector::CreateFromString("0x02"),        // 00 : IR
    BinaryVector::CreateFromString("0x6060_6060"), // 01 : DR
    BinaryVector::CreateFromString("0x01"),        // 02 : IR
    BinaryVector::CreateFromString("0b11"),        // 03 : DR
    BinaryVector::CreateFromString("0x02"),        // 04 : IR
    BinaryVector::CreateFromString("0x6363_6363"), // 05 : DR
    BinaryVector::CreateFromString("0x01"),        // 06 : IR
    BinaryVector::CreateFromString("0b10"),        // 07 : DR
    BinaryVector::CreateFromString("0x02"),        // 08 : IR
    BinaryVector::CreateFromString("0x6262_6262"), // 09 : DR
    BinaryVector::CreateFromString("0x01"),        // 10 : IR
    BinaryVector::CreateFromString("0b01"),        // 11 : DR
    BinaryVector::CreateFromString("0x02"),        // 12 : IR
    BinaryVector::CreateFromString("0x6161_6161"), // 13 : DR
    BinaryVector::CreateFromString("0x01"),        // 14 : IR
    BinaryVector::CreateFromString("0b00"),        // 15 : DR
    BinaryVector::CreateFromString("0xFF"),        // 16 : IR
  };

  TS_ASSERT_EQUALS (gotSutVectors, expected);
}


//! Checks SystemModelManager DoDataCycles when using "MIB_Multichain_Pre" testcase
//! with lazy selection algorithm
//!
void UT_SystemModelManager::test_DoDataCycles_MIB_Multichain_Pre_Lazy ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  auto ai  = Create_TestCase_MIB_Multichain_Pre(sm, false);
  auto spy = make_shared<Spy_AccessInterfaceProtocols>();
  ai->SetProtocol (spy);

  auto configureAlgo = make_shared<ConfigureAlgorithm_Last_Lazy>();
  SystemModelManager sut(sm, configureAlgo);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  auto gotSutVectors = spy->ToSutVectors();

  std::vector<mast::BinaryVector> expected
  {
    BinaryVector::CreateFromString("0x02"),        // 00 : IR
    BinaryVector::CreateFromString("0x6060_6060"), // 01 : DR
    BinaryVector::CreateFromString("0x01"),        // 02 : IR
    BinaryVector::CreateFromString("0b11"),        // 03 : DR
    BinaryVector::CreateFromString("0x02"),        // 04 : IR
    BinaryVector::CreateFromString("0x6363_6363"), // 05 : DR
    BinaryVector::CreateFromString("0x01"),        // 06 : IR
    BinaryVector::CreateFromString("0b10"),        // 07 : DR
    BinaryVector::CreateFromString("0x02"),        // 08 : IR
    BinaryVector::CreateFromString("0x6262_6262"), // 09 : DR
    BinaryVector::CreateFromString("0x01"),        // 10 : IR
    BinaryVector::CreateFromString("0b01"),        // 11 : DR
    BinaryVector::CreateFromString("0x02"),        // 12 : IR
    BinaryVector::CreateFromString("0x6161_6161"), // 13 : DR
  };

  TS_ASSERT_EQUALS (gotSutVectors, expected);
}

//! Checks SystemModelManager DoDataCycles when using "MIB_Multichain_Post" testcase
//!
void UT_SystemModelManager::test_DoDataCycles_MIB_Multichain_Post ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  auto ai = Create_TestCase_MIB_Multichain_Post(sm, false);

  auto spy = make_shared<Spy_AccessInterfaceProtocols>();
  ai->SetProtocol (spy);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  auto gotSutVectors = spy->ToSutVectors();

  std::vector<mast::BinaryVector> expected
  {
    BinaryVector::CreateFromString("0x02"),        // 00 : IR
    BinaryVector::CreateFromString("0b11"),        // 01 : DR
    BinaryVector::CreateFromString("0x01"),        // 02 : IR
    BinaryVector::CreateFromString("0x6363_6363"), // 03 : DR
    BinaryVector::CreateFromString("0x02"),        // 04 : IR
    BinaryVector::CreateFromString("0b10"),        // 05 : DR
    BinaryVector::CreateFromString("0x01"),        // 06 : IR
    BinaryVector::CreateFromString("0x6262_6262"), // 07 : DR
    BinaryVector::CreateFromString("0x02"),        // 08 : IR
    BinaryVector::CreateFromString("0b01"),        // 09 : DR
    BinaryVector::CreateFromString("0x01"),        // 10 : IR
    BinaryVector::CreateFromString("0x6161_6161"), // 11 : DR
    BinaryVector::CreateFromString("0x02"),        // 12 : IR
    BinaryVector::CreateFromString("0b00"),        // 13 : DR
    BinaryVector::CreateFromString("0x01"),        // 14 : IR
    BinaryVector::CreateFromString("0x6060_6060"), // 15 : DR
    BinaryVector::CreateFromString("0xFF"),        // 16 : IR
  };

  TS_ASSERT_EQUALS (gotSutVectors, expected);
}


//! Checks SystemModelManager::CreateApplicationThread() - Single application
//!
void UT_SystemModelManager::test_CreateApplicationThread_1_App ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);
  auto mux  = sm.LinkerWithId(2u);
  TS_ASSERT_NOT_NULLPTR (mux);

  SystemModelManager sut(sm);

  // ---------------- Create a functor that tally value when not zero
  //
  std::atomic_uint value(0);
  std::atomic_bool started(false);
  value = 0u;

  uint32_t sum = 0;
  auto appFunctor = [&value, &sum, &started]()
  {
    started = true;
    while (true)
    {
      if (value != 0) // This is the "protocol" to say that there is a new value
      {
        sum  += value;
        if (value >= 100u)
        {
          break;
        }
        value = 0;
      }
    }
  };

  // ---------------- Exercise
  //  There is an artificial block scope too wait for sut destructor
  {
    TS_ASSERT_THROWS_NOTHING (sut.CreateApplicationThread(mux, appFunctor));

    // ---------------- Verify
    //
    value = 1u; // 1rst value
    std::this_thread::sleep_for(2ms);    // Let the thread to be started by the system
    TS_ASSERT_EQUALS (sum,          0);  // Thread is waiting for start signal ==> sum does not change
    TS_ASSERT_EQUALS (value.load(), 1u); // Value is also not changed
    sut.StartCreatedApplicationThreads();
    while (!started){std::this_thread::sleep_for(10us);}  // Wait for start signal being seen by application thread function
    TS_ASSERT_EQUALS (sum,          1u); // Thread is now started ==> sum has been updated
    TS_ASSERT_EQUALS (value.load(), 0);  // Value has been reset

    for (uint32_t ii = 2u ; ii <= 100u ; ++ii)
    {
      while (value != 0);
      value = ii;
    }
    sut.JoinAllApplicationThreads();  // Make sure the application thread has seen the last value

    TS_ASSERT_EQUALS (sum, 5050u);
  }
}


//! Checks SystemModelManager::CreateApplicationThread() - Two applications
//!
void UT_SystemModelManager::test_CreateApplicationThread_2_App ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);
  auto mux  = sm.LinkerWithId(2u);
  TS_ASSERT_NOT_NULLPTR (mux);

  g3::logEnabled(true);
  SystemModelManager sut(sm);

  // ---------------- Create a functor that tally value when not zero
  //
  std::atomic_uint valueApp_1(0);
  std::atomic_uint valueApp_2(0);

  auto app_1_Functor = [&valueApp_1]() { valueApp_1 = 13u; };
  auto app_2_Functor = [&valueApp_2]() { valueApp_2 = 37u; };

  // ---------------- Exercise
  //  There is an artificial block scope too wait for sut destructor
  {
    TS_ASSERT_THROWS_NOTHING (sut.CreateApplicationThread(mux, app_1_Functor));
    TS_ASSERT_THROWS_NOTHING (sut.CreateApplicationThread(mux, app_2_Functor));

    // ---------------- Verify
    //
    std::this_thread::sleep_for(1ms); // To let threads to be started
    TS_ASSERT_EQUALS (valueApp_1.load(), 0u);
    TS_ASSERT_EQUALS (valueApp_2.load(), 0u);

    sut.StartCreatedApplicationThreads(); // Do effectively start both application threads
    sut.JoinAllApplicationThreads();      // Make sure application have done their job

    TS_ASSERT_EQUALS (valueApp_1.load(), 13u);
    TS_ASSERT_EQUALS (valueApp_2.load(), 37u);
  }
  g3::logEnabled(false);
}


//! Checks SystemModelManager::CreateApplicationThread() passing a nullptr for application top node
//!
void UT_SystemModelManager::test_CreateApplicationThread_Top_is_Nullptr ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  SystemModelManager sut(sm);

  // ---------------- Create a do nothing functor
  //
  auto appFunctor = []() { };

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS (sut.CreateApplicationThread(nullptr, appFunctor), std::exception);
}


//! Checks SystemModelManager::iPrefix() using same thread as SystemModelManager
//!
void UT_SystemModelManager::test_iPrefix_Thread_is_SystemModelManager ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto prefix = "TAP_DR_Mux.MIB_mux";
  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.iPrefix(prefix));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.iPrefix(), prefix);
}


//! Checks SystemModelManager::iPrefix() using thread managed (known) by SystemModelManager
//!
void UT_SystemModelManager::test_iPrefix_Thread_is_Known ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);
  auto mux  = sm.LinkerWithId(2u);   // This is Tap mux

  SystemModelManager sut(sm);

  auto   prefix = "MIB_mux";
  string gotPrefix;

  auto appFunctor = [prefix, &gotPrefix, &sut]()
  {
    // ---------------- Exercise (set & get)
    //
    TS_ASSERT_THROWS_NOTHING (sut.iPrefix(prefix));
    TS_ASSERT_THROWS_NOTHING (gotPrefix = sut.iPrefix());
  };

//+  g3::logEnabled(true);
  sut.CreateApplicationThread(mux, appFunctor); // Include "Exercise" in created thread
  sut.StartCreatedApplicationThreads();

  // ---------------- Verify
  //
  sut.JoinAllApplicationThreads();  // Make sure application as done its action
//+  g3::logEnabled(false);

  CxxTest::setStringResultsOnNewLine(false);
  TS_ASSERT_EQUALS (gotPrefix, prefix);
}


//! Checks SystemModelManager::iPrefix() using thread not managed (unknown) by SystemModelManager
//!
void UT_SystemModelManager::test_iPrefix_Thread_is_Unknown ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto mux  = sm.LinkerWithId(2u);   // This is Tap mux

  SystemModelManager sut(sm);

  auto   prefix = "MIB_mux";
  string gotPrefix;

  auto appFunctor = [prefix, &gotPrefix, &sut]()
  {
    // ---------------- Exercise & Verify (set & get)
    //
    TS_ASSERT_THROWS (sut.iPrefix(prefix),       std::exception);
    TS_ASSERT_THROWS (gotPrefix = sut.iPrefix(), std::exception);
  };

  auto unkwnownThread = std::thread(appFunctor);
  unkwnownThread.join();
}


//! Checks SystemModelManager::iGet() using same thread as SystemModelManager
//!
void UT_SystemModelManager::test_iGet_Thread_is_SystemModelManager ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto reg  = sm.RegisterWithId(7u);
  reg->SetFromSut(BinaryVector::CreateFromHexString("ABCD_0123"));

  SystemModelManager sut(sm);
  sut.iPrefix("TAP_DR_Mux.MIB_mux");

  // ---------------- Exercise
  //
  auto lastFromSut = sut.iGet("dynamic_1");

  // ---------------- Verify
  //
  auto expected = BinaryVector::CreateFromHexString("ABCD_0123");
  TS_ASSERT_EQUALS (lastFromSut, expected);
}


//! Checks SystemModelManager::iGet() using thread managed (known) by SystemModelManager
//!
void UT_SystemModelManager::test_iGet_Thread_is_Known ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto mux  = sm.LinkerWithId(2u);   // This is Tap mux
  auto reg  = sm.RegisterWithId(7u);
  reg->SetFromSut(BinaryVector::CreateFromHexString("ABCD_0123"));

  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut]()
  {
    // ---------------- Setup
    //
    sut.iPrefix("MIB_mux");

    // ---------------- Exercise
    //
    auto lastFromSut = sut.iGet("dynamic_1");

    // ---------------- Verify
    //
    CxxTest::setStringResultsOnNewLine(false);
    auto expected = BinaryVector::CreateFromHexString("ABCD_0123");
    TS_ASSERT_EQUALS (lastFromSut, expected);
  };

  g3::logEnabled(true);
  sut.CreateApplicationThread(mux, appFunctor); // Include "Exercise" in created thread
  sut.StartCreatedApplicationThreads();

  // ---------------- Verify
  //
  sut.JoinAllApplicationThreads();  // Make sure application as done its action
  g3::logEnabled(false);
}


//! Checks SystemModelManager::iGet() using thread not managed (unknown) by SystemModelManager
//!
void UT_SystemModelManager::test_iGet_Thread_is_Unknown ()
{
  // ---------------- Setup (cxxtest thread)
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto reg  = sm.RegisterWithId(7u);
  reg->SetFromSut(BinaryVector::CreateFromHexString("ABCD_0123"));

  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut]()
  {
    // ---------------- Exercise & Verify (functor thread)
    //
    TS_ASSERT_THROWS (sut.iGet("MIB_mux.dynamic_1"), std::exception);
  };

  // Start thread
  g3::logEnabled(true);
  auto unkwnownThread = std::thread(appFunctor);
  unkwnownThread.join();

  // Wait end of thread
  sut.JoinAllApplicationThreads();  // Make sure application as done its action
  g3::logEnabled(false);
}


//! Checks SystemModelManager::iWrite() using same thread as SystemModelManager
//!
void UT_SystemModelManager::test_iWrite_Thread_is_SystemModelManager ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto reg  = sm.RegisterWithId(7u);

  SystemModelManager sut(sm);
  sut.iPrefix("TAP_DR_Mux.MIB_mux");

  auto nextToSut = BinaryVector::CreateFromHexString("ABCD_4567");

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.iWrite("dynamic_1", nextToSut));

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (reg->NextToSut(), nextToSut);
}


//! Checks SystemModelManager::iWrite() using thread managed (known) by SystemModelManager
//!
void UT_SystemModelManager::test_iWrite_Thread_is_Known ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto mux  = sm.LinkerWithId(2u);   // This is Tap mux
  auto reg  = sm.RegisterWithId(9u);

  g3::logEnabled(true);
  {
    LOG_FUNCTION_SCOPE;
    SystemModelManager sut(sm);

    // Thread functor
    auto appFunctor = [&sut]()
    {
      // ---------------- Setup
      //
      auto nextToSut = BinaryVector::CreateFromHexString("FADE_CAFE");
      sut.iPrefix("MIB_mux");

      // ---------------- Exercise
      //
      TS_ASSERT_THROWS_NOTHING (sut.iWrite("dynamic_3", nextToSut));
    };

    // ---------------- Setup (main thread)
    //
    sut.CreateApplicationThread(mux, appFunctor); // Include "Exercise" in created thread
    sut.StartCreatedApplicationThreads();
    sut.JoinAllApplicationThreads();  // Make sure application as done its action

    // ---------------- Verify
    //
    auto expected = BinaryVector::CreateFromHexString("FADE_CAFE");
    TS_ASSERT_EQUALS (reg->NextToSut(), expected);
  }
  g3::logEnabled(false);
}


//! Checks SystemModelManager::iWrite() using thread not managed (unknown) by SystemModelManager
//!
void UT_SystemModelManager::test_iWrite_Thread_is_Unknown ()
{
  // ---------------- Setup (main thread)
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut]()
  {
    // ---------------- Setup
    //
    auto nextToSut = BinaryVector::CreateFromHexString("2BAD_CAFE");

    // ---------------- Exercise & Verify (functor thread)
    //
    TS_ASSERT_THROWS (sut.iWrite("MIB_mux.dynamic_0", nextToSut), std::exception);
  };

  // Start thread
  auto unkwnownThread = std::thread(appFunctor);
  unkwnownThread.join();
}


//! Checks SystemModelManager::iApply() using same thread as SystemModelManager
//!
void UT_SystemModelManager::test_iApply_Thread_is_SystemModelManager ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto reg  = sm.RegisterWithId(7u);

  SystemModelManager sut(sm);
  sut.iPrefix("TAP_DR_Mux.MIB_mux");

  auto nextToSut = BinaryVector::CreateFromHexString("ABCD_4567");
  sut.iWrite("dynamic_1", nextToSut);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.iApply());

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (reg->NextToSut(), reg->LastToSut());
}


//! Checks SystemModelManager::iApply() using same thread as SystemModelManager and there is no pending registers
//!
void UT_SystemModelManager::test_iApply_Thread_is_SystemModelManager_NoPending ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto reg  = sm.RegisterWithId(7u);

  SystemModelManager sut(sm);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.iApply());
}


//! Checks SystemModelManager::iApply() using thread managed (known) by SystemModelManager
//!
void UT_SystemModelManager::test_iApply_Thread_is_Known ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto mux  = sm.LinkerWithId(2u);   // This is Tap mux
  auto reg  = sm.RegisterWithId(9u);

  ENABLE_LOG_IN_SCOPE;
  LOG_FUNCTION_SCOPE;
  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut]()
  {
    // ---------------- Setup
    //
    auto nextToSut = BinaryVector::CreateFromHexString("FADE_CAFE");
    sut.iPrefix("MIB_mux");
    sut.iWrite("dynamic_3", nextToSut);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.iApply());
  };

  // ---------------- Setup (main thread)
  //
  sut.CreateApplicationThread(mux, appFunctor); // Include "Exercise" in created thread
  sut.StartInBackground();
  sut.StartCreatedApplicationThreads();
  sut.JoinAllApplicationThreads();              // Make sure application as done its action
  sut.Stop();

  // ---------------- Verify
  //
  auto expected = BinaryVector::CreateFromHexString("FADE_CAFE");
  TS_ASSERT_EQUALS (reg->NextToSut(), reg->LastToSut());
}


//! Checks SystemModelManager::iApply() when SystemModelManager data cycle loop has not been started
//!
void UT_SystemModelManager::test_iApply_DataCycleLoop_NotStarted ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto mux  = sm.LinkerWithId(2u);   // This is Tap mux
  auto reg  = sm.RegisterWithId(9u);

  ENABLE_LOG_IN_SCOPE;
  LOG_FUNCTION_SCOPE;
  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut]()
  {
    // ---------------- Setup
    //
    auto nextToSut = BinaryVector::CreateFromHexString("FADE_CAFE");
    sut.iPrefix("MIB_mux");
    sut.iWrite("dynamic_3", nextToSut);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.iApply());
  };

  // ---------------- Setup (main thread)
  //
  //                  No data cycle loop do not prevent to quit the application
  //
  sut.CreateApplicationThread(mux, appFunctor); // Include "Exercise" in created thread
  sut.StartCreatedApplicationThreads();
  sut.JoinAllApplicationThreads();              // Make sure application as done its action
  sut.Stop();

  // ---------------- Verify
  //
  auto expected = BinaryVector::CreateFromHexString("FADE_CAFE");
  TS_ASSERT_DIFFERS (reg->NextToSut(), reg->LastToSut());   // As no data cycle took place new value has not been transfered to SUT
}


//! Checks SystemModelManager::Start() from another thread
//!
void UT_SystemModelManager::test_Start_from_Another_Thread ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto mux  = sm.LinkerWithId(2u);   // This is Tap mux
  auto reg  = sm.RegisterWithId(9u);

  ENABLE_LOG_IN_SCOPE;
  LOG_FUNCTION_SCOPE;
  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut]()
  {
    TS_ASSERT_THROWS_NOTHING
    (
      auto nextToSut = BinaryVector::CreateFromHexString("FADE_CAFE");
      sut.iPrefix("MIB_mux");
      sut.iWrite("dynamic_3", nextToSut);
      sut.iApply();
    );
  };

  sut.CreateApplicationThread(mux, appFunctor);
  std::atomic_bool started(false);

  auto startFunctor = [&sut, & started]() { TS_ASSERT_THROWS_NOTHING (started = true; sut.Start()); };

  // ---------------- Exercise
  //
  auto startThread = std::thread(startFunctor);

  // ---------------- Verify
  //
  while (!started)  //!< Wait data cycle loop thread is effectively started
  {
    std::this_thread::sleep_for(100us);
  }
  sut.StartCreatedApplicationThreads();
  sut.JoinAllApplicationThreads();              // Make sure application as done its action
  sut.Stop();
  if (startThread.joinable())
  {
    startThread.join();
  }
  auto expected = BinaryVector::CreateFromHexString("FADE_CAFE");
  TS_ASSERT_EQUALS (reg->NextToSut(), reg->LastToSut());
}


//! Checks SystemModelManager::iApply() using thread managed (known) by SystemModelManager
//!
void UT_SystemModelManager::test_iApply_Thread_is_Known_NoPending ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto mux  = sm.LinkerWithId(2u);   // This is Tap mux
  auto reg  = sm.RegisterWithId(9u);

  ENABLE_LOG_IN_SCOPE;
  LOG_FUNCTION_SCOPE;
  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut]()
  {
    // ---------------- Setup
    //
    auto nextToSut = BinaryVector::CreateFromHexString("FADE_CAFE");

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS_NOTHING (sut.iApply());
  };

  // ---------------- Setup (main thread)
  //
  TS_ASSERT_THROWS_NOTHING
  (
    sut.CreateApplicationThread(mux, appFunctor, "test_iApply_Thread_is_Known_NoPending"); // Include "Exercise" in created thread
    sut.StartInBackground();
    sut.StartCreatedApplicationThreads();
    sut.JoinAllApplicationThreads();              // Make sure application as done its action
  );
}


//! Checks SystemModelManager::iApply() using thread managed (known) by SystemModelManager
//!
void UT_SystemModelManager::test_iApply_Thread_is_Known_NoPending_WrongStart ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto mux  = sm.LinkerWithId(2u);   // This is Tap mux
  auto reg  = sm.RegisterWithId(9u);

  ENABLE_LOG_IN_SCOPE;
  LOG_FUNCTION_SCOPE;
  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut]()
  {
    // ---------------- Setup
    //
    auto nextToSut = BinaryVector::CreateFromHexString("FADE_CAFE");

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS_NOTHING (sut.iApply());
  };

  // ---------------- Setup (main thread)
  //
  TS_ASSERT_THROWS_NOTHING (sut.CreateApplicationThread(mux, appFunctor)); // Include "Exercise" in created thread );
  TS_ASSERT_THROWS (sut.Start(), std::exception);
}




//! Checks SystemModelManager::iApply() using thread not managed (unknown) by SystemModelManager
//!
void UT_SystemModelManager::test_iApply_Thread_is_Unknown ()
{
  // ---------------- Setup (main thread)
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut]()
  {
    // ---------------- Setup
    //
    auto nextToSut = BinaryVector::CreateFromHexString("2BAD_CAFE");

    // ---------------- Exercise & Verify (functor thread)
    //
    TS_ASSERT_THROWS (sut.iApply(), std::exception);
  };

  // Start thread
  auto unkwnownThread = std::thread(appFunctor);
  unkwnownThread.join();
}



//===========================================================================
// End of UT_SystemModelManager.cpp
//===========================================================================
