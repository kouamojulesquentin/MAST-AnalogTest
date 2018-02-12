//===========================================================================
//                           UT_PrettyPrinter.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_PrettyPrinter.cpp
//!
//! Implements test fixture for testing PrettyPrinter
//!
//===========================================================================

#include "UT_PrettyPrinter.hpp"
#include "PrettyPrinter.hpp"
#include "SystemModelNodes.hpp"
#include "TestModelBuilder.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "SystemModel.hpp"
#include "VirtualRegister.hpp"
#include "RegistersAlias.hpp"
#include "EnumsUtility.hpp"

#include "Mast_Core_Traits.hpp"

using std::string;
using std::experimental::string_view;
using std::make_shared;

using namespace mast;
using namespace test;

//! Initializes tests (called for each test)
//!
void UT_PrettyPrinter::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}


//! Checks PrettyPrinter constructor
//!
void UT_PrettyPrinter::test_Constructor ()
{
  // ---------------- Exercise
  //
  PrettyPrinter sut;

  // ---------------- Verify
  //
  TS_ASSERT_EMPTY (sut.PrettyPrint());
}

//! Checks PrettyPrinter::VisitAccessInterface()
//!
void UT_PrettyPrinter::test_VisitAccessInterface ()
{
  // ---------------- Setup
  //
  AccessInterface accessInterface("Access interface name", nullptr);

  PrettyPrinter sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitAccessInterface(accessInterface));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Access_I](0)  \"Access interface name\"");
  TS_ASSERT_EQUALS (got, expected);
}

//! Checks PrettyPrinter::VisitAccessInterface() with verbose mode
//!
void UT_PrettyPrinter::test_VisitAccessInterface_Verbose ()
{
  // ---------------- Setup
  //
  AccessInterface accessInterface("Access interface name", nullptr);

  PrettyPrinter sut;
  sut.SetVerbose(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitAccessInterface(accessInterface));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Access_I](0)  \"Access interface name\", Protocol: Not set, pending: false, has_conditioner: false, priority: 0");
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::VisitAccessInterface() with ShowProtocol option
//!
void UT_PrettyPrinter::test_VisitAccessInterface_ShowProtocol ()
{
  // ---------------- Setup
  //
  AccessInterface accessInterface("Access interface name", nullptr);

  PrettyPrinter sut(PrettyPrinterOptions::ShowProtocol | PrettyPrinterOptions::ShowNodesIdentifier);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitAccessInterface(accessInterface));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Access_I](0)  \"Access interface name\", Protocol: Not set");
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::VisitChain()
//!
void UT_PrettyPrinter::test_VisitChain ()
{
  // ---------------- Setup
  //
  Chain chain("Chain name");

  PrettyPrinter sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitChain(chain));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Chain](0)     \"Chain name\"");
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::VisitChain() with verbose mode
//!
void UT_PrettyPrinter::test_VisitChain_Verbose ()
{
  // ---------------- Setup
  //
  Chain chain("Chain name");

  PrettyPrinter sut;
  sut.SetVerbose(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitChain(chain));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Chain](0)     \"Chain name\", pending: false, has_conditioner: false, priority: 0");
  TS_ASSERT_EQUALS (got, expected);
}

//! Checks PrettyPrinter::VisitLinker() when selector is based upon a single register
//!
void UT_PrettyPrinter::test_VisitLinker_SingleRegSelector ()
{
  // ---------------- Setup
  //
  auto chain  = make_shared<Chain>     ("Chain");
  auto muxReg = make_shared<Register>  ("Mux register name", BinaryVector::CreateFromBinaryString("000"));
  chain->AppendChild(muxReg);
  auto pathSelector   = make_shared<DefaultBinaryPathSelector>(muxReg, 5u);

  Linker linker("Linker name", pathSelector);

  PrettyPrinter sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitLinker(linker));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Linker](2)    \"Linker name\"\n"
                         " :Selector:(1)  \"Mux register name\"");
  TS_ASSERT_EQUALS (got, expected);
}





