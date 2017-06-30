//===========================================================================
//                           UT_SystemModelFactory.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SystemModelFactory.cpp
//!
//! Implements test fixture for testing SystemModelFactory
//!
//===========================================================================

#include "UT_SystemModelFactory.hpp"
#include "SystemModelFactory.hpp"
#include "TestModelBuilder.hpp"
#include "PrettyPrinter.hpp"
#include "Utility.hpp"

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


//! Initializes test (called for each test)
void UT_SystemModelFactory::setUp ()
{
  SystemModelNode::ResetNodeIdentifier();                                      // Needed to check with pretty print that include node identifiers
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL); // Keep quotes, HT, and new lines unescaped
  CxxTest::setStringResultsOnNewLine(true);
}

//! Checks SystemModelFactory::Instance()
//!
void UT_SystemModelFactory::test_Instance ()
{
  // ---------------- Setup
  //
  SystemModelFactory* pInstance = nullptr;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (pInstance = &SystemModelFactory::Instance());

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);
  TS_ASSERT_NOT_NULLPTR (pInstance);

  TS_ASSERT_EQUALS (pInstance->RegisteredCreatorsCount(), 0u);   // This is to detect when one should consider adding a new test for a new default creation method
}


//! Checks SystemModelFactory::RegisterCreator() when adding a new one when none are registered
//!
void UT_SystemModelFactory::test_RegisterCreator_NewOne_when_None ()
{
  // ---------------- Setup
  //
  auto& sut         = SystemModelFactory::Instance();
  auto  newFactory  = [](SystemModel& sm, const string& /* unused */)
  {
    auto chain = sm.CreateChain("chain_name");
    auto reg   = sm.CreateRegister("reg_name", BinaryVector::CreateFromHexString("Fade"), true, chain);
    return chain;
  };

  sut.Clear();

  // ---------------- Exercise
  //
  sut.RegisterCreator("Make_Chain"s, newFactory);

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.RegisteredCreatorsCount(), 1u);
  TS_ASSERT_TRUE   (sut.HasCreator("Make_Chain"));
  TS_ASSERT_FALSE  (sut.HasCreator("MAKE_CHAIN"));
}


//! Checks SystemModelFactory::Clear()
//!
void UT_SystemModelFactory::test_Clear ()
{
  // ---------------- Setup
  //
  auto& sut = SystemModelFactory::Instance();
  sut.RegisterCreator("Make_SIB"s, [](SystemModel& sm, const string& parameters)
  {
    test::TestModelBuilder builder(sm);
    auto node = builder.Create_Default_SIB(parameters);
    return node;
  });
  auto initialRegisteredCount = sut.RegisteredCreatorsCount();
  TS_ASSERT_DIFFERS (initialRegisteredCount, 0);

  // ---------------- Exercise
  //
  sut.Clear();

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.RegisteredCreatorsCount(), 0u);
}


//! Checks SystemModelFactory::RegisterCreator() when adding a new one when one is already registered
//!
void UT_SystemModelFactory::test_RegisterCreator_NewOne_when_One ()
{
  // ---------------- Setup
  //
  auto& sut = SystemModelFactory::Instance();
  sut.Clear();

  sut.RegisterCreator("1st", [](SystemModel& sm, const string& /* unused */)
  {
    auto chain = sm.CreateChain("1");
    return chain;
  });
  TS_ASSERT_EQUALS (sut.RegisteredCreatorsCount(), 1u);

  // ---------------- Exercise
  //
  sut.RegisterCreator("2nd"s, [](SystemModel& sm, const string& /* unused */)
  {
    auto chain = sm.CreateChain("2");
    return chain;
  });

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.RegisteredCreatorsCount(), 2u);
  TS_ASSERT_TRUE   (sut.HasCreator("1st"));
  TS_ASSERT_TRUE   (sut.HasCreator("2nd"));
}


