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
#include "TestModelBuilder.hpp"
#include "GenericAccessInterfaceProtocol.hpp"
#include "LoopbackAccessInterfaceProtocol.hpp"
#include "SpiedProtocolsCommands.hpp"
#include "Spy_AccessInterfaceProtocols.hpp"
#include "Spy_SVF_Protocol.hpp"
#include "Spy_I2C_Protocol.hpp"
#include "Spy_Emulation_Translator.hpp"
#include "GmlPrinter.hpp"
#include "BinaryVector_Traits.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "ConfigureAlgorithm_LastOrDefault_Greedy.hpp"
#include "ConfigureAlgorithm_Last_Lazy.hpp"
#include "VirtualRegister.hpp"
#include "RegistersAlias.hpp"
#include "g3log/g3log.hpp"
#include "Emulation_TranslatorProtocol.hpp"
#include "I2C_RawPlayer.hpp"
#include "SVF_RawPlayer.hpp"
#include <initializer_list>
#include "SystemModelChecker.hpp"
#include "JTAG_to_I2C_TranslatorProtocol.hpp"

#include "PrettyPrinter.hpp"

#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <thread>
#include <atomic>
#include <chrono>

using std::shared_ptr;
using std::make_shared;
using std::make_tuple;
using std::string;
using std::experimental::string_view;
using std::ostringstream;
using std::vector;
using std::dynamic_pointer_cast;

using namespace std::chrono_literals;
using namespace std::string_literals;
using namespace mast;
using namespace test;

using Primitive = GenericAccessInterfaceProtocol::Primitive;
using Action    = GenericAccessInterfaceProtocol::Action;

#define LOG_TEST_DURATION

namespace
{
  #ifdef LOG_TEST_DURATION
  std::chrono::time_point<std::chrono::steady_clock> g_testStartTime;
  #endif  // not define LOG_TEST_DURATION
} // End of unnamed namespace

namespace
{

//! Creates test case "1500", setting some register "next to sut" values
//!
std::shared_ptr<AccessInterface> Create_TestCase_1500 (SystemModel& sm, string_view name = "Tap", bool reportGml = false)
{
  TestModelBuilder builder(sm);

  auto ai        = builder.Create_TestCase_1500(name, 4u);
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
    TS_TRACE (GmlPrinter::Graph(ai, "Testcase_1500"));
  }

  return ai;
}
//
//  End of: Create_TestCase_1500
//---------------------------------------------------------------------------


//! Creates test case "MIB_Multichain_Pre"
//!
std::shared_ptr<AccessInterface> Create_TestCase_MIB_Multichain_Pre (SystemModel& sm,
                                                                     bool         reportGml     = false,
                                                                     uint32_t     regsBitsCount = DYNAMIC_TDR_LEN)
{
  TestModelBuilder builder(sm);

  auto tap = builder.Create_TestCase_MIB_Multichain_Pre("TAP", 4u, regsBitsCount);

  auto regDyn_0  = sm.RegisterWithId(6u);
  auto regDyn_1  = sm.RegisterWithId(7u);
  auto regDyn_2  = sm.RegisterWithId(8u);
  auto regDyn_3  = sm.RegisterWithId(9u);

  regDyn_0->SetToSut   (BinaryVector(regsBitsCount, 0x60));
  regDyn_1->SetToSut   (BinaryVector(regsBitsCount, 0x61));
  regDyn_2->SetToSut   (BinaryVector(regsBitsCount, 0x62));
  regDyn_3->SetToSut   (BinaryVector(regsBitsCount, 0x63));

  regDyn_0->SetBypass  (BinaryVector(regsBitsCount, 0x40));
  regDyn_1->SetBypass  (BinaryVector(regsBitsCount, 0x41));
  regDyn_2->SetBypass  (BinaryVector(regsBitsCount, 0x42));
  regDyn_3->SetBypass  (BinaryVector(regsBitsCount, 0x43));

  regDyn_0->SetPendingForRead(); regDyn_0->SetFromSut(BinaryVector(regsBitsCount, 0x50));
  regDyn_1->SetPendingForRead(); regDyn_1->SetFromSut(BinaryVector(regsBitsCount, 0x51));
  regDyn_2->SetPendingForRead(); regDyn_2->SetFromSut(BinaryVector(regsBitsCount, 0x52));
  regDyn_3->SetPendingForRead(); regDyn_3->SetFromSut(BinaryVector(regsBitsCount, 0x53));

  if (reportGml)
  {
    TS_TRACE (GmlPrinter::Graph(tap, "MIB_Multichain_Pre"));
  }

  return tap;
}
//
//  End of: Create_TestCase_MIB_Multichain_Pre
//---------------------------------------------------------------------------


//! Creates test case for iPrefix related test using "MIB_Multichain_Pre" type model
//!
std::shared_ptr<AccessInterface> Create_TestCase_ForIPrefix (SystemModel& sm, bool reportGml = false)
{
  SystemModelBuilder builder(sm);
  TestModelBuilder   testBuilder(sm);

  auto tap         = testBuilder.Create_JTAG_TAP("Tap", 8u, 3u);

  // ---------------- Append MIB with control register before mux
  //
  auto chainsCount = uint32_t(4u);
  auto res         = builder.Create_PathSelector(SelectorKind::Binary, "MIB_ctrl", chainsCount);
  auto mibCtrl     = res.first;
  auto mibSelector = res.second;

  tap->AppendChild(mibCtrl);
  auto mibMux      = sm.CreateLinker("MIB_mux", mibSelector, tap);

  // ---------------- Add wrapped cores (registers)
  //
  testBuilder.AppendChains(chainsCount, "Chain_", mibMux);

  if (reportGml)
  {
    TS_TRACE (GmlPrinter::Graph(tap, "Testcase_iPrefix"));
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
  TestModelBuilder builder(sm);

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
    TS_TRACE (GmlPrinter::Graph(tap, "MIB_Multichain_Post"));
  }

  return tap;
}
//
//  End of: Create_TestCase_MIB_Multichain_Post
//---------------------------------------------------------------------------


//! Creates test case "Brocade"
//!
std::shared_ptr<AccessInterfaceTranslator> Create_TestCase_Brocade (SystemModel&                        sm,
                                                shared_ptr<AccessInterfaceProtocol> masterProtocol,
                                                shared_ptr<AccessInterfaceProtocol> slaveProtocol,
                                                bool                                reportGml = false)
{
  SystemModelBuilder builder(sm);

  auto tap1 = builder.Create_JTAG_TAP("Zybo", 6u, 2u, make_shared<LoopbackAccessInterfaceProtocol>());
  auto tap2 = builder.Create_JTAG_TAP("Tap",  6u, 3u, make_shared<LoopbackAccessInterfaceProtocol>());
  auto tap3 = builder.Create_JTAG_TAP("TAP3", 6u, 4u, make_shared<LoopbackAccessInterfaceProtocol>());
  auto tap4 = builder.Create_JTAG_TAP("",     6u, 2u, make_shared<LoopbackAccessInterfaceProtocol>());

  builder.AppendRegisters(1u, "reg_", BinaryVector::CreateFromString("0x123"), tap1);
  builder.AppendRegisters(2u, "R_",   BinaryVector::CreateFromString("0x45"),  tap2);
  builder.AppendRegisters(3u, "reg_", BinaryVector::CreateFromString("0x678"), tap3);
  builder.AppendRegisters(1u, "reg_", BinaryVector::CreateFromString("0x9A"),  tap4);

  auto taps = { tap1, tap2, tap3, tap4 };
  auto root = builder.Create_Brocade(masterProtocol, slaveProtocol, taps);

  auto reg_16 = sm.RegisterWithId(16u);  // tap1
  auto reg_17 = sm.RegisterWithId(17u);  // tap2
  auto reg_18 = sm.RegisterWithId(18u);  // tap2
  auto reg_19 = sm.RegisterWithId(19u);  // tap3
  auto reg_20 = sm.RegisterWithId(20u);  // tap3
  auto reg_21 = sm.RegisterWithId(21u);  // tap3
  auto reg_22 = sm.RegisterWithId(22u);  // tap4

  reg_16->SetToSut (BinaryVector::CreateFromString("0x040")); // tap1
  reg_17->SetToSut (BinaryVector::CreateFromString("0x90"));  // tap2
  reg_18->SetToSut (BinaryVector::CreateFromString("0x91"));  // tap2
  reg_19->SetToSut (BinaryVector::CreateFromString("0x671")); // tap3
  reg_20->SetToSut (BinaryVector::CreateFromString("0x672")); // tap3
  reg_21->SetToSut (BinaryVector::CreateFromString("0x673")); // tap3
  reg_22->SetToSut (BinaryVector::CreateFromString("0x9B"));  // tap4

  reg_16->SetBypass (BinaryVector::CreateFromString("0x111")); // tap1
  reg_17->SetBypass (BinaryVector::CreateFromString("0x22"));  // tap2
  reg_18->SetBypass (BinaryVector::CreateFromString("0x23"));  // tap2
  reg_19->SetBypass (BinaryVector::CreateFromString("0x331")); // tap3
  reg_20->SetBypass (BinaryVector::CreateFromString("0x332")); // tap3
  reg_21->SetBypass (BinaryVector::CreateFromString("0x333")); // tap3
  reg_22->SetBypass (BinaryVector::CreateFromString("0xAA"));  // tap4

  if (reportGml)
  {
    TS_TRACE (GmlPrinter::Graph(root, "Brocade"));
  }

  sm.ReplaceRoot(root, false);

  return root;
}
//
//  End of: Create_TestCase_Brocade
//---------------------------------------------------------------------------