//! Checks PrettyPrinter::VisitLinker() with verbose mode
//!
void UT_PrettyPrinter::test_VisitLinker_SingleRegSelector_Verbose ()
{
  // ---------------- Setup
  //
  auto chain  = make_shared<Chain>     ("Chain");
  auto muxReg = make_shared<Register>  ("Mux register name", BinaryVector::CreateFromBinaryString("000"));
  chain->AppendChild(muxReg);
  auto pathSelector   = make_shared<DefaultBinaryPathSelector>(muxReg, 5u);

  Linker linker("Linker name", pathSelector);

  PrettyPrinter sut;
  sut.SetVerbose(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitLinker(linker));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Linker](2)    \"Linker name\", ignore_in_path: true, pending: false, has_conditioner: false, priority: 0\n"
                         " :Selector:(1)  \"Mux register name\""
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::VisitLinker() when selector is based upon a multiple registers
//!
void UT_PrettyPrinter::test_VisitLinker_MultipleRegSelector ()
{
  // ---------------- Setup
  //
  auto chain    = make_shared<Chain>    ("Chain");
  auto muxReg_1 = make_shared<Register> ("MuxReg_1", BinaryVector::CreateFromBinaryString("000"));
  auto muxReg_2 = make_shared<Register> ("MuxReg_2", BinaryVector::CreateFromBinaryString("11"));

  chain->AppendChild(muxReg_1);
  chain->AppendChild(muxReg_2);
  VirtualRegister virtualRegister;
  virtualRegister.Append(RegisterSlice{muxReg_1, IndexedRange(2u, 0u)});
  virtualRegister.Append(RegisterSlice{muxReg_2, IndexedRange(1u, 0u)});

  auto pathSelector = make_shared<DefaultBinaryPathSelector>(virtualRegister, 5u);

  Linker linker("Linker name", pathSelector);

  PrettyPrinter sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitLinker(linker));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Linker](3)    \"Linker name\"\n"
                         " :Selector:     \"MuxReg_1:MuxReg_2\"");
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::VisitLinker() when selector is based upon multiple, partial, registers
//!
void UT_PrettyPrinter::test_VisitLinker_MultiplePartialRegSelector ()
{
  // ---------------- Setup
  //
  auto chain    = make_shared<Chain>    ("Chain");
  auto muxReg_1 = make_shared<Register> ("MuxReg_1", BinaryVector::CreateFromBinaryString("000"));
  auto muxReg_2 = make_shared<Register> ("MuxReg_2", BinaryVector::CreateFromBinaryString("11"));

  chain->AppendChild(muxReg_1);
  chain->AppendChild(muxReg_2);
  VirtualRegister virtualRegister;
  virtualRegister.Append(RegisterSlice{muxReg_1, IndexedRange(2u, 1u)});
  virtualRegister.Append(RegisterSlice{muxReg_2, IndexedRange(1u, 1u)});
  virtualRegister.Append(RegisterSlice{muxReg_1, IndexedRange(0u, 0u)});

  auto pathSelector = make_shared<DefaultBinaryPathSelector>(virtualRegister, 5u);

  Linker linker("Linker name", pathSelector);

  PrettyPrinter sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitLinker(linker));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Linker](3)    \"Linker name\"\n"
                         " :Selector:     \"MuxReg_1[2:1]:MuxReg_2[1]:MuxReg_1[0]\"");
  TS_ASSERT_EQUALS (got, expected);
}



//! Checks PrettyPrinter::VisitRegister()
//!
void UT_PrettyPrinter::test_VisitRegister ()
{
  // ---------------- Setup
  //

  Register reg("A register name", BinaryVector::CreateFromBinaryString("1111_1111:0"));

  PrettyPrinter sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitRegister(reg));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Register](0)  \"A register name\", length: 9, bypass: 1111_1111:0");
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::VisitRegister() with verbose mode
//!
void UT_PrettyPrinter::test_VisitRegister_Verbose ()
{
  // ---------------- Setup
  //
  Register reg("A register name", BinaryVector::CreateFromBinaryString("1111_1111:0"));

  PrettyPrinter sut;
  sut.SetVerbose(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitRegister(reg));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Register](0)  \"A register name\", length: 9, bypass:            1111_1111:0\n"
                         "                                           , next_to_sut:       1111_1111:0\n"
                         "                                           , last_to_sut:       1111_1111:0\n"
                         "                                           , last_from_sut:     1111_1111:0\n"
                         "                                           , expected_from_sut: 1111_1111:0\n"
                         "                                           , pending: false, has_conditioner: false, priority: 0"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::VisitChain() when there one register
//! managed by the chain
//!
void UT_PrettyPrinter::test_VisitChain_with_Register ()
{
  // ---------------- Setup
  //
  auto chain = make_shared<Chain>     ("Chain");
  auto reg   = make_shared<Register>  ("Reg_1", BinaryVector::CreateFromBinaryString("1010_11"));
  chain->AppendChild(reg);

  PrettyPrinter sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitChain(*chain));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Chain](0)     \"Chain\"\n"
                         " [Register](1)  \"Reg_1\", length: 6, bypass: 1010_11");

  TS_ASSERT_EQUALS (got, expected);
}

//! Checks PrettyPrinter::VisitChain() when there are registers
//! managed by the chain
//!
void UT_PrettyPrinter::test_VisitChain_with_Registers ()
{
  // ---------------- Setup
  //
  auto chain = make_shared<Chain>     ("Chain");
  auto reg_1 = make_shared<Register>  ("Reg_1", BinaryVector::CreateFromBinaryString("1010_01"));
  auto reg_2 = make_shared<Register>  ("Reg_2", BinaryVector::CreateFromBinaryString("1010_10"));
  chain->AppendChild(reg_1);
  chain->AppendChild(reg_2);

  PrettyPrinter sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitChain(*chain));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Chain](0)     \"Chain\"\n"
                         " [Register](1)  \"Reg_1\", length: 6, bypass: 1010_01\n"
                         " [Register](2)  \"Reg_2\", length: 6, bypass: 1010_10"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::VisitChain() when there are several registers
