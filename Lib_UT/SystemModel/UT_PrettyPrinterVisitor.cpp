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
  auto expected = string("AccessInterface(0, \"Access interface name\")\n");
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
  auto expected = string("Chain(0, \"Chain name\")\n");
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
  auto expected = string("Linker(1, \"Linker name\")\n");
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
  auto expected = string("Register(0, \"A register name\", 1111_1111:0)\n");
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
  auto expected = string("Tap(0, \"Tap name\")\n");
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
  auto expected = string("Chain(0, \"Chain\")\n"
                         "--Register(1, \"Reg_1\", 1010_11)\n");

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
  auto expected = string("Chain(0, \"Chain\")\n"
                         "--Register(1, \"Reg_1\", 1010_01)\n"
                         "--Register(2, \"Reg_2\", 1010_10)\n"
                        );

  TS_ASSERT_EQUALS (got, expected);
}


//===========================================================================
// End of UT_PrettyPrinterVisitor.cpp
//===========================================================================
