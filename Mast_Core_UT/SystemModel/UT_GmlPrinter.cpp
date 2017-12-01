//===========================================================================
//                           UT_GmlPrinter.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file UT_GmlPrinter.cpp
//!
//! Implements test fixture for testing GmlPrinter
//!
//===========================================================================


#include "UT_GmlPrinter.hpp"
#include "GmlPrinter.hpp"
#include "SystemModelNodes.hpp"
#include "DefaultBinaryPathSelector.hpp"
#include "SystemModelBuilder.hpp"
#include "TestModelBuilder.hpp"
#include "EnumsUtility.hpp"

#include <cxxtest/ValueTraits.h>
using std::string;
using std::experimental::string_view;
using std::make_shared;

using namespace mast;
using namespace test;

namespace
{

//! Creates a tap, appending two chains with each two registers
//!
std::shared_ptr<AccessInterface> Create_TestCase_Tap_With_Children (SystemModel& sm)
{
  TestModelBuilder builder(sm);

  string_view noName;
  uint32_t    irBitsCount   = 6u;
  uint32_t    muxPathsCount = 5u;
  auto        tap           = builder.Create_JTAG_TAP(noName, irBitsCount, muxPathsCount);

  auto chain_1 = sm.CreateChain("Chain_1", tap);

  auto reg_1 = sm.CreateRegister("Reg_1", BinaryVector::CreateFromBinaryString("1010_01"), tap);
  auto reg_2 = sm.CreateRegister("Reg_2", BinaryVector::CreateFromBinaryString("1010_10"), tap);

  auto chain_2 = sm.CreateChain("Chain_2", chain_1);

  builder.AppendRegisters(4, "Reg_a_", BinaryVector(5,  0xff), chain_1);
  builder.AppendRegisters(3, "Reg_b_", BinaryVector(15, 0x03), chain_2);

  return tap;
}
//
//  End of: Create_TestCase_Tap_With_Children
//---------------------------------------------------------------------------



} // End of unnamed namespace

//! Initializes tests (called for each test)
//!
void UT_GmlPrinter::setUp ()
{
  CxxTest::setStringResultsOnNewLine(true);
  CxxTest::setCharactersMapping(CxxTest::CharacterMapping::MAP_CHARS_MINIMAL);  // Keep quotes, HT, and new lines unescaped

  SystemModelNode::ResetNodeIdentifier();
}


//! Checks GmlPrinter constructor
//!
void UT_GmlPrinter::test_Constructor ()
{
  // ---------------- Exercise
  //
  GmlPrinter sut;

  // ---------------- Verify
  //
  string graph    = sut.Graph();   // Should return valid empty graph
  string expected = "graph\n"
                    "[\n"
                    "   hierarchic 1 directed 1\n"
                    "]";

  TS_ASSERT_EQUALS (graph, expected);
}



//! Checks GmlPrinter::VisitAccessInterface() when there are several child beneath
//!
void UT_GmlPrinter::test_VisitAccessInterface_with_Child ()
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

  chain->AppendChild(muxNode);
  chain->AppendChild(reg_1);
  chain->AppendChild(reg_2);
  chain->AppendChild(reg_3);

  auto pathSelector    = make_shared<DefaultBinaryPathSelector>(muxNode, 2);
  auto linker          = make_shared<Linker>("Linker name", pathSelector);
  auto accessInterface = make_shared<AccessInterface>("Access interface name", nullptr);

  accessInterface->AppendChild(chain);
  accessInterface->AppendChild(linker);
  linker->AppendChild(reg_a);
  linker->AppendChild(reg_b);

  GmlPrinter sut("", GmlPrinterOptions::ShowSelectorWithEdge);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitAccessInterface(*accessInterface));

  // ---------------- Verify
  //
  auto gotGraph = sut.Graph();
  auto expected = string("graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 8 graphics [ type \"octagon\" fill \"#10FFFF\" w 239 h 43 ] LabelGraphics [ text \"Access interface name\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 0 graphics [ type \"ellipse\" fill \"#FFCC20\" w 135 h 43 ] LabelGraphics [ text \"Chain name\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 6 graphics [ type \"rectangle\" fill \"#59FF20\" w 76 h 35 ] LabelGraphics [ text \"Selector\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"Reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"Reg_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"Reg_3\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 7 graphics [ type \"trapezoid\" fill \"#FF3060\" outlineStyle \"dashed\" w 144 h 43 ] LabelGraphics [ text \"Linker name\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 4 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"R_A\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 114 h 35 ] LabelGraphics [ text \"the register\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   edge [ source 0 target 6 label \"1\" ]\n"
                         "   edge [ source 0 target 1 label \"2\" ]\n"
                         "   edge [ source 0 target 2 label \"3\" ]\n"
                         "   edge [ source 0 target 3 label \"4\" ]\n"
                         "   edge [ source 8 target 0 label \"1\" ]\n"
                         "   edge [ source 7 target 4 label \"1\" ]\n"
                         "   edge [ source 7 target 5 label \"2\" ]\n"
                         "   edge [ source 7 target 6 graphics [ width 1 style \"dashed\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 8 target 7 label \"2\" ]\n"
                         "]"
                        );

  TS_ASSERT_EQUALS (gotGraph, expected);
}