//! managed by the chain and verbose mode set
//!
void UT_PrettyPrinter::test_VisitChain_with_Registers_Verbose ()
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

  PrettyPrinter sut;
  sut.SetVerbose(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitChain(*chain));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Chain](0)     \"Chain\", pending: false, has_conditioner: false, priority: 0\n"
                         " [Register](1)  \"Reg_1\", length: 6, bypass:            1010_01\n"
                         "                                  , next_to_sut:       1010_01\n"
                         "                                  , last_to_sut:       1010_01\n"
                         "                                  , last_from_sut:     1010_01\n"
                         "                                  , expected_from_sut: 1010_01\n"
                         "                                  , pending: false, has_conditioner: false, priority: 0\n"
                         " [Register](2)  \"Reg_2\", length: 6, bypass:            1010_10\n"
                         "                                  , next_to_sut:       1010_10\n"
                         "                                  , last_to_sut:       1010_10\n"
                         "                                  , last_from_sut:     1010_10\n"
                         "                                  , expected_from_sut: 1010_10\n"
                         "                                  , pending: false, has_conditioner: false, priority: 0\n"
                         " [Register](3)  \"Register Foo\", length: 15, bypass:            1110_1110:1111_101\n"
                         "                                          , next_to_sut:       1110_1110:1111_101\n"
                         "                                          , last_to_sut:       1110_1110:1111_101\n"
                         "                                          , last_from_sut:     1110_1110:1111_101\n"
                         "                                          , expected_from_sut: 1110_1110:1111_101\n"
                         "                                          , pending: false, has_conditioner: false, priority: 0"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::VisitChain() when it has aliases
//!
void UT_PrettyPrinter::test_VisitChain_with_Aliases ()
{
  // ---------------- Setup
  //
  auto chain = make_shared<Chain>     ("Chain");
  auto reg_1 = make_shared<Register>  ("Reg_1", BinaryVector::CreateFromBinaryString("1010_01"));
  auto reg_2 = make_shared<Register>  ("Reg_2", BinaryVector::CreateFromBinaryString("1010_10"));
  chain->AppendChild(reg_1);
  chain->AppendChild(reg_2);

  // Create 1st alias
  VirtualRegister virtualRegister_1;
  virtualRegister_1.Append({reg_1, IndexedRange{4, 1}});
  virtualRegister_1.Append({reg_2, IndexedRange{2, 1}});
  chain->AddAlias({"Foo"s, std::move(virtualRegister_1)});

  // Create 2nd alias
  VirtualRegister virtualRegister_2;
  virtualRegister_2.Append({reg_1, IndexedRange{5, 5}});
  virtualRegister_2.Append({reg_2, IndexedRange{1, 0}});
  chain->AddAlias({"Bar", std::move(virtualRegister_2)});

  PrettyPrinter sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitChain(*chain));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Chain](0)     \"Chain\"\n"
                         " Alias Foo: Reg_1[4:1], Reg_2[2:1]\n"
                         " Alias Bar: Reg_1[5], Reg_2[1:0]\n"
                         " [Register](1)  \"Reg_1\", length: 6, bypass: 1010_01\n"
                         " [Register](2)  \"Reg_2\", length: 6, bypass: 1010_10"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::VisitLinker() when there are several child beneath
//!
void UT_PrettyPrinter::test_VisitLinker_with_SingleRegSelector ()
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

  PrettyPrinter sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitLinker(*linker));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Linker](7)    \"Link\"\n"
                         " :Selector:(6)  \"Mux_Reg\"\n"
                         " [Chain](0)     \"Chain\"\n"
                         "  [Register](6)  \"Mux_Reg\", length: 2, bypass: 01\n"
                         "  [Register](1)  \"Reg_1\", length: 5, bypass: 1010_1\n"
                         "  [Register](2)  \"Reg_2\", length: 6, bypass: 1010_10\n"
                         "  [Register](3)  \"Reg_3\", length: 7, bypass: 1010_111\n"
                         " [Register](4)  \"R_A\", length: 15, bypass: 1110_1110:1111_101\n"
                         " [Register](5)  \"R_b\", length: 20, bypass: 1110_1110:1111_1010:1110"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::VisitLinker() when there are several child beneath
