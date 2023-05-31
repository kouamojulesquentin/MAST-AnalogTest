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


//===========================================================================
// End of UT_SystemModelManagerTranslator.cpp
//===========================================================================