//! Creates test case "Brocade"
//!
std::shared_ptr<AccessInterfaceTranslator> Create_TestCase_Brocade (SystemModel&                        sm,
                                                shared_ptr<AccessInterfaceTranslatorProtocol>  TopProtocol,
                                                shared_ptr<AccessInterfaceProtocol> masterProtocol,
                                                shared_ptr<AccessInterfaceProtocol> slaveProtocol,
                                                bool                                reportGml = false)
{
  SystemModelBuilder builder(sm);

  auto tap1 = builder.Create_JTAG_TAP("Zybo", 6u, 2u, slaveProtocol);
  auto tap2 = builder.Create_JTAG_TAP("Tap",  6u, 3u, slaveProtocol);
  auto tap3 = builder.Create_JTAG_TAP("TAP3", 6u, 4u, slaveProtocol);
  auto tap4 = builder.Create_JTAG_TAP("",     6u, 2u, slaveProtocol);

  builder.AppendRegisters(1u, "reg_", BinaryVector::CreateFromString("0x123"), tap1);
  builder.AppendRegisters(2u, "R_",   BinaryVector::CreateFromString("0x45"),  tap2);
  builder.AppendRegisters(3u, "reg_", BinaryVector::CreateFromString("0x678"), tap3);
  builder.AppendRegisters(1u, "reg_", BinaryVector::CreateFromString("0x9A"),  tap4);

  auto taps = { tap1, tap2, tap3, tap4 };
  auto root = builder.Create_Brocade(TopProtocol,masterProtocol, slaveProtocol, taps);


  auto reg_16 = sm.RegisterWithId(16u);  // tap1
  auto reg_17 = sm.RegisterWithId(17u);  // tap2
  auto reg_18 = sm.RegisterWithId(18u);  // tap2
  auto reg_19 = sm.RegisterWithId(19u);  // tap3
  auto reg_20 = sm.RegisterWithId(20u);  // tap3
  auto reg_21 = sm.RegisterWithId(21u);  // tap3
  auto reg_22 = sm.RegisterWithId(22u);  // tap4

  reg_16->SetToSut (BinaryVector::CreateFromString("0x040")); // tap1
  reg_17->SetToSut (BinaryVector::CreateFromString("0x90"));  // tap2
  reg_18->SetToSut (BinaryVector::CreateFromString("0x91"));  // tap2
  reg_19->SetToSut (BinaryVector::CreateFromString("0x671")); // tap3
  reg_20->SetToSut (BinaryVector::CreateFromString("0x672")); // tap3
  reg_21->SetToSut (BinaryVector::CreateFromString("0x673")); // tap3
  reg_22->SetToSut (BinaryVector::CreateFromString("0x9B"));  // tap4

  reg_16->SetBypass (BinaryVector::CreateFromString("0x111")); // tap1
  reg_17->SetBypass (BinaryVector::CreateFromString("0x22"));  // tap2
  reg_18->SetBypass (BinaryVector::CreateFromString("0x23"));  // tap2
  reg_19->SetBypass (BinaryVector::CreateFromString("0x331")); // tap3
  reg_20->SetBypass (BinaryVector::CreateFromString("0x332")); // tap3
  reg_21->SetBypass (BinaryVector::CreateFromString("0x333")); // tap3
  reg_22->SetBypass (BinaryVector::CreateFromString("0xAA"));  // tap4

  if (reportGml)
  {
    TS_TRACE (GmlPrinter::Graph(root, "Brocade"));
  }

  sm.ReplaceRoot(root, false);

  return root;
}
//
//  End of: Create_TestCase_Brocade
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

  auto endpointAction = [](const std::vector<Primitive>& primitives, uint32_t endpointId, void* /* data */, const BinaryVector& toSutData)
  {
    BinaryVector   fromSutData;
    PrimitiveParam param(toSutData, fromSutData);

    primitives[endpointId](&param);

    return param.fromSutData;
  };

  auto action_1 = [endpointAction](const std::vector<Primitive>& primitives, void* data, const BinaryVector& toSutData)
  {
    return endpointAction(primitives, 1u, data, toSutData);
  };

  auto action_2 = [endpointAction](const std::vector<Primitive>& primitives, void* data, const BinaryVector& toSutData)
  {
    return endpointAction(primitives, 2u, data, toSutData);
  };

  vector<Primitive> primitives = {primitive_0, primitive_1, primitive_2};
  vector<Action>    actions    = {action_0,    action_1,    action_2};

  auto protocol = make_shared<GenericAccessInterfaceProtocol>(actions, primitives);
  return protocol;
}
//
//  End of: CreateSut
//---------------------------------------------------------------------------

//! Checks SystemModelManager::iWrite() using same thread as SystemModelManager
//!
template<typename T> void Check_iWrite_SingleThread (T value, string_view expected)
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto reg  = sm.RegisterWithId(7u);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.iWrite("dynamic_1", value));

  // ---------------- Verify
  //
  TS_ASSERT_THROWS_NOTHING (sut.iApply());
  auto expectedNextToSut = BinaryVector::CreateFromHexString(expected);

  TS_ASSERT_EQUALS (reg->NextToSut(), expectedNextToSut);
}
//
//  End of: Check_iWrite_SingleThread
//---------------------------------------------------------------------------


//! Checks SystemModelManager::iGet() using same thread as SystemModelManager
//!
template<typename T> void Check_iGet_SingleThread (string_view initialValue, T expected)
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto reg  = sm.RegisterWithId(7u);
  reg->SetPendingForRead();
  reg->SetFromSut(BinaryVector::CreateFromHexString(initialValue));

  SystemModelManager sut(sm);
  T lastFromSut;

  // ---------------- Exercise
  //
  sut.iGet("dynamic_1", lastFromSut);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (lastFromSut, expected);
}


//! Checks SystemModelManager::iRead()
//!
//! @param iReadValue         The value passed to iRead
//! @param iWriteValue        The value passed to iWrite
//! @param expectedMismatch   Expected mismatch (checked with 40 bits register)
//!
template<typename T> void Check_iRead_SingleThread (T iReadValue, T iWriteValue, string_view expectedMismatch)
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm, false, 40u);
  SystemModelManager sut(sm);

  auto regPath = "dynamic_2";

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.iRead(regPath, iReadValue));

  // ---------------- Verify
  //
  sut.iWrite (regPath, iWriteValue); // Loopback (default protocol) will force FromSut to be updated
  sut.iApply();

  auto xorResult         = sut.iGetMiscompares (regPath);
  auto status            = sut.iGetStatus      (regPath, false);
  auto expectedXorResult = BinaryVector::CreateFromHexString(expectedMismatch);

  TS_ASSERT_EQUALS (status,    1u);
  TS_ASSERT_EQUALS (xorResult, expectedXorResult);   // 40 bits reg
}


//! Checks SystemModelManager::iRead() with a don't care value in case there is a mismatch
//!
//! @param iReadExpectedValue The expected value passed to iRead
//! @param iReadDontCareValue The don't care value passed to iRead
//! @param iWriteValue        The value passed to iWrite
//! @param expectedMismatch   Expected mismatch (checked with 40 bits register)
//!
template<typename T> void Check_iRead_DontCare_SingleThread (T           iReadExpectedValue,
                                                             T           iReadDontCareValue,
                                                             T           iWriteValue,
                                                             string_view expectedMismatch)
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm, false, 40u);
  SystemModelManager sut(sm);

  auto regPath = "dynamic_2";

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.iRead(regPath, iReadExpectedValue, iReadDontCareValue));

  // ---------------- Verify
  //
  sut.iWrite (regPath, iWriteValue); // Loopback (default protocol) will force FromSut to be updated
  sut.iApply();

  auto xorResult         = sut.iGetMiscompares (regPath);
  auto status            = sut.iGetStatus      (regPath, false);
  auto expectedXorResult = BinaryVector::CreateFromHexString(expectedMismatch);

  TS_ASSERT_EQUALS (status,    1u);
  TS_ASSERT_EQUALS (xorResult, expectedXorResult);   // 40 bits reg
}


//! Checks SystemModelManager::iGetRefresh() with 4 application threads accessing multiple times their own register
//!
//! @note It just check that application and manager threads are not blocked
template<typename T> void Check_iGetRefresh ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto mux  = sm.LinkerWithId(2u);   // This is Tap mux
  auto reg_0  = sm.RegisterWithId(6u); reg_0->SetHoldValue(true); // Hold is to maintain the same value (nextToSut become also bypass)
  auto reg_1  = sm.RegisterWithId(7u); reg_1->SetHoldValue(true);
  auto reg_2  = sm.RegisterWithId(8u); reg_2->SetHoldValue(true);
  auto reg_3  = sm.RegisterWithId(9u); reg_3->SetHoldValue(true);

  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut](string_view regName)
  {
    TS_ASSERT_THROWS_NOTHING
    (
      T gotData;

      for (int ii = 0 ; ii < 8 ; ++ii)
      {
        sut.iGetRefresh(regName, gotData);
      }
    );
  };

  // ---------------- Setup (main thread)
  //
  sut.CreateApplicationThread(mux, [appFunctor]() { appFunctor("dynamic_0"); }, "App_0");
  sut.CreateApplicationThread(mux, [appFunctor]() { appFunctor("dynamic_1"); }, "App_1");
  sut.CreateApplicationThread(mux, [appFunctor]() { appFunctor("dynamic_2"); }, "App_2");
  sut.CreateApplicationThread(mux, [appFunctor]() { appFunctor("dynamic_3"); }, "App_3");
  sut.Start();
  sut.StartCreatedApplicationThreads();
  sut.WaitForApplicationsEnd();              // Make sure applications have done their action
  sut.Stop();
}

} // End of unnamed namespace


//! Initializes test (called for each test)
void UT_SystemModelManager::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  #if defined(LOG_TEST_DURATION)
  g_testStartTime = std::chrono::steady_clock::now();
  #endif

  SystemModelNode::ResetNodeIdentifier();
}

//! Cleanups test (called for each test)
void UT_SystemModelManager::tearDown ()
{
  #if defined(LOG_TEST_DURATION)
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - g_testStartTime).count();
  ENABLE_LOG_IN_SCOPE;
  LOG(INFO) << std::setw(3) << duration << " ms for: UT_SystemModelManager::" << TS_NAME;
  #endif
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
  TS_ASSERT_THROWS_NOTHING (SystemModelManager sut(sm));
}

//! Checks SystemModelManager constructor when there are no node in system model
//!
void UT_SystemModelManager::test_Constructor_SystemModel_Without_AI ()
{
  // ---------------- Setup
  //
  SystemModel      sm;
  TestModelBuilder builder(sm);

  auto root = builder.Create_Simple_MIB();

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS_NOTHING (SystemModelManager sut(sm));
}

//! Checks SystemModelManager constructor when root is an AccessInterface
//!
void UT_SystemModelManager::test_Constructor_Root_is_AI ()
{
  // ---------------- Setup
  //
  SystemModel        sm;
  TestModelBuilder builder(sm);

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
  TestModelBuilder builder(sm);

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
  TestModelBuilder builder(sm);

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


//! Checks SystemModelManager::Start when there is AccessInterface in system model
//!
void UT_SystemModelManager::test_Start_SystemModel_Without_AI ()
{
  // ---------------- Setup
  //
  SystemModel      sm;
  TestModelBuilder builder(sm);

  auto root = builder.Create_Simple_MIB();
  SystemModelManager sut(sm);

  // ---------------- Exercise & Verify
  //
  TS_ASSERT_THROWS (sut.Start(), std::exception);
}


//! Checks SystemModelManager DoDataCycles when using AccessInterface testcase
//!
void UT_SystemModelManager::test_DoDataCycles_AccessInterface ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

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

//+  TS_TRACE (GmlPrinter::Graph(ai, "Testcase_AccessInterface"));

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

//! Checks SystemModelManager DoDataCycles when using AccessInterfaceTranslator testcase
//!
void UT_SystemModelManager::test_DoDataCycles_AccessInterfaceTranslator ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto at    = builder.Create_TestCase_Emulation_Translator("Emulator");
  auto ir    = sm.RegisterWithId(1u);
  auto bpy   = sm.RegisterWithId(3u);
  auto reg_1 = sm.RegisterWithId(5u);
  auto reg_2 = sm.RegisterWithId(7u);

  reg_1->SetToSut   (BinaryVector(STATIC_TDR_LEN, 0x0A));
  reg_2->SetToSut   (BinaryVector(STATIC_TDR_LEN, 0xB0));
  reg_1->SetBypass  (BinaryVector(STATIC_TDR_LEN, 0x41));
  reg_2->SetBypass  (BinaryVector(STATIC_TDR_LEN, 0x42));

//+  TS_TRACE (GmlPrinter::Graph(ai, "Testcase_AccessInterface"));

  SystemModelManager sut(sm);


  auto result = SystemModelChecker::Check(sm);
//  TS_TRACE (result.MakeReport());
TS_ASSERT_FALSE (result.HasIssues());

  CxxTest::setAbortTestOnFail(true);
//  auto gotPretty      = PrettyPrinter::PrettyPrint(sm.Root(), PrettyPrinterOptions::All);
//  TS_ASSERT_EQUALS (gotPretty, "");

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  auto gotCommands = dynamic_pointer_cast<Spy_Emulation_Translator>(at->Protocol())->Commands();

  std::vector<std::string> expectedCommands
  {
    "SIR 8 TDI(02);",   // 00 : IR
    "SDR 16 TDI(B0B0);", // 01 : DR (reg_2)
    "SIR 8 TDI(01);",   // 02 : IR
    "SDR 16 TDI(0A0A);", // 03 : DR (reg_1)
    "SIR 8 TDI(FF);",   // 04 : IR
  };

  TS_ASSERT_EQUALS (gotCommands, expectedCommands);
}