//! Checks SystemModelFactory::Create() when must be successful
//!
void UT_SystemModelFactory::test_Create_Success ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto  creatorId           = std::get<0>(data);
    auto  creatorParameters   = std::get<1>(data);
    auto& expectedNodeType    = std::get<2>(data);
    auto  expectedPrettyPrint = std::get<3>(data);
    auto  sm                  = SystemModel{};
    auto  builder             = test::TestModelBuilder (sm);
    auto  tap                 = builder.Create_JTAG_TAP ("Tap", test::DEFAULT_IR_LEN, test::DEFAULT_TDR_LEN);
    auto& sut                 = SystemModelFactory::Instance();

    sut.RegisterCreator("Make_MIB"s, [](SystemModel& sm, const string& parameters)
    {
      test::TestModelBuilder builder(sm);

      auto node = builder.Create_Default_MIB(parameters, 3u);
      sm.CreateRegister("FooReg", BinaryVector::CreateFromString("0xDead"), false, node);
      sm.CreateRegister("BarReg", BinaryVector::CreateFromString("0xFace"), false, node);
      sm.CreateRegister("TarReg", BinaryVector::CreateFromString("0xBeef"), false, node);
      return node;
    });

    sut.RegisterCreator("Make_SIB"s, [](SystemModel& sm, const string& parameters)
    {
      test::TestModelBuilder builder(sm);

      auto node = builder.Create_Default_SIB(parameters);
      sm.CreateRegister("FarReg", BinaryVector::CreateFromString("0x345"), false, node);
      return node;
    });

    CxxTest::setAbortTestOnFail(true);
    shared_ptr<ParentNode> node;

    // ---------------- Exercise
    //
    TS_ASSERT_THROWS_NOTHING (node = sut.Create(creatorId, sm, creatorParameters));

    // ---------------- Verify
    //
    TS_ASSERT_NOT_NULLPTR (node);

    const auto& actualNodeType = typeid(*node);

    TS_ASSERT_EQUALS (actualNodeType, expectedNodeType);

    tap->AppendChild(node);

    // Check with pretty printer
    auto prettyPrint = PrettyPrinter::PrettyPrint(*tap, PrettyPrinterOptions::DisplayValueAuto
                                                      | PrettyPrinterOptions::ShowNodeIsIgnored);
    TS_ASSERT_EQUALS (prettyPrint, expectedPrettyPrint);

    // Check with checker
    auto checkResult = sm.Check();
    TS_ASSERT_EMPTY (checkResult.InformativeReport());
  };

  auto data =
  {
    // 0
    make_tuple("Make_MIB"s, "creatorMIB"s, std::cref(typeid(Chain)),
               "[Access_I](0)  \"Tap\"\n"
               " [Register](1)  \"Tap_IR\", length: 8, Hold value: true, bypass: 0xFF\n"
               " [Linker](2)    \"Tap_DR_Mux\"\n"
               "  :Selector:(1)  \"Tap_IR\"\n"
               "  [Register](3)  \"Tap_BPY\", length: 1, bypass: 0b1\n"
               "  [Chain](5)     \"creatorMIB\"\n"
               "   [Register](4)  \"creatorMIB_ctrl\", length: 2, Hold value: true, bypass: 0b00\n"
               "   [Linker](6)    \"creatorMIB_mux\"\n"
               "    :Selector:(4)  \"creatorMIB_ctrl\"\n"
               "    [Register](7)  \"FooReg\", length: 16, bypass: 0xDEAD\n"
               "    [Register](8)  \"BarReg\", length: 16, bypass: 0xFACE\n"
               "    [Register](9)  \"TarReg\", length: 16, bypass: 0xBEEF"
               ),

    // 1
    make_tuple("Make_SIB"s, "creatorSIB"s, std::cref(typeid(Chain)),
               "[Access_I](0)  \"Tap\"\n"
               " [Register](1)  \"Tap_IR\", length: 8, Hold value: true, bypass: 0xFF\n"
               " [Linker](2)    \"Tap_DR_Mux\"\n"
               "  :Selector:(1)  \"Tap_IR\"\n"
               "  [Register](3)  \"Tap_BPY\", length: 1, bypass: 0b1\n"
               "  [Chain](5)     \"creatorSIB\"\n"
               "   [Register](4)  \"creatorSIB_ctrl\", length: 1, Hold value: true, bypass: 0b0\n"
               "   [Linker](6)    \"creatorSIB_mux\"\n"
               "    :Selector:(4)  \"creatorSIB_ctrl\"\n"
               "    [Register](7)  \"FarReg\", length: 12, bypass: 0x345"
               ),
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}


