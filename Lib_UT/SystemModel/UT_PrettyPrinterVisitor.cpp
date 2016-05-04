//===========================================================================
//                           UT_PrettyPrinterVisitor.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_PrettyPrinterVisitor.cpp
//!
//! Implements test fixture for testing PrettyPrinterVisitor
//!
//===========================================================================

#include "UT_PrettyPrinterVisitor.hpp"
#include "PrettyPrinterVisitor.hpp"
#include "SystemModelNodes.hpp"
#include "DefaultBinaryPathSelector.hpp"

#include <cxxtest/ValueTraits.h>

using std::string;
using std::make_shared;

using namespace mast;

//! Initializes tests (called for each test)
//!
void UT_PrettyPrinterVisitor::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}


//! Checks PrettyPrinterVisitor constructor
//!
void UT_PrettyPrinterVisitor::test_Constructor ()
{
  // ---------------- Exercise
  //
  PrettyPrinterVisitor sut;

  // ---------------- Verify
  //
  TS_ASSERT_EQUALS (sut.GetPrettyPrint(), "");
}

//! Checks PrettyPrinterVisitor::VisitAccessInterface()
//!
void UT_PrettyPrinterVisitor::test_VisitAccessInterface ()
{
  // ---------------- Setup
  //
  AccessInterface accessInterface("Access interface name", nullptr);

  PrettyPrinterVisitor sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitAccessInterface(accessInterface));

  // ---------------- Verify
  //
  auto got      = sut.GetPrettyPrint();
  auto expected = string("(0) [Access_I] \"Access interface name\"");
  TS_ASSERT_EQUALS (got, expected);
}

//! Checks PrettyPrinterVisitor::VisitChain()
//!
void UT_PrettyPrinterVisitor::test_VisitChain ()
{
  // ---------------- Setup
  //
  Chain chain("Chain name");

  PrettyPrinterVisitor sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitChain(chain));

  // ---------------- Verify
  //
  auto got      = sut.GetPrettyPrint();
  auto expected = string("(0) [Chain]    \"Chain name\"");
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinterVisitor::VisitLinker()
//!
void UT_PrettyPrinterVisitor::test_VisitLinker ()
{
  // ---------------- Setup
  //
  auto bypassSequence = BinaryVector::CreateFromBinaryString("000");
  auto isInverted     = false;
  auto canSelectNone  = false;
  auto associatedNode = make_shared<Register>("My register name", bypassSequence);
  auto pathSelector   = make_shared<DefaultBinaryPathSelector>(associatedNode, 5, isInverted, canSelectNone);

  auto linker = Linker("Linker name", pathSelector);

  PrettyPrinterVisitor sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitLinker(linker));

  // ---------------- Verify
  //
  auto got      = sut.GetPrettyPrint();
  auto expected = string("(1) [Linker]   \"Linker name\"");
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinterVisitor::VisitRegister()
//!
void UT_PrettyPrinterVisitor::test_VisitRegister ()
{
  // ---------------- Setup
  //
  Register reg("A register name", BinaryVector::CreateFromBinaryString("1111_1111:0"));
  PrettyPrinterVisitor sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitRegister(reg));

  // ---------------- Verify
  //
  auto got      = sut.GetPrettyPrint();
  auto expected = string("(0) [Register] \"A register name\", length: 9, bypass: 1111_1111:0");
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinterVisitor::VisitRegister() with verbose mode
//!
void UT_PrettyPrinterVisitor::test_VisitRegister_Verbose ()
{
  // ---------------- Setup
  //
  Register reg("A register name", BinaryVector::CreateFromBinaryString("1111_1111:0"));

  PrettyPrinterVisitor sut;
  sut.SetVerbose(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitRegister(reg));

  // ---------------- Verify
  //
  auto got      = sut.GetPrettyPrint();
  auto expected = string("(0) [Register] \"A register name\", length: 9, bypass:            1111_1111:0\n"
                         "                                           , next_to_sut:       1111_1111:0\n"
                         "                                           , last_to_sut:       1111_1111:0\n"
                         "                                           , last_from_sut:     1111_1111:0\n"
                         "                                           , expected_from_sut: 1111_1111:0\n"
                         "                                           , pending: false, has_condition: false, priority: 0"
                        );
  TS_ASSERT_EQUALS (got, expected);
}