//! Checks SystemModelManager DoDataCycles when using AccessInterfaceTranslator 
//! and two AccessInterfaces
//!
void UT_SystemModelManager::test_DoDataCycles_DoubleAccessInterfaceTranslator ()
{
  /* Manually-built testcase causes system to hang : should put a Smoke Test instead?
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto at    = builder.Create_TestCase_Emulation_Translator();
  auto ir    = sm.RegisterWithId(1u);
  auto bpy   = sm.RegisterWithId(3u);
  auto reg_1 = sm.RegisterWithId(5u);
  auto reg_2 = sm.RegisterWithId(7u);

  reg_1->SetToSut   (BinaryVector(STATIC_TDR_LEN, 0x0A));
  reg_2->SetToSut   (BinaryVector(STATIC_TDR_LEN, 0xB0));
  reg_1->SetBypass  (BinaryVector(STATIC_TDR_LEN, 0x41));
  reg_2->SetBypass  (BinaryVector(STATIC_TDR_LEN, 0x42));


  auto second_tap     = builder.Create_JTAG_TAP    ("second_Tap",       DEFAULT_IR_LEN, 3u,make_shared<SVF_RawPlayer>());

  at->RegisterInterface(second_tap);

  auto second_chain_1 = sm.CreateChain    ("second_sut_1",    second_tap);
  auto second_reg_1   = sm.CreateRegister ("second_static_1", BinaryVector(STATIC_TDR_LEN, 0), second_chain_1);

  auto second_chain_2 = sm.CreateChain    ("second_sut_2",    second_tap);
  auto second_reg_2   = sm.CreateRegister ("second_static_2", BinaryVector(STATIC_TDR_LEN, 0), second_chain_2);

  second_reg_1->SetToSut   (BinaryVector(STATIC_TDR_LEN, 0x0A));
  second_reg_2->SetToSut   (BinaryVector(STATIC_TDR_LEN, 0xB0));
  second_reg_1->SetBypass  (BinaryVector(STATIC_TDR_LEN, 0x41));
  second_reg_2->SetBypass  (BinaryVector(STATIC_TDR_LEN, 0x42));

  at->RegisterInterface(second_tap);

  at->AppendChild(second_tap);

//+  TS_TRACE (GmlPrinter::Graph(ai, "Testcase_AccessInterface"));

  SystemModelManager sut(sm);

//  auto gotPretty      = PrettyPrinter::PrettyPrint(sm.Root(), PrettyPrinterOptions::All);
//  TS_ASSERT_EQUALS (gotPretty, "");

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  auto gotCommands = dynamic_pointer_cast<Spy_Emulation_Translator>(at->Protocol())->Commands();
 
  //NB: Commands from the two taps are mixed together
  std::vector<std::string> expectedCommands
  {
    "SIR 8 TDI(02);",   // 00 : IR
    "SDR 16 TDI(B0B0);", // 01 : DR (reg_2)
    "SIR 8 TDI(02);",   // 00 : second_IR
    "SDR 16 TDI(B0B0);", // 01 : second_DR (reg_2)
    "SIR 8 TDI(01);",   // 02 : IR
    "SDR 16 TDI(0A0A);", // 03 : DR (reg_1)
    "SIR 8 TDI(01);",   // 02 : second_IR
    "SDR 16 TDI(0A0A);", // 03 : second_DR (reg_1)
    "SIR 8 TDI(FF);",   // 04 : IR
    "SIR 8 TDI(FF);",   // 04 : second_IR
  };

  TS_ASSERT_EQUALS (gotCommands, expectedCommands);
  */
}

//! Checks SystemModelManager DoDataCycles when using AccessInterfaceTranslator testcase
//!
void UT_SystemModelManager::test_DoDataCycles_I2C_AccessInterfaceTranslator ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto at    = builder.Create_TestCase_I2C_Emulation_Translator();


  auto reg_1 = sm.RegisterWithId(1u);
  auto reg_2 = sm.RegisterWithId(2u);
  auto reg_3 = sm.RegisterWithId(3u);

  reg_1->SetToSut   (BinaryVector( reg_1->BitsCount(), 0xA0));
  reg_2->SetToSut   (BinaryVector(reg_2->BitsCount(), 0xB0));
  reg_3->SetToSut   (BinaryVector(reg_3->BitsCount(), 0xC0));
  reg_1->SetBypass  (BinaryVector(reg_1->BitsCount(), 0x41));
  reg_2->SetBypass  (BinaryVector(reg_2->BitsCount(), 0x42));
  reg_3->SetBypass  (BinaryVector(reg_3->BitsCount(), 0x43));

//+  TS_TRACE (GmlPrinter::Graph(ai, "Testcase_AccessInterface"));

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  auto gotCommands = dynamic_pointer_cast<Spy_Emulation_Translator>(at->Protocol())->Commands();

  std::vector<std::string> expectedCommands
  {
    "I2C_READ (0x31);",
    "I2C_WRITE (0x31, 0b1010);",
    "I2C_READ (0x32);",
    "I2C_WRITE (0x32, 0b1011);",
    "I2C_READ (0x33);",
    "I2C_WRITE (0x33, 0b1100);",
  };

  TS_ASSERT_EQUALS (gotCommands, expectedCommands);
}

//! Checks SystemModelManager DoDataCycles when using Brocade testcase with two Raw Protocols and one translator
//!
void UT_SystemModelManager::test_DoDataCycles_BrocadeTranslator ()
{
/* the Brocade Translator as defined in the SystemModelBuilder is not supported by the P1687.1 implementation
  IT would require a complete rework to correctly register the Raw protocols
   ...maybe by using a Dummy Translator as top node instead of a chain?
   
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto addresses = { 0x00u, 0x41u, 0x42u };
  auto topProtocol = make_shared<Spy_Emulation_Translator>();
  auto masterProtocol = make_shared<I2C_RawPlayer>(addresses);
  auto slaveProtocol  = make_shared<SVF_RawPlayer>();


  auto root = Create_TestCase_Brocade(sm, topProtocol,masterProtocol, slaveProtocol);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //

  // ---------------- Verify
  //
  auto gotCommands = topProtocol->Commands();

  vector<string> expected
  {
   "S2R I2C_READ(0x41)",
   "S2R I2C_WRITE(0x41, 0x0F)",
   "SIR 24 TDI(0420C1);",
   "SDR 40 TDI(040916739B);",
   "S2R I2C_READ(0x41)",
   "S2R I2C_WRITE(0x41, 0x00)",
   "S2R I2C_READ(0x41)",
   "S2R I2C_WRITE(0x41, 0x0F)",
   "SIR 24 TDI(FC10BF);",
   "SDR 22 TDI(320CE5);",
   "S2R I2C_READ(0x41)",
   "S2R I2C_WRITE(0x41, 0x00)",
   "S2R I2C_READ(0x41)",
   "S2R I2C_WRITE(0x41, 0x06)",
   "SIR 12 TDI(0FC1);",
   "SDR 13 TDI(1671);",
   "S2R I2C_READ(0x41)",
   "S2R I2C_WRITE(0x41, 0x00)",
   "S2R I2C_READ(0x41)",
   "S2R I2C_WRITE(0x41, 0x04)",
   "SIR 6 TDI(3F);",
   "S2R I2C_READ(0x41)",
   "S2R I2C_WRITE(0x41, 0x00)",
  };

//  TS_ASSERT_EQUALS (gotCommands, expected);

 */
}

//! Checks SystemModelManager DoDataCycles when using a JTAG_to_I2C_Translator
//!
void UT_SystemModelManager::test_DoDataCycles_JTAG_to_I2C_Translator ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto at    = builder.Create_TestCase_JTAG_to_I2C_Translator();

  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_NOT_NULLPTR (at);

  // Model coherency
  auto result = SystemModelChecker::Check(sm);
//  TS_TRACE (result.MakeReport());
TS_ASSERT_FALSE (result.HasIssues());
  
  
//  auto gotPretty      = PrettyPrinter::PrettyPrint(sm.Root(), PrettyPrinterOptions::All);
//  auto expectedPretty = string("");
//  TS_ASSERT_EQUALS (gotPretty, expectedPretty);

  auto ir    = sm.RegisterWithId(1u);
  auto bpy   = sm.RegisterWithId(3u);
  auto reg_1 = sm.RegisterWithId(5u);
  auto reg_2 = sm.RegisterWithId(7u);

  reg_1->SetToSut   (BinaryVector(STATIC_TDR_LEN, 0x0A));
  reg_2->SetToSut   (BinaryVector(STATIC_TDR_LEN, 0xB0));
  reg_1->SetBypass  (BinaryVector(STATIC_TDR_LEN, 0x41));
  reg_2->SetBypass  (BinaryVector(STATIC_TDR_LEN, 0x42));


  SystemModelManager sut(sm);


  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  auto gotCommands = dynamic_pointer_cast<Spy_Emulation_Translator>(at->Protocol())->Commands();

  std::vector<std::string> expectedCommands
  {
    "I2C_READ (0x31);",   // 00 : IR
    "I2C_WRITE (0x31, 0x02);",   // 00 : IR
    "I2C_READ (0x32);", // 01 : DR (reg_2)
    "I2C_WRITE (0x32, 0xB0B0);", // 01 : DR (reg_2)
    "I2C_READ (0x31);",   // 02 : IR
    "I2C_WRITE (0x31, 0x01);",   // 02 : IR
    "I2C_READ (0x32);", // 03 : DR (reg_1)
    "I2C_WRITE (0x32, 0x0A0A);", // 03 : DR (reg_1)
    "I2C_READ (0x31);",   // 04 : IR
    "I2C_WRITE (0x31, 0xFF);",   // 04 : IR
  };

  TS_ASSERT_EQUALS (gotCommands, expectedCommands);
}