//! Checks SystemModelFactory::Create() when must detect an error
//!
void UT_SystemModelFactory::test_Create_Error ()
{
  // ---------------- DDT Setup
  //
  auto checker = [](const auto& data)
  {
    // ---------------- Setup
    //
    auto  creatorId         = std::get<0>(data);
    auto  creatorParameters = std::get<1>(data);
    auto  sm                = SystemModel{};
    auto& sut               = SystemModelFactory::Instance();

    sut.RegisterCreator("Make_SIB"s, [](SystemModel& sm, const string& parameters)
    {
      test::TestModelBuilder builder(sm);

      auto node = builder.Create_Default_MIB(parameters, 2u);
      sm.CreateRegister("FatReg", BinaryVector::CreateFromString("0x12"),  false, node);
      sm.CreateRegister("FarReg", BinaryVector::CreateFromString("0x345"), false, node);
      return node;
    });

    // ---------------- Exercise & Verify
    //
    TS_ASSERT_THROWS (sut.Create(creatorId, sm, creatorParameters), std::exception);
  };

  auto data =
  {
    make_tuple("MAKE_Sib", ""), // 00: Wrong casing
    make_tuple("MAKE_Sub", ""), // 01: Unregistered creator name
    make_tuple("Make",     ""), // 02: Unregistered creator name (start of name is not sufficient)
    make_tuple("",         ""), // 03: Missing creator identifier
  };

  // ---------------- DDT Exercise
  //
  TS_DATA_DRIVEN_TEST(checker, data);
}

//! Checks SystemModelFactory::RegisterCreator() when replacing an existing one
//!
void UT_SystemModelFactory::test_RegisterCreator_Replace_Previous ()
{
  // ---------------- Setup
  //
  auto  sm        = SystemModel{};
  auto  builder   = test::TestModelBuilder (sm);
  auto  tap       = builder.Create_JTAG_TAP ("Tap", test::DEFAULT_IR_LEN, test::DEFAULT_TDR_LEN);
  auto  factory_1 = [](SystemModel& sm, const string& parameters)
  {
    test::TestModelBuilder builder(sm);

    auto node     = builder.Create_Default_SIB(parameters);
    sm.CreateRegister("FatReg", BinaryVector::CreateFromString("0x12"),  false, node);
    return node;
  };
  auto  factory_2 = [](SystemModel& sm, const string& parameters)
  {
    test::TestModelBuilder builder(sm);

    auto node     = builder.Create_Default_SIB(parameters);
    sm.CreateRegister("BitReg", BinaryVector::CreateFromString("0x67"),  false, node);
    return node;
  };

  auto& sut = SystemModelFactory::Instance();
  sut.RegisterCreator("Make-Sub", factory_1);

  auto nbFactories = sut.RegisteredCreatorsCount();

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.RegisterCreator("Make-Sub"s, factory_2));

  // ---------------- Verify
  //
  CxxTest::setAbortTestOnFail(true);

  TS_ASSERT_EQUALS (sut.RegisteredCreatorsCount(), nbFactories);
  TS_ASSERT_TRUE   (sut.HasCreator("Make-Sub"));

  auto node = sut.Create("Make-Sub", sm, "test");

  // ---------------- Verify
  //
  TS_ASSERT_NOT_NULLPTR (node);

  tap->AppendChild(node);

  // Check with pretty printer that this is the 2nd creator function that is used
  auto prettyPrint = PrettyPrinter::PrettyPrint(*tap, PrettyPrinterOptions::DisplayValueAuto
                                                    | PrettyPrinterOptions::ShowNodeIsIgnored);
  TS_ASSERT_EQUALS (prettyPrint,
                    "[Access_I](0)  \"Tap\"\n"
                    " [Register](1)  \"Tap_IR\", length: 8, Hold value: true, bypass: 0xFF\n"
                    " [Linker](2)    \"Tap_DR_Mux\"\n"
                    "  :Selector:(1)  \"Tap_IR\"\n"
                    "  [Register](3)  \"Tap_BPY\", length: 1, bypass: 0b1\n"
                    "  [Chain](5)     \"test\"\n"
                    "   [Register](4)  \"test_ctrl\", length: 1, Hold value: true, bypass: 0b0\n"
                    "   [Linker](6)    \"test_mux\"\n"
                    "    :Selector:(4)  \"test_ctrl\"\n"
                    "    [Register](7)  \"BitReg\", length: 8, bypass: 0x67"
                   );
}








//===========================================================================
// End of UT_SystemModelFactory.cpp
//===========================================================================
