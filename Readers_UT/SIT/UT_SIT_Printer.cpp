//===========================================================================
//                           UT_SIT_Printer.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_SIT_Printer.cpp
//!
//! Implements test fixture for testing SIT_Printer
//!
//===========================================================================

#include "UT_SIT_Printer.hpp"
#include "SIT_Printer.hpp"
#include "SystemModelNodes.hpp"
#include "TestModelBuilder.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "SystemModel.hpp"

#include <cxxtest/ValueTraits.h>

using std::string;
using std::experimental::string_view;
using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;
using std::make_shared;

using namespace mast;
using namespace test;

//! Initializes tests (called for each test)
//!
void UT_SIT_Printer::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}


//! Checks SIT_Printer constructor
//!
void UT_SIT_Printer::test_Constructor ()
{
  // ---------------- Exercise
  //
  SIT_Printer sut;

  // ---------------- Verify
  //
  TS_ASSERT_EMPTY (sut.SIT_Text());
}

//! Checks SIT_Printer::VisitAccessInterface()
//!
void UT_SIT_Printer::test_VisitAccessInterface ()
{
  // ---------------- Setup
  //
  AccessInterface accessInterface("Access interface name", nullptr);

  SIT_Printer sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitAccessInterface(accessInterface));

  // ---------------- Verify
  //
  auto got      = sut.SIT_Text();
  auto expected = "ACCESS_INTERFACE \"Access interface name\" PROTOCOL_NOT_SET\n"
                  "(\n"
                  ")";
  TS_ASSERT_EQUALS (got, expected);
}





//! Checks SIT_Printer::VisitChain()
//!
void UT_SIT_Printer::test_VisitChain ()
{
  // ---------------- Setup
  //
  Chain chain("Chain name");

  SIT_Printer sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitChain(chain));

  // ---------------- Verify
  //
  auto got      = sut.SIT_Text();
  auto expected = "CHAIN \"Chain name\"\n"
                  "{\n"
                  "}";
  TS_ASSERT_EQUALS (got, expected);
}



//! Checks SIT_Printer::VisitLinker()
//!
void UT_SIT_Printer::test_VisitLinker ()
{
  // ---------------- Setup
  //
  auto chain  = make_shared<Chain>     ("Chain");
  auto muxReg = make_shared<Register>  ("Mux register name", BinaryVector::CreateFromBinaryString("000"));
  chain->AppendChild(muxReg);
  auto pathSelector   = make_shared<DefaultBinaryPathSelector>(muxReg, 5u);

  auto linker = Linker("Linker name", pathSelector);

  SIT_Printer sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitLinker(linker));

  // ---------------- Verify
  //
  auto got      = sut.SIT_Text();
  auto expected = "LINKER \"Linker name\" Binary \"Mux register name\" 5 NOT_IN_PATH\n"
                  "(\n"
                  ")";
  TS_ASSERT_EQUALS (got, expected);
}




//! Checks SIT_Printer::VisitRegister()
//!
void UT_SIT_Printer::test_VisitRegister ()
{
  // ---------------- Setup
  //

  Register reg("A register name", BinaryVector::CreateFromBinaryString("1111_1111:0"));

  SIT_Printer sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitRegister(reg));

  // ---------------- Verify
  //
  auto got      = sut.SIT_Text();
  auto expected = string("REGISTER \"A register name\" 9 Bypass: \"0xFF/b0\"");
  TS_ASSERT_EQUALS (got, expected);
}




//! Checks SIT_Printer::VisitChain() when there one register
//! managed by the chain
//!
void UT_SIT_Printer::test_VisitChain_with_Register ()
{
  // ---------------- Setup
  //
  auto chain = make_shared<Chain>     ("Chain");
  auto reg   = make_shared<Register>  ("Reg_1", BinaryVector::CreateFromBinaryString("1010_11"));
  chain->AppendChild(reg);

  SIT_Printer sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitChain(*chain));

  // ---------------- Verify
  //
  auto got      = sut.SIT_Text();
  auto expected = "CHAIN Chain\n"
                  "{\n"
                  "  REGISTER Reg_1 6 Bypass: \"0b1010_11\"\n"
                  "}";


  TS_ASSERT_EQUALS (got, expected);
}

//! Checks SIT_Printer::VisitChain() when there are registers
//! managed by the chain
//!
void UT_SIT_Printer::test_VisitChain_with_Registers ()
{
  // ---------------- Setup
  //
  auto chain = make_shared<Chain>     ("Chain");
  auto reg_1 = make_shared<Register>  ("Reg_1", BinaryVector::CreateFromBinaryString("1010_01"));
  auto reg_2 = make_shared<Register>  ("Reg_2", BinaryVector::CreateFromBinaryString("1010_10"));
  chain->AppendChild(reg_1);
  chain->AppendChild(reg_2);

  SIT_Printer sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitChain(*chain));

  // ---------------- Verify
  //
  auto got      = sut.SIT_Text();
  auto expected = "CHAIN Chain\n"
                  "\{\n"
                  "  REGISTER Reg_1 6 Bypass: \"0b1010_01\"\n"
                  "  REGISTER Reg_2 6 Bypass: \"0b1010_10\"\n"
                  "}";

  TS_ASSERT_EQUALS (got, expected);
}