//! Checks SystemModelManager DoDataCycles when using a JTAG_Bitbang_Translator
//!
void UT_SystemModelManager::test_DoDataCycles_JTAG_BitBang_Translator ()
{
  TS_ASSERT_EQUALS ("1", "1"); //Useless Assert to avoid warning
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
     "SIR 8 TDI(01);",                   // 00
     "SDR 18 TDI(001416);",              // 01
     "SDR 20 TDI(036364);",              // 02
     "SDR 49 TDI(006C6DC0C0C0C0);",      // 03
     "SDR 20 TDI(036363);",              // 04
     "SDR 49 TDI(006C6DB0B0B0B0);",      // 05
     "SDR 20 TDI(036362);",              // 06
     "SDR 49 TDI(006C6D0A0A0A0A);",      // 07
     "SDR 20 TDI(036361);",              // 08
     "SDR 49 TDI(006C6C09090909);",      // 09
     "SDR 49 TDI(006C6D40404040);",      // 10
     "SDR 20 TDI(036368);",              // 11
     "SDR 20 TDI(036360);",              // 12
     "SIR 8 TDI(FF);",                   // 13
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
    "S2R I2C_READ(0x41)",
    "S2R I2C_WRITE(0x41, 0x01)",
    "S2R I2C_READ(0x42)",
    "S2R I2C_WRITE(0x42, 0x0505_/b10)",
    "S2R I2C_READ(0x42)",
    "S2R I2C_WRITE(0x42, 0x3636_4)",
    "S2R I2C_READ(0x42)",
    "S2R I2C_WRITE(0x42, 0x3636_E060:6060_/b0)",
    "S2R I2C_READ(0x42)",
    "S2R I2C_WRITE(0x42, 0x3636_3)",
    "S2R I2C_READ(0x42)",
    "S2R I2C_WRITE(0x42, 0x3636_D858:5858_/b0)",
    "S2R I2C_READ(0x42)",
    "S2R I2C_WRITE(0x42, 0x3636_2)",
    "S2R I2C_READ(0x42)",
    "S2R I2C_WRITE(0x42, 0x3636_8505:0505_/b0)",
    "S2R I2C_READ(0x42)",
    "S2R I2C_WRITE(0x42, 0x3636_1)",
    "S2R I2C_READ(0x42)",
    "S2R I2C_WRITE(0x42, 0x3636_0484:8484_/b1)",
    "S2R I2C_READ(0x42)",
    "S2R I2C_WRITE(0x42, 0x3636_A020:2020_/b0)",
    "S2R I2C_READ(0x42)",
    "S2R I2C_WRITE(0x42, 0x3636_8)",
    "S2R I2C_READ(0x42)",
    "S2R I2C_WRITE(0x42, 0x3636_0)",
    "S2R I2C_READ(0x41)",
    "S2R I2C_WRITE(0x41, 0xFF)"
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
//+  monitor->ExportBasePath("1500_Lazy");
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
//+  monitor->ExportBasePath("MIB_Multichain_Pre_Greedy");
//+  SystemModelManager sut(sm, configureAlgo, monitor);
//+   ENABLE_LOG_IN_SCOPE;

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


//! Checks SystemModelManager DoDataCycles when using "Brocade" testcase
//!
void UT_SystemModelManager::test_DoDataCycles_Brocade ()
{
/* the Brocade Translator as defined in the SystemModelBuilder is not supported by the P1687.1 implementation
  IT would require a complete rework to correctly register the Raw protocols
   ...maybe by using a Dummy Translator as top node instead of a chain?

  // ---------------- Setup
  //
  SystemModel sm;

  auto spiedCommands = make_shared<SpiedProtocolsCommands>();

  auto addresses = { 0x00u, 0x41u, 0x42u };

  auto topProtocol = make_shared<Spy_Emulation_Translator>();
  auto masterProtocol = make_shared<I2C_RawPlayer>(addresses);
  auto slaveProtocol  = make_shared<SVF_RawPlayer>();


  auto root = Create_TestCase_Brocade(sm, topProtocol,masterProtocol, slaveProtocol);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.DoDataCycles());

  // ---------------- Verify
  //
  const auto& gotCommands = spiedCommands->Commands();

  vector<string> expected
  {
   "S2R I2C_READ(0x41)",
   "S2R I2C_WRITE(0x41, 0x0F)",
   "SIR 24 TDI(0420C1);",
   "SDR 40 TDI(040916739B);",
   "S2R I2C_READ(0x41)",
   "S2R I2C_WRITE(0x41, 0x00)",
   "S2R I2C_READ(0x41)",
   "S2R I2C_WRITE(0x41, 0x0F)",
   "SIR 24 TDI(FC10BF);",
   "SDR 22 TDI(320CE5);",
   "S2R I2C_READ(0x41)",
   "S2R I2C_WRITE(0x41, 0x00)",
   "S2R I2C_READ(0x41)",
   "S2R I2C_WRITE(0x41, 0x06)",
   "SIR 12 TDI(0FC1);",
   "SDR 13 TDI(1671);",
   "S2R I2C_READ(0x41)",
   "S2R I2C_WRITE(0x41, 0x00)",
   "S2R I2C_READ(0x41)",
   "S2R I2C_WRITE(0x41, 0x04)",
   "SIR 6 TDI(3F);",
   "S2R I2C_READ(0x41)",
   "S2R I2C_WRITE(0x41, 0x00)",
  };

  TS_ASSERT_EQUALS (gotCommands, expected);
  */
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
    TS_ASSERT_EQUALS (sum,          0);  // Thread is waiting for start signal ==> sum does not change
    TS_ASSERT_EQUALS (value.load(), 1u); // Value is also not changed
    sut.StartCreatedApplicationThreads();
    while (!started){std::this_thread::sleep_for(0ms);}  // Wait for start signal being seen by application thread function

    for (uint32_t ii = 2u ; ii <= 100u ; ++ii)
    {
      while (value != 0);
      value = ii;
    }
    sut.WaitForApplicationsEnd();  // Make sure the application thread has seen the last value

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

  ENABLE_LOG_IN_SCOPE;
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
    sut.WaitForApplicationsEnd();      // Make sure application have done their job

    TS_ASSERT_EQUALS (valueApp_1.load(), 13u);
    TS_ASSERT_EQUALS (valueApp_2.load(), 37u);
  }
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
  Create_TestCase_ForIPrefix(sm);

  auto prefix = "Chain_1";
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
  Create_TestCase_ForIPrefix(sm);
  auto mux  = sm.LinkerWithId(2u);   // This is Tap mux

  SystemModelManager sut(sm);

  auto   prefix = "Chain_2";
  string gotPrefix;

  auto appFunctor = [prefix, &gotPrefix, &sut]()
  {
    // ---------------- Exercise (set & get)
    //
    TS_ASSERT_THROWS_NOTHING (sut.iPrefix(prefix));
    TS_ASSERT_THROWS_NOTHING (gotPrefix = sut.iPrefix());
  };

//+   ENABLE_LOG_IN_SCOPE;
  sut.CreateApplicationThread(mux, appFunctor); // Include "Exercise" in created thread
  sut.StartCreatedApplicationThreads();

  // ---------------- Verify
  //
  sut.WaitForApplicationsEnd();  // Make sure application has done its action

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
void UT_SystemModelManager::test_iGet_SingleThread_BinaryVector () { Check_iGet_SingleThread("ABCD_4567", BinaryVector::CreateFromHexString("ABCD_4567")); }
void UT_SystemModelManager::test_iGet_SingleThread_uint8        () { Check_iGet_SingleThread("0000_007B", uint8_t(123));          }
void UT_SystemModelManager::test_iGet_SingleThread_uint16       () { Check_iGet_SingleThread("0000_3039", uint16_t(12345));       }
void UT_SystemModelManager::test_iGet_SingleThread_uint32       () { Check_iGet_SingleThread("0012_D687", uint32_t(1234567L));    }
void UT_SystemModelManager::test_iGet_SingleThread_uint64       () { Check_iGet_SingleThread("075B_CD15", uint64_t(123456789LL)); }
void UT_SystemModelManager::test_iGet_SingleThread_int8         () { Check_iGet_SingleThread("FFFF_FF85", int8_t(-123));          }
void UT_SystemModelManager::test_iGet_SingleThread_int16        () { Check_iGet_SingleThread("FFFF_CFC7", int16_t(-12345));       }
void UT_SystemModelManager::test_iGet_SingleThread_int32        () { Check_iGet_SingleThread("FFED_2979", int32_t(-1234567L));    }
void UT_SystemModelManager::test_iGet_SingleThread_int64        () { Check_iGet_SingleThread("F8A4_32EB", int64_t(-123456789LL)); }

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
  reg->SetPendingForRead();
  reg->SetFromSut(BinaryVector::CreateFromHexString("ABCD_0123"));

  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut]()
  {
    // ---------------- Exercise
    //
    auto lastFromSut = sut.iGet("dynamic_1");

    // ---------------- Verify
    //
    CxxTest::setStringResultsOnNewLine(false);
    auto expected = BinaryVector::CreateFromHexString("ABCD_0123");
    TS_ASSERT_EQUALS (lastFromSut, expected);
  };

//+   ENABLE_LOG_IN_SCOPE;
  sut.CreateApplicationThread(mux, appFunctor); // Include "Exercise" in created thread
  sut.StartCreatedApplicationThreads();

  // ---------------- Verify
  //
  sut.WaitForApplicationsEnd();  // Make sure application has done its action
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
//+   ENABLE_LOG_IN_SCOPE;
  auto unkwnownThread = std::thread(appFunctor);
  unkwnownThread.join();

  // Wait end of thread
  sut.WaitForApplicationsEnd();  // Make sure application has done its action
}


//! Checks SystemModelManager::iWrite() using same thread as SystemModelManager
//!
void UT_SystemModelManager::test_iWrite_SingleThread_BinaryVector () { Check_iWrite_SingleThread(BinaryVector::CreateFromHexString("ABCD_4567"), "ABCD_4567"); }
void UT_SystemModelManager::test_iWrite_SingleThread_uint8        () { Check_iWrite_SingleThread(uint8_t(123),          "0000_007B"); }
void UT_SystemModelManager::test_iWrite_SingleThread_uint16       () { Check_iWrite_SingleThread(uint16_t(12345),       "0000_3039"); }
void UT_SystemModelManager::test_iWrite_SingleThread_uint32       () { Check_iWrite_SingleThread(uint32_t(1234567L),    "0012_D687"); }
void UT_SystemModelManager::test_iWrite_SingleThread_uint64       () { Check_iWrite_SingleThread(uint64_t(123456789LL), "075B_CD15"); }
void UT_SystemModelManager::test_iWrite_SingleThread_int8         () { Check_iWrite_SingleThread(int8_t(-123),          "FFFF_FF85"); }
void UT_SystemModelManager::test_iWrite_SingleThread_int16        () { Check_iWrite_SingleThread(int16_t(-12345),       "FFFF_CFC7"); }
void UT_SystemModelManager::test_iWrite_SingleThread_int32        () { Check_iWrite_SingleThread(int32_t(-1234567L),    "FFED_2979"); }
void UT_SystemModelManager::test_iWrite_SingleThread_int64        () { Check_iWrite_SingleThread(int64_t(-123456789LL), "F8A4_32EB"); }



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

