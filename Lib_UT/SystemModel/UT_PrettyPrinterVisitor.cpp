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
  auto expected = string("[Access_I](0)  \"Access interface name\"");
  TS_ASSERT_EQUALS (got, expected);
}

//! Checks PrettyPrinterVisitor::VisitAccessInterface() with verbose mode
//!
void UT_PrettyPrinterVisitor::test_VisitAccessInterface_Verbose ()
{
  // ---------------- Setup
  //
  AccessInterface accessInterface("Access interface name", nullptr);

  PrettyPrinterVisitor sut;
  sut.SetVerbose(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitAccessInterface(accessInterface));

  // ---------------- Verify
  //
  auto got      = sut.GetPrettyPrint();
  auto expected = string("[Access_I](0)  \"Access interface name\", pending: false, has_condition: false, priority: 0");
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
  auto expected = string("[Chain](0)     \"Chain name\"");
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinterVisitor::VisitChain() with verbose mode
//!
void UT_PrettyPrinterVisitor::test_VisitChain_Verbose ()
{
  // ---------------- Setup
  //
  Chain chain("Chain name");

  PrettyPrinterVisitor sut;
  sut.SetVerbose(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitChain(chain));

  // ---------------- Verify
  //
  auto got      = sut.GetPrettyPrint();
  auto expected = string("[Chain](0)     \"Chain name\", pending: false, has_condition: false, priority: 0");
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
  auto expected = string("[Linker](1)    \"Linker name\"");
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinterVisitor::VisitLinker() with verbose mode
//!
void UT_PrettyPrinterVisitor::test_VisitLinker_Verbose ()
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
  sut.SetVerbose(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitLinker(linker));

  // ---------------- Verify
  //
  auto got      = sut.GetPrettyPrint();
  auto expected = string("[Linker](1)    \"Linker name\", pending: false, has_condition: false, priority: 0");
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
  auto expected = string("[Register](0)  \"A register name\", length: 9, bypass: 1111_1111:0");
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
  auto expected = string("[Register](0)  \"A register name\", length: 9, bypass:            1111_1111:0\n"
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
  auto expected = string("[Tap](0)       \"Tap name\"");
  TS_ASSERT_EQUALS (got, expected);
}

//! Checks PrettyPrinterVisitor::VisitTap()
//!
void UT_PrettyPrinterVisitor::test_VisitTap_Verbose ()
{
  // ---------------- Setup
  //
  Tap tap("Tap name", nullptr);

  PrettyPrinterVisitor sut;
  sut.SetVerbose(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitTap(tap));

  // ---------------- Verify
  //
  auto got      = sut.GetPrettyPrint();
  auto expected = string("[Tap](0)       \"Tap name\", pending: false, has_condition: false, priority: 0");
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
  auto expected = string("[Chain](0)     \"Chain\"\n"
                         " [Register](1)  \"Reg_1\", length: 6, bypass: 1010_11");

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
  auto expected = string("[Chain](0)     \"Chain\"\n"
                         " [Register](1)  \"Reg_1\", length: 6, bypass: 1010_01\n"
                         " [Register](2)  \"Reg_2\", length: 6, bypass: 1010_10"
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
  auto expected = string("[Chain](0)     \"Chain\", pending: false, has_condition: false, priority: 0\n"
                         " [Register](1)  \"Reg_1\", length: 6, bypass:            1010_01\n"
                         "                                  , next_to_sut:       1010_01\n"
                         "                                  , last_to_sut:       1010_01\n"
                         "                                  , last_from_sut:     1010_01\n"
                         "                                  , expected_from_sut: 1010_01\n"
                         "                                  , pending: false, has_condition: false, priority: 0\n"
                         " [Register](2)  \"Reg_2\", length: 6, bypass:            1010_10\n"
                         "                                  , next_to_sut:       1010_10\n"
                         "                                  , last_to_sut:       1010_10\n"
                         "                                  , last_from_sut:     1010_10\n"
                         "                                  , expected_from_sut: 1010_10\n"
                         "                                  , pending: false, has_condition: false, priority: 0\n"
                         " [Register](3)  \"Register Foo\", length: 15, bypass:            1110_1110:1111_101\n"
                         "                                          , next_to_sut:       1110_1110:1111_101\n"
                         "                                          , last_to_sut:       1110_1110:1111_101\n"
                         "                                          , last_from_sut:     1110_1110:1111_101\n"
                         "                                          , expected_from_sut: 1110_1110:1111_101\n"
                         "                                          , pending: false, has_condition: false, priority: 0"
                        );
  TS_ASSERT_EQUALS (got, expected);
}

//! Checks PrettyPrinterVisitor::VisitLinker() when there are several child beneath
//!
void UT_PrettyPrinterVisitor::test_VisitLinker_with_Child ()
{
  // ---------------- Setup
  //
  auto chain = make_shared<Chain>    ("Chain");
  auto reg_1 = make_shared<Register> ("Reg_1", BinaryVector::CreateFromBinaryString("1010_1"));
  auto reg_2 = make_shared<Register> ("Reg_2", BinaryVector::CreateFromBinaryString("1010_10"));
  auto reg_3 = make_shared<Register> ("Reg_3", BinaryVector::CreateFromBinaryString("1010_111"));
  auto reg_a = make_shared<Register> ("R_A",   BinaryVector::CreateFromBinaryString("1110_1110:1111_101"));
  auto reg_b = make_shared<Register> ("R_b",   BinaryVector::CreateFromBinaryString("1110_1110:1111_1010:1110"));

  chain->AppendChild(reg_1);
  chain->AppendChild(reg_2);
  chain->AppendChild(reg_3);

  auto muxNode = make_shared<Register>("Selector", BinaryVector::CreateFromBinaryString("01"));
  auto pathSelector   = make_shared<DefaultBinaryPathSelector>(muxNode, 2);
  auto linker         = make_shared<Linker>("Link", pathSelector);
  linker->AppendChild(chain);
  linker->AppendChild(reg_a);
  linker->AppendChild(reg_b);

  PrettyPrinterVisitor sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitLinker(*linker));

  // ---------------- Verify
  //
  auto got      = sut.GetPrettyPrint();
  auto expected = string("[Linker](7)    \"Link\"\n"
                         " [Chain](0)     \"Chain\"\n"
                         "  [Register](1)  \"Reg_1\", length: 5, bypass: 1010_1\n"
                         "  [Register](2)  \"Reg_2\", length: 6, bypass: 1010_10\n"
                         "  [Register](3)  \"Reg_3\", length: 7, bypass: 1010_111\n"
                         " [Register](4)  \"R_A\", length: 15, bypass: 1110_1110:1111_101\n"
                         " [Register](5)  \"R_b\", length: 20, bypass: 1110_1110:1111_1010:1110"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinterVisitor::VisitLinker() when there are several child beneath
//! and verbose mode
void UT_PrettyPrinterVisitor::test_VisitLinker_with_Child_Verbose ()
{
  // ---------------- Setup
  //
  auto chain = make_shared<Chain>    ("Chain");
  auto reg_1 = make_shared<Register> ("Reg_1", BinaryVector::CreateFromBinaryString("1010_1"));
  auto reg_2 = make_shared<Register> ("Reg_2", BinaryVector::CreateFromBinaryString("1010_10"));
  auto reg_3 = make_shared<Register> ("Reg_3", BinaryVector::CreateFromBinaryString("1010_111"));
  auto reg_a = make_shared<Register> ("R_A",   BinaryVector::CreateFromBinaryString("1110_1110:1111_101"));
  auto reg_b = make_shared<Register> ("R_b",   BinaryVector::CreateFromBinaryString("1110_1110:1111_1010:1110"));

  chain->AppendChild(reg_1);
  chain->AppendChild(reg_2);
  chain->AppendChild(reg_3);

  auto muxNode = make_shared<Register>("Selector", BinaryVector::CreateFromBinaryString("01"));
  auto pathSelector   = make_shared<DefaultBinaryPathSelector>(muxNode, 2);
  auto linker         = make_shared<Linker>("Link", pathSelector);
  linker->AppendChild(chain);
  linker->AppendChild(reg_a);
  linker->AppendChild(reg_b);

  PrettyPrinterVisitor sut;
  sut.SetVerbose(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitLinker(*linker));

  // ---------------- Verify
  //
  auto got      = sut.GetPrettyPrint();
  auto expected = string("[Linker](7)    \"Link\", pending: false, has_condition: false, priority: 0\n"
                         " [Chain](0)     \"Chain\", pending: false, has_condition: false, priority: 0\n"
                         "  [Register](1)  \"Reg_1\", length: 5, bypass:            1010_1\n"
                         "                                   , next_to_sut:       1010_1\n"
                         "                                   , last_to_sut:       1010_1\n"
                         "                                   , last_from_sut:     1010_1\n"
                         "                                   , expected_from_sut: 1010_1\n"
                         "                                   , pending: false, has_condition: false, priority: 0\n"
                         "  [Register](2)  \"Reg_2\", length: 6, bypass:            1010_10\n"
                         "                                   , next_to_sut:       1010_10\n"
                         "                                   , last_to_sut:       1010_10\n"
                         "                                   , last_from_sut:     1010_10\n"
                         "                                   , expected_from_sut: 1010_10\n"
                         "                                   , pending: false, has_condition: false, priority: 0\n"
                         "  [Register](3)  \"Reg_3\", length: 7, bypass:            1010_111\n"
                         "                                   , next_to_sut:       1010_111\n"
                         "                                   , last_to_sut:       1010_111\n"
                         "                                   , last_from_sut:     1010_111\n"
                         "                                   , expected_from_sut: 1010_111\n"
                         "                                   , pending: false, has_condition: false, priority: 0\n"
                         " [Register](4)  \"R_A\", length: 15, bypass:            1110_1110:1111_101\n"
                         "                                 , next_to_sut:       1110_1110:1111_101\n"
                         "                                 , last_to_sut:       1110_1110:1111_101\n"
                         "                                 , last_from_sut:     1110_1110:1111_101\n"
                         "                                 , expected_from_sut: 1110_1110:1111_101\n"
                         "                                 , pending: false, has_condition: false, priority: 0\n"
                         " [Register](5)  \"R_b\", length: 20, bypass:            1110_1110:1111_1010:1110\n"
                         "                                 , next_to_sut:       1110_1110:1111_1010:1110\n"
                         "                                 , last_to_sut:       1110_1110:1111_1010:1110\n"
                         "                                 , last_from_sut:     1110_1110:1111_1010:1110\n"
                         "                                 , expected_from_sut: 1110_1110:1111_1010:1110\n"
                         "                                 , pending: false, has_condition: false, priority: 0"
                        );
  TS_ASSERT_EQUALS (got, expected);
}

//! Checks PrettyPrinterVisitor::VisitAccessInterface() when there are several child beneath
//!
void UT_PrettyPrinterVisitor::test_VisitAccessInterface_with_Child ()
{
  // ---------------- Setup
  //
  auto chain = make_shared<Chain>    ("Chain");
  auto reg_1 = make_shared<Register> ("Reg_1", BinaryVector::CreateFromBinaryString("1010_1"));
  auto reg_2 = make_shared<Register> ("Reg_2", BinaryVector::CreateFromBinaryString("1010_10"));
  auto reg_3 = make_shared<Register> ("Reg_3", BinaryVector::CreateFromBinaryString("1010_111"));
  auto reg_a = make_shared<Register> ("R_A",   BinaryVector::CreateFromBinaryString("1110_1110:1111_101"));
  auto reg_b = make_shared<Register> ("R_b",   BinaryVector::CreateFromBinaryString("1110_1110:1111_1010:1110"));

  chain->AppendChild(reg_1);
  chain->AppendChild(reg_2);
  chain->AppendChild(reg_3);

  auto muxNode         = make_shared<Register>("Selector", BinaryVector::CreateFromBinaryString("01"));
  auto pathSelector    = make_shared<DefaultBinaryPathSelector>(muxNode, 2);
  auto linker          = make_shared<Linker>("Link", pathSelector);
  auto accessInterface = make_shared<AccessInterface>("Access interface name", nullptr);

  accessInterface->AppendChild(chain);
  accessInterface->AppendChild(linker);
  linker->AppendChild(reg_a);
  linker->AppendChild(reg_b);


  PrettyPrinterVisitor sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitAccessInterface(*accessInterface));

  // ---------------- Verify
  //
  auto got      = sut.GetPrettyPrint();
  auto expected = string("[Access_I](8)  \"Access interface name\"\n"
                         " [Chain](0)     \"Chain\"\n"
                         "  [Register](1)  \"Reg_1\", length: 5, bypass: 1010_1\n"
                         "  [Register](2)  \"Reg_2\", length: 6, bypass: 1010_10\n"
                         "  [Register](3)  \"Reg_3\", length: 7, bypass: 1010_111\n"
                         " [Linker](7)    \"Link\"\n"
                         "  [Register](4)  \"R_A\", length: 15, bypass: 1110_1110:1111_101\n"
                         "  [Register](5)  \"R_b\", length: 20, bypass: 1110_1110:1111_1010:1110"
                        );
  TS_ASSERT_EQUALS (got, expected);
}

//===========================================================================
// End of UT_PrettyPrinterVisitor.cpp
//===========================================================================