//! and verbose mode
void UT_PrettyPrinter::test_VisitLinker_with_SingleRegSelector_Verbose ()
{
  // ---------------- Setup
  //
  auto chain   = make_shared<Chain>    ("Chain");
  auto reg_1   = make_shared<Register> ("Reg_1", BinaryVector::CreateFromBinaryString("1010_1"));
  auto reg_2   = make_shared<Register> ("Reg_2", BinaryVector::CreateFromBinaryString("1010_10"));
  auto reg_3   = make_shared<Register> ("Reg_3", BinaryVector::CreateFromBinaryString("1010_111"));
  auto reg_a   = make_shared<Register> ("R_A",   BinaryVector::CreateFromBinaryString("1110_1110:1111_101"));
  auto reg_b   = make_shared<Register> ("R_b",   BinaryVector::CreateFromBinaryString("1110_1110:1111_1010:1110"));
  auto muxNode = make_shared<Register> ("Sel_1", BinaryVector::CreateFromBinaryString("01"));

  chain->AppendChild(muxNode);
  chain->AppendChild(reg_1);
  chain->AppendChild(reg_2);
  chain->AppendChild(reg_3);

  auto pathSelector   = make_shared<DefaultBinaryPathSelector>(muxNode, 2);
  auto linker         = make_shared<Linker>("Link", pathSelector);
  linker->AppendChild(reg_a);
  linker->AppendChild(reg_b);

  PrettyPrinter sut;
  sut.SetVerbose(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitLinker(*linker));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Linker](7)    \"Link\", ignore_in_path: true, pending: false, has_conditioner: false, priority: 0\n"
                         " :Selector:(6)  \"Sel_1\"\n"
                         " [Register](4)  \"R_A\", length: 15, bypass:            1110_1110:1111_101\n"
                         "                                 , next_to_sut:       1110_1110:1111_101\n"
                         "                                 , last_to_sut:       1110_1110:1111_101\n"
                         "                                 , last_from_sut:     1110_1110:1111_101\n"
                         "                                 , expected_from_sut: 1110_1110:1111_101\n"
                         "                                 , pending: false, has_conditioner: false, priority: 0\n"
                         " [Register](5)  \"R_b\", length: 20, bypass:            1110_1110:1111_1010:1110\n"
                         "                                 , next_to_sut:       1110_1110:1111_1010:1110\n"
                         "                                 , last_to_sut:       1110_1110:1111_1010:1110\n"
                         "                                 , last_from_sut:     1110_1110:1111_1010:1110\n"
                         "                                 , expected_from_sut: 1110_1110:1111_1010:1110\n"
                         "                                 , pending: false, has_conditioner: false, priority: 0"
                        );
  TS_ASSERT_EQUALS (got, expected);
}

//! Checks PrettyPrinter::VisitAccessInterface() when there are several child beneath
//!
void UT_PrettyPrinter::test_VisitAccessInterface_with_Child ()
{
  // ---------------- Setup
  //
  auto chain   = make_shared<Chain>    ("Chain name");
  auto reg_1   = make_shared<Register> ("Reg_1",        BinaryVector::CreateFromBinaryString("1010_1"));
  auto reg_2   = make_shared<Register> ("Reg_2",        BinaryVector::CreateFromBinaryString("1010_10"));
  auto reg_3   = make_shared<Register> ("Reg_3",        BinaryVector::CreateFromBinaryString("1010_111"));
  auto reg_a   = make_shared<Register> ("R_A",          BinaryVector::CreateFromBinaryString("1110_1110:1111_101"));
  auto reg_b   = make_shared<Register> ("the register", BinaryVector::CreateFromBinaryString("1110_1110:1111_1010:1110"));
  auto muxNode = make_shared<Register> ("Selector",     BinaryVector::CreateFromBinaryString("01"));

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


  PrettyPrinter sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitAccessInterface(*accessInterface));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Access_I](8)  \"Access interface name\"\n"
                         " [Chain](0)     \"Chain name\"\n"
                         "  [Register](1)  \"Reg_1\", length: 5, bypass: 1010_1\n"
                         "  [Register](2)  \"Reg_2\", length: 6, bypass: 1010_10\n"
                         "  [Register](3)  \"Reg_3\", length: 7, bypass: 1010_111\n"
                         "  [Register](6)  \"Selector\", length: 2, bypass: 01\n"
                         " [Linker](7)    \"Linker name\"\n"
                         "  :Selector:(6)  \"Selector\"\n"
                         "  [Register](4)  \"R_A\", length: 15, bypass: 1110_1110:1111_101\n"
                         "  [Register](5)  \"the register\", length: 20, bypass: 1110_1110:1111_1010:1110"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::VisitAccessInterface() when printing Registers in hex format