//+   ENABLE_LOG_IN_SCOPE;
  {
    SystemModelManager sut(sm);

    // Thread functor
    auto appFunctor = [&sut]()
    {
      // ---------------- Setup
      //
      auto nextToSut = BinaryVector::CreateFromHexString("FADE_CAFE");

      // ---------------- Exercise
      //
      TS_ASSERT_THROWS_NOTHING (sut.iWrite("dynamic_3", nextToSut));
      TS_ASSERT_THROWS_NOTHING (sut.iApply());
    };

    // ---------------- Setup (main thread)
    //
    sut.CreateApplicationThread(mux, appFunctor); // Include "Exercise" in created thread
    sut.StartCreatedApplicationThreads();
    sut.WaitForApplicationsEnd();  // Make sure application has done its action

    // ---------------- Verify
    //
    auto expected = BinaryVector::CreateFromHexString("FADE_CAFE");
    TS_ASSERT_EQUALS (reg->NextToSut(), expected);
  }
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


//! Checks SystemModelManager::iWrite() using register alias
//!
void UT_SystemModelManager::test_iWrite_Thread_WithAlias ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto mux      = sm.LinkerWithId(2u);   // This is Tap mux
  auto regDyn_0 = sm.RegisterWithId(6u); // 32 bits initialized with 0x60606060
  auto regDyn_1 = sm.RegisterWithId(7u); // 32 bits initialized with 0x61616161
  auto regDyn_2 = sm.RegisterWithId(8u); // 32 bits initialized with 0x62626262
  auto regDyn_3 = sm.RegisterWithId(9u); // 32 bits initialized with 0x63636363

  VirtualRegister virtualRegister;
  virtualRegister.Append({regDyn_1, IndexedRange{31, 24}});
  virtualRegister.Append({regDyn_2, IndexedRange{23, 16}});
  virtualRegister.Append({regDyn_3, IndexedRange{15, 8}});
  mux->AddAlias({"Foo"s, std::move(virtualRegister)});

//+   ENABLE_LOG_IN_SCOPE;
  {
    SystemModelManager sut(sm);

    // Thread functor
    auto appFunctor = [&sut]()
    {
      // ---------------- Setup
      //
      auto nextToSut = BinaryVector::CreateFromHexString("FADE_BD");

      // ---------------- Exercise
      //
      TS_ASSERT_THROWS_NOTHING (sut.iWrite("Foo", nextToSut));
      TS_ASSERT_THROWS_NOTHING (sut.iApply());
    };

    // ---------------- Setup (main thread)
    //
    sut.CreateApplicationThread(mux, appFunctor); // Include "Exercise" in created thread
    sut.StartCreatedApplicationThreads();
    sut.WaitForApplicationsEnd();  // Make sure application has done its action

    // ---------------- Verify
    //
    TS_ASSERT_EQUALS (regDyn_0->NextToSut(), BinaryVector::CreateFromHexString("6060_6060"));
    TS_ASSERT_EQUALS (regDyn_1->NextToSut(), BinaryVector::CreateFromHexString("FA61_6161"));
    TS_ASSERT_EQUALS (regDyn_2->NextToSut(), BinaryVector::CreateFromHexString("62DE_6262"));
    TS_ASSERT_EQUALS (regDyn_3->NextToSut(), BinaryVector::CreateFromHexString("6363_BD63"));
  }
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

//  ENABLE_LOG_IN_SCOPE;
  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut]()
  {
    // ---------------- Setup
    //
    auto nextToSut = BinaryVector::CreateFromHexString("FADE_CAFE");
    sut.iWrite("dynamic_3", nextToSut);

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.iApply());
  };

  // ---------------- Setup (main thread)
  //
  sut.CreateApplicationThread(mux, appFunctor); // Include "Exercise" in created thread
  sut.Start();
  sut.StartCreatedApplicationThreads();
  sut.WaitForApplicationsEnd();              // Make sure application has done its action
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

  //  ENABLE_LOG_IN_SCOPE;
  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut]()
  {
    // ---------------- Setup
    //
    auto nextToSut = BinaryVector::CreateFromHexString("FADE_CAFE");
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
  sut.WaitForApplicationsEnd();              // Make sure application has done its action
  sut.Stop();

  // ---------------- Verify
  //
  auto expected = BinaryVector::CreateFromHexString("FADE_CAFE");
  TS_ASSERT_DIFFERS (reg->NextToSut(), reg->LastToSut());   // As no data cycle took place new value has not been transfered to SUT
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

//+  ENABLE_LOG_IN_SCOPE;
//+  LOG_FUNCTION_SCOPE;
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
    sut.Start();
    sut.StartCreatedApplicationThreads();
    sut.WaitForApplicationsEnd();              // Make sure application has done its action
  );
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


//! Checks SystemModelManager::iApply() with 4 application threads accessing once same register
//!
void UT_SystemModelManager::test_iApply_4_Threads_Once_SameReg ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto mux  = sm.LinkerWithId(2u);   // This is Tap mux
  auto reg  = sm.RegisterWithId(9u);

//+  ENABLE_LOG_IN_SCOPE;
  LOG_FUNCTION_SCOPE;
  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut]()
  {
    auto nextToSut = BinaryVector::CreateFromHexString("FADE_CAFE");

    TS_ASSERT_THROWS_NOTHING
    (
      sut.iWrite("dynamic_3", nextToSut);
      sut.iApply();
    );
  };

  // ---------------- Setup (main thread)
  //
  sut.CreateApplicationThread(mux, appFunctor, "App_1");
  sut.CreateApplicationThread(mux, appFunctor, "App_2");
  sut.CreateApplicationThread(mux, appFunctor, "App_3");
  sut.CreateApplicationThread(mux, appFunctor, "App_4");
  sut.Start();
  sut.StartCreatedApplicationThreads();
  sut.WaitForApplicationsEnd();              // Make sure application has done its action
  sut.Stop();

  // ---------------- Verify
  //
  auto expected = BinaryVector::CreateFromHexString("FADE_CAFE");
  TS_ASSERT_EQUALS (reg->NextToSut(), reg->LastToSut());
}


//! Checks SystemModelManager::iApply() with 4 application threads accessing once their own register
//!
void UT_SystemModelManager::test_iApply_4_Threads_1_Write ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto mux  = sm.LinkerWithId(2u);   // This is Tap mux
  auto reg_0  = sm.RegisterWithId(6u); reg_0->SetHoldValue(true); // Hold is to maintain the same value (nextToSut becoume also bypass)
  auto reg_1  = sm.RegisterWithId(7u); reg_1->SetHoldValue(true);
  auto reg_2  = sm.RegisterWithId(8u); reg_2->SetHoldValue(true);
  auto reg_3  = sm.RegisterWithId(9u); reg_3->SetHoldValue(true);

  auto gotVector_0 = BinaryVector();
  auto gotVector_1 = BinaryVector();
  auto gotVector_2 = BinaryVector();
  auto gotVector_3 = BinaryVector();

//+  ENABLE_LOG_IN_SCOPE;
  LOG_FUNCTION_SCOPE;
  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut](string_view regName, string_view regValue, BinaryVector& gotVector)
  {
    auto nextToSut = BinaryVector::CreateFromHexString(regValue);

    TS_ASSERT_THROWS_NOTHING
    (
      sut.iWrite(regName, nextToSut);
      sut.iApply();
      sut.iRead(regName, nextToSut);
      sut.iApply();
      gotVector = sut.iGet(regName);
    );
  };

  // ---------------- Setup (main thread)
  //
  sut.CreateApplicationThread(mux, [appFunctor, &gotVector_0]() { appFunctor("dynamic_0", "CAFE_8760", gotVector_0); }, "App_0");
  sut.CreateApplicationThread(mux, [appFunctor, &gotVector_1]() { appFunctor("dynamic_1", "CAFE_8761", gotVector_1); }, "App_1");
  sut.CreateApplicationThread(mux, [appFunctor, &gotVector_2]() { appFunctor("dynamic_2", "CAFE_8762", gotVector_2); }, "App_2");
  sut.CreateApplicationThread(mux, [appFunctor, &gotVector_3]() { appFunctor("dynamic_3", "CAFE_8763", gotVector_3); }, "App_3");
  sut.Start();
  sut.StartCreatedApplicationThreads();
  sut.WaitForApplicationsEnd(); // Make sure applications have done their action
  sut.Stop();

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (reg_0->NextToSut(), reg_0->LastToSut());
  TS_ASSERT_EQUALS (reg_1->NextToSut(), reg_1->LastToSut());
  TS_ASSERT_EQUALS (reg_2->NextToSut(), reg_2->LastToSut());
  TS_ASSERT_EQUALS (reg_3->NextToSut(), reg_3->LastToSut());

  TS_ASSERT_EQUALS (reg_0->NextToSut(), BinaryVector::CreateFromHexString("CAFE_8760"));
  TS_ASSERT_EQUALS (reg_1->NextToSut(), BinaryVector::CreateFromHexString("CAFE_8761"));
  TS_ASSERT_EQUALS (reg_2->NextToSut(), BinaryVector::CreateFromHexString("CAFE_8762"));
  TS_ASSERT_EQUALS (reg_3->NextToSut(), BinaryVector::CreateFromHexString("CAFE_8763"));

  TS_ASSERT_EQUALS (gotVector_0, BinaryVector::CreateFromHexString("CAFE_8760"));
  TS_ASSERT_EQUALS (gotVector_1, BinaryVector::CreateFromHexString("CAFE_8761"));
  TS_ASSERT_EQUALS (gotVector_2, BinaryVector::CreateFromHexString("CAFE_8762"));
  TS_ASSERT_EQUALS (gotVector_3, BinaryVector::CreateFromHexString("CAFE_8763"));
}


//! Checks SystemModelManager::iApply() with 4 application threads accessing multiple times their own register
//!
void UT_SystemModelManager::test_iApply_4_Threads_N_Writes ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto mux  = sm.LinkerWithId(2u);   // This is Tap mux
  auto reg_0  = sm.RegisterWithId(6u); reg_0->SetHoldValue(true); // Hold is to maintain the same value (nextToSut becoume also bypass)
  auto reg_1  = sm.RegisterWithId(7u); reg_1->SetHoldValue(true);
  auto reg_2  = sm.RegisterWithId(8u); reg_2->SetHoldValue(true);
  auto reg_3  = sm.RegisterWithId(9u); reg_3->SetHoldValue(true);