//! Checks SIT_Printer::VisitLinker() when there are several child beneath
//!
void UT_SIT_Printer::test_VisitLinker_with_Child ()
{
  // ---------------- Setup
  //
  auto chain  = make_shared<Chain>    ("Chain");
  auto reg_1  = make_shared<Register> ("Reg_1",   BinaryVector::CreateFromBinaryString("1010_1"));
  auto reg_2  = make_shared<Register> ("Reg_2",   BinaryVector::CreateFromBinaryString("1010_10"));
  auto reg_3  = make_shared<Register> ("Reg_3",   BinaryVector::CreateFromBinaryString("1010_111"));
  auto reg_a  = make_shared<Register> ("R_A",     BinaryVector::CreateFromBinaryString("1110_1110:1111_101"));
  auto reg_b  = make_shared<Register> ("R_b",     BinaryVector::CreateFromBinaryString("1110_1110:1111_1010:1110"));
  auto muxReg = make_shared<Register> ("Mux_Reg", BinaryVector::CreateFromBinaryString("01"));

  chain->AppendChild(muxReg);
  chain->AppendChild(reg_1);
  chain->AppendChild(reg_2);
  chain->AppendChild(reg_3);

  auto pathSelector   = make_shared<DefaultBinaryPathSelector>(muxReg, 3);
  auto linker         = make_shared<Linker>("Link", pathSelector);
  linker->AppendChild(chain);
  linker->AppendChild(reg_a);
  linker->AppendChild(reg_b);

  SIT_Printer sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitLinker(*linker));

  // ---------------- Verify
  //
  auto got      = sut.SIT_Text();
  auto expected = "LINKER Link Binary Mux_Reg 3 NOT_IN_PATH\n"
                  "(\n"
                  "  CHAIN Chain\n"
                  "  {\n"
                  "    REGISTER Mux_Reg 2 Bypass: \"0b01\"\n"
                  "    REGISTER Reg_1 5 Bypass: \"0b1010_1\"\n"
                  "    REGISTER Reg_2 6 Bypass: \"0b1010_10\"\n"
                  "    REGISTER Reg_3 7 Bypass: \"0b1010_111\"\n"
                  "  }\n"
                  "  REGISTER R_A 15 Bypass: \"0xEEF/b101\"\n"
                  "  REGISTER R_b 20 Bypass: \"0xEEFA_E\"\n"
                  ")";
  TS_ASSERT_EQUALS (got, expected);
}



//! Checks SIT_Printer::VisitAccessInterface() when there are several child beneath
//!
void UT_SIT_Printer::test_VisitAccessInterface_with_Child ()
{
  // ---------------- Setup
  //
  auto chain   = make_shared<Chain>    ("Chain name");
  auto reg_1   = make_shared<Register> ("Reg_1",        BinaryVector::CreateFromBinaryString("1010_1"));
  auto reg_2   = make_shared<Register> ("Reg_2",        BinaryVector::CreateFromBinaryString("1010_10"));
  auto reg_3   = make_shared<Register> ("Reg_3",        BinaryVector::CreateFromBinaryString("1010_111"));
  auto reg_a   = make_shared<Register> ("R_A",          BinaryVector::CreateFromBinaryString("1110_1110:1111_101"));
  auto reg_b   = make_shared<Register> ("the register", BinaryVector::CreateFromBinaryString("1110_1110:1111_1010:1110"));
  auto muxNode = make_shared<Register> ("SReg",         BinaryVector::CreateFromBinaryString("01"));

  chain->AppendChild(reg_1);
  chain->AppendChild(reg_2);
  chain->AppendChild(reg_3);
  chain->AppendChild(muxNode);

  auto pathSelector    = make_shared<DefaultBinaryPathSelector>(muxNode, 2);
  auto linker          = make_shared<Linker>("Linker name", pathSelector);
  auto accessInterface = make_shared<AccessInterface>("Access interface name", nullptr);

  accessInterface->AppendChild(chain);
  accessInterface->AppendChild(linker);
  linker->AppendChild(reg_a);
  linker->AppendChild(reg_b);


  SIT_Printer sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitAccessInterface(*accessInterface));

  // ---------------- Verify
  //
  auto got      = sut.SIT_Text();
  auto expected = "ACCESS_INTERFACE \"Access interface name\" PROTOCOL_NOT_SET\n"
                  "(\n"
                  "  CHAIN \"Chain name\"\n"
                  "  {\n"
                  "    REGISTER Reg_1 5 Bypass: \"0b1010_1\"\n"
                  "    REGISTER Reg_2 6 Bypass: \"0b1010_10\"\n"
                  "    REGISTER Reg_3 7 Bypass: \"0b1010_111\"\n"
                  "    REGISTER SReg 2 Bypass: \"0b01\"\n"
                  "  }\n"
                  "  LINKER \"Linker name\" Binary SReg 2 NOT_IN_PATH\n"
                  "  (\n"
                  "    REGISTER R_A 15 Bypass: \"0xEEF/b101\"\n"
                  "    REGISTER \"the register\" 20 Bypass: \"0xEEFA_E\"\n"
                  "  )\n"
                  ")";
  TS_ASSERT_EQUALS (got, expected);
}