//!
void UT_PrettyPrinter::test_VisitAccessInterface_AutoFormat ()
{
  // ---------------- Setup
  //
  auto accessInterface = make_shared<AccessInterface>           ("Access interface name", nullptr);
  auto muxReg          = make_shared<Register>                  ("Mux_reg", BinaryVector::CreateFromBinaryString("01"));
  auto pathSelector    = make_shared<DefaultBinaryPathSelector> (muxReg, 2);
  auto linker          = make_shared<Linker>   ("Linker name",  pathSelector);
  auto chain           = make_shared<Chain>    ("Chain name");
  auto reg_1           = make_shared<Register> ("Reg_1",        BinaryVector::CreateFromBinaryString("1010_1101_1"));
  auto reg_2           = make_shared<Register> ("Reg_2",        BinaryVector::CreateFromBinaryString("1010_1101_10"));
  auto reg_3           = make_shared<Register> ("Reg_3",        BinaryVector::CreateFromBinaryString("1010_1101_111"));
  auto reg_a           = make_shared<Register> ("R_A",          BinaryVector::CreateFromBinaryString("1110_1101_1110_1111_101"));
  auto reg_b           = make_shared<Register> ("the register", BinaryVector::CreateFromHexString("FACE_DEAD:BEEF_CAFE:DECA"));

  accessInterface->AppendChild(muxReg);
  accessInterface->AppendChild(chain);
  accessInterface->AppendChild(linker);
  linker->AppendChild(reg_a);
  linker->AppendChild(reg_b);
  chain->AppendChild(reg_1);
  chain->AppendChild(reg_2);
  chain->AppendChild(reg_3);

  PrettyPrinter sut(PrettyPrinterOptions::DisplayValueAuto | PrettyPrinterOptions::ShowNodesIdentifier);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitAccessInterface(*accessInterface));

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string(
                         "[Access_I](0)  \"Access interface name\"\n"
                         " [Register](1)  \"Mux_reg\", length: 2, bypass: 0b01\n"
                         " [Chain](3)     \"Chain name\"\n"
                         "  [Register](4)  \"Reg_1\", length: 9, bypass: 0xAD/b1\n"
                         "  [Register](5)  \"Reg_2\", length: 10, bypass: 0xAD/b10\n"
                         "  [Register](6)  \"Reg_3\", length: 11, bypass: 0xAD/b111\n"
                         " [Linker](2)    \"Linker name\"\n"
                         "  :Selector:(1)  \"Mux_reg\"\n"
                         "  [Register](7)  \"R_A\", length: 19, bypass: 0xEDEF_/b101\n"
                         "  [Register](8)  \"the register\", length: 80, bypass: 0xFACE_DEAD:BEEF_CAFE:DECA"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::Visit_xxx() with a tap
//!
void UT_PrettyPrinter::test_VisitTap ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  string_view noName;
  uint32_t    irBitsCount   = 6u;
  uint32_t    muxPathsCount = 5u;
  auto        tap           = builder.Create_JTAG_TAP(noName, irBitsCount, muxPathsCount);

  PrettyPrinter sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut)); // Will indirectly call Visit for several types of nodes

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Access_I](0)  \"1149_1_TAP\"\n"
                         " [Register](1)  \"TAP_IR\", length: 6, Hold value: true, bypass: 1111_11\n"
                         " [Linker](2)    \"TAP_DR_Mux\"\n"
                         "  :Selector:(1)  \"TAP_IR\"\n"
                         "  [Register](3)  \"TAP_BPY\", length: 1, bypass: 1"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::Visit_xxx() with a tap with added sub-nodes