//+  ENABLE_LOG_IN_SCOPE;
  LOG_FUNCTION_SCOPE;
  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut](string_view regName, uint32_t initialValue)
  {
    auto nextToSut    = BinaryVector(32u);
    auto currentValue = initialValue;
    auto gotValue     = BinaryVector(32u);

    try
    {
      for (int ii = 0 ; ii <= 48 ; ++ii)
      {
        nextToSut.Set(currentValue++);
        LOG(INFO) << "Writing to " << regName << ": 0x" << std::hex << currentValue;
        sut.iWrite(regName, nextToSut);

        // ---------------- Stupid double write without iApply...
        //
        if (ii == 47)
        {
          ++ii;
          nextToSut.Set(currentValue++);
          sut.iWrite(regName, nextToSut);
        }

        //! Make it "dream" between iWrite and iApply
        //!
        if (ii == 37)
        {
          std::this_thread::sleep_for(1ms);
        }

        sut.iApply();

        // ---------------- Make it "think" after iApply
        //
        if (ii == 33)
        {
          std::this_thread::sleep_for(2ms);
        }

        sut.iRead(regName, nextToSut);
        sut.iApply();
        sut.iGet(regName, gotValue);
      }
    }
    catch(std::exception& exc)  // Catch C++ standard exceptions
    {
      TS_FAIL (exc.what());
    }
    catch (...)
    {
      TS_FAIL ("Caught unknown exception");
    }
  };

  // ---------------- Setup (main thread)
  //
  sut.CreateApplicationThread(mux, [appFunctor]() { appFunctor("dynamic_0", 0x00005000u); }, "App_0");
  sut.CreateApplicationThread(mux, [appFunctor]() { appFunctor("dynamic_1", 0x00015001u); }, "App_1");
  sut.CreateApplicationThread(mux, [appFunctor]() { appFunctor("dynamic_2", 0x00025002u); }, "App_2");
  sut.CreateApplicationThread(mux, [appFunctor]() { appFunctor("dynamic_3", 0x00035003u); }, "App_3");
  sut.Start();
  sut.StartCreatedApplicationThreads();
  sut.WaitForApplicationsEnd();              // Make sure applications have done their action
  sut.Stop();

  // ---------------- Verify
  //
  // Updated up to the last value
  //
  TS_ASSERT_EQUALS (reg_0->NextToSut(), reg_0->LastToSut());
  TS_ASSERT_EQUALS (reg_1->NextToSut(), reg_1->LastToSut());
  TS_ASSERT_EQUALS (reg_2->NextToSut(), reg_2->LastToSut());
  TS_ASSERT_EQUALS (reg_3->NextToSut(), reg_3->LastToSut());

  // Terminal value
  //
  auto expected_0 = BinaryVector(32u); expected_0.Set(0x00005030u);
  auto expected_1 = BinaryVector(32u); expected_1.Set(0x00015031u);
  auto expected_2 = BinaryVector(32u); expected_2.Set(0x00025032u);
  auto expected_3 = BinaryVector(32u); expected_3.Set(0x00035033u);

  TS_ASSERT_EQUALS (reg_0->NextToSut(), expected_0);
  TS_ASSERT_EQUALS (reg_1->NextToSut(), expected_1);
  TS_ASSERT_EQUALS (reg_2->NextToSut(), expected_2);
  TS_ASSERT_EQUALS (reg_3->NextToSut(), expected_3);

  TS_ASSERT_EQUALS (reg_0->Mismatches(), 0);
  TS_ASSERT_EQUALS (reg_1->Mismatches(), 0);
  TS_ASSERT_EQUALS (reg_2->Mismatches(), 0);
  TS_ASSERT_EQUALS (reg_3->Mismatches(), 0);
}

//! Checks SystemModelManager::iApply() with 4 application threads accessing multiple times
//! their own register and base on testcase "1500"
//!
void UT_SystemModelManager::test_iApply_4_Threads_N_Writes_TC_1500 ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_1500(sm, "Tap", false);

  auto mux    = sm.LinkerWithId(12u);   // This is Tap mux

  auto reg_0  = sm.RegisterWithId(14u);
  auto reg_1  = sm.RegisterWithId(15u);
  auto reg_2  = sm.RegisterWithId(16u);
  auto reg_3  = sm.RegisterWithId(17u);

//+  ENABLE_LOG_IN_SCOPE;
  LOG_FUNCTION_SCOPE;
  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut](string_view regName, uint32_t initialValue)
  {
    auto nextToSut    = BinaryVector(32u);
    auto currentValue = initialValue;

    TS_ASSERT_THROWS_NOTHING
    (
      sut.iPrefix(".");

      for (int ii = 0 ; ii <= 48 ; ++ii)
      {
        nextToSut.Set(currentValue++);
        LOG(INFO) << "Writing to " << regName << ": 0x" << std::hex << currentValue;
        sut.iWrite(regName, nextToSut);

        // ---------------- Stupid double write without iApply...
        //
        if (ii == 37) { ++ii; nextToSut.Set(currentValue++); sut.iWrite(regName, nextToSut); }

        //! Make it "dream" between iWrite and iApply
        //!
        if (ii == 27) { std::this_thread::sleep_for(1ms); }

        sut.iApply();

        // ---------------- Make is "think" after iApply
        //
        if (ii == 17)
        {
          std::this_thread::sleep_for(2ms);
        }
      }
    );
  };

  // ---------------- Setup (main thread)
  //
  sut.CreateApplicationThread(mux, [appFunctor]() { appFunctor("dynamic_0", 0x00005000u); }, "TC_1500_App_0");
  sut.CreateApplicationThread(mux, [appFunctor]() { appFunctor("dynamic_1", 0x00015001u); }, "TC_1500_App_1");
  sut.CreateApplicationThread(mux, [appFunctor]() { appFunctor("dynamic_2", 0x00025002u); }, "TC_1500_App_2");
  sut.CreateApplicationThread(mux, [appFunctor]() { appFunctor("dynamic_3", 0x00035003u); }, "TC_1500_App_3");
  sut.Start();
  sut.StartCreatedApplicationThreads();
  sut.WaitForApplicationsEnd();              // Make sure applications have done their action
  sut.Stop();

  // ---------------- Verify
  //
  // Updated up to the last value
  //
  TS_ASSERT_EQUALS (reg_0->NextToSut(), reg_0->LastToSut());
  TS_ASSERT_EQUALS (reg_1->NextToSut(), reg_1->LastToSut());
  TS_ASSERT_EQUALS (reg_2->NextToSut(), reg_2->LastToSut());
  TS_ASSERT_EQUALS (reg_3->NextToSut(), reg_3->LastToSut());

  // Terminal value
  //
  auto expected_0 = BinaryVector(32); expected_0.Set(0x00005030u);
  auto expected_1 = BinaryVector(32); expected_1.Set(0x00015031u);
  auto expected_2 = BinaryVector(32); expected_2.Set(0x00025032u);
  auto expected_3 = BinaryVector(32); expected_3.Set(0x00035033u);

  TS_ASSERT_EQUALS (reg_0->NextToSut(), expected_0);
  TS_ASSERT_EQUALS (reg_1->NextToSut(), expected_1);
  TS_ASSERT_EQUALS (reg_2->NextToSut(), expected_2);
  TS_ASSERT_EQUALS (reg_3->NextToSut(), expected_3);
}

//! Checks SystemModelManager::iRefresh() with 3 application threads accessing multiple times their own register
//!
//! @note It just check that application and manager threads are not blocked
void UT_SystemModelManager::test_iRefresh ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto mux  = sm.LinkerWithId(2u);   // This is Tap mux

  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut](string_view regName)
  {
    try
    {
      for (int ii = 0 ; ii <= 7 ; ++ii)
      {
        sut.iRefresh(regName);

        // ---------------- "Stupid" double refresh without iApply...
        //
        if ((ii % 3) == 0)
        {
          sut.iRefresh(regName);
        }

        //! Make it "dream" between iWrite and iApply
        //!
        if ((ii % 5) == 0)
        {
          std::this_thread::sleep_for(1ms);
        }

        sut.iApply();

        // ---------------- Make it "think" for some time
        //
        if ((ii % 6) == 0)
        {
          std::this_thread::sleep_for(2ms);
        }
      }
    }
    catch(std::exception& exc)  // Catch C++ standard exceptions
    {
      TS_FAIL (exc.what());
    }
    catch (...)
    {
      TS_FAIL ("Caught unknown exception");
    }
  };

  // ---------------- Setup (main thread)
  //
  sut.CreateApplicationThread(mux, [appFunctor]() { appFunctor("dynamic_0"); }, "App_0");
  sut.CreateApplicationThread(mux, [appFunctor]() { appFunctor("dynamic_1"); }, "App_1");
  sut.CreateApplicationThread(mux, [appFunctor]() { appFunctor("dynamic_2"); }, "App_2");
  sut.Start();
  sut.StartCreatedApplicationThreads();
  sut.WaitForApplicationsEnd();              // Make sure applications have done their action
  sut.Stop();

  // ---------------- Verify
  //
  auto reg_0  = sm.RegisterWithId(6u);
  auto reg_1  = sm.RegisterWithId(7u);
  auto reg_2  = sm.RegisterWithId(8u);
  auto reg_3  = sm.RegisterWithId(9u);

  TS_ASSERT_FALSE (reg_0->IsPending());
  TS_ASSERT_FALSE (reg_1->IsPending());
  TS_ASSERT_FALSE (reg_2->IsPending());
  TS_ASSERT_FALSE (reg_3->IsPending());
}


//! Checks SystemModelManager::iRefresh() with 3 application threads accessing multiple times four registers
//!
//! @note It just check that application and manager threads are not blocked
void UT_SystemModelManager::test_iRefresh_4_RegPerApp ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto mux  = sm.LinkerWithId(2u);   // This is Tap mux

  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut]()
  {
    try
    {
      for (int ii = 0 ; ii <= 9 ; ++ii)
      {
        sut.iRefresh("dynamic_0");
        sut.iRefresh("dynamic_1");
        sut.iRefresh("dynamic_2");
        sut.iRefresh("dynamic_3");

        sut.iApply();
      }
    }
    catch(std::exception& exc)  // Catch C++ standard exceptions
    {
      TS_FAIL (exc.what());
    }
    catch (...)
    {
      TS_FAIL ("Caught unknown exception");
    }
  };

  // ---------------- Setup (main thread)
  //
  sut.CreateApplicationThread(mux, appFunctor, "App_0");
  sut.CreateApplicationThread(mux, appFunctor, "App_1");
  sut.CreateApplicationThread(mux, appFunctor, "App_2");
  sut.Start();
  sut.StartCreatedApplicationThreads();
  sut.WaitForApplicationsEnd();              // Make sure applications have done their action
  sut.Stop();

  // ---------------- Verify
  //
  auto reg_0  = sm.RegisterWithId(6u);
  auto reg_1  = sm.RegisterWithId(7u);
  auto reg_2  = sm.RegisterWithId(8u);
  auto reg_3  = sm.RegisterWithId(9u);

  TS_ASSERT_FALSE (reg_0->IsPending());
  TS_ASSERT_FALSE (reg_1->IsPending());
  TS_ASSERT_FALSE (reg_2->IsPending());
  TS_ASSERT_FALSE (reg_3->IsPending());
}