//! Checks GmlPrinter::VisitAccessInterface() when there are several child beneath
//!
void UT_GmlPrinter::test_Visit_After_Graph ()
{
  // ---------------- Setup
  //
  auto chain           = make_shared<Chain>           ("Chain");
  auto reg_1           = make_shared<Register>        ("Reg_1",  BinaryVector::CreateFromBinaryString("1010_1"));
  auto reg_2           = make_shared<Register>        ("Reg_2",  BinaryVector::CreateFromBinaryString("1010_10"));
  auto accessInterface = make_shared<AccessInterface> ("Access", nullptr);

  chain->AppendChild(reg_1);
  chain->AppendChild(reg_2);


  accessInterface->AppendChild(chain);

  GmlPrinter sut;
  sut.Graph();   // Should return valid empty graph

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (sut.VisitAccessInterface(*accessInterface));

  // ---------------- Verify
  //
  auto gotGraph = sut.Graph();
  auto expected = string("graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 3 graphics [ type \"octagon\" fill \"#10FFFF\" w 97 h 43 ] LabelGraphics [ text \"Access\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 0 graphics [ type \"ellipse\" fill \"#FFCC20\" w 90 h 43 ] LabelGraphics [ text \"Chain\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"Reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"Reg_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   edge [ source 0 target 1 label \"1\" ]\n"
                         "   edge [ source 0 target 2 label \"2\" ]\n"
                         "   edge [ source 3 target 0 label \"1\" ]\n"
                         "]"
                        );

  TS_ASSERT_EQUALS (gotGraph, expected);
}