//! Checks PrettyPrinterVisitor::VisitTap()
//!
void UT_PrettyPrinterVisitor::test_VisitTap ()
{
  // ---------------- Setup
  //
  Tap tap("Tap name", nullptr);

  PrettyPrinterVisitor sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitTap(tap));

  // ---------------- Verify
  //
  auto got      = sut.GetPrettyPrint();
  auto expected = string("(0) [Tap]      \"Tap name\"");
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinterVisitor::VisitChain() when there one register
//! managed by the chain
//!
void UT_PrettyPrinterVisitor::test_VisitChain_with_Register ()
{
  // ---------------- Setup
  //
  auto chain = make_shared<Chain>     ("Chain");
  auto reg   = make_shared<Register>  ("Reg_1", BinaryVector::CreateFromBinaryString("1010_11"));
  chain->AppendChild(reg);

  PrettyPrinterVisitor sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitChain(*chain));

  // ---------------- Verify
  //
  auto got      = sut.GetPrettyPrint();
  auto expected = string("(0) [Chain]    \"Chain\"\n"
                         "-(1) [Register] \"Reg_1\", length: 6, bypass: 1010_11");

  TS_ASSERT_EQUALS (got, expected);
}

//! Checks PrettyPrinterVisitor::VisitChain() when there are registers
//! managed by the chain
//!
void UT_PrettyPrinterVisitor::test_VisitChain_with_Registers ()
{
  // ---------------- Setup
  //
  auto chain = make_shared<Chain>     ("Chain");
  auto reg_1 = make_shared<Register>  ("Reg_1", BinaryVector::CreateFromBinaryString("1010_01"));
  auto reg_2 = make_shared<Register>  ("Reg_2", BinaryVector::CreateFromBinaryString("1010_10"));
  chain->AppendChild(reg_1);
  chain->AppendChild(reg_2);

  PrettyPrinterVisitor sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitChain(*chain));

  // ---------------- Verify
  //
  auto got      = sut.GetPrettyPrint();
  auto expected = string("(0) [Chain]    \"Chain\"\n"
                         "-(1) [Register] \"Reg_1\", length: 6, bypass: 1010_01\n"
                         "-(2) [Register] \"Reg_2\", length: 6, bypass: 1010_10"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinterVisitor::VisitChain() when there are several registers
//! managed by the chain and verbose mode set
//!
void UT_PrettyPrinterVisitor::test_VisitChain_with_Registers_Verbose ()
{
  // ---------------- Setup
  //
  auto chain = make_shared<Chain>     ("Chain");
  auto reg_1 = make_shared<Register>  ("Reg_1", BinaryVector::CreateFromBinaryString("1010_01"));
  auto reg_2 = make_shared<Register>  ("Reg_2", BinaryVector::CreateFromBinaryString("1010_10"));
  auto reg_3 = make_shared<Register>  ("Register Foo", BinaryVector::CreateFromBinaryString("1110_1110:1111_101"));

  chain->AppendChild(reg_1);
  chain->AppendChild(reg_2);
  chain->AppendChild(reg_3);

  PrettyPrinterVisitor sut;
  sut.SetVerbose(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitChain(*chain));

  // ---------------- Verify
  //
  auto got      = sut.GetPrettyPrint();
  auto expected = string("(0) [Chain]    \"Chain\"\n"
                         "-(1) [Register] \"Reg_1\", length: 6, bypass:            1010_01\n"
                         "                                  , next_to_sut:       1010_01\n"
                         "                                  , last_to_sut:       1010_01\n"
                         "                                  , last_from_sut:     1010_01\n"
                         "                                  , expected_from_sut: 1010_01\n"
                         "                                  , pending: false, has_condition: false, priority: 0\n"
                         "-(2) [Register] \"Reg_2\", length: 6, bypass:            1010_10\n"
                         "                                  , next_to_sut:       1010_10\n"
                         "                                  , last_to_sut:       1010_10\n"
                         "                                  , last_from_sut:     1010_10\n"
                         "                                  , expected_from_sut: 1010_10\n"
                         "                                  , pending: false, has_condition: false, priority: 0\n"
                         "-(3) [Register] \"Register Foo\", length: 15, bypass:            1110_1110:1111_101\n"
                         "                                          , next_to_sut:       1110_1110:1111_101\n"
                         "                                          , last_to_sut:       1110_1110:1111_101\n"
                         "                                          , last_from_sut:     1110_1110:1111_101\n"
                         "                                          , expected_from_sut: 1110_1110:1111_101\n"
                         "                                          , pending: false, has_condition: false, priority: 0"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//===========================================================================
// End of UT_PrettyPrinterVisitor.cpp
//===========================================================================