// Checks SystemModelManager::iGetRefresh() with 4 application threads accessing multiple times their own register
//
void UT_SystemModelManager::test_iGetRefresh_uint8        () { Check_iGetRefresh<uint8_t>();      }
void UT_SystemModelManager::test_iGetRefresh_uint16       () { Check_iGetRefresh<uint16_t>();     }
void UT_SystemModelManager::test_iGetRefresh_uint32       () { Check_iGetRefresh<uint32_t>();     }
void UT_SystemModelManager::test_iGetRefresh_uint64       () { Check_iGetRefresh<uint64_t>();     }
void UT_SystemModelManager::test_iGetRefresh_int8         () { Check_iGetRefresh<int8_t>();       }
void UT_SystemModelManager::test_iGetRefresh_int16        () { Check_iGetRefresh<int16_t>();      }
void UT_SystemModelManager::test_iGetRefresh_int32        () { Check_iGetRefresh<int32_t>();      }
void UT_SystemModelManager::test_iGetRefresh_int64        () { Check_iGetRefresh<int64_t>();      }
void UT_SystemModelManager::test_iGetRefresh_BinaryVector () { Check_iGetRefresh<BinaryVector>(); }


//! Checks SystemModelManager::iGetStatus() without requesting to reset the counter
//!
void UT_SystemModelManager::test_iGetStatus_Register_WithoutClear ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto reg  = sm.RegisterWithId(8u);
  reg->SetCheckExpected(true);
  reg->SetExpectedFromSut (BinaryVector::CreateFromHexString("FADE_6666"));
  reg->SetFromSut         (BinaryVector::CreateFromHexString("FADE_5555"));
  reg->SetFromSut         (BinaryVector::CreateFromHexString("FADE_4444"));

  TS_ASSERT_EQUALS (reg->Mismatches(), 2u);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  auto count_1 = sut.iGetStatus("dynamic_2", false);
  auto count_2 = sut.iGetStatus("dynamic_2", false);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (count_1, 2u);
  TS_ASSERT_EQUALS (count_1, count_2);
}


//! Checks SystemModelManager::iGetStatus() without requesting to reset the counter
//!
void UT_SystemModelManager::test_iGetStatus_Register_WithClear ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto reg  = sm.RegisterWithId(8u);
  reg->SetCheckExpected(true);
  reg->SetExpectedFromSut (BinaryVector::CreateFromHexString("FADE_6666"));
  reg->SetFromSut         (BinaryVector::CreateFromHexString("FADE_5555"));
  reg->SetFromSut         (BinaryVector::CreateFromHexString("FADE_4444"));

  TS_ASSERT_EQUALS (reg->Mismatches(), 2u);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  auto count_1 = sut.iGetStatus("dynamic_2", true);
  auto count_2 = sut.iGetStatus("dynamic_2", true);
  auto count_3 = sut.iGetStatus("dynamic_2", true);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (count_1, 2u);
  TS_ASSERT_EQUALS (count_2, 0u);
  TS_ASSERT_EQUALS (count_3, 0u);
}


//! Checks SystemModelManager::iGetStatus() from root node without requesting to reset the counters
//!
void UT_SystemModelManager::test_iGetStatus_Root_WithoutClear ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto reg_1  = sm.RegisterWithId(6u);
  reg_1->SetCheckExpected(true);
  reg_1->SetExpectedFromSut (BinaryVector::CreateFromHexString("CAFE_1111"));
  reg_1->SetFromSut         (BinaryVector::CreateFromHexString("CAFE_2222"));
  reg_1->SetFromSut         (BinaryVector::CreateFromHexString("CAFE_3333"));
  reg_1->SetFromSut         (BinaryVector::CreateFromHexString("CAFE_4444"));


  auto reg_2  = sm.RegisterWithId(8u);
  reg_2->SetCheckExpected(true);
  reg_2->SetExpectedFromSut (BinaryVector::CreateFromHexString("FADE_6666"));
  reg_2->SetFromSut         (BinaryVector::CreateFromHexString("FADE_5555"));
  reg_2->SetFromSut         (BinaryVector::CreateFromHexString("FADE_4444"));

  TS_ASSERT_EQUALS (reg_1->Mismatches(), 3u);
  TS_ASSERT_EQUALS (reg_2->Mismatches(), 2u);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  auto count_1 = sut.iGetStatus(false);
  auto count_2 = sut.iGetStatus(false);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (count_1, 5u);
  TS_ASSERT_EQUALS (count_1, count_2);
}


//! Checks SystemModelManager::iGetStatus() from root node requesting to reset the counters
//!
void UT_SystemModelManager::test_iGetStatus_Root_WithClear ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto reg_1  = sm.RegisterWithId(6u);
  reg_1->SetCheckExpected(true);
  reg_1->SetExpectedFromSut (BinaryVector::CreateFromHexString("CAFE_1111"));
  reg_1->SetFromSut         (BinaryVector::CreateFromHexString("CAFE_2222"));
  reg_1->SetFromSut         (BinaryVector::CreateFromHexString("CAFE_3333"));
  reg_1->SetFromSut         (BinaryVector::CreateFromHexString("CAFE_4444"));


  auto reg_2  = sm.RegisterWithId(8u);
  reg_2->SetCheckExpected(true);
  reg_2->SetExpectedFromSut (BinaryVector::CreateFromHexString("FADE_6666"));
  reg_2->SetFromSut         (BinaryVector::CreateFromHexString("FADE_5555"));
  reg_2->SetFromSut         (BinaryVector::CreateFromHexString("FADE_4444"));

  TS_ASSERT_EQUALS (reg_1->Mismatches(), 3u);
  TS_ASSERT_EQUALS (reg_2->Mismatches(), 2u);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  auto count_1 = sut.iGetStatus(true);
  auto count_2 = sut.iGetStatus(true);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (count_1, 5u);
  TS_ASSERT_EQUALS (count_2, 0u);
}



//! Checks SystemModelManager::iGetStatus() for single register when one thread cause mismatches and a second one get the status
//!
//! @note It also check that application and manager threads are not blocked
void UT_SystemModelManager::test_iGetStatus_Register_Multithread ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto mux = sm.LinkerWithId(2u);   // This is Tap mux

  SystemModelManager sut(sm);

  auto causeMismatchesFunctor = [&sut]()
  {
    TS_ASSERT_THROWS_NOTHING
    (
      auto expected = BinaryVector::CreateFromHexString("FADE_6666");

      for (int ii = 0 ; ii < 5 ; ++ii)
      {
        sut.iWrite ("dynamic_3", ii);
        sut.iRead  ("dynamic_3", expected);
        sut.iApply();
      }
    );
  };

  auto reachedTimeout   = false;
  auto getStatusFunctor = [&sut, &reachedTimeout]()
  {
    auto timeout = std::chrono::steady_clock::now() + 100ms;
    TS_ASSERT_THROWS_NOTHING
    (
      // ---------------- Exercise
      //
      while (sut.iGetStatus("dynamic_3", false) < 5u)
      {
        if (std::chrono::steady_clock::now() > timeout)
        {
          reachedTimeout = true;
          break;
        }
        std::this_thread::yield();
      }
    );
  };


  // ---------------- Setup (main thread)
  //
  sut.CreateApplicationThread(mux, getStatusFunctor,       "App_getStatus");
  sut.CreateApplicationThread(mux, causeMismatchesFunctor, "App_causeMismatches");
  sut.Start();
  sut.StartCreatedApplicationThreads();

  // ---------------- Verify
  //
  // Proper termination means successful test
  //
  sut.WaitForApplicationsEnd();              // Make sure applications have done their action
  sut.Stop();
  TS_ASSERT_FALSE (reachedTimeout);
}



//! Checks SystemModelManager::iGetStatus() for all SystemModel when one thread cause mismatches and a second one get the status
//!
//! @note It also check that application and manager threads are not blocked
void UT_SystemModelManager::test_iGetStatus_Root_Multithread ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto mux = sm.LinkerWithId(2u);   // This is Tap mux

  SystemModelManager sut(sm);

  auto causeMismatchesFunctor = [&sut]()
  {
    TS_ASSERT_THROWS_NOTHING
    (
      for (int ii = 0 ; ii < 5 ; ++ii)
      {
        sut.iWrite ("dynamic_3", ii);
        sut.iApply();
        std::this_thread::yield();
      }
    );
  };

  auto reachedTimeout   = false;
  auto getStatusFunctor = [&sut, &reachedTimeout]()
  {
    auto expected = BinaryVector::CreateFromHexString("FADE_6666");

    auto timeout = std::chrono::steady_clock::now() + 100ms;
    TS_ASSERT_THROWS_NOTHING
    (
      // ---------------- Exercise
      //
      do
      {
        if (std::chrono::steady_clock::now() > timeout)
        {
          reachedTimeout = true;
          break;
        }
        sut.iRead  ("dynamic_3", expected);  // Set the expected value
        sut.iApply();
        std::this_thread::yield();
      } while (sut.iGetStatus(false) < 5u);
    );
  };


  // ---------------- Setup (main thread)
  //
  sut.CreateApplicationThread(mux, getStatusFunctor,       "App_getStatus");
  sut.CreateApplicationThread(mux, causeMismatchesFunctor, "App_causeMismatches");
  sut.Start();
  sut.StartCreatedApplicationThreads();

  // ---------------- Verify
  //
  // Proper termination means successful test
  //
  sut.WaitForApplicationsEnd();              // Make sure applications have done their action
  sut.Stop();
  TS_ASSERT_FALSE (reachedTimeout);
}

//! Checks SystemModelManager::iGetMiscompares() in single thread context
//!
void UT_SystemModelManager::test_iGetMiscompares ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto reg  = sm.RegisterWithId(8u);
  reg->SetCheckExpected(true);
  reg->SetPendingForRead(true);
  reg->SetExpectedFromSut (BinaryVector::CreateFromHexString("FADE_6666"));
  reg->SetFromSut         (BinaryVector::CreateFromHexString("FADE_5555"));

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  auto compareResult = sut.iGetMiscompares("dynamic_2");

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (compareResult, BinaryVector::CreateFromHexString("0000_3333"));
}

//! Checks SystemModelManager::iGetMiscompares() in multi-thread context
//!
void UT_SystemModelManager::test_iGetMiscompares_Multithread ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm);

  auto mux = sm.LinkerWithId(2u);   // This is Tap mux

  SystemModelManager sut(sm);

  auto causeMismatchesFunctor = [&sut]()
  {
    TS_ASSERT_THROWS_NOTHING
    (
      for (int ii = 1 ; ii < 7 ; ++ii)
      {
        sut.iWrite ("dynamic_3", ii);
        sut.iRead  ("dynamic_3", 0xA007u);
        sut.iApply();
      }
    );
  };

  auto reachedTimeout = false;

  auto getMiscomparesFunctor = [&sut, &reachedTimeout]()
  {
    auto timeout = std::chrono::steady_clock::now() + 10ms;
    TS_ASSERT_THROWS_NOTHING
    (
      auto expectedLastMiscompare = BinaryVector::CreateFromHexString("0000_A001");   // Last written value is 6

      // ---------------- Exercise
      //
      do
      {
        auto compareResult = sut.iGetMiscompares("dynamic_3");

        if (compareResult == expectedLastMiscompare)
        {
          break;
        }

        if (std::chrono::steady_clock::now() > timeout)
        {
          reachedTimeout = true;
          break;
        }
        std::this_thread::sleep_for(1ms);
      } while (true);
    );
  };


  // ---------------- Setup (main thread)
  //
  sut.CreateApplicationThread(mux, getMiscomparesFunctor,  "App_getMiscomparesFunctor");
  sut.CreateApplicationThread(mux, causeMismatchesFunctor, "App_causeMismatches");
  sut.Start();
  sut.StartCreatedApplicationThreads();

  // ---------------- Verify
  //
  // Proper termination means successful test
  //
  sut.WaitForApplicationsEnd();              // Make sure applications have done their actions
  sut.Stop();
  TS_ASSERT_FALSE (reachedTimeout);
}