//!
void UT_PrettyPrinter::test_VisitTap_With_SubNodes ()
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

  PrettyPrinter sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut)); // Will indirectly call Visit for several types of nodes

  // ---------------- Verify
  //
  auto got      = sut.PrettyPrint();
  auto expected = string("[Access_I](0)  \"1149_1_TAP\"\n"
                         " [Register](1)  \"TAP_IR\", length: 6, Hold value: true, bypass: 1111_11\n"
                         " [Linker](2)    \"TAP_DR_Mux\"\n"
                         "  :Selector:(1)  \"TAP_IR\"\n"
                         "  [Register](3)  \"TAP_BPY\", length: 1, bypass: 1\n"
                         "  [Chain](4)     \"Chain name\"\n"
                         "  [Register](5)  \"Reg_1\", length: 6, bypass: 1010_01\n"
                         "  [Register](6)  \"Reg_2\", length: 6, bypass: 1010_10"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::PrettyPrint() with default options
//!
void UT_PrettyPrinter::test_PrettyPrint ()
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
  auto got = PrettyPrinter::PrettyPrint(tap);

  // ---------------- Verify
  //
  auto expected = string("[Access_I](0)  \"1149_1_TAP\"\n"
                         " [Register](1)  \"TAP_IR\", length: 5, Hold value: true, bypass: 1111_1\n"
                         " [Linker](2)    \"TAP_DR_Mux\"\n"
                         "  :Selector:(1)  \"TAP_IR\"\n"
                         "  [Register](3)  \"TAP_BPY\", length: 1, bypass: 1\n"
                         "  [Chain](4)     \"Chain name\"\n"
                         "  [Register](5)  \"Reg_1\", length: 6, bypass: 1010_01\n"
                         "  [Register](6)  \"Reg_2\", length: 6, bypass: 1010_10"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::PrettyPrint() with verbose option
//!
void UT_PrettyPrinter::test_PrettyPrint_Verbose ()
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
  auto got = PrettyPrinter::PrettyPrint(tap, PrettyPrinterOptions::Verbose | PrettyPrinterOptions::ShowNodesIdentifier);

  // ---------------- Verify
  //
  auto expected = string("[Access_I](0)  \"1149_1_TAP\", Protocol: Loopback, pending: false, has_conditioner: false, priority: 0\n"
                         " [Register](1)  \"TAP_IR\", length: 5, Hold value: true, bypass:            1111_1\n"
                         "                                                     , next_to_sut:       1111_1\n"
                         "                                                     , last_to_sut:       1111_1\n"
                         "                                                     , last_from_sut:     1111_1\n"
                         "                                                     , expected_from_sut: 1111_1\n"
                         "                                                     , pending: false, has_conditioner: false, priority: 0\n"
                         " [Linker](2)    \"TAP_DR_Mux\", ignore_in_path: true, pending: false, has_conditioner: false, priority: 0\n"
                         "  :Selector:(1)  \"TAP_IR\"\n"
                         "  [Register](3)  \"TAP_BPY\", length: 1, bypass:            1\n"
                         "                                     , next_to_sut:       1\n"
                         "                                     , last_to_sut:       1\n"
                         "                                     , last_from_sut:     1\n"
                         "                                     , expected_from_sut: 1\n"
                         "                                     , pending: false, has_conditioner: false, priority: 0\n"
                         "  [Chain](4)     \"Chain name\", pending: false, has_conditioner: false, priority: 0\n"
                         "  [Register](5)  \"Reg_1\", length: 6, bypass:            1010_01\n"
                         "                                   , next_to_sut:       1010_01\n"
                         "                                   , last_to_sut:       1010_01\n"
                         "                                   , last_from_sut:     1010_01\n"
                         "                                   , expected_from_sut: 1010_01\n"
                         "                                   , pending: false, has_conditioner: false, priority: 0\n"
                         "  [Register](6)  \"Reg_2\", length: 6, bypass:            1010_10\n"
                         "                                   , next_to_sut:       1010_10\n"
                         "                                   , last_to_sut:       1010_10\n"
                         "                                   , last_from_sut:     1010_10\n"
                         "                                   , expected_from_sut: 1010_10\n"
                         "                                   , pending: false, has_conditioner: false, priority: 0"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::PrettyPrint() with DisplayValueAuto option
//!
void UT_PrettyPrinter::test_PrettyPrint_AutoFormat ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP ("", 5u, 4u);
  auto chain = sm.CreateChain    ("Chain name", tap);
  auto reg_1 = sm.CreateRegister ("Reg_1", BinaryVector::CreateFromBinaryString("1010_0110:110"), tap);
  auto reg_2 = sm.CreateRegister ("Reg_2", BinaryVector::CreateFromBinaryString("1010_10"),       tap);

  // ---------------- Exercise
  //
  auto got = PrettyPrinter::PrettyPrint(tap, PrettyPrinterOptions::DisplayValueAuto | PrettyPrinterOptions::ShowNodesIdentifier);

  // ---------------- Verify
  //
  auto expected = string("[Access_I](0)  \"1149_1_TAP\"\n"
                         " [Register](1)  \"TAP_IR\", length: 5, Hold value: true, bypass: 0b1111_1\n"
                         " [Linker](2)    \"TAP_DR_Mux\"\n"
                         "  :Selector:(1)  \"TAP_IR\"\n"
                         "  [Register](3)  \"TAP_BPY\", length: 1, bypass: 0b1\n"
                         "  [Chain](4)     \"Chain name\"\n"
                         "  [Register](5)  \"Reg_1\", length: 11, bypass: 0xA6/b110\n"
                         "  [Register](6)  \"Reg_2\", length: 6, bypass: 0b1010_10"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::PrettyPrint() with Std option
//!
void UT_PrettyPrinter::test_PrettyPrint_Std ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap   = builder.Create_JTAG_TAP ("", 5u, 4u);
  auto chain = sm.CreateChain    ("Chain name", tap);
  auto reg_1 = sm.CreateRegister ("Reg_1", BinaryVector::CreateFromBinaryString("1010_0110:110"), tap);
  auto reg_2 = sm.CreateRegister ("Reg_2", BinaryVector::CreateFromBinaryString("1010_10"),       tap);

  // ---------------- Exercise
  //
  auto got = PrettyPrinter::PrettyPrint(tap, PrettyPrinterOptions::Std);

  // ---------------- Verify
  //
  auto expected = string("[Access_I](0)  \"1149_1_TAP\", Protocol: Loopback, pending: false, has_conditioner: false, priority: 0\n"
                         " [Register](1)  \"TAP_IR\", length: 5, Hold value: true, bypass:            0b1111_1\n"
                         "                                                     , next_to_sut:       0b1111_1\n"
                         "                                                     , last_to_sut:       0b1111_1\n"
                         "                                                     , last_from_sut:     0b1111_1\n"
                         "                                                     , expected_from_sut: 0b1111_1\n"
                         "                                                     , pending: false, has_conditioner: false, priority: 0\n"
                         " [Linker](2)    \"TAP_DR_Mux\", ignore_in_path: true, pending: false, has_conditioner: false, priority: 0\n"
                         "  :Selector:(1)  \"TAP_IR\"\n"
                         "  [Register](3)  \"TAP_BPY\", length: 1, bypass:            0b1\n"
                         "                                     , next_to_sut:       0b1\n"
                         "                                     , last_to_sut:       0b1\n"
                         "                                     , last_from_sut:     0b1\n"
                         "                                     , expected_from_sut: 0b1\n"
                         "                                     , pending: false, has_conditioner: false, priority: 0\n"
                         "  [Chain](4)     \"Chain name\", pending: false, has_conditioner: false, priority: 0\n"
                         "  [Register](5)  \"Reg_1\", length: 11, bypass:            0xA6/b110\n"
                         "                                    , next_to_sut:       0xA6/b110\n"
                         "                                    , last_to_sut:       0xA6/b110\n"
                         "                                    , last_from_sut:     0xA6/b110\n"
                         "                                    , expected_from_sut: 0xA6/b110\n"
                         "                                    , pending: false, has_conditioner: false, priority: 0\n"
                         "  [Register](6)  \"Reg_2\", length: 6, bypass:            0b1010_10\n"
                         "                                   , next_to_sut:       0b1010_10\n"
                         "                                   , last_to_sut:       0b1010_10\n"
                         "                                   , last_from_sut:     0b1010_10\n"
                         "                                   , expected_from_sut: 0b1010_10\n"
                         "                                   , pending: false, has_conditioner: false, priority: 0"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::PrettyPrint() with "Show Selection Value" option
//!
void UT_PrettyPrinter::test_PrettyPrint_SelectionValue ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap    = builder.Create_JTAG_TAP ("", 5u, 4u);
  auto linker = sm.LinkerWithId(2u);
  auto chain  = sm.CreateChain    ("Chain name", tap);
  auto reg_1  = sm.CreateRegister ("Reg_1", BinaryVector::CreateFromBinaryString("1010_0110:110"), tap);
  auto reg_2  = sm.CreateRegister ("Reg_2", BinaryVector::CreateFromBinaryString("1010_10"),       tap);

  linker->IgnoreForNodePath(true);

  // ---------------- Exercise
  //
  auto got = PrettyPrinter::PrettyPrint(tap, PrettyPrinterOptions::ShowSelectionValue | PrettyPrinterOptions::ShowNodesIdentifier);

  // ---------------- Verify
  //
  auto expected = string(
                         "[Access_I](0)  \"1149_1_TAP\"\n"
                         " [Register](1)  \"TAP_IR\", length: 5, Hold value: true, bypass: 1111_1\n"
                         " [Linker](2)    \"TAP_DR_Mux\"\n"
                         "  :Selector:(1)  \"TAP_IR\"\n"
                         "  [Register](3)  \"TAP_BPY\",     :0b11111:, length: 1, bypass: 1\n"
                         "  [Chain](4)     \"Chain name\",  :0b00001:\n"
                         "  [Register](5)  \"Reg_1\",       :0b00010:, length: 11, bypass: 1010_0110:110\n"
                         "  [Register](6)  \"Reg_2\",       :0b00011:, length: 6, bypass: 1010_10"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::PrettyPrint() with "Show Selection State" option
//!
void UT_PrettyPrinter::test_PrettyPrint_SelectionState ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap    = builder.Create_JTAG_TAP ("", 5u, 4u);
  auto linker = sm.LinkerWithId(2u);
  auto chain  = sm.CreateChain    ("Chain name", tap);
  auto reg_1  = sm.CreateRegister ("Reg_1", BinaryVector::CreateFromBinaryString("1010_0110:110"), tap);
  auto reg_2  = sm.CreateRegister ("Reg_2", BinaryVector::CreateFromBinaryString("1010_10"),       tap);

  linker->IgnoreForNodePath(true);

  // ---------------- Exercise
  //
  auto got = PrettyPrinter::PrettyPrint(tap, PrettyPrinterOptions::ShowSelectionState | PrettyPrinterOptions::ShowNodesIdentifier);

  // ---------------- Verify
  //
  auto expected = string(
                          "[Access_I](0)  \"1149_1_TAP\"\n"
                          " [Register](1)  \"TAP_IR\", length: 5, Hold value: true, bypass: 1111_1\n"
                          " [Linker](2)    \"TAP_DR_Mux\"\n"
                          "  :Selector:(1)  \"TAP_IR\"\n"
                          "  [Register](3)  \"TAP_BPY\",     :S:A:, length: 1, bypass: 1\n"
                          "  [Chain](4)     \"Chain name\"\n"
                          "  [Register](5)  \"Reg_1\", length: 11, bypass: 1010_0110:110\n"
                          "  [Register](6)  \"Reg_2\", length: 6, bypass: 1010_10"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks PrettyPrinter::PrettyPrint() with "Show Selection State and tables" option
//!
void UT_PrettyPrinter::test_PrettyPrint_SelectionTables ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap    = builder.Create_JTAG_TAP ("", 5u, 4u);
  auto linker = sm.LinkerWithId(2u);
  auto chain  = sm.CreateChain    ("Chain name", tap);
  auto reg_1  = sm.CreateRegister ("Reg_1", BinaryVector::CreateFromBinaryString("1010_0110:110"), tap);
  auto reg_2  = sm.CreateRegister ("Reg_2", BinaryVector::CreateFromBinaryString("1010_10"),       tap);

  linker->IgnoreForNodePath(true);

  // ---------------- Exercise
  //
  auto got = PrettyPrinter::PrettyPrint(tap,  PrettyPrinterOptions::ShowSelectionState
                                            | PrettyPrinterOptions::ShowSelectorProperties
                                            | PrettyPrinterOptions::ShowSelectorTables
                                            | PrettyPrinterOptions::ShowNodesIdentifier);

  // ---------------- Verify
  //
  auto expected = string(
                          "[Access_I](0)  \"1149_1_TAP\"\n"
                          " [Register](1)  \"TAP_IR\", length: 5, Hold value: true, bypass: 1111_1\n"
                          " [Linker](2)    \"TAP_DR_Mux\"\n"
                          "  :Selector:(1)  \"TAP_IR\", kind: Table_Based, can_select_none: false, inverted_bits: false, reversed_order: false\n"
                          "  Selection Table:\n"
                          "    [0] 0b1111_1\n"
                          "    [1] 0b1111_1\n"
                          "    [2] 0b0000_1\n"
                          "    [3] 0b0001_0\n"
                          "    [4] 0b0001_1\n"
                          "  Deselection Table:\n"
                          "    [0] 0b1111_1\n"
                          "    [1] 0b1111_1\n"
                          "    [2] 0b1111_1\n"
                          "    [3] 0b1111_1\n"
                          "    [4] 0b1111_1\n"
                          "  [Register](3)  \"TAP_BPY\",     :S:A:, length: 1, bypass: 1\n"
                          "  [Chain](4)     \"Chain name\"\n"
                          "  [Register](5)  \"Reg_1\", length: 11, bypass: 1010_0110:110\n"
                          "  [Register](6)  \"Reg_2\", length: 6, bypass: 1010_10"
                        );
  TS_ASSERT_EQUALS (got, expected);
}



//! Checks PrettyPrinter::PrettyPrint() with all options
//!
void UT_PrettyPrinter::test_PrettyPrint_AllOptions ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  TestModelBuilder builder(sm);

  auto tap    = builder.Create_JTAG_TAP ("", 5u, 4u);
  auto linker = sm.LinkerWithId(2u);
  auto chain  = sm.CreateChain    ("Chain name", tap);
  auto reg_1  = sm.CreateRegister ("Reg_1", BinaryVector::CreateFromBinaryString("1010_0110:110"), tap);
  auto reg_2  = sm.CreateRegister ("Reg_2", BinaryVector::CreateFromBinaryString("1010_10"),       tap);

  linker->IgnoreForNodePath(true);

  // ---------------- Exercise
  //
  auto got = PrettyPrinter::PrettyPrint(tap, PrettyPrinterOptions::All);

  // ---------------- Verify
  //
  auto expected = string(
                         "[Access_I](0)  \"1149_1_TAP\", Protocol: Loopback, pending: false, has_conditioner: false, priority: 0\n"
                         " [Register](1)  \"TAP_IR\", length: 5, Hold value: true, bypass:            0b1111_1\n"
                         "                                                     , next_to_sut:       0b1111_1\n"
                         "                                                     , last_to_sut:       0b1111_1\n"
                         "                                                     , last_from_sut:     0b1111_1\n"
                         "                                                     , expected_from_sut: 0b1111_1\n"
                         "                                                     , pending: false, has_conditioner: false, priority: 0\n"
                         " [Linker](2)    \"TAP_DR_Mux\", ignore_in_path: true, pending: false, has_conditioner: false, priority: 0\n"
                         "  :Selector:(1)  \"TAP_IR\", kind: Table_Based, can_select_none: false, inverted_bits: false, reversed_order: false\n"
                         "  Selection Table:\n"
                         "    [0] 0b1111_1\n"
                         "    [1] 0b1111_1\n"
                         "    [2] 0b0000_1\n"
                         "    [3] 0b0001_0\n"
                         "    [4] 0b0001_1\n"
                         "  Deselection Table:\n"
                         "    [0] 0b1111_1\n"
                         "    [1] 0b1111_1\n"
                         "    [2] 0b1111_1\n"
                         "    [3] 0b1111_1\n"
                         "    [4] 0b1111_1\n"
                         "  [Register](3)  \"TAP_BPY\",     :0b11111:S:A:, length: 1, bypass:            0b1\n"
                         "                                                        , next_to_sut:       0b1\n"
                         "                                                        , last_to_sut:       0b1\n"
                         "                                                        , last_from_sut:     0b1\n"
                         "                                                        , expected_from_sut: 0b1\n"
                         "                                                        , pending: false, has_conditioner: false, priority: 0\n"
                         "  [Chain](4)     \"Chain name\",  :0b00001:, pending: false, has_conditioner: false, priority: 0\n"
                         "  [Register](5)  \"Reg_1\",       :0b00010:, length: 11, bypass:            0xA6/b110\n"
                         "                                                     , next_to_sut:       0xA6/b110\n"
                         "                                                     , last_to_sut:       0xA6/b110\n"
                         "                                                     , last_from_sut:     0xA6/b110\n"
                         "                                                     , expected_from_sut: 0xA6/b110\n"
                         "                                                     , pending: false, has_conditioner: false, priority: 0\n"
                         "  [Register](6)  \"Reg_2\",       :0b00011:, length: 6, bypass:            0b1010_10\n"
                         "                                                    , next_to_sut:       0b1010_10\n"
                         "                                                    , last_to_sut:       0b1010_10\n"
                         "                                                    , last_from_sut:     0b1010_10\n"
                         "                                                    , expected_from_sut: 0b1010_10\n"
                         "                                                    , pending: false, has_conditioner: false, priority: 0"
                        );
  TS_ASSERT_EQUALS (got, expected);
}

//===========================================================================
// End of UT_PrettyPrinter.cpp
//===========================================================================