//! Checks SIT_Printer::Visit_xxx() with a tap
//!
void UT_SIT_Printer::test_VisitTap ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  string_view noName;
  uint32_t    irBitsCount   = 6u;
  uint32_t    muxPathsCount = 5u;
  auto        tap           = builder.Create_JTAG_TAP(noName, irBitsCount, muxPathsCount);

  SIT_Printer sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut)); // Will indirectly call Visit for several types of nodes

  // ---------------- Verify
  //
  auto got      = sut.SIT_Text();
  auto expected = "ACCESS_INTERFACE 1149_1_TAP Loopback\n"
                  "(\n"
                  "  REGISTER TAP_IR 6 Hold_value Bypass: \"0b1111_11\"\n"
                  "  LINKER TAP_DR_Mux Table_Based TAP_IR 5 NOT_IN_PATH\n"
                  "  (\n"
                  "    REGISTER TAP_BPY 1 Bypass: \"0b1\"\n"
                  "  )\n"
                  ")";
  TS_ASSERT_EQUALS (got, expected);
  TS_WARN ("Table based not defined");
}


//! Checks SIT_Printer::Visit_xxx() with a tap with added sub-nodes
//!
void UT_SIT_Printer::test_VisitTap_With_SubNodes ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  string_view noName;
  uint32_t    irBitsCount   = 6u;
  uint32_t    muxPathsCount = 5u;
  auto        tap           = builder.Create_JTAG_TAP(noName, irBitsCount, muxPathsCount);

  auto chain = sm.CreateChain("Chain name", tap);
  auto reg_1 = sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("1010_01"), tap);
  auto reg_2 = sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("1010_10"), tap);

  SIT_Printer sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut)); // Will indirectly call Visit for several types of nodes

  // ---------------- Verify
  //
  auto got      = sut.SIT_Text();
  auto expected = "ACCESS_INTERFACE 1149_1_TAP Loopback\n"
                  "(\n"
                  "  REGISTER TAP_IR 6 Hold_value Bypass: \"0b1111_11\"\n"
                  "  LINKER TAP_DR_Mux Table_Based TAP_IR 5 NOT_IN_PATH\n"
                  "  (\n"
                  "    REGISTER TAP_BPY 1 Bypass: \"0b1\"\n"
                  "    CHAIN \"Chain name\"\n"
                  "    {\n"
                  "    }\n"
                  "    REGISTER Reg_1 6 Bypass: \"0b1010_01\"\n"
                  "    REGISTER Reg_2 6 Bypass: \"0b1010_10\"\n"
                  "  )\n"
                  ")";
  TS_ASSERT_EQUALS (got, expected);
  TS_WARN ("Table based not defined");
}


//! Checks SIT_Printer::MakeSIT() with default options
//!
void UT_SIT_Printer::test_MakeSIT ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP ("", 5u, 4u);
  auto chain = sm.CreateChain    ("Chain name", tap);
  auto reg_1 = sm.CreateRegister ("Reg_1", BinaryVector::CreateFromBinaryString("1010_01"), tap);
  auto reg_2 = sm.CreateRegister ("Reg_2", BinaryVector::CreateFromBinaryString("1010_10"), tap);

  // ---------------- Exercise
  //
  auto got = SIT_Printer::MakeSIT(tap);

  // ---------------- Verify
  //
  auto expected = "ACCESS_INTERFACE 1149_1_TAP Loopback\n"
                  "(\n"
                  "  REGISTER TAP_IR 5 Hold_value Bypass: \"0b1111_1\"\n"
                  "  LINKER TAP_DR_Mux Table_Based TAP_IR 4 NOT_IN_PATH\n"
                  "  (\n"
                  "    REGISTER TAP_BPY 1 Bypass: \"0b1\"\n"
                  "    CHAIN \"Chain name\"\n"
                  "    {\n"
                  "    }\n"
                  "    REGISTER Reg_1 6 Bypass: \"0b1010_01\"\n"
                  "    REGISTER Reg_2 6 Bypass: \"0b1010_10\"\n"
                  "  )\n"
                  ")";
  TS_ASSERT_EQUALS (got, expected);
  TS_WARN ("Table based not defined");
}



//===========================================================================
// End of UT_SIT_Printer.cpp
//===========================================================================