//! Checks SystemModelManager::iRead() using same thread as SystemModelManager
//!                                                                           // iReadValue, iWriteValue, expectedMismatch
void UT_SystemModelManager::test_iRead_uint8        () { Check_iRead_SingleThread<uint8_t>  (0xF0,        0xAC,        "000000005C"); }
void UT_SystemModelManager::test_iRead_uint16       () { Check_iRead_SingleThread<uint16_t> (0x0FF0,      0xACDE,      "000000A32E"); }
void UT_SystemModelManager::test_iRead_uint32       () { Check_iRead_SingleThread<uint32_t> (0xFF00FF00,  0x89ABCDEF,  "0076AB32EF"); }
void UT_SystemModelManager::test_iRead_uint64       () { Check_iRead_SingleThread<uint64_t> (0xCFF00FF00, 0xD89ABCDEF, "0176AB32EF"); }
void UT_SystemModelManager::test_iRead_int8         () { Check_iRead_SingleThread<int8_t>   (0x70,        0x6C,        "000000001C"); }
void UT_SystemModelManager::test_iRead_int16        () { Check_iRead_SingleThread<int16_t>  (0x0FF0,      0x3CDE,      "000000332E"); }
void UT_SystemModelManager::test_iRead_int32        () { Check_iRead_SingleThread<int32_t>  (0x2F00FF00,  0x49ABCDEF,  "0066AB32EF"); }
void UT_SystemModelManager::test_iRead_int64        () { Check_iRead_SingleThread<int64_t>  (0xCFF00FF00, 0xD89ABCDEF, "0176AB32EF"); }
void UT_SystemModelManager::test_iRead_BinaryVector () { Check_iRead_SingleThread(BinaryVector::CreateFromHexString("0CFF00FF00"),
                                                                                  BinaryVector::CreateFromHexString("0D89ABCDEF"),
                                                                                                                    "0176AB32EF"); }

//! Checks SystemModelManager::iRead()
//! @note Forces a mismatch by writing a value different from the expected value provided to iRead
void UT_SystemModelManager::test_iRead_WithAlias ()
{
  // ---------------- Setup
  //
  auto iReadValue        = BinaryVector::CreateFromHexString("FF00FF");
  auto iWriteValue       = BinaryVector::CreateFromHexString("89ABCD");
  auto expectedXorResult = BinaryVector::CreateFromHexString("76AB32");

  SystemModel sm;
  Create_TestCase_MIB_Multichain_Pre(sm, false, 40u);
  SystemModelManager sut(sm);

  auto mux      = sm.LinkerWithId(2u);   // This is Tap mux
  auto regDyn_0 = sm.RegisterWithId(6u); // 32 bits initialized with 0x60606060
  auto regDyn_1 = sm.RegisterWithId(7u); // 32 bits initialized with 0x61616161
  auto regDyn_2 = sm.RegisterWithId(8u); // 32 bits initialized with 0x62626262
  auto regDyn_3 = sm.RegisterWithId(9u); // 32 bits initialized with 0x63636363

  VirtualRegister virtualRegister;
  virtualRegister.Append({regDyn_1, IndexedRange{31, 24}});
  virtualRegister.Append({regDyn_2, IndexedRange{23, 16}});
  virtualRegister.Append({regDyn_3, IndexedRange{15, 8}});
  mux->AddAlias({"Foo"s, std::move(virtualRegister)});    // ==> 24 bits

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.iRead("Foo", iReadValue));

  // ---------------- Verify
  //
  sut.iWrite ("Foo", iWriteValue); // Loopback (default protocol) will force FromSut to be updated
  sut.iApply();

  BinaryVector xorResult;
  uint32_t     status = 0;

  TS_ASSERT_THROWS_NOTHING (xorResult = sut.iGetMiscompares ("Foo"));
  TS_ASSERT_THROWS_NOTHING (status    = sut.iGetStatus      (".", false));   // Must use a single node (that is not "ignored" in paths)

  TS_ASSERT_EQUALS (xorResult, expectedXorResult);
  TS_ASSERT_EQUALS (status,    3u); // ==> 3 registers detected mismatch
}


//! Checks SystemModelManager::iRead() when there is a don't care value - using same thread as SystemModelManager
//!                                                                                                 // iRead-ExpectedValue, iRead-DontCare, iWrite-Value, expectedMismatch
//!
void UT_SystemModelManager::test_iRead_DontCare_uint8        () { Check_iRead_DontCare_SingleThread<uint8_t>  (0xF0,        0xF3,        0xAC,        "0000000050"); }
void UT_SystemModelManager::test_iRead_DontCare_uint16       () { Check_iRead_DontCare_SingleThread<uint16_t> (0x0FF0,      0x0FFF,      0xACDE,      "000000032E"); }
void UT_SystemModelManager::test_iRead_DontCare_uint32       () { Check_iRead_DontCare_SingleThread<uint32_t> (0xFF00FF00,  0xFF00FF00,  0x89ABCDEF,  "0076003200"); }
void UT_SystemModelManager::test_iRead_DontCare_uint64       () { Check_iRead_DontCare_SingleThread<uint64_t> (0xCFF00FF00, 0xFFF00FF00, 0xD89ABCDEF, "0176003200"); }
void UT_SystemModelManager::test_iRead_DontCare_int8         () { Check_iRead_DontCare_SingleThread<int8_t>   (0x70,        0x70,        0x6C,        "0000000010"); }
void UT_SystemModelManager::test_iRead_DontCare_int16        () { Check_iRead_DontCare_SingleThread<int16_t>  (0x4280,      0x7F80,      0x3CDE,      "0000007E00"); }
void UT_SystemModelManager::test_iRead_DontCare_int32        () { Check_iRead_DontCare_SingleThread<int32_t>  (0x12345678,  0x3F00FF00,  0x98765432,  "000A000200"); }
void UT_SystemModelManager::test_iRead_DontCare_int64        () { Check_iRead_DontCare_SingleThread<int64_t>  (0x123456789, 0xCFF00FF00, 0x987654321, "08A4002400"); }
void UT_SystemModelManager::test_iRead_DontCare_BinaryVector () { Check_iRead_DontCare_SingleThread(BinaryVector::CreateFromHexString("0CAD005A00"),
                                                                                                    BinaryVector::CreateFromHexString("0FFF00FF00"),
                                                                                                    BinaryVector::CreateFromHexString("0D89ABCDEF"),
                                                                                                    "0124009700");
                                                                }

//! Checks SystemModelManager::iReset when called from main thread
//!
void UT_SystemModelManager::test_iReset_Main_Thread ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  auto ai  = Create_TestCase_1500(sm);
  auto spy = make_shared<Spy_SVF_Protocol>();
  spy->SupportTRST(false); // Cannot use TRST signal instead of RESET command
  ai->SetProtocol (spy);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.iReset());

  // ---------------- Verify
  //
  auto gotSvfCommands = spy->SVFCommands();

  vector<string> expected
  {
    "STATE RESET;",
  };

  TS_ASSERT_EQUALS (gotSvfCommands, expected);
}

//! Checks SystemModelManager::iReset when called from main thread and support TRST
//!
void UT_SystemModelManager::test_iReset_Main_Thread_SupportTRST ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  auto ai  = Create_TestCase_1500(sm);
  auto spy = make_shared<Spy_SVF_Protocol>();
  spy->SupportTRST(true); // Direct to use TRST signal instead of RESET command
  ai->SetProtocol (spy);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.iReset());

  // ---------------- Verify
  //
  auto gotSvfCommands = spy->SVFCommands();

  vector<string> expected
  {
     "TRST ON;",
     "TRST OFF;",
  };

  TS_ASSERT_EQUALS (gotSvfCommands, expected);
}

//! Checks SystemModelManager::iReset when called from main thread and required synchronous reset
//!
void UT_SystemModelManager::test_iReset_Main_Thread_Sync ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  auto ai  = Create_TestCase_1500(sm);
  auto spy = make_shared<Spy_SVF_Protocol>();
  spy->SupportTRST(true); // Direct to use TRST signal instead of RESET command, unless 'sync' mode is requested
  ai->SetProtocol (spy);

  SystemModelManager sut(sm);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.iReset(true));

  // ---------------- Verify
  //
  auto gotSvfCommands = spy->SVFCommands();

  vector<string> expected
  {
     "STATE RESET;",
  };

  TS_ASSERT_EQUALS (gotSvfCommands, expected);
}

//! Checks SystemModelManager::iReset when called from an application thread associated with root node
//!
void UT_SystemModelManager::test_iReset_App_Thread_Root ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  auto ai  = Create_TestCase_1500(sm);
  auto spy = make_shared<Spy_SVF_Protocol>();
  spy->SupportTRST(true); // Direct to use TRST signal instead of RESET command
  ai->SetProtocol (spy);

  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut]()
  {
    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.iReset());
  };

  sut.CreateApplicationThread(ai, appFunctor); // Include "Exercise" in created thread
  sut.StartCreatedApplicationThreads();

  // ---------------- Verify
  //
  sut.WaitForApplicationsEnd();  // Make sure application has done its action
  auto gotSvfCommands = spy->SVFCommands();

  vector<string> expected
  {
     "TRST ON;",
     "TRST OFF;",
  };

  TS_ASSERT_EQUALS (gotSvfCommands, expected);
}


//! Checks SystemModelManager::iReset when called from an application thread is not associated with root node
//!
void UT_SystemModelManager::test_iReset_App_Thread_NotRoot ()
{
  // ---------------- Setup
  //
  SystemModel sm;

  auto ai  = Create_TestCase_1500(sm);
  auto mux = sm.LinkerWithId(12u);
  auto spy = make_shared<Spy_SVF_Protocol>();
  spy->SupportTRST(true);
  ai->SetProtocol (spy);

  SystemModelManager sut(sm);

  // Thread functor
  auto appFunctor = [&sut]()
  {
    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (sut.iReset());
  };

  ENABLE_LOG_IN_SCOPE;
  sut.CreateApplicationThread(mux, appFunctor); // Application is not associated with root node
  sut.StartCreatedApplicationThreads();

  // ---------------- Verify
  //
  sut.WaitForApplicationsEnd();  // Make sure application has done its action
  auto gotSvfCommands = spy->SVFCommands();

  TS_ASSERT_TRUE (gotSvfCommands.empty());
}

//===========================================================================
// End of UT_SystemModelManager.cpp
//===========================================================================