//! Checks GmlPrinter::Visit_xxx() with a tap
//!
void UT_GmlPrinter::test_VisitTap ()
{
  // ---------------- Setup
  //
  SystemModel      sm;
  TestModelBuilder builder(sm);
  string_view      noName;
  uint32_t         irBitsCount   = 6u;
  uint32_t         muxPathsCount = 5u;
  auto             tap           = builder.Create_JTAG_TAP(noName, irBitsCount, muxPathsCount);

  GmlPrinter sut("", GmlPrinterOptions::ShowSelectorWithEdge);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut)); // Will indirectly call Visit for several types of nodes

  // ---------------- Verify
  //
  auto got      = sut.Graph();
  auto expected = string("graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 0 graphics [ type \"octagon\" fill \"#10FFFF\" w 135 h 43 ] LabelGraphics [ text \"1149_1_TAP\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 57 h 35 ] LabelGraphics [ text \"TAP_IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" outlineStyle \"dashed\" w 135 h 43 ] LabelGraphics [ text \"TAP_DR_Mux\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"TAP_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   edge [ source 0 target 1 label \"1\" ]\n"
                         "   edge [ source 2 target 3 label \"1\" ]\n"
                         "   edge [ source 2 target 1 graphics [ width 1 style \"dashed\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 0 target 2 label \"2\" ]\n"
                         "]"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks GmlPrinter::Visit_xxx() with a tap with added sub-nodes
//!
void UT_GmlPrinter::test_VisitTap_With_SubNodes ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto tap = Create_TestCase_Tap_With_Children(sm);

  GmlPrinter sut("", GmlPrinterOptions::ShowSelectorWithEdge);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut)); // Will indirectly call Visit for several types of nodes

  // ---------------- Verify
  //
  auto got      = sut.Graph();
  auto expected = string("graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 0 graphics [ type \"octagon\" fill \"#10FFFF\" w 135 h 43 ] LabelGraphics [ text \"1149_1_TAP\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 57 h 35 ] LabelGraphics [ text \"TAP_IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" outlineStyle \"dashed\" w 135 h 43 ] LabelGraphics [ text \"TAP_DR_Mux\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"TAP_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 4 graphics [ type \"ellipse\" fill \"#FFCC20\" w 106 h 43 ] LabelGraphics [ text \"Chain_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 7 graphics [ type \"ellipse\" fill \"#FFCC20\" w 106 h 43 ] LabelGraphics [ text \"Chain_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 12 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_b_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 13 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_b_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 14 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_b_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 8 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_a_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 9 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_a_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 10 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_a_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 11 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_a_3\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"Reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 6 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"Reg_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   edge [ source 0 target 1 label \"1\" ]\n"
                         "   edge [ source 2 target 3 label \"1\" ]\n"
                         "   edge [ source 7 target 12 label \"1\" ]\n"
                         "   edge [ source 7 target 13 label \"2\" ]\n"
                         "   edge [ source 7 target 14 label \"3\" ]\n"
                         "   edge [ source 4 target 7 label \"1\" ]\n"
                         "   edge [ source 4 target 8 label \"2\" ]\n"
                         "   edge [ source 4 target 9 label \"3\" ]\n"
                         "   edge [ source 4 target 10 label \"4\" ]\n"
                         "   edge [ source 4 target 11 label \"5\" ]\n"
                         "   edge [ source 2 target 4 label \"2\" ]\n"
                         "   edge [ source 2 target 5 label \"3\" ]\n"
                         "   edge [ source 2 target 6 label \"4\" ]\n"
                         "   edge [ source 2 target 1 graphics [ width 1 style \"dashed\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 0 target 2 label \"2\" ]\n"
                         "]"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks GmlPrinter::Visit_xxx() with a tap with added sub-nodes
//!
void UT_GmlPrinter::test_VisitTap_ShowProtocol ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto tap = Create_TestCase_Tap_With_Children(sm);

  GmlPrinter sut("", GmlPrinterOptions::ShowSelectorWithEdge | GmlPrinterOptions::ShowProtocol);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut)); // Will indirectly call Visit for several types of nodes

  // ---------------- Verify
  //
  auto got      = sut.Graph();
  auto expected = string("graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 0 graphics [ type \"octagon\" fill \"#10FFFF\" w 211 h 44 ] LabelGraphics [ text \"1149_1_TAP\nProtocol: Loopback\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 57 h 35 ] LabelGraphics [ text \"TAP_IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" outlineStyle \"dashed\" w 135 h 43 ] LabelGraphics [ text \"TAP_DR_Mux\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"TAP_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 4 graphics [ type \"ellipse\" fill \"#FFCC20\" w 106 h 43 ] LabelGraphics [ text \"Chain_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 7 graphics [ type \"ellipse\" fill \"#FFCC20\" w 106 h 43 ] LabelGraphics [ text \"Chain_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 12 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_b_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 13 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_b_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 14 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_b_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 8 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_a_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 9 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_a_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 10 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_a_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 11 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_a_3\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"Reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 6 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"Reg_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   edge [ source 0 target 1 label \"1\" ]\n"
                         "   edge [ source 2 target 3 label \"1\" ]\n"
                         "   edge [ source 7 target 12 label \"1\" ]\n"
                         "   edge [ source 7 target 13 label \"2\" ]\n"
                         "   edge [ source 7 target 14 label \"3\" ]\n"
                         "   edge [ source 4 target 7 label \"1\" ]\n"
                         "   edge [ source 4 target 8 label \"2\" ]\n"
                         "   edge [ source 4 target 9 label \"3\" ]\n"
                         "   edge [ source 4 target 10 label \"4\" ]\n"
                         "   edge [ source 4 target 11 label \"5\" ]\n"
                         "   edge [ source 2 target 4 label \"2\" ]\n"
                         "   edge [ source 2 target 5 label \"3\" ]\n"
                         "   edge [ source 2 target 6 label \"4\" ]\n"
                         "   edge [ source 2 target 1 graphics [ width 1 style \"dashed\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 0 target 2 label \"2\" ]\n"
                         "]"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks GmlPrinter::Visit_xxx() when does not use edge between Linker and Selector Register
//!
void UT_GmlPrinter::test_Visit_NoEdgeForSelector ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto tap = Create_TestCase_Tap_With_Children(sm);

  GmlPrinter sut;

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut)); // Will indirectly call Visit for several types of nodes

  // ---------------- Verify
  //
  auto got      = sut.Graph();
  auto expected = string("graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 0 graphics [ type \"octagon\" fill \"#10FFFF\" w 135 h 43 ] LabelGraphics [ text \"1149_1_TAP\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 57 h 35 ] LabelGraphics [ text \"TAP_IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" outlineStyle \"dashed\" w 135 h 44 ] LabelGraphics [ text \"TAP_DR_Mux\n:1:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"TAP_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 4 graphics [ type \"ellipse\" fill \"#FFCC20\" w 106 h 43 ] LabelGraphics [ text \"Chain_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 7 graphics [ type \"ellipse\" fill \"#FFCC20\" w 106 h 43 ] LabelGraphics [ text \"Chain_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 12 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_b_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 13 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_b_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 14 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_b_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 8 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_a_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 9 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_a_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 10 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_a_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 11 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_a_3\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"Reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 6 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"Reg_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   edge [ source 0 target 1 label \"1\" ]\n"
                         "   edge [ source 2 target 3 label \"1\" ]\n"
                         "   edge [ source 7 target 12 label \"1\" ]\n"
                         "   edge [ source 7 target 13 label \"2\" ]\n"
                         "   edge [ source 7 target 14 label \"3\" ]\n"
                         "   edge [ source 4 target 7 label \"1\" ]\n"
                         "   edge [ source 4 target 8 label \"2\" ]\n"
                         "   edge [ source 4 target 9 label \"3\" ]\n"
                         "   edge [ source 4 target 10 label \"4\" ]\n"
                         "   edge [ source 4 target 11 label \"5\" ]\n"
                         "   edge [ source 2 target 4 label \"2\" ]\n"
                         "   edge [ source 2 target 5 label \"3\" ]\n"
                         "   edge [ source 2 target 6 label \"4\" ]\n"
                         "   edge [ source 0 target 2 label \"2\" ]\n"
                         "]"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks GmlPrinter::Visit_xxx() when requesting selection values for Linker endpoints
//!
void UT_GmlPrinter::test_Visit_SelectionValues ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto tap = Create_TestCase_Tap_With_Children(sm);

  GmlPrinter sut("", GmlPrinterOptions::ShowSelectionValues);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut)); // Will indirectly call Visit for several types of nodes

  // ---------------- Verify
  //
  auto got      = sut.Graph();
  auto expected = string("graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 0 graphics [ type \"octagon\" fill \"#10FFFF\" w 135 h 43 ] LabelGraphics [ text \"1149_1_TAP\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 57 h 35 ] LabelGraphics [ text \"TAP_IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" outlineStyle \"dashed\" w 135 h 44 ] LabelGraphics [ text \"TAP_DR_Mux\n"
                         ":1:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"TAP_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 4 graphics [ type \"ellipse\" fill \"#FFCC20\" w 106 h 43 ] LabelGraphics [ text \"Chain_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 7 graphics [ type \"ellipse\" fill \"#FFCC20\" w 106 h 43 ] LabelGraphics [ text \"Chain_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 12 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_b_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 13 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_b_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 14 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_b_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 8 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_a_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 9 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_a_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 10 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_a_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 11 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"Reg_a_3\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"Reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 6 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"Reg_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   edge [ source 0 target 1 label \"1\" ]\n"
                         "   edge [ source 2 target 3 label \"1/[0b111111:S:A]\" ]\n"
                         "   edge [ source 7 target 12 label \"1\" ]\n"
                         "   edge [ source 7 target 13 label \"2\" ]\n"
                         "   edge [ source 7 target 14 label \"3\" ]\n"
                         "   edge [ source 4 target 7 label \"1\" ]\n"
                         "   edge [ source 4 target 8 label \"2\" ]\n"
                         "   edge [ source 4 target 9 label \"3\" ]\n"
                         "   edge [ source 4 target 10 label \"4\" ]\n"
                         "   edge [ source 4 target 11 label \"5\" ]\n"
                         "   edge [ source 2 target 4 label \"2/[0b000001]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 2 target 5 label \"3/[0b000010]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 2 target 6 label \"4/[0b000011]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 0 target 2 label \"2\" ]\n"
                         "]"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks GmlPrinter::Visit_xxx() when requesting to display linker selector properties
//!
void UT_GmlPrinter::test_Visit_SelectorProperties ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto tap = Create_TestCase_Tap_With_Children(sm);

  GmlPrinter sut("",   GmlPrinterOptions::DisplayIdentifiers
                     | GmlPrinterOptions::ShowSelectionValues
                     | GmlPrinterOptions::ShowSelectorProperties);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut)); // Will indirectly call Visit for several types of nodes

  // ---------------- Verify
  //
  auto got      = sut.Graph();
  auto expected = string("graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 0 graphics [ type \"octagon\" fill \"#10FFFF\" w 135 h 43 ] LabelGraphics [ text \"(0)\n"
                         "1149_1_TAP\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 57 h 35 ] LabelGraphics [ text \"(1)\n"
                         "TAP_IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" outlineStyle \"dashed\" w 135 h 44 ] LabelGraphics [ text \"(2)\n"
                         "TAP_DR_Mux\n"
                         ":1:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2147483647 graphics [ type \"rectangle\" fill \"#E8E8E8\" w 209 h 90 ] LabelGraphics [ text \"Selector :1:\n"
                         "Kind: Table_Based\n"
                         "Can_select_none: false\n"
                         "Reversed_order:  false\n"
                         "Inverted_bits:   false\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(3)\n"
                         "TAP_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 4 graphics [ type \"ellipse\" fill \"#FFCC20\" w 106 h 43 ] LabelGraphics [ text \"(4)\n"
                         "Chain_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 7 graphics [ type \"ellipse\" fill \"#FFCC20\" w 106 h 43 ] LabelGraphics [ text \"(7)\n"
                         "Chain_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 12 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(12)\n"
                         "Reg_b_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 13 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(13)\n"
                         "Reg_b_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 14 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(14)\n"
                         "Reg_b_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 8 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(8)\n"
                         "Reg_a_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 9 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(9)\n"
                         "Reg_a_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 10 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(10)\n"
                         "Reg_a_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 11 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(11)\n"
                         "Reg_a_3\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(5)\n"
                         "Reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 6 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(6)\n"
                         "Reg_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   edge [ source 0 target 1 label \"1\" ]\n"
                         "   edge [ source 2147483647 target 2 graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 2 target 3 label \"1/[0b111111:S:A]\" ]\n"
                         "   edge [ source 7 target 12 label \"1\" ]\n"
                         "   edge [ source 7 target 13 label \"2\" ]\n"
                         "   edge [ source 7 target 14 label \"3\" ]\n"
                         "   edge [ source 4 target 7 label \"1\" ]\n"
                         "   edge [ source 4 target 8 label \"2\" ]\n"
                         "   edge [ source 4 target 9 label \"3\" ]\n"
                         "   edge [ source 4 target 10 label \"4\" ]\n"
                         "   edge [ source 4 target 11 label \"5\" ]\n"
                         "   edge [ source 2 target 4 label \"2/[0b000001]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 2 target 5 label \"3/[0b000010]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 2 target 6 label \"4/[0b000011]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 0 target 2 label \"2\" ]\n"
                         "]"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks GmlPrinter::Visit_xxx() when requesting selector tables (and properties)
//!
void UT_GmlPrinter::test_Visit_SelectorTables ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto tap = Create_TestCase_Tap_With_Children(sm);

  GmlPrinter sut("",   GmlPrinterOptions::DisplayIdentifiers
                     | GmlPrinterOptions::ShowSelectionValues
                     | GmlPrinterOptions::ShowSelectorTables);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut)); // Will indirectly call Visit for several types of nodes

  // ---------------- Verify
  //
  auto got      = sut.Graph();
  auto expected = string("graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 0 graphics [ type \"octagon\" fill \"#10FFFF\" w 135 h 43 ] LabelGraphics [ text \"(0)\n"
                         "1149_1_TAP\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 57 h 35 ] LabelGraphics [ text \"(1)\n"
                         "TAP_IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" outlineStyle \"dashed\" w 135 h 44 ] LabelGraphics [ text \"(2)\n"
                         "TAP_DR_Mux\n"
                         ":1:\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2147483647 graphics [ type \"rectangle\" fill \"#E8E8E8\" w 209 h 378 ] LabelGraphics [ text \"Selector :1:\n"
                         "Kind: Table_Based\n"
                         "Can_select_none: false\n"
                         "Reversed_order:  false\n"
                         "Inverted_bits:   false\n"
                         "\n"
                         "Selection Table:\n"
                         "  [0] 0b1111_11\n"
                         "  [1] 0b1111_11\n"
                         "  [2] 0b0000_01\n"
                         "  [3] 0b0000_10\n"
                         "  [4] 0b0000_11\n"
                         "  [5] 0b0001_00\n"
                         "\n"
                         "Deselection Table:\n"
                         "  [0] 0b1111_11\n"
                         "  [1] 0b1111_11\n"
                         "  [2] 0b1111_11\n"
                         "  [3] 0b1111_11\n"
                         "  [4] 0b1111_11\n"
                         "  [5] 0b1111_11\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(3)\n"
                         "TAP_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 4 graphics [ type \"ellipse\" fill \"#FFCC20\" w 106 h 43 ] LabelGraphics [ text \"(4)\n"
                         "Chain_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 7 graphics [ type \"ellipse\" fill \"#FFCC20\" w 106 h 43 ] LabelGraphics [ text \"(7)\n"
                         "Chain_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 12 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(12)\n"
                         "Reg_b_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 13 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(13)\n"
                         "Reg_b_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 14 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(14)\n"
                         "Reg_b_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 8 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(8)\n"
                         "Reg_a_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 9 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(9)\n"
                         "Reg_a_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 10 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(10)\n"
                         "Reg_a_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 11 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(11)\n"
                         "Reg_a_3\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(5)\n"
                         "Reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 6 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(6)\n"
                         "Reg_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   edge [ source 0 target 1 label \"1\" ]\n"
                         "   edge [ source 2147483647 target 2 graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 2 target 3 label \"1/[0b111111:S:A]\" ]\n"
                         "   edge [ source 7 target 12 label \"1\" ]\n"
                         "   edge [ source 7 target 13 label \"2\" ]\n"
                         "   edge [ source 7 target 14 label \"3\" ]\n"
                         "   edge [ source 4 target 7 label \"1\" ]\n"
                         "   edge [ source 4 target 8 label \"2\" ]\n"
                         "   edge [ source 4 target 9 label \"3\" ]\n"
                         "   edge [ source 4 target 10 label \"4\" ]\n"
                         "   edge [ source 4 target 11 label \"5\" ]\n"
                         "   edge [ source 2 target 4 label \"2/[0b000001]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 2 target 5 label \"3/[0b000010]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 2 target 6 label \"4/[0b000011]\" graphics [ width 1 style \"dotted\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 0 target 2 label \"2\" ]\n"
                         "]"
                        );
  TS_ASSERT_EQUALS (got, expected);
}



//! Checks GmlPrinter::Visit_xxx() when DisplayIdentifier is true
//!
void UT_GmlPrinter::test_Visit_With_DisplayIdentifier_true ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto tap = Create_TestCase_Tap_With_Children(sm);

  GmlPrinter sut("", GmlPrinterOptions::ShowSelectorWithEdge);
  sut.DisplayIdentifier(true);

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut)); // Will indirectly call Visit for several types of nodes

  // ---------------- Verify
  //
  auto got      = sut.Graph();
  auto expected = string("graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 0 graphics [ type \"octagon\" fill \"#10FFFF\" w 135 h 43 ] LabelGraphics [ text \"(0)\n"
                         "1149_1_TAP\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 57 h 35 ] LabelGraphics [ text \"(1)\n"
                         "TAP_IR\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" outlineStyle \"dashed\" w 135 h 43 ] LabelGraphics [ text \"(2)\n"
                         "TAP_DR_Mux\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(3)\n"
                         "TAP_BPY\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 4 graphics [ type \"ellipse\" fill \"#FFCC20\" w 106 h 43 ] LabelGraphics [ text \"(4)\n"
                         "Chain_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 7 graphics [ type \"ellipse\" fill \"#FFCC20\" w 106 h 43 ] LabelGraphics [ text \"(7)\n"
                         "Chain_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 12 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(12)\n"
                         "Reg_b_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 13 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(13)\n"
                         "Reg_b_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 14 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(14)\n"
                         "Reg_b_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 8 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(8)\n"
                         "Reg_a_0\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 9 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(9)\n"
                         "Reg_a_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 10 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(10)\n"
                         "Reg_a_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 11 graphics [ type \"rectangle\" fill \"#59FF20\" w 66 h 35 ] LabelGraphics [ text \"(11)\n"
                         "Reg_a_3\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(5)\n"
                         "Reg_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 6 graphics [ type \"rectangle\" fill \"#59FF20\" w 50 h 35 ] LabelGraphics [ text \"(6)\n"
                         "Reg_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   edge [ source 0 target 1 label \"1\" ]\n"
                         "   edge [ source 2 target 3 label \"1\" ]\n"
                         "   edge [ source 7 target 12 label \"1\" ]\n"
                         "   edge [ source 7 target 13 label \"2\" ]\n"
                         "   edge [ source 7 target 14 label \"3\" ]\n"
                         "   edge [ source 4 target 7 label \"1\" ]\n"
                         "   edge [ source 4 target 8 label \"2\" ]\n"
                         "   edge [ source 4 target 9 label \"3\" ]\n"
                         "   edge [ source 4 target 10 label \"4\" ]\n"
                         "   edge [ source 4 target 11 label \"5\" ]\n"
                         "   edge [ source 2 target 4 label \"2\" ]\n"
                         "   edge [ source 2 target 5 label \"3\" ]\n"
                         "   edge [ source 2 target 6 label \"4\" ]\n"
                         "   edge [ source 2 target 1 graphics [ width 1 style \"dashed\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 0 target 2 label \"2\" ]\n"
                         "]"
                        );
  TS_ASSERT_EQUALS (got, expected);
}


//! Checks GmlPrinter::Visit_xxx() when DisplayIdentifier and DisplayRegisterValue are true
//!
void UT_GmlPrinter::test_Visit_With_DisplayIdentifierAndRegister_true ()
{
  // ---------------- Setup
  //
  SystemModel sm;
  auto tap = Create_TestCase_Tap_With_Children(sm);

  GmlPrinter sut("",   GmlPrinterOptions::ShowSelectorWithEdge
                            | GmlPrinterOptions::DisplayIdentifiers
                            | GmlPrinterOptions::DisplayRegisterValue
                       );

  // ---------------- Exercise
  //
  TS_ASSERT_THROWS_NOTHING (tap->Accept(sut)); // Will indirectly call Visit for several types of nodes

  // ---------------- Verify
  //
  auto got      = sut.Graph();
  auto expected = string("graph\n"
                         "[\n"
                         "   hierarchic 1 directed 1\n"
                         "   node [ id 0 graphics [ type \"octagon\" fill \"#10FFFF\" w 135 h 43 ] LabelGraphics [ text \"(0)\n"
                         "1149_1_TAP\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 1 graphics [ type \"rectangle\" fill \"#59FF20\" w 171 h 144 ] LabelGraphics [ text \"(1)\n"
                         "TAP_IR\n"
                         "Width: 6\n"
                         "Hold value: true\n"
                         "Bypass:    1111_11\n"
                         "Next to:   1111_11\n"
                         "Last to:   1111_11\n"
                         "Last from: 1111_11\n"
                         "Expected:  1111_11\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 2 graphics [ type \"trapezoid\" fill \"#FF3060\" outlineStyle \"dashed\" w 135 h 43 ] LabelGraphics [ text \"(2)\n"
                         "TAP_DR_Mux\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 3 graphics [ type \"rectangle\" fill \"#59FF20\" w 114 h 126 ] LabelGraphics [ text \"(3)\n"
                         "TAP_BPY\n"
                         "Width: 1\n"
                         "Bypass:    1\n"
                         "Next to:   1\n"
                         "Last to:   1\n"
                         "Last from: 1\n"
                         "Expected:  1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 4 graphics [ type \"ellipse\" fill \"#FFCC20\" w 106 h 43 ] LabelGraphics [ text \"(4)\n"
                         "Chain_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 7 graphics [ type \"ellipse\" fill \"#FFCC20\" w 106 h 43 ] LabelGraphics [ text \"(7)\n"
                         "Chain_2\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 12 graphics [ type \"rectangle\" fill \"#59FF20\" w 275 h 126 ] LabelGraphics [ text \"(12)\n"
                         "Reg_b_0\n"
                         "Width: 15\n"
                         "Bypass:    0000_0011:0000_001\n"
                         "Next to:   0000_0011:0000_001\n"
                         "Last to:   0000_0011:0000_001\n"
                         "Last from: 0000_0011:0000_001\n"
                         "Expected:  0000_0011:0000_001\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 13 graphics [ type \"rectangle\" fill \"#59FF20\" w 275 h 126 ] LabelGraphics [ text \"(13)\n"
                         "Reg_b_1\n"
                         "Width: 15\n"
                         "Bypass:    0000_0011:0000_001\n"
                         "Next to:   0000_0011:0000_001\n"
                         "Last to:   0000_0011:0000_001\n"
                         "Last from: 0000_0011:0000_001\n"
                         "Expected:  0000_0011:0000_001\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 14 graphics [ type \"rectangle\" fill \"#59FF20\" w 275 h 126 ] LabelGraphics [ text \"(14)\n"
                         "Reg_b_2\n"
                         "Width: 15\n"
                         "Bypass:    0000_0011:0000_001\n"
                         "Next to:   0000_0011:0000_001\n"
                         "Last to:   0000_0011:0000_001\n"
                         "Last from: 0000_0011:0000_001\n"
                         "Expected:  0000_0011:0000_001\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 8 graphics [ type \"rectangle\" fill \"#59FF20\" w 161 h 126 ] LabelGraphics [ text \"(8)\n"
                         "Reg_a_0\n"
                         "Width: 5\n"
                         "Bypass:    1111_1\n"
                         "Next to:   1111_1\n"
                         "Last to:   1111_1\n"
                         "Last from: 1111_1\n"
                         "Expected:  1111_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 9 graphics [ type \"rectangle\" fill \"#59FF20\" w 161 h 126 ] LabelGraphics [ text \"(9)\n"
                         "Reg_a_1\n"
                         "Width: 5\n"
                         "Bypass:    1111_1\n"
                         "Next to:   1111_1\n"
                         "Last to:   1111_1\n"
                         "Last from: 1111_1\n"
                         "Expected:  1111_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 10 graphics [ type \"rectangle\" fill \"#59FF20\" w 161 h 126 ] LabelGraphics [ text \"(10)\n"
                         "Reg_a_2\n"
                         "Width: 5\n"
                         "Bypass:    1111_1\n"
                         "Next to:   1111_1\n"
                         "Last to:   1111_1\n"
                         "Last from: 1111_1\n"
                         "Expected:  1111_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 11 graphics [ type \"rectangle\" fill \"#59FF20\" w 161 h 126 ] LabelGraphics [ text \"(11)\n"
                         "Reg_a_3\n"
                         "Width: 5\n"
                         "Bypass:    1111_1\n"
                         "Next to:   1111_1\n"
                         "Last to:   1111_1\n"
                         "Last from: 1111_1\n"
                         "Expected:  1111_1\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 5 graphics [ type \"rectangle\" fill \"#59FF20\" w 171 h 126 ] LabelGraphics [ text \"(5)\n"
                         "Reg_1\n"
                         "Width: 6\n"
                         "Bypass:    1010_01\n"
                         "Next to:   1010_01\n"
                         "Last to:   1010_01\n"
                         "Last from: 1010_01\n"
                         "Expected:  1010_01\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   node [ id 6 graphics [ type \"rectangle\" fill \"#59FF20\" w 171 h 126 ] LabelGraphics [ text \"(6)\n"
                         "Reg_2\n"
                         "Width: 6\n"
                         "Bypass:    1010_10\n"
                         "Next to:   1010_10\n"
                         "Last to:   1010_10\n"
                         "Last from: 1010_10\n"
                         "Expected:  1010_10\" fontSize 13 fontStyle \"bold\" fontName \"Lucida Console\"] ]\n"
                         "   edge [ source 0 target 1 label \"1\" ]\n"
                         "   edge [ source 2 target 3 label \"1\" ]\n"
                         "   edge [ source 7 target 12 label \"1\" ]\n"
                         "   edge [ source 7 target 13 label \"2\" ]\n"
                         "   edge [ source 7 target 14 label \"3\" ]\n"
                         "   edge [ source 4 target 7 label \"1\" ]\n"
                         "   edge [ source 4 target 8 label \"2\" ]\n"
                         "   edge [ source 4 target 9 label \"3\" ]\n"
                         "   edge [ source 4 target 10 label \"4\" ]\n"
                         "   edge [ source 4 target 11 label \"5\" ]\n"
                         "   edge [ source 2 target 4 label \"2\" ]\n"
                         "   edge [ source 2 target 5 label \"3\" ]\n"
                         "   edge [ source 2 target 6 label \"4\" ]\n"
                         "   edge [ source 2 target 1 graphics [ width 1 style \"dashed\" targetArrow \"standard\" ] ]\n"
                         "   edge [ source 0 target 2 label \"2\" ]\n"
                         "]"
                        );
  TS_ASSERT_EQUALS (got, expected);
}

//===========================================================================
// End of UT_GmlPrinter.cpp
//===========================================================================
